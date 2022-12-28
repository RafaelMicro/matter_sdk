/** @file ble_hci_event.h
 *
 * @addtogroup BLE
 * @{
 * @addtogroup hci HCI
 * @{
 * @addtogroup hci_event HCI events
 * @{
 * @brief BLE HCI data definitions.
 * @}
 * @}
 * @}
 */

#ifndef __BLE_HCI_EVENT_H__
#define __BLE_HCI_EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "ble_gap.h"
/**************************************************
  HCI Event Code
 *************************************************/

/**
 * @ingroup hci_event
 * @{
 */
#define HCI_EVENT_CODE_INQUIRY_COMPLETE                                         0x01  /**< Inquiry complete event. */
#define HCI_EVENT_CODE_INQUIRY_RESULT                                           0x02  /**< Inquiry result event. */
#define HCI_EVENT_CODE_CONNECTION_COMPLETE                                      0x03  /**< Connection complete event. */
#define HCI_EVENT_CODE_CONNECTION_REQUEST                                       0x04  /**< Connection request event. */
#define HCI_EVENT_CODE_DISCONNECTION_COMPLETE                                   0x05  /**< Disconnection complete event. */
#define HCI_EVENT_CODE_AUTHENTICATION_COMPLETE                                  0x06  /**< Authentication complete event. */
#define HCI_EVENT_CODE_REMOTE_NAME_REQUEST_COMPLETE                             0x07  /**< Rmote name request complete event. */
#define HCI_EVENT_CODE_ENCRYPTION_CHANGE                                        0x08  /**< Encryption change event. */
#define HCI_EVENT_CODE_CHANGE_CONNECTION_LINK_KEY_COMLETE                       0x09  /**< Change connection link key complate event. */
#define HCI_EVENT_CODE_LINK_KEY_TYPE_CHANGED                                    0x0A  /**< Link key type changed event. */
#define HCI_EVENT_CODE_READ_REMOTE_SUPPORTED_FEATURES_COMPLETE                  0x0B  /**< Read remote supported features complete event. */
#define HCI_EVENT_CODE_READ_REMOTE_VERSION_INFORMATION_COMPLETE                 0x0C  /**< Read remote version information complete event. */
#define HCI_EVENT_CODE_QOS_SETUP_COMPLETE                                       0x0D  /**< QoS setup complete event. */
#define HCI_EVENT_CODE_COMMAND_COMPLETE                                         0x0E  /**< Command complete event. */
#define HCI_EVENT_CODE_COMMAND_STATUS                                           0x0F  /**< Command status event. */
#define HCI_EVENT_CODE_HARDWARE_ERROR                                           0x10  /**< Hardware error event. */
#define HCI_EVENT_CODE_FLUSH_OCCURRED                                           0x11  /**< Flush occurred event. */
#define HCI_EVENT_CODE_ROLE_CHANGE                                              0x12  /**< Role change event. */
#define HCI_EVENT_CODE_NUMBER_OF_COMPLETE_PACKETS                               0x13  /**< Number of complete packets event. */
#define HCI_EVENT_CODE_MODE_CHANGE                                              0x14  /**< Mode change event. */
#define HCI_EVENT_CODE_RETURN_LINK_KEYS                                         0x15  /**< Return link keys event. */
#define HCI_EVENT_CODE_PIN_CODE_REQUEST                                         0x16  /**< Pin code request event. */
#define HCI_EVENT_CODE_LINK_KEY_REQUEST                                         0x17  /**< Link key request event. */
#define HCI_EVENT_CODE_LINK_NOTIFICATION                                        0x18  /**< Link notification event. */
#define HCI_EVENT_CODE_LOOPBACK_COMMAND                                         0x19  /**< Loopback command event. */
#define HCI_EVENT_CODE_DATA_BUFFER_OVERFLOW                                     0x1A  /**< Data buffer overflow event. */
#define HCI_EVENT_CODE_MAX_SLOTS_CHANGE                                         0x1B  /**< Max slots change event. */
#define HCI_EVENT_CODE_READ_CLOCK_OFFSET_COMPLETE                               0x1C  /**< Read clock offset complete event. */
#define HCI_EVENT_CODE_CONNECTION_PACKET_TYPE_CHANGED                           0x1D  /**< Connection packet type change event. */
#define HCI_EVENT_CODE_QOS_VIOLATION                                            0x1E  /**< QoS violation event. */
#define HCI_EVENT_CODE_PAGE_SCAN_REPETITION_MODE_CHANGE                         0x20  /**< Page scan repetition mode change event. */
#define HCI_EVENT_CODE_FLOW_SPECIFICATION_COMPLETE                              0x21  /**< Flow specification complete event. */
#define HCI_EVENT_CODE_INQUIRY_RESULT_WITH_RSSI                                 0x22  /**< Inquiry result with RSSI event. */
#define HCI_EVENT_CODE_READ_REMOTE_EXTENDED_FEATURES_COMPLETE                   0x23  /**< Read remote extended features complete event. */
#define HCI_EVENT_CODE_SYNCHRONOUS_CONNECTION_COMPLETE                          0x2C  /**< Synchromous connection complete event. */
#define HCI_EVENT_CODE_SYNCHRONOUS_CONNECTION_CHANGED                           0x2D  /**< Synchronous connection changed event. */
#define HCI_EVENT_CODE_SNIFF_SUBRATING                                          0x2E  /**< Sniff subrating event. */
#define HCI_EVENT_CODE_EXTENDED_INQUIRY_RESULT                                  0x2F  /**< Extended inquiry result event. */
#define HCI_EVENT_CODE_ENCRYPTION_KEY_REFRESH                                   0x30  /**< Encryption key refresh event. */
#define HCI_EVENT_CODE_IO_CAPABILITY_REQUEST                                    0x31  /**< IO capability request event. */
#define HCI_EVENT_CODE_IO_CAPABILITY_RESPONSE                                   0x32  /**< IO capability response event. */
#define HCI_EVENT_CODE_USER_CONFIRMATION_REQUEST                                0x33  /**< User confirmation request event. */
#define HCI_EVENT_CODE_USER_PASSKEY_REQUEST                                     0x34  /**< User passkey request event. */
#define HCI_EVENT_CODE_REMOTE_OOB_DATA_REQUEST                                  0x35  /**< Remote OOB data request event. */
#define HCI_EVENT_CODE_SIMPLE_PAIRING_COMPLETE                                  0x36  /**< Simple pairing complete event. */
#define HCI_EVENT_CODE_SUPERVISION_TIMEOUT_CHANGED                              0x38  /**< Supervision timeout changed event. */
#define HCI_EVENT_CODE_ENHANCED_FLUSH_COMPLETE                                  0x39  /**< Enhanced flush complete event. */
#define HCI_EVENT_CODE_USER_PASSKEY_NOTIFICATION                                0x3B  /**< User passkry notification event. */
#define HCI_EVENT_CODE_KEYPRESS_NOTIFICATION                                    0x3C  /**< Keypress notification event. */
#define HCI_EVENT_CODE_REMOTE_HOST_SUPPORTED_FEATURES_NOTIFICATION              0x3D  /**< Remote host supported features notification event. */
#define HCI_EVENT_CODE_LE_META                                                  0x3E  /**< LE meta event. */
#define HCI_EVENT_CODE_NUMBER_OF_COMPLETE_DATA_BLOCKS                           0x48  /**< Number of complete data blocks event. */
#define HCI_EVENT_CODE_TRIGGER_CLOCK_CAPTURE                                    0x4E  /**< Trigger clock capture event. */
#define HCI_EVENT_CODE_SYNCHRONIZATION_TRAIN_COMPLETE                           0x4F  /**< Synchronization train complete event. */
#define HCI_EVENT_CODE_SYNCHRONIZATION_TRAIN_RECEIVED                           0x50  /**< Synchronization train received event. */
#define HCI_EVENT_CODE_CONNECTIONLESS_PERIPHERAL_BROADCAST_RECEIVE              0x51  /**< Connectionless peripheral broadcast receive event. */
#define HCI_EVENT_CODE_CONNECTIONLESS_PERIPHERAL_BROADCAST_TIMEOUT              0x52  /**< Connectionless peripheral broadcast timeout event. */
#define HCI_EVENT_CODE_TRUNCATED_PAGE_COMPLETE                                  0x53  /**< Truncated page complete event. */
#define HCI_EVENT_CODE_PERIPHERAL_PAGE_RESPONSE_TIMEOUT                         0x54  /**< Peripheral page response timeout event. */
#define HCI_EVENT_CODE_CONNECTIONLESS_PERIPHERAL_BROADCAST_CHANNEL_MAP_CHANGE   0x55  /**< Connectionless peripheral broadcast channel map change event. */
#define HCI_EVENT_CODE_INQUIRY_RESPONSE_NOTIFICATION                            0x56  /**< Inquiry response notification event. */
#define HCI_EVENT_CODE_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED                    0x57  /**< Authenticated payload timeout expired event. */
#define HCI_EVENT_CODE_SAM_STATUS_CHANGE                                        0x58  /**< SAM status change event. */
#define HCI_EVENT_CODE_VENDOR_EVENT                                             0xFF  /**< vendor event. */
/**************************************************
  HCI LE Sub Event Code
 *************************************************/
