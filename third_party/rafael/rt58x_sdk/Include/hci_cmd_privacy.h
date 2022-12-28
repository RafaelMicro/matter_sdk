/** @file hci_cmd_privacy.h
 *
 * @brief Includes LE Controller commands (OGF = 0x08):
 *        - OCF = 0x0017 : LE Encrypt command
 *        - OCF = 0x0018 : LE Rand command
 *        - OCF = 0x0019 : LE Enable Encryption command
 *        - OCF = 0x001A : LE Long Term Key Request Reply command
 *        - OCF = 0x001B : LE Long Term Key Request Negative Reply command
 */

#ifndef __HCI_CMD_PRIVACY_H__
#define __HCI_CMD_PRIVACY_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "ble_hci.h"


/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** LE Add Device To Resolving List command.
 *
 * @ingroup hci_cmd_privacy
 *
 */
int8_t hci_le_add_device_to_resolving_list_cmd(ble_hci_cmd_add_device_to_resolving_list_param_t *p_param);


/** LE Remove Device From Resolving List command.
 *
 * @ingroup hci_cmd_privacy
 *
 */
int8_t hci_le_remove_device_from_resolving_list_cmd(ble_hci_cmd_remove_device_from_resolving_list_param_t *p_param);


/** LE Clear Resolving List command.
 *
 * @ingroup hci_cmd_privacy
 *
 */
int8_t hci_le_clear_resolving_list_cmd(void);


/** LE Read Resolving List Size command.
 *
 * @ingroup hci_cmd_privacy
 *
 */
int8_t hci_le_read_resolving_list_size_cmd(void);


/** LE Read Peer Resolvable Address command.
 *
 * @ingroup hci_cmd_privacy
 *
 */
int8_t hci_le_read_peer_resolvable_addr_cmd(ble_hci_cmd_read_peer_resolvable_addr_param_t *p_param);


/** LE Read Local Resolvable Address command.
 *
 * @ingroup hci_cmd_privacy
 *
 */
int8_t hci_le_read_local_resolvable_addr_cmd(ble_hci_cmd_read_local_resolvable_addr_param_t *p_param);


/** LE Set Address Resolution Enable command.
 *
 * @ingroup hci_cmd_privacy
 *
 */
int8_t hci_le_set_addr_resolution_enable_cmd(ble_hci_cmd_set_addr_resolution_enable_param_t *p_param);


/** LE Set Privacy Mode command.
 *
 * @ingroup hci_cmd_privacy
 *
 */
int8_t hci_le_set_privacy_mode_cmd(ble_hci_cmd_set_privacy_mode_param_t *p_param);


#endif // __HCI_CMD_PRIVACY_H__

