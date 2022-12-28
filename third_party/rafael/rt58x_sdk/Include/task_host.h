/** @file task_host.h
 *
 * @brief Handle BLE HCI event and ACL data.
 *
 * @addtogroup BLE
 * @{
 * @addtogroup Task
 * @{
 * @defgroup task_host BLE Host Task
 * @{ @}
 * @}
 * @}
 */

#ifndef __TASK_HOST_H__
#define __TASK_HOST_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include "sys_arch.h"
#include "task_hci.h"
#include "ble_att_gatt.h"
#include "ble_event_module.h"

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/**@brief The different queue types of BLE host data.
 * @ingroup task_host
 */
typedef enum
{
    TASK_HOST_QUEUE_TX_ACL_DATA,          /**< Queue type: TX ACL data request. */
    TASK_HOST_QUEUE_TO_APP,               /**< Queue type: transmit BLE event or service data to BLE application task request. */
    TASK_HOST_QUEUE_TO_RX_COMMON,         /**< Queue type: RX common request. */
} task_host_queue_type_t;


/**@brief The different message types of BLE host data.
 * @ingroup task_host
 */
typedef enum HOST_MSG_TAG_DEF
{
    HOST_MSG_ISR_BASE = 0x0000,
    HOST_MSG_TX_HCI_CMD = HOST_MSG_ISR_BASE,
    HOST_MSG_RX_HCI_EVENT,
    HOST_MSG_RX_HCI_ACL_DATA,
    HOST_MSG_DB_PARSING_EVENT,
    HOST_MSG_AUTHE_EVENT,
    HOST_MSG_AUTHO_EVENT,
    HOST_MSG_BY_PASS_GENERAL_EVENT,
    HOST_MSG_SM_BOND_SPACE_INT,
    HOST_MSG_APP_MAX,
} host_msg_tag_def_t;


/**@brief BLE host TX event parameter definition.
 * @ingroup task_host
 */
typedef struct __attribute__((packed)) host_rx_event_param_s
{
    uint8_t event_code;     /**< Event. */
    uint8_t parameter[];    /**< Parameters. */
} host_rx_event_param_t;


/**@brief BLE host RX ACL data parameter definition.
 * @ingroup task_host
 */
typedef struct __attribute__((packed)) host_rx_acl_data_param_s
{
    uint16_t conn_handle;   /**< Connection id. */
    uint8_t parameter[];    /**< Parameters. */
} host_rx_acl_data_param_t;



/**@brief BLE host encryption parameter definition.
 * @ingroup task_host
 */
typedef struct encrypt_queue_s
{
    uint8_t host_id;        /**< Host id. */
    uint8_t encrypt_state;  /**< Encryption state. */
} encrypt_queue_t;



/**@brief BLE host event parameter definition.
 * @ingroup task_host
 */
typedef struct
{
    uint32_t  u32_send_systick;
    uint32_t  evt_type;
    uint8_t   parameter[];  /**< Parameters. */
} ble_host_to_app_evt_t;


/**************************************************************************
 * EXTERN DEFINITIONS
 **************************************************************************/

/** @brief The handle of BLE host RX queue.
 * @ingroup task_host
*/
extern sys_queue_t g_host_rx_handle;

/** @brief  The handle of BLE host encryption queue.
 * @ingroup task_host
*/
extern sys_queue_t g_host_encrypt_handle;

/** @brief  The handle of BLE command transport queue.
 * @ingroup task_host
*/
extern sys_queue_t g_cmd_transport_handle;

/** @brief  The handle of BLE host sw timer.
 * @ingroup task_host
*/
extern TimerHandle_t g_ble_host_timer;


/** @brief The handle of BLE host RX cmd semaphore.
 * @ingroup task_host
*/
extern sys_sem_t g_host_rx_cmd_sem;


/** @brief The handle of BLE host RX data semaphore.
 * @ingroup task_host
*/
extern sys_sem_t g_host_rx_data_sem;


/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/**@brief BLE host subsystem initialization.
 *
 * @ingroup host_subsystem
 *
 * @return status
 */
int8_t ble_host_subsystem_init(void);

/**@brief BLE host subsystem delete initialization.
 *
 * @ingroup host_subsystem
 *
 * @return status
 */
int8_t ble_delete_host_subsystem(void);

/**@brief Retrun the space of free space in BLE host ACL data queue.
 *
 * @ingroup host_subsystem
 *
 * @return The number of free spaces available in the queue.
 */
uint32_t host_acl_data_queue_remaining_size(void);


/**@brief Retrun the space of free space in BLE host encrypt queue.
 *
 * @ingroup host_subsystem
 *
 * @return The number of free spaces available in the queue.
 */
uint32_t host_encrypt_queue_remaining_size(void);


/**@brief Post the data on the BLE host queue by @ref task_host_queue_type_t "the queue type".
 *
 * @ingroup host_subsystem
 *
 * @param queue_type : BLE host data type.
 * @param p_data: a pointer to the data.
 *
 * @return none
 */
void task_host_queue_send(task_host_queue_type_t queue_type, void *p_data);


/**@brief Post the command to HCI task on the BLE HCI common queue.
 *
 * @ingroup host_subsystem
 *
 * @param hci_comm_msg : data.
 *
 * @retval TRUE: command sent successfully.
 * @retval FALSE: command sent failed.
 */
uint8_t host_send_cmd_to_hci(hci_task_common_queue_t hci_comm_msg);



/**@brief Post the ACL data completed event to HCI task on the BLE HCI common queue.
 *
 * @ingroup host_subsystem
 *
 * @param num_of_complete_acl_data : number of completed ACL data.
 *
 * @return none
 */
int8_t host_notify_acl_data_complete(uint16_t num_of_complete_acl_data);



#ifdef __cplusplus
};
#endif

#endif /* __TASK_HOST_H__*/
