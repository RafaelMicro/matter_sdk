/** @file hci_cmd_controller.h
 *
 * @defgroup hci_cmd_cntlr HCI Controller Related Commands APIs
 * @{
 * @ingroup hci_cmd
 *
 * @brief Includes Controller & Baseband commands (OGF = 0x03):
 *        - OCF = 0x0001 : Set Event Mask command
 *        - OCF = 0x0003 : Reset command
 *
 *        Includes Information Parameters commands (OGF = 0x04):
 *        - OCF = 0x0001 : Read Local Version Information command
 *        - OCF = 0x0002 : Read Local Supported Commands command
 *        - OCF = 0x0003 : Read Local Supported Features command
 *        - OCF = 0x0005 : Read Buffer Size command
 *        - OCF = 0x0009 : Read BD_ADDR command
 *
 *        Includes LE Controller commands (OGF = 0x08):
 *        - OCF = 0x0001 : LE Set Event Mask command
 *        - OCF = 0x0002 : LE Read Buffer Size command
 *        - OCF = 0x0003 : LE Read Local Supported Features command
 *        - OCF = 0x0005 : LE Set Random Address command
 *        - OCF = 0x000F : LE Read Filter Accept List Size command
 *        - OCF = 0x0010 : LE Clear Filter Accept List command
 *        - OCF = 0x0011 : LE Add Device To Filter Accept List command
 *        - OCF = 0x0012 : LE Remove Device From Filter Accept List command
 *        - OCF = 0x001C : LE Read Supported States command
 *        - OCF = 0x0058 : LE Read Antenna Information command
 * @}
 */

#ifndef __HCI_CMD_CONTROLLER_H__
#define __HCI_CMD_CONTROLLER_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "ble_hci.h"


/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** Set Event Mask command.
 *
 * @ingroup hci_cmd_cntlr
 *
 */
//bool hci_set_event_mask_cmd(void);


/** Reset command.
 *
 * @ingroup hci_cmd_cntlr
 *
 */
//bool hci_reset_cmd(void);


/** Read Local Version Information command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_read_local_ver_info_cmd(void);


/** Read Local Supported Commands command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_read_local_supported_cmds_cmd(void);


/** Read Local Supported Features command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_read_local_supported_feature_cmd(void);



/** Read BD_ADDR command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_read_bd_addr_cmd(void);


/** LE Set Event Mask command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @param p_param : a pointer to the envent mask.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_set_event_mask_cmd(uint8_t *p_param);


/** LE Read Buffer Size command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_read_buffer_size_cmd(void);


/** LE Read Local Supported Features command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_read_local_supported_feature_cmd(void);



/** LE Set Random Address command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @param p_param : a pointer to the set random address parameter.
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_set_random_addr_cmd(ble_hci_cmd_set_random_addr_param_t *p_param);


/** LE Read Filter Accept List Size command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_read_filter_accept_list_size_cmd(void);


/** LE Clear Filter Accept List command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_clear_filter_accept_list_cmd(void);


/** LE Add Device To Filter Accept List command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_add_device_to_filter_accept_list_cmd(ble_hci_cmd_add_device_to_accept_list_t *p_param);


/** LE Remove Device From Filter Accept List command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_remove_device_from_filter_accept_list_cmd(ble_hci_cmd_remove_device_from_accept_list_t *p_param);


/** LE Read Supported States command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_read_supported_state_cmd(void);


/** LE Read Antenna Information command.
 *
 * @ingroup hci_cmd_cntlr
 *
 * @return ERR_OK   : success.
 * @return ERR_MEM  : failed to send HCI command.
 */
int8_t hci_le_read_antenna_info_cmd(void);


#endif // #define __HCI_CMD_CONTROLLER_H__


