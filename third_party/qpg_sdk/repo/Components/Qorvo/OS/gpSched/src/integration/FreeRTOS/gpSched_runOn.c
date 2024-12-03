/*
 * Copyright (c) 2020-2022, Qorvo Inc
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 * CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 * LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * QORVO INC. SHALL NOT, IN ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 * FOR ANY REASON WHATSOEVER.
 *
 * Alternatively, this software may be distributed under the terms of the
 * modified BSD License or the 3-clause BSD License as published by the Free
 * Software Foundation @ https://directory.fsf.org/wiki/License:BSD-3-Clause
 *
 */

/**
 * @file gpSched_runOn.c
 * @brief GP Scheduler extern function run on gpScheduler task implementation
 *
 * This file implements the function enable run extern function on gpscheduler task
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_SCHED

#include "hal.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include "gpSched.h"
#include "gpSched_defs.h"

#include "gpLog.h"
#include "gpSched_runOn.h"
/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/


/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
typedef struct {
    gpSched_RunOnCallback_t     fn;
    void *                      arg;
    uint32_t                    status;
} runOnData_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
static SemaphoreHandle_t mainLoopReachedSemaphore = NULL;
static SemaphoreHandle_t runOnDoneMutex = NULL;
static SemaphoreHandle_t runOnFunctionMutex = NULL;

#if configSUPPORT_DYNAMIC_ALLOCATION == 0
static StaticSemaphore_t mainLoopReachedSemaphoreBuffer;
static StaticSemaphore_t runOnDoneMutexBuffer;
static StaticSemaphore_t runOnFunctionMutexBuffer;
#endif
/*****************************************************************************
 *                    External Function Prototypes
 *****************************************************************************/


/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
void runOn_executor(void* arg)
{
    if (arg != NULL)
    {
        GP_LOG_PRINTF("runon executor fn addr:%lx", 0, (uint32_t)((runOnData_t *)arg)->fn);
        // run scheduled function from scheduler task
        ((runOnData_t *)arg)->status = ((runOnData_t *)arg)->fn(((runOnData_t *)arg)->arg);
        // wait for main loop semaphore to take it if it is possible (wait time 0)
        xSemaphoreTake(mainLoopReachedSemaphore, 0);
        // function dane mutex unlock
        xSemaphoreGive(runOnDoneMutex);
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
void gpSched_RunOnInit(void)
{
#if configSUPPORT_DYNAMIC_ALLOCATION
    mainLoopReachedSemaphore = xSemaphoreCreateBinary();
    runOnDoneMutex = xSemaphoreCreateBinary();
    runOnFunctionMutex = xSemaphoreCreateMutex();
#else    // Create static buffer for mutex memory outside of function
    mainLoopReachedSemaphore = xSemaphoreCreateBinaryStatic(&mainLoopReachedSemaphoreBuffer);
    runOnDoneMutex = xSemaphoreCreateBinaryStatic(&runOnDoneMutexBuffer);
    runOnFunctionMutex = xSemaphoreCreateMutexStatic(&runOnFunctionMutexBuffer);
#endif //configSUPPORT_DYNAMIC_ALLOCATION

    if (mainLoopReachedSemaphore == NULL || runOnDoneMutex == NULL || runOnFunctionMutex == NULL)
    {
        GP_ASSERT_DEV_EXT(false);
    }
}

uint32_t gpSched_RunOn(gpSched_RunOnCallback_t externFunction, void* arg)
{
    runOnData_t runOnData = {
        .fn = externFunction,
        .arg = arg,
        .status = 0,
    };
    // wait for function protect mutex lock
    xSemaphoreTakeRecursive(runOnFunctionMutex, portMAX_DELAY);
    // schedule function to run on scheduler task
    gpSched_ScheduleEventArg(0, runOn_executor, &runOnData);
    // wait for mutex function done
    xSemaphoreTake(runOnDoneMutex, portMAX_DELAY);
    // wait for semaphore main loop reached
    xSemaphoreTake(mainLoopReachedSemaphore, portMAX_DELAY);
    // function finish mutex unlock
    xSemaphoreGiveRecursive(runOnFunctionMutex);
    return runOnData.status;
}

void gpSched_RunOnMainLoopReached(void)
{
    xSemaphoreGive(mainLoopReachedSemaphore);
}
