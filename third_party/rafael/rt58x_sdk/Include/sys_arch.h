/**
 * @file sys_arch.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
 * @defgroup SYS_API System API Definition
 * Define System API definitions, structures, and functions.
 * @ingroup System
 *
 * @{
 */

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
#ifndef __SYS_ARCH_H__
#define __SYS_ARCH_H__

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
//                Include (Better to prevent)
//=============================================================================
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "err.h"

#include "util_log.h"
#include "util_printf.h"
#include "util_list.h"

#include "sys_memory.h"
#include "sys_timer.h"
#include "sys_printf.h"
//=============================================================================
//                Public Definitions of const value
//=============================================================================
#define SYS_ARCH_TIMEOUT                0xffffffffUL
#define sys_task_delay(n)               vTaskDelay(n)
#define sys_task_yield()                taskYIELD()
#define sys_task_suspend(n)             vTaskSuspend(n)
#define sys_task_resume(n)              vTaskResume(n)
#define sys_task_resume_from_isr(n)     xTaskResumeFromISR(n)
#define sys_sem_get_cnt(sem)            uxSemaphoreGetCount(sem)
#define SYS_TLV_HEADER_SIZE             (sizeof(sys_tlv_t))

//=============================================================================
//                Public ENUM
//=============================================================================
typedef enum TASK_HANDLE_RESULT
{
    LEAVE_HANDLE_DO_NOTHING = 0,
    LEAVE_HANDLE_MUST_FREE_TLV,
} task_result_t;


typedef enum SYSTEM_ERR
{
    SYS_ERR_TIMER_OP                = -17,
    SYS_ERR_ALLOC_MEMORY_FAIL       = -16,
    SYS_ERR_RECVFROM_LEN_NOT_ENOUGH = -15,
    SYS_ERR_RECVFROM_FAIL           = -14,
    SYS_ERR_RECVFROM_NO_DATA        = -13,
    SYS_ERR_RECVFROM_POINTER_NULL   = -12,
    SYS_ERR_SENDTO_FAIL             = -11,
    SYS_ERR_SENDTO_POINTER_NULL     = -10,
    SYS_BUSY                        = -9,
    SYS_ERR_WRONG_CONFIG            = -8,
    SYS_ERR_SEMAPHORE_MALLOC_FAIL   = -7,
    SYS_ERR_THREAD_MALLOC_FAIL      = -6,
    SYS_ERR_QUEUE_MALLOC_FAIL       = -5,
    SYS_ERR_DATA_MALLOC_FAIL        = -4,
    SYS_ERR_DUPLICATE_INIT          = -3,
    SYS_ERR_NOT_INIT                = -2,
    SYS_ERR_UNKNOW_TYPE             = -1,
    SYS_ERR_OK                      = 0,
} sys_err_t;


//=============================================================================
//                Public Struct
//=============================================================================
typedef SemaphoreHandle_t   sys_sem_t;
typedef SemaphoreHandle_t   sys_mutex_t;
typedef TaskHandle_t        sys_task_t;
typedef QueueHandle_t       sys_mbox_t;
typedef QueueHandle_t       sys_queue_t;
typedef TickType_t          sys_tick_t;
/*
 * Defines the prototype to which task functions must conform.
 */
typedef void (*sys_thread_fn)( void *arg);


typedef struct SYSTEM_TLV_FORMAT
{
    uint16_t    type;       // Payload Identifier : defined by each module
    uint16_t    length;     // Payload Length : the length of the payload data
    uint8_t     value[];    // Payload Data
} sys_tlv_t;

typedef struct TASK_QUEUE
{
    uint32_t u32_send_systick;
    sys_tlv_t *pt_tlv;
} task_queue_t;
//=============================================================================
//                Public Function Declaration
//=============================================================================

//==================================
//     Mutex functions
//==================================
/** Create a new mutex
 * @param mutex Pointer to the mutex to create
 * @return a new mutex
 */
err_t sys_mutex_new(sys_mutex_t *p_mutex);

