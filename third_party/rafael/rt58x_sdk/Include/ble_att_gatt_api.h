/** @file       ble_att_gatt_api.h
 *
*/

#ifndef _BLE_ATT_GATT_API_H_
#define _BLE_ATT_GATT_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "ble_api.h"
#include "ble_att_gatt.h"
#include "ble_event_app.h"

/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/
/** @brief Set suggested data length.
 *
 * @ingroup ble_att_api
 *
 * @note The range of tx_octets is @ref BLE_GATT_DATA_LENGTH_MIN to @ref BLE_GATT_DATA_LENGTH_MAX bytes.
 *
 * @param[in] p_param : a pointer to the preferred maximum number of payload octets.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_suggested_data_length_set(ble_gatt_suggested_data_len_param_t *p_param);


/** @brief Set preferred MTU size.
 *
 * @ingroup ble_att_api
 *
 * @note The range of mtu is @ref BLE_GATT_ATT_MTU_MIN to @ref BLE_GATT_ATT_MTU_MAX bytes.
 *
 * @param[in] p_param : a pointer to the preferred MTU size.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_preferred_mtu_set(ble_gatt_mtu_param_t *p_param);



/** @brief ATT_MTU exchange request.
 *
 * @ingroup ble_att_api
 *
 * @par BLE Event
 * Wait for @ref BLECMD_EVENT_EXCHANGE_MTU_SIZE event to get exchanged MTU size. \n
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
 *
 * @note The range of mtu is @ref BLE_GATT_ATT_MTU_MIN to @ref BLE_GATT_ATT_MTU_MAX bytes.
 *
 * @param[in] p_param : a pointer to the preferred MTU parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_exchange_mtu_req(ble_gatt_mtu_param_t *p_param);


/** @brief Set data length update.
 *
 * @ingroup ble_att_api
 *
 * @par BLE Event
 * Wait for @ref BLECMD_EVENT_DATA_LENGTH_UPDATE event to get updated BLE data length.
 *
 * @note The range of tx_octets is @ref BLE_GATT_DATA_LENGTH_MIN to @ref BLE_GATT_DATA_LENGTH_MAX bytes.
 *
 * @param[in] p_param : a pointer to the preferred data length parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_data_length_update(ble_gatt_data_len_param_t *p_param);



/** @brief Get BLE GATT MTU Size.
 *
 * @ingroup ble_att_api
 *
 * @param[in] p_param : the pointer to p_param.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_mtu_get(ble_gatt_get_mtu_param_t *p_param);



/** @brief Get BLE GATT Attribute Handles Mapping Table.
 *
 * @ingroup ble_att_api
 *
 * @param[out] p_param : a point to handle mapping parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_att_handle_mapping_get(ble_gatt_handle_table_param_t *p_param);



/** @brief BLE GATT Read Response.
 *
 * @ingroup ble_att_api
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
 *
 * @note The read response is sent in reply to a received Read Request and contains the value of the attribute that has been read.
 *
 * @param[in] p_param : a pointer to the read response parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_read_rsp(ble_gatt_data_param_t *p_param);



/** @brief BLE GATT Read By Type Response.
 *
 * @ingroup ble_att_api
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
 *
 * @note The read by type response is sent in reply to a received Read By Type Request and contains the handle number and value of the attribute that has been read.
 *
 * @param[in] p_param : a pointer to the read by type parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_read_by_type_rsp(ble_gatt_data_param_t *p_param);



/** @brief BLE GATT Read Blob Response.
 *
 * @ingroup ble_att_api
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
 *
 * @note The read blob response is sent in reply to a received Read Blob Request and contains the value of the attribute that has been read.
 *
 * @param[in] p_param : a pointer to the read blob response parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_read_blob_rsp(ble_gatt_data_param_t *p_param);


/** @brief BLE GATT Error Response.
 *
 * @ingroup ble_att_api
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
 *
 * @note The Error Response is used to state that a given request cannot be performed, and to provide the reason.
 *
 * @param[in] p_param : a pointer to the error response parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_error_rsp(ble_gatt_err_rsp_param_t *p_param);


/** @brief BLE GATT Notification.
 *
 * @ingroup ble_att_api
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
 *
 * @note When a server is configured to notify a Characteristic Value to a client without the acknowledgment that the notification was successfully received.
 *
 * @param[in] p_param : a pointer to the notification parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_notification(ble_gatt_data_param_t *p_param);



/** @brief BLE GATT Indication.
 *
 * @ingroup ble_att_api
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
 *
 * @note When a server is configured to indicate a Characteristic Value to a client and expects the indication was successfully received.
 *
 * @param[in] p_param : a pointer to the indication parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_indication(ble_gatt_data_param_t *p_param);



/** @brief BLE GATT Write Request.
 *
 * @ingroup ble_att_api
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
 *
 * @note The Write Request is used to request the server to write the value of an attribute and acknowledge that this has been achieved in a Write Response. \n
 * A Write Response shall be sent by the server if the write of the Characteristic Value succeeded.
 *
 * @param[in] p_param : a pointer to the the write request parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_write_req(ble_gatt_data_param_t *p_param);


/** @brief BLE GATT Write Command.
 *
 * @ingroup ble_att_api
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
 *
 * @note Write a Characteristic Value to a server when the client knows the Characteristic Value Handle \n
 *       and the client does not need an acknowledgment that the write was successfully performed.
 *
 * @param[in] p_param : a pointer to the the write command parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_write_cmd(ble_gatt_data_param_t *p_param);


/** @brief BLE GATT Read Request.
 *
 * @ingroup ble_att_api
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
 *
 * @note Read a Characteristic Value from a server when the client knows the Characteristic Value Handle.
 *
 * @param[in] p_param : a pointer to the the read request parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_read_req(ble_gatt_read_req_param_t *p_param);


/** @brief BLE GATT Read Blob Request.
 *
 * @ingroup ble_att_api
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
 *
 * @note Read part of the value of an attribute at a given offset from a server when the client knows the Characteristic Value Handle.
 *
 * @param[in] p_param : a pointer to the the read blob request parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_gatt_read_blob_req(ble_gatt_read_blob_req_param_t *p_param);


/** @brief BLE ATT/GATT events handler.
 *
 * @ingroup ble_att_api
 *
 * @param p_param :  a pointer to the event parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_evt_att_gatt_handler(void *p_param);


#ifdef __cplusplus
};
#endif

#endif // _BLE_ATT_GATT_API_H_




