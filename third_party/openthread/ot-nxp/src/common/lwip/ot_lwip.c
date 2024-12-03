/*
 *  Copyright (c) 2023, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "ot_lwip.h"
#include "token_bucket.h"

#include <string.h>

#include <common/code_utils.hpp>
#include <openthread-core-config.h>
#include <openthread/icmp6.h>
#include <openthread/instance.h>
#include <openthread/ip6.h>
#include <openthread/message.h>
#include <openthread/nat64.h>
#include <openthread/thread.h>

#include "lwip_tcpip_init_once.h"
#include "lwip/tcpip.h"

/* -------------------------------------------------------------------------- */
/*                                 Definitions                                */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static struct netif     sThreadNetIf;
static otInstance      *sInstance = NULL;
static bool             sAddrAssigned[LWIP_IPV6_NUM_ADDRESSES];
static otPlatLockTaskCb sLockTaskCb          = NULL;
static bool             sLwipUninitialized   = true;
static bool             sThreadIfaceNotAdded = true;
#if defined(OT_APP_THREAD_RATE_LIMIT) && (OT_APP_THREAD_RATE_LIMIT >= 8)
static otTokenBucket sTokenBucket;
#endif
/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static err_t   otPlatLwipThreadNetIfInitCallback(struct netif *netif);
static err_t   otPlatLwipSendPacket(struct netif *netif, struct pbuf *pkt, const struct ip6_addr *ipaddr);
static err_t   otPlatLwipSendIp4Packet(struct netif *netif, struct pbuf *pkt, const struct ip4_addr *ipaddr);
static void    otPlatLwipReceivePacket(otMessage *pkt, void *context);
static otError otPlatLwipCopyToOtMsg(struct pbuf *lwipIpPkt, otMessage *otIpPkt);

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void otPlatLwipInit(otInstance *aInstance, otPlatLockTaskCb lockTaskCb)
{
    VerifyOrExit(sLwipUninitialized);
    VerifyOrExit(aInstance != NULL);
    VerifyOrExit(lockTaskCb != NULL);

    sInstance   = aInstance;
    sLockTaskCb = lockTaskCb;

#ifndef DISABLE_TCPIP_INIT
    /* Initialize LWIP stack */
    tcpip_init_once(NULL, NULL);
#endif

    memset(sAddrAssigned, 0, sizeof(sAddrAssigned));
    sLwipUninitialized = false;
exit:
    return;
}

void otPlatLwipAddThreadInterface(void)
{
    struct netif *pNetIf;

    VerifyOrExit(sThreadIfaceNotAdded);

    /* otPlatLwipInit must be called first */
    VerifyOrExit(sInstance != NULL);

    /* Lock LwIP stack */
    LOCK_TCPIP_CORE();

    /* Initialize a LwIP netif structure for the OpenThread interface and add it to the list of interfaces known to
     * LwIP. */
    pNetIf = netif_add(&sThreadNetIf, NULL, NULL, NULL, NULL, otPlatLwipThreadNetIfInitCallback, tcpip_input);

    /* Start with the interface in the down state. */
    netif_set_link_down(&sThreadNetIf);

#if defined(OT_APP_THREAD_RATE_LIMIT) && (OT_APP_THREAD_RATE_LIMIT >= 8)
    /* Initialize rate limiter. This is done while lwIP lock is held to prevent it being called before initialization is
     * complete. */
    otTokenBucketInit(&sTokenBucket,
                      OT_APP_THREAD_RATE_LIMIT / 8U); /* Convert from bits per second to bytes per second. */
#endif
    /* Unlock LwIP stack */
    UNLOCK_TCPIP_CORE();

    VerifyOrExit(pNetIf != NULL);

    /* Arrange for OpenThread to call our otPlatLwipReceivePacket() method whenever an IPv6 packet is received. */
    otIp6SetReceiveCallback(sInstance, otPlatLwipReceivePacket, NULL);
#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE
    /* We can use the same function for IPv6 and translated IPv4 messages. */
    otNat64SetReceiveIp4Callback(sInstance, otPlatLwipReceivePacket, NULL);
#endif /* OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE */
    /* ICMPv6 Echo processing enabled for unicast and multicast requests */
    otIcmp6SetEchoMode(sInstance, OT_ICMP6_ECHO_HANDLER_ALL);
    /* Enable the receive filter for Thread control traffic. */
    otIp6SetReceiveFilterEnabled(sInstance, true);

    sThreadIfaceNotAdded = false;
exit:
    return;
}

