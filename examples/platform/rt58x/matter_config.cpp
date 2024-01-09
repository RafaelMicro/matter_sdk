/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2022 Silabs.
 *    All rights reserved.
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

#include "AppConfig.h"
#include "OTAConfig.h"
#include <matter_config.h>

#include <FreeRTOS.h>
#include "task.h"

#include <mbedtls/platform.h>

#ifdef SL_WIFI
#include "wfx_host_events.h"
#endif /* SL_WIFI */

#if PW_RPC_ENABLED
#include "Rpc.h"
#endif

#ifdef ENABLE_CHIP_SHELL
#include "matter_shell.h"
#endif

#ifdef HEAP_MONITORING
#include "MemMonitoring.h"
#endif

using namespace ::chip;
using namespace ::chip::Inet;
using namespace ::chip::DeviceLayer;

#include <crypto/CHIPCryptoPAL.h>
#if CHIP_ENABLE_OPENTHREAD
#include <inet/EndPointStateOpenThread.h>
#include <openthread/cli.h>
#include <openthread/dataset.h>
#include <openthread/error.h>
#include <openthread/heap.h>
#include <openthread/icmp6.h>
#include <openthread/instance.h>
#include <openthread/link.h>
#include <openthread/platform/openthread-system.h>
#include <openthread/tasklet.h>
#include <openthread/thread.h>

// ================================================================================
// Matter Networking Callbacks
// ================================================================================
#endif // CHIP_ENABLE_OPENTHREAD

#include "cm3_mcu.h"
#include "util_log.h"
#include <EnhancedFlashDataset.h>

extern void vPortSetupTimerInterrupt(void);
// ================================================================================
// FreeRTOS Callbacks
// ================================================================================
extern "C" void vApplicationIdleHook(void)
{
    otTaskletsSignalPending(0);
    // FreeRTOS Idle callback
    // Check CHIP Config nvm3 and repack flash if necessary.
    //Internal::RT58xConfig::RepackNvm3Flash();
}

extern "C" void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime_ms)
{
#if CHIP_CONFIG_ENABLE_ICD_SERVER
    TickType_t xModifiableIdleTime;

    __disable_irq();

    if( eTaskConfirmSleepModeStatus() == eAbortSleep )
    {
        __enable_irq();
    }
    else
    {
        timern_t *TIMER;
        uint32_t now_v;

        if(xExpectedIdleTime_ms > 0)
        {
            TIMER = TIMER4;

            TIMER->LOAD = (xExpectedIdleTime_ms * 40) -1;

            TIMER->CLEAR = 1;
            TIMER->CONTROL.bit.INT_ENABLE = 1;
            TIMER->CONTROL.bit.EN = 1;

            Lpm_Enter_Low_Power_Mode();

            TIMER->CONTROL.bit.EN = 0;
            TIMER->CONTROL.bit.INT_ENABLE = 0;
            TIMER->CLEAR = 1;
            Delay_us(250);
            now_v = (TIMER->VALUE/40);

            if(now_v > xExpectedIdleTime_ms)
            {
                now_v = 0;
            }
            xModifiableIdleTime = xExpectedIdleTime_ms - now_v;

            vTaskStepTick( xModifiableIdleTime );

            __enable_irq();
        }
    }
#endif
}