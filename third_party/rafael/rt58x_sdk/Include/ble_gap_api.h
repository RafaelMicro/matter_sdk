/** @file ble_gap_api.h
 *
 * @addtogroup BLE
 * @{
 * @defgroup ble_gap_api BLE GAP APIs
 * @brief Define BLE GAP related definitions, structures and functions.
 * @}
 */
#ifndef __BLE_GAP_API_H__
#define __BLE_GAP_API_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdbool.h>
#include "ble_api.h"
#include "ble_gap.h"
#include "ble_event_app.h"
#include "ble_connect_cte.h"

/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/
/** @brief Check the init state is in idle mode (create connection disabled).
 *
 * @ingroup ble_gap_api
 *
 * @return true : create connection disabled.
 * @return false :  create connection enabled.
 */
bool ble_init_idle_state_check(void);


/** @brief Get BLE device address and device address type.
 *
 *  This function is used to get BLE Local Device Address and Device Address Type.
 *
 * @ingroup ble_gap_api
 *
 * @note       BLE Address : Little Endian and the length is @ref BLE_ADDR_LEN \n
 *             If Device BLE Address is set to "01:02:03:04:05:06", addrParam->addr[0] = 0x06
 *
 * @param[out] p_addr : a pointer to the device BLE address and BLE address type.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gap_device_address_get(ble_gap_addr_t *p_addr);


/** @brief Get BLE identity device address and device address type.
 *
 *  This function is used to get BLE Identity Device Address and Device Address Type.
 *
 * @ingroup ble_gap_api
 *
 * @note       BLE Address : Little Endian and the length is @ref BLE_ADDR_LEN \n
 *             If Device BLE Address is set to "01:02:03:04:05:06", addrParam->addr[0] = 0x06
 *
 * @param[out] p_addr : a pointer to the device BLE address and BLE address type.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gap_device_identity_address_get(ble_gap_addr_t *p_addr);


/** @brief Compare BLE identity device address and global device address.
 *
 *  This function is used to compare BLE Identity Device Address and Global Device Address.
 *
 * @ingroup ble_gap_api
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gap_device_address_compare(void);


/** @brief Set BLE device address and device address type.
 *
 * This function is used to set BLE Local Device Address and Device Address Type.
 *
 * @ingroup ble_gap_api
 *
 *
 * @note      BLE Address : Little Endian and the length is @ref BLE_ADDR_LEN \n
 *            If Device BLE Address is set to "01:02:03:04:05:06", addrParam->addr[0] = 0x06
 *
 * @param[in] p_addr : a pointer to the device BLE address and BLE address type.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gap_device_address_set(ble_gap_addr_t *p_addr);



/** @brief BLE connection create command.
 *
 * @ingroup ble_gap_api
 *
 * @param[in] p_param : a pointer to the create connection parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gap_connection_create(ble_gap_create_conn_param_t *p_param);


/** @brief BLE cancel create connection process command.
 *
 * @ingroup ble_gap_api
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gap_connection_cancel(void);


/** @brief Terminate the BLE connection link.
 *
 * @ingroup ble_gap_func
 *
 * @param[in] p_param : a pointer to the connection terminate parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gap_conn_terminate(ble_gap_conn_terminate_param_t *p_param);



/** @brief BLE connection parameter update.
 *
 * @ingroup ble_gap_func
 *
 * @param[in] p_param : a pointer to the connection parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gap_connection_update(ble_gap_conn_param_update_param_t *p_param);


/** @brief BLE PHY update.
 *
 * @ingroup ble_gap_func
 *
 * @param[in] p_param : a pointer to the PHY parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gap_phy_update(ble_gap_phy_update_param_t *p_param);


/** @brief BLE read PHY.
 *
 * @ingroup ble_gap_func
 *
 * @param[in] p_param : a pinter to the phy read parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gap_phy_read(ble_gap_phy_read_param_t *p_param);


/** @brief BLE read RSSI value.
 *
 * @ingroup ble_gap_func
 *
 * @param[in] p_param : a pointer to rssi read parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gap_rssi_read(ble_gap_rssi_read_param_t *p_param);


/** @brief BLE set le host channel classification value.
 *
 * @ingroup ble_gap_func
 *
 * @param[in] p_param : a pointer to channel map parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gap_host_channel_classification_set(ble_gap_host_ch_classif_t *p_param);


/** @brief BLE read channel map value.
 *
 * @ingroup ble_gap_func
 *
 * @param[in] p_param : a pointer to read channel map parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gap_channel_map_read(ble_gap_channel_map_read_t *p_param);


/** @brief BLE Init resolvable address.
 *
 * @ingroup ble_gap_func
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_resolvable_address_init(void);


/** @brief BLE regenerate resolvable address.
 *
 * @ingroup ble_gap_func
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_regenerate_resolvable_address(ble_gap_regen_resol_addr_t *p_param);


/** @brief BLE set connection cte rx parameters.
 *
 * @ingroup ble_gap_func
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_connection_cte_rx_param_set(ble_connection_cte_rx_param_t *p_param);


/** @brief BLE set connection cte tx parameters.
 *
 * @ingroup ble_gap_func
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_connection_cte_tx_param_set(ble_connection_cte_tx_param_t *p_param);


/** @brief BLE set connection cte request enable.
 *
 * @ingroup ble_gap_func
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_connection_cte_req_set(ble_connection_cte_req_enable_t *p_param);


/** @brief BLE set connection cte response enable.
 *
 * @ingroup ble_gap_func
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_connection_cte_rsp_set(ble_connection_cte_rsp_enable_t *p_param);


/** @brief BLE GAP events handler.
 *
 * @ingroup ble_gap_func
 *
 * @param p_param :  a pointer to the event parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_evt_gap_handler(void *p_param);


#ifdef __cplusplus
};
#endif

#endif /* __BLE_GAP_API_H__*/