#define HCI_EVENT_CODE_LE_CONNECTION_COMPLETE                                   0x01  /**< LE connection complete sub-event. */
#define HCI_EVENT_CODE_LE_ADVERTISING_REPORT                                    0x02  /**< LE advertising report sub-event. */
#define HCI_EVENT_CODE_LE_CONNECTION_UPDATE_COMPLETE                            0x03  /**< LE connection update complete sub-event. */
#define HCI_EVENT_CODE_LE_READ_REMOTE_EXTENDED_FEATURES_COMPLETE                0x04  /**< LE read remote extended features complete sub-event. */
#define HCI_EVENT_CODE_LE_LONG_TERM_KEY_REQUEST                                 0x05  /**< LE long term key request sub-event. */
#define HCI_EVENT_CODE_LE_REMOETE_CONNECTION_PARAMETE_REQUEST                   0x06  /**< LE remote connection parameter request sub-event. */
#define HCI_EVENT_CODE_LE_DATA_LENGTH_CHANGE                                    0x07  /**< LE data length change sub-event. */
#define HCI_EVENT_CODE_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE                   0x08  /**< LE read local P-256 public key complete sub-event. */
#define HCI_EVENT_CODE_LE_GENERATE_DHKEY_COMPLETE                               0x09  /**< LE generate DHKey complete  sub-event. */
#define HCI_EVENT_CODE_LE_ENHANCED_CONNECTION_COMPLETE                          0x0A  /**< LE enhanced connection complete sub-event. */
#define HCI_EVENT_CODE_LE_DIRECTED_ADVERTISING_REPORT                           0x0B  /**< LE directed advertising report sub-event. */
#define HCI_EVENT_CODE_LE_PHY_UPDATE_COMPLETE                                   0x0C  /**< LE PHY update complete sub-event. */
#define HCI_EVENT_CODE_LE_EXTENDED_ADVERTISING_REPORT                           0x0D  /**< LE extended advertising report sub-event. */
#define HCI_EVENT_CODE_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED                 0x0E  /**< LE periodic advertising sync established sub-event. */
#define HCI_EVENT_CODE_LE_PERIODIC_ADVERTISING_REPORT                           0x0F  /**< LE periodic advertising report sub-event. */
#define HCI_EVENT_CODE_LE_PERIODIC_SYNC_LOST                                    0x10  /**< LE periodic sync lost sub-event. */
#define HCI_EVENT_CODE_LE_SCAN_TIMEOUT                                          0x11  /**< LE scan timeout sub-event. */
#define HCI_EVENT_CODE_LE_ADVERTISING_SET_TERMINATED                            0x12  /**< LE advertising set terminated sub-event. */
#define HCI_EVENT_CODE_LE_SCAN_REQUEST_RECEIVED                                 0x13  /**< LE scan request received sub-event. */
#define HCI_EVENT_CODE_LE_CHANNEL_SELECTION_ALGORITHM                           0x14  /**< LE channel selection algorithm sub-event. */
#define HCI_EVENT_CODE_LE_CONNECTIONLESS_IQ_REPORT                              0x15  /**< LE connectionless IQ report sub-event. */
#define HCI_EVENT_CODE_LE_CONNECTION_IO_REPORT                                  0x16  /**< LE connection IO report sub-event. */
#define HCI_EVENT_CODE_LE_CTE_REQUEST_FAILED                                    0x17  /**< LE CTE request failed sub-event. */
#define HCI_EVENT_CODE_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED           0x18  /**< LE periodic advertising sync transfer received sub-event. */
#define HCI_EVENT_CODE_LE_CIS_ESTABLISHED                                       0x19  /**< LE CIS established sub-event. */
#define HCI_EVENT_CODE_LE_CIS_REQUEST                                           0x1A  /**< LE CIS request sub-event. */
#define HCI_EVENT_CODE_LE_CREATE_BIG_COMPLETE                                   0x1B  /**< LE create BIG complete sub-event. */
#define HCI_EVENT_CODE_LE_TERMINATE_BIG_COMPLETE                                0x1C  /**< LE terminate BIG complete sub-event. */
#define HCI_EVNET_CODE_LE_BIG_SYNC_ESTABLISHED                                  0x1D  /**< LE BIG sync established sub-event. */
#define HCI_EVENT_CODE_LE_BIG_SYNC_LOST                                         0x1E  /**< LE BIG sync lost sub-event. */
#define HCI_EVENT_CODE_LE_REQUEST_PEER_SCA_COMPLETE                             0x1F  /**< LE request peer SCA complete sub-event. */
#define HCI_EVENT_CODE_LE_PATH_LOSS_THRESHOLD                                   0x20  /**< LE path loss threshold sub-event. */
#define HCI_EVENT_CODE_LE_TRANSMIT_POWER_REPORTING                              0x21  /**< LE transmit power reporting sub-event. */
#define HCI_EVENT_CODE_LE_BIGINFO_ADVERTISING_REPORT                            0x22  /**< LE BIGInfo advertising report sub-event. */
#define HCI_EVENT_CODE_LE_SUBRATE_CHANGE                                        0x23  /**< LE subrate change sub-event. */


