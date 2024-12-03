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

/**
 * @file
 *   This file implements the OpenThread platform abstraction for adjacent infrastructure interface.
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "infra_if.h"
#include "assert.h"
#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE
#include "ot_lwip.h"
#endif
#include "fsl_common.h"
#include "ot_platform_common.h"
#include <openthread/cli.h>
#include <openthread/ip6.h>
#include <openthread/nat64.h>
#include <openthread/platform/infra_if.h>
#include <openthread/platform/memory.h>
#include "common/code_utils.hpp"
#include "lwip/api.h"
#include "lwip/dns.h"
#include "lwip/icmp6.h"
#include "lwip/inet.h"
#include "lwip/ip.h"
#include "lwip/mld6.h"
#include "lwip/nd6.h"
#include "lwip/prot/nd6.h"
#include "lwip/raw.h"
#include "lwip/sockets.h"
#include "lwip/tcpip.h"

/* -------------------------------------------------------------------------- */
/*                                 Definitions                                */
/* -------------------------------------------------------------------------- */

#define ICMP_RA_MINIMUM_SIZE 16

struct ndSendContext
{
    ip_addr_t    dstIp;
    ip_addr_t    srcIp;
    struct pbuf *pktBuffer;
    uint32_t     infraIfIndex;
};

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static uint8_t         sInfraIfIndex = 0;
static otInstance     *sInstance     = NULL;
static struct netif   *sNetifPtr;
static struct raw_pcb *sIcmp6RawPcb;
#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE
static struct raw_pcb *sIcmpRawPcb;
static struct raw_pcb *sUdpRawPcb;
static struct raw_pcb *sTcpRawPcb;
#endif /* OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE */
#if OPENTHREAD_CONFIG_BORDER_ROUTING_ENABLE
static const char         sWellKnownIpv4OnlyName[]   = "ipv4only.arpa";
static const otIp4Address sWellKnownIpv4OnlyAddress1 = {{{192, 0, 0, 170}}};
static const otIp4Address sWellKnownIpv4OnlyAddress2 = {{{192, 0, 0, 171}}};
static const uint8_t      sValidNat64PrefixLength[]  = {96, 64, 56, 48, 40, 32};
#endif /* OPENTHREAD_CONFIG_BORDER_ROUTING_ENABLE */

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */
static void    LwipTaskCb(void *context);
static bool    GetAddrFromRa(const uint8_t *aBuffer,
                             uint16_t       aBufferLength,
                             ip6_addr_t    *addr,
                             uint32_t      *valid_t,
                             uint32_t      *pref_t);
static void    SetOrUpdateAddrFromRa(struct netif *netif, ip6_addr_t *addr, uint32_t valid_t, uint32_t pref_t);
static void    RaFromOtToLwip(uint32_t aInfraIfIndex, const uint8_t *aBuffer, uint16_t aBufferLength);
static uint8_t ReceiveIcmp6Message(void *arg, struct raw_pcb *pcb, struct pbuf *p, const ip_addr_t *addr);

