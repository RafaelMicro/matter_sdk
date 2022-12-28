/** @file ble_host_cmd.h
 *
 * @addtogroup BLE
 * @{
 * @addtogroup ble_host BLE Host
 * @{
 * @defgroup ble_host_api BLE Host APIs
 * @}
 * @}
 */
#ifndef __BLE_BLE_HOST_CMD_H__
#define __BLE_BLE_HOST_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "ble_hci.h"
#include "task_host.h"
#include "ble_security_manager.h"
#include "smp.h"

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/** @defgroup ble_host_timer BLE Host Timer Type Definition
 * @{
 * @ingroup ble_host_api
 * @brief  Define BLE Host timer event type.
 */
typedef uint8_t host_timer_evt;
#define TIMER_EVENT_NULL                                 0xFF                           /**< Null event.*/
#define TIMER_EVENT_CONN_PARAMETER_UPDATE_RSP            0x00                           /**< Host sw timer event for connection update parameter update response.*/
#define TIMER_EVENT_CONN_UPDATE_COMPLETE                 0x01                           /**< Host sw timer event for connection update complete.*/
#define TIMER_EVENT_AUTH_STATUS                          0x02                           /**< Host sw timer event for authentication status.*/
#define TIMER_EVENT_CLIENT_DB_PARSING                    0x03                           /**< Host sw timer event for database parsing.*/
#define TIMER_EVENT_GATT_COMMAND                         0x04                           /**< Host sw timer event for gatt command response*/
#define TOTAL_TIMER_EVENT                                (TIMER_EVENT_GATT_COMMAND+1)   /**< Total sw timer event number.*/
/** @} */


/** @brief  Define BLE Host timer message parameter.
 * @ingroup ble_host_api
 */
typedef struct ble_host_timer_msg_s
{
    uint32_t  current_time;   /**< number of sw timer count.*/
    uint32_t  timeout_base;   /**< timeout*/
    uint8_t   event;          /**< @ref ble_host_timer "Event type". */
    uint8_t   rsv[3];         /**< reserved bytes*/
} ble_host_timer_msg_t;



/**************************************************************************
 * EXTERN DEFINITIONS
 **************************************************************************/

/** @brief  BLE host timer parameter.
 * @ingroup ble_host_api
 */
extern ble_host_timer_msg_t *host_timer;


/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** @brief Host parameter initialization.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : host id of the link.
 *
 * @return none.
 */
void bhc_host_param_init(uint8_t host_id);


/** @brief BLE host attribute related parameters intitialization.
 *
 * @ingroup ble_host_api
 *
 * @return none.
 */
void bhc_att_param_init(void);


/** @brief BLE bonding storage intitialization.
 *
 * @ingroup ble_host_api
 *
 * @return none.
 */
void bhc_bonding_storage_init(void);


/** @brief Query the host ID by connection handle value.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  conn_id : connection handle value.
 *
 * @return host id.
 */
uint8_t bhc_query_host_id_by_conn_id(uint16_t conn_id);


/** @brief Release the DB parsing resource.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : host id of the link.
 *
 * @return none.
 */
void bhc_host_release_db_parsing_resource(uint8_t host_id);


/** @brief Release the ATT queue write resource.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : host id of the link.
 *
 * @return none.
 */
void bhc_att_queue_write_resource(uint8_t host_id);


/** @brief Get the number of BLE connected links.
 *
 * @ingroup ble_host_api
 *
 * @return connected links.
 */
uint8_t bhc_host_connected_link_number_get(void);



/**@brief  Check host ID is valid or not.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : the link's host id.
 *
 * @retval  true : the host id is in the range.
 * @retval  false: the host id is invalid.
 */
bool bhc_host_id_is_valid_check(uint8_t host_id);


/** @brief Check host ID is connected to the device or not.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : the link's host id.
 * @param[out] conn_id : the link's connection id.
 *
 * @retval  true : the host id is connected.
 * @retval  false: the host id is free.
 */
bool bhc_host_id_is_connected_check(uint8_t host_id, uint16_t *conn_id);


