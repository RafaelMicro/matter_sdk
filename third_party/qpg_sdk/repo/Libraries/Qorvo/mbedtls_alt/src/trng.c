/*
 * Copyright (c) 2020, Qorvo Inc
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
 */

/** @file "trng.c"
 *
 *  True random number generator for Qorvo chips
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT) && defined(MBEDTLS_ENTROPY_C)

#include "mbedtls/entropy.h"

extern void gpRandom_GetFromDRBG(uint8_t nmbrRandomBytes, uint8_t* pBuffer);

int mbedtls_hardware_poll(void* data, unsigned char* output, size_t len, size_t* olen)
{
    (void)data;

    if(NULL == output || 0 == len)
    {
        return -1;
    }
    if(len > 256)
    {
        len = 256;
    }
    gpRandom_GetFromDRBG((uint8_t)len, (uint8_t*)output);

    *olen = len;
    return 0;
}

#endif /* MBEDTLS_ENTROPY_HARDWARE_ALT && MBEDTLS_ENTROPY_C */
