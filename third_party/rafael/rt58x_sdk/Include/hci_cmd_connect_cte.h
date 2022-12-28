/** @file hci_cmd_connect_cte.h
 *
 * @brief Includes LE Controller commands (OGF = 0x08):
 *        - OCF = 0x0054 : LE Set Connection CTE Receive Parameters command
 *        - OCF = 0x0055 : LE Set Connection CTE Transmit Parameters command
 *        - OCF = 0x0056 : LE Connection CTE Request Enable command
 *        - OCF = 0x0057 : LE Connection CTE Response Enable command
 */

#ifndef __HCI_CMD_CONN_CTE_H__
#define __HCI_CMD_CONN_CTE_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "ble_hci.h"


/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** LE Set Connection CTE Receive Parameters command.
 *
 * @ingroup hci_cmd_conn_cte
 *
 */
int8_t hci_le_set_conn_cte_rx_param_cmd(ble_hci_cmd_set_conn_cte_rx_param_t *p_param);


/** LE Set Connection CTE Transmit Parameters command.
 *
 * @ingroup hci_cmd_conn_cte
 *
 */
int8_t hci_le_set_conn_cte_tx_param_cmd(ble_hci_cmd_set_conn_cte_tx_param_t *p_param);


/** LE Connection CTE Request Enable command.
 *
 * @ingroup hci_cmd_conn_cte
 *
 */
int8_t hci_le_conn_cte_req_enable_cmd(ble_hci_cmd_set_conn_cte_req_param_t *p_param);


/** LE Connection CTE Response Enable command.
 *
 * @ingroup hci_cmd_conn_cte
 *
 */
int8_t hci_le_conn_cte_rsp_enable_cmd(ble_hci_cmd_set_conn_cte_rsp_param_t *p_param);


#endif // __HCI_CMD_CONN_CTE_H__

