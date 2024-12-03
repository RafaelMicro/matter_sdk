/*
 *  Copyright (c) 2022, NXP
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
 * This file implements FreeRTOS hooks that can be enabled through FreeRTOSConfig.h
 *
 * This file is just for example, but not for production.
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "FreeRTOS.h"
#include "assert.h"
#include "fsl_common.h"
#include "fwk_platform.h"
#include "ot_platform_common.h"
#include "task.h"

#ifdef OT_APP_CLI_LOWPOWER_ADDON
#include "PWR_Interface.h"
#endif

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

#define US_TO_TICK(us) (TickType_t)((uint64_t)(us) / ((uint64_t)portTICK_PERIOD_MS * (uint64_t)1000U))

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */
void vApplicationIdleHook(void)
{
    uint32_t   irqMask;
    uint64_t   preOtIdleTaskTimeStampUs;
    uint64_t   postOtIdleTaskTimeStampUs;
    uint64_t   otIdleTaskDurationUs;
    TickType_t otIdleTaskDurationTick;
    TickType_t preOtIdleTaskTickCount;
    TickType_t postOtIdleTaskTickCount;

    /* The following implementation aims to mitigate the impact of saving to flash on the FreeRTOS time base
     * When saving to flash, the whole system is blocked and depending on the flash and the operation it can take more
     * than 1 tick. During this time, the FreeRTOS timebase is not incremented so the whole scheduled activity is
     * shifted in time. This can cause problems with the FreeRTOS timers. FreeRTOS provides the API xTaskCatchUpTicks
     * for this exact type of scenario.
     * Here, we measure the time elapsed during the otSysRunIdleTask execution using SOC timers and we convert this
     * time to ticks. The kernel is then notified with xTaskCatchUpTicks about those missing ticks.
     * */
    irqMask                  = DisableGlobalIRQ();
    preOtIdleTaskTickCount   = xTaskGetTickCount();
    preOtIdleTaskTimeStampUs = PLATFORM_GetTimeStamp();
    EnableGlobalIRQ(irqMask);

    /* We let the interrupts unmasked during the idle task to we can still get pre-empted by the OS and interrupts.
     * We'll use the preOtIdleTaskTickCount and postOtIdleTaskTickCount to measure the ticks taken into account by
     * FreeRTOS during this time, and substract it from the overall duration measured with the SOC timers. */
    otSysRunIdleTask();

    irqMask                   = DisableGlobalIRQ();
    postOtIdleTaskTickCount   = xTaskGetTickCount();
    postOtIdleTaskTimeStampUs = PLATFORM_GetTimeStamp();

    if (postOtIdleTaskTimeStampUs < preOtIdleTaskTimeStampUs)
    {
        /* Handle wrap */
        otIdleTaskDurationUs = PLATFORM_GetMaxTimeStamp() - preOtIdleTaskTimeStampUs + postOtIdleTaskTimeStampUs;
    }
    else
    {
        otIdleTaskDurationUs = postOtIdleTaskTimeStampUs - preOtIdleTaskTimeStampUs;
    }

    otIdleTaskDurationTick = US_TO_TICK(otIdleTaskDurationUs);

    if (otIdleTaskDurationTick > 0U)
    {
        TickType_t tmpTickCount;

        /* The otSysRunIdleTask function was called with interrupts unmasked, meaning the systicks interrupts could
         * execute and increase the tick count. We have to consider the number of tick counts taken into account by
         * these interrupts and substract this duration from the overall duration measured with the SOC timers.
         * If we don't do this, we may pass a higher tick count to xTaskCatchUpTicks than expected. */
        if (postOtIdleTaskTickCount < preOtIdleTaskTickCount)
        {
            /* Tick count wrapped */
            tmpTickCount = portMAX_DELAY - preOtIdleTaskTickCount + postOtIdleTaskTickCount;
        }
        else
        {
            tmpTickCount = postOtIdleTaskTickCount - preOtIdleTaskTickCount;
        }

        if (otIdleTaskDurationTick > tmpTickCount)
        {
            otIdleTaskDurationTick -= tmpTickCount;
            /* This function corrects the tick count value after the application code has held
             * interrupts disabled for an extended period resulting in tick interrupts having
             * been missed.
             *
             * This function is similar to vTaskStepTick(), however, unlike
             * vTaskStepTick(), xTaskCatchUpTicks() may move the tick count forward past a
             * time at which a task should be removed from the blocked state.  That means
             * tasks may have to be removed from the blocked state as the tick count is
             * moved. */
            (void)xTaskCatchUpTicks(otIdleTaskDurationTick);
        }
    }

    EnableGlobalIRQ(irqMask);
#ifdef WIFI_IDLE_HOOK_CALL_REQUIRED
    extern void vApplicationIdleHook_wifi(void);
    vApplicationIdleHook_wifi();
#endif
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    assert(0);
}

void vApplicationMallocFailedHook(void)
{
    assert(0);
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
#ifdef OT_APP_CLI_LOWPOWER_ADDON
    bool     abortIdle = false;
    uint64_t expectedIdleTimeUs, actualIdleTimeUs;

    uint32_t irqMask = DisableGlobalIRQ();

    /* Disable and prepare systicks for low power */
    abortIdle = PWR_SysticksPreProcess((uint32_t)xExpectedIdleTime, &expectedIdleTimeUs);

    if (abortIdle == false)
    {
        /* Enter low power with a maximal timeout */
        actualIdleTimeUs = PWR_EnterLowPower(expectedIdleTimeUs);

        /* Re enable systicks and compensate systick timebase */
        PWR_SysticksPostProcess(expectedIdleTimeUs, actualIdleTimeUs);
    }

    /* Exit from critical section */
    EnableGlobalIRQ(irqMask);
#endif
}
