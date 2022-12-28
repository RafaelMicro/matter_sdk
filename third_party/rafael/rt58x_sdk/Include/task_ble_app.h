/** @file task_ble_app.h
 *
 * @brief Handle BLE application request, BLE event and BLE service data.
 *
 * @addtogroup BLE
 * @{
 * @addtogroup Task
 * @{
 * @defgroup task_ble_app BLE Application Task
 * @{ @}
 * @}
 * @}
 */

#ifndef __TASK_BLE_APP_H__
#define __TASK_BLE_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include "sys_arch.h"
#include "ble_api.h"
#include "ble_event_module.h"


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/


/**@brief The definition of BLE application event parameter.
 * @ingroup task_ble_app
 */
typedef struct
{
    uint32_t         type;  /**< Queue type. */
    union
    {
        ble_evt_param_t      *p_ble_evt_param;      /**< BLE event parameter. */
        ble_evt_att_param_t  *p_ble_service_param;  /**< BLE service data parameter. */
    } event_param;
} ble_app_evt_param_t;


/**************************************************************************
 * EXTERN DEFINITIONS
 **************************************************************************/
extern sys_queue_t    g_ble_cfm_queue;

/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/**@brief BLE application task initialization.
 *
 * @ingroup task_ble_app
 *
 * @return none
 */
int task_ble_app_init(void);


/**@brief BLE application task delete.
 *
 * @ingroup task_ble_app
 *
 * @return none
 */
int8_t task_delete_ble_app(void);


/** Return BLE application task (task_ble_app) queue remaining size.
 *
 * @ingroup task_ble_app
 *
 * @return The number of free spaces available in the queue.
 */
uint8_t get_app_queue_remaining_size(void);


/**@brief Post the application event parameters on the BLE application queue.
 *
 * @ingroup task_ble_app
 *
 * @param param :  @ref ble_app_evt_param_t "BLE application event parameter".
 *
 * @return BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t task_ble_app_queue_send(ble_app_evt_param_t p_param);


uint32_t ble_wait_cfm(sys_tlv_t **pt_cfm_tlv, uint32_t u32_timeout);


ble_err_t ble_queue_sendto(sys_tlv_t *pt_tlv);


ble_err_t notify_evt_queue_send(uint8_t type, uint16_t param_len, void *param);


#ifdef __cplusplus
};
#endif

#endif /* __TASK_BLE_APP_H__*/