/** @brief Get the active host id.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  role : GAP role, @ref ble_gap_role_t.
 *
 * @return host_id : the link's host id.
 */
uint8_t bhc_host_id_state_active_get(uint8_t role);


/** @brief The BLE host ID is set to active mode.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : the link's host id.
 * @param[in]  role : GAP role, @ref ble_gap_role_t.
 *
 * @return none.
 */
void bhc_host_id_state_active_set(uint8_t host_id, uint8_t role);


/** @brief The BLE host ID is set to idle mode.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  role : GAP role, @ref ble_gap_role_t.
 *
 * @return none.
 */
void bhc_host_id_state_active_release(uint8_t role);


/** @brief The BLE host processing connection completion event.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  p_data : a pointer to the connection complete event data.
 *
 * @return none.
 */
void bhc_connection_complete_handle(ble_hci_le_meta_evt_param_conn_complete_t *p_data);


/** @brief The BLE host processing connection update completion event.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  p_data : a pointer to the connection update complete event data.
 *
 * @return none.
 */
void bhc_connection_update_handle(ble_hci_le_meta_evt_param_conn_update_t *p_data);


/** @brief The BLE host prossing long term key request event.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  p_data : a pointer to the long term key request event data.
 *
 * @retval  0 : successful.
 * @retval  -1: the queue is full or memory not enough.
 */
int8_t bhc_smp_long_term_key_req(ble_hci_le_meta_evt_param_long_term_key_req_t *p_data);


/** @brief The BLE host processing encryption change event.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  p_data : a pointer to the encryption change event data.
 *
 * @return none.
 */
void bhc_prcss_encrpytion_change(ble_hci_evt_param_encrypt_change_t *p_data);


/** @brief The BLE host processing encrypt key refresh event.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  p_data : a pointer to the encrypt key refresh event data.
 *
 * @retval  true : successful.
 * @retval  false: the queue is full or memory not enough.
 */
int8_t bhc_prcss_encrpyt_key_refresh(ble_hci_evt_param_key_refresh_complete_t *p_data);


/** @brief The BLE host generate a random value for security.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  p_data : a pointer to the random event data.
 * @param[in]  msg : encrpyt queue message.
 *
 * @retval  true : successful.
 * @retval  false: the queue is full or memory not enough.
 */
int8_t bhc_gen_random_value(ble_hci_return_param_rand_t *p_data, encrypt_queue_t msg);

//privacy add
int8_t private_address_construct_by_id(uint8_t host_id, private_addr_gen_t *private_addr_param);
int8_t private_address_resolve_by_id(uint8_t host_id, uint8_t *private_addr24);


/** @brief The BLE host generate a random value for security.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  data : encrpyt queue message data.
 * @param[in]  encrypt_out : a pointer to the le encrypt event data.
 *
 * @retval  true : successful.
 * @retval  false: the queue is full or memory not enough.
 */
int8_t bhc_prcss_le_encrypt_event(encrypt_queue_t data, uint8_t *encrypt_out);


/** @brief Check host DB parsing or encryption process is finished or not.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : the link's host id.
 *
 * @retval  true : the host is processing DB parsing or encryption.
 * @retval  false: the host is ready.
 */
bool bhc_host_parsing_process_is_finished_check(uint8_t host_id);


/** @brief Check the host is in encryption process or not.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : the link's host id.
 *
 * @retval  true : the host is busy.
 * @retval  false: the host is ready.
 */
bool bhc_host_is_in_encryption_check(uint8_t host_id);


/** @brief Check the GATT response is received or not.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : the link's host id.
 *
 * @retval  true : the host is waiting for the GATT response.
 * @retval  false: the host is ready.
 */
bool bhc_host_is_wating_gatt_rsp_check(uint8_t host_id);


/** @brief Check the BLE characteristic properties definition is defined or not.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id    : the link's host id.
 * @param[in]  handle_num : attribute handle number.
 * @param[in]  property   : @ref bleCharPropertie.
 *
 * @retval  true : the definition is match.
 * @retval  false: the definition is not match.
 */
