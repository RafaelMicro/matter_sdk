/** @file swt.c
 *
 * @brief Software timer driver file.
 *
 */
/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "cm3_mcu.h"
#include "swt.h"


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define RT_TIMER0                 0
#define RT_TIMER1                 1
#define RT_TIMER2                 2
#define RT_TIMER3                 3
#define RT_TIMER4                 4
#define MAX_NUMBER_OF_TIMER       5             /*RT570 has 5 timer*/
#define Timer_ID                  RT_TIMER3     /** @ Select one of the timers instead of Software Timer Tick*/

#define configTICK_RATE_HZ                     1000
#define configTIMER_INTERRUPT_PRIORITY         255

#define SWT_QUEUE_SIZE            32
#define CONTEXT_QUEUE_SIZE_MAX    2


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/


/**************************************************************************************************
 *    GLOBAL VARIABLES
 *************************************************************************************************/
static swt_queue_header_t *p_swt_queue = NULL;                                  /**< Array of timer users. */
static swt_node_t *swt_id_head;                                                 /**< First timer in list of running timers. */
static uint32_t swt_ticks_latest;                                               /**< Last known Software Timer Tick counter value. */
static uint32_t swt_ticks_elapsed[CONTEXT_QUEUE_SIZE_MAX];                      /**< Timer internal elapsed ticks queue. */
static uint8_t swt_ticks_elapsed_q_read_ind;                                    /**< Timer internal elapsed ticks queue read index. */
static uint8_t swt_ticks_elapsed_q_write_ind;                                   /**< Timer internal elapsed ticks queue write index. */
static swt_evt_schedule_func_t swt_evt_schedule_func;                           /**< Pointer to function for propagating timeout events to the scheduler. */
static bool swt_tick_running;                                                   /**< Boolean indicating if Software Timer Tick is running. */
static bool swt_tick_reset;                                                     /**< Boolean indicating if Software Timer Tick counter has been reset due to last timer removed from timer list during the timer list handling. */
uint32_t swt_tick_cnt = 0;                                                      /**< Software Timer Tick counter value. */

timern_t *TIMER;
timern_t *Timer_Base[MAX_NUMBER_OF_TIMER] = {TIMER0, TIMER1, TIMER2, TIMER3, TIMER4};

static uint8_t swt_queue_buffer[SWT_BUF_SIZE(SWT_QUEUE_SIZE)];


/**************************************************************************************************
 *    LOCAL FUNCTIONS
 *************************************************************************************************/
void Swt_Tick_Timer_Int_Callback(uint32_t timer_id);


/**************************************************************************************************
 *    GLOBAL FUNCTIONS
 *************************************************************************************************/

/**
 * @brief Function for timer deinitialization to cancel the feature as software timer tick.
 * @param[in] timer_id Timer ID.
 * @return None
 */
void Timer_DeInit(uint32_t timer_id)
{
    TIMER = Timer_Base[timer_id];

    switch (timer_id)
    {
    case RT_TIMER0:
        NVIC_DisableIRQ((IRQn_Type)(Timer0_IRQn));
        break;

    case RT_TIMER1:
        NVIC_DisableIRQ((IRQn_Type)(Timer1_IRQn));
        break;

    case RT_TIMER2:
        NVIC_DisableIRQ((IRQn_Type)(Timer2_IRQn));
        break;

    case RT_TIMER3:
        NVIC_DisableIRQ((IRQn_Type)(Timer3_IRQn));
        break;

    case RT_TIMER4:
        NVIC_DisableIRQ((IRQn_Type)(Timer4_IRQn));
        break;

    default:
        break;
    }

    TIMER->LOAD = 0;
    TIMER->CONTROL.reg = 0;
    TIMER->CLEAR = 1;    /*clear interrupt REQ*/
}


/**
 * @brief Function for timer configuration to initialize the feature as software timer tick.
 * @param[in] timer_id Timer ID.
 * @param[in] Timer_Init Timer initialization parameters.
 * @param[in] timer_handler Register Timer interrupt callback function handler.
 * @return None
 */
