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
 *   This file implements the OpenThread platform abstraction for the alarm.
 *
 */

#include "EmbeddedTypes.h"
#include "PhyInterface.h"
#include "fsl_device_registers.h"
#include <stdint.h>

#include <openthread/platform/alarm-milli.h>
#include <openthread/platform/diag.h>

static phyTimeEvent_t   timerEvent;
static phyTimeTimerId_t timerId;
static bool_t           expired = FALSE;

static void timerCallback(uint32_t param)
{
    expired = TRUE;
}

static uint32_t timestamp_to_ms(uint64_t timestamp)
{
    return (((uint64_t)timestamp * 16) / 1000);
}

static uint32_t ms_to_timestamp(uint32_t ms)
{
    return (((uint64_t)ms * 1000) / 16);
}

void otPlatAlarmInit(void)
{
    timerEvent.timestamp = 0U;
    timerEvent.callback  = timerCallback;
    timerEvent.parameter = 0U;
}

void otPlatAlarmProcess(otInstance *aInstance)
{
    if (expired == TRUE)
    {
        expired = FALSE;
#if OPENTHREAD_CONFIG_DIAG_ENABLE
        if (otPlatDiagModeGet())
        {
            otPlatDiagAlarmFired(aInstance);
        }
        else
#endif
        {
            otPlatAlarmMilliFired(aInstance);
        }
    }
}

void otPlatAlarmMilliStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
    OT_UNUSED_VARIABLE(aInstance);

    timerEvent.timestamp = ms_to_timestamp(aT0);
    timerEvent.timestamp += ms_to_timestamp(aDt);
    timerEvent.callback  = timerCallback;
    timerEvent.parameter = 0U;
    PhyTime_CancelEvent(timerId);
    timerId = PhyTime_ScheduleEvent(&timerEvent);
}

void otPlatAlarmMilliStop(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    PhyTime_CancelEvent(timerId);
}

uint32_t otPlatAlarmMilliGetNow(void)
{
    return timestamp_to_ms(PhyTime_GetTimestamp());
}