#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE
static uint8_t ReceiveIPV4Message(void *arg, struct raw_pcb *pcb, struct pbuf *p, const ip_addr_t *addr);
#endif /* OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE */
#if OPENTHREAD_CONFIG_BORDER_ROUTING_ENABLE
static void DnsNat64Callback(const char *name, const ip_addr_t *ipaddr, void *arg);
#endif /* OPENTHREAD_CONFIG_BORDER_ROUTING_ENABLE */

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void InfraIfInit(otInstance *aInstance, struct netif *netif)
{
    ip6_addr_t ip6_allrouters_ll;
    ip6_addr_set_allrouters_linklocal(&ip6_allrouters_ll);

    assert(netif != NULL);

    sInstance = aInstance;
    sNetifPtr = netif;

    sInfraIfIndex = netif_get_index(sNetifPtr);

    LOCK_TCPIP_CORE();

    sIcmp6RawPcb = raw_new_ip_type(IPADDR_TYPE_V6, IP6_NEXTH_ICMP6);
    /* Check config. There might now be enough raw PCBs configured */
    assert(sIcmp6RawPcb != NULL);

    raw_bind_netif(sIcmp6RawPcb, netif);
    raw_recv(sIcmp6RawPcb, ReceiveIcmp6Message, NULL);

    /* Enable checksum computation for TX and set checksum offset in ICMP packet since we are using RAW send */
    sIcmp6RawPcb->chksum_reqd   = 1;
    sIcmp6RawPcb->chksum_offset = 2;

#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE
    sIcmpRawPcb = raw_new_ip_type(IPADDR_TYPE_V4, IPPROTO_ICMP);
    assert(sIcmpRawPcb != NULL);

    raw_bind_netif(sIcmpRawPcb, netif);
    raw_recv(sIcmpRawPcb, ReceiveIPV4Message, NULL);

    sUdpRawPcb = raw_new_ip_type(IPADDR_TYPE_V4, IPPROTO_UDP);
    assert(sUdpRawPcb != NULL);

    raw_bind_netif(sUdpRawPcb, netif);
    raw_recv(sUdpRawPcb, ReceiveIPV4Message, NULL);

    sTcpRawPcb = raw_new_ip_type(IPADDR_TYPE_V4, IPPROTO_TCP);
    assert(sTcpRawPcb != NULL);

    raw_bind_netif(sTcpRawPcb, netif);
    raw_recv(sTcpRawPcb, ReceiveIPV4Message, NULL);
#endif /* OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE */

    // Register to all routers multicast address to recive RS messages
    assert(ERR_OK == mld6_joingroup_netif(netif, &ip6_allrouters_ll));

    UNLOCK_TCPIP_CORE();
}

void InfraIfDeInit()
{
    sInstance     = NULL;
    sNetifPtr     = NULL;
    sInfraIfIndex = 0;

    if (sIcmp6RawPcb != NULL)
    {
        raw_remove(sIcmp6RawPcb);
        sIcmp6RawPcb = NULL;
    }
#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE
    if (sIcmpRawPcb != NULL)
    {
        raw_remove(sIcmpRawPcb);
        sIcmpRawPcb = NULL;
    }
    if (sUdpRawPcb != NULL)
    {
        raw_remove(sUdpRawPcb);
        sUdpRawPcb = NULL;
    }
    if (sTcpRawPcb != NULL)
    {
        raw_remove(sTcpRawPcb);
        sTcpRawPcb = NULL;
    }
#endif /* OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE */
}

void InfraIfLinkState(bool bUp)
{
    if ((sInstance != NULL) && (sInfraIfIndex != 0))
    {
        otPlatInfraIfStateChanged(sInstance, sInfraIfIndex, bUp);
    }
}

otError otPlatInfraIfSendIcmp6Nd(uint32_t            aInfraIfIndex,
                                 const otIp6Address *aDestAddress,
                                 const uint8_t      *aBuffer,
                                 uint16_t            aBufferLength)
{
    otError           retError = OT_ERROR_NONE;
    const ip6_addr_t *srcIpPtr = NULL;

    struct ndSendContext *ndSendContexPtr = (struct ndSendContext *)otPlatCAlloc(1, sizeof(struct ndSendContext));
    VerifyOrExit(NULL != ndSendContexPtr, retError = OT_ERROR_FAILED);

    memcpy(ip_2_ip6(&ndSendContexPtr->dstIp)->addr, aDestAddress->mFields.m8,
           sizeof(ip_2_ip6(&ndSendContexPtr->dstIp)->addr));
    ndSendContexPtr->dstIp.type            = IPADDR_TYPE_V6;
    ndSendContexPtr->dstIp.u_addr.ip6.zone = IP6_NO_ZONE;

    srcIpPtr = netif_ip6_addr(sNetifPtr, 0);
    memcpy(ip_2_ip6(&ndSendContexPtr->srcIp)->addr, srcIpPtr->addr, sizeof(ip_2_ip6(&ndSendContexPtr->srcIp)->addr));
    ndSendContexPtr->srcIp.type            = IPADDR_TYPE_V6;
    ndSendContexPtr->srcIp.u_addr.ip6.zone = IP6_NO_ZONE;

    ndSendContexPtr->pktBuffer = pbuf_alloc(PBUF_TRANSPORT, aBufferLength, PBUF_RAM);
    VerifyOrExit(ndSendContexPtr->pktBuffer != NULL, retError = OT_ERROR_NO_BUFS);
    memcpy(ndSendContexPtr->pktBuffer->payload, aBuffer, aBufferLength);

    ndSendContexPtr->infraIfIndex = aInfraIfIndex;

    if (ERR_OK != tcpip_callback(LwipTaskCb, (void *)ndSendContexPtr))
    {
        pbuf_free(ndSendContexPtr->pktBuffer);
        otPlatFree(ndSendContexPtr);
        retError = OT_ERROR_FAILED;
    }

exit:
    return retError;
}

