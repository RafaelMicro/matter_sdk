/**
 * Copyright (c) 2021 All Rights Reserved.
 */
/** @file sys_timer.c
 *
 * @author Rex
 * @version 0.1
 * @date 2021/06/10
 * @license
 * @description
 */

//=============================================================================
//                Include
//=============================================================================
#include "cm3_mcu.h"
#include "FreeRTOSConfig.h"
#include "sys_arch.h"
#include "sys_timer.h"
#include "mem_mgmt.h"
//=============================================================================
//                Private Definitions of const value
//=============================================================================
#define CONFIG_SYS_TIMER_TASK_PRIORITY               TASK_PRIORITY_TIMER_TASK_PRIORITY
#define CONFIG_SYS_TIMER_STACK_SIZE                  128
#define CONFIG_SYS_TIMER_RUNNING_TASK_PRIORITY       TASK_PRIORITY_TIMER_RUNNING_TASK_PRIORITY
#define CONFIG_SYS_TIMER_RUNNING_STACK_SIZE          512

#define SYS_TIMER_TRIGGER_TASK_TIMEOUT       100 // 100 ms
#define SYS_TIMER_CMD_QUEUE_LENGTH           20
#define SYS_TIMER_CMD_QUEUE_ITEM_SIZES       (sizeof(sys_timer_queue_t))
#define SYS_TIMER_CMD_QUEUE_SEND_TIMEOUT     5 // 5 ms

#define SYS_TIMER_RUNNING_QUEUE_LENGTH       20
#define SYS_TIMER_RUNNING_QUEUE_ITEM_SIZES   (sizeof(void *))

#define SYS_TIMER_RUNNING_RESERVED_LENGTH    10

#define TIMER_ERR_LOG() err("TIMER_ERR:%s(%u)\n", __FUNCTION__, __LINE__)

#define SYS_TIMER_START(timer)   timer_cmd_send(timer, SYS_TIMER_CMD_START, 0, 0)
#define SYS_TIMER_STOP(timer)    timer_cmd_send(timer, SYS_TIMER_CMD_STOP, 0, 0)
#define SYS_TIMER_RESET(timer)   timer_cmd_send(timer, SYS_TIMER_CMD_RESET, 0, 0)
#define SYS_TIMER_CHANGE_PERIOD(timer, period)       timer_cmd_send(timer, SYS_TIMER_CMD_CHANGE_PERIOD, period, 0)
#define SYS_TIMER_CHANGE_PRIORITY(timer, priority)   timer_cmd_send(timer, SYS_TIMER_CMD_CHANGE_PRIORITY, priority, 0)
#define SYS_TIMER_CHANGE_EXECUTE_MODE(timer, execute_mode)   timer_cmd_send(timer, SYS_TIMER_CMD_CHANGE_EXECUTE_MODE, execute_mode, 0)
#define SYS_TIMER_DELETE(timer)  timer_cmd_send(timer, SYS_TIMER_CMD_DELETE, 0, 0)

#define SYS_TIMER_START_FROM_ISR(timer)   timer_cmd_send(timer, SYS_TIMER_CMD_START, 0, 1)
#define SYS_TIMER_STOP_FROM_ISR(timer)    timer_cmd_send(timer, SYS_TIMER_CMD_STOP, 0, 1)
#define SYS_TIMER_RESET_FROM_ISR(timer)   timer_cmd_send(timer, SYS_TIMER_CMD_RESET, 0, 1)
#define SYS_TIMER_CHANGE_PERIOD_FROM_ISR(timer, period)       timer_cmd_send(timer, SYS_TIMER_CMD_CHANGE_PERIOD, period, 1)
#define SYS_TIMER_CHANGE_PRIORITY_FROM_ISR(timer, priority)   timer_cmd_send(timer, SYS_TIMER_CMD_CHANGE_PRIORITY, priority, 1)
#define SYS_TIMER_CHANGE_EXECUTE_MODE_FROM_ISR(timer, execute_mode)   timer_cmd_send(timer, SYS_TIMER_CMD_CHANGE_EXECUTE_MODE, execute_mode, 1)
#define SYS_TIMER_DELETE_FROM_ISR(timer)  timer_cmd_send(timer, SYS_TIMER_CMD_DELETE, 0, 1)
#define SYS_TIMER_TASK_WAKE_UP_FROM_ISR   timer_cmd_send((sys_timer_t *)0x12345678, SYS_TIMER_CMD_TASK_WAKE_UP, 0, 1)

