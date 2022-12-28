/**
 * @file sys_timer.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */


/**
 * @defgroup SW_TIMER_API Software timer API Definition
 * Define Software timer API definitions, structures, and functions.
 * @ingroup System
 * @{
 */
#ifndef __SYS_TIMER_H__
#define __SYS_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
//                Include (Better to prevent)
//=============================================================================
#include "sys_arch.h"

//=============================================================================
//                Public Definitions of const value
//=============================================================================
#define SYS_MAX_TIMER                0xFFFFFFFF
#define SYS_TIMER_MAX_PERIOD         0x0A4CB800  // 172800000 ms = 48 hour
#define SYS_TIMER_SHIFT_THRESHOLD    0x80000000  // About 24.8 day

//############################################################
// SYS Timer Tick = 0.1 ms
// MAX Time = 48 Hour = 2880 min = 172800 sec =  172800000 ms
//############################################################
#define SYS_TIMER_HOUR_TO_TICK(h)            ((h) * 3600000)
#define SYS_TIMER_MINUTE_TO_TICK(m)          ((m) * 60000)
#define SYS_TIMER_SECOND_TO_TICK(s)          ((s) * 1000)
#define SYS_TIMER_MILLISECOND_TO_TICK(ms)    (ms)

//=============================================================================
//                Public ENUM
//=============================================================================
typedef enum SYS_TIMER_PRIORITY
{
    SYS_TIMER_PRIORITY_0 = 0,    // Low
    SYS_TIMER_PRIORITY_MAX,
} sys_timer_pri_t;

typedef enum SYS_TIMER_EXECUTE_MODE
{
    SYS_TIMER_EXECUTE_ONCE_FOR_EACH_TIMEOUT      = 0,
    SYS_TIMER_EXECUTE_ONCE_FOR_DUPLICATE_TIMEOUT = 1,
} sys_timer_mode_t;

typedef enum SYS_TIMER_ERRNO
{
    SYS_TIMER_EXECUTE_MODE_FAIL  = -5,
    SYS_TIMER_PERIOD_FAIL        = -4,
    SYS_TIMER_PRIORITY_FAIL      = -3,
    SYS_TIMER_CMD_SEND_FAIL      = -2,
    SYS_TIMER_NULL               = -1,
    SYS_TIMER_PASS               = 0,
} sys_timer_err_t;

//=============================================================================
//                Public Struct
//=============================================================================
typedef void (*sys_timer_cb)(void *p_param);

typedef struct SYS_TIMER
{
    uint32_t            period;
    const char          *name;

    uint32_t            auto_reload             : 1;
    uint32_t            priority                : 2;
    uint32_t            execute_mode            : 1;    // follow enum sys_timer_execute_mode_e
    uint32_t            receive_delete_cmd      : 1;
    uint32_t            wating_for_execution    : 8;
    uint32_t            running                 : 1;
    uint32_t            reserved                : 18;

    void                *cb_param;
    sys_timer_cb         cb_function;

    uint32_t            timeout;
    link_list_t         list;
} sys_timer_t;

//=============================================================================
//                Public Function Declaration
//=============================================================================
/**
 * @brief Create a software timer
 *
 * @param name A human readable text name that is assigned to the timer.
 * @param period The period of the timer.
 * @param auto_reload If auto_reload is set to TRUE, then the timer will
 * expire repeatedly with a frequency set by the period parameter.
 * If auto_reload is set to FALSE, then the timer will
 * be a one-shot and enter the dormant state after it expires.
 * @param priority The priority of the timer.
 * @param execute_mode The execute mode of the timer.
 * @param cb_param The input parameter of the call back function.
 * @param cb_function The function to call when the timer expires.
 * @return sys_timer_t* If the timer is created successfully then a handle to the newly created timer is returned.
 * If the timer cannot be created the NULL is returned.
 */
sys_timer_t *sys_timer_create(const char *name, uint32_t period, uint32_t auto_reload,
                              sys_timer_pri_t priority, uint32_t execute_mode,
                              void *cb_param, sys_timer_cb cb_function);
