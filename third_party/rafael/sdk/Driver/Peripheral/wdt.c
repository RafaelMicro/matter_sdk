/** @file wdt.c
 *
 * @brief watchdog  timer driver file.
 *
 */
/**************************************************************************************************
*    INCLUDES
*************************************************************************************************/
#include "cm3_mcu.h"

#include <stdio.h>
#include <string.h>

/**************************************************************************************************
 *    LOCAL VARIABLES
 *************************************************************************************************/
static wdt_isr_handler_t   user_wdt_isr = NULL;

/**************************************************************************************************
 *    GLOBAL FUNCTIONS
 *************************************************************************************************/


void Wdt_Int_Callback_Register(wdt_isr_handler_t wdt_handler)
{
    user_wdt_isr = wdt_handler;

    return;
}

uint32_t Wdt_Start(
    wdt_config_mode_t wdt_mode,
    wdt_config_tick_t wdt_cfg_ticks,
    wdt_isr_handler_t wdt_handler)
{
    wdt_ctrl_t controller;
    controller.reg = 0;

    controller.bit.WDT_EN = 1;

    if (WDT->CONTROL.bit.LOCKOUT)
    {
        return STATUS_INVALID_REQUEST;    /*Lockmode can Not change anymore.*/
    }

    if (wdt_mode.int_enable)
    {
        /*interrupt mode should has interrupt ISR*/
        if (wdt_handler == NULL)
        {
            return STATUS_INVALID_PARAM;
        }

        if (wdt_cfg_ticks.int_ticks >= wdt_cfg_ticks.wdt_ticks)
        {
            return STATUS_INVALID_PARAM;    /*the setting is nonsense.*/
        }

        WDT->CLEAR = 1; /*clear interrupt REQ?*/

        Wdt_Int_Callback_Register(wdt_handler);
        controller.bit.INT_EN = 1;

        WDT->INT_VALUE = wdt_cfg_ticks.int_ticks;

        NVIC_EnableIRQ(Wdt_IRQn);
    }
    else
    {
        /*No interrupt mode.*/
        user_wdt_isr = NULL;
        NVIC_DisableIRQ(Wdt_IRQn);
    }

    if (wdt_mode.reset_enable)
    {
        controller.bit.RESET_EN = 1;    /*Lock*/
    }

    if (wdt_mode.lock_enable)
    {
        controller.bit.LOCKOUT = 1;
    }

    controller.bit.PRESCALE = wdt_mode.prescale;

    WDT->WINDOW_MIN = wdt_cfg_ticks.wdt_min_ticks;
    WDT->LOAD = wdt_cfg_ticks.wdt_ticks;
    WDT->CONTROL.reg = controller.reg;

    return STATUS_SUCCESS;
}

uint32_t Wdt_Stop(void)
{
    if (WDT->CONTROL.bit.LOCKOUT)
    {
        return STATUS_INVALID_REQUEST;    /*Lockmode can Not change anymore.*/
    }

    WDT->CONTROL.bit.WDT_EN = 0;

    return STATUS_SUCCESS;
}

/**
 * @ingroup wdt_group
 * @brief Watch dog Interrupt Handler
 */
void Wdt_Handler(void)
{
    WDT->CLEAR = 1; /*clear interrupt REQ?*/
    if (user_wdt_isr)
    {
        user_wdt_isr();
    }
    else
    {
        /*this is debug only.. it should be removed.*/
        while (1)
        {
            printf("what ??? \n"); /*debug*/
        }
    }
}

