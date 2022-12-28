/** @file hci_cmd_vendor.h
 *
 * @defgroup hci_cmd_vendor HCI Vendor Commands APIs
 * @{
 * @ingroup hci_cmd
 * @brief Includes Vendor commands (OGF = 0x3F):
 *        - OCF = 0x0001 : Set Controller Information vendor command
 * @}
 */

#ifndef __HCI_CMD_VENDOR_H__
#define __HCI_CMD_VENDOR_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "ble_hci.h"


/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** Set Controller Information vendor command.
 *
 * @ingroup hci_cmd_vendor
 *
 * @param p_param : a pointer to the ventdor controller information parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_vendor_set_controller_info_cmd(ble_hci_vcmd_cntlr_info_param_t *p_param);


/** Set scan report vendor command.
 *
 * @ingroup hci_cmd_vendor
 *
 * @param p_param : a pointer to the ventdor scan request report parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_vendor_set_scan_request_report_cmd(ble_hci_vcmd_scan_req_rpt_param_t *p_param);


#endif // __HCI_CMD_VENDOR_H__



