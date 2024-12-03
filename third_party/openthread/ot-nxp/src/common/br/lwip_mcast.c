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

#include "lwip_mcast.h"

#include <string.h>

static ip6_addr_t mcastFilterAddrs[MEMP_NUM_MLD6_GROUP] = {0};

bool lwipMcastFilterHas(ip6_addr_t *addr)
{
    for (size_t i = 0; i < MEMP_NUM_MLD6_GROUP; i++)
    {
        /*
            Because of the bogus zone assignment in lwipMcastSubscribe,
            only the address itself (excluding its zone) is compared.
        */
        if (memcmp(addr->addr, mcastFilterAddrs[i].addr, sizeof(addr->addr)) == 0)
            return true;
    }

    return false;
}

static err_t filterAdd(ip6_addr_t *addr)
{
    for (size_t i = 0; i < MEMP_NUM_MLD6_GROUP; i++)
    {
        if (ip6_addr_eq(addr, &mcastFilterAddrs[i]))
            return ERR_OK;

        if (ip6_addr_eq(IP6_ADDR_ANY6, &mcastFilterAddrs[i]))
        {
            mcastFilterAddrs[i] = *addr;
            return ERR_OK;
        }
    }

    return ERR_MEM;
}

static void filterRemove(ip6_addr_t *addr)
{
    for (size_t i = 0; i < MEMP_NUM_MLD6_GROUP; i++)
    {
        if (ip6_addr_eq(addr, &mcastFilterAddrs[i]))
        {
            ip6_addr_set_any(&mcastFilterAddrs[i]);
            return;
        }
    }
}

err_t lwipMcastSubscribe(otIp6Address *addr, struct netif *ifInfra)
{
    /*
        FIXME: We are setting a bogus zone index so that lwIP does not consume incoming multicast
        traffic and instead forwards it. Yes, it's an ugly hack.

        The thing is, mld6_lookfor_group compares adresses literally, thus also compares zone indexes.
        If it finds a group (previously subscribed to by mld6_joingroup_netif, which emits an MLD report
        and configures the MAC address filter), it finds a valid netif from which the packet came and
        decides to further process the packet instead of forwarding it.

        We still need to configure that MAC address filter and express the intent to join a multicast group,
        but also need to forward that traffic.
    */
    ip6_addr_t lwipAddr = {.addr = {[0] = addr->mFields.m32[0],
                                    [1] = addr->mFields.m32[1],
                                    [2] = addr->mFields.m32[2],
                                    [3] = addr->mFields.m32[3]},
                           .zone = 255};

    err_t status = filterAdd(&lwipAddr);
    if (status != ERR_OK)
        return status;

    return mld6_joingroup_netif(ifInfra, &lwipAddr);
}

err_t lwipMcastUnsubscribe(otIp6Address *addr, struct netif *ifInfra)
{
    ip6_addr_t lwipAddr = {.addr = {[0] = addr->mFields.m32[0],
                                    [1] = addr->mFields.m32[1],
                                    [2] = addr->mFields.m32[2],
                                    [3] = addr->mFields.m32[3]},
                           .zone = 255};

    err_t status = mld6_leavegroup_netif(ifInfra, &lwipAddr);
    if (status != ERR_OK)
        return status;

    filterRemove(&lwipAddr);
    return ERR_OK;
}