void otPlatLwipUpdateState(otChangedFlags flags, void *context)
{
    // If the Thread device role has changed, or if an IPv6 address has been added or
    // removed in the Thread stack, update the state and configuration of the LwIP Thread interface.
    if (flags & (OT_CHANGED_THREAD_ROLE | OT_CHANGED_IP6_ADDRESS_ADDED | OT_CHANGED_IP6_ADDRESS_REMOVED))
    {
        err_t lwipErr = ERR_OK;
        bool  isInterfaceUp;
        bool  addrChange = flags & (OT_CHANGED_IP6_ADDRESS_ADDED | OT_CHANGED_IP6_ADDRESS_REMOVED) ? true : false;
        bool  addrAssigned[LWIP_IPV6_NUM_ADDRESSES];

        memset(addrAssigned, 0, sizeof(addrAssigned));

        // Lock LwIP stack.
        LOCK_TCPIP_CORE();

        // Determine whether the device Thread interface is up..
        isInterfaceUp = otIp6IsEnabled(sInstance);

        // If needed, adjust the link state of the LwIP netif to reflect the state of the OpenThread stack.
        // Set ifConnectivity to indicate the change in the link state.
        if (isInterfaceUp != (bool)netif_is_link_up(&sThreadNetIf))
        {
            if (isInterfaceUp)
            {
                netif_set_link_up(&sThreadNetIf);
            }
            else
            {
                netif_set_link_down(&sThreadNetIf);
            }

            // Presume the interface addresses are also changing.
            addrChange = true;
        }

        // If needed, adjust the set of addresses associated with the LwIP netif to reflect those
        // known to the Thread stack.
        if (addrChange)
        {
            // If attached to a Thread network, add addresses to the LwIP netif to match those
            // configured in the Thread stack...
            if (isInterfaceUp)
            {
                // Enumerate the list of unicast IPv6 addresses known to OpenThread...
                const otNetifAddress *otAddrs = otIp6GetUnicastAddresses(sInstance);
                for (const otNetifAddress *otAddr = otAddrs; otAddr != NULL; otAddr = otAddr->mNext)
                {
                    // Assign the following OpenThread addresses to LwIP's address table:
                    //   - link-local addresses.
                    //   - mesh-local addresses that are NOT RLOC addresses.
                    //   - global unicast addresses.
                    if (otAddr->mValid && !otAddr->mRloc)
                    {
                        ip6_addr_t lwipAddr = {0};
                        memcpy(&lwipAddr.addr, otAddr->mAddress.mFields.m8, sizeof(lwipAddr.addr));
                        int8_t addrIdx;

                        // Add the address to the LwIP netif.  If the address is a link-local, and the primary
                        // link-local address* for the LwIP netif has not been set already, then use
                        // netif_ip6_addr_set() to set the primary address.  Otherwise use netif_add_ip6_address(). This
                        // special case is required because LwIP's netif_add_ip6_address() will never set the primary
                        // link-local address.
                        //
                        // * -- The primary link-local address always appears in the first slot in the netif address
                        // table.
                        //
                        if (ip6_addr_islinklocal(&lwipAddr) && !addrAssigned[0])
                        {
                            netif_ip6_addr_set(&sThreadNetIf, 0, &lwipAddr);
                            addrIdx = 0;
                        }
                        else
                        {
                            // Add the address to the LwIP netif.  If the address table fills (ERR_VAL), simply stop
                            // adding addresses.  If something else fails, log it and soldier on.
                            lwipErr = netif_add_ip6_address(&sThreadNetIf, &lwipAddr, &addrIdx);
                            if (lwipErr == ERR_VAL)
                            {
                                break;
                            }
                        }
                        // Set non-mesh-local address state to PREFERRED or ACTIVE depending on the state in OpenThread.
                        netif_ip6_addr_set_state(
                            &sThreadNetIf, addrIdx,
                            (otAddr->mPreferred && otAddr->mAddressOrigin != OT_ADDRESS_ORIGIN_THREAD)
                                ? IP6_ADDR_PREFERRED
                                : IP6_ADDR_VALID);

                        // Record that the netif address slot was assigned during this loop.
                        addrAssigned[addrIdx] = true;
                    }
                }
            }
            // For each address associated with the netif that was *not* assigned above, remove the address
            // from the netif if the address is one that was previously assigned by this method.
            // In the case where the device is no longer attached to a Thread network, remove all addresses
            // from the netif.
            for (u8_t addrIdx = 0; addrIdx < LWIP_IPV6_NUM_ADDRESSES; addrIdx++)
            {
                if (!isInterfaceUp || (sAddrAssigned[addrIdx] && !addrAssigned[addrIdx]))
                {
                    // Remove the address from the netif by setting its state to INVALID
                    netif_ip6_addr_set_state(&sThreadNetIf, addrIdx, IP6_ADDR_INVALID);
                }
            }

            // Remember the set of assigned addresses.
            memcpy(sAddrAssigned, addrAssigned, sizeof(sAddrAssigned));
        }

        UNLOCK_TCPIP_CORE();
    }
}

