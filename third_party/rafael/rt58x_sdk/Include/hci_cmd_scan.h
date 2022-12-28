/** @file hci_cmd_scan.h
 *
 * @defgroup hci_cmd_scan HCI Scan Related Commands APIs
 * @{
 * @ingroup hci_cmd
 *
 * @brief Includes LE Controller commands (OGF = 0x08):
 *        - OCF = 0x000B : LE Set Scan Parameters command
 *        - OCF = 0x000C : LE Set Scan Enable command
 * @}
 */

#ifndef __HCI_CMD_SCAN_H__
#define __HCI_CMD_SCAN_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "ble_hci.h"


/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** LE Set Scan Parameters command.
 *
 * @ingroup hci_cmd_scan
 *
 * @param p_param : a pointer to the scan parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_set_scan_param_cmd(ble_hci_cmd_set_scan_param_param_t *p_param);


/** LE Set Scan Enable command.
 *
 * @ingroup hci_cmd_scan
 *
 * @param p_param : a pointer to the set scan enable parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_set_scan_enable_cmd(ble_hci_cmd_set_scan_enable_param_t *p_param);


#endif // #define __HCI_CMD_SCAN_H__
