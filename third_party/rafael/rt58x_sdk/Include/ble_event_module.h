/** @file ble_event_module.h
 *
 * @addtogroup ble_common
 * @{
 * @addtogroup ble_common_evt BLE Events
 * @{
 * @defgroup ble_common_evt_module BLE Module Event APIs
 * @brief Handle BLE events from BLE stack.
 * @}
 * @}
 */
#ifndef __BLE_EVENT_MODULE_H__
#define __BLE_EVENT_MODULE_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include "ble_event.h"


/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** @brief Post @ref ble_cmd_evt_t with parameters to the related BLE module handle.
 *
 * @ingroup ble_common_evt_module
 *
 * @param[in] p_param : a pointer to the parameters.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_event_post_to_module(ble_evt_param_t *p_param);

#ifdef __cplusplus
};
#endif

#endif /* __BLE_EVENT_MODULE_H__*/