void Timer_Config(uint32_t timer_id, timern_t Timer_Init, timer_isr_handler_t timer_handler)
{
    TIMER = Timer_Base[timer_id];

    switch (timer_id)
    {
    case RT_TIMER0:
        NVIC_DisableIRQ((IRQn_Type)(Timer0_IRQn));
        break;

    case RT_TIMER1:
        NVIC_DisableIRQ((IRQn_Type)(Timer1_IRQn));
        break;

    case RT_TIMER2:
        NVIC_DisableIRQ((IRQn_Type)(Timer2_IRQn));
        break;

    case RT_TIMER3:
        NVIC_DisableIRQ((IRQn_Type)(Timer3_IRQn));
        break;

    case RT_TIMER4:
        NVIC_DisableIRQ((IRQn_Type)(Timer4_IRQn));
        break;

    default:
        break;
    }

    TIMER->LOAD = 0;
    TIMER->CONTROL.reg = 0;
    TIMER->CLEAR = 1;    /*clear interrupt REQ*/

    if (Timer_Init.CONTROL.bit.INT_ENABLE == 1)
    {
        TIMER->CLEAR = 1;    /*clear interrupt REQ*/

        if (timer_handler != NULL)
        {
            Timer_Int_Callback_Register(timer_id, timer_handler);
        }

        switch (timer_id)
        {
        case RT_TIMER0:
            NVIC_SetPriority((IRQn_Type)(Timer0_IRQn), configTIMER_INTERRUPT_PRIORITY);
            NVIC_EnableIRQ((IRQn_Type)(Timer0_IRQn));
            break;

        case RT_TIMER1:
            NVIC_SetPriority((IRQn_Type)(Timer1_IRQn), configTIMER_INTERRUPT_PRIORITY);
            NVIC_EnableIRQ((IRQn_Type)(Timer1_IRQn));
            break;

        case RT_TIMER2:
            NVIC_SetPriority((IRQn_Type)(Timer2_IRQn), configTIMER_INTERRUPT_PRIORITY);
            NVIC_EnableIRQ((IRQn_Type)(Timer2_IRQn));
            break;

        case RT_TIMER3:
            NVIC_SetPriority((IRQn_Type)(Timer3_IRQn), configTIMER_INTERRUPT_PRIORITY);
            NVIC_EnableIRQ((IRQn_Type)(Timer3_IRQn));
            break;

        case RT_TIMER4:
            NVIC_SetPriority((IRQn_Type)(Timer4_IRQn), configTIMER_INTERRUPT_PRIORITY);
            NVIC_EnableIRQ((IRQn_Type)(Timer4_IRQn));
            break;

        default:
            break;
        }
        //NVIC_EnableIRQ((IRQn_Type)(TIMER0_IRQn+timer_id));
    }

    TIMER->LOAD = Timer_Init.LOAD;
    TIMER->CONTROL.reg = Timer_Init.CONTROL.reg;
}


/**
 * @brief Function for initializing and starting the Software Timer Tick counter.
 * @param[in] None
 * @return None
 */
static void Swt_Tick_Init(void)
{
    timern_t Timer_Init;


#if (EXT32K_GPIO_ENABLE==1)
    /** @ Timer interrupt enable, timerClk = 32KHz, clock is divided by 1, timer clock = 1KHz (1ms), load = 32, timer interrupt = 1ms*/
    Timer_Init.CONTROL.reg = 0;
    Timer_Init.CONTROL.bit.INT_ENABLE = 1;
    Timer_Init.CONTROL.bit.PRESCALE = 0;    /*clock is divided by 32*/
    Timer_Init.CONTROL.bit.MODE = 1;        /*Periodic mode*/
    Timer_Init.CONTROL.bit.EN = 1;
    Timer_Init.LOAD = 32;
#else
    /** @ Timer interrupt enable, timerClk = 40KHz, clock is divided by 1, timer clock = 1KHz (1ms), load = 39, timer interrupt = 1ms*/
    Timer_Init.CONTROL.reg = 0;
    Timer_Init.CONTROL.bit.INT_ENABLE = 1;
    Timer_Init.CONTROL.bit.PRESCALE = 0;    /*clock is divided by 1*/
    Timer_Init.CONTROL.bit.MODE = 1;        /*Periodic mode*/
    Timer_Init.CONTROL.bit.EN = 1;
    Timer_Init.LOAD = 39;
#endif



#if (Timer_ID == RT_TIMER3)
    Timer_Config(Timer_ID, Timer_Init, Swt_Tick_Timer_Int_Callback);
#endif

    swt_tick_cnt = 0;

    swt_tick_running = TRUE;
}


/**
 * @brief Function for stopping the Software Timer Tick timer.
 * @param[in] None
 * @return None
 */
