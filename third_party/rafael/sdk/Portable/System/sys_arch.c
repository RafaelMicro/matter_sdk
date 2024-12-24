//=============================================================================
//                Include
//=============================================================================
#include "sys_arch.h"
#include "util_log.h"
//=============================================================================
//                Private Definitions of const value
//=============================================================================
#ifndef configMAX_TASK_NAME_LEN
#define MALLOC_DEBUG_INFO_TASK_SIZE         10
#else
#define MALLOC_DEBUG_INFO_TASK_SIZE         configMAX_TASK_NAME_LEN
#endif
#define MALLOC_DEBUG_INFO_SIZE              100

//=============================================================================
//                Private ENUM
//=============================================================================

//=============================================================================
//                Private Struct
//=============================================================================
typedef struct MALLOC_DEBUG_LOG_ENTRY
{
    char *func;
    uint32_t file_line  : 16;   // max 0xFFFF = 65535
    uint32_t malloc_len : 16;   // max 0xFFFF = 65535

    uint32_t malloc_offset  : 19;   // max 256k = 0x40000(19 bits)
    uint32_t valid          : 1;
    uint32_t                : 4;
    uint32_t task_index     : 8;
} malloc_log_entry_t;

typedef struct MALLOC_DEBUG_LOG
{
    char *task_name_ptr[MALLOC_DEBUG_INFO_TASK_SIZE];
    uint32_t malloc_base_addr;
    malloc_log_entry_t entry[MALLOC_DEBUG_INFO_SIZE];
} malloc_log_t;

//=============================================================================
//                Private Function Declaration
//=============================================================================

//=============================================================================
//                Private Global Variables
//=============================================================================
static uint32_t gu32_random_seed;

static malloc_log_t g_malloc_log;
//=============================================================================
//                Public Global Variables
//=============================================================================

//=============================================================================
//                Private Definition of Compare/Operation/Inline funciton/
//=============================================================================

//=============================================================================
//                Functions
//=============================================================================


//==================================
//     Mutex functions
//==================================
/** Create a new mutex
 * @param mutex Pointer to the mutex to create
 * @return a new mutex
 */