struct pbuf *otPlatLwipConvertToLwipMsg(otMessage *otIpPkt, bool bTransport)
{
    struct pbuf *lwipIpPkt    = NULL;
    bool         bFreeLwipPkt = false;
    uint16_t     lwipIpPktLen = otMessageGetLength(otIpPkt);

    // Allocate an LwIP pbuf to hold the inbound packet.
    if (bTransport)
    {
        lwipIpPkt = pbuf_alloc(PBUF_TRANSPORT, lwipIpPktLen, PBUF_RAM);
    }
    else
    {
        lwipIpPkt = pbuf_alloc(PBUF_LINK, lwipIpPktLen, PBUF_POOL);
    }

    VerifyOrExit(lwipIpPkt != NULL);

    // Copy the packet data from the OpenThread message object to the pbuf.
    if (otMessageRead(otIpPkt, 0, lwipIpPkt->payload, lwipIpPktLen) != lwipIpPktLen)
    {
        ExitNow(bFreeLwipPkt = true);
    }

exit:
    if (bFreeLwipPkt)
    {
        pbuf_free(lwipIpPkt);
        lwipIpPkt = NULL;
    }
    return lwipIpPkt;
}

otMessage *otPlatLwipConvertToOtMsg(struct pbuf *lwipIpPkt)
{
    otMessage              *otIpPkt     = NULL;
    const otMessageSettings msgSettings = {true, OT_MESSAGE_PRIORITY_NORMAL};
    bool                    bFreeOtPkt  = false;

    // Allocate an OpenThread message
    otIpPkt = otIp6NewMessage(sInstance, &msgSettings);
    VerifyOrExit(otIpPkt != NULL);

    // Copy data into the OpenThread message
    VerifyOrExit(otPlatLwipCopyToOtMsg(lwipIpPkt, otIpPkt) == OT_ERROR_NONE, bFreeOtPkt = true);

exit:
    if (bFreeOtPkt)
    {
        otMessageFree(otIpPkt);
        otIpPkt = NULL;
    }
    return otIpPkt;
}

struct netif *otPlatLwipGetOtNetif(void)
{
    return &sThreadNetIf;
}

