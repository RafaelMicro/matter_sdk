
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

#include "token_bucket.h"

#include <assert.h>

/* -------------------------------------------------------------------------- */
/*                                 Definitions                                */
/* -------------------------------------------------------------------------- */

#define TO_FRACTIONS(val) ((val)*1000U)
#define MAX_RATE (0xFFFFFFFFU / 1000U)

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static void otTokenBucketCallback(TimerHandle_t xTimer);

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void otTokenBucketInit(otTokenBucket *aBucket, uint32_t rate)
{
    uint32_t timerPeriodTicks;

    assert(aBucket != NULL);
    assert(rate != 0U);
    assert(rate <= MAX_RATE);

    aBucket->mutex = xSemaphoreCreateMutex();
    assert(aBucket->mutex != NULL);

    /* Rate and tokens are internally stored as thousandths to have "three decimal places" */
    aBucket->rate   = TO_FRACTIONS(rate);
    aBucket->tokens = aBucket->rate;

    /* Period in RTOS ticks after which token(s) should be added */
    timerPeriodTicks = configTICK_RATE_HZ / rate;
    if (timerPeriodTicks == 0U)
    {
        timerPeriodTicks = 1U;
    }

    /* Set token increment to ((rate per second) / (timer period in Hz)) */
    aBucket->increment = aBucket->rate / (configTICK_RATE_HZ / timerPeriodTicks);

    aBucket->timer = xTimerCreate("tokenTimer", timerPeriodTicks, pdTRUE, (void *)aBucket, otTokenBucketCallback);
    assert(aBucket->timer != NULL);

    assert(xTimerStart(aBucket->timer, 0) == pdPASS);
}

uint32_t otTokenBucketTake(otTokenBucket *aBucket, uint32_t tokens)
{
    uint32_t fractionsToTake;
    uint32_t tokensTaken = 0U;

    assert(aBucket != NULL);
    assert(tokens <= MAX_RATE);

    fractionsToTake = TO_FRACTIONS(tokens);

    xSemaphoreTake(aBucket->mutex, portMAX_DELAY);
    if (fractionsToTake <= aBucket->tokens)
    {
        aBucket->tokens -= fractionsToTake;
        tokensTaken = tokens;
    }
    xSemaphoreGive(aBucket->mutex);

    return tokensTaken;
}

bool otTokenBucketCanTake(otTokenBucket *aBucket, uint32_t tokens)
{
    uint32_t fractionsToTake;
    bool     canTake = false;

    assert(aBucket != NULL);
    assert(tokens <= MAX_RATE);

    fractionsToTake = TO_FRACTIONS(tokens);

    xSemaphoreTake(aBucket->mutex, portMAX_DELAY);
    if (fractionsToTake <= aBucket->tokens)
    {
        canTake = true;
    }
    xSemaphoreGive(aBucket->mutex);

    return canTake;
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static void otTokenBucketCallback(TimerHandle_t xTimer)
{
    otTokenBucket *aBucket;
    uint32_t       newTokens;

    aBucket = (otTokenBucket *)pvTimerGetTimerID(xTimer);

    xSemaphoreTake(aBucket->mutex, portMAX_DELAY);

    if (aBucket->tokens != aBucket->rate)
    {
        /* Bucket not full at the moment */
        newTokens = aBucket->tokens += aBucket->increment;

        if (newTokens < aBucket->tokens)
        {
            /* Integral overflow, set the bucket to the full */
            aBucket->tokens = aBucket->rate;
        }
        else if (newTokens >= aBucket->rate)
        {
            /* Would exceed maximum tokens, set the bucket to the full */
            aBucket->tokens = aBucket->rate;
        }
        else
        {
            aBucket->tokens = newTokens;
        }
    }

    xSemaphoreGive(aBucket->mutex);
}
