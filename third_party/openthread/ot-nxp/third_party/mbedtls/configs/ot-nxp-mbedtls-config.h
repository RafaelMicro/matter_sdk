/*
 *  Copyright (c) 2021-2022, The OpenThread Authors.
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

#ifndef RT_MBEDTLS_CONFIG_H
#define RT_MBEDTLS_CONFIG_H

/* FreeRTOS is always supported for RT platform so enable threading */
#define MBEDTLS_MCUX_FREERTOS_THREADING_ALT
#define MBEDTLS_THREADING_C
#define MBEDTLS_THREADING_ALT

/* SDK mbetdls config include */
#include "ksdk_mbedtls_config.h"

/* Undef defines that would be re-defined in the ksdk mbedtls config file */
#undef MBEDTLS_MPI_MAX_SIZE
#undef MBEDTLS_SSL_MAX_CONTENT_LEN
#undef MBEDTLS_ECP_MAX_BITS
#undef MBEDTLS_PLATFORM_MEMORY
#undef MBEDTLS_PLATFORM_STD_CALLOC
#undef MBEDTLS_PLATFORM_STD_FREE

/* Always undef this flag on SDK 2.13 as the new version of freeRTOS heap defines the function pvPortCalloc */
#undef MBEDTLS_FREESCALE_FREERTOS_CALLOC_ALT

/* Openthread mbetdls config include */
#include "mbedtls-config.h"

/* Always undef MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES */
#undef MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
/* Disable SHA512 which is not used and does not have hardware support */
#undef MBEDTLS_SHA512_C

#endif // RT_MBEDTLS_CONFIG_H
