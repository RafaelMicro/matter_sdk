/** @file ble_event.h
 *
 * @brief BLE event header file.
 *
 */

/**
 * @defgroup ble_event BLE Event
 * @ingroup BLE_group
 * @{
 * @brief Define BLE Event definitions, structures, and functions.
 * @}
 */
#ifndef __BLE_EVENT_H__
#define __BLE_EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include "ble_common.h"
#include "ble_advertising.h"
#include "ble_scan.h"
#include "ble_gap.h"
#include "ble_att_gatt.h"
#include "ble_security_manager.h"
#include "ble_connect_cte.h"

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/**
 * @defgroup ble_evt_type BLE Event Type Definition
 * @{
 * @ingroup ble_event
 * @details BLE module event type definition which is indicated the event from BLE stack.
 */
typedef uint8_t ble_module_evt_t;
#define BLE_COMMON_EVT_BASE                                      0x00                           /**< Common module event.*/
#define BLE_COMMON_EVT_SET_CONTROLLER_INFO                      (BLE_COMMON_EVT_BASE + 0x00)    /**< Common module event: set controller information event.*/
#define BLE_COMMON_EVT_SET_EVENT_MASK                           (BLE_COMMON_EVT_BASE + 0x01)    /**< Common module event: set event mask event.*/
#define BLE_COMMON_EVT_READ_BUFFER_SIZE                         (BLE_COMMON_EVT_BASE + 0x02)    /**< Common module event: read buffer size event.*/
#define BLE_COMMON_EVT_READ_LOCAL_VER                           (BLE_COMMON_EVT_BASE + 0x03)    /**< Common module event: read local version event.*/
#define BLE_COMMON_EVT_READ_FILTER_ACCEPT_LIST_SIZE             (BLE_COMMON_EVT_BASE + 0x04)    /**< Common module event: read filter accept list size event.*/
#define BLE_COMMON_EVT_CLEAR_FILTER_ACCEPT_LIST                 (BLE_COMMON_EVT_BASE + 0x05)    /**< Common module event: clear filter accept list event.*/
#define BLE_COMMON_EVT_ADD_FILTER_ACCEPT_LIST                   (BLE_COMMON_EVT_BASE + 0x06)    /**< Common module event: add filter accept list event.*/
#define BLE_COMMON_EVT_REMOVE_FILTER_ACCEPT_LIST                (BLE_COMMON_EVT_BASE + 0x07)    /**< Common module event: remove filter accept list event.*/
#define BLE_COMMON_EVT_READ_ANTENNA_INFO                        (BLE_COMMON_EVT_BASE + 0x08)    /**< Common module event: read antenna information event.*/

#define BLE_GAP_EVT_BASE                                         0x10                           /**< GAP module event.*/
#define BLE_GAP_EVT_SET_RANDOM_ADDR                             (BLE_GAP_EVT_BASE + 0x00)       /**< GAP module event: set random address event.*/
#define BLE_GAP_EVT_SET_VENDOR_CMD                              (BLE_GAP_EVT_BASE + 0x01)       /**< GAP module event: set vendor command event. */
#define BLE_GAP_EVT_CONN_COMPLETE                               (BLE_GAP_EVT_BASE + 0x02)       /**< GAP module event: connection complete event. */
#define BLE_GAP_EVT_CONN_CANCEL                                 (BLE_GAP_EVT_BASE + 0x03)       /**< GAP module event: connection cancel event.*/
#define BLE_GAP_EVT_CONN_PARAM_UPDATE                           (BLE_GAP_EVT_BASE + 0x04)       /**< GAP module event: connection parameter update event.*/
#define BLE_GAP_EVT_DISCONN_COMPLETE                            (BLE_GAP_EVT_BASE + 0x05)       /**< GAP module event: disconnect event. */
#define BLE_GAP_EVT_PHY_READ                                    (BLE_GAP_EVT_BASE + 0x06)       /**< GAP module event: PHY read event. */
#define BLE_GAP_EVT_PHY_UPDATE                                  (BLE_GAP_EVT_BASE + 0x07)       /**< GAP module event: PHY update event. */
#define BLE_GAP_EVT_RSSI_READ                                   (BLE_GAP_EVT_BASE + 0x08)       /**< GAP module event: RSSI read event. */
#define BLE_GAP_EVT_SET_LE_HOST_CH_CLASSIFICATION               (BLE_GAP_EVT_BASE + 0x09)       /**< GAP module event: set le host channel classification event. */
#define BLE_GAP_EVT_READ_CHANNEL_MAP                            (BLE_GAP_EVT_BASE + 0x0A)       /**< GAP module event: channel map read event. */

#define BLE_CTE_EVT_BASE                                         0x20                           /**< CTE module event.*/
#define BLE_CTE_EVT_SET_CONN_CTE_RX_PARAM                       (BLE_CTE_EVT_BASE + 0x00)       /**< CTE module event: set connection cte rx oaraneters event. */
#define BLE_CTE_EVT_SET_CONN_CTE_TX_PARAM                       (BLE_CTE_EVT_BASE + 0x01)       /**< CTE module event: set connection cte tx parameters event. */
#define BLE_CTE_EVT_SET_CONN_CTE_REQ                            (BLE_CTE_EVT_BASE + 0x02)       /**< CTE module event: set connection cte request event. */
#define BLE_CTE_EVT_SET_CONN_CTE_RSP                            (BLE_CTE_EVT_BASE + 0x03)       /**< CTE module event: set connection cte response event. */
#define BLE_CTE_EVT_IQ_REPORT                                   (BLE_CTE_EVT_BASE + 0x04)       /**< CTE module event: connection IQ event. */
#define BLE_CTE_EVT_CTE_REQ_FAILED                              (BLE_CTE_EVT_BASE + 0x05)       /**< CTE module event: CTE Request failed event. */