#define HCI_SUBEVENT_VENDOR_SCAN_REQUEST_REPORT                                 0x01  /**< Vendor sub event : scan request report. */
/** @} */

/**************************************************************************************************
 * EXTERN DEFINITIONS
 *************************************************************************************************/

/** @brief  BLE host HCI event handler.
 * @ingroup hci_event
 */
extern int8_t (* const host_prcss_hci_event[])(uint8_t *p_data, uint8_t length);
extern int8_t prcss_hci_vendor_event(uint8_t *p_data, uint8_t length);

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/**
 * @defgroup ble_hci_evt_events BLE HCI Event Parameters
 * @{
 * @ingroup hci_event
 * @details Here shows the structures of BLE HCI events parameters.
 * @}
 */

/**
 * @defgroup hci_evt_disconn_complete Disconnection Complete Event.
 * @{
 * @ingroup ble_hci_evt_events
 * @detail event_code = 0x05
 */
typedef struct __attribute__((packed)) ble_hci_evt_param_disconn_complete_s
{
    uint8_t status;                       /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t conn_handle;                 /**< Connection handle. */
    uint8_t reason;                       /**< Reason for disconnection, @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_evt_param_disconn_complete_t;
/** @} */


/**
 * @defgroup hci_evt_read_remote_ver_info Command Complete Event
 * @{
 * @ingroup ble_hci_evt_events
 * @detail event_code = 0x0C
 */
typedef struct __attribute__((packed)) ble_hci_evt_param_read_remote_ver_info_s
{
    uint8_t status;                       /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t conn_handle;                 /**< Connection handle. */
    uint8_t version;                      /**< Version of the Current LMP or link layer supported by the remote controller. */
    uint16_t company_identifier;          /**< Company identifier for manufacturer of the remote controller. */
    uint16_t subversion;                  /**< Revision of the LMP or link layer implentation in the remote controller. */
} ble_hci_evt_param_read_remote_ver_info_t;
/** @} */


/**
 * @defgroup hci_evt_encry_change Encryption Change Event.
 * @{
 * @ingroup ble_hci_evt_events
 * @detail event_cdoe = 0x08
*/
typedef struct __attribute__((packed)) ble_hci_evt_param_encrypt_change_s
{
    uint8_t  status;                      /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t conn_handle;                 /**< Connection handle. */
    uint8_t  encrypt_enabled;             /**< Link Level encryption is enabled or not. */
} ble_hci_evt_param_encrypt_change_t;
/** @} */


/**
 * @defgroup hci_evt_key_refresh Encryption Key Refresh Complete Event.
 * @{
 * @ingroup ble_hci_evt_events
 * @detail event_cdoe = 0x30
*/
typedef struct __attribute__((packed)) ble_hci_evt_param_key_refresh_complete_s
{
    uint8_t  Status;                        /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t conn_handle;                   /**< Connection handle. */
} ble_hci_evt_param_key_refresh_complete_t;
/** @} */


/**
 * @defgroup hci_evt_cmd_status Command Status Event.
 * @{
 * @ingroup ble_hci_evt_events
 * @dtail event_code = 0x0F
*/
typedef struct __attribute__((packed)) ble_hci_evt_param_cmd_status_s
{
    uint8_t status;                     /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t hci_num_cmd_pckts;          /**< The Number of HCI Command packets which are allowed to be sent to the Controller from the Host. */
    uint8_t ocf;                        /**< Opcode command field. */
    uint8_t ogf;                        /**< Opcode group field. */
} ble_hci_evt_param_cmd_status_t;
/** @} */

/**
 * @defgroup hci_evt_num_complete Number Of Completed Packets Event.
 * @{
 * @ingroup ble_hci_evt_events
 * @dtail event_code = 0x13
*/
typedef struct  __attribute__((packed)) ble_hci_evt_param_num_of_complete_packets_s
{
    uint8_t num_of_handle;              /**< The number of Connection_Handles and Num_HCI_Data_Packets parameters pairs contained in this event. */
    uint16_t conn_handle;               /**< Connection handle. */
    uint16_t num_cmplt_pkt;             /**< The number of HCI Data packets that have been completed for the associated Connection_Handle since the previous time the event was returned. */
} ble_hci_evt_param_num_of_complete_packets_t;
/** @} */



/**
 * @defgroup ble_hci_evt_info_return_param BLE HCI Information Parameter Command Return Parameters
 * @{
 * @ingroup hci_event
 * @details Here shows the structures of BLE HCI information parameter command return parameters.
 * @}
*/
/**
 * @defgroup ble_hci_evt_info_return_read_ver Read Local Version Information Command.
 * @{
 * @ingroup ble_hci_evt_info_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_read_local_ver_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t     hci_version;            /**< HCI version. */
    uint16_t    hci_revision;           /**< Revision of the Current HCI in the BR/EDR Controller. */
    uint8_t     lmp_pal_version;        /**< Version of the Current LMP or PAL in the Controller. */
    uint16_t    manufacturer_name;      /**< Manufacturer Name of the BR/EDR Controller. */
    uint16_t    lmp_pal_subversion;     /**< Subversion of the Current LMP or PAL in the Controller. This value is implementation dependent. */
} ble_hci_return_param_read_local_ver_t;
/** @} */


