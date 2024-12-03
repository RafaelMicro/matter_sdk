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
 *   This file includes rw610n compile-time configuration constants
 *   for OpenThread.
 */

#ifndef OPENTHREAD_CORE_RW610N_CONFIG_H_
#define OPENTHREAD_CORE_RW610N_CONFIG_H_

/**
 * @def OPENTHREAD_CONFIG_PLATFORM_INFO
 *
 * The platform-specific string to insert into the OpenThread version string.
 *
 */
#define OPENTHREAD_CONFIG_PLATFORM_INFO "RW610N"

/**
 * @def OPENTHREAD_CONFIG_LOG_OUTPUT
 *
 * The rw610n platform provides an otPlatLog() function.
 */
#ifndef OPENTHREAD_CONFIG_LOG_OUTPUT /* allow command line override */
#define OPENTHREAD_CONFIG_LOG_OUTPUT OPENTHREAD_CONFIG_LOG_OUTPUT_NONE
#endif

/**
 * @def OPENTHREAD_CONFIG_LOG_LEVEL
 *
 * The log level (used at compile time). If `OPENTHREAD_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL`
 * is set, this defines the most verbose log level possible. See
 *`OPENTHREAD_CONFIG_INITIAL_LOG_LEVEL` to set the initial log level.
 *
 */
#ifndef OPENTHREAD_CONFIG_LOG_LEVEL
#define OPENTHREAD_CONFIG_LOG_LEVEL OT_LOG_LEVEL_NONE
#endif

/**
 * @def OPENTHREAD_CONFIG_PLATFORM_INFO
 *
 * The platform-specific string to insert into the OpenThread version string.
 *
 */
#define OPENTHREAD_CONFIG_PLATFORM_INFO "RW610N"

/**
 * @def OPENTHREAD_SETTINGS_RAM
 *
 *  No flash on RW610N
 *
 */
#define OPENTHREAD_SETTINGS_RAM 1

/**
 * @def OPENTHREAD_CONFIG_MLE_LINK_METRICS_INITIATOR_ENABLE
 *
 * Define as 1 to enable Link Metrics initiator feature.
 *
 */
#ifndef OPENTHREAD_CONFIG_MLE_LINK_METRICS_INITIATOR_ENABLE
#define OPENTHREAD_CONFIG_MLE_LINK_METRICS_INITIATOR_ENABLE 1
#endif

/**
 * @def OPENTHREAD_CONFIG_MLE_LINK_METRICS_SUBJECT_ENABLE
 *
 * Define as 1 to enable Link Metrics subject feature.
 *
 */
#ifndef OPENTHREAD_CONFIG_MLE_LINK_METRICS_SUBJECT_ENABLE
#define OPENTHREAD_CONFIG_MLE_LINK_METRICS_SUBJECT_ENABLE 1
#endif

/**
 * @def OPENTHREAD_CONFIG_DUA_ENABLE
 *
 * Define as 1 to support Thread 1.2 Domain Unicast Address feature.
 *
 */
#ifndef OPENTHREAD_CONFIG_DUA_ENABLE
#define OPENTHREAD_CONFIG_DUA_ENABLE 1
#endif

/**
 * @def OPENTHREAD_CONFIG_MLR_ENABLE
 *
 * Define as 1 to support Thread 1.2 Multicast Listener Registration feature.
 *
 */
#ifndef OPENTHREAD_CONFIG_MLR_ENABLE
#define OPENTHREAD_CONFIG_MLR_ENABLE 1
#endif

/**
 * @def OPENTHREAD_CONFIG_PLATFORM_USEC_TIMER_ENABLE
 *
 * Define to 1 if you want to enable microsecond backoff timer implemented in platform.
 *
 */
#ifndef OPENTHREAD_CONFIG_PLATFORM_USEC_TIMER_ENABLE
#define OPENTHREAD_CONFIG_PLATFORM_USEC_TIMER_ENABLE 1
#endif

/**
 * @def OPENTHREAD_CONFIG_MAC_SOFTWARE_TX_TIMING_ENABLE
 *
 * Define to 1 to enable software transmission target time logic.
 *
 */
#ifndef OPENTHREAD_CONFIG_MAC_SOFTWARE_TX_TIMING_ENABLE
#define OPENTHREAD_CONFIG_MAC_SOFTWARE_TX_TIMING_ENABLE 1
#endif

/* The  accuracy, in units of +/- ppm, of the clock used for scheduling CSL operations */
#ifndef CONFIG_PLATFORM_CSL_ACCURACY
#define CONFIG_PLATFORM_CSL_ACCURACY 40
#endif

/* Should cover Tx tune time (warm-up) + encryption time +
   us timer inaccuracy (it uses ticks ~= 30.5us) + RCP communication overhead */
/**
 * @def OPENTHREAD_CONFIG_MAC_CSL_REQUEST_AHEAD_US
 *
 * Define how many microseconds ahead should MAC deliver CSL frame to SubMac.
 *
 */
#ifndef OPENTHREAD_CONFIG_MAC_CSL_REQUEST_AHEAD_US
#define OPENTHREAD_CONFIG_MAC_CSL_REQUEST_AHEAD_US 2000
#endif

/**
 * @def OPENTHREAD_CONFIG_MAC_SOFTWARE_TX_SECURITY_ENABLE
 *
 * Define to 1 to enable software transmission security logic.
 *
 */
#ifndef OPENTHREAD_CONFIG_MAC_SOFTWARE_TX_SECURITY_ENABLE
#define OPENTHREAD_CONFIG_MAC_SOFTWARE_TX_SECURITY_ENABLE 0
#endif

/**
 * @def OPENTHREAD_CONFIG_MAC_SOFTWARE_RETRANSMIT_ENABLE
 *
 * Define to 1 if you want to enable software retransmission logic.
 *
 */
#define OPENTHREAD_CONFIG_MAC_SOFTWARE_RETRANSMIT_ENABLE 1

/**
 * @def OPENTHREAD_CONFIG_MAC_SOFTWARE_CSMA_BACKOFF_ENABLE
 *
 * Define to 1 if you want to enable software CSMA-CA backoff logic.
 *
 */
#define OPENTHREAD_CONFIG_MAC_SOFTWARE_CSMA_BACKOFF_ENABLE 1

/**
 * @def OPENTHREAD_CONFIG_NCP_TX_BUFFER_SIZE
 *
 * The size of NCP message buffer in bytes.
 *
 */
#define OPENTHREAD_CONFIG_NCP_TX_BUFFER_SIZE 4096

/**
 * @def OPENTHREAD_CONFIG_NUM_MESSAGE_BUFFERS
 *
 * The number of message buffers in the buffer pool.
 *
 */
#define OPENTHREAD_CONFIG_NUM_MESSAGE_BUFFERS 160

/**
 * @def OPENTHREAD_CONFIG_PLATFORM_RADIO_COEX_ENABLE
 *
 * Define to 1 if you want to enable radio coexistence implemented in platform.
 *
 */
#ifndef OPENTHREAD_CONFIG_PLATFORM_RADIO_COEX_ENABLE
#define OPENTHREAD_CONFIG_PLATFORM_RADIO_COEX_ENABLE 1
#endif

#endif // OPENTHREAD_CORE_RW610N_CONFIG_H_
