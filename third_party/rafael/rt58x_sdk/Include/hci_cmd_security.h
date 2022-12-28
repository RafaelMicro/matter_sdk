/** @file hci_cmd_security.h
 *
 * @defgroup hci_cmd_sec HCI Security Related Commands APIs
 * @{
 * @ingroup hci_cmd
 *
 * @brief Includes LE Controller commands (OGF = 0x08):
 *        - OCF = 0x0017 : LE Encrypt command
 *        - OCF = 0x0018 : LE Rand command
 *        - OCF = 0x0019 : LE Enable Encryption command
 *        - OCF = 0x001A : LE Long Term Key Request Reply command
 *        - OCF = 0x001B : LE Long Term Key Request Negative Reply command
 * @}
 */

#ifndef __HCI_CMD_SECURITY_H__
#define __HCI_CMD_SECURITY_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "ble_hci.h"


/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** LE Encrypt command.
 *
 * @ingroup hci_cmd_sec
 *
 * @param p_param : a pointer to the encrypt parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_encrypt_cmd(ble_hci_le_encrypt_param_t *p_param);


/** LE Rand command.
 *
 * @ingroup hci_cmd_sec
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_random_cmd(void);


/** LE Enable Encryption command.
 *
 * @ingroup hci_cmd_sec
 *
 * @param p_param : a pointer to the encryption enable parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_enable_encryption_cmd(ble_hci_le_enable_encrypt_param_t *p_param);


/** LE Long Term Key Request Reply command.
 *
 * @ingroup hci_cmd_sec
 *
 * @param p_param : a pointer to the long term key request reply parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_long_term_key_req_reply_cmd(ble_hci_le_long_term_key_req_reply_param_t *p_param);


/** LE Long Term Key Request Negative Reply command.
 *
 * @ingroup hci_cmd_sec
 *
 * @param p_param : a pointer to the long term key negative reply parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_long_term_key_req_neg_reply_cmd(ble_hci_le_long_term_key_neg_reply_param_t *p_param);


#endif // __HCI_CMD_SECURITY_H__