//#define CONFIG_UNITTEST_ENABLE
//=============================================================================
//                Private ENUM
//=============================================================================
typedef enum SYS_TIMER_CMD
{
    SYS_TIMER_CMD_START                  = 0,
    SYS_TIMER_CMD_STOP                   = 1,
    SYS_TIMER_CMD_RESET                  = 2,
    SYS_TIMER_CMD_CHANGE_PERIOD          = 3,
    SYS_TIMER_CMD_CHANGE_PRIORITY        = 4,
    SYS_TIMER_CMD_CHANGE_EXECUTE_MODE    = 5,
    SYS_TIMER_CMD_DELETE                 = 6,
    SYS_TIMER_CMD_TASK_WAKE_UP           = 7,
} sys_timer_cmd_t;

//=============================================================================
//                Private Struct
//=============================================================================
typedef struct SYS_TIMER_CONTROL
{
    link_list_t         working_list;
    uint32_t            current_time; // ms
    uint32_t            alarm_time;
} sys_timer_ctrl_t;

/*************************************************************/
/*********************** SYS Timer Queue ************************/
/*************************************************************/
typedef struct SYS_TIMER_QUEUE
{
    sys_timer_t          *timer;
    uint32_t            cmd_op      : 8;
    uint32_t            cmd_value   : 24;
} sys_timer_queue_t;

//=============================================================================
//                Private Function Declaration
//=============================================================================
static void timer_low_running_handler(void *params);
//=============================================================================
//                Private Global Variables
//=============================================================================

sys_timer_ctrl_t g_timer_ctrl;
static sys_queue_t g_timer_cmd_queue;
static sys_queue_t g_timer_running_queue[SYS_TIMER_PRIORITY_MAX];

static void *gpt_timer_execute_handler[SYS_TIMER_PRIORITY_MAX] =
{
    timer_low_running_handler,
};

static uint32_t u32_timer_init = 0;
//=============================================================================
//                Public Global Variables
//=============================================================================
uint32_t u32_debug_timer;
volatile uint32_t CPU_RunTime;
//=============================================================================
//                Private Definition of Compare/Operation/Inline funciton/
//=============================================================================

//=============================================================================
//                Functions
//=============================================================================
void sys_timer_state_print(void)
{
    u32_debug_timer = 1;
}