bool bhc_client_property_value_is_match_check(uint8_t host_id, uint16_t handle_num, uint8_t property);


/** @brief Check the BLE server characteristic properties definition is defined or not.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id    : the link's host id.
 * @param[in]  handle_num : attribute handle number.
 * @param[in]  property   : @ref bleCharPropertie, NULL means only check if it is server role.
 *
 * @retval  true : the definition is match.
 * @retval  false: the definition is not match.
 */
bool bhc_server_property_value_is_match_check(uint8_t host_id, uint16_t handle_num, uint8_t property);


/** @brief  Set GATT preferred ATT_MTU size.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id        : the link's host id.
 * @param[in]  preferred_mtu  : preferred ATT_MTU size.
 *
 * @retval  true : successful.
 * @retval  false: the host is in invalid state.
 */
bool bhc_gatt_preferred_mtu_set(uint8_t host_id, uint16_t preferred_mtu);


/** @brief Get ATT_MTU size.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : the link's host id.
 *
 * @return  ATT_MTU size.
 */
uint16_t bhc_gatt_att_mtu_get(uint8_t host_id);



/** @brief  Get BLE GATT attribute handles mapping table.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id            : the link's host id.
 * @param[in]  role               : GATT role.
 * @param[in]  p_att_element      : a pointer to service attribute element.
 * @param[in]  p_handle_num_addr  : a point to handle number table.
 *
 * @retval  true : successful.
 * @retval  false: invalid parameter.
 */
bool bhc_gatt_att_handle_mapping_get(uint8_t host_id, ble_gatt_role_t role, ble_att_param_t *p_att_element, void *p_handle_num_addr);



/** @brief  Send ATT request.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  conn_id    : the link's connection id.
 * @param[in]  att_opcode : @ref ble_att_opcode_t "attribute opcode" .
 * @param[in]  handle_num : attribute handle number, set to "NULL" if att_opecode is @ref OPCODE_ATT_EXCHANGE_MTU_REQUEST.
 * @param[in]  p_data     : a point to the data.
 * @param[in]  length     : data length.
 * @retval  true : successful.
 * @retval  false: the queue is full or error handle number.
 */
int8_t bhc_att_req(uint16_t conn_id, uint8_t att_opcode, uint16_t handle_num, uint8_t *p_data, uint16_t length);


/** @brief  Send ATT error response request.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  conn_id    : the link's connection id.
 * @param[in]  att_opcode : @ref ble_att_opcode_t "attribute opcode" .
 * @param[in]  handle_num : attribute handle number, set to "NULL" if att_opecode is @ref OPCODE_ATT_EXCHANGE_MTU_REQUEST.
 * @param[in]  error_code : error code.
 *
 * @retval  true : successful.
 * @retval  false: the queue is full.
 */
int8_t bhc_att_error_rsp_req(uint16_t conn_id, uint8_t att_opcode, uint16_t handle_num, ble_att_error_code_t error_code);


/** @brief Get BLE hOST timer event status.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : the link's host id.
 * @param[in]  event : select the host timer event, refer to @ref ble_host_timer.
 *
 * @return  host timer status.
 */
uint8_t bhc_timer_evt_get(uint8_t host_id, host_timer_evt event);


/** @brief BLE host timer clear.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : the link's host id.
 * @param[in]  event : select the host timer event to clear.
 *
 * @return  none.
 */
void bhc_timer_clear(uint8_t host_id, host_timer_evt event);


/** @brief SET BLE host timer.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : the link's host id.
 * @param[in]  event : select the host timer event to set.
 * @param[in]  timeout_base : timeout counter (10ms).
 *
 * @retval  true : setting success.
 * @retval  false: event timer busy.
 */
uint8_t bhc_timer_set(uint8_t host_id, host_timer_evt event, uint32_t timeout_base);


/** @brief BLE Host transmit the connection parameter update request.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  conn_id : the link's host id.
 * @param[in]  p_param : a pointer to connection parameter.
 *
 * @retval  true : setting success.
 * @retval  false: event timer busy.
 */