bool otPlatInfraIfHasAddress(uint32_t aInfraIfIndex, const otIp6Address *aAddress)
{
    ip_addr_t searchedAddress = IPADDR6_INIT(0, 0, 0, 0);
    memcpy(ip_2_ip6(&searchedAddress), &aAddress->mFields.m32, sizeof(aAddress->mFields.m32));

    return (netif_get_ip6_addr_match(sNetifPtr, (const ip6_addr_t *)&searchedAddress) > 0 ? true : false);
}

#if OPENTHREAD_CONFIG_BORDER_ROUTING_ENABLE
otError otPlatInfraIfDiscoverNat64Prefix(uint32_t aInfraIfIndex)
{
    otError    res  = OT_ERROR_FAILED;
    ip_addr_t *addr = NULL;
    err_t      error;

    VerifyOrExit(aInfraIfIndex == (uint32_t)sInfraIfIndex);

    addr = (ip_addr_t *)otPlatCAlloc(1, sizeof(ip_addr_t));
    VerifyOrExit(addr != NULL);

    LOCK_TCPIP_CORE();
    /* Note it processes just the first address returned by DNS */
    error = dns_gethostbyname_addrtype(sWellKnownIpv4OnlyName, addr, DnsNat64Callback, (void *)addr,
                                       LWIP_DNS_ADDRTYPE_IPV6);
    UNLOCK_TCPIP_CORE();

    if (error == ERR_OK)
    {
        /* Address already resolved */
        DnsNat64Callback(sWellKnownIpv4OnlyName, addr, (void *)addr);
        addr = NULL; /* Just deallocated by the callback */
        res  = OT_ERROR_NONE;
    }
    else if (error == ERR_INPROGRESS)
    {
        addr = NULL; /* Will be deallocated by the callback */
        res  = OT_ERROR_NONE;
    } /* else failed */

exit:
    if (addr != NULL)
    {
        otPlatFree(addr);
    }
    return res;
}
#endif /* OPENTHREAD_CONFIG_BORDER_ROUTING_ENABLE */

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static void LwipTaskCb(void *context)
{
    struct ndSendContext *ndSendContexPtr = (struct ndSendContext *)context;

    /* Parse RA and extract prefix form PIO to allow LWIP to configure IP from announced prefix. */
    /* This must be executed before raw_sendto_if_src because the payload from pktBuffer is modified
       inside raw_sendto_if_src */
    RaFromOtToLwip(ndSendContexPtr->infraIfIndex, ndSendContexPtr->pktBuffer->payload, ndSendContexPtr->pktBuffer->len);

    raw_sendto_if_src(sIcmp6RawPcb, ndSendContexPtr->pktBuffer, &ndSendContexPtr->dstIp, sNetifPtr,
                      &ndSendContexPtr->srcIp);

    pbuf_free(ndSendContexPtr->pktBuffer);
    otPlatFree(context);
}

/**
 * Checks if packet is RA and tries to find prefix option in a buffer with icmp6 packet.
 *
 * @param[in]  aBuffer        ICMP6 packet
 * @param[in]  aBufferLength  Length of data
 * @param[out] addr           Pointer to address where prefix bits will be set.
 * @param[out] pref_t         Preferred prefix lifetime
 * @param[out] valid_t        Valid prefix lifetime
 * @return                    True if prefix was found.
 */