#define BLE_ADV_EVT_BASE                                        0x30                            /**< Advertising module event.*/
#define BLE_ADV_EVT_SET_PARAM                                   (BLE_ADV_EVT_BASE + 0x00)       /**< Advertising module event: set parameter event. */
#define BLE_ADV_EVT_SET_DATA                                    (BLE_ADV_EVT_BASE + 0x01)       /**< Advertising module event: set data event. */
#define BLE_ADV_EVT_SET_SCAN_RSP                                (BLE_ADV_EVT_BASE + 0x02)       /**< Advertising module event: set scan response event. */
#define BLE_ADV_EVT_SET_ENABLE                                  (BLE_ADV_EVT_BASE + 0x03)       /**< Advertising module event: set enable event. */

#define BLE_SCAN_EVT_BASE                                       0x40                            /**< Scan module event.*/
#define BLE_SCAN_EVT_SET_PARAM                                  (BLE_SCAN_EVT_BASE + 0x00)      /**< Scan module event: set parameter event. */
#define BLE_SCAN_EVT_SET_ENABLE                                 (BLE_SCAN_EVT_BASE + 0x01)      /**< Scan module event: set enable event. */
#define BLE_SCAN_EVT_ADV_REPORT                                 (BLE_SCAN_EVT_BASE + 0x02)      /**< Scan module event: advertising report event. */

#define BLE_ATT_GATT_EVT_BASE                                   0x50                            /**< ATT/ GATT module event.*/
#define BLE_ATT_GATT_EVT_DB_PARSE_COMPLETE                      (BLE_ATT_GATT_EVT_BASE + 0x00)  /**< ATT/ GATT module event: DB parsing complete event. */
#define BLE_ATT_GATT_EVT_GET_ATT_HANDLES_TABLE_COMPLETE         (BLE_ATT_GATT_EVT_BASE + 0x01)  /**< ATT/ GATT module event: Get ATT handles complete event. */
#define BLE_ATT_GATT_EVT_MTU_EXCHANGE                           (BLE_ATT_GATT_EVT_BASE + 0x02)  /**< ATT/ GATT module event: MTU change event. */
#define BLE_ATT_GATT_EVT_DATA_LENGTH_CHANGE                     (BLE_ATT_GATT_EVT_BASE + 0x03)  /**< ATT/ GATT module event: data length change event. */
#define BLE_ATT_GATT_EVT_WRITE_SUGGESTED_DEFAULT_DATA_LENGTH    (BLE_ATT_GATT_EVT_BASE + 0x04)  /**< ATT/ GATT module event: write suggested default data length event.*/
#define BLE_ATT_GATT_EVT_DATA_LENGTH_SET                        (BLE_ATT_GATT_EVT_BASE + 0x05)  /**< ATT/ GATT module event: data length set event. */

#define BLE_SM_EVT_BASE                                         0x60                            /**< Security module event.*/
#define BLE_SM_EVT_STK_GENERATION_METHOD                        (BLE_SM_EVT_BASE + 0x00)        /**< Security module event: STK generation method event. */
#define BLE_SM_EVT_PASSKEY_CONFIRM                              (BLE_SM_EVT_BASE + 0x01)        /**< Security module event: passkey comfirmation event. */
#define BLE_SM_EVT_AUTH_STATUS                                  (BLE_SM_EVT_BASE + 0x02)        /**< Security module event: authentication status event. */
#define BLE_SM_EVT_IRK_RESOLVING_FAIL                           (BLE_SM_EVT_BASE + 0x03)        /**< Security module event: resolving private address status event. */

#define BLE_VENDOR_EVT_BASE                                     0x80                            /**< vendor event.*/
#define BLE_VENDOR_EVT_SCAN_REQ_REPORT                          (BLE_VENDOR_EVT_BASE + 0x00)    /**< vendor event: scan request report event. */
/** @} */


/**
 * @brief BLE Event Parameters.
 * @ingroup ble_event
 */
typedef struct ble_evt_param_s
{
    ble_module_evt_t         event;    /**< BLE event @ref ble_evt_type "ble_module_evt_t". */
    union
    {
        ble_evt_common_t        ble_evt_common;     /**< BLE common related event parameters. */
        ble_evt_gap_t           ble_evt_gap;        /**< BLE GAP related event parameters. */
        ble_evt_adv_t           ble_evt_adv;        /**< BLE advertising related event parameters. */
        ble_evt_scan_t          ble_evt_scan;       /**< BLE scan related event parameters. */
        ble_evt_att_gatt_t      ble_evt_att_gatt;   /**< BLE ATT/GATT related event parameters. */
        ble_evt_sm_t            ble_evt_sm;         /**< BLE security manager related event parameters. */
        ble_evt_cte_t           ble_evt_cte;        /**< BLE connection CTE event parameters. */
    } event_param;                                  /**< Event parameters. */
} ble_evt_param_t;


#ifdef __cplusplus
};
#endif

#endif /* __BLE_EVENT_H__*/

