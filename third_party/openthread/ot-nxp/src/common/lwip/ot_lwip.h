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

#ifndef OT_LWIP_H_
#define OT_LWIP_H_

#include <openthread/instance.h>
#include <openthread/message.h>

#include "lwip/tcpip.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function is used to lock/unlock the Open Thread task
 *
 * @param[in] bLockState         Set to TRUE to lock the task and to FALSE to unlock it.
 *
 */
typedef void (*otPlatLockTaskCb)(bool bLockState);

/*!
 * @brief This function initializes LWIP stack
 *
 * @param[in] aInstance pointer to openthread instance
 * @param[in] lockTaskCb a function pointer to lock/unlock the Thread task
 */
void otPlatLwipInit(otInstance *aInstance, otPlatLockTaskCb lockTaskCb);

/*!
 * @brief This function creates an OpenThread interface in Lwip
 *        otPlatLwipInit must be called before
 *
 */
void otPlatLwipAddThreadInterface(void);

/*!
 * @brief otStateChangedCallback that must be registered directly to OpenThread using otSetStateChangedCallback or
 * called from another otStateChangedCallback This function will check if the device role has changed or if ipv6
 * addresses have been added or removed, and will forward the updates to the LWIP interface
 *
 * @param[in] flags flags informing on what's changed
 * @param[in] context not used
 */
void otPlatLwipUpdateState(otChangedFlags flags, void *context);

/*!
 * @brief This function converts an OT message to a Lwip message
 *
 * @param[in] otIpPkt pointer to OT message
 * @param[in] bTransport
 * @return struct pbuf* pointer to Lwip message
 */
struct pbuf *otPlatLwipConvertToLwipMsg(otMessage *otIpPkt, bool bTransport);

/*!
 * @brief This function converts a Lwip message to an OT message
 *
 * @param[in] lwipIpPkt pointer to Lwip message
 * @return otMessage* pointer to OT message
 */
otMessage *otPlatLwipConvertToOtMsg(struct pbuf *lwipIpPkt);

/*!
 * @brief This function returns a pointer to the already assigned OpenThread network interface
 *
 * @return struct netif* pointer to OT network intergace
 */
struct netif *otPlatLwipGetOtNetif(void);

#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE
/*!
 * @brief This function checks if an IPv4 lwIP datagram should be translated to an IPv6 datagram
 * and sends it via the Thread interface if so.
 *
 * @param[in] lwipIpv4Pkt pointer to Lwip IPv4 message to forward as IPv6
 * @return OT_ERROR_NONE if translated to IPv6 and successfully sent via NAT64, failure code otherwise
 */
otError otPlatLwipNat64Send(struct pbuf *lwipIpv4Pkt);
#endif /* OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* OT_LWIP_H_ */