#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE
otError otPlatLwipNat64Send(struct pbuf *lwipIpv4Pkt)
{
    otMessageSettings settings;
    otMessage        *message = NULL;
    otNat64State      state;
    otError           error;
    otError           result = OT_ERROR_NONE;

    /* Check if NAT64 translator is running to avoid copying the packet */
    state = otNat64GetTranslatorState(sInstance);
    VerifyOrExit(state == OT_NAT64_STATE_ACTIVE, result = OT_ERROR_INVALID_STATE);

#if defined(OT_APP_THREAD_RATE_LIMIT) && (OT_APP_THREAD_RATE_LIMIT >= 8)
#define IPV6_HEADER_OVERHEAD                                                                                     \
    (20U) /* Expected length increase after translating from IPv4 to IPv6 (IPv6 header size (40) - smallest IPv4 \
             header size (20)) */
    /* Check we haven't exceeded rate limit so far */
    VerifyOrExit(otTokenBucketCanTake(&sTokenBucket, lwipIpv4Pkt->tot_len + IPV6_HEADER_OVERHEAD),
                 result = OT_ERROR_BUSY);
#endif

    /* Create OT message to copy lwIP packet into */
    settings.mLinkSecurityEnabled = (otThreadGetDeviceRole(sInstance) != OT_DEVICE_ROLE_DISABLED);
    settings.mPriority            = OT_MESSAGE_PRIORITY_LOW;
    message                       = otIp4NewMessage(sInstance, &settings);
    VerifyOrExit(message != NULL, result = OT_ERROR_NO_BUFS);

    /* Copy lwIP packet into OT message */
    error = otPlatLwipCopyToOtMsg(lwipIpv4Pkt, message);
    VerifyOrExit(error == OT_ERROR_NONE, result = error);

    /* Time to Live in IPv4 packet is not adjusted intentionally.
     * If it will be routed further to Thread network, router should do this. */

    /* Try to forward packet to translator if it will consume it or not */
    error   = otNat64Send(sInstance, message);
    message = NULL; /* otNat64Send took ownership of message regardless if it has been sent or not */
    VerifyOrExit(error == OT_ERROR_NONE, result = error);
#if defined(OT_APP_THREAD_RATE_LIMIT) && (OT_APP_THREAD_RATE_LIMIT >= 8)
    /* Packet really consumed by translator, count it to the rate limit now.
     * If tokenBucket has been depleted after the call to otTokenBucketCanTake
     * from a concurrent task, we may have exceeded a rate limit a little. */
    (void)otTokenBucketTake(&sTokenBucket, lwipIpv4Pkt->tot_len + IPV6_HEADER_OVERHEAD);
#endif

exit:
    if (message != NULL)
    {
        otMessageFree(message);
    }

    return result;
}
#endif /* OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE */

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static err_t otPlatLwipThreadNetIfInitCallback(struct netif *netif)
{
    netif->name[0]    = 'o';
    netif->name[1]    = 't';
    netif->output_ip6 = otPlatLwipSendPacket;
    netif->output     = otPlatLwipSendIp4Packet;
    netif->linkoutput = NULL;
    netif->flags      = NETIF_FLAG_UP | NETIF_FLAG_LINK_UP | NETIF_FLAG_BROADCAST;
    netif->mtu        = OPENTHREAD_CONFIG_IP6_MAX_DATAGRAM_LENGTH;
    return ERR_OK;
}