/**
 * @defgroup ble_hci_evt_info_return_read_local_supported_cmds Read Local Supported Commands Command.
 * @{
 * @ingroup ble_hci_evt_info_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_read_local_supported_cmds_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t     supported_commands[64]; /**< Bit mask for each HCI command. */
} ble_hci_return_param_read_local_supported_cmds_t;
/** @} */


/**
 * @defgroup ble_hci_evt_info_return_read_local_supported_features Read Local Supported Features Command.
 * @{
 * @ingroup ble_hci_evt_info_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_read_local_supported_features_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t     lmp_features[8];        /**< Bit mask List of LMP features. */
} ble_hci_return_param_read_local_supported_features_t;
/** @} */


/**
 * @defgroup ble_hci_evt_info_return_read_bd_addr Read Local Supported Features Command.
 * @{
 * @ingroup ble_hci_evt_info_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_read_bd_addr_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t     bd_addr[6];             /**< BD_ADDR of the device. */
} ble_hci_return_param_read_bd_addr_t;
/** @} */



/**
 * @defgroup ble_hci_evt_status_param_return_param BLE HCI Status Parameters Return Parameters
 * @{
 * @ingroup hci_event
 * @details Here shows the structures of BLE HCI status parameters command return parameters.
 * @}
*/
/**
 * @defgroup ble_hci_evt_status_param_read_rssi Read RSSI command
 * @{
 * @ingroup ble_hci_evt_status_param_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_read_rssi_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t    handle;                 /**< Status. */
    uint8_t     rssi;                   /**< RSSI. */
} ble_hci_return_param_read_rssi_t;
/** @} */


/**
 * @defgroup ble_hci_evt_return_param BLE HCI LE Controller Command Return Parameters
 * @{
 * @ingroup hci_event
 * @details Here shows the structures of BLE HCI command return parameters.
 * @}
*/
/**
 * @defgroup hci_evt_return_set_evt_mask LE Set Event Mask Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_set_event_mask_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_set_event_mask_t;
/** @} */


/**
 * @defgroup hci_evt_return_read_buffer LE Read Buffer Size Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_read_buffer_size_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t    data_packet_length;     /**< Data packet length. */
    uint8_t     total_num_data_packet;  /**< Total number of data packets. */
} ble_hci_return_param_read_buffer_size_t;
/** @} */