static void Swt_Tick_Stop(void)
{
#if (Timer_ID == RT_TIMER3)
    Timer_DeInit(Timer_ID);
#endif

    swt_ticks_latest = 0;

    swt_tick_running = FALSE;

    swt_tick_cnt = 0;
}


/**
 * @brief Function for returning the current value of the Software Timer Tick counter.
 * @param[in] None
 * @return Current value of the Software Timer Tick counter.
 */
static __INLINE uint32_t Swt_Tick_Counter_Get(void)
{
    return swt_tick_cnt;
}


/**
 * @brief Function for computing the difference between two Software Timer Tick counter values.
 * @param[in] ticks_now Now Software Timer Tick counter values.
 * @param[in] ticks_old Old Software Timer Tick counter values.
 * @return Number of ticks elapsed from ticks_old to ticks_now.
 */
static __INLINE uint32_t Swt_Ticks_Diff_Get(uint32_t ticks_now, uint32_t ticks_old)
{
    return ((ticks_now - ticks_old) & MAX_SWT_TICK_COUNTER_VAL);
}


/**
 * @brief Function for scheduling a check for timeouts by generating a Software Timer Tick interrupt.
 * @param[in] None
 * @return None
 */
static void Swt_Timeouts_Check_Sched(void)
{
#if (Timer_ID == RT_TIMER3)
    //NVIC_SetPendingIRQ(Timer3_IRQn);
    Swt_Timeout_Handler();
#endif
}


static void Swt_List_Handler(void);


/**
 * @brief Function for scheduling a timer list update.
 * @param[in] None
 * @return None
 */
static void Swt_List_Handler_sched(void)
{
    Swt_List_Handler();
    //Swi_Int_Trigger(SWI_ID_0);
}


/**
 * @brief Function for executing an application timeout handler, either by calling it directly, or by passing time-out event to the scheduler.
 * @param[in] p_node Pointer to expired timer.
 * @return None
 */
static void Swt_Timeout_Handler_Exec(swt_node_t *p_node)
{
    uint32_t err_code __attribute__((unused));

    if (swt_evt_schedule_func != NULL)
    {
        err_code = swt_evt_schedule_func(p_node->p_timeout_handler, p_node->p_context);
    }
    else
    {
        p_node->p_timeout_handler(p_node->p_context);
    }
}


/**
 * @brief Function for inserting a timer in the timer list.
 * @param[in] p_node Pointer to the timer to be inserted.
 * @return None
 */
static void Swt_List_Insert(swt_node_t *p_node)
{
    if (swt_id_head == NULL)
    {
        swt_id_head = p_node;
    }
    else
    {
        if (p_node->ticks_to_expire <= swt_id_head->ticks_to_expire)
        {
            swt_id_head->ticks_to_expire -= p_node->ticks_to_expire;

            p_node->next   = swt_id_head;
            swt_id_head = p_node;
        }
        else
        {
            swt_node_t *p_previous;
            swt_node_t *p_current;
            uint32_t       ticks_to_expire;

            ticks_to_expire   = p_node->ticks_to_expire;
            p_previous        = swt_id_head;
            p_current         = swt_id_head;

            while ((p_current != NULL) && (ticks_to_expire > p_current->ticks_to_expire))
            {
                ticks_to_expire   -= p_current->ticks_to_expire;
                p_previous         = p_current;
                p_current          = p_current->next;
            }

            if (p_current != NULL)
            {
                p_current->ticks_to_expire -= ticks_to_expire;
            }

            p_node->ticks_to_expire = ticks_to_expire;
            p_node->next            = p_current;
            p_previous->next         = p_node;
        }
    }
}


/**
 * @brief Function for removing a timer from the timer list.
 * @param[in] p_node Pointer to the timer to be removed.
 * @return None
 */
static void Swt_List_Remove(swt_node_t *p_node)
{
    swt_node_t *p_previous;
    swt_node_t *p_current;
    uint32_t    timeout;

    // Find the timer's position in timer list.
    p_previous = swt_id_head;
    p_current  = p_previous;

    while (p_current != NULL)
    {
        if (p_current == p_node)
        {
            break;
        }
        p_previous = p_current;
        p_current  = p_current->next;
    }

    // Timer not in active list.
    if (p_current == NULL)
    {
        return;
    }

    // Timer is the first in the list
    if (p_previous == p_current)
    {
        swt_id_head = swt_id_head->next;

        // No more timers in the list. Reset Software Timer Tick in case Start timer operations are present in the queue.
        if (swt_id_head == NULL)
        {
            swt_ticks_latest        = 0;
            swt_tick_reset          = TRUE;
        }
    }

    // Remaining timeout between next timeout.
    timeout = p_current->ticks_to_expire;

    // Link previous timer with next of this timer, i.e. removing the timer from list.
    p_previous->next = p_current->next;

    // If this is not the last timer, increment the next timer by this timer timeout.
    p_current = p_previous->next;
    if (p_current != NULL)
    {
        p_current->ticks_to_expire += timeout;
    }
}


