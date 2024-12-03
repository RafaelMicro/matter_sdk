/*
 *
 *    Copyright (c) 2020-2022 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *          Platform-specific configuration overrides for CHIP on
 *          Silcon Labs EFR32 platforms.
 */

#pragma once

#include <stdint.h>
#include "cm3_mcu.h"

// ==================== General Platform Adaptations ====================

#define NL_ASSERT_ABORT()   Sys_Software_Reset()
#define CHIP_CONFIG_ABORT() Sys_Software_Reset()

#define CHIP_CONFIG_PERSISTED_STORAGE_KEY_TYPE uint16_t
#define CHIP_CONFIG_PERSISTED_STORAGE_ENC_MSG_CNTR_ID 1
#define CHIP_CONFIG_PERSISTED_STORAGE_MAX_KEY_LENGTH 2

#define CHIP_CONFIG_LIFETIIME_PERSISTED_COUNTER_KEY 0x01

// ==================== Security Adaptations ====================
#if CHIP_HAVE_CONFIG_H
//#include <crypto/CryptoBuildConfig.h>
#endif

// ==================== General Configuration Overrides ====================

#ifndef CHIP_CONFIG_MAX_UNSOLICITED_MESSAGE_HANDLERS
#define CHIP_CONFIG_MAX_UNSOLICITED_MESSAGE_HANDLERS 8
#endif // CHIP_CONFIG_MAX_UNSOLICITED_MESSAGE_HANDLERS

#ifndef CHIP_CONFIG_MAX_EXCHANGE_CONTEXTS
#define CHIP_CONFIG_MAX_EXCHANGE_CONTEXTS 16
#endif // CHIP_CONFIG_MAX_EXCHANGE_CONTEXTS

#ifndef CHIP_LOG_FILTERING
#define CHIP_LOG_FILTERING 0
#endif // CHIP_LOG_FILTERING

/**
 *  @def CHIP_CONFIG_MAX_FABRICS
 *
 *  @brief
 *    Maximum number of fabrics the device can participate in.  Each fabric can
 *    provision the device with its unique operational credentials and manage
 *    its own access control lists.
 */
#ifndef CHIP_CONFIG_MAX_FABRICS
#define CHIP_CONFIG_MAX_FABRICS 5 // 4 fabrics + 1 for rotation slack
#endif

// ==================== FreeRTOS Configuration Overrides ====================
#ifndef CHIP_CONFIG_FREERTOS_USE_STATIC_TASK
#define CHIP_CONFIG_FREERTOS_USE_STATIC_TASK 1
#endif

#ifndef CHIP_CONFIG_FREERTOS_USE_STATIC_QUEUE
#define CHIP_CONFIG_FREERTOS_USE_STATIC_QUEUE 1
#endif


#ifndef CHIP_DEVICE_CONFIG_SED_IDLE_INTERVAL
#define CHIP_DEVICE_CONFIG_SED_IDLE_INTERVAL chip::System::Clock::Milliseconds32(1000)
#endif

#ifndef CHIP_DEVICE_CONFIG_SED_ACTIVE_INTERVAL
#define CHIP_DEVICE_CONFIG_SED_ACTIVE_INTERVAL chip::System::Clock::Milliseconds32(200)
#endif

#ifndef CHIP_DEVICE_CONFIG_ICD_SLOW_POLL_INTERVAL
#define CHIP_DEVICE_CONFIG_ICD_SLOW_POLL_INTERVAL CHIP_DEVICE_CONFIG_SED_IDLE_INTERVAL
#endif

#ifndef CHIP_DEVICE_CONFIG_ICD_FAST_POLL_INTERVAL
#define CHIP_DEVICE_CONFIG_ICD_FAST_POLL_INTERVAL CHIP_DEVICE_CONFIG_SED_ACTIVE_INTERVAL
#endif

#ifndef CHIP_CONFIG_ICD_IDLE_MODE_INTERVAL_SEC
#define CHIP_CONFIG_ICD_IDLE_MODE_INTERVAL_SEC 60
#endif

//minimum 300ms
#ifndef CHIP_CONFIG_ICD_ACTIVE_MODE_INTERVAL_MS
#define CHIP_CONFIG_ICD_ACTIVE_MODE_INTERVAL_MS 300
#endif

//minimum 300ms
#ifndef CHIP_CONFIG_ICD_ACTIVE_MODE_THRESHOLD_MS
#define CHIP_CONFIG_ICD_ACTIVE_MODE_THRESHOLD_MS 300
#endif

#ifndef CHIP_CONFIG_ICD_CLIENTS_SUPPORTED_PER_FABRIC
#define CHIP_CONFIG_ICD_CLIENTS_SUPPORTED_PER_FABRIC 2
#endif

#define CHIP_DISPATCH_EVENT_LONG_DISPATCH_TIME_WARNING_THRESHOLD_MS 500

#define CHIP_CONFIG_VERBOSE_VERIFY_OR_DIE 1