/**
 * @defgroup hci_evt_return_le_read_local_supported_features LE Read local supported features Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_le_read_local_supported_features_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t     le_features[8];         /**< Bit mask list of supported LE features. */
} ble_hci_return_param_le_read_local_supported_features_t;
/** @} */


/**
 * @defgroup hci_evt_return_set_rand_addr LE Set Random Address Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct  __attribute__((packed)) ble_hci_return_param_set_random_addr_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_set_random_addr_t;
/** @} */


/**
 * @defgroup hci_evt_return_set_adv_param LE Set Advertising Parameters Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_set_adv_param_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_set_adv_param_t;
/** @} */


/**
 * @defgroup hci_evt_return_set_adv_data LE Set Advertising Data Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_set_adv_data_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_set_adv_data_t;
/** @} */


/**
 * @defgroup hci_evt_return_set_scan_rsp LE Set Scan Response Data Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_set_scan_rsp_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_set_scan_rsp_t;
/** @} */


/**
 * @defgroup hci_evt_return_set_adv_enable  LE Set Advertising Enable Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_set_adv_enable_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_set_adv_enable_t;
/** @} */


/**
 * @defgroup hci_evt_return_set_scan_param LE Set Scan Parameters Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_set_scan_param_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_set_scan_param_t;
/** @} */


/**
 * @defgroup hci_evt_return_set_scan_enable LE Set Scan Enable Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_set_scan_enable_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_set_scan_enable_t;
/** @} */


/**
 * @defgroup hci_evt_return_create_conn LE Create Connection cancel command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_create_conn_cancel_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_create_conn_cancel_t;
/** @} */


/**
 * @defgroup hci_evt_return_accept_list_size LE Read Filter accept list size command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_accept_list_size_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t     accept_list_size;       /**< Filter accept list size. */
} ble_hci_return_param_accept_list_size_t;
/** @} */


/**
 * @defgroup hci_evt_return_clear_accept_list LE Clear Filter accept list command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_clear_accept_list_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_clear_accept_list_t;
/** @} */


/**
 * @defgroup hci_evt_return_add_accept_list LE Add Filter accept list command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_add_accept_list_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_add_accept_list_t;
/** @} */


/**
 * @defgroup hci_evt_return_remove_accept_list LE Remove Filter accept list command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_remove_accept_list_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_remove_accept_list_t;
/** @} */



/**
 * @defgroup hci_evt_return_encrypt LE Encrypt Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_encrypt_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t     encrypted_data[16];     /**< 128 bit encrypted data block. \n
                                           The most significant octet of the Encrypted_Data corresponds to \n
                                           out[0] using the notation specified in FIPS 197.*/
} ble_hci_return_param_encrypt_t;
/** @} */


/**
 * @defgroup hci_evt_return_rand LE Set Rand Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct  __attribute__((packed)) ble_hci_return_param_rand_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t     random_number[8];       /**< Random number. */
} ble_hci_return_param_rand_t;
/** @} */


/**
 * @defgroup hci_evt_return_long_term_key_req_reply LE Set Long Term Key Request Reply Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct  __attribute__((packed)) ble_hci_return_param_set_long_term_key_req_reply_s
{
    uint8_t status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t conn_handle;           /**< Connection handle. */
} ble_hci_return_param_set_long_term_key_req_reply_t;
/** @} */


/**
 * @defgroup hci_evt_return_long_term_key_req_neg_reply LE Set Long Term Key Request Negative Reply Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct  __attribute__((packed)) ble_hci_return_param_set_long_term_key_req_neg_reply_s
{
    uint8_t status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t conn_handle;           /**< Connection handle. */
} ble_hci_return_param_set_long_term_key_req_neg_reply_t;
/** @} */


/**
 * @defgroup hci_evt_return_read_supported_state LE Read Supported States Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct  __attribute__((packed)) ble_hci_return_param_read_supported_states_s
{
    uint8_t status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t le_states[8];           /**< LE supported states. */
} ble_hci_return_param_read_supported_states_t;
/** @} */


/**
 * @defgroup hci_evt_return_remote_conn_param_req_reply LE Remote Connection Parameter Request Reply Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct  __attribute__((packed)) ble_hci_return_param_remote_conn_param_req_reply_s
{
    uint8_t status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t conn_handle;           /**< Connection handle. */
} ble_hci_return_param_remote_conn_param_req_reply_t;
/** @} */


/**
 * @defgroup hci_evt_return_remote_conn_param_req_neg_reply LE Remote Connection Parameter Request Negative Reply Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct  __attribute__((packed)) ble_hci_return_param_remote_conn_param_req_neg_reply_s
{
    uint8_t status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t conn_handle;           /**< Connection handle. */
} ble_hci_return_param_remote_conn_param_req_neg_reply_t;
/** @} */


/**
 * @defgroup hci_evt_return_set_data_length LE Set Data Length Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct  __attribute__((packed)) ble_hci_return_param_set_data_length_s
{
    uint8_t status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t conn_handle;           /**< Connection handle. */
} ble_hci_return_param_set_data_length_t;
/** @} */


/**
 * @defgroup hci_evt_return_read_suggested_default_data_length LE Read Suggested Default Data Length Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct  __attribute__((packed)) ble_hci_return_param_read_suggested_default_data_length_s
{
    uint8_t status;                     /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t suggested_max_tx_octets;   /**< The Host's suggested value for the controller's maximum transmitted number of payload octets. */
    uint16_t suggested_max_tx_times;    /**< The Host's suggested value for the controller's maximum packet transmission time for packets. */
} ble_hci_return_param_read_suggested_default_data_length_t;
/** @} */