/**
 * @brief Function for acquiring the number of ticks elapsed.
 * @param[in] p_ticks_elapsed Number of ticks elapsed.
 * @return TRUE if elapsed ticks was read from queue, FALSE otherwise.
 */
static bool Swt_Elapsed_Ticks_Acquire(uint32_t *p_ticks_elapsed)
{
    // Pick the elapsed value from queue.
    if (swt_ticks_elapsed_q_read_ind != swt_ticks_elapsed_q_write_ind)
    {
        // Dequeue elapsed value.
        swt_ticks_elapsed_q_read_ind++;
        if (swt_ticks_elapsed_q_read_ind == CONTEXT_QUEUE_SIZE_MAX)
        {
            swt_ticks_elapsed_q_read_ind = 0;
        }

        *p_ticks_elapsed = swt_ticks_elapsed[swt_ticks_elapsed_q_read_ind];

        swt_ticks_latest += *p_ticks_elapsed;
        swt_ticks_latest &= MAX_SWT_TICK_COUNTER_VAL;

        return TRUE;
    }
    else
    {
        // No elapsed value in queue.
        *p_ticks_elapsed = 0;
        return FALSE;
    }
}


/**
 * @brief Function for handling the timer list deletions.
 * @param[in] None
 * @return TRUE if Capture Compare register must be updated, FALSE otherwise.
 */
static bool Swt_List_Deletions_Handler(void)
{
    swt_node_t *p_timer_old_head;

    // Remember the old head, so as to decide if new compare needs to be set.
    p_timer_old_head = swt_id_head;

    swt_queue_header_t *p_queue         = p_swt_queue;
    uint8_t        queue_entry_first = p_queue->first;

    while (queue_entry_first != p_queue->last)
    {
        swt_queue_entry_t *p_queue_entry = &p_queue->p_swt_queue_entry[queue_entry_first];

        // Traverse to next operation in queue.
        queue_entry_first++;
        if (queue_entry_first == p_queue->swt_queue_size)
        {
            queue_entry_first = 0;
        }

        switch (p_queue_entry->op_type)
        {
        case SWT_QUEUE_ENTRY_TYPE_STOP:
            // Delete node if timer is running.
            Swt_List_Remove(p_queue_entry->p_node);
            break;

        case SWT_QUEUE_ENTRY_TYPE_STOP_ALL:
            // Delete list of running timers, and mark all timers as not running.
            while (swt_id_head != NULL)
            {
                swt_node_t *p_head = swt_id_head;

                p_head->is_running = FALSE;
                swt_id_head    = p_head->next;
            }
            break;

        default:
            // No implementation needed.
            break;
        }
    }

    // Detect change in head of the list.
    return (swt_id_head != p_timer_old_head);
}


/**
 * @brief Function for updating the timer list for expired timers.
 * @param[in] ticks_elapsed Number of elapsed ticks.
 * @param[in] ticks_previous Previous known value of the Software Timer Tick counter.
 * @param[in] p_restart_list_head
 * @return None
 */
static void Swt_Expired_Timers_Handler(uint32_t         ticks_elapsed,
                                       uint32_t         ticks_previous,
                                       swt_node_t   **p_restart_list_head)
{
    uint32_t ticks_expired = 0;

    while (swt_id_head != NULL)
    {
        swt_node_t *p_node;
        swt_node_t *p_node_expired;

        // Auto variable for current timer node.
        p_node = swt_id_head;

        // Do nothing if timer did not expire
        if (ticks_elapsed < p_node->ticks_to_expire)
        {
            p_node->ticks_to_expire -= ticks_elapsed;
            break;
        }

        // Decrement ticks_elapsed and collect expired ticks.
        ticks_elapsed -= p_node->ticks_to_expire;
        ticks_expired += p_node->ticks_to_expire;

        // Timer expired, set ticks_to_expire zero.
        p_node->ticks_to_expire = 0;

        // Remove the expired timer from head.
        p_node_expired  = swt_id_head;
        swt_id_head = p_node->next;

        // Timer will be restarted if periodic.
        if (p_node->ticks_periodic_interval != 0)
        {
            p_node->ticks_at_start       = (ticks_previous + ticks_expired) & MAX_SWT_TICK_COUNTER_VAL;
            p_node->ticks_first_interval = p_node->ticks_periodic_interval;
            p_node->next                 = *p_restart_list_head;
            *p_restart_list_head          = p_node_expired;
        }
    }
}


