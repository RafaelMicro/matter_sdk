/** @file ble_host_ref.h
 *
 * @brief BLE Queue reference definition file.
 *
 */
/**
 * @defgroup ble_host_ref BLE Host Queue Size reference definition
 * @ingroup BLE_group
 * @{
 * @brief Define BLE Host Queue Size reference definitions, structures.
 * @attention 1. The value of the definitions which are defined in this file shall NOT be modified. \n
 * @}
 */
#ifndef __BLE_HOST_REF_H__
#define __BLE_HOST_REF_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "project_config.h"
#include "task_hci.h"

/* Check if defined "BLE_SUPPORT_NUM_CONN_MAX" or show error message. */
#ifndef BLE_SUPPORT_NUM_CONN_MAX
#error "Undefine BLE_SUPPORT_NUM_CONN_MAX."
#endif
/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/** @brief BLE HCI acl data reassemble parameters.
 * @ingroup ble_host_ref
 * @{
*/
typedef struct
{
    void     *acl_data_ptr;
    uint16_t conn_handle;
    uint16_t total_data_length;
    uint16_t rcvd_data_length;
} hci_acl_data_reassemble;
/** @} */

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
/** @ingroup ble_host_ref
 * @{
*/
/** Define maximum connection links for Link Layer. */
#define NUM_LL_LINK                           BLE_SUPPORT_NUM_CONN_MAX

/** Define the minimum queue size for HCI command events. */
#define NUM_QUEUE_HCI_COMMAND_EVENT           8

/** Define the minimum queue size for HCI ACL data TX. */
#define NUM_QUEUE_HCI_ACL_DATA_TX             50

/** Define maximum connection links for Host Layer. */
#define NUM_HOST_LINK                         BLE_SUPPORT_NUM_CONN_MAX

/** Define the minimum queue size for Host to Application events. */
#define NUM_QUEUE_HOST_TO_APP_EVENT           40

/** Define the minimum queue size for Host to HCI ACL data. */
#define NUM_QUEUE_HOST_TO_HCI_ACL_DATA        8

/** Define the minimum queue size for Host RX. */
#define NUM_QUEUE_HOST_RX                     40

/** Define the minimum queue size for Host encryption. */
#define NUM_QUEUE_HOST_ENCRYPT                10

/** REF_SIZE_CMDTMR for checking the BLE host timers size in host layer. */
#define REF_SIZE_CMDTMR                       60

/**************************************************************************
 * EXTERN DEFINITIONS
 **************************************************************************/
extern uint8_t const MAX_CONN_NO_APP;                                   /**< Define maximum number of BLE connection link for BLE stack. */
extern uint16_t const QUEUE_HCI_COMMAND_EVENT;                          /**< Define the size of the hci command queue for hci task. */
extern uint16_t const QUEUE_HCI_ACL_DATA_TX;                            /**< Define the size of the acl data queue for hci task. */
extern uint16_t const QUEUE_HCI_COMMON;                                 /**< Define the size of the HCI common queue for HCI task.. */
extern uint16_t const QUEUE_HOST_TO_APP_EVENT;                          /**< Define the size of application event buffer queue for host task. */
extern uint16_t const QUEUE_HOST_TO_HCI_ACL_DATA;                       /**< Define the size of acl data buffer queue for host task. */
extern uint16_t const QUEUE_HOST_RX;                                    /**< Define the size of the rx queue for host task. */
extern uint16_t const QUEUE_HOST_ENCRYPT;                               /**< Define the size of the host encrypt queue for host task. */
extern uint32_t param_host_timer[NUM_LL_LINK][REF_SIZE_CMDTMR >> 2];    /**< A predefined and reserved block of memory used for the BLE host timers. */
extern hci_acl_data_reassemble reassemble_acl_data[NUM_LL_LINK];        /**< A predefined and reserved block of memory used for the BLE HCI to reassemble incomplete ACL packets. */
extern uint8_t param_host_identity[BLE_SUPPORT_NUM_CONN_MAX][22];       /**< A predefined and reserved block of memory used for the BLE Identity fuction. */
/** @} */

#ifdef __cplusplus
};
#endif

#endif //__BLE_HOST_REF_H__