/**
 * @defgroup hci_evt_return_write_suggested_default_data_length LE Write Suggested Default Data Length Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct  __attribute__((packed)) ble_hci_return_param_write_suggested_default_data_length_s
{
    uint8_t status;                     /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_write_suggested_default_data_length_t;
/** @} */


/**
 * @defgroup hci_evt_return_read_maximum_data_length LE Read Maximum Data Length Command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct  __attribute__((packed)) ble_hci_return_param_read_maximum_data_length_s
{
    uint8_t status;                     /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t suggested_max_tx_octets;   /**< Maximum number of payload octets that the local controller supports for transmission. */
    uint16_t suggested_max_tx_times;    /**< Maximum time. */
    uint16_t suggested_max_rx_octets;   /**< Maximum number of payload octets that the local controller supports for reception of a link. */
    uint16_t suggested_max_rx_times;    /**< Maximum time. */
} ble_hci_return_param_read_maximum_data_length_t;
/** @} */


/**
 * @defgroup hci_evt_return_read_phy LE Read PHY command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_read_phy_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t    conn_handle;            /**< Connection handle. */
    uint8_t     tx_phy;                 /**< TX PHY. */
    uint8_t     rx_phy;                 /**< RX PHY. */
} ble_hci_return_param_read_phy_t;
/** @} */


/**
 * @defgroup hci_evt_return_set_default_phy LE Set Default PHY command.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_set_default_phy_s
{
    uint8_t     status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_hci_return_param_set_default_phy_t;
/** @} */



/**
 * @defgroup ble_hci_meta_evt_param BLE HCI Meta Event Parameters
 * @{
 * @ingroup hci_event
 * @details Here shows the structures of BLE HCI Meta event parameters.
 * @}
*/
/**
 * @defgroup hci_evt_meta_adv_report LE Advertising Report Event.
 * @{
 * @ingroup ble_hci_meta_evt_param
*/
typedef struct  __attribute__((packed)) ble_hci_le_meta_evt_param_adv_report_s
{
    uint8_t   num_reports;                  /**< Number of responses in event. */
    uint8_t   event_type0;                  /**< Event type, @ref ble_adv_type "ble_adv_type_t". */
    uint8_t   addr_type0;                   /**< Address type. */
    uint8_t   addr0[BLE_ADDR_LEN];          /**< Address. */
    uint8_t   length0;                      /**< Data length. */
    uint8_t   data0[31 + 1];//+1: uRSSI0;   /**< Data. */
    //uint8_t   RSSI0;                      /**< RSSI. */
} ble_hci_le_meta_evt_param_adv_report_t;
/** @} */


/**
 * @defgroup hci_evt_meta_directed_adv_report LE direct Advertising Report Event.
 * @{
 * @ingroup ble_hci_meta_evt_param
*/
typedef struct  __attribute__((packed)) ble_hci_le_meta_evt_param_directed_adv_report_s
{
    uint8_t   num_reports;                  /**< Number of responses in event. */
    uint8_t   event_type0;                  /**< Event type, @ref ble_adv_type "ble_adv_type_t". */
    uint8_t   addr_type0;                   /**< Address type. */
    uint8_t   addr0[BLE_ADDR_LEN];          /**< Address. */
    uint8_t   direct_addr_type;             /**< Direct Addr type. */
    uint8_t   direct_addr;                  /**< Direct Address. */
} ble_hci_le_meta_evt_param_directed_adv_report_t;
/** @} */


/**
 * @defgroup hci_evt_meta_conn_complete LE Connection Complete Event.
 * @{
 * @ingroup ble_hci_meta_evt_param
*/
typedef struct __attribute__((packed)) ble_hci_le_meta_evt_param_conn_complete_s
{
    uint8_t   status;                       /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t  conn_handle;                  /**< Connection handle. */
    uint8_t   role;                         /**< @ref ble_gap_role "ble_gap_role_t". */
    uint8_t   peer_addr_type;               /**< Peer address type. */
    uint8_t   peer_addr[BLE_ADDR_LEN];      /**< Peer address. */
    uint16_t  conn_interval;                /**< Connection interval used on this connection. */
    uint16_t  periph_latency;               /**< Peripheral latency for the connection in number of connection events. */
    uint16_t  supv_timeout;                 /**< Connection supervision timeout. */
    uint8_t   master_clk_accuracy;          /**< Clock accuracy. */
} ble_hci_le_meta_evt_param_conn_complete_t;
/** @} */


/**
 * @defgroup hci_evt_meta_conn_update LE Connection Update Complete Event.
 * @{
 * @ingroup ble_hci_meta_evt_param
*/
typedef struct __attribute__((packed)) ble_hci_le_meta_evt_param_conn_update_s
{
    uint8_t   status;                     /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t  conn_handle;                /**< Connection handle. */
    uint16_t  conn_interval;              /**< Connection interval used on this connection. */
    uint16_t  periph_latency;             /**< Peripheral latency for the connection in number of subrated connection events. */
    uint16_t  supv_timeout;               /**< Supervision timeout for this connection. */
} ble_hci_le_meta_evt_param_conn_update_t;
/** @} */


