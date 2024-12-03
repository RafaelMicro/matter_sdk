/*
 *  Copyright (c) 2022, The OpenThread Authors.
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

#include "EmbeddedTypes.h"
#include "fsl_component_timer_manager.h"

#include "openthread-system.h"
#include <common/logging.hpp>
#include <openthread/platform/alarm-micro.h>
#include <openthread/platform/diag.h>

static bool_t sEventMicroFired = FALSE;
TIMER_MANAGER_HANDLE_DEFINE(sAlarmMicroTimerHandle);

static void timerMicroCallback(void *param)
{
    sEventMicroFired = TRUE;
    otSysEventSignalPending();
}

void MicroAlarmInit(void)
{
    (void)TM_Open((timer_handle_t)sAlarmMicroTimerHandle);
    (void)TM_InstallCallback((timer_handle_t)sAlarmMicroTimerHandle, (timer_callback_t)timerMicroCallback, NULL);
}

uint32_t otPlatAlarmMicroGetNow(void)
{
    return TM_GetTimestamp();
}

void otPlatAlarmMicroStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
    /* Need support for us timer in SDK. Untill then we contiune to use ms timer and devide input
       time to 1000 */
    OT_UNUSED_VARIABLE(aInstance);

    TM_Stop(sAlarmMicroTimerHandle);

    /* Calculate the difference between now and the requested timestamp aT0 - this time will be
       substracted from the total time until the event needs to fire */
    uint32_t timestamp = TM_GetTimestamp();

    otLogInfoPlat("Start timer: timestamp:%lu, aTo:%lu, aDt:%lu", timestamp, aT0, aDt);
    if (timestamp >= aT0)
    {
        timestamp = timestamp - aT0;
    }
    else
    {
        timestamp = (~0UL) - aT0 + timestamp + 1;
    }

    if (aDt >= timestamp)
    {
        aDt -= timestamp;
    }

    if (aDt > 0)
    {
        TM_Start(sAlarmMicroTimerHandle, kTimerModeSingleShot | kTimerModeSetMicrosTimer, aDt);
    }
    else
    {
        sEventMicroFired = true;
        otSysEventSignalPending();
    }
}

void otPlatAlarmMicroStop(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    TM_Stop(sAlarmMicroTimerHandle);
}