ble_err_t bhc_gap_connection_update(uint16_t conn_id, ble_gap_conn_param_t *p_param);


/** @brief BLE Host send security request / pairing request.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : the link's host id.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t bhc_sm_security_request(uint8_t host_id);



/** @brief Set security passkey.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  hex_passkey : the passkey in hex value.
 * @param[in]  conn_id : the link's connection id.
 *
 * @retval  true : always return true.
 */
int8_t bhc_sm_passkey_set(uint32_t hex_passkey, uint16_t conn_id);


/** @brief Set security parameters - IO capabilities.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  p_param : a pointer to the io capability parameter.
 *
 * @return none.
 */
void bhc_sm_io_caps_set(ble_evt_sm_io_cap_t *p_param);


/** @brief Set security parameters - bonding flag.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  p_param : a pointer to the bonding flag parameter.
 *
 * @return none.
 */
void bhc_sm_bonding_flag_set(ble_evt_sm_bonding_flag_t *p_param);


/** @brief BLE restore cccd value command.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : conn_id : the link's host id.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t bhc_sm_restore_cccd_from_bond(uint8_t host_id);


/** @brief BLE Send Bonding space space init command.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : conn_id : the link's host id.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t bhc_sm_bonding_space_init(void);


/** @brief BLE process bonding space init command.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  none
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
void bhc_sm_prcss_bonding_space_init(void);


/** @brief BLE host L2CAP events handler.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  conn_handle : connection handle.
 * @param[in]  p_data : a pointer to the data.
 * @param[in]  length : data length.
 *
 * @retval  0 : successful.
 * @retval  -1: the queue is full or memory not enough.
 */
int8_t bhc_host_prcss_l2cap_cid(uint16_t conn_handle, uint8_t *p_data, uint16_t length);


/** @brief BLE host database parsing events handler.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  p_data : a pointer to the data.
 *
 * @return none.
 */
void bhc_db_parsing_evt_handle(void *pdata);


/** @brief BLE host authentication events handler.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  p_data : a pointer to the data.
 *
 * @return none.
 */
void bhc_authentication_evt_handle(void *p_data);


/** @brief BLE host authorization events handler.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  p_data : a pointer to the data.
 *
 * @return none.
 */
void bhc_authorization_evt_handle(void *p_data);


/** @brief The BLE host ID is set to active mode.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : the link's host id.
 * @param[in]  role : GAP role, @ref ble_gap_role_t.
 *
 * @return none.
 */
int8_t bhc_le_remote_conn_parameter_req_handle(ble_hci_le_meta_evt_param_conn_para_req_t *p_data);


/** @brief The BLE host ID is set to active mode.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : the link's host id.
 * @param[in]  role : GAP role, @ref ble_gap_role_t.
 *
 * @return none.
 */
int8_t bhc_le_remote_conn_parameter_req_neg_handle(ble_hci_cmd_le_remote_conn_param_req_neg_reply_param_t *p_data);


/** @brief The BLE set identity resolving key.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  p_param : a pointer to the identity reolving key parameter.
 *
 * @return none.
 */
ble_err_t bhc_sm_identity_resolving_key_set(ble_sm_irk_param_t *p_param);


/** @brief The BLE set tx acl data clear.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  conn_handle : connection id.
 *
 * @retval  0 : successful.
 * @retval  -1: the queue is full or memory not enough.
 */
int8_t bhc_disconnect_handle(uint16_t conn_handle);


/** @brief The BLE gen resolvable address.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : host id.
 *
 * @retval  0 : successful.
 * @retval  -1: the queue is full or memory not enough.
 */
int8_t bhc_host_gen_resolvable_address(uint8_t host_id);


/** @brief The BLE gen new identity resolving key.
 *
 * @ingroup ble_host_api
 *
 * @param[in]  host_id : host id.
 *
 * @retval  0 : successful.
 * @retval  -1: the queue is full or memory not enough.
 */
int8_t bhc_host_gen_new_irk(uint8_t host_id);

#ifdef __cplusplus
};
#endif

#endif // __BLE_BLE_HOST_CMD_H__

