/** @file ble_event_app.h
 *
 * @addtogroup ble_common
 * @{
 * @addtogroup ble_common_evt BLE Events
 * @{
 * @defgroup ble_common_evt_app BLE Application Event APIs
 * @brief Handle BLE events from BLE host.
 * @}
 * @}
 */

#ifndef __BLE_EVENT_APP_H__
#define __BLE_EVENT_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include "ble_api.h"

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**
 * @defgroup ble_common_evt_err BLE Error Event Parameters
 * @{
 * @ingroup ble_common_evt_app
*/
typedef struct ble_evt_error_s
{
    uint8_t             moudle_evt;     /**< @ref ble_common_evt_module_cmd "ble_module_evt_t". */
    uint8_t             status;         /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_evt_error_t;

/** @} */

/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** @brief  Post general BLE event with parameters to application task.
 *
 * @ingroup ble_common_evt_app
 *
 * @param[in] type  : event type @ref BLE_APP_GENERAL_EVENT or @ref BLE_APP_SERVICE_EVENT.
 * @param[in] p_param : a pointer to the event parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_event_post_to_notify(uint8_t type, void *p_param);



#ifdef __cplusplus
};
#endif

#endif /* __BLE_EVENT_APP_H__*/
