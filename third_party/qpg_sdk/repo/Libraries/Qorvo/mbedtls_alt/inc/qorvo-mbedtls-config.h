/*
 * Copyright (c) 2017-2024, Qorvo Inc
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 * CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 * LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * QORVO INC. SHALL NOT, IN ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 * FOR ANY REASON WHATSOEVER.
 *
 *
 */

#pragma once

#ifdef QORVO_MBEDTLS_DEBUG
#ifndef MBEDTLS_DEBUG_C
#define MBEDTLS_DEBUG_C
#endif // MBEDTLS_DEBUG_C
#endif // QORVO_MBEDTLS_DEBUG

// Enables secured CoAP, not used by Thread nor Matter
#ifndef OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE
#define OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE 0
#endif // OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE

// Required for SRP client and server and thus in Thread 1.2 and Matter
#ifndef OPENTHREAD_CONFIG_ECDSA_ENABLE
#define OPENTHREAD_CONFIG_ECDSA_ENABLE 1
#endif // OPENTHREAD_CONFIG_ECDSA_ENABLE

// Enables the optional Thread Commissioner feature
#ifndef OPENTHREAD_CONFIG_COMMISSIONER_ENABLE
#define OPENTHREAD_CONFIG_COMMISSIONER_ENABLE 0
#endif // OPENTHREAD_CONFIG_COMMISSIONER_ENABLE

// Enables TCP, Mandatory for Thread Components, optional for Thread Products
#ifndef OPENTHREAD_CONFIG_TCP_ENABLE
#define OPENTHREAD_CONFIG_TCP_ENABLE 0
#endif // OPENTHREAD_CONFIG_TCP_ENABLE

// Enables the Thread Joiner role, Mandatory for Thread Components, optional for Thread Products
#ifndef OPENTHREAD_CONFIG_JOINER_ENABLE
#define OPENTHREAD_CONFIG_JOINER_ENABLE 0
#endif // OPENTHREAD_CONFIG_JOINER_ENABLE

// Enables the Thread Border Agent functionality, Mandatory on Border Routers
#ifndef OPENTHREAD_CONFIG_BORDER_AGENT_ENABLE
#define OPENTHREAD_CONFIG_BORDER_AGENT_ENABLE 0
#endif // OPENTHREAD_CONFIG_BORDER_AGENT_ENABLE