/**
 * @brief Start a software timer that was previously created using the sys_timer_create()API function.
 *
 * @param timer The handle of the timer being started/restarted.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the start command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_start(sys_timer_t *timer);

/**
 * @brief Stop a software timer that was previously created using the sys_timer_create()API function.
 * Stopping a timer ensures the timer is not in the active state.
 *
 * @param timer The handle of the timer being stopped.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the stop command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the stop command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_stop(sys_timer_t *timer);

/**
 * @brief Re-start a software timer that was previously created using the sys_timer_create()API function.
 * Restting a timer ensures the timer is in the active state.
 *
 * @param timer The handle of the timer being reset/started/restarted.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the reset command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the reset command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_reset(sys_timer_t *timer);

/**
 * @brief Changes the period of a software timer that was previously created using the sys_timer_create()API function.
 *
 * @param timer The handle of the timer that is having its period chanded.
 * @param period The new period for timer.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the change period command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the change period command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_change_period(sys_timer_t *timer, uint32_t period);

/**
 * @brief Changes the priority of a software timer that was previously created using the sys_timer_create()API function.
 *
 * @param timer The handle of the timer that is having its period priority.
 * @param priority The new priority for timer.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the change priority command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the change priority command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_change_priority(sys_timer_t *timer, uint32_t priority);

/**
 * @brief Changes the execute mode of a software timer that was previously created using the sys_timer_create()API function.
 *
 * @param timer The handle of the timer that is having its execute mode.
 * @param execute_mode The new execute mode for timer.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the change execute mode command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the change execute mode command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_change_execute_mode(sys_timer_t *timer, uint32_t execute_mode);

/**
 * @brief Delete a software timer that was previously created using the sys_timer_create()API function.
 *
 * @param timer The handle of the timer being deleted.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the delete command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the delete command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_delete(sys_timer_t *timer);

/**
 * @brief Get the human readable text name of a software timer.
 *
 * @param timer The timer being queried.
 * @return const char* A pointer to the text name of the timer as a standard NULL terminated C string.
 */
const char *sys_timer_get_timer_name(sys_timer_t *timer);

/**
 * @brief Start a software timer that was previously created using the sys_timer_create()API function from an interrupt service routine.
 *
 * @param timer The handle of the timer being started/restarted.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the start command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_start_from_isr(sys_timer_t *timer);

/**
 * @brief Stop a software timer that was previously created using the sys_timer_create()API function from an interrupt service routine.
 * Stopping a timer ensures the timer is not in the active state.
 *
 * @param timer The handle of the timer being stopped.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the stop command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the stop command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_stop_from_isr(sys_timer_t *timer);

/**
 * @brief Re-start a software timer that was previously created using the sys_timer_create()API function from an interrupt service routine.
 * Restting a timer ensures the timer is in the active state.
 *
 * @param timer The handle of the timer being reset/started/restarted.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the reset command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the reset command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_reset_from_isr(sys_timer_t *timer);

/**
 * @brief Changes the priority of a software timer that was previously created using the sys_timer_create()API function from an interrupt service routine.
 *
 * @param timer The handle of the timer that is having its period priority.
 * @param priority The new priority for timer.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the change priority command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the change priority command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_change_period_from_isr(sys_timer_t *timer, uint32_t period);

/**
 * @brief Changes the priority of a software timer that was previously created using the sys_timer_create()API function from an interrupt service routine.
 *
 * @param timer The handle of the timer that is having its period priority.
 * @param priority The new priority for timer.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the change priority command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the change priority command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_change_priority_from_isr(sys_timer_t *timer, uint32_t priority);

/**
 * @brief Changes the execute mode of a software timer that was previously created using the sys_timer_create()API function from an interrupt service routine.
 *
 * @param timer The handle of the timer that is having its execute mode.
 * @param execute_mode The new execute mode for timer.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the change execute mode command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the change execute mode command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_change_execute_mode_from_isr(sys_timer_t *timer, uint32_t execute_mode);

/**
 * @brief Delete a software timer that was previously created using the sys_timer_create()API function from an interrupt service routine.
 *
 * @param timer The handle of the timer being deleted.
 * @return sys_timer_err_t SYS_TIMER_PASS will be returned if the delete command was successfully sent to the command queue.
 * SYS_TIMER_CMD_SEND_FAIL will be returned if the delete command could not be sent to the timer command queue.
 */
sys_timer_err_t sys_timer_delete_from_isr(sys_timer_t *timer);

/**
 * @brief Initial the software tasks.
 *
 * @param u32_hw_timer_id Hardware timer id for the software timer task.
 * @return int SYS_ERR_OK will be returned if the initial was successfully.
 */
int sys_timer_init(uint32_t u32_hw_timer_id);

/**
 * @brief Show all of the created software timer state.
 *
 */
void sys_timer_state_print(void);

/**
 * @brief Queries a software timer to see if it is active or dormant.
 *
 * @param timer The timer being queried.
 * @return true The timer is active.
 * @return false The timer is dormant.
 */
bool sys_timer_get_running(sys_timer_t *timer);

/**
 * @brief Get the tick of the software timer.
 *
 * @return uint32_t The tick ot the time.
 */
uint32_t sys_timer_get_tick(void);

/** @} */

#ifdef __cplusplus
};
#endif
#endif /* __SYS_TIMER_H__ */
