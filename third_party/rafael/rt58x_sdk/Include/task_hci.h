/**
 * @defgroup RF_FW_Control_group RF FW Control Task
 * @{
 * @brief Define task for control RF FW such as HCI task, PCI task, and dual mode task.
 * @}
 */

/**
 * @defgroup task_hci TASK HCI
 * @ingroup RF_FW_Control_group
 * @{
 * @brief Define task HCI definitions, structures, and functions.
 * @}
 */

#ifndef __TASK_HCI_H__
#define __TASK_HCI_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "project_config.h"
#include "sys_arch.h"

#include "rf_mcu.h"

#include "rf_common_init.h"

#include "ble_host_ref.h"

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/

#define PHY_STATUS (0x05)     /**< Length of PHY status (HW generate). */
#define CRC (0x03)            /**< Length of CRC (HW generate).*/
#define MIC (0x04)            /**< Length of MIC (HW generate).*/
#define HCI_PKT_IND (0x01)    /**< Length of HCI packet indicator. */
#define HANDLE_PB_PC (0x02)   /**< Length of HCI ACL data header, handle + PB + PC. */
#define DATA_TOTAL_LEN (0x02) /**< Length of HCI ACL data header, data total length. */

#define BLE_TRANSPORT_HCI_COMMAND (0x01)  /**< HCI packet indicators for command packet. */
#define BLE_TRANSPORT_HCI_ACL_DATA (0x02) /**< HCI packet indicators for ACL data packet. */
#define BLE_TRANSPORT_HCI_EVENT (0x04)    /**< HCI packet indicators for event packet. */

#define HCI_ACL_DATA_MAX_LENGTH (251 + MIC) /**< Maximum ACL data length. */
#define HCI_ARRAY_MAX_LENGTH                                                                                                       \
    (HCI_ACL_DATA_MAX_LENGTH + PHY_STATUS + CRC + HCI_PKT_IND + HANDLE_PB_PC +                                                     \
     DATA_TOTAL_LEN) /**< Maximum data length get from HW. */

#define ISR_MSG_RX_EVENT (0x00) /**< Message for isr to notify RX event. */
#define ISR_MSG_RX_DATA (0x01)  /**< Message for isr to notify RX data */
#define ISR_MSG_TRAP (0x02)     /**< Message for isr to notify that controller fall into trap */
#define ISR_MSG_TX_DONE (0x03)  /**< Message for isr to notify TX done */
#define MSG_RX_EVENT (0x04)
#define MSG_RX_DATA (0x05)
#define ISR_MSG_MAX (0x10)
/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/**
 * @brief The different message types of HCI task.
 * @ingroup task_hci
 */

typedef enum
{
    HCI_MSG_BASE = 0x0010,           /**< Message base that send to HCI task */
    HCI_MSG_HOST_HCI_CMD,            /**< Message for upper layer to notify HCI TX HCI command */
    HCI_MSG_HOST_HCI_ACL_DATA,       /**< Message for upper layer to notify HCI TX ACL data */
    HCI_MSG_HOST_NOCP_EVENT,         /**< Message for Host to notify HCI how many TX ACL data complete*/
    HCI_MSG_HOST_HCI_ACL_DATA_CLEAR, /**< Message for Host to notify HCI how many TX ACL data complete*/
    HCI_MSG_MAX = 0x0020,
} hci_msg_tag_def_t;

/**
 * @brief The parameter definition of HCI task transmit HCI command.
 * @ingroup task_hci
 */

typedef struct __attribute__((packed))
{
    uint8_t transport_id; /**< Transport id. */
    uint16_t ocf : 10;    /**< Opcode command field. */
    uint16_t ogf : 6;     /**< Opcode group field. */
    uint8_t length;       /**< Length. */
    uint8_t parameter[];  /**< Parameters. */
} ble_hci_command_hdr_t;

/**
 * @brief The parameter definition of HCI task receive HCI ACL data.
 * @ingroup task_hci
 */

