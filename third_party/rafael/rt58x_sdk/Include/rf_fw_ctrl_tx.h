
#ifndef __RF_FW_CTRL_TX_H__
#define __RF_FW_CTRL_TX_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "sys_arch.h"
#if (MODULE_ENABLE(SUPPORT_ZB))
#include "task_pci.h"
#endif
#if (MODULE_ENABLE(SUPPORT_BLE))
#include "task_host.h"
#endif

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/

#define TX_TYPE_DATA 0x00
#define TX_TYPE_CMD  0x01
/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

typedef enum
{
    INTERNAL_MSG_BASE = 0x0000,
    INTERNAL_MSG_TX_DONE,
    INTERNAL_MSG_MAX = 0x0020,
} rf_fw_ctrl_internal_msg_def_t;

typedef struct
{
    uint32_t msg_tag;                          /**< hci message tag. */
    union
    {
#if (MODULE_ENABLE(SUPPORT_BLE))
        ble_hci_message_struct_t *hci_msg_ptr;        /**< this parameter saving pointer. */
#endif
#if (MODULE_ENABLE(SUPPORT_ZB))
        pci_message_param_type_struct_t pci_msg;     /**< this parameter saving pointer or variable. */
        uint32_t param_var;                          /**< this parameter saving variable.*/
#endif
    } param_type;
} rf_fw_ctrl_msg_t;

/**************************************************************************
 * EXTERN DEFINITIONS
 **************************************************************************/
/**@brief The handle of HCI common queue.
 * @ingroup task_hci
 */

extern sys_queue_t g_rfc_common_queue_handle;


/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/**@brief BLE HCI task initialization.
 *
 * @ingroup task_hci
 *
 * @return none
 */
void task_tx_init(void);


/**@brief Set to BLE HCI Tx task delete.
 *
 * @ingroup task_hci
 *
 * @return none
 */
void task_tx_delete(void);

#ifdef __cplusplus
};
#endif

#endif /* __RF_FW_CTRL_TX_H__*/