/**
 * @brief Function for handling timer list insertions.
 * @param[in] p_restart_list_head List of repeating timers to be restarted.
 * @return TRUE if Capture Compare register must be updated, FALSE otherwise.
 */
static bool Swt_List_Insertions_Handler(swt_node_t *p_restart_list_head)
{
    swt_node_t *p_timer_id_old_head;

    // Remember the old head, so as to decide if new compare needs to be set.
    p_timer_id_old_head = swt_id_head;

    swt_queue_header_t *p_queue = p_swt_queue;

    // Handle insertions of timers.
    while ((p_restart_list_head != NULL) || (p_queue->first != p_queue->last))
    {
        swt_node_t *p_node;

        if (p_restart_list_head != NULL)
        {
            p_node           = p_restart_list_head;
            p_restart_list_head = p_node->next;
        }
        else
        {
            swt_queue_entry_t *p_queue_entry = &p_queue->p_swt_queue_entry[p_queue->first];

            p_queue->first++;
            if (p_queue->first == p_queue->swt_queue_size)
            {
                p_queue->first = 0;
            }

            p_node = p_queue_entry->p_node;

            if ((p_queue_entry->op_type != SWT_QUEUE_ENTRY_TYPE_START) || p_node->is_running)
            {
                continue;
            }

            p_node->ticks_at_start          = p_queue_entry->params.start.ticks_at_start;
            p_node->ticks_first_interval    = p_queue_entry->params.start.ticks_first_interval;
            p_node->ticks_periodic_interval = p_queue_entry->params.start.ticks_periodic_interval;
            p_node->p_context               = p_queue_entry->params.start.p_context;

            if (swt_tick_reset)
            {
                p_node->ticks_at_start = 0;
            }
        }

        // Prepare the node to be inserted.
        if (
            ((p_node->ticks_at_start - swt_ticks_latest) & MAX_SWT_TICK_COUNTER_VAL)
            <
            (MAX_SWT_TICK_COUNTER_VAL / 2)
        )
        {
            p_node->ticks_to_expire = Swt_Ticks_Diff_Get(p_node->ticks_at_start, swt_ticks_latest) +
                                      p_node->ticks_first_interval;
        }
        else
        {
            uint32_t delta_current_start;

            delta_current_start = Swt_Ticks_Diff_Get(swt_ticks_latest, p_node->ticks_at_start);
            if (p_node->ticks_first_interval > delta_current_start)
            {
                p_node->ticks_to_expire = p_node->ticks_first_interval - delta_current_start;
            }
            else
            {
                p_node->ticks_to_expire = 0;
            }
        }

        p_node->ticks_at_start       = 0;
        p_node->ticks_first_interval = 0;
        p_node->is_running           = TRUE;
        p_node->next                 = NULL;

        // Insert into list
        Swt_List_Insert(p_node);
    }

    return (swt_id_head != p_timer_id_old_head);
}


/**
 * @brief Function for updating the Capture Compare register.
 * @param[in] p_timer_id_head_old
 * @return None
 */
static void Swt_Compare_Reg_Update(swt_node_t *p_timer_id_head_old)
{
    // Setup the timeout for timers on the head of the list
    if (swt_id_head != NULL)
    {
        uint32_t ticks_to_expire = swt_id_head->ticks_to_expire;
        uint32_t pre_counter_val = Swt_Tick_Counter_Get();
        uint32_t cc              = swt_ticks_latest;
        uint32_t ticks_elapsed   = Swt_Ticks_Diff_Get(pre_counter_val, cc) + SWT_TICK_COMPARE_OFFSET_MIN;

        if (!swt_tick_running)
        {
            // No timers were already running, start SWT Tick
            Swt_Tick_Init();
        }

        cc += (ticks_elapsed < ticks_to_expire) ? ticks_to_expire : ticks_elapsed;
        cc &= MAX_SWT_TICK_COUNTER_VAL;

        uint32_t post_counter_val = Swt_Tick_Counter_Get();

        if (
            (Swt_Ticks_Diff_Get(post_counter_val, pre_counter_val) + SWT_TICK_COMPARE_OFFSET_MIN)
            >
            Swt_Ticks_Diff_Get(cc, pre_counter_val)
        )
        {
            Delay_us(MAX_SWT_TICK_TASKS_DELAY);
            Swt_Timeouts_Check_Sched();
        }
    }
    else
    {
        // No timers are running, stop Software Timer Tick
        Swt_Tick_Stop();
    }
}


