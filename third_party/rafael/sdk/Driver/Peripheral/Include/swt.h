/** @file swt.h
 *
 * @brief Software timer driver header file.
 *
 */

#ifndef __SWT_H__
#define __SWT_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
/**@brief Set of available timer operation types. */
typedef enum
{
    SWT_QUEUE_ENTRY_TYPE_NONE,                                                /**< Invalid timer operation type. */
    SWT_QUEUE_ENTRY_TYPE_START,                                               /**< Timer operation type Start. */
    SWT_QUEUE_ENTRY_TYPE_STOP,                                                /**< Timer operation type Stop. */
    SWT_QUEUE_ENTRY_TYPE_STOP_ALL                                             /**< Timer operation type Stop All. */
} swt_queue_entry_type_t;

/**@brief Timer modes. */
typedef enum
{
    SWT_MODE_SINGLE_SHOT,                                             /**< The timer will expire only once. */
    SWT_MODE_REPEATED                                                 /**< The timer will restart each time it expires. */
} swt_mode_t;

#define SWT_MIN_TIMEOUT_TICKS  5                                      /**< Minimum value of the timeout_ticks parameter of Swt_Start(). */
#define SWT_QUEUE_ENTRY_SIZE   sizeof(swt_queue_entry_t)  //24        /**< Size of app_timer.swt_queue_entry_t (only for use inside SWT_BUF_SIZE()). */
#define SWT_QUEUE_HEADER_SIZE  sizeof(swt_queue_header_t) //8         /**< Size of app_timer.swt_queue_header_t (only for use inside SWT_BUF_SIZE()). */

/**@brief Compute number of bytes required to hold the SWT data structures.
 * @param[in] OP_QUEUE_SIZE Size of queues holding timer operations that are pending execution.
 *                          Note that due to the queue implementation, this size must be one more
 *                          than the size that is actually needed.
 * @return Required SWT buffer size (in bytes).
 */
#define SWT_BUF_SIZE(OP_QUEUE_SIZE)                                              \
    (                                                                            \
        (                                                                        \
            (SWT_QUEUE_HEADER_SIZE + ((OP_QUEUE_SIZE) + 1) * SWT_QUEUE_ENTRY_SIZE)           \
        )                                                                        \
    )

#define MAX_SWT_TICK_COUNTER_VAL     0xFFFFFFFF                                  /**< Maximum value of the Software Timer Tick counter. */
#define MAX_SWT_TICK_TASKS_DELAY     47                                          /**< Maximum delay until an Software Timer Tick task is executed. */
#define SWT_TICK_COMPARE_OFFSET_MIN  3                                           /**< Minimum offset between the current Software Timer Tick counter value and the Capture Compare register. */


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/**
 * @brief SWT time-out handler type.
 *        This function is called when the requested number of samples has been processed.
 * @param p_context CB.
 */
typedef void (*swt_timeout_handler_t)(void *p_context);

/**@brief Type of function for passing events from the timer module to the scheduler. */
typedef uint32_t (*swt_evt_schedule_func_t) (swt_timeout_handler_t timeout_handler,
        void                 *p_context);


/**@brief Timer node type. The nodes will be used form a linked list of running timers.*/
typedef struct
{
    uint32_t                    ticks_to_expire;                            /**< Number of ticks from previous timer interrupt to timer expiry. */
    uint32_t                    ticks_at_start;                             /**< Current Software Timer Tick counter value when the timer was started. */
    uint32_t                    ticks_first_interval;                       /**< Number of ticks in the first timer interval. */
    uint32_t                    ticks_periodic_interval;                    /**< Timer period (for repeating timers). */
    bool                        is_running;                                 /**< True if timer is running, False otherwise. */
    swt_mode_t                  mode;                                       /**< Timer mode. */
    swt_timeout_handler_t       p_timeout_handler;                          /**< Pointer to function to be executed when the timer expires. */
    void                       *p_context;                                  /**< General purpose pointer. Will be passed to the timeout handler when the timer expires. */
    void                       *next;                                       /**< Pointer to the next node. */
} swt_node_t;

/**@brief Structure describing a timer start operation.*/
typedef struct
{
    uint32_t ticks_at_start;                                                /**< Current Software Timer Tick counter value when the timer was started. */
    uint32_t ticks_first_interval;                                          /**< Number of ticks in the first timer interval. */
    uint32_t ticks_periodic_interval;                                       /**< Timer period (for repeating timers). */
    void    *p_context;                                                     /**< General purpose pointer. Will be passed to the timeout handler when the timer expires. */
} swt_queue_entry_start_t;