static bool GetAddrFromRa(const uint8_t *aBuffer,
                          uint16_t       aBufferLength,
                          ip6_addr_t    *addr,
                          uint32_t      *valid_t,
                          uint32_t      *pref_t)
{
    const uint16_t opt_hdr_len = 2;
    uint16_t       i           = sizeof(struct ra_header);

    if (aBufferLength < sizeof(struct ra_header))
    {
        return false;
    }
    if (aBuffer[0] != ICMP6_TYPE_RA)
    {
        return false;
    }

    while (i + opt_hdr_len <= aBufferLength)
    {
        const struct prefix_option *pref_opt = (struct prefix_option *)&aBuffer[i];
        const uint16_t              opt_len  = pref_opt->length * 8;

        if (pref_opt->type == 3) /* Route information option */
        {
            if (opt_len < sizeof(struct prefix_option))
            {
                /* option too short */
                return false;
            }

            *valid_t = ntohl(pref_opt->valid_lifetime);
            *pref_t  = ntohl(pref_opt->preferred_lifetime);

            /* Copy prefix */
            addr->addr[0] = pref_opt->prefix.addr[0];
            addr->addr[1] = pref_opt->prefix.addr[1];

            return true;
        }
        i += opt_len;
    }
    /* Prefix not found */
    return false;
}

/**
 * Copies second half of link local addr to second half of addr, sets it into netif and updates its lifetimes.
 * @param[in,out] netif   Infrastructure netif
 * @param[in,out] addr    Prefix (Second half will be changed)
 * @param[in]     valid_t Valid lifetime
 * @param[in]     pref_t  Preferred lifetime
 */
static void SetOrUpdateAddrFromRa(struct netif *netif, ip6_addr_t *addr, uint32_t valid_t, uint32_t pref_t)
{
    int8_t addr_idx;

    /* Copy the second half of IP from link local addr (always on index 0) */
    addr->addr[2] = netif_ip6_addr(netif, 0)->addr[2];
    addr->addr[3] = netif_ip6_addr(netif, 0)->addr[3];
    ip6_addr_assign_zone(addr, IP6_UNICAST, netif);

    addr_idx = netif_get_ip6_addr_match(netif, addr);

    if (pref_t > 0)
    {
        /* set preferred */
        if (addr_idx < 0)
        {
            /* not found - add it */
            netif_add_ip6_address(netif, addr, &addr_idx);
        }
        else if (ip6_addr_isdeprecated(netif_ip6_addr_state(netif, addr_idx)))
        {
            /* Addr found and is deprecated - change to preferred  */
            netif_ip6_addr_set_state(netif, addr_idx, IP6_ADDR_PREFERRED);
        }
        /* Make address static otherwise LWIP will not be able to send packets using this prefix */
        netif_ip6_addr_set_valid_life(netif, addr_idx, IP6_ADDR_LIFE_STATIC);
        netif_ip6_addr_set_pref_life(netif, addr_idx, pref_t);
    }
    else if (valid_t > 0)
    {
        /* set deprecated (not preferred) */
        if (addr_idx < 0)
        {
            /* Addr not found - add */
            netif_add_ip6_address(netif, addr, &addr_idx);
        }
        else if (ip6_addr_ispreferred(netif_ip6_addr_state(netif, addr_idx)))
        {
            /* Addr found and is preferred - change to deprecated */
            netif_ip6_addr_set_state(netif, addr_idx, IP6_ADDR_DEPRECATED);
        }
        netif_ip6_addr_set_valid_life(netif, addr_idx, IP6_ADDR_LIFE_STATIC);
        netif_ip6_addr_set_pref_life(netif, addr_idx, 0);
    }
    else
    {
        if (addr_idx > 0)
        {
            /* Addr found - invalidate */
            netif_ip6_addr_set_state(netif, addr_idx, IP6_ADDR_INVALID);
        }
        /* If addr not found - do nothing. */
    }
}
/**
 *  Parses icmp6 packet and sets ip based on prefix to infra netif if found.
 *
 * @param aInfraIfIndex Index of infrastructure netif
 * @param aBuffer       Buffer with icmp6 packet
 * @param aBufferLength Lenght of packet
 */
static void RaFromOtToLwip(uint32_t aInfraIfIndex, const uint8_t *aBuffer, uint16_t aBufferLength)
{
    ip6_addr_t addr;
    uint32_t   valid_t;
    uint32_t   pref_t;

    LWIP_ASSERT_CORE_LOCKED();

    if (GetAddrFromRa(aBuffer, aBufferLength, &addr, &valid_t, &pref_t))
    {
        struct netif *netif = netif_get_by_index(aInfraIfIndex);
        SetOrUpdateAddrFromRa(netif, &addr, valid_t, pref_t);
    }
}

