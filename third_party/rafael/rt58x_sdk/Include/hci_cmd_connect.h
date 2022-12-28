/** @file hci_cmd_connect.h
 *
 * @defgroup hci_cmd_conn HCI Connection Related Commands APIs
 * @{
 * @ingroup hci_cmd
 *
 * @brief Includes Link Control commands (OGF = 0x01):
 *        - OCF = 0x0001 : Disconnect command
 *
 *        Includes Status Parameter commands (OGF = 0x05):
 *        - OCF = 0x0005 : Read RSSI command
 *
 *        Includes LE Controller commands (OGF = 0x08):
 *        - OCF = 0x000D : LE Create Connection command
 *        - OCF = 0x000E : LE Create Connection Cancel command
 *        - OCF = 0x0013 : LE Connection Update command
 *        - OCF = 0x0014 : LE Set Host Channel Classification command
 *        - OCF = 0x0015 : LE Read Channel Map command
 *        - OCF = 0x0016 : LE Read Remote Features command
 *        - OCF = 0x0020 : LE Remote Connection Parameter Request Reply command
 *        - OCF = 0x0021 : LE Remote Connection Parameter Request Negative Replycommand
 *        - OCF = 0x0022 : LE Set Data Length command
 *        - OCF = 0x0023 : LE Read Suggested Default Data Length command
 *        - OCF = 0x0024 : LE Write Suggested Default Data Length command
 *        - OCF = 0x002F : LE Read Maximum Data Length command
 *        - OCF = 0x0030 : LE Read PHY command
 *        - OCF = 0x0031 : LE Set Default PHY command
 *        - OCF = 0x0032 : LE Set PHY command
 * @}
 */

#ifndef __HCI_CMD_CONNECTION_H__
#define __HCI_CMD_CONNECTION_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "ble_hci.h"



/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** Disconnect command
 *
 * @ingroup hci_cmd_conn.
 *
 * @param p_param : a pointer to disconnect parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_disconn_cmd(ble_hci_cmd_disconnect_param_t *p_param);


/** Read remote version information command
 *
 * @ingroup hci_cmd_conn.
 *
 * @param p_param : a pointer to disconnect parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_read_remote_version_info_cmd(ble_hci_cmd_disconnect_param_t *p_param);


/** Read RSSI command.
 *
 * @ingroup hci_cmd_conn
 *
 * @param p_param : a pointer to the read RSSI parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_read_rssi_cmd(ble_hci_cmd_read_rssi_param_t *p_param);


/** LE Create Connection command.
 *
 * @ingroup hci_cmd_conn
 *
 * @param p_param : a pointer to the create connection parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_create_conn_cmd(ble_hci_cmd_create_conn_param_t *p_param);


/** LE Create Connection Cancel command.
 *
 * @ingroup hci_cmd_conn
 *
 * @param p_param : a pointer to cancel connection parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_create_conn_cancel_cmd(void);


/** LE Connection Update command.
 *
 * @ingroup hci_cmd_conn
 *
 * @param p_param : a pointer to connection update parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_conn_update_cmd(ble_hci_cmd_conn_updated_param_t *p_param);


/** LE Set Host Channel Classification command.
 *
 * @ingroup hci_cmd_conn
 *
 * @param p_param : a pointer to channel map parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_set_host_channel_classif_cmd(ble_hci_cmd_le_channel_classification_t *p_param);


/** LE Read Channel Map command.
 *
 * @ingroup hci_cmd_conn
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_read_channel_map_cmd(ble_hci_cmd_le_read_channel_map_t *p_param);


/** LE Read Remote Features command.
 *
 * @ingroup hci_cmd_conn
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_read_remote_features_cmd(ble_hci_cmd_read_remote_features_t *p_param);


/** LE Remote Connection Parameter Request Reply command.
 *
 * @ingroup hci_cmd_conn
 *
 * @param p_param : a pointer to set connection parameter reeust reply parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_remote_conn_param_req_reply_cmd(ble_hci_cmd_le_remote_conn_param_req_reply_param_t *p_param);


/** LE Remote Connection Parameter Request Negative Replycommand.
 *
 * @ingroup hci_cmd_conn
 *
 * @param p_param : a pointer to set connection parameter reeust negative parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_remote_conn_param_req_neg_reply_cmd(ble_hci_cmd_le_remote_conn_param_req_neg_reply_param_t *p_param);


/** LE Set Data Length command.
 *
 * @ingroup hci_cmd_conn
 *
 * @param p_param : a pointer to set data length parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_set_data_length_cmd(ble_hci_cmd_set_data_length_param_t *p_param);


/** LE Read suggest default data length command.
 *
 * @ingroup hci_cmd_conn
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_read_suggested_default_data_length_cmd(void);


/** LE Read Suggested Default Data Length command.
 *
 * @ingroup hci_cmd_conn
 *
 * @param p_param : a pointer to write default data length parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_write_suggested_default_data_length_cmd(ble_hci_cmd_write_default_data_length_param_t *p_param);


/** LE Read Maximum Data Length command.
 *
 * @ingroup hci_cmd_conn
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
bool hci_le_read_max_data_length_cmd(void);


/** LE Read PHY command.
 *
 * @ingroup hci_cmd_conn
 *
 * @param p_param : a pointer to read PHY parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_read_phy_cmd(ble_hci_cmd_read_phy_param_t *p_param);


/** LE Set Default PHY command.
 *
 * @ingroup hci_cmd_conn
 *
 * @param p_param : a pointer to set default phy parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_set_default_phy_cmd(ble_hci_cmd_set_default_phy_param_t *p_param);


/** LE Set PHY command.
 *
 * @ingroup hci_cmd_conn
 *
 * @param p_param : a pointer to set PHY parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_set_phy_cmd(ble_hci_cmd_set_phy_param_t *p_param);


#endif // #define __HCI_CMD_CONNECTION_H__

