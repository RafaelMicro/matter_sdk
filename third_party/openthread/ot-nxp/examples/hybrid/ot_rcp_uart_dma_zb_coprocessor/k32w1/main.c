/*! *********************************************************************************
 *
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * \file
 *
 * This file is the source file for the ot_rcp_uart_dma_zb_coprocessor_app
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
#include "app_uart.h"
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
/* define this to overwrite the default UART interface used by Open Thread
 * uint8_t gOtAppUartInstance = 1;
 */
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
extern void otAppNcpInit(otInstance *aInstance);
extern void vAppMain(bool_t bColdStart);
extern void APP_taskAtSerial(void);
extern void APP_SerialCmdTask(void);

/* DualPAN stuff */
// extern void sched_enable(void);
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

        otAppNcpInit(gpOpenThreadInstance);

        App_ZB_Init();

        DBG_vPrintf(1, "\r\n OT RCP + ZC app \r\n");

        /*
         * Effectively let the other protocol use
         * the medium exclusively
         */
        // APP_vSetSchedPolicy(ZB_PROTO_ID, E_STATE_ZB_APP_INACTIVE);
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
        /* Device was previously in a network, restore policy */
        /* if (APP_eGetCurrentApplicationState() >= E_RUNNING)
        {
            APP_vSetSchedPolicy(ZB_PROTO_ID, E_STATE_ZB_APP_ACTIVE);
        } */
        // BDB_vStart();

        initialized = TRUE;
    }

    while (1)
    {
        /* place event handler code here... */
        APP_vRunZigbee();
        ZTIMER_vTask();
        APP_ZpsEventTask();
        APP_taskAtSerial();
        APP_SerialCmdTask();

        /* If ZB can go to sleep, just change the policy and mark that ZB is sleeping */
        /* if (PWR_CheckIfDeviceCanGoToSleep())
        {
            APP_vSetSchedPolicy(ZB_PROTO_ID, E_STATE_ZB_APP_INACTIVE);
            gZBSleeping = TRUE;
        } */

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

    UART_vInit(NULL);

    APP_vInitialiseCoordinator();

    /* Signal to LPC that I'm ready to receive serial commands */
    extern uint8 u8JNReadyForCmds;
    u8JNReadyForCmds = 1;
}
/*! *********************************************************************************
+ * @}
+ ********************************************************************************** */
