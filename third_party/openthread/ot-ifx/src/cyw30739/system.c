/*
 *  Copyright (c) 2021, The OpenThread Authors.
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
 * @brief
 *   This file includes the platform-specific initializers.
 */

#include "system.h"

#include <stdio.h>

#include <openthread-system.h>
#ifndef CHIP_HAVE_CONFIG_H
#include <openthread/tasklet.h>
#endif

#include <wiced_memory.h>
#include <wiced_platform.h>
#include <wiced_platform_memory.h>
#include <wiced_rtos.h>

#if !OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
#include "ifx_ot_cli_cmd.h"
#endif

#ifdef CHIP_HAVE_CONFIG_H
#define SYSTEM_EVENT_WAIT_TIME WICED_NO_WAIT
#else
#define SYSTEM_EVENT_WAIT_TIME WICED_WAIT_FOREVER
#endif

#ifndef BITS_PER_BYTE
#define BITS_PER_BYTE (8)
#endif // BITS_PER_BYTE

extern void otPlatAlramInit(void);
extern void otPlatRadioInit(void);
extern void otPlatUartInit(void);

typedef struct system_event
{
    uint32_t                event_code;
    system_event_handler_t *p_hanlder;
    struct system_event    *p_next;
} system_event_t;

typedef struct
{
    struct
    {
        wiced_event_flags_t *p_handle;
        uint32_t             assigned_event_code_shiftment;
        system_event_t      *p_list;
    } event;

#ifndef CHIP_HAVE_CONFIG_H
    struct
    {
        uint32_t post_init;
        uint32_t ot_tasklets;
    } event_code;
#endif
} system_cb_t;

static system_cb_t system_cb = {0};

static void system_event_dispatch(void);

#ifndef CHIP_HAVE_CONFIG_H
static void system_post_init(void)
{
    printf("system_post_init\n");

#if !OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
    ifx_ot_cli_cmd_install(otInstanceInitSingle());
#endif // !OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE

    printf("Free RAM sizes: %lu\n", wiced_memory_get_free_bytes());
}
#endif // !CHIP_HAVE_CONFIG_H

void otSysInit(int argc, char *argv[])
{
    wiced_result_t result;

    OT_UNUSED_VARIABLE(argc);
    OT_UNUSED_VARIABLE(argv);

    /* Create system event flag handle. */
    system_cb.event.p_handle = wiced_rtos_create_event_flags();

    if (!system_cb.event.p_handle)
    {
        printf("%s: Fail to create system event flag handle.\n", __FUNCTION__);
        return;
    }

    /* Initialize the system event flags. */
    result = wiced_rtos_init_event_flags(system_cb.event.p_handle);
    if (result != WICED_SUCCESS)
    {
        printf("%s: Fail to init. system event flags.\n", __FUNCTION__);
        return;
    }

    /* Initialize the Alarm Abstraction Layer. */
    otPlatAlramInit();

    /* Initialize the Radio Abstraction Layer. */
    otPlatRadioInit();

#ifndef CHIP_HAVE_CONFIG_H
    /* Initialize the UART Abstraction Layer. */
    otPlatUartInit();

    /* Get event code for system post-initialization. */
    if (!system_event_register(&system_cb.event_code.post_init, system_post_init))
    {
        printf("%s: Fail to get event code.\n", __FUNCTION__);
        return;
    }

    /* Issue an event for system post-initialization. */
    system_event_set(system_cb.event_code.post_init);

    /* Get event code for openthread tasklets. */
    if (!system_event_register(&system_cb.event_code.ot_tasklets, NULL))
    {
        printf("%s: Fail to get event code for openthread tasklets.\n", __FUNCTION__);
        return;
    }
#endif /* CHIP_HAVE_CONFIG_H */
}

void otSysDeinit(void) {}

bool otSysPseudoResetWasRequested(void) { return false; }

void otSysProcessDrivers(otInstance *aInstance)
{
#ifndef CHIP_HAVE_CONFIG_H
    // Check if there is queued OpenThread tasklet events.
    if (otTaskletsArePending(aInstance))
    {
        /* Issue an event for openthread tasklets. */
        system_event_set(system_cb.event_code.ot_tasklets);
    }
#endif /* CHIP_HAVE_CONFIG_H */

    system_event_dispatch();
}

OT_TOOL_WEAK void otSysEventSignalPending(void) {}

/**
 *  \brief Get system event code and the register the corresponding event handler if
 *         provided.
 *
 *  \param [out]    allocated event code
 *  \param [in]     user specified event handler
 *
 *  \return WICED_TRUE  : Success
 *          WICED_FALSE : Fail.
 *
 */
wiced_bool_t system_event_register(uint32_t *p_event_code, system_event_handler_t *p_event_handler)
{
    system_event_t *p_new;

    /* Check parameter. */
    if (!p_event_code)
    {
        return WICED_FALSE;
    }

    /* Check availability of event code. */
    if (system_cb.event.assigned_event_code_shiftment == (sizeof(uint32_t) * BITS_PER_BYTE))
    {
        return WICED_FALSE;
    }

    if (!p_event_handler)
    {
        *p_event_code = (1 << system_cb.event.assigned_event_code_shiftment++);

        return WICED_TRUE;
    }

    /* Allocate memory. */
    p_new = (system_event_t *)wiced_platform_memory_allocate(sizeof(system_event_t));

    if (!p_new)
    {
        return WICED_FALSE;
    }

    p_new->event_code = (1 << system_cb.event.assigned_event_code_shiftment++);
    p_new->p_hanlder  = p_event_handler;
    p_new->p_next     = system_cb.event.p_list;

    system_cb.event.p_list = p_new;

    *p_event_code = p_new->event_code;

    return WICED_TRUE;
}

/**
 *  \brief Set a system event
 *
 *  \param [in] event code (get by register utility)
 *
 */
void system_event_set(uint32_t event_code)
{
    assert(!wiced_rtos_check_for_stack_overflow());

    if (wiced_rtos_set_event_flags(system_cb.event.p_handle, event_code) != WICED_SUCCESS)
    {
        printf("Err: Fail to set system event (%lu).\n", event_code);
    }
    otSysEventSignalPending();
}

/**
 *  \brief Wait a system event
 *
 *  \param [in] event code (get by register utility)
 *
 */
void system_event_wait(uint32_t event_code)
{
    uint32_t flags_set;

    wiced_rtos_wait_for_event_flags(system_cb.event.p_handle, event_code, &flags_set, WICED_TRUE, WAIT_FOR_ALL_EVENTS,
                                    WICED_WAIT_FOREVER);
}

/**
 * \brief This function waits and dispatches system events.
 */
void system_event_dispatch(void)
{
    uint32_t        flags_set;
    system_event_t *p_index;

    /* Wait for event. */
    flags_set = 0;
    wiced_rtos_wait_for_event_flags(system_cb.event.p_handle, 0xffffffff, &flags_set, WICED_TRUE, WAIT_FOR_ANY_EVENT,
                                    SYSTEM_EVENT_WAIT_TIME);

    /* Process this event if registered. */
    p_index = system_cb.event.p_list;
    while (p_index != NULL && flags_set != 0)
    {
        if (p_index->event_code & flags_set)
        {
            (*p_index->p_hanlder)();

            flags_set &= ~p_index->event_code;
        }

        p_index = p_index->p_next;
    }
}
