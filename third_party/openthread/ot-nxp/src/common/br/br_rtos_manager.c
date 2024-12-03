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
 *   This file implements the Border Router's initialization helper functions.
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "br_rtos_manager.h"
#include "border_agent.h"
#include "infra_if.h"
#include "udp_plat.h"

#include "lwip_hooks.h"
#include "lwip_mcast.h"
#include "lwip/dhcp6.h"

#include <openthread/backbone_router_ftd.h>
#include <openthread/border_router.h>
#include <openthread/border_routing.h>
#include <openthread/nat64.h>
#include <openthread/srp_server.h>

#include <string.h>
/* -------------------------------------------------------------------------- */
/*                             Private memory                                 */
/* -------------------------------------------------------------------------- */
otInstance *sInstance;

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static void HandleMulticastListenerCallback(void                                  *aContext,
                                            otBackboneRouterMulticastListenerEvent aEvent,
                                            const otIp6Address                    *aAddress);

static void Dhcp6PrefixChangedCb(struct netif *netif, const struct dhcp6_delegated_prefix *prefix, u8_t valid);

static void Dhcp6PdInit(struct netif *netif);
/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void BrInitServices(otInstance *aInstance, struct netif *aExtNetif, struct netif *aThreadNetif)
{
    sInstance = aInstance;

#if OT_APP_BR_LWIP_HOOKS_EN
    lwipHooksInit(sInstance, aExtNetif, aThreadNetif);
#endif

    UdpPlatInit(aInstance, aExtNetif, aThreadNetif);
    InfraIfInit(aInstance, aExtNetif);

    otBorderRoutingInit(aInstance, netif_get_index(aExtNetif), true);
    otBorderRoutingSetEnabled(aInstance, true);
    otBorderRoutingDhcp6PdSetEnabled(aInstance, true);
    otBackboneRouterSetEnabled(aInstance, true);
    otBackboneRouterSetMulticastListenerCallback(aInstance, HandleMulticastListenerCallback, aExtNetif);
    otSrpServerSetAutoEnableMode(aInstance, true);

    BorderAgentInit(aInstance);
    Dhcp6PdInit(aExtNetif);

#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE || OPENTHREAD_CONFIG_NAT64_BORDER_ROUTING_ENABLE
    otNat64SetEnabled(aInstance, true);
#endif
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static void Dhcp6PrefixChangedCb(struct netif *netif, const struct dhcp6_delegated_prefix *prefix, u8_t valid)
{
    if ((netif != NULL) && (prefix != NULL) && (valid == true))
    {
        otBorderRoutingPrefixTableEntry prefixEntry = {0};

        prefixEntry.mIsOnLink          = true;
        prefixEntry.mValidLifetime     = prefix->prefix_valid;
        prefixEntry.mPreferredLifetime = prefix->prefix_valid;

        // Use only 64 long pref to allow SLAAC even if we got smaller prefix. The remainig bits until 64
        // legth will be 0s.
        prefixEntry.mPrefix.mLength = 64;
        memcpy(prefixEntry.mPrefix.mPrefix.mFields.m8, prefix->prefix.addr, sizeof(otIp6Address));

        otBorderRoutingDhcp6PdProcessPrefix(sInstance, &prefixEntry);
    }
}

static void Dhcp6PdInit(struct netif *netif)
{
    const struct dhcp6_delegated_prefix *prefix;

    dhcp6_enable(netif);
    dhcp6_register_pd_callback(netif, &Dhcp6PrefixChangedCb);

    // Update OT with current state of PD
    prefix = dhcp6_get_delegated_prefix(netif);
    Dhcp6PrefixChangedCb(netif, prefix, (prefix->prefix_valid > 0));
}

static void HandleMulticastListenerCallback(void                                  *aContext,
                                            otBackboneRouterMulticastListenerEvent aEvent,
                                            const otIp6Address                    *aAddress)

{
    if (aEvent == OT_BACKBONE_ROUTER_MULTICAST_LISTENER_ADDED)
    {
        lwipMcastSubscribe((otIp6Address *)aAddress, (struct netif *)aContext);
    }
    else
    {
        lwipMcastUnsubscribe((otIp6Address *)aAddress, (struct netif *)aContext);
    }
}