/**
 * @brief Function for handling changes to the timer list.
 * @param[in] None
 * @return None
 */
static void Swt_List_Handler(void)
{
    swt_node_t *p_restart_list_head = NULL;

    uint32_t       ticks_elapsed;
    uint32_t       ticks_previous;
    bool           ticks_have_elapsed;
    bool           compare_update;
    swt_node_t *p_timer_id_head_old;

    // Back up the previous known tick and previous list head
    ticks_previous    = swt_ticks_latest;
    p_timer_id_head_old = swt_id_head;

    // Get number of elapsed ticks
    ticks_have_elapsed = Swt_Elapsed_Ticks_Acquire(&ticks_elapsed);

    // Handle list deletions
    compare_update = Swt_List_Deletions_Handler();

    // Handle expired timers
    if (ticks_have_elapsed)
    {
        Swt_Expired_Timers_Handler(ticks_elapsed, ticks_previous, &p_restart_list_head);
        compare_update = TRUE;
    }

    // Handle list insertions
    if (Swt_List_Insertions_Handler(p_restart_list_head))
    {
        compare_update = TRUE;
    }

    // Update compare register if necessary
    if (compare_update)
    {
        Swt_Compare_Reg_Update(p_timer_id_head_old);
    }
    swt_tick_reset = FALSE;
}


/**
 * @brief Function for enqueueing a new operations queue entry.
 * @param[in] p_queue Queue that the entry is to be enqueued for.
 * @param[in] last_index Index of the next last index to be enqueued.
 * @return None
 */
static void Swt_Queue_Entry_Enque(swt_queue_header_t *p_queue, uint8_t last_index)
{
    p_queue->last = last_index;
}


/**
 * @brief Function for allocating a new operations queue entry.
 * @param[in] p_queue Queue that the entry is to be allocated for.
 * @param[in] p_last_index Index of the next last index to be enqueued.
 * @return Pointer to allocated queue entry, or NULL if queue is full.
 */
static swt_queue_entry_t *Swt_Queue_Entry_Alloc(swt_queue_header_t *p_queue, uint8_t *p_last_index)
{
    uint8_t           last;
    swt_queue_entry_t *p_queue_entry;

    last = p_queue->last + 1;
    if (last == p_queue->swt_queue_size)
    {
        // Overflow case.
        last = 0;
    }
    if (last == p_queue->first)
    {
        // Queue is full.
        return NULL;
    }

    *p_last_index = last;
    p_queue_entry     = &p_queue->p_swt_queue_entry[p_queue->last];

    return p_queue_entry;
}


/**
 * @brief Function for scheduling a Timer Start operation.
 * @param[in] timer_id Id of timer to start.
 * @param[in] timeout_initial Time (in ticks) to first timer expiry.
 * @param[in] timeout_periodic Time (in ticks) between periodic expiries.
 * @param[in] p_context General purpose pointer. Will be passed to the timeout handler when the timer expires.
 * @return STATUS_SUCCESS on success, otherwise an error code.
 */
static uint32_t Swt_Start_Node_Schedule(swt_node_t *p_node,
                                        uint32_t     timeout_initial,
                                        uint32_t     timeout_periodic,
                                        void        *p_context)
{
    uint8_t last_index;

    swt_queue_entry_t *p_queue_entry = Swt_Queue_Entry_Alloc(p_swt_queue, &last_index);
    if (p_queue_entry == NULL)
    {
        return STATUS_ERROR;
    }

    p_queue_entry->op_type                              = SWT_QUEUE_ENTRY_TYPE_START;
    p_queue_entry->p_node                               = p_node;
    p_queue_entry->params.start.ticks_at_start          = Swt_Tick_Counter_Get();
    p_queue_entry->params.start.ticks_first_interval    = timeout_initial;
    p_queue_entry->params.start.ticks_periodic_interval = timeout_periodic;
    p_queue_entry->params.start.p_context               = p_context;

    Swt_Queue_Entry_Enque(p_swt_queue, last_index);

    Swt_List_Handler_sched();

    return STATUS_SUCCESS;
}


