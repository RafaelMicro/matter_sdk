/*
 *  Copyright (c) 2017, The OpenThread Authors.
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
 *   This file includes the platform-specific initializers.
 *
 */

#ifndef PLATFORM_K32W1_H_
#define PLATFORM_K32W1_H_

#ifndef OT_EXTERNAL_BUILD
#include <openthread-core-config.h>
#include <openthread/config.h>
#endif

#include <stdint.h>

#include <openthread/instance.h>

/**
 * This function initializes the alarm service used by OpenThread.
 *
 */
void otPlatAlarmInit(void);

/**
 * This function performs alarm driver processing.
 *
 * @param[in]  aInstance  The OpenThread instance structure.
 *
 */
void otPlatAlarmProcess(otInstance *aInstance);

/**
 * This function performs uart driver processing.
 *
 */
void otPlatUartProcess();

/**
 * This function performs UART Blocking Send
 *
 * @param[in]  aBuf  Buffer to be sent over UART
 * @param[in]  len   Length of the above buffer
 *
 */
void otPlatUartSendBlocking(const uint8_t *aBuf, uint32_t len);

/**
 * This function initializes the radio service used by OpenThread.
 *
 */
void otPlatRadioInit(void);

/**
 * This function performs radio driver processing.
 *
 * @param[in]  aInstance  The OpenThread instance structure.
 *
 */
void otPlatRadioProcess(otInstance *aInstance);

#if (OPENTHREAD_CONFIG_LOG_OUTPUT == OPENTHREAD_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED)
/**
 * This function initializes the platform defined logging.
 *
 */
void K32WLogInit();
#endif /* OPENTHREAD_CONFIG_LOG_OUTPUT == OPENTHREAD_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED */
#endif // PLATFORM_K32W1_H_

/**
 * This function initializes the random number service used by OpenThread.
 *
 */
void K32WRandomInit(void);
