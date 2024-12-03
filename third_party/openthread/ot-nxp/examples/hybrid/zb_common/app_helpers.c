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

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern bool_t gZBSleeping;
/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
 * \brief  This function represents the ZB app portion of the task.
 *         This task reuses the stack allocated for the MainThread.
 *         This function is called to process all events for the task. Events
 *         include timers, messages and any other user defined events.
 * \param[in]  argument
 *
 ********************************************************************************** */
void APP_vSetSchedPolicy(uint8_t u8ProtoId, APP_teState eState)
{
    struct sched_policy_config config;
    sched_status               status = E_SCHED_OK;

    if (sched_enabled() != TRUE)
    {
        PRINTF("Failed configure policy %d for protocol %d: scheduling not enabled\n", config.policy, u8ProtoId);
        return;
    }

    switch (eState)
    {
    case E_STATE_ZB_APP_INIT:
        /* Intentional fallthrough */
    case E_STATE_ZB_APP_INACTIVE:
        /*
         * This sets the template for the ZB protocol. Setting
         * slice to 0 => it will not 'waste' time from the other
         * protocol, this will 'switch to itself'.
         */
        config.duty_cycle.slice = 0;
        config.policy           = E_SCHED_DUTY_CYCLE;

        break;

    case E_STATE_ZB_SCAN_NETWORKS:
        /*
         * The ZB node is scanning. Since the RX needs to
         * be on for 30720us (DurScan = 0), lockout the other protocol
         * for ~31ms.
         * NOTE: This needs to be done before every attempt for NWK
         *       Steering
         */
        config.policy         = E_SCHED_LOCKED;
        config.locked.timeout = MILISECONDS_TO_SCHEDTICKS(31);
        break;

    case E_STATE_ZB_NETWORK_ENTRY:
        /*
         * The ZB node is trying to join a network. Allow ZB to join network
         * by locking it around 1 second. Actually it's less because after joining
         * it will start POLLING stopping network entry policy.
         */
        config.policy         = E_SCHED_LOCKED;
        config.locked.timeout = MILISECONDS_TO_SCHEDTICKS(1000);
        break;

    case E_STATE_ZB_APP_POLL:
        /*
         * The ZB node is in a network and polls for data. If we'd
         * switch to OT after a poll and miss the frame from the parent,
         * the whole mechanism will be broken. So enforce a locked
         * policy for the maximum time a frame would take
         */
        config.duty_cycle.slice = MILISECONDS_TO_SCHEDTICKS(25);
        config.policy           = E_SCHED_LOCKED;
        break;

    case E_STATE_ZB_APP_ACTIVE:
        /*
         * Allow the ZB node that is in a network to have some
         * time to do TXs.
         */
        config.duty_cycle.slice = MILISECONDS_TO_SCHEDTICKS(30);
        config.policy           = E_SCHED_DUTY_CYCLE;
        break;

    default:
        PRINTF("APP_vSetPolicy: invalid state %d\n", eState);
        status = E_SCHED_FAILED;
        break;
    }

    if (status == E_SCHED_OK)
    {
        status = sched_configure_policy(u8ProtoId, &config);
        if (status == E_SCHED_OK)
        {
            status = sched_set_policy(u8ProtoId, config.policy);
            if (status != E_SCHED_OK)
            {
                PRINTF("Failed to set policy %d for protocol %d\n", config.policy, u8ProtoId);
            }
        }
    }
    else
    {
        PRINTF("Failed to configure policy %d for protocol %d\n", config.policy, u8ProtoId);
    }
}

/*! *********************************************************************************
 * \brief  This function is called within the wakeup callback from ZB
 *         ZED application, the sole purpose is to mark that ZB is not sleeping
 *         anymore and to switch to active policy.
 ********************************************************************************** */
void App_ZB_WakeCallBack(void)
{
    gZBSleeping = FALSE;

    /* Set policy active on waking up */
    APP_vSetSchedPolicy(ZB_PROTO_ID, E_STATE_ZB_APP_ACTIVE);
}

/*! *********************************************************************************
 * \brief  This function is called to decide if the ZB related processing can be run.
 ********************************************************************************** */
bool App_ZB_CanRun()
{
#if SUPPORT_FOR_BLE
    extern teDynState dyn_state[E_DYN_PROTOCOL_COUNT];
    /*
     * For lack of a better mechanism, query directly the MAC to figure out if
     * the BLE protocol is active. If so, don't run the ZB loop.
     */
    return (eDynGetProtocolState(E_DYN_MASTER) == E_DYN_STATE_OFF);
#else
    return TRUE;
#endif
}
/*! *********************************************************************************
 * @}
 ********************************************************************************** */
