/*! *********************************************************************************
 *
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * \file
 *
 * This file is the source file for the cli_ftd_zb_uart_app
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
#include "MemManager.h"
#include "Messaging.h"
#include "PDM.h"
#include "Panic.h"
#include "RNG_Interface.h"
#include "SecLib.h"
#include "SerialManager.h"
#include "TimersManager.h"
#include "fsl_debug_console.h"
#include "fsl_flash.h"
#include "fsl_os_abstraction.h"
/* ZB stuff */
#include "app_end_device_node.h"
#include "pwrm.h"
#include "zigbee_config.h"

/* OT includes */
#include <openthread-system.h>
#include "openthread/cli.h"
#include "openthread/tasklet.h"
#include "openthread/thread.h"

#include "app_main.h"

#include "app_helpers.h"
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
static char        initString[]         = "app";

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
/* define this to overwrite the default UART interface used by Open Thread
 * uint8_t gOtAppUartInstance = 1;
 */
bool_t gZBSleeping = FALSE;
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
extern void otAppCliInit(otInstance *aInstance);
extern void vAppMain(bool_t bColdStart);
/* DualPAN stuff */
extern void sched_enable(void);
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
        char *argv[1] = {0};
        argv[0]       = &initString[0];

        platformInitialized = 1;

        /* This has to be the first. Bad things happen otherwise */
        PWRM_vColdStart();

        /* Framework init */
        MEM_Init();

        PRINTF("OT + ZB ZED app\r\n");

        /* Cryptographic and RNG hardware initialization */
        SecLib_Init();
        /* RNG software initialization and PRNG initial seeding (from hardware) */
        RNG_Init();
        RNG_SetPseudoRandomNoSeed(NULL);

#if (defined gRadioUsePdm_d && gRadioUsePdm_d)
        PDM_Init();
#endif
#if defined(gMWS_UseCoexistence_d) && (gMWS_UseCoexistence_d)
        otPlatRadioSetCoexEnabled(gpOpenThreadInstance, true);
#endif
        /* Init ZB first */
        App_ZB_Init();

        /* Initialize OpenThread timer, RNG and Radio */
        otSysInit(1, argv);
        gpOpenThreadInstance = otInstanceInitSingle();
        otAppCliInit(gpOpenThreadInstance);

        /* After this point, cannot register a protocol anymore */
        PRINTF("Enabling dual PAN scheduler");
        sched_enable();

        /*
         * Effectively let the other protocol use
         * the medium exclusively
         */
        APP_vSetSchedPolicy(ZB_PROTO_ID, E_STATE_ZB_APP_INACTIVE);
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

        /* This device cannot perform sleep (OT active),
         * but we mimic sleeping by not running ZB loop
         * Also check if other protocol is active.
         */
        if (!gZBSleeping && App_ZB_CanRun())
        {
            App_ZB_Run(0);
        }

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
        if (APP_eGetCurrentApplicationState() >= E_RUNNING)
        {
            APP_vSetSchedPolicy(ZB_PROTO_ID, E_STATE_ZB_APP_ACTIVE);
        }
        BDB_vStart();

        initialized = TRUE;
    }

    while (1)
    {
        /* place event handler code here... */
        APP_vRunZigbee();
        ZTIMER_vTask();

        APP_taskEndDevicNode();

        /* If ZB can go to sleep, just change the policy and mark that ZB is sleeping */
        if (PWR_CheckIfDeviceCanGoToSleep())
        {
            APP_vSetSchedPolicy(ZB_PROTO_ID, E_STATE_ZB_APP_INACTIVE);
            gZBSleeping = TRUE;
        }

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
    bool bColdStart = TRUE;

    /* place initialization code here... */
    APP_vInitResources();
    APP_vInitZigbeeResources();

    if (bColdStart)
    {
        PWR_ChangeDeepSleepMode((uint8_t)E_AHI_SLEEP_OSCON_RAMON);
        PWR_Init();
        PWR_vForceRadioRetention(TRUE);
    }
    /* Initialise the Persistent Data Manager */
    PDM_eInitialise(1200, 63, NULL);

    APP_vInitialiseEndDevice(bColdStart);
}
/*! *********************************************************************************
 * @}
 ********************************************************************************** */
