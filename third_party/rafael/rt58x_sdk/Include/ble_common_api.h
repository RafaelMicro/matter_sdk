/** @file ble_common_api.h
 *
 * @addtogroup BLE
 * @{
 * @defgroup ble_common_api BLE Common APIs
 * @{
 * @brief Define BLE common definitions, structures and functions.
 * @}
 * @defgroup ble_common BLE Common Others
 * @{
 * @brief Define BLE modulfe and application events, BLE status and BLE UUIDs.
 * @}
 * @}
 */

#ifndef __BLE_COMMON_API_H__
#define __BLE_COMMON_API_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "ble_common.h"


/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** @brief BLE common controller setting initial function (BLE stack initialization included).
 *
 * @ingroup ble_common_api
 *
 * @param p_param :  a pointer to controller information data @ref ble_common_controller_info_t.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_common_controller_init(void *p_param);


/** @brief BLE set controller information function.
 *
 * @ingroup ble_common_api
 *
 * @param version :  BLE version.
 * @param company_id : company id.
 * @param p_addr :  a pointer to the BLE device address.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_common_controller_info_set(uint8_t version, uint16_t company_id, uint8_t *p_addr);


/** @brief BLE read filter accept list size function.
 *
 * @ingroup ble_common_api
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_common_read_filter_accept_list_size(void);


/** @brief BLE clear filter accept list function.
 *
 * @ingroup ble_common_api
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_common_clear_filter_accept_list(void);


/** @brief BLE add device to filter accept list function.
 *
 * @ingroup ble_common_api
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_common_add_device_to_filter_accept_list(ble_filter_accept_list_t *p_param);


/** @brief BLE remove device from filter accept list function.
 *
 * @ingroup ble_common_api
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_common_remove_device_from_filter_accept_list(ble_filter_accept_list_t *p_param);


/** @brief BLE read antenna information function.
 *
 * @ingroup ble_common_api
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_common_read_antenna_info(void);


/** @brief BLE Vendor command : scan request report.
 *
 * @ingroup ble_common_api
 *
 * @param p_param :  a pointer to the scan request report parameter @ref ble_hci_vcmd_scan_req_rpt_param_t.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_vendor_scan_req_report_set(ble_vendor_scan_req_rpt_t *p_param);


/** @brief BLE common events handler
 *
 * @ingroup ble_common_api
 *
 * @param p_param :  a pointer to the event parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_evt_common_handler(void *p_param);


/** @brief BLE common events handler
 *
 * @ingroup ble_common_api
 *
 * @param p_param :  a pointer to the event parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_evt_vendor_handler(void *p_param);


// TODO
ble_err_t prcss_api_cmd_transport(uint16_t type, void *p_param);

#ifdef __cplusplus
};
#endif

#endif /* __BLE_COMMON_API_H__*/