/**
 * @defgroup hci_evt_meta_conn_update LE Connection Update Complete Event.
 * @{
 * @ingroup ble_hci_meta_evt_param
*/
typedef struct __attribute__((packed)) ble_hci_le_meta_evt_param_conn_para_req_s
{
    uint16_t  conn_handle;               /**< Connection handle. */
    uint16_t  min_interval;              /**< Connection interval used on this connection. */
    uint16_t  max_interval;              /**< Connection interval used on this connection. */
    uint16_t  max_latency;               /**< Peripheral latency for the connection in number of subrated connection events. */
    uint16_t  timeout;                   /**< Supervision timeout for this connection. */
} ble_hci_le_meta_evt_param_conn_para_req_t;
/** @} */



/**
 * @defgroup hci_evt_meta_read_remote_features LE Read Remote Features Event.
 * @{
 * @ingroup ble_hci_meta_evt_param
*/
typedef struct __attribute__((packed)) ble_hci_le_meta_evt_param_read_remote_features_s
{
    uint8_t   status;                     /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t  conn_handle;                /**< Connection handle. */
    uint16_t  le_features;                /**< Bit mask list of LE features. */
} ble_hci_le_meta_evt_param_read_remote_features_t;
/** @} */


/**
 * @defgroup hci_evt_meta_data_len_change LE Data Length Change Event.
 * @{
 * @ingroup ble_hci_meta_evt_param
*/
typedef struct __attribute__((packed)) ble_hci__le_meta_evt_param_data_length_change_s
{
    uint16_t  conn_handle;                /**< Connection handle. */
    uint16_t  max_tx_octets;              /**< The maximum number of payload octets in a LLData PDU. */
    uint16_t  max_tx_time;                /**< The maximum time that the local Controller will take to send a Link Layer packet containing an LL Data PDU on this connection. */
    uint16_t  max_rx_octets;              /**< The maximum number of payload octets in a Link Layer packet that the local Controller expects to receive on this connection. */
    uint16_t  max_rx_time;                /**< The maximum time that the local Controller expects to take to receive aLink Layer packet on this connection. */
} ble_hci_le_meta_evt_param_data_length_change_t;
/** @} */


/**
 * @defgroup hci_evt_meta_phy_update LE PHY Update Complete Event.
 * @{
 * @ingroup ble_hci_meta_evt_param
*/
typedef struct __attribute__((packed)) ble_hci_le_meta_evt_param_phy_update_complete_s
{
    uint8_t   status;                     /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t  conn_handle;                /**< Connection handle. */
    uint8_t   tx_phy;                     /**< TX PHY. */
    uint8_t   rx_phy;                     /**< RX PHY. */
} ble_hci_le_meta_evt_param_phy_update_complete_t;
/** @} */


/**
 * @defgroup hci_evt_meta_key_req LE Long Term Key Request Event.
 * @{
 * @ingroup ble_hci_meta_evt_param
*/
typedef struct __attribute__((packed)) ble_hci_le_meta_evt_param_long_term_key_req_s
{
    uint16_t  conn_handle;                /**< Connection handle. */
    uint8_t   random_num[8];              /**< 64-bit random number. */
    uint8_t   encry_div[2];               /**< 16-bit encrypted diversifier.. */
} ble_hci_le_meta_evt_param_long_term_key_req_t;
/** @} */


/**
 * @defgroup hci_evt_return_read_ch_map LE Read Channel Map Event.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_read_ch_map_s
{
    uint8_t   status;                     /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t  conn_handle;                /**< Connection handle. */
    uint8_t   channel_map[5];             /**< Channel map value. */
} ble_hci_return_param_read_ch_map_t;
/** @} */


/**
 * @defgroup hci_evt_return_set_ctrl_info LE Read Channel Map Event.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed))
{
    uint8_t             status;                        /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
}
ble_hci_return_param_set_ctrl_info_t;
/** @} */


/**
 * @defgroup hci_evt_return_param_cte_rx LE Set connection CTE receive parameters Event.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_cte_rx_s
{
    uint8_t   status;                     /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t  conn_handle;                /**< Connection handle. */
} ble_hci_return_param_cte_rx_t;
/** @} */


/**
 * @defgroup hci_evt_return_param_cte_tx LE Set connection CTE transmit parameters Event.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_cte_tx_s
{
    uint8_t   status;                     /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t  conn_handle;                /**< Connection handle. */
} ble_hci_return_param_cte_tx_t;
/** @} */


/**
 * @defgroup hci_evt_return_param_cte_req LE Set connection CTE request enable Event.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_cte_req_s
{
    uint8_t   status;                     /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t  conn_handle;                /**< Connection handle. */
} ble_hci_return_param_cte_req_t;


/**
 * @defgroup hci_evt_return_param_cte_rsp LE Set connection CTE response enable Event.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed)) ble_hci_return_param_cte_rsp_s
{
    uint8_t   status;                     /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t  conn_handle;                /**< Connection handle. */
} ble_hci_return_param_cte_rsp_t;


/**
 * @defgroup hci_evt_return_set_ctrl_info LE Read Channel Map Event.
 * @{
 * @ingroup ble_hci_evt_return_param
*/
typedef struct __attribute__((packed))
{
    uint8_t             status;                        /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t             supported_rates;               /**< Supported switching sampling rates. */
    uint8_t             num_antennae;                  /**< The number of antennae supported. */
    uint8_t             max_pattern_length;            /**< Maximum length of antenna switching pattern supported. */
    uint8_t             max_cte_length;                /**< Maximum length of a transmitted constant tone extension supported in 8us uints. */
}
ble_hci_return_param_read_antenna_info_t;
/** @} */