static err_t otPlatLwipSendPacket(struct netif *netif, struct pbuf *pkt, const struct ip6_addr *ipaddr)
{
    err_t lwipErr = ERR_IF;

    /* Temporarily release the TCP/IP mutex, take OT mutex, and take back TCP/IP mutex to preserve the
     * mutex take order as to avoid deadlock. */
    UNLOCK_TCPIP_CORE();

    // Lock OT
    sLockTaskCb(true);

    // Take back TCP/IP mutex which was temporarily released above
    LOCK_TCPIP_CORE();

#if defined(OT_APP_THREAD_RATE_LIMIT) && (OT_APP_THREAD_RATE_LIMIT >= 8)
    /* TODO: Instead of using lwIP packet length for rate limiting, calculate how many bytes
     * would be output to Thread network with its fragmentation and different header size. */
    if (otTokenBucketTake(&sTokenBucket, pkt->tot_len) != pkt->tot_len)
    {
        /* Sending the packet would exceed the rate limit. */
        lwipErr = ERR_IF;
    }
    else
    {
#endif
        otMessage *otIpPkt = otPlatLwipConvertToOtMsg(pkt);

        if (otIpPkt != NULL)
        {
            /* Pass the packet to OpenThread to be sent.  Note that OpenThread takes care of releasing the otMessage
             * object regardless of whether otIp6Send() succeeds or fails. */
            if (otIp6Send(sInstance, otIpPkt) == OT_ERROR_NONE)
            {
                lwipErr = ERR_OK;
            }
        }
#if defined(OT_APP_THREAD_RATE_LIMIT) && (OT_APP_THREAD_RATE_LIMIT >= 8)
    }
#endif

    // Unlock OT
    sLockTaskCb(false);

    /* pkt is freed by LWIP stack */
    return lwipErr;
}

static err_t otPlatLwipSendIp4Packet(struct netif *netif, struct pbuf *pkt, const struct ip4_addr *ipaddr)
{
    err_t lwipErr = ERR_IF;

    // Lock OT
    sLockTaskCb(true);

    (void)netif;
    (void)ipaddr;

    /* If IPv4 packet is to be output on Thread interface, it can only be translated to IPv6 via NAT64,
     * otherwise it is an error */
#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE
    switch (otPlatLwipNat64Send(pkt))
    {
    case OT_ERROR_NONE:
        lwipErr = ERR_OK;
        break;
    case OT_ERROR_NO_BUFS:
        lwipErr = ERR_BUF;
        break;
    case OT_ERROR_DROP:
    case OT_ERROR_NO_ROUTE:
        lwipErr = ERR_RTE;
        break;
    case OT_ERROR_BUSY:
        lwipErr = ERR_WOULDBLOCK;
        break;
    default:
        lwipErr = ERR_IF;
        break;
    }
#endif /* OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE */

    // Unlock OT
    sLockTaskCb(false);

    /* pkt is freed by LWIP stack */
    return lwipErr;
}

static void otPlatLwipReceivePacket(otMessage *pkt, void *context)
{
    struct pbuf *lwipIpPkt = otPlatLwipConvertToLwipMsg(pkt, false);

    if (lwipIpPkt != NULL)
    {
        /* Deliver the packet to the input function associated with the LwIP netif.
         * NOTE: The input function posts the inbound packet to LwIP's TCPIP thread.
         * Thus there's no need to acquire the LwIP TCPIP core lock here. */
        if (sThreadNetIf.input(lwipIpPkt, &sThreadNetIf) != ERR_OK)
        {
            pbuf_free(lwipIpPkt);
        }
    }

    /* Always free the OpenThread message. */
    otMessageFree(pkt);
}

static otError otPlatLwipCopyToOtMsg(struct pbuf *lwipIpPkt, otMessage *otIpPkt)
{
    uint16_t remainingLen;
    otError  error = OT_ERROR_FAILED;

    // Copy data from LwIP's packet buffer chain into the OpenThread message.
    remainingLen = lwipIpPkt->tot_len;
    for (struct pbuf *partialPkt = lwipIpPkt; (partialPkt != NULL) && (remainingLen > 0); partialPkt = partialPkt->next)
    {
        VerifyOrExit(partialPkt->len <= remainingLen);

        VerifyOrExit(otMessageAppend(otIpPkt, partialPkt->payload, partialPkt->len) == OT_ERROR_NONE);
        remainingLen = (uint16_t)(remainingLen - partialPkt->len);
    }
    VerifyOrExit(remainingLen == 0);
    error = OT_ERROR_NONE;

exit:
    return error;
}
