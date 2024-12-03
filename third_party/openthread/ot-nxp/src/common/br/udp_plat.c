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
 *   This file implements the OpenThread platform abstraction for UDP.
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "udp_plat.h"
#include <openthread/ip6.h>
#include <openthread/tasklet.h>
#include <openthread/udp.h>
#include <openthread/platform/memory.h>
#include <openthread/platform/udp.h>
#include "lwip/api.h"
#include "lwip/icmp6.h"
#include "lwip/inet.h"
#include "lwip/mld6.h"
#include "lwip/prot/dns.h"
#include "lwip/prot/iana.h"
#include "lwip/raw.h"
#include "lwip/sockets.h"
#include "lwip/tcpip.h"
#include "lwip/udp.h"

#include "common/code_utils.hpp"

/* -------------------------------------------------------------------------- */
/*                                 Definitions                                */
/* -------------------------------------------------------------------------- */
struct udpSendContext
{
    struct udp_pcb *pcb;
    struct pbuf    *buf;
    ip_addr_t       peer_addr;
    uint16_t        peerPort;
};

struct udpReceiveContext
{
    otUdpSocket  *socket;
    otMessage    *message;
    otMessageInfo message_info;
};
/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static uint8_t sBackboneNetifIdx;
static uint8_t sOtNetifIdx;