err_t sys_mutex_new(sys_mutex_t *p_mutex)
{
    err_t t_return = ERR_OK;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT(p_mutex != NULL);

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    *p_mutex = xSemaphoreCreateMutex();

    if (*p_mutex == NULL)
    {
        t_return = ERR_MEM;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}

/** Delete a semaphore
 * @param p_mutex the mutex to delete
 */
void sys_mutex_free(sys_mutex_t *p_mutex)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((p_mutex != NULL) && (*p_mutex != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (*p_mutex != NULL)
    {
        vSemaphoreDelete(*p_mutex);
        *p_mutex = NULL;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

/** Lock a mutex
 * @param p_mutex The mutex to lock
 */
void sys_mutex_lock(sys_mutex_t *p_mutex)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((p_mutex != NULL) && (*p_mutex != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    while (xSemaphoreTake(*p_mutex, portMAX_DELAY) != pdPASS);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

/** Try lock a mutex
 * @param p_mutex The mutex to lock
 * @return ERR_OK is success, and ERR_MEM is fail
 */
err_t sys_mutex_trylock(sys_mutex_t *p_mutex)
{
    err_t t_return = ERR_OK;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((p_mutex != NULL) && (*p_mutex != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (xSemaphoreTake(*p_mutex, 0) != pdPASS)
    {
        t_return = ERR_MEM;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}

/** Unlock a mutex
 * @param p_mutex the mutex to unlock
 */
void sys_mutex_unlock(sys_mutex_t *p_mutex)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((p_mutex != NULL) && (*p_mutex != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    xSemaphoreGive(*p_mutex);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

//==================================
//     Semaphore functions
//==================================
/** Create a new semaphore
 * @param sem pointer to the semaphore to create
 * @param count initial count of the semaphore
 * @return ERR_OK if successful, another err_t otherwise
 */
err_t sys_sem_new(sys_sem_t *sem, uint8_t u8_count_max, uint8_t u8_count_init)
{
    err_t t_return = ERR_OK;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT(sem != NULL);

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    *sem = xSemaphoreCreateCounting(u8_count_max, u8_count_init);

    if (*sem == NULL)
    {
        t_return = ERR_MEM;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}

/** Create a new semaphore
 * @param sem pointer to the semaphore to create
 * @param count initial count of the semaphore
 * @return ERR_OK if successful, another err_t otherwise
 */
err_t sys_binary_sem_new(sys_sem_t *sem)
{
    err_t t_return = ERR_OK;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT(sem != NULL);

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    *sem = xSemaphoreCreateBinary();

    if (*sem == NULL)
    {
        t_return = ERR_MEM;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}

/** Delete a semaphore
 * @param sem semaphore to delete
 */
void sys_sem_free(sys_sem_t *sem)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT(sem != NULL);

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (*sem != NULL)
    {
        vSemaphoreDelete(*sem);
        *sem = NULL;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

/** Signals a semaphore
 * @param sem the semaphore to signal
 */
void sys_sem_signal(sys_sem_t *sem)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((sem != NULL) && (*sem != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    xSemaphoreGive(*sem);
}

/** Signals a semaphore (from ISR)
 * @param sem the semaphore to signal
 */
void sys_sem_signal_isr(sys_sem_t *sem)
{
    BaseType_t context_switch = pdFALSE;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((sem != NULL) && (*sem != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    xSemaphoreGiveFromISR(*sem, &context_switch);
    portYIELD_FROM_ISR(context_switch);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

/** Blocks the thread while waiting for the semaphore to be signaled.
 *  If the "timeout" argument is non-zero, the thread should only be
 *  blocked for the specified time (measured in milliseconds).
 * @param sem the semaphore to wait for
 * @param timeout timeout in milliseconds to wait (0 = wait forever)
 * @return If the timeout argument is non-zero, the return value is
 *         the number of milliseconds spent waiting for the semaphore
 *         to be signaled. If the semaphore wasn't signaled within
 *         the specified time, the return value is SYS_ARCH_TIMEOUT.
 *         If the thread didn't have to wait for the semaphore(i.e.,
 *         it was already signaled), the function may return zero.
 */
uint32_t sys_sem_wait(sys_sem_t *sem, uint32_t u32_timeout)
{
    sys_tick_t StartTime, EndTime, Elapsed;
    uint32_t u32_return;

    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((sem != NULL) && (*sem != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    StartTime = xTaskGetTickCount();

    if (u32_timeout != 0)
    {
        if (xSemaphoreTake(*sem, u32_timeout * portTICK_RATE_MS) == pdTRUE)
        {
            EndTime = xTaskGetTickCount();
            Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;

            if (Elapsed == 0)
            {
                Elapsed = 1;
            }

            u32_return = Elapsed;
        }
        else
        {
            u32_return = SYS_ARCH_TIMEOUT;
        }
    }
    else
    {
        // must block without a timeout
        while (xSemaphoreTake(*sem, portMAX_DELAY) != pdTRUE);

        EndTime = xTaskGetTickCount();
        Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;

        if (Elapsed == 0)
        {
            Elapsed = 1;
        }

        u32_return = Elapsed;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return u32_return; // return time blocked
}

//==================================
//     Mailbox functions
//==================================
/** Creates an empty mailbox.
 * @param mbox The mbox handle
 * @param size the length of mbox
 */
err_t sys_mbox_new(sys_mbox_t *mbox, uint32_t size)
{
    err_t t_return = ERR_MEM;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT(mbox != NULL);

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    *mbox = xQueueCreate(size, sizeof(void *));

    if (*mbox != NULL)
    {
        t_return = ERR_OK;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}

/** Deallocates a mailbox. If there are messages still present in the
 *  mailbox when the mailbox is deallocated, it is an indication of a
 *  programming error and the developer should be notified.
 * @param mbox The mbox handle
 */
void sys_mbox_free(sys_mbox_t *mbox)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((mbox != NULL) && (*mbox != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    vQueueDelete(*mbox);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

/** Posts the "msg" to the mailbox.
 * @param mbox The mbox handle
 * @param msg The pointer of the msg
 */
void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((mbox != NULL) && (*mbox != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    while (xQueueSendToBack(*mbox, &msg, portMAX_DELAY) != pdTRUE);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

/** Posts the "msg" to the mailbox.
 * @param mbox The mbox handle
 * @param msg The pointer of the msg
 * @param u32_timeout millisecond timer
 */
err_t sys_mbox_post_with_timeout(sys_mbox_t *mbox, void *msg, uint32_t u32_timeout)
{
    err_t t_return = ERR_OK;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((mbox != NULL) && (*mbox != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (xQueueSendToBack(*mbox, &msg, u32_timeout) != pdTRUE)
    {
        t_return = ERR_TIMEOUT;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}

/** Try posts the "msg" to the mailbox.
 * @param mbox The mbox handle
 * @param msg The pointer of the msg
 */
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    err_t t_return = ERR_MEM;

    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((mbox != NULL) && (*mbox != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (xQueueSend(*mbox, &msg, (portTickType)0) == pdPASS)
    {
        t_return = ERR_OK;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}

/** Blocks the thread until a message arrives in the mailbox, but does
 *  not block the thread longer than "timeout" milliseconds (similar to
 *  the sys_sem_wait() function). The "msg" argument is a result
 *  parameter that is set by the function (i.e., by doing "*msg =
 *  ptr"). The "msg" parameter maybe NULL to indicate that the message
 *  should be dropped.
 * @param mbox The mbox handle
 * @param msg The pointer of the msg
 * @return The return values are the same as for the sys_arch_sem_wait()
 *         function: Number of milliseconds spent waiting or
 *         SYS_ARCH_TIMEOUT if there was a timeout.
 */
uint32_t sys_mbox_fetch(sys_mbox_t *mbox, void **msg, uint32_t u32_timeout)
{
    portTickType StartTime, EndTime, Elapsed;
    uint32_t u32_Return;

    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((mbox != NULL) && (*mbox != NULL));
    configASSERT(msg != NULL);

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    StartTime = xTaskGetTickCount();

    if (u32_timeout != 0)
    {
        if (pdTRUE == xQueueReceive(*mbox, msg, u32_timeout * portTICK_RATE_MS))
        {
            EndTime = xTaskGetTickCount();
            Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;

            if (Elapsed == 0)
            {
                Elapsed = 1;
            }

            u32_Return = Elapsed;
        }
        else
        {
            // timed out blocking for message
            *msg = NULL;
            u32_Return = SYS_ARCH_TIMEOUT;
        }
    }
    else
    {
        // block forever for a message.
        while (pdTRUE != xQueueReceive(*mbox, msg, portMAX_DELAY));

        EndTime = xTaskGetTickCount();
        Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;

        if (Elapsed == 0)
        {
            Elapsed = 1;
        }

        u32_Return = Elapsed;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return u32_Return; // return time blocked TBD test
}

uint32_t sys_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
    uint32_t u32_Return = SYS_ARCH_TIMEOUT;

    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((mbox != NULL) && (*mbox != NULL));
    configASSERT(msg != NULL);

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (pdTRUE == xQueueReceive(*mbox, msg, 0))
    {
        u32_Return = ERR_OK;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return u32_Return;
}

/** Get the number of free spaces in a queue.
 * @param queue The queue handle
 */
uint32_t sys_mbox_remaining_size(sys_mbox_t *mbox)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((mbox != NULL) && (*mbox != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return (uint32_t)uxQueueSpacesAvailable(*mbox);
}

//==================================
//     Queue functions
//==================================
/** Creates an empty queue.
 * @param queue The queue handle
 * @param queue_length the length of queue
 * @param item_size the size of item of queue
 */
err_t sys_queue_new(sys_queue_t *queue, uint32_t queue_length, uint32_t item_size)
{
    err_t t_return = ERR_MEM;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT(queue != NULL);

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    *queue = xQueueCreate(queue_length, item_size);

    if (*queue != NULL)
    {
        t_return = ERR_OK;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}

/** Deallocates a queue. If there are messages still present in the
 *  queue when the queue is deallocated, it is an indication of a
 *  programming error and the developer should be notified.
 * @param queue The queue handle
 */
void sys_queue_free(sys_queue_t *queue)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((queue != NULL) && (*queue != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    vQueueDelete(*queue);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

/** Send the "msg" to the queue.
 * @param queue The queue handle
 * @param msg The pointer of the msg
 */
void sys_queue_send(sys_queue_t *queue, void *msg)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((queue != NULL) && (*queue != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    while (xQueueSendToBack(*queue, msg, portMAX_DELAY) != pdTRUE);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

void sys_queue_send_from_isr(sys_queue_t *queue, void *msg)
{
    BaseType_t context_switch = pdFALSE;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((queue != NULL) && (*queue != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    xQueueSendToBackFromISR(*queue, msg, &context_switch);
    portYIELD_FROM_ISR(context_switch);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

/** Send the "msg" to the queue.
 * @param queue The queue handle
 * @param msg The pointer of the msg
 * @param u32_timeout millisecond timer
 */
err_t sys_queue_send_with_timeout(sys_queue_t *queue, void *msg, uint32_t u32_timeout)
{
    err_t t_return = ERR_OK;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((queue != NULL) && (*queue != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (xQueueSendToBack(*queue, msg, u32_timeout) != pdTRUE)
    {
        t_return = ERR_TIMEOUT;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}
void sys_queue_sendtofront(sys_queue_t *queue, void *msg)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((queue != NULL) && (*queue != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    while (xQueueSendToFront(*queue, msg, portMAX_DELAY) != pdTRUE);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

void sys_queue_sendtofront_from_isr(sys_queue_t *queue, void *msg)
{
    BaseType_t context_switch = pdFALSE;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((queue != NULL) && (*queue != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    xQueueSendToFrontFromISR(*queue, msg, &context_switch);
    portYIELD_FROM_ISR(context_switch);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

/** Send the "msg" to the queue.
 * @param queue The queue handle
 * @param msg The pointer of the msg
 * @param u32_timeout millisecond timer
 */
err_t sys_queue_sendtofront_with_timeout(sys_queue_t *queue, void *msg, uint32_t u32_timeout)
{
    err_t t_return = ERR_OK;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((queue != NULL) && (*queue != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (xQueueSendToFront(*queue, msg, u32_timeout) != pdTRUE)
    {
        t_return = ERR_TIMEOUT;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}
/** Try sends the "msg" to the queue.
 * @param queue The queue handle
 * @param msg The pointer of the msg
 */
err_t sys_queue_trysend(sys_queue_t *queue, void *msg)
{
    err_t t_return = ERR_MEM;

    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((queue != NULL) && (*queue != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (xQueueSend(*queue, msg, (portTickType)0) == pdPASS)
    {
        t_return = ERR_OK;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_return;
}

/** Blocks the thread until a message arrives in the queue, but does
 *  not block the thread longer than "timeout" milliseconds (similar to
 *  the sys_sem_wait() function). The "msg" argument is a result
 *  parameter that is set by the function (i.e., by doing "*msg =
 *  ptr"). The "msg" parameter maybe NULL to indicate that the message
 *  should be dropped.
 * @param queue The queue handle
 * @param msg The pointer of the msg
 * @return The return values are the same as for the sys_arch_sem_wait()
 *         function: Number of milliseconds spent waiting or
 *         SYS_ARCH_TIMEOUT if there was a timeout.
 */
uint32_t sys_queue_recv(sys_queue_t *queue, void *msg, uint32_t u32_timeout)
{
    portTickType StartTime, EndTime, Elapsed;
    uint32_t u32_Return;

    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((queue != NULL) && (*queue != NULL));
    configASSERT(msg != NULL);

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    StartTime = xTaskGetTickCount();

    if (u32_timeout != 0)
    {
        if (pdTRUE == xQueueReceive(*queue, msg, (u32_timeout * portTICK_RATE_MS)))
        {
            EndTime = xTaskGetTickCount();
            Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;

            if (Elapsed == 0)
            {
                Elapsed = 1;
            }

            u32_Return = Elapsed;
        }
        else
        {
            // timed out blocking for message
            u32_Return = SYS_ARCH_TIMEOUT;
        }
    }
    else
    {
        // block forever for a message.
        while (pdTRUE != xQueueReceive(*queue, msg, portMAX_DELAY));

        EndTime = xTaskGetTickCount();
        Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;

        if (Elapsed == 0)
        {
            Elapsed = 1;
        }

        u32_Return = Elapsed;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return u32_Return; // return time blocked TBD test
}

/** Get the number of free spaces in a queue.
 * @param queue The queue handle
 */
uint32_t sys_queue_remaining_size(sys_queue_t *queue)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((queue != NULL) && (*queue != NULL));

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return (uint32_t)uxQueueSpacesAvailable(*queue);
}

uint32_t sys_arch_queue_tryrecv(sys_queue_t *queue, void *msg)
{
    uint32_t u32_Return = SYS_ARCH_TIMEOUT;

    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    configASSERT((queue != NULL) && (*queue != NULL));
    configASSERT(msg != NULL);

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (pdTRUE == xQueueReceive(*queue, msg, 0))
    {
        u32_Return = ERR_OK;
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return u32_Return;
}

//==================================
//     Task functions
//==================================
/** creat task
 * @param name A descriptive name for the thread
 * @param thread Pointer to the thread entry function.
 * @param arg Pointer that will be used as the parameter for the thread being created.
 * @param u32_stacksize The size of the thread stack specified as the number of
 *     variables the stack can hold - not the number of bytes.
 * @param u32_priority The priority at which the thread should run.
 *     It must less than CONFIG_FREERTOS_MAX_PRIORITIES
 */
sys_task_t sys_task_new(const char *name,
                        sys_thread_fn thread,
                        void *arg,
                        uint32_t u32_stacksize,
                        uint32_t u32_priority
                       )
{
    sys_task_t t_thread = NULL;

    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    if (u32_priority >= configMAX_PRIORITIES)
    {
        return t_thread;
    }

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    xTaskCreate(thread, (const char *)name, u32_stacksize, arg, u32_priority, &t_thread);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return t_thread;
}

/** get a descriptive name for the thread
 * @return thread name
 */
char *sys_current_thread_name(void)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return pcTaskGetTaskName(xTaskGetCurrentTaskHandle());
}

/** get the thread priority
 * @param thread_handle
 * @return thread priority
 */
uint32_t sys_thread_priority_get(sys_task_t thread_handle)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return uxTaskPriorityGet(thread_handle);
}

/** set the thread priority
 * @param thread_handle
 * @param u32_priority The priority at which the thread should run.
 *     It must less than CONFIG_FREERTOS_MAX_PRIORITIES
 */
void sys_thread_priority_set(sys_task_t thread_handle, uint32_t u32_priority)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    vTaskPrioritySet(thread_handle, (UBaseType_t)u32_priority);

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

void sys_set_random_seed(uint32_t random_seed)
{
    gu32_random_seed = random_seed;
}

uint32_t sys_random(void)
{
    return sys_now() + gu32_random_seed * 1103515245 + 12345678;
}

//==================================
//     Time functions
//==================================
/** get current system time
 */
uint32_t sys_now(void)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return xTaskGetTickCount() * portTICK_RATE_MS;
}

/** get current system time
 */
uint32_t sys_now_from_isr(void)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return xTaskGetTickCountFromISR() * portTICK_RATE_MS;
}

/** task sleep
 * @param u32_ms sleep time in milliseconds
 */
void sys_msleep(uint32_t u32_ms)  /* only has a (close to) 1 jiffy resolution. */
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    if (u32_ms >= 1)
    {
        vTaskDelay(u32_ms / portTICK_PERIOD_MS);
    }

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

//==================================
//     Memory functions
//==================================
/** Task name pointer transfor to table index
 * @param task_name task name pointer
 * @return table index
 */
static uint8_t search_current_task_name_index(void)
{
    char *current_task_name = sys_current_thread_name();
    uint32_t u32_idx;

    for (u32_idx = 0; u32_idx < MALLOC_DEBUG_INFO_TASK_SIZE; u32_idx++)
    {
        if (g_malloc_log.task_name_ptr[u32_idx] == NULL)
        {
            g_malloc_log.task_name_ptr[u32_idx] = current_task_name;
            break;
        }
        else if (g_malloc_log.task_name_ptr[u32_idx] == current_task_name)
        {
            break;
        }
    }

    if (u32_idx < MALLOC_DEBUG_INFO_TASK_SIZE)
    {
        return u32_idx;
    }
    else
    {
        return 0xFF;
    }
}

static void malloc_info_insert(void *p_mptr, uint32_t u32_msize, char *pc_func_ptr, uint32_t u32_line)
{
    uint32_t u32_idx;

    if (g_malloc_log.malloc_base_addr == 0)
    {
        g_malloc_log.malloc_base_addr = (uint32_t)p_mptr & 0xFFF00000;
    }

    for (u32_idx = 0; u32_idx < MALLOC_DEBUG_INFO_SIZE; u32_idx++)
    {
        if (g_malloc_log.entry[u32_idx].valid == 0)
        {
            g_malloc_log.entry[u32_idx].func = pc_func_ptr;
            g_malloc_log.entry[u32_idx].file_line = u32_line & 0xFFFF;
            g_malloc_log.entry[u32_idx].malloc_len = u32_msize & 0xFFFF;
            g_malloc_log.entry[u32_idx].malloc_offset = (uint32_t)p_mptr & 0x7FFFF; // max 512k
            //g_malloc_log.entry[u32_idx].task_index = search_current_task_name_index();
            g_malloc_log.entry[u32_idx].valid = 1;
            break;
        }
    }
}

static void malloc_info_delete(void *p_mptr)
{
    uint32_t u32_idx, malloc_offset;

    malloc_offset = (uint32_t)p_mptr & 0xFFFFF;

    for (u32_idx = 0; u32_idx < MALLOC_DEBUG_INFO_SIZE; u32_idx++)
    {
        if (g_malloc_log.entry[u32_idx].valid)
        {
            if (g_malloc_log.entry[u32_idx].malloc_offset == malloc_offset)
            {
                memset(&g_malloc_log.entry[u32_idx], 0x0, sizeof(g_malloc_log.entry[u32_idx]));
                break;
            }
        }
    }
}

void sys_malloc_info_printf(void)
{
    uint32_t u32_idx;
    info("+------------+-------+---------------------------\n");
    info("|  Pointer   | Size  | Func-Line(Task)\n");
    info("+------------+-------+---------------------------\n");
    for (u32_idx = 0; u32_idx < MALLOC_DEBUG_INFO_SIZE; u32_idx++)
    {
        if (g_malloc_log.entry[u32_idx].valid)
        {
            info("| 0x%08X | %05u | %s-%d(%s)\n",
                 g_malloc_log.malloc_base_addr | g_malloc_log.entry[u32_idx].malloc_offset,
                 g_malloc_log.entry[u32_idx].malloc_len,
                 g_malloc_log.entry[u32_idx].func,
                 g_malloc_log.entry[u32_idx].file_line,
                 g_malloc_log.task_name_ptr[g_malloc_log.entry[u32_idx].task_index]);
        }
    }
    info("+------------+-------+---------------------------\n");
    info("   Free Size : 0x%06X(%d), Min 0x%06X(%d)\n", sys_get_free_size(), sys_get_free_size(),
         xPortGetMinimumEverFreeHeapSize(), xPortGetMinimumEverFreeHeapSize());
}
/** Memory allocation
 * @param u32_size sleep time in milliseconds
 */
void *sys_malloc_fn(uint32_t u32_size, const char *pc_func_ptr, uint32_t u32_line)
{
    void *ptr;
    ptr = NULL;
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    if (u32_size == 0)
    {
        return NULL;
    }

    if (u32_size & portBYTE_ALIGNMENT_MASK)
    {
        u32_size += portBYTE_ALIGNMENT - (u32_size & portBYTE_ALIGNMENT_MASK);
    }

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    msg(UTIL_LOG_DEBUG, "@@@@@ malloc %p(%d) %s %d\n", ptr, u32_size, pc_func_ptr, u32_line);
    ptr = pvPortMalloc(u32_size);

    if (ptr)
    {
        vTaskSuspendAll();
        malloc_info_insert(ptr, u32_size, (char *)pc_func_ptr, u32_line);
        xTaskResumeAll();
    }
    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return ptr;
}

/** Memory free
 * @param p_pointer
 */
void sys_free_fn(void *p_pointer, const char *pc_func_ptr, uint32_t u32_line)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/
    if (p_pointer == NULL)
    {
        err("free null pointer\n",
            pc_func_ptr, u32_line, sys_current_thread_name());
    }
    configASSERT(p_pointer != NULL);

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/
    msg(UTIL_LOG_DEBUG, "@@@@@ free %p %s %d \n", p_pointer, pc_func_ptr, u32_line);
    vPortFree(p_pointer);

    vTaskSuspendAll();
    malloc_info_delete(p_pointer);
    xTaskResumeAll();
    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
}

/** Get Memory reamin size
 */
uint32_t sys_get_free_size(void)
{
    /*-----------------------------------*/
    /* A.Input Parameter Range Check     */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* B. Main Functionality             */
    /*-----------------------------------*/

    /*-----------------------------------*/
    /* C. Result & Return                */
    /*-----------------------------------*/
    return (uint32_t)xPortGetFreeHeapSize();
}
