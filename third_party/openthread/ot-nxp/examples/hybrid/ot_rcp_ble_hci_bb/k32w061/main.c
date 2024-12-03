/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * \file
 *
 * This file is the source file for the rcp_hci_bb_app
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

/* Bluetooth Low Energy */
#include "ble_init.h"

#include <openthread-system.h>
#include <openthread/instance.h>
#include <openthread/ncp.h>
#include <openthread/tasklet.h>

#include "controller_interface.h"

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

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static void App_Thread(uint32_t param);
static void App_GenericCallback(gapGenericEvent_t *pGenericEvent);

/************************************************************************************
*************************************************************************************
* Public functions prototypes
*************************************************************************************
************************************************************************************/

extern void vMMAC_IntHandlerBbc();
extern void vMMAC_IntHandlerPhy();
extern void otAppNcpInit(otInstance *aInstance);
/* Multi protocol stuff */
extern void sched_enable(void);
extern void __vMMAC_Enable(uint8_t u8ProtoTag);
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
 * \brief    Initializes application specific functionality before the BLE stack init.
 *
 ********************************************************************************** */
static void BleApp_Init(void)
{
    // TMR_TimeStampInit();
}

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

        /* Framework init */
        MEM_Init();

        PRINTF("Hci Black Box\n\r");

        /* Cryptographic and RNG hardware initialization */
        SecLib_Init();
        /* RNG software initialization and PRNG initial seeding (from hardware) */
        RNG_Init();
        RNG_SetPseudoRandomNoSeed(NULL);

#if defined(gMWS_UseCoexistence_d) && (gMWS_UseCoexistence_d)
        otPlatRadioSetCoexEnabled(gpOpenThreadInstance, true);
#else
        BOARD_InitCoexOutPin();
#endif

#if (defined gRadioUsePdm_d && gRadioUsePdm_d)
        PDM_Init();
#endif

        /* Initialize peripheral drivers specific to the application */
        BleApp_Init();

        /* BLE Host Stack Init */
        if (Ble_Initialize(App_GenericCallback) != gBleSuccess_c)
        {
            panic(0, 0, 0, 0);
            return;
        }

        /* Initialize OpenThread timer, RNG and Radio */
        otSysInit(1, argv);
        gpOpenThreadInstance = otInstanceInitSingle();
        otAppNcpInit(gpOpenThreadInstance);

        /*
         * Register here the OT protocol. The otPlatEnableRadio() comes in late, i.e. when the RCP
         * f.i. starts the Thread network. This means that up until then, the scheduler can't be
         * started. This means the ZB app cannot do TX/RX until then. Furthermore, enabling the
         * scheduler will lead to the OT protocol not being registered at all, so Thread won't
         * work at all. So for now, as a temporary fix, register the OT protocol here. This should
         * not affect the functionality, since the OT will set the proper channel, interrupt
         * handler, etc. as part of the initialization.
         */
        __vMMAC_Enable(1);

        /* After this point, cannot register a protocol anymore */
        PRINTF("Enabling dual PAN scheduler");
        sched_enable();
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
        /* For BareMetal break the while(1) after 1 run */
        if (gUseRtos_c == 0)
        {
            break;
        }
    }
}

STATIC void App_GenericCallback(gapGenericEvent_t *pGenericEvent)
{
}
/*! *********************************************************************************
 * @}
 ********************************************************************************** */