static uint8_t ReceiveIcmp6Message(void *arg, struct raw_pcb *pcb, struct pbuf *p, const ip_addr_t *addr)
{
    (void)arg;
    (void)pcb;

    size_t       icmpv6_type_pos = 40;
    otIp6Address aPeerAddr;

    uint8_t icmpv6_type = *(uint8_t *)(p->payload + icmpv6_type_pos);

    switch (icmpv6_type)
    {
    case ICMP6_TYPE_RS: /* Router solicitation */
    case ICMP6_TYPE_RA: /* Router advertisement */
    case ICMP6_TYPE_NA: /* Neighbor advertisement */

        memcpy(aPeerAddr.mFields.m8, ip_2_ip6(addr), sizeof(otIp6Address));
        otPlatInfraIfRecvIcmp6Nd(sInstance, sInfraIfIndex, &aPeerAddr,
                                 (const uint8_t *)((uint8_t *)p->payload + icmpv6_type_pos), p->len);
        break;

    default:
        break;
    }

    return 0; // packet eaten = 0 => packet was not consumed by application
}

#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE
static uint8_t ReceiveIPV4Message(void *arg, struct raw_pcb *pcb, struct pbuf *p, const ip_addr_t *addr)
{
    uint8_t ret = 0U; /* 0: packet not eaten, return it to lwIP for processing */

    (void)arg;
    (void)pcb;
    (void)addr;

    /* Try to send the packet to NAT64 */
    if (otPlatLwipNat64Send(p) == OT_ERROR_NONE)
    {
        /* Packet eaten by NAT64 translator */
        pbuf_free(p);
        ret = 1U;
    } /* else packet not to be translated or it failed to be translated/sent */

    return ret;
}
#endif /* OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE */

#if OPENTHREAD_CONFIG_BORDER_ROUTING_ENABLE
static void DnsNat64Callback(const char *name, const ip_addr_t *ipaddr, void *arg)
{
    otIp6Address ip6Address;
    otIp6Prefix  prefix = {}; /* Initialize to an empty prefix */
    int          i;
    int          j;
    uint8_t      length;
    uint8_t      dupLength;
    otIp4Address ip4Address;
    otIp4Address dupIp4Address;
    bool         foundDuplicate;

    VerifyOrExit(ipaddr != NULL);
    memcpy(&ip6Address.mFields.m8, (void *)&(ip_2_ip6(ipaddr)->addr), OT_IP6_ADDRESS_SIZE);

    for (i = 0; i < ARRAY_SIZE(sValidNat64PrefixLength); i++)
    {
        length = sValidNat64PrefixLength[i];
        otIp4ExtractFromIp6Address(length, &ip6Address, &ip4Address);

        if (otIp4IsAddressEqual(&ip4Address, &sWellKnownIpv4OnlyAddress1) ||
            otIp4IsAddressEqual(&ip4Address, &sWellKnownIpv4OnlyAddress2))
        {
            // We check that the well-known IPv4 address is present only once in the IPv6 address.
            // In case another instance of the value is found for another prefix length, we ignore this address
            // and search for the other well-known IPv4 address (per RFC 7050 section 3).
            foundDuplicate = false;

            for (j = 0; j < ARRAY_SIZE(sValidNat64PrefixLength); j++)
            {
                if (i == j)
                {
                    continue;
                }

                dupLength = sValidNat64PrefixLength[j];
                otIp4ExtractFromIp6Address(dupLength, &ip6Address, &dupIp4Address);

                if (otIp4IsAddressEqual(&dupIp4Address, &ip4Address))
                {
                    foundDuplicate = true;
                    break;
                }
            }

            if (!foundDuplicate)
            {
                otIp6GetPrefix(&ip6Address, length, &prefix);
                break;
            }
        }
    }

exit:
    /* Notify about the found NAT64 prefix (or empty prefix if query failed) */
    otPlatInfraIfDiscoverNat64PrefixDone(sInstance, sInfraIfIndex, &prefix);
    otPlatFree(arg);
}
#endif /* OPENTHREAD_CONFIG_BORDER_ROUTING_ENABLE */