static otInstance   *sInstance = NULL;
static struct netif *sBackboneNetifPtr;
static struct netif *sOtNetifPtr;
/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static void         recv_fcn(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
static uint8_t      getInterfaceIndex(otNetifIdentifier identifier);
static struct pbuf *convertToLwipMsg(otMessage *otIpPkt, bool bTransport);
static void         lwipTaskCb(void *context);
static void         otTaskCb(void *context);
/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void UdpPlatInit(otInstance *aInstance, struct netif *backboneNetif, struct netif *otNetif)
{
    sInstance         = aInstance;
    sBackboneNetifPtr = backboneNetif;
    sOtNetifPtr       = otNetif;
    sBackboneNetifIdx = netif_get_index(backboneNetif);
    sOtNetifIdx       = netif_get_index(otNetif);
}

otError otPlatUdpSocket(otUdpSocket *aUdpSocket)
{
    otError error = OT_ERROR_NONE;

    struct udp_pcb *pcb;
    pcb = udp_new();

    VerifyOrExit(pcb != NULL, error = OT_ERROR_FAILED);

    udp_recv(pcb, recv_fcn, aUdpSocket);
    aUdpSocket->mHandle = pcb;

exit:
    return error;
}

otError otPlatUdpClose(otUdpSocket *aUdpSocket)
{
    otError         error = OT_ERROR_NONE;
    struct udp_pcb *pcb   = (struct udp_pcb *)aUdpSocket->mHandle;

    VerifyOrExit(pcb != NULL, error = OT_ERROR_INVALID_ARGS);

    udp_remove(pcb);

exit:
    return error;
}

otError otPlatUdpBind(otUdpSocket *aUdpSocket)
{
    otError         error = OT_ERROR_NONE;
    struct udp_pcb *pcb   = (struct udp_pcb *)aUdpSocket->mHandle;
    uint16_t        port  = aUdpSocket->mSockName.mPort;
    ip_addr_t       addr;

    memcpy(ip_2_ip6(&addr)->addr, aUdpSocket->mSockName.mAddress.mFields.m8, sizeof(ip_2_ip6(&addr)->addr));
    addr.type = IPADDR_TYPE_ANY;

    VerifyOrExit(ERR_OK == udp_bind(pcb, &addr, port), error = OT_ERROR_FAILED);

exit:
    return error;
}

otError otPlatUdpBindToNetif(otUdpSocket *aUdpSocket, otNetifIdentifier aNetifIdentifier)
{
    otError         error        = OT_ERROR_NONE;
    struct udp_pcb *pcb          = (struct udp_pcb *)aUdpSocket->mHandle;
    struct netif   *currentNetif = NULL;

    switch (aNetifIdentifier)
    {
    case OT_NETIF_BACKBONE:
        currentNetif = sBackboneNetifPtr;
        break;
    case OT_NETIF_THREAD:
        currentNetif = sOtNetifPtr;
        break;
    case OT_NETIF_UNSPECIFIED:
    default:
        break;
    }

    udp_bind_netif(pcb, currentNetif);

    return error;
}

otError otPlatUdpConnect(otUdpSocket *aUdpSocket)
{
    otError         error = OT_ERROR_NONE;
    struct udp_pcb *pcb   = (struct udp_pcb *)aUdpSocket->mHandle;
    uint16_t        port  = aUdpSocket->mPeerName.mPort;
    ip_addr_t       addr  = {0};

    // LWIP doesn't threat the case were port or address are 0. In this case, the connect should act more like a
    // disconnect and clear the connect information stored in PCB. If we let LWIP connect with 0, it will drop
    // valid UDP packets because the source port/address doesn't match 0.
    if ((port != 0) && !otIp6IsAddressUnspecified(&aUdpSocket->mPeerName.mAddress))
    {
        memcpy(ip_2_ip6(&addr)->addr, aUdpSocket->mPeerName.mAddress.mFields.m8, sizeof(ip_2_ip6(&addr)->addr));
        addr.type            = IPADDR_TYPE_V6;
        addr.u_addr.ip6.zone = IP6_NO_ZONE;

        VerifyOrExit(ERR_OK == udp_connect(pcb, &addr, port), error = OT_ERROR_FAILED);
    }
exit:
    return error;
}

otError otPlatUdpSend(otUdpSocket *aUdpSocket, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    otError                error            = OT_ERROR_NONE;
    struct udpSendContext *udpSendContexPtr = (struct udpSendContext *)otPlatCAlloc(1, sizeof(struct udpSendContext));
    VerifyOrExit(NULL != udpSendContexPtr, error = OT_ERROR_FAILED);

    udpSendContexPtr->pcb = (struct udp_pcb *)aUdpSocket->mHandle;
    ip_addr_t src_addr    = {0};
    uint16_t  src_port;
    uint8_t   hop_limit;
    bool      isMulticastLoop;
    uint8_t   netif_idx;

    if (udpSendContexPtr->pcb->netif_idx == NETIF_NO_INDEX)
    {
        if (aMessageInfo->mIsHostInterface)
        {
            netif_idx = netif_get_index(sBackboneNetifPtr);
        }
        else
        {
            netif_idx = netif_get_index(sOtNetifPtr);
        }
    }
    else
    {
        netif_idx = udpSendContexPtr->pcb->netif_idx;
    }

    udpSendContexPtr->buf = convertToLwipMsg(aMessage, true);
    VerifyOrExit(udpSendContexPtr->buf != NULL, error = OT_ERROR_FAILED);

    src_port                   = aMessageInfo->mSockPort;
    udpSendContexPtr->peerPort = aMessageInfo->mPeerPort;
    isMulticastLoop            = aMessageInfo->mMulticastLoop;
    hop_limit                  = aMessageInfo->mHopLimit ? aMessageInfo->mHopLimit : UDP_TTL;

    memcpy(ip_2_ip6(&src_addr)->addr, aMessageInfo->mSockAddr.mFields.m8, sizeof(ip_2_ip6(&src_addr)->addr));
    memcpy(ip_2_ip6(&udpSendContexPtr->peer_addr)->addr, aMessageInfo->mPeerAddr.mFields.m8,
           sizeof(ip_2_ip6(&udpSendContexPtr->peer_addr)->addr));

    src_addr.type                    = IPADDR_TYPE_V6;
    udpSendContexPtr->peer_addr.type = IPADDR_TYPE_V6;

    udpSendContexPtr->pcb->ttl = hop_limit;
    udpSendContexPtr->pcb->flags &= ~(UDP_FLAGS_MULTICAST_LOOP);
    udpSendContexPtr->pcb->local_ip   = src_addr;
    udpSendContexPtr->pcb->local_port = src_port;
    if (isMulticastLoop)
    {
        udpSendContexPtr->pcb->flags |= (UDP_FLAGS_MULTICAST_LOOP);
    }

    if (!ip_addr_isany(&udpSendContexPtr->pcb->local_ip))
    {
        // Assign zone if the source address has been specified by the application
        ip6_addr_assign_zone(ip_2_ip6(&udpSendContexPtr->pcb->local_ip), IP6_UNICAST, netif_get_by_index(netif_idx));
    }

    // The LWIP address needs to be intilialized correctly with a zone
    if (ip_addr_ismulticast(&udpSendContexPtr->peer_addr))
    {
        ip6_addr_assign_zone(ip_2_ip6(&udpSendContexPtr->peer_addr), IP6_MULTICAST, netif_get_by_index(netif_idx));
    }
    else
    {
        ip6_addr_assign_zone(ip_2_ip6(&udpSendContexPtr->peer_addr), IP6_UNICAST, netif_get_by_index(netif_idx));
    }

    if (ERR_OK != tcpip_callback(lwipTaskCb, (void *)udpSendContexPtr))
    {
        pbuf_free(udpSendContexPtr->buf);
        otPlatFree(udpSendContexPtr);
        error = OT_ERROR_FAILED;
    }

exit:
    otMessageFree(aMessage);
    return error;
}

otError otPlatUdpJoinMulticastGroup(otUdpSocket        *aUdpSocket,
                                    otNetifIdentifier   aNetifIdentifier,
                                    const otIp6Address *aAddress)
{
    otError error = OT_ERROR_NONE;

    ip6_addr_t addr = {0};
    memcpy(&addr.addr, aAddress->mFields.m8, sizeof(addr.addr));

    VerifyOrExit(ERR_OK == mld6_joingroup_netif(netif_get_by_index(getInterfaceIndex(aNetifIdentifier)), &addr),
                 error = OT_ERROR_FAILED);

exit:
    return error;
}

otError otPlatUdpLeaveMulticastGroup(otUdpSocket        *aUdpSocket,
                                     otNetifIdentifier   aNetifIdentifier,
                                     const otIp6Address *aAddress)
{
    otError error = OT_ERROR_NONE;

    ip6_addr_t addr = {0};
    memcpy(&addr.addr, aAddress->mFields.m8, sizeof(addr.addr));

    VerifyOrExit(ERR_OK == mld6_leavegroup_netif(netif_get_by_index(getInterfaceIndex(aNetifIdentifier)), &addr),
                 error = OT_ERROR_FAILED);

exit:
    return error;
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static void recv_fcn(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    (void)pcb;
    otError           error       = OT_ERROR_NONE;
    otMessageSettings msgSettings = {.mLinkSecurityEnabled = false, .mPriority = OT_MESSAGE_PRIORITY_NORMAL};

    struct udpReceiveContext *udpReceiveContextPtr =
        (struct udpReceiveContext *)otPlatCAlloc(1, sizeof(struct udpReceiveContext));
    VerifyOrExit(NULL != udpReceiveContextPtr);

    const struct ip6_hdr *ip6_header   = ip6_current_header();
    struct netif         *source_netif = ip_current_netif();
    uint8_t              *data_ptr     = (uint8_t *)p->payload;

    udpReceiveContextPtr->socket                 = (otUdpSocket *)arg;
    udpReceiveContextPtr->message_info.mSockPort = 0;
    // message_info.mPeerAddr is populated with the remote IPv6 address from which the packet was received.
    memcpy(&udpReceiveContextPtr->message_info.mPeerAddr, ip_2_ip6(addr)->addr,
           sizeof(udpReceiveContextPtr->message_info.mPeerAddr));
    // message_info.mSockAddr is populated with the destination IPv6 address to which the packet is sent.
    memcpy(&udpReceiveContextPtr->message_info.mSockAddr, ip6_current_dest_addr(),
           sizeof(udpReceiveContextPtr->message_info.mSockAddr));
    // message_info.mPeerPort is populated with the remote port from which the packet was received.
    udpReceiveContextPtr->message_info.mPeerPort = port;
    udpReceiveContextPtr->message_info.mHopLimit = IP6H_HOPLIM(ip6_header);

    udpReceiveContextPtr->message_info.mIsHostInterface = (netif_get_index(source_netif) == sBackboneNetifIdx);

    udpReceiveContextPtr->message = otUdpNewMessage(sInstance, &msgSettings);

    VerifyOrExit(udpReceiveContextPtr->message != NULL, otPlatFree(udpReceiveContextPtr));
    VerifyOrExit(otMessageAppend(udpReceiveContextPtr->message, data_ptr, p->tot_len) == OT_ERROR_NONE,
                 error = OT_ERROR_FAILED);
    VerifyOrExit(otTaskletExecute(sInstance, otTaskCb, (void *)udpReceiveContextPtr) == OT_ERROR_NONE,
                 error = OT_ERROR_FAILED);

exit:
    if (error == OT_ERROR_FAILED)
    {
        otMessageFree(udpReceiveContextPtr->message);
        otPlatFree(udpReceiveContextPtr);
    }
    pbuf_free(p);
}

static uint8_t getInterfaceIndex(otNetifIdentifier aNetifIdentifier)
{
    switch (aNetifIdentifier)
    {
    case OT_NETIF_THREAD:
        return sOtNetifIdx;
        break;
    case OT_NETIF_BACKBONE:
        return sBackboneNetifIdx;
        break;
    case OT_NETIF_UNSPECIFIED:
    default:
        return NETIF_NO_INDEX;
        break;
    }
}

static struct pbuf *convertToLwipMsg(otMessage *otIpPkt, bool bTransport)
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

static void lwipTaskCb(void *context)
{
    struct udpSendContext *udpSendContexPtr = (struct udpSendContext *)context;

    udp_sendto(udpSendContexPtr->pcb, udpSendContexPtr->buf, &udpSendContexPtr->peer_addr, udpSendContexPtr->peerPort);
    pbuf_free(udpSendContexPtr->buf);
    otPlatFree(context);
}

static void otTaskCb(void *context)
{
    struct udpReceiveContext *udpReceiveContextPtr = (struct udpReceiveContext *)context;
    udpReceiveContextPtr->socket->mHandler(udpReceiveContextPtr->socket->mContext, udpReceiveContextPtr->message,
                                           &udpReceiveContextPtr->message_info);

    otMessageFree(udpReceiveContextPtr->message);
    otPlatFree(context);
}