/** Delete a semaphore
 * @param p_mutex the mutex to delete
 */
void sys_mutex_free(sys_mutex_t *p_mutex);

/** Lock a mutex
 * @param p_mutex The mutex to lock
 */
void sys_mutex_lock(sys_mutex_t *p_mutex);

/** Try lock a mutex
 * @param p_mutex The mutex to lock
 * @return ERR_OK is success, and ERR_MEM is fail
 */
err_t sys_mutex_trylock(sys_mutex_t *p_mutex);

/** Unlock a mutex
 * @param p_mutex the mutex to unlock
 */
void sys_mutex_unlock(sys_mutex_t *p_mutex);


//==================================
//     Semaphore functions
//==================================
/** Create a new semaphore
 * @param sem pointer to the semaphore to create
 * @param count initial count of the semaphore
 * @return ERR_OK if successful, another err_t otherwise
 */
err_t sys_sem_new(sys_sem_t *sem, uint8_t u8_count_max, uint8_t u8_count_init);

/** Create a new semaphore
 * @param sem pointer to the semaphore to create
 * @param count initial count of the semaphore
 * @return ERR_OK if successful, another err_t otherwise
 */
err_t sys_binary_sem_new(sys_sem_t *sem);

/** Delete a semaphore
 * @param sem semaphore to delete
 */
void sys_sem_free(sys_sem_t *sem);

/** Signals a semaphore
 * @param sem the semaphore to signal
 */
void sys_sem_signal(sys_sem_t *sem);

/** Signals a semaphore (from ISR)
 * @param sem the semaphore to signal
 */
void sys_sem_signal_isr(sys_sem_t *sem);

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
uint32_t sys_sem_wait(sys_sem_t *sem, uint32_t u32_timeout);


//==================================
//     Mailbox functions
//==================================
/** Creates an empty mailbox.
 * @param mbox The mbox handle
 * @param size the length of mbox
 */
err_t sys_mbox_new(sys_mbox_t *mbox, uint32_t size);

/** Deallocates a mailbox. If there are messages still present in the
 *  mailbox when the mailbox is deallocated, it is an indication of a
 *  programming error and the developer should be notified.
 * @param mbox The mbox handle
 */
void sys_mbox_free(sys_mbox_t *mbox);

/** Posts the "msg" to the mailbox.
 * @param mbox The mbox handle
 * @param msg The pointer of the msg
 */
void sys_mbox_post(sys_mbox_t *mbox, void *msg);

/** Posts the "msg" to the mailbox.
 * @param mbox The mbox handle
 * @param msg The pointer of the msg
 * @param u32_timeout millisecond timer
 */
err_t sys_mbox_post_with_timeout(sys_mbox_t *mbox, void *msg, uint32_t u32_timeout);

/** Try posts the "msg" to the mailbox.
 * @param mbox The mbox handle
 * @param msg The pointer of the msg
 */
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg);

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
uint32_t sys_mbox_fetch(sys_mbox_t *mbox, void **msg, uint32_t u32_timeout);

uint32_t sys_mbox_tryfetch(sys_mbox_t *mbox, void **msg);
uint32_t sys_mbox_remaining_size(sys_mbox_t *mbox);


//==================================
//     Queue functions
//==================================
/** Creates an empty queue.
 * @param queue The queue handle
 * @param queue_length the length of queue
 * @param item_size the size of item of queue
 */
err_t sys_queue_new(sys_queue_t *queue, uint32_t queue_length, uint32_t item_size);

sys_queue_t sys_queue_static_new(uint32_t queue_length,
                                 uint32_t item_size,
                                 uint8_t *pucQueueStorageBuffer,
                                 StaticQueue_t *pxQueueBuffer
                                );

/** Deallocates a queue. If there are messages still present in the
 *  queue when the queue is deallocated, it is an indication of a
 *  programming error and the developer should be notified.
 * @param queue The queue handle
 */
void sys_queue_free(sys_queue_t *queue);

/** Sends the "msg" to the queue.
 * @param queue The queue handle
 * @param msg The pointer of the msg
 */