typedef struct __attribute__((packed))
{
    uint8_t transport_id; /**< Transport id. */
    uint16_t handle : 12; /**< Connection id. */
    uint16_t pb_flag : 2; /**< Packet boundary flag. */
    uint16_t bc_flag : 2; /**< Broadcast flag. */
    uint16_t length;      /**< Data total length. */
    uint8_t data[];       /**< ACL data. */
} ble_hci_rx_acl_data_hdr_t;

/**
 * @brief The parameter definition of HCI task transmit HCI ACL data.
 * @ingroup task_hci
 */

typedef struct __attribute__((packed))
{
    uint8_t transport_id; /**< Transport id. */
    uint8_t sequence;     /**< Sequence number. */
    uint16_t handle : 12; /**< Connection id. */
    uint16_t pb_flag : 2; /**< Packet boundary flag. */
    uint16_t bc_flag : 2; /**< Broadcast flag. */
    uint16_t length;      /**< Data total length. */
    void * p_data;        /**< ACL data. */
} ble_hci_tx_acl_data_hdr_t;

/**
 * @brief The parameter definition of HCI task ACL data clear by conn handle.
 * @ingroup task_hci
 */

typedef struct __attribute__((packed))
{
    uint16_t conn_handle; /**< Connection id. */
} ble_hci_acl_data_clear_t;

/**
 * @brief The parameter definition of HCI task receive HCI event.
 * @ingroup task_hci
 */

typedef struct __attribute__((packed))
{
    uint8_t transport_id; /**< Transport id. */
    uint8_t event_code;   /**< Event. */
    uint8_t length;       /**< Event parameter length. */
    uint8_t parameter[];  /**< Parameter. */
} ble_hci_event_t;

/**
 * @brief The parameter definition of BLE host to notify HCI task how many transmitted ACL data completed.
 * @ingroup task_hci
 */

typedef struct __attribute__((packed))
{
    uint16_t complete_num; /**< Complete data number. */
} ble_hci_acl_data_complete_event_t;

/**
 * @brief All messages that HCI task can handle.
 * @ingroup task_hci
 */

typedef struct
{
    union
    {
        uint8_t ble_hci_array[HCI_ARRAY_MAX_LENGTH]; /**< HCI array for read data from communication subsystem. */
        ble_hci_command_hdr_t hci_command;           /**< HCI command format send to controller. */
        ble_hci_tx_acl_data_hdr_t hci_tx_acl_data;   /**< HCI ACL data format send to controller. */
        ble_hci_rx_acl_data_hdr_t hci_rx_acl_data;   /**< HCI ACL data format get from controller. */
        ble_hci_event_t hci_event;                   /**< HCI event format get from controller. */
        ble_hci_acl_data_complete_event_t hci_data;  /**< ACL data complete number event get from BLE host. */
        ble_hci_acl_data_clear_t hci_data_clear;     /**< ACL data clear. */
    } msg_type;
} ble_hci_message_struct_t;

/**
 * @brief The message parameter of HCI task.
 * @ingroup task_hci
 */

typedef struct
{
    uint32_t hci_msg_tag;                   /**< hci message tag. */
    ble_hci_message_struct_t * hci_msg_ptr; /**< hci message pointer. */
} hci_task_common_queue_t;

/**************************************************************************
 * EXTERN DEFINITIONS
 **************************************************************************/
/**@brief The handle of HCI common queue.
 * @ingroup task_hci
 */

extern sys_queue_t g_hci_common_handle;

/**@brief The handle of HCI TX ACL data queue.
 * @ingroup task_hci
 */

extern sys_queue_t g_hci_tx_acl_handle;

/**@brief The handle of HCI TX command queue.
 * @ingroup task_hci
 */

extern sys_queue_t g_hci_tx_cmd_handle;

/**@brief The handle of HCI RC Common command queue.
 * @ingroup task_hci
 */
extern sys_queue_t g_rx_common_queue_handle;

/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/**@brief BLE HCI task initialization.
 *
 * @ingroup task_hci
 *
 * @return none
 */
void task_hci_init(void);

/**@brief BLE HCI task delete.
 *
 * @ingroup task_hci
 *
 * @return none
 */
void task_hci_delete(void);

#ifdef __cplusplus
};
#endif

#endif /* __TASK_HCI_H__*/