/**
 * @defgroup hci_evt_meta_conn_iq_report LE Connection IQ Report Event.
 * @{
 * @ingroup ble_hci_meta_evt_param
*/
typedef struct __attribute__((packed)) ble_hci_le_meta_evt_param_conn_iq_report_s
{
    uint16_t    conn_handle;                /**< Connection handle. */
    uint8_t     rx_phy;                     /**< Receiver PHY. */
    uint8_t     data_ch_idx;                /**< The index of the data channel. */
    int16_t     rssi;                       /**< RSSI of packet. */
    uint8_t     rssi_antenna_id;            /**< ID of the antenna on which the rssi. */
    uint8_t     cte_type;                   /**< CTE Type. */
    uint8_t     slot_durations;             /**< Switching and sampling slots. */
    uint8_t     packet_status;              /**< Packet status. */
    uint16_t    conn_evt_cnt;               /**< the value of connection event counter. */
    uint8_t     sample_cnt;                 /**< Sample counter. */
    uint8_t     sample_iq[];                /**< pointer of iq value. */
} ble_hci_le_meta_evt_param_conn_iq_report_t;
/** @} */


/**
 * @defgroup hci_evt_meta_conn_iq_report LE CTE Request failed Event.
 * @{
 * @ingroup ble_hci_meta_evt_param
*/
typedef struct __attribute__((packed)) ble_hci_le_meta_evt_param_cte_req_failed_s
{
    uint8_t   status;                     /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t  conn_handle;                /**< Connection handle. */
} ble_hci_le_meta_evt_param_cte_req_failed_t;
/** @} */


/**
 * @defgroup hci_evt_vendor Vendor Event.
 * @{
 * @ingroup ble_hci_evt_events
 * @detail event_cdoe = 0xFF
*/
typedef struct __attribute__((packed)) ble_hci_evt_param_vendor_s
{
    uint8_t  subevent_code;
    uint8_t  num_reports;
    uint8_t  event_type;
    uint8_t  addr_type;                   /**< Address type. */
    uint8_t  addr[BLE_ADDR_LEN];          /**< Address. */
    int8_t   rssi;
} ble_hci_evt_param_vendor_t;
/** @} */


/**
 * @defgroup hci_evt_cmd_complete Command Complete Event
 * @{
 * @ingroup ble_hci_evt_events
 * @detaul event_cdoe = 0x0E
 */
typedef struct __attribute__((packed))
{
    uint8_t hci_num_cmd_pckts;        /**< The Number of HCI Command packets which are allowed to be sent to the Controller from the Host.*/
    uint8_t ocf;                      /**< Opcode command field. */
    uint8_t ogf;                      /**< Opcode group field. */
    __packed union
    {
        ble_hci_return_param_read_local_ver_t                       read_local_ver;
        ble_hci_return_param_read_rssi_t                            read_rssi;
        ble_hci_return_param_set_event_mask_t                       set_event_mask;
        ble_hci_return_param_read_buffer_size_t                     read_buffer_size;
        ble_hci_return_param_set_random_addr_t                      set_random_addr;
        ble_hci_return_param_set_adv_param_t                        set_adv_param;
        ble_hci_return_param_set_adv_data_t                         set_adv_data;
        ble_hci_return_param_set_scan_rsp_t                         set_scan_rsp;
        ble_hci_return_param_set_adv_enable_t                       set_adv_enable;
        ble_hci_return_param_set_scan_param_t                       set_scan_param;
        ble_hci_return_param_set_scan_enable_t                      set_scan_enable;
        ble_hci_return_param_create_conn_cancel_t                   create_conn_cancel;
        ble_hci_return_param_accept_list_size_t                     read_accept_list_size;
        ble_hci_return_param_clear_accept_list_t                    clear_accept_list;
        ble_hci_return_param_add_accept_list_t                      add_accept_list;
        ble_hci_return_param_remove_accept_list_t                   remove_accept_list;
        ble_hci_return_param_encrypt_t                              set_encrypt;
        ble_hci_return_param_rand_t                                 set_rand;
        ble_hci_return_param_read_phy_t                             read_phy;
        ble_hci_return_param_set_data_length_t                      set_data_length;
        ble_hci_return_param_read_suggested_default_data_length_t   read_suggested_default_data_length;
        ble_hci_return_param_write_suggested_default_data_length_t  write_suggested_default_data_length;
        ble_hci_return_param_read_ch_map_t                          read_channel_map;
        ble_hci_return_param_set_ctrl_info_t                        set_ctrl_info;
        ble_hci_return_param_cte_rx_t                               set_cte_rx_param;
        ble_hci_return_param_cte_tx_t                               set_cte_tx_param;
        ble_hci_return_param_cte_req_t                              set_cte_req;
        ble_hci_return_param_cte_rsp_t                              set_cte_rsp;
        ble_hci_return_param_read_antenna_info_t                    read_antenna_info;
    } para;
}
ble_hci_evt_param_cmd_complete_t;
/** @} */


/**
 * @defgroup hci_le_meta_evt LE Meta Event
 * @{
 * @ingroup ble_hci_evt_events
 * @detaul event_cdoe = 0x3E
 */
typedef struct __attribute__((packed))
{
    uint8_t subevent_code;
    __packed union
    {
        ble_hci_le_meta_evt_param_conn_complete_t                    conn_complete;
        ble_hci_le_meta_evt_param_conn_iq_report_t                   conn_iq_report;
        ble_hci_le_meta_evt_param_cte_req_failed_t                   cte_req_failed;
    } para;
}
ble_hci_le_meta_evt_t;
/** @} */



/** @} */
#ifdef __cplusplus
};
#endif

#endif /* __BLE_HCI_EVENT_H__*/