/**
 * @brief Function for scheduling a Timer Stop operation.
 * @param[in] timer_id Id of timer to stop.
 * @return STATUS_SUCCESS on successful scheduling a timer stop operation. STATUS_ERROR when there is no memory left to schedule the timer stop operation.
 */
static uint32_t Swt_Stop_Node_Schedule(swt_node_t *p_node)
{
    uint8_t last_index;

    swt_queue_entry_t *p_queue_entry = Swt_Queue_Entry_Alloc(p_swt_queue, &last_index);
    if (p_queue_entry == NULL)
    {
        return STATUS_ERROR;
    }

    p_queue_entry->op_type  = SWT_QUEUE_ENTRY_TYPE_STOP;
    p_queue_entry->p_node = p_node;

    Swt_Queue_Entry_Enque(p_swt_queue, last_index);

    Swt_List_Handler_sched();

    return STATUS_SUCCESS;
}


/**
 * @brief Function for scheduling a Timer Stop All operation.
 * @param[in] None
 * @return STATUS_SUCCESS on successful scheduling a timer stop all operation. STATUS_ERROR when there is no memory left to schedule the timer stop all operation.
 */
static uint32_t Swt_Stop_All_Node_Schedule(void)
{
    uint8_t last_index;

    swt_queue_entry_t *p_queue_entry = Swt_Queue_Entry_Alloc(p_swt_queue, &last_index);
    if (p_queue_entry == NULL)
    {
        return STATUS_ERROR;
    }

    p_queue_entry->op_type  = SWT_QUEUE_ENTRY_TYPE_STOP_ALL;
    p_queue_entry->p_node = NULL;

    Swt_Queue_Entry_Enque(p_swt_queue, last_index);

    Swt_List_Handler_sched();

    return STATUS_SUCCESS;
}


/**
 * @brief Function for handling the TIMER interrupt callback.
 * @details Checks for timeouts, and executes timeout handlers for tick timer.
 * @param[in] timer_id Timer ID.
 * @return None
 */
void Swt_Tick_Timer_Int_Callback(uint32_t timer_id)
{
    swt_tick_cnt++;

    return;
}

/**
 * @brief Function for handling the software interrupt callback.
 * @details Checks for timeouts, and executes timeout handlers for tick timer.
 * @param[in] swi_id Software interrupt ID.
 * @return None
 */
void Swi_Int_Callback_Handler_0(uint32_t swi_id)
{
    if (swi_id != SWI_ID_0)
    {
        ASSERT();
    }

    Swt_List_Handler();
}

/**
 * @brief Function for handling the software interrupt initinal
 * @details Softare Interrupt clera the callback function and Register SWI_ID_0, Interrupt callback handler 0
 * @param  None
 * @return STATUS_SUCCESS SWI clear and enable the callback function is successful.
 */
uint32_t Swi_Init(void)
{
    Swi_Int_Callback_Clear();
    Swi_Int_Enable(SWI_ID_0, Swi_Int_Callback_Handler_0);

    return STATUS_SUCCESS;
}


uint32_t Swt_Init(swt_evt_schedule_func_t evt_schedule_func)
{
    swt_queue_entry_t *p_queue_entry;
    swt_queue_header_t *p_queue;
    uint8_t swt_queues_size;

    //Swi_Init();                          /*Init SWI*/

    // Stop Software Timer Tick to prevent any running timers from expiring (in case of reinitialization)
    Swt_Tick_Stop();

    swt_evt_schedule_func = evt_schedule_func;

    // Initialize swt queue buffer
    p_swt_queue = (swt_queue_header_t *)(&swt_queue_buffer[0]);

    // Skip swt queue header
    p_queue_entry = (swt_queue_entry_t *)(&swt_queue_buffer[sizeof(swt_queue_header_t)]);

    // Initialize operation queues
    p_queue = p_swt_queue;
    swt_queues_size = SWT_QUEUE_SIZE;

    p_queue->first            = 0;
    p_queue->last             = 0;
    p_queue->swt_queue_size   = swt_queues_size;
    p_queue->p_swt_queue_entry = p_queue_entry;

    swt_id_head                   = NULL;
    swt_ticks_elapsed_q_read_ind  = 0;
    swt_ticks_elapsed_q_write_ind = 0;

    Swt_Tick_Init();

    swt_ticks_latest = Swt_Tick_Counter_Get();

    return STATUS_SUCCESS;
}


