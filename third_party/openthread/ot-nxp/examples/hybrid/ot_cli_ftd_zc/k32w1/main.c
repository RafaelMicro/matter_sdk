/*! *********************************************************************************
 *
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
/* Framework / Drivers */
#include "FunctionLib.h"
#include "PDM.h"
#include "RNG_Interface.h"
#include "SecLib.h"
#include "fsl_debug_console.h"
#include "fsl_flash.h"
#include "fsl_os_abstraction.h"

/* ZB stuff */
#include "app_coordinator.h"
#include "app_main.h"
#include "dbg.h"
#include "zigbee_config.h"

/* OT includes */
#include <openthread-system.h>
#include "openthread/cli.h"
#include "openthread/tasklet.h"
#include "openthread/thread.h"

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static uint8_t platformInitialized = 0;

static otInstance *gpOpenThreadInstance = NULL;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static void App_Thread(uint32_t param);
static void App_ZB_Run(uint32_t param);
static void App_ZB_Init(void);
/************************************************************************************
*************************************************************************************
* Public functions prototypes
*************************************************************************************
************************************************************************************/
void        otAppCliInit(otInstance *aInstance);
extern void UART_vInit(void);
extern void APP_taskAtSerial(void);
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
 * \brief  This is the first task created by the OS. This task will initialize
 *         the system
 *
 * \param[in]  param
 *
 ********************************************************************************** */
void main_task(uint32_t param)
{
    if (!platformInitialized)
    {
        platformInitialized = 1;

        /* Initialize OpenThread timer, RNG and Radio */
        otSysInit(0, NULL);
        gpOpenThreadInstance = otInstanceInitSingle();

        /* Cryptographic and RNG hardware initialization */
        SecLib_Init();

        /* RNG software initialization and PRNG initial seeding (from hardware) */
        RNG_Init();

        /* otAppCliInit() enables UART */
        otAppCliInit(gpOpenThreadInstance);

        App_ZB_Init();

        DBG_vPrintf(1, "\r\n OT + ZC app \r\n");
    }

    /* Call application task */
    App_Thread(param);
}

/*! *********************************************************************************
 * \brief  This function represents the Application task.
 *         This task reuses the stack allocated for the MainThread.
 *         This function is called to process all events for the task. Events
 *         include timers, messages and any other user defined events.
 * \param[in]  argument
 *
 ********************************************************************************** */
void App_Thread(uint32_t param)
{
    while (1)
    {
        otSysProcessDrivers(gpOpenThreadInstance);

        while (otTaskletsArePending(gpOpenThreadInstance))
        {
            otTaskletsProcess(gpOpenThreadInstance);
            otSysProcessDrivers(gpOpenThreadInstance);
        }

        App_ZB_Run(0);

        /* For BareMetal break the while(1) after 1 run */
        if (gUseRtos_c == 0)
        {
            break;
        }
    }
}

/*! *********************************************************************************
 * \brief  This function represents the ZB app portion of the task.
 *         This task reuses the stack allocated for the MainThread.
 *         This function is called to process all events for the task. Events
 *         include timers, messages and any other user defined events.
 * \param[in]  argument
 *
 ********************************************************************************** */
void App_ZB_Run(uint32_t param)
{
    OT_UNUSED_VARIABLE(param);

    /* e.g. osaEventFlags_t ev; */
    static uint8_t initialized = FALSE;

    if (!initialized)
    {
        BDB_vStart();

        initialized = TRUE;
    }

    while (1)
    {
        /* place event handler code here... */
        APP_vRunZigbee();
        ZTIMER_vTask();

        APP_taskCoordinator();
        APP_taskAtSerial();
        if (!gUseRtos_c)
        {
            break;
        }
    }
}

/*! *********************************************************************************
 * \brief  This function represents the ZB app init portion.
 *         This is calling ZB related init functions for timers, buffers,
 *         pwr and pdm components.
 ********************************************************************************** */
void App_ZB_Init(void)
{
    /* place initialization code here... */
    APP_vInitResources();
    APP_vInitZigbeeResources();

    /* Initialise the Persistent Data Manager */
    PDM_eInitialise(1200, 63, NULL);

    UART_vInit();

    APP_vInitialiseCoordinator();
}
/*! *********************************************************************************
 * @}
 ********************************************************************************** */
