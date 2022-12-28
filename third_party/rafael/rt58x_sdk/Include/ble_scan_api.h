/** @file ble_scan_api.h
 *
 */
#ifndef __BLE_SCAN_API_H__
#define __BLE_SCAN_API_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "ble_api.h"
#include "ble_scan.h"

/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** @brief Set BLE scan parameter.
 *
 * @ingroup ble_scan_api
 *
 * @note Scan window can only be less than or equal to the scan interval.
 *
 * @param[in] scanParam : a pointer to scanning parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_scan_param_set(ble_scan_param_t *p_param);


/** @brief Set BLE start Scanning.
 *
 * @ingroup ble_scan_api
 *
 * @par BLE Event
 * Wait for @ref BLECMD_EVENT_SCAN_COMPLETE event which indicates the scan has been started. \n
 * Wait for @ref BLECMD_EVENT_SCAN_REPORT event to receive scanned devices information.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_scan_enable(void);


/** @brief Set BLE stop scanning.
 *
 * @ingroup ble_scan_api
 *
 * @par BLE Event
 * Wait for @ref BLECMD_EVENT_SCAN_COMPLETE event which indicates the scan has been stopped.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_scan_disable(void);


/** @brief Check the scan state is in idle mode (scan disabled).
 *
 * @ingroup ble_scan_api
 *
 * @return true : scan disabled.
 * @return false : scan enabled.
 */
bool ble_scan_idle_state_check(void);


/** BLE scan events handler.
 *
 * @ingroup ble_scan_api
 *
 * @param p_param :  a pointer to the event parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_evt_scan_handler(void *p_param);


#ifdef __cplusplus
};
#endif

#endif /* __BLE_SCAN_API_H__*/
