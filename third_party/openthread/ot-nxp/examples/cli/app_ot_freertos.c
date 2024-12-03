/*
 *  Copyright (c) 2021, The OpenThread Authors.
 *  Copyright (c) 2022-2023, NXP.
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
 * This file implements functions to run the cli on freertos
 *
 * This file is just for example, but not for production.
 *
 */

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include "openthread-system.h"
#include <openthread-core-config.h>
#include <openthread/cli.h>
#include <openthread/diag.h>
#include <openthread/tasklet.h>

#include <assert.h>
#include <string.h>

#include "addons_cli.h"
#include "app_ot.h"

#ifndef OT_MAIN_TASK_PRIORITY
#define OT_MAIN_TASK_PRIORITY 3
#endif

#ifndef OT_MAIN_TASK_SIZE
#define OT_MAIN_TASK_SIZE ((configSTACK_DEPTH_TYPE)8192 / sizeof(portSTACK_TYPE))
#endif

#if configAPPLICATION_ALLOCATED_HEAP
uint8_t __attribute__((section(".heap"))) ucHeap[configTOTAL_HEAP_SIZE];
#endif

static otInstance       *sInstance      = NULL;
static TaskHandle_t      sMainTask      = NULL;
static SemaphoreHandle_t sMainStackLock = NULL;

extern void otAppCliInit(otInstance *aInstance);
extern void otSysRunIdleTask(void);

static void appOtInit()
{
#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
    size_t   otInstanceBufferLength = 0;
    uint8_t *otInstanceBuffer       = NULL;
#endif

    otSysInit(0, NULL);

#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
    // Call to query the buffer size
    (void)otInstanceInit(NULL, &otInstanceBufferLength);

    // Call to allocate the buffer
    otInstanceBuffer = (uint8_t *)pvPortMalloc(otInstanceBufferLength);
    assert(otInstanceBuffer);

    // Initialize OpenThread with the buffer
    sInstance = otInstanceInit(otInstanceBuffer, &otInstanceBufferLength);
#else
    sInstance = otInstanceInitSingle();
#endif

#if OPENTHREAD_ENABLE_DIAG
    otDiagInit(sInstance);
#endif
    /* Init the CLI */
    otAppCliInit(sInstance);
    /* Init CLI addons */
    otAppCliAddonsInit(sInstance);
}

static void mainloop(void *aContext)
{
    OT_UNUSED_VARIABLE(aContext);
    appOtInit();

    otSysProcessDrivers(sInstance);
    while (!otSysPseudoResetWasRequested())
    {
        /* Aqquired the task mutex lock and release after OT processing is done */
        appOtLockOtTask(true);
        otTaskletsProcess(sInstance);
        otSysProcessDrivers(sInstance);
        appOtLockOtTask(false);

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }

    otInstanceFinalize(sInstance);
    vTaskDelete(NULL);
}

void appOtLockOtTask(bool bLockState)
{
    if (bLockState)
    {
        /* Aqquired the task mutex lock */
        xSemaphoreTakeRecursive(sMainStackLock, portMAX_DELAY);
    }
    else
    {
        /* Release the task mutex lock */
        xSemaphoreGiveRecursive(sMainStackLock);
    }
}

void appOtStart(int argc, char *argv[])
{
    sMainStackLock = xSemaphoreCreateRecursiveMutex();
    assert(sMainStackLock != NULL);

    xTaskCreate(mainloop, "ot", OT_MAIN_TASK_SIZE, NULL, OT_MAIN_TASK_PRIORITY, &sMainTask);
}

void otTaskletsSignalPending(otInstance *aInstance)
{
    (void)aInstance;

    if (sMainTask != NULL)
    {
        xTaskNotifyGive(sMainTask);
    }
}

void otSysEventSignalPending(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (sMainTask != NULL)
    {
        vTaskNotifyGiveFromISR(sMainTask, &xHigherPriorityTaskWoken);
        /* Context switch needed? */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

#if OPENTHREAD_CONFIG_HEAP_EXTERNAL_ENABLE
void *otPlatCAlloc(size_t aNum, size_t aSize)
{
    size_t total_size = aNum * aSize;
    void  *ptr        = pvPortMalloc(total_size);
    if (ptr)
    {
        memset(ptr, 0, total_size);
    }

    return ptr;
}

void otPlatFree(void *aPtr)
{
    vPortFree(aPtr);
}
#endif

#if OPENTHREAD_CONFIG_LOG_OUTPUT == OPENTHREAD_CONFIG_LOG_OUTPUT_APP
void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, ...)
{
    va_list ap;

    va_start(ap, aFormat);
    otCliPlatLogv(aLogLevel, aLogRegion, aFormat, ap);
    va_end(ap);
}
#endif