static void debug_timer_cb(void *cb_param)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (u32_debug_timer)
    {
        sys_timer_t *timer_list = NULL;
        u32_debug_timer = 0;
        info("current_time %u, alarm_time %u\n",
             g_timer_ctrl.current_time, g_timer_ctrl.alarm_time);
        list_for_each(timer_list, &g_timer_ctrl.working_list, sys_timer_t, list)
        {
            info("TIMER :%s (TO %u, Pri %u, Period %u, Auto %u)\n",
                 timer_list->name, timer_list->timeout, timer_list->priority,
                 timer_list->period, timer_list->auto_reload);
        }
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

static sys_err_t debug_timer_init(void)
{
    sys_timer_t *debug_timer;
    sys_err_t t_return = SYS_ERR_OK;
    sys_timer_err_t t_timer_return;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    do
    {
        debug_timer = sys_timer_create("BUG_t",
                                       SYS_TIMER_SECOND_TO_TICK(1),
                                       1,
                                       SYS_TIMER_PRIORITY_0,
                                       SYS_TIMER_EXECUTE_ONCE_FOR_EACH_TIMEOUT,
                                       NULL,
                                       debug_timer_cb);

        if (debug_timer == NULL)
        {
            err("dbgtimer create fail\n");
            t_return = SYS_ERR_THREAD_MALLOC_FAIL;
            break;
        }

        t_timer_return = sys_timer_start(debug_timer);
        if (t_timer_return != SYS_TIMER_PASS)
        {
            err("dbgtimer_op fail(%d)\n", t_timer_return);
            t_return = SYS_ERR_TIMER_OP;
            break;
        }
    } while (0);
    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}


static void timer_excute_cb_timer_cb(sys_timer_t *ptimer)
{
    if (ptimer->wating_for_execution > 1)
    {
        info("timer(%s:%p) duplicate timeout(%d), tick %d\n",
             ptimer->name, ptimer, ptimer->wating_for_execution, sys_now());
    }

    ptimer->running = 0;
    ptimer->cb_function(ptimer->cb_param);

    if (ptimer->wating_for_execution)
    {
        ptimer->wating_for_execution--;
        if ((ptimer->receive_delete_cmd == 1) &&
                (ptimer->wating_for_execution == 0))
        {
            sys_timer_delete(ptimer);
        }
    }
    else
    {
        TIMER_ERR_LOG();
    }
}


static void timer_low_running_handler(void *params)
{
    sys_timer_t *p_timer;
    info("%s(%u)\n", __FUNCTION__, SYS_TIMER_PRIORITY_0);

    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    while (1)
    {
        if (sys_queue_recv(&g_timer_running_queue[SYS_TIMER_PRIORITY_0], (void *)&p_timer, 0) == SYS_ARCH_TIMEOUT)
        {
            TIMER_ERR_LOG();
            break;
        }
        timer_excute_cb_timer_cb(p_timer);
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}
static sys_err_t timer_running_task_create(void)
{
    ///static char *task_name[SYS_TIMER_PRIORITY_MAX] = {"RT_L", "RT_H"};
    static char *task_name[SYS_TIMER_PRIORITY_MAX] = {"RT"};
    uint32_t u32_idx, u32_priority;
    sys_err_t t_return = SYS_ERR_OK;
    sys_task_t t_task;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    for (u32_idx = 0; u32_idx < SYS_TIMER_PRIORITY_MAX; u32_idx++)
    {
        sys_queue_new(&g_timer_running_queue[u32_idx], SYS_TIMER_RUNNING_QUEUE_LENGTH, SYS_TIMER_RUNNING_QUEUE_ITEM_SIZES);

        if (g_timer_running_queue[u32_idx] == NULL)
        {
            err("run_queue[%d] create fail\n", u32_idx);
            t_return = SYS_ERR_QUEUE_MALLOC_FAIL;
            break;
        }
        u32_priority = TASK_PRIORITY_TIMER_RUNNING_TASK_PRIORITY - (SYS_TIMER_PRIORITY_MAX - u32_idx - 1);
        t_task = sys_task_new(task_name[u32_idx],
                              (sys_thread_fn )gpt_timer_execute_handler[u32_idx],
                              NULL,
                              CONFIG_SYS_TIMER_RUNNING_STACK_SIZE,
                              u32_priority);
        if (t_task == NULL)
        {
            err("run_thread[%d] create fail\n", u32_idx);
            t_return = SYS_ERR_THREAD_MALLOC_FAIL;
            break;
        }
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}

static sys_timer_err_t timer_parameter_check(sys_timer_cmd_t cmd_op, uint32_t value)
{
    sys_timer_err_t t_result = SYS_TIMER_PASS;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    switch (cmd_op)
    {
    case SYS_TIMER_CMD_CHANGE_PERIOD:
        if (value > SYS_TIMER_MAX_PERIOD)
        {
            TIMER_ERR_LOG();
            t_result = SYS_TIMER_PERIOD_FAIL;
        }
        break;
    case SYS_TIMER_CMD_CHANGE_PRIORITY:
        if (value > SYS_TIMER_PRIORITY_MAX)
        {
            TIMER_ERR_LOG();
            t_result = SYS_TIMER_PRIORITY_FAIL;
        }
        break;
    case SYS_TIMER_CMD_CHANGE_EXECUTE_MODE:
        if (value > SYS_TIMER_EXECUTE_ONCE_FOR_DUPLICATE_TIMEOUT)
        {
            TIMER_ERR_LOG();
            t_result = SYS_TIMER_EXECUTE_MODE_FAIL;
        }
        break;
    case SYS_TIMER_CMD_START:
    case SYS_TIMER_CMD_STOP:
    case SYS_TIMER_CMD_RESET:
    case SYS_TIMER_CMD_DELETE:
    case SYS_TIMER_CMD_TASK_WAKE_UP:
    default:
        break;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_result;
}

static sys_timer_err_t timer_cmd_send(sys_timer_t *timer, sys_timer_cmd_t cmd_op, uint32_t cmd_value, uint32_t is_isr)
{
    sys_timer_err_t t_result = SYS_TIMER_PASS;

    do
    {
        sys_timer_queue_t timer_queue;

        /*-----------------------------------*/
        /* A.Input Parameter Range Check     */
        /*-----------------------------------*/
        if (timer == NULL)
        {
            TIMER_ERR_LOG();
            t_result = SYS_TIMER_NULL;
            break;
        }

        t_result = timer_parameter_check(cmd_op, cmd_value);
        if (t_result < 0)
        {
            TIMER_ERR_LOG();
            break;
        }

        /*-----------------------------------*/
        /* B. Main Functionality             */
        /*-----------------------------------*/

        timer_queue.timer = timer;
        timer_queue.cmd_op = cmd_op;
        timer_queue.cmd_value = cmd_value;

        if (is_isr)
        {
            sys_queue_send_from_isr(&g_timer_cmd_queue, &timer_queue);
        }
        else
        {
            if (sys_queue_send_with_timeout(&g_timer_cmd_queue, &timer_queue, SYS_TIMER_CMD_QUEUE_SEND_TIMEOUT) != ERR_OK)
            {
                TIMER_ERR_LOG();
                t_result = SYS_TIMER_CMD_SEND_FAIL;
                break;
            }
        }
    } while (0);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_result;
}

static inline void alarm_time_reset(void)
{
    sys_timer_t *timer_list = list_first(&g_timer_ctrl.working_list, sys_timer_t, list);
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (timer_list)
    {
        g_timer_ctrl.alarm_time = timer_list->timeout;
    }
    else
    {
        g_timer_ctrl.alarm_time = SYS_MAX_TIMER;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

static void _timer_start(sys_timer_t *timer)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    list_detach(&timer->list);
    timer->running = 1;
    timer->timeout = g_timer_ctrl.current_time + timer->period;
    if (list_empty(&g_timer_ctrl.working_list))
    {
        list_attach_next(&g_timer_ctrl.working_list, &timer->list);
        g_timer_ctrl.alarm_time = timer->timeout;
    }
    else
    {
        sys_timer_t *timer_list;
        list_for_each(timer_list, &g_timer_ctrl.working_list, sys_timer_t, list)
        {
            if (timer_list->timeout > timer->timeout)
            {
                list_attach_prev(&timer_list->list, &timer->list);
                break;
            }
            else if ((timer_list->timeout == timer->timeout) &&
                     (timer_list->priority < timer->priority))
            {
                list_attach_prev(&timer_list->list, &timer->list);
                break;
            }
        }
        if (list_empty(&timer->list))
        {
            list_attach_tail(&g_timer_ctrl.working_list, &timer->list);
        }

        alarm_time_reset();
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

static void _timer_stop(sys_timer_t *timer)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    timer->running = 0;
    list_detach(&timer->list);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

static void timer_reset(sys_timer_t *timer)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (list_empty(&timer->list))
    {
        // not in working list
    }
    else
    {
        _timer_start(timer);
    }
    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

static void timer_change_period(sys_timer_t *timer, uint32_t period)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    timer->period = period;
    timer_reset(timer);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

static void timer_change_priority(sys_timer_t *timer, uint32_t priority)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    timer->priority = priority;
    timer_reset(timer);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

static void timer_change_execute_mode(sys_timer_t *timer, uint32_t execute_mode)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    timer->execute_mode = execute_mode;
    timer_reset(timer);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

static void timer_delete(sys_timer_t *timer)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    _timer_stop(timer);
    // in hplc_timer_handler, if auto_reload is one, than it will auto restart
    memset(timer, 0x0, sizeof(sys_timer_t));
    mem_free(timer);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

static void timer_shift(uint32_t shift_time)
{
    sys_timer_t *timer_list;

    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    g_timer_ctrl.current_time -= shift_time;

    if (list_empty(&g_timer_ctrl.working_list))
    {
        g_timer_ctrl.alarm_time = SYS_MAX_TIMER;
    }
    else
    {
        list_for_each(timer_list, &g_timer_ctrl.working_list, sys_timer_t, list)
        {
            if (timer_list->timeout <= shift_time)
            {
                timer_list->timeout = 0;
            }
            else
            {
                timer_list->timeout -= shift_time;
            }
        }
        alarm_time_reset();
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

static void switch_to_running_task(sys_timer_t *timer)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    if (timer->cb_function == NULL)
    {
        // running event already in running task queue
        err("Timer(%s) cb = NULL\n", timer->name);
        return;
    }

    if ((timer->execute_mode == SYS_TIMER_EXECUTE_ONCE_FOR_DUPLICATE_TIMEOUT) &&
            (timer->wating_for_execution > 0))
    {
        // running event already in running task queue
        err("Timer(%s) Waiting for execution\n", timer->name);
        return;
    }

    if (timer->receive_delete_cmd == 1)
    {
        err("Timer(%s) will be delete, not run again\n", timer->name);
        return;
    }

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    timer->wating_for_execution++;
    if ((sys_queue_remaining_size(&g_timer_running_queue[timer->priority]) < SYS_TIMER_RUNNING_RESERVED_LENGTH) &&
            (timer->wating_for_execution > 2))
    {
        // give up current timer timeout
        timer->wating_for_execution--;
        err("Timer(%s) expire give up\n", timer->name);
        return;
    }

    if (sys_queue_send_with_timeout(&g_timer_running_queue[timer->priority], (void *)&timer, SYS_TIMER_CMD_QUEUE_SEND_TIMEOUT) != ERR_OK)
    {
        timer->wating_for_execution--;
        TIMER_ERR_LOG();
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

static void timer_cmd_queue_check(void)
{
    // check cmd queue
    do
    {
        sys_timer_queue_t timer_queue;
        /*-----------------------------------*/
        /* A.Input Parameter Range Check     */
        /*-----------------------------------*/
        if (sys_queue_recv(&g_timer_cmd_queue, (void *)&timer_queue, 0) == SYS_ARCH_TIMEOUT)
        {
            TIMER_ERR_LOG();
            break;
        }

        /*-----------------------------------*/
        /* B. Main Functionality             */
        /*-----------------------------------*/
        switch (timer_queue.cmd_op)
        {
        case SYS_TIMER_CMD_START:
            _timer_start(timer_queue.timer);
            break;
        case SYS_TIMER_CMD_STOP:
            _timer_stop(timer_queue.timer);
            break;
        case SYS_TIMER_CMD_RESET:
            timer_reset(timer_queue.timer);
            break;
        case SYS_TIMER_CMD_CHANGE_PERIOD:
            timer_change_period(timer_queue.timer, timer_queue.cmd_value);
            break;
        case SYS_TIMER_CMD_CHANGE_PRIORITY:
            timer_change_priority(timer_queue.timer, timer_queue.cmd_value);
            break;
        case SYS_TIMER_CMD_CHANGE_EXECUTE_MODE:
            timer_change_execute_mode(timer_queue.timer, timer_queue.cmd_value);
            break;
        case SYS_TIMER_CMD_DELETE:
            if (timer_queue.timer->wating_for_execution)
            {
                // Waiting for the timeout event to complete
                timer_queue.timer->receive_delete_cmd = 1;
            }
            else
            {
                timer_delete(timer_queue.timer);
            }
            break;
        case SYS_TIMER_CMD_TASK_WAKE_UP:
        default:
            break;
        }
    } while (uxQueueMessagesWaiting(g_timer_cmd_queue));

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

static void timer_list_check(void)
{
    sys_timer_t *timer_list = NULL;

    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/

    if (g_timer_ctrl.current_time > SYS_TIMER_SHIFT_THRESHOLD)
    {
        timer_shift(SYS_TIMER_SHIFT_THRESHOLD);
    }

    // check timer list
    while (g_timer_ctrl.current_time >= g_timer_ctrl.alarm_time)
    {
        timer_list = list_first(&g_timer_ctrl.working_list, sys_timer_t, list);
        if (timer_list)
        {
            if (g_timer_ctrl.current_time > timer_list->timeout)
            {
                list_detach(&timer_list->list);
            }
            else
            {
                timer_list = NULL;
            }
        }
        else
        {
            TIMER_ERR_LOG();
            g_timer_ctrl.alarm_time = SYS_MAX_TIMER;
            break;
        }

        if (timer_list)
        {
            switch_to_running_task(timer_list);

            if (timer_list->auto_reload)
            {
                _timer_start(timer_list);
            }
        }

        alarm_time_reset();
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

static void sys_timer_isr(uint32_t timer_id)
{
    static uint32_t last_alarm_time = 0, u32_same_alarm_time = 0;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    if (!u32_timer_init)
    {
        return;
    }
    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    CPU_RunTime++;
    g_timer_ctrl.current_time++;
    if ((g_timer_ctrl.current_time >= g_timer_ctrl.alarm_time) ||
            (g_timer_ctrl.current_time > SYS_TIMER_SHIFT_THRESHOLD))
    {
        if (last_alarm_time != g_timer_ctrl.alarm_time)
        {
            last_alarm_time = g_timer_ctrl.alarm_time;
            u32_same_alarm_time = 0;
        }
        else
        {
            u32_same_alarm_time++;
            if ((u32_same_alarm_time % SYS_TIMER_TRIGGER_TASK_TIMEOUT) == 1)
            {
                SYS_TIMER_TASK_WAKE_UP_FROM_ISR;
            }
        }
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

sys_err_t sys_timer_hw_init(uint32_t u32_timer_id)
{
    sys_err_t t_return = SYS_ERR_OK;
    timern_t *TIMER;
    timern_t *Timer_Base[5] = {TIMER0, TIMER1, TIMER2, TIMER3, TIMER4};
    timer_config_mode_t cfg;

    do
    {
        cfg.int_en = ENABLE;
        cfg.mode = TIMER_PERIODIC_MODE;
        /*the input clock is 32M/s, so it will become 4M ticks per second */
        cfg.prescale = TIMER_PRESCALE_1;

        TIMER = Timer_Base[u32_timer_id];
        TIMER->CLEAR = 1;       /*clear interrupt*/

        Timer_Open(u32_timer_id, cfg, sys_timer_isr);
        NVIC_SetPriority((IRQn_Type)(Timer0_IRQn + u32_timer_id), configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
        Timer_Start(u32_timer_id, 32000);     /*so each tick is 1ms, 1000HZ*/

    } while (0);

    return t_return;
}

static void timer_handler(void *params)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    while (1)
    {
        timer_cmd_queue_check();

        timer_list_check();
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

int sys_timer_init(uint32_t u32_hw_timer_id)
{
    int t_return = 0;
    sys_task_t  t_task;

    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    do
    {
        sys_timer_hw_init(u32_hw_timer_id);
        memset(&g_timer_ctrl, 0x0, sizeof(g_timer_ctrl));
        g_timer_ctrl.alarm_time = SYS_MAX_TIMER;

        CPU_RunTime = 0;

        list_init(&g_timer_ctrl.working_list);
        sys_queue_new(&g_timer_cmd_queue, SYS_TIMER_CMD_QUEUE_LENGTH, SYS_TIMER_CMD_QUEUE_ITEM_SIZES);
        if (g_timer_cmd_queue == NULL)
        {
            err("timer_queue create fail\n");
            t_return = SYS_ERR_QUEUE_MALLOC_FAIL;
            break;
        }

        t_task = sys_task_new("s_timer_h",
                              (TaskFunction_t)timer_handler,
                              NULL,
                              CONFIG_SYS_TIMER_STACK_SIZE,
                              CONFIG_SYS_TIMER_TASK_PRIORITY);
        if (t_task == NULL)
        {
            err("timer_thread create fail\n");
            t_return = SYS_ERR_THREAD_MALLOC_FAIL;
            break;
        }

        t_return = timer_running_task_create();
        if (t_return != SYS_ERR_OK)
        {
            break;
        }

        t_return = debug_timer_init();
        if (t_return != SYS_ERR_OK)
        {
            break;
        }
        u32_timer_init = 1;
    } while (0);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}

sys_timer_t *sys_timer_create(const char *name, uint32_t period, uint32_t auto_reload, sys_timer_pri_t priority, uint32_t execute_mode, void *cb_param, sys_timer_cb cb_function)
{
    sys_timer_t *new_timer;
    sys_timer_err_t result;

    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    result = timer_parameter_check(SYS_TIMER_CMD_CHANGE_PERIOD, period);
    if (result < 0)
    {
        TIMER_ERR_LOG();
        return NULL;
    }
    result = timer_parameter_check(SYS_TIMER_CMD_CHANGE_PRIORITY, priority);
    if (result < 0)
    {
        TIMER_ERR_LOG();
        return NULL;
    }
    result = timer_parameter_check(SYS_TIMER_CMD_CHANGE_EXECUTE_MODE, execute_mode);
    if (result < 0)
    {
        TIMER_ERR_LOG();
        return NULL;
    }

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    new_timer = (sys_timer_t *)mem_malloc(sizeof(sys_timer_t));

    if (new_timer)
    {
        memset(new_timer, 0x0, sizeof(sys_timer_t));
        list_init(&new_timer->list);
        new_timer->name = name;
        new_timer->period = period;
        new_timer->auto_reload = auto_reload ? 1 : 0;
        new_timer->priority = priority;
        new_timer->execute_mode = execute_mode;
        new_timer->cb_param = cb_param;
        new_timer->cb_function = cb_function;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return new_timer;
}

sys_timer_err_t sys_timer_start(sys_timer_t *timer)
{
    return SYS_TIMER_START(timer);
}

sys_timer_err_t sys_timer_stop(sys_timer_t *timer)
{
    return SYS_TIMER_STOP(timer);
}

sys_timer_err_t sys_timer_reset(sys_timer_t *timer)
{
    return SYS_TIMER_RESET(timer);
}

sys_timer_err_t sys_timer_change_period(sys_timer_t *timer, uint32_t period)
{
    return SYS_TIMER_CHANGE_PERIOD(timer, period);
}

sys_timer_err_t sys_timer_change_priority(sys_timer_t *timer, uint32_t priority)
{
    return SYS_TIMER_CHANGE_PRIORITY(timer, priority);
}

sys_timer_err_t sys_timer_change_execute_mode(sys_timer_t *timer, uint32_t execute_mode)
{
    return SYS_TIMER_CHANGE_EXECUTE_MODE(timer, execute_mode);
}

sys_timer_err_t sys_timer_delete(sys_timer_t *timer)
{
    return SYS_TIMER_DELETE(timer);
}

const char *sys_timer_get_timer_name(sys_timer_t *timer)
{
    return timer->name;
}

sys_timer_err_t sys_timer_start_from_isr(sys_timer_t *timer)
{
    return SYS_TIMER_START_FROM_ISR(timer);
}

sys_timer_err_t sys_timer_stop_from_isr(sys_timer_t *timer)
{
    return SYS_TIMER_STOP_FROM_ISR(timer);
}

sys_timer_err_t sys_timer_reset_from_isr(sys_timer_t *timer)
{
    return SYS_TIMER_RESET_FROM_ISR(timer);
}

sys_timer_err_t sys_timer_change_period_from_isr(sys_timer_t *timer, uint32_t period)
{
    return SYS_TIMER_CHANGE_PERIOD_FROM_ISR(timer, period);
}

sys_timer_err_t sys_timer_change_priority_from_isr(sys_timer_t *timer, uint32_t priority)
{
    return SYS_TIMER_CHANGE_PRIORITY_FROM_ISR(timer, priority);
}

sys_timer_err_t sys_timer_change_execute_mode_from_isr(sys_timer_t *timer, uint32_t execute_mode)
{
    return SYS_TIMER_CHANGE_EXECUTE_MODE_FROM_ISR(timer, execute_mode);
}

sys_timer_err_t sys_timer_delete_from_isr(sys_timer_t *timer)
{
    return SYS_TIMER_DELETE_FROM_ISR(timer);
}
bool sys_timer_get_running(sys_timer_t *timer)
{
    return timer->running;
}
uint32_t sys_timer_get_tick(void)
{
    return g_timer_ctrl.current_time;
}