uint32_t Swt_Start(swt_node_t *p_node,
                   uint32_t timeout_ticks,
                   swt_mode_t mode,
                   swt_timeout_handler_t timeout_handler,
                   void *p_context)
{
    uint32_t timeout_periodic;

    if (p_node == NULL)
    {
        return STATUS_INVALID_PARAM;
    }
    if (timeout_handler == NULL)
    {
        return STATUS_INVALID_PARAM;
    }
    if (p_node->is_running)
    {
        return STATUS_ERROR;
    }
    if (timeout_ticks < SWT_MIN_TIMEOUT_TICKS)
    {
        return STATUS_INVALID_PARAM;
    }

    p_node->is_running        = FALSE;
    p_node->mode              = mode;
    p_node->p_timeout_handler = timeout_handler;

    // Schedule timer start operation
    timeout_periodic = (p_node->mode == SWT_MODE_REPEATED) ? timeout_ticks : 0;

    return Swt_Start_Node_Schedule(p_node,
                                   timeout_ticks,
                                   timeout_periodic,
                                   p_context);
}

uint32_t Swt_Stop(swt_node_t *p_node)
{
    if ((p_node == NULL) || (p_node->p_timeout_handler == NULL))
    {
        return STATUS_INVALID_PARAM;
    }

    p_node->is_running = FALSE;
    // Schedule timer stop operation
    return Swt_Stop_Node_Schedule(p_node);
}


uint32_t Swt_Stop_All(void)
{
    return Swt_Stop_All_Node_Schedule();
}


uint32_t Swt_Cnt_Get(uint32_t *p_ticks)
{
    *p_ticks = Swt_Tick_Counter_Get();
    return STATUS_SUCCESS;
}


uint32_t Swt_Cnt_Diff_Compute(uint32_t   ticks_to,
                              uint32_t   ticks_from,
                              uint32_t *p_ticks_diff)
{
    *p_ticks_diff = Swt_Ticks_Diff_Get(ticks_to, ticks_from);
    return STATUS_SUCCESS;
}


bool Swt_Is_Running(swt_node_t *p_node)
{
    return (p_node->is_running == TRUE);
}



void Swt_Timeout_Handler(void)
{
    // Handle expired of timer
    if (swt_id_head != NULL)
    {
        swt_node_t *p_node;
        swt_node_t *p_previous_timer;
        uint32_t    ticks_elapsed;
        uint32_t    ticks_expired;

        // Initialize actual elapsed ticks being consumed to 0.
        ticks_expired = 0;

        // ticks elapsed from the latest tick to current tick.
        ticks_elapsed = Swt_Ticks_Diff_Get(Swt_Tick_Counter_Get(), swt_ticks_latest);

        // Auto variable containing the head of timers expiring.
        p_node = swt_id_head;

        // Expire all timers within ticks_elapsed and collect ticks_expired.
        while (p_node != NULL)
        {
            // Do nothing if timer did not expire.
            if (ticks_elapsed < p_node->ticks_to_expire)
            {
                break;
            }

            // Decrement ticks_elapsed and collect expired ticks.
            ticks_elapsed -= p_node->ticks_to_expire;
            ticks_expired += p_node->ticks_to_expire;

            // Move to next timer.
            p_previous_timer = p_node;
            p_node = p_node->next;

            // Execute Task.
            if (p_previous_timer->is_running)
            {
                p_previous_timer->is_running = FALSE;
                Swt_Timeout_Handler_Exec(p_previous_timer);
            }
        }

        // Prepare to queue the ticks expired in the m_ticks_elapsed queue.
        if (swt_ticks_elapsed_q_read_ind == swt_ticks_elapsed_q_write_ind)
        {
            // The read index of the queue is equal to the write index. This means the new
            // value of ticks_expired should be stored at a new location in the m_ticks_elapsed
            // queue (which is implemented as a double buffer).

            // Check if there will be a queue overflow.
            if (++swt_ticks_elapsed_q_write_ind == CONTEXT_QUEUE_SIZE_MAX)
            {
                // There will be a queue overflow. Hence the write index should point to the start
                // of the queue.
                swt_ticks_elapsed_q_write_ind = 0;
            }
        }

        // Queue the ticks expired.
        swt_ticks_elapsed[swt_ticks_elapsed_q_write_ind] = ticks_expired;

        Swt_List_Handler_sched();
    }
}
