/*
 * Copyright (c) 2023, Qorvo Inc
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

#include "common-mbedtls-config.h"

#define MBEDTLS_AES_ALT
#define MBEDTLS_ECDH_COMPUTE_SHARED_ALT
#define MBEDTLS_ECDH_GEN_PUBLIC_ALT
#define MBEDTLS_ECDSA_VERIFY_ALT
#define MBEDTLS_ECDSA_SIGN_ALT
#define MBEDTLS_ECP_ALT
#define MBEDTLS_SHA256_ALT

#define MBEDTLS_ENTROPY_HARDWARE_ALT

// Using Qorvo HW crypto engine
#undef MBEDTLS_ECP_WINDOW_SIZE
#undef MBEDTLS_ECP_FIXED_POINT_OPTIM

#if OPENTHREAD_CONFIG_JOINER_ENABLE || OPENTHREAD_CONFIG_COMMISSIONER_ENABLE
#define MBEDTLS_ECJPAKE_ALT
#endif