/**@brief Structure describing a timer operation.*/
typedef struct
{
    swt_queue_entry_type_t op_type;                                           /**< Id of timer on which the operation is to be performed. */
    swt_node_t            *p_node;
    union
    {
        swt_queue_entry_start_t start;                                        /**< Structure describing a timer start operation. */
    } params;
} swt_queue_entry_t;

/**@brief Structure describing a timer user.
 * @details For each user of the timer module, there will be a timer operations queue. This queue
 *          will hold timer operations issued by this user until the timer interrupt handler
 *          processes these operations. The purpose of the queues is to be able to have a completely
 *          lockless timer implementation.
 */
typedef struct
{
    uint8_t            first;                                                    /**< Index of first entry to have been inserted in the queue (i.e. the next entry to be executed). */
    uint8_t            last;                                                     /**< Index of last entry to have been inserted in the queue. */
    uint8_t            swt_queue_size;                                           /**< Queue size. */
    swt_queue_entry_t *p_swt_queue_entry;                                        /**< Queue entry buffer. */
} swt_queue_header_t;



/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

/**
 * @brief Function for initializing the timer module.
 * Normally, initialization will both allocate the buffers needed by the timer module (including
 * aligning the buffers correctly) and take care of connecting the timer module to the scheduler
 * (if specified).
 * @param[in] evt_schedule_func Function for passing time-out events to the scheduler. Point to
 *                              swt_evt_schedule() to connect to the scheduler. Set to NULL
 *                              to make the timer module call the time-out handler directly from
 *                              the timer interrupt handler.
 * @retval STATUS_SUCCESS If the module was initialized successfully.
 * @retval STATUS_INVALID_PARAM If a parameter was invalid (buffer not aligned to a 4 byte boundary or NULL).
 */
uint32_t Swt_Init(swt_evt_schedule_func_t evt_schedule_func);


/**
 * @brief Function for starting a timer.
 * @param[in] p_node Pointer to timer node.
 * @param[in] timeout_ticks Number of ticks (of Software Timer Tick, including prescaling) to time-out event.
 * @param[in] mode Timer mode.
 * @param[in] timeout_handler Function to be executed when the timer expires.
 * @param[in] p_context General purpose pointer. Will be passed to the time-out handler when the timer expires.
 * @retval STATUS_SUCCESS If the timer was successfully started.
 * @retval STATUS_INVALID_PARAM If a parameter was invalid.
 * @retval STATUS_ERROR If the application timer module has not been initialized or the timer has not been created,
 *                      or the timer operations queue was full.
 */
uint32_t Swt_Start(swt_node_t *p_node,
                   uint32_t timeout_ticks,
                   swt_mode_t mode,
                   swt_timeout_handler_t timeout_handler,
                   void *p_context);


/**
 * @brief Function for stopping the specified timer.
 * @param[in] p_node Pointer to timer node.
 * @retval STATUS_SUCCESS If the timer was successfully stopped.
 * @retval STATUS_INVALID_PARAM If a parameter was invalid.
 * @retval STATUS_ERROR If the application timer module has not been initialized or the timer has not been created,
 *                      or the timer operations queue was full.
 */
uint32_t Swt_Stop(swt_node_t *p_node);


/**
 * @brief Function for stopping all running timers.
 * @retval STATUS_SUCCESS If all timers were successfully stopped.
 * @retval STATUS_ERROR If the application timer module has not been initialized,
 *                      or the timer operations queue was full.
 */
uint32_t Swt_Stop_All(void);


/**
 * @brief Function for returning the current value of the Software Timer Tick counter.
 * @param[out] p_ticks Current value of the Software Timer Tick counter.
 * @retval STATUS_SUCCESS If the counter was successfully read.
 */
uint32_t Swt_Cnt_Get(uint32_t *p_ticks);


/**
 * @brief Function for computing the difference between two Software Timer Tick counter values.
 * @param[in] ticks_to Value returned by Swt_Cnt_Get().
 * @param[in] ticks_from Value returned by Swt_Cnt_Get().
 * @param[out] p_ticks_diff Number of ticks from ticks_from to ticks_to.
 * @retval STATUS_SUCCESS If the counter difference was successfully computed.
 */
uint32_t Swt_Cnt_Diff_Compute(uint32_t   ticks_to,
                              uint32_t   ticks_from,
                              uint32_t *p_ticks_diff);


/**@brief Function for returning the current running state of the specified timer.
 * @param[in] p_node Pointer to timer node.
 * @retval TRUE If the specified timer is running.
 * @retval FALSE If the specified timer has stopped.
 */
bool Swt_Is_Running(swt_node_t *p_node);


/**
 * @brief Function for checking for expired timers.
 * @param  None
 * @return None
 */
void Swt_Timeout_Handler(void);


#endif /* End of _SWT_H_ */



