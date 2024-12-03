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

#include "lwip_hooks.h"
#include "lwip_mcast.h"

#include <assert.h>
#include <string.h>

#include <openthread/backbone_router_ftd.h>
#include <openthread/dataset.h>
#include <openthread/ip6.h>
#include <openthread/netdata.h>
#include <openthread/thread.h>
#include "lwip/ip.h"
#include "lwip/ip6.h"
#include "lwip/stats.h"

static otInstance   *sInstance = NULL;
static struct netif *ifInfra   = NULL;
static struct netif *ifThread  = NULL;

/**
 * @brief Checks if lwIP address matches given prefix.
 */
static int lwipAddressMatchesPrefix(const ip6_addr_t *address, const uint8_t *prefix, const uint8_t prefix_len)
{
    uint8_t        bytes;
    uint32_t       mask;
    const uint8_t *addr;

    assert(address != NULL);
    assert(prefix != NULL);
    assert(prefix_len <= 128U);

    bytes = prefix_len / 8U;
    addr  = (uint8_t *)address->addr;

    if (memcmp(addr, prefix, bytes) != 0)
    {
        return 0;
    }

    mask = (0xFFU << (8U - (prefix_len % 8U))) & 0xFFU;
    return ((addr[bytes] & mask) == (prefix[bytes] & mask));
}

/**
 * @brief Facilitates multicast forwarding.
 */
static int forwardMulticast(ip6_addr_t *src, ip6_addr_t *dest, struct pbuf *p, struct netif *netif)
{
    struct ip6_hdr *hdr = (struct ip6_hdr *)p->payload;

    /* This hook does not care about non-multicast addresses */
    if (!ip6_addr_ismulticast(dest))
        return 1;

    /* Multicast traffic below admin local (thus not inter-network multicast) should not be forwarded */
    if (ip6_addr_multicast_scope(dest) < IP6_MULTICAST_SCOPE_ADMIN_LOCAL)
        return 0;

    /* Packet coming from the infra network - forward it to the Thread netif */
    if (netif == ifInfra)
    {
        /* Forward only packets coming from groups that are listened to */
        if (lwipMcastFilterHas(dest))
        {
            if (IP6H_HOPLIM(hdr)-- > 1)
            {
                ifThread->output_ip6(ifThread, p, dest);
            }
        }

        return 0;
    }
    /* Packet coming from the Thread netif - forward it to the infra network */
    else if (netif == ifThread)
    {
        // A secondary BBR SHOULD NOT forward a multicast packet onto an External Interface except when:
        // 1) The secondary BBR is explicitly configured to forward (method outside the scope of Thread spec)
        // 2) The secondary BBR is connected to other external networks than the Primary BBR (method outside the
        // scope of Thread spec)

        if (otBackboneRouterGetState(sInstance) == OT_BACKBONE_ROUTER_STATE_SECONDARY ||
            otBackboneRouterGetState(sInstance) == OT_BACKBONE_ROUTER_STATE_DISABLED)
        {
            return 0;
        }

        if (IP6H_HOPLIM(hdr)-- > 1)
        {
            ifInfra->output_ip6(ifInfra, p, dest);
        }
        return 0;
    }

    /* Unknown netif - we don't care */
    return 1;
}

/**
 * @brief Determines if packet is allowed to be forwarded (according to Thread specification).
 */
static int isLegalToForward(ip6_addr_t *src, ip6_addr_t *dest, struct pbuf *p, struct netif *netif)
{
    otNetworkDataIterator    iterator = OT_NETWORK_DATA_ITERATOR_INIT;
    otBorderRouterConfig     config;
    const otMeshLocalPrefix *meshLocalPrefix;

    if (netif == ifThread)
    {
        while (otNetDataGetNextOnMeshPrefix(sInstance, &iterator, &config) == OT_ERROR_NONE)
        {
            if (config.mDp)
            {
                continue;
            }
            if (lwipAddressMatchesPrefix(dest, config.mPrefix.mPrefix.mFields.m8, config.mPrefix.mLength))
            {
                /* An IPv6 packet with a destination address within the Thread Network OMR prefix is
                 * never forwarded from the Thread Network to outside the Thread Network. */
                return 0;
            }
        }
    }

    meshLocalPrefix = otThreadGetMeshLocalPrefix(sInstance);
    if (meshLocalPrefix != NULL)
    {
        if (lwipAddressMatchesPrefix(src, meshLocalPrefix->m8, OT_IP6_PREFIX_SIZE * 8U) ||
            lwipAddressMatchesPrefix(dest, meshLocalPrefix->m8, OT_IP6_PREFIX_SIZE * 8U))
        {
            /* An IPv6 packet with a mesh-local source address or a mesh-local destination address
             * is never forwarded between the Thread Network and the Adjacent Infrastructure Link. */
            return 0;
        }
    }

    /* Otherwise - let packet be processed by lwIP forwarding rules. */
    return 1;
}

void lwipHooksInit(otInstance *aInstance, struct netif *infra, struct netif *thread)
{
    assert(aInstance != NULL && infra != NULL && thread != NULL);

    sInstance = aInstance;
    ifInfra   = infra;
    ifThread  = thread;
}

int lwipCanForwardHook(ip6_addr_t *src, ip6_addr_t *dest, struct pbuf *p, struct netif *netif)
{
    if (forwardMulticast(src, dest, p, netif) == 0)
        return 0;

    if (isLegalToForward(src, dest, p, netif) == 0)
        return 0;

    return 1;
}

int lwipInputHook(struct pbuf *pbuf, struct netif *input_netif)
{
    otNetworkDataIterator iterator = OT_NETWORK_DATA_ITERATOR_INIT;
    otBorderRouterConfig  config;
    struct ip6_hdr       *ip6hdr;
    ip_addr_t             src;

    if ((ifThread == NULL) || (ifInfra == NULL))
        return 0;

    if (input_netif != ifThread)
    {
        ip6hdr = (struct ip6_hdr *)pbuf->payload;

        /* Ignore packet if header length exceeds first pbuf length, or ip length exceeds total pbuf length
         * as it would not be safe to get address from it and it will be dropped by lwIP anyway. */
        if ((IP6_HLEN > pbuf->len) || (IP6H_PLEN(ip6hdr) > (pbuf->tot_len - IP6_HLEN)))
        {
            return 0;
        }

        /* Result of ip6_current_src_addr() is still empty at the time of LWIP_HOOK_IP6_INPUT call,
         * so copy the source address from pbuf. */
        ip_addr_copy_from_ip6_packed(src, ip6hdr->src);

        /* Compare source address with OMR prefix. */
        while (otNetDataGetNextOnMeshPrefix(sInstance, &iterator, &config) == OT_ERROR_NONE)
        {
            if (config.mDp)
            {
                continue;
            }
            if (lwipAddressMatchesPrefix(ip_2_ip6(&src), config.mPrefix.mPrefix.mFields.m8, config.mPrefix.mLength))
            {
                /* Packet with source address within Thread Network OMR prefix received on a non-Thread interface. Drop
                 * it. */
                pbuf_free(pbuf);
                IP6_STATS_INC(ip6.drop);
                return 1;
            }
        }
    }

    return 0;
}
