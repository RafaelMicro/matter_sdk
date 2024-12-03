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

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <assert.h>
#include <string.h>

#include "fsl_common.h"

#include <openthread/cli.h>
#include <openthread/error.h>
#include <openthread/instance.h>

#include "FreeRTOS.h"
#include "timers.h"

#include "PWR_Interface.h"

/* -------------------------------------------------------------------------- */
/*                             Private definitions                            */
/* -------------------------------------------------------------------------- */

#define LOWPOWER_DEFAULT_ENABLE_DURATION_MS (10000 / portTICK_PERIOD_MS)

/* -------------------------------------------------------------------------- */
/*                          Private type definitions                          */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static void otAppLowPowerCliDisplayUsage(void);
static void otAppLowPowerCliTimerCallback(TimerHandle_t timer);
static void otAppLowPowerCliConfigureNextMode(PWR_LowpowerMode_t nextMode);

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static PWR_LowpowerMode_t currentMode;
static TimerHandle_t      lpTimer      = NULL;
static uint32_t           lpDurationMs = LOWPOWER_DEFAULT_ENABLE_DURATION_MS;

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void otAppLowPowerCliInit(otInstance *aInstance)
{
    /* Initial low power mode is WFI */
    currentMode = PWR_WFI;

    /* Create FreeRTOS timer which will be used to disable low power after a specific time */
    lpTimer = xTimerCreate("LP timer", lpDurationMs / portTICK_PERIOD_MS, pdFALSE, NULL, otAppLowPowerCliTimerCallback);
    assert(lpTimer != NULL);
}

otError ProcessLowPower(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    otError            status        = OT_ERROR_NONE;
    int                arg           = 0;
    PWR_LowpowerMode_t nextMode      = currentMode;
    uint32_t           newDurationMs = lpDurationMs;

    struct
    {
        unsigned help : 1;
        unsigned mode : 1;
        unsigned time : 1;
    } info;

    (void)memset(&info, 0, sizeof(info));

    do
    {
        if (aArgsLength == 0)
        {
            otAppLowPowerCliDisplayUsage();
            break;
        }

        do
        {
            if (!strcmp("-h", aArgs[arg]))
            {
                otAppLowPowerCliDisplayUsage();
                info.help = 1;
                break;
            }
            if (!strcmp("-m", aArgs[arg]))
            {
                info.mode = 1;
                arg += 1;

                if (!strcmp("wfi", aArgs[arg]))
                {
                    nextMode = PWR_WFI;
                }
                else if (!strcmp("sleep", aArgs[arg]))
                {
                    nextMode = PWR_Sleep;
                }
                else if (!strcmp("deepsleep", aArgs[arg]))
                {
                    nextMode = PWR_DeepSleep;
                }
                else if (!strcmp("powerdown", aArgs[arg]))
                {
                    nextMode = PWR_PowerDown;
                }
                else if (!strcmp("deeppowerdown", aArgs[arg]))
                {
                    nextMode = PWR_DeepPowerDown;
                }
                else
                {
                    otCliOutputFormat("Invalid low power mode\r\n");
                    status = OT_ERROR_INVALID_ARGS;
                    break;
                }
                arg += 1;
            }
            else if (!strcmp("-t", aArgs[arg]))
            {
                arg += 1;
                info.time     = 1;
                newDurationMs = strtoul(aArgs[arg], NULL, 10);
                arg += 1;

                /* Update timer duration */
                lpDurationMs = newDurationMs;
                otCliOutputFormat("Timer updated\r\n");
            }
            else
            {
                otAppLowPowerCliDisplayUsage();
                status = OT_ERROR_INVALID_ARGS;
                break;
            }

        } while (arg < aArgsLength);

        if (status != OT_ERROR_NONE)
        {
            break;
        }

        if (info.mode)
        {
            /* Apply next mode constraints */
            otAppLowPowerCliConfigureNextMode(nextMode);

            /* Update timer only when actually enabling low power period */
            (void)xTimerChangePeriod(lpTimer, lpDurationMs / portTICK_PERIOD_MS, 0);

            /* Start the timer, during this time, the configured low power mode will be used as much as possible by
             * the system, when the timer expires, the low power mode will be limited to WFI until next command
             * This is to make sure the serial interface becomes available again to the user */
            if (xTimerStart(lpTimer, 0) != pdPASS)
            {
                assert(0);
            }

            otCliOutputFormat("Select mode will be used for the next %u ms\r\n", lpDurationMs);
        }
        else
        {
            /* nothing to do */
        }

    } while (false);

    return status;
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static void otAppLowPowerCliDisplayUsage(void)
{
    otCliOutputFormat("Usage:\r\n");
    otCliOutputFormat("\tlp [options]\r\n");
    otCliOutputFormat("\r\n");
    otCliOutputFormat("This tool allows to select the maximum low power mode allowed when entering idle\r\n");
    otCliOutputFormat("This low power mode will be used during a specific amount of time (10sec by default)\r\n");
    otCliOutputFormat("After this period, the default low power mode will be switched back to WFI\r\n");
    otCliOutputFormat("This is to ensure the serial interface becomes available again after some time\r\n");
    otCliOutputFormat("\r\n");
    otCliOutputFormat("Options:\r\n");
    otCliOutputFormat("\t-h Display this message\r\n");
    otCliOutputFormat("\t-m [wfi|sleep|deepsleep|powerdown|deeppowerdown] Select low power mode\r\n");
    otCliOutputFormat("\t-t <time> Low power duration (in ms) while the low power mode is used (Default: 10000ms)\r\n");
}

static void otAppLowPowerCliTimerCallback(TimerHandle_t timer)
{
    (void)timer;

    otCliOutputFormat("Low power period ended\r\n");

    /* Timer expired, allow only WFI mode to make sure serial interface is available to the user */
    otAppLowPowerCliConfigureNextMode(PWR_WFI);
}

static void otAppLowPowerCliConfigureNextMode(PWR_LowpowerMode_t nextMode)
{
    PWR_ReleaseLowPowerModeConstraint(currentMode);
    PWR_SetLowPowerModeConstraint(nextMode);
    currentMode = nextMode;

    switch (currentMode)
    {
    case PWR_WFI:
        otCliOutputFormat("WFI");
        break;
    case PWR_Sleep:
        otCliOutputFormat("Sleep");
        break;
    case PWR_DeepSleep:
        otCliOutputFormat("DeepSleep");
        break;
    case PWR_PowerDown:
        otCliOutputFormat("PowerDown");
        break;
    case PWR_DeepPowerDown:
        otCliOutputFormat("DeepPowerDown");
        break;
    default:
        assert(0);
    }

    otCliOutputFormat(" mode selected\r\n", lpDurationMs);
}