void sys_queue_send(sys_queue_t *queue, void *msg);
void sys_queue_send_from_isr(sys_queue_t *queue, void *msg);

void sys_queue_sendtofront(sys_queue_t *queue, void *msg);
void sys_queue_sendtofront_from_isr(sys_queue_t *queue, void *msg);

/** Sends the "msg" to the queue.
 * @param queue The queue handle
 * @param msg The pointer of the msg
 * @param u32_timeout millisecond timer
 */
err_t sys_queue_send_with_timeout(sys_queue_t *queue, void *msg, uint32_t u32_timeout);

err_t sys_queue_sendtofront_with_timeout(sys_queue_t *queue, void *msg, uint32_t u32_timeout);

/** Try sends the "msg" to the queue.
 * @param queue The queue handle
 * @param msg The pointer of the msg
 */
err_t sys_queue_trysend(sys_queue_t *queue, void *msg);

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
uint32_t sys_queue_recv(sys_queue_t *queue, void *msg, uint32_t u32_timeout);

/** Get the number of free spaces in a queue.
 * @param queue The queue handle
 */
uint32_t sys_queue_remaining_size(sys_queue_t *queue);

uint32_t sys_arch_queue_tryrecv(sys_queue_t *queue, void *msg);

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
                        uint32_t u32_priority);

sys_task_t sys_task_static_new(sys_thread_fn            thread,
                               const char               *name,
                               void                     *arg,
                               uint32_t                 u32_stacksize,
                               uint32_t                 u32_priority,
                               StackType_t *const      puxStackBuffer,
                               StaticTask_t *const     pxTaskBuffer
                              ) ;

/** get a descriptive name for the thread
 * @return thread name
 */
char *sys_current_thread_name(void);

/** get the thread priority
 * @param thread_handle
 * @return thread priority
 */
uint32_t sys_thread_priority_get(sys_task_t thread_handle);

/** set the thread priority
 * @param thread_handle
 * @param u32_priority The priority at which the thread should run.
 *     It must less than CONFIG_FREERTOS_MAX_PRIORITIES
 */
void sys_thread_priority_set(sys_task_t thread_handle, uint32_t u32_priority);


//==================================
//     Time functions
//==================================
/**
 * @brief Get current system time
 *
 * @return Current system time
 */
uint32_t sys_now(void);

/**
 * @brief Get current system time in ISR
 *
 * @return Current system time
 */
uint32_t sys_now_from_isr(void);

/**
 * @brief Task sleep
 *
 * @param u32_ms sleep time in milliseconds
 */
void sys_msleep(uint32_t u32_ms);


//==================================
//     Memory functions
//==================================
/**
 * @brief Show the memory allocation information
 *
 */
void sys_malloc_info_printf(void);

/** Memory allocation
 * @param u32_size memory size in bytes
 */

void *sys_malloc_fn(uint32_t u32_size, const char *pc_func_ptr, uint32_t u32_line);

/**
 * @brief Memory allocate
 * @param len Size of allocated
 * @return The Pointer of the allocated buffer.
 *
 */
#define sys_malloc(len) sys_malloc_fn(len, __FUNCTION__, __LINE__)

/** Memory free
 * @param p_pointer
 */
void sys_free_fn(void *p_pointer, const char *pc_func_ptr, uint32_t u32_line);

/**
 * @brief Memory free
 * @param ptr The pointer to free
 */
#define sys_free(ptr) sys_free_fn(ptr, __FUNCTION__, __LINE__)

/** Get Memory reamin size
 */

/**
 * @brief Get the system memory free size
 *
 * @return The free size of memory
 */
uint32_t sys_get_free_size(void);

/**
 * @brief Set the random seed.
 *
 * @param random_seed Random seed.
 */
void sys_set_random_seed(uint32_t random_seed);

/**
 * @brief Generate random number
 *
 * @return Random number
 */
uint32_t sys_random(void);

/** @} */

#ifdef __cplusplus
};
#endif
#endif /* __SYS_ARCH_H__ */
