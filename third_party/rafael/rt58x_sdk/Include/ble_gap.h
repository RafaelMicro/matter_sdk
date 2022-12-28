/** @file ble_gap.h
 *
 *  * @brief BLE GAP header file.
 *
 */

/**
 * @defgroup ble_gap Generic access profile
 * @ingroup BLE_group
 * @{
 * @brief Define BLE GAP definitions, structures, and functions.
 * @}
 */

#ifndef __BLE_GAP_H__
#define __BLE_GAP_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include "ble_api.h"

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define BLE_ADDR_LEN                6


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/** @defgroup ble_gap_role BLE GAP Role Definition
 * @{
 * @ingroup ble_gap
 * @details  Define GAP role.
 */
typedef uint8_t ble_gap_role_t;
#define BLE_GAP_ROLE_CENTRAL        (0x00)  /**< Central.  */
#define BLE_GAP_ROLE_PERIPHERAL     (0x01)  /**< Peripheral.  */
/** @} */


/** @defgroup ble_gap_addr_type BLE GAP Device Address Type Definition
 * @{
 * @ingroup ble_gap
 * @details Define different BLE address types.
 */
typedef uint8_t ble_addr_type_t;
#define PUBLIC_ADDR                   0x00  /**< Public device address.  */
#define RANDOM_STATIC_ADDR            0x01  /**< Random static device address.  */
#define RANDOM_NON_RESOLVABLE_ADDR    0x02  /**< Random private non-resolvable address). */
#define RANDOM_RESOLVABLE_ADDR        0x03  /**< Random private resolvable address). */
/** @} */


/** @defgroup ble_gap_peer_addr_type BLE Peer Address Type Definition
 * @{
 * @ingroup ble_gap
 * @details Define different BLE peer address types.
 */
typedef uint8_t ble_peer_addr_type_t;
#define PUBLIC_ADDR                   0x00  /**< Public device address.  */
#define RANDOM_ADDR                   0x01  /**< Random device address.  */
#define PUBLIC_IDENTITY_ADDR          0x02  /**< Public identity address. */
#define RANDOM_IDENTITY_ADDR          0x03  /**< Random (static) identity address. */
/** @} */


/** @defgroup ble_gap_adv_type BLE Gap Data Type Formats
 * @{
 * @ingroup ble_gap
 * @details  BLE GAP data type format.
 * @note https://www.bluetooth.org/en-us/specification/assigned-numbers/generic-access-profile
 */
typedef uint8_t ble_gap_ad_type_t;
#define GAP_AD_TYPE_FLAGS                                                       0x01  /**< @ref ble_gap_flag "Flags for discoverability." */
#define GAP_AD_TYPE_SERVICE_MORE_16B_UUID                                       0x02  /**< Partial list of 16 bit service UUIDs. */
#define GAP_AD_TYPE_SERVICE_CPLT_16B_UUID                                       0x03  /**< Complete list of 16 bit service UUIDs. */
#define GAP_AD_TYPE_SERVICE_MORE_32B_UUID                                       0x04  /**< Partial list of 32 bit service UUIDs. */
#define GAP_AD_TYPE_SERVICE_CPLT_32B_UUID                                       0x05  /**< Complete list of 32 bit service UUIDs. */
#define GAP_AD_TYPE_SERVICE_MORE_128B_UUID                                      0x06  /**< Partial list of 128 bit service UUIDs. */
#define GAP_AD_TYPE_SERVICE_CPLT_128B_UUID                                      0x07  /**< Complete list of 128 bit service UUIDs. */
#define GAP_AD_TYPE_LOCAL_NAME_SHORTENED                                        0x08  /**< Short local device name. */
#define GAP_AD_TYPE_LOCAL_NAME_COMPLETE                                         0x09  /**< Complete local device name. */
#define GAP_AD_TYPE_TX_POWER_LEVEL                                              0x0A  /**< Transmit power level, 1byte : 0xXX:-127 to +127dBm */
#define GAP_AD_TYPE_SIMPLE_PAIRING_OPT_OOB_CLASS_OF_DEVICE                      0x0D  /**< Class of device. */
#define GAP_AD_TYPE_SIMPLE_PAIRING_OPT_OOB_HASH_C                               0x0E  /**< Simple pairing hash C. */
#define GAP_AD_TYPE_SIMPLE_PAIRING_OPT_OOB_RANDOMIZER_R                         0x0F  /**< Simple pairing randomizer R. */
#define GAP_AD_TYPE_SECURITY_MANAGER_TK_VALUE                                   0x10  /**< Security manager TK value. */
#define GAP_AD_TYPE_SECURITY_MANAGER_OOB_FLAGS                                  0x11  /**< Security manager out of band flags. */
#define GAP_AD_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE                             0x12  /**< Slave connection interval range. */
#define GAP_AD_TYPE_SERVICE_SOLICITATION_16B_UUID                               0x14  /**< List of 16-bit service solicitation UUIDs. */
#define GAP_AD_TYPE_SERVICE_SOLICITATION_128B_UUID                              0x15  /**< List of 128-bit service solicitation UUIDs. */
#define GAP_AD_TYPE_SERVICE_DATA                                                0x16  /**< Service sata - 16-bit UUID. */
#define GAP_AD_TYPE_PUBLIC_TARGET_ADDRESS                                       0x17  /**< Public target address. */
#define GAP_AD_TYPE_RANDOM_TARGET_ADDRESS                                       0x18  /**< Random target address. */
#define GAP_AD_TYPE_APPEARANCE                                                  0x19  /**< Appearance. */
#define GAP_AD_TYPE_ADVERTISING_INTERVAL                                        0x1A  /**< Advertising interval. */
#define GAP_AD_TYPE_3D_INFORMATION_DATA                                         0x3D  /**< 3D information data. */
#define GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA                                  0xFF  /**< Manufacturer specific data. */
/** @} */


/**
 * @defgroup ble_gap_flag BLE Gap Flags Mode
 * @{
 * @ingroup ble_gap
 * @details  BLE GAP flags mode.
 * @note Bluetooth CSS. v9 page 12 of 37
 */
typedef uint8_t ble_gap_flag_t;
#define GAP_FLAGS_LIMITED_DISCOVERABLE_MODE                                     0x01  /**< LE limited discoverable mode */
#define GAP_FLAGS_GENERAL_DISCOVERABLE_MODE                                     0x02  /**< LE general discoverable mode */
#define GAP_FLAGS_BR_EDR_NOT_SUPPORTED                                          0x04  /**< BR/EDR not supported. */
#define GAP_FLAGS_SIMUL_DEV_CAP_CONTROLLER                                      0x08  /**< Simultaneous LE and BR/EDR to same device capable (Controller). */
#define GAP_FLAGS_SIMUL_DEV_CAP_HOST                                            0x10  /**< Simultaneous LE and BR/EDR to same device capable (Host). */
#define BLE_GAP_FLAGS_LIMITED_DISCOVERABLE_MODE                                 (GAP_FLAGS_LIMITED_DISCOVERABLE_MODE|GAP_FLAGS_BR_EDR_NOT_SUPPORTED)  /**< LE limited discoverable mode, BR/EDR not supported. */
#define BLE_GAP_FLAGS_GENERAL_DISCOVERABLE_MODE                                 (GAP_FLAGS_GENERAL_DISCOVERABLE_MODE|GAP_FLAGS_BR_EDR_NOT_SUPPORTED)  /**< LE general discoverable mode, BR/EDR not supported. */
/** @} */


/** @defgroup ble_gap_appearance BLE GAP Appearance values
 * @ingroup ble_gap
 * @note https://www.bluetooth.org/en-us/specification/assigned-numbers
 * @{
 */
typedef uint8_t ble_gap_appearance_t;
#define BLE_APPEARANCE_UNKNOWN                                                  0x0000  /**< Unknown. */
#define BLE_APPEARANCE_GENERIC_PHONE                                            0x0040  /**< Generic Phone. */
#define BLE_APPEARANCE_GENERIC_COMPUTER                                         0x0080  /**< Generic Computer. */
#define BLE_APPEARANCE_GENERIC_WATCH                                            0x00C0  /**< Generic Watch. */
#define BLE_APPEARANCE_WATCH_SPORTS_WATCH                                       0x00C1  /**< Watch, Sports Watch. */
#define BLE_APPEARANCE_GENERIC_CLOCK                                            0x0100  /**< Generic Clock. */
#define BLE_APPEARANCE_GENERIC_DISPLAY                                          0x0140  /**< Generic Display. */
#define BLE_APPEARANCE_GENERIC_REMOTE_CONTROL                                   0x0180  /**< Generic Remote Control. */
#define BLE_APPEARANCE_GENERIC_EYE_GLASSES                                      0x01C0  /**< Generic Eye-glasses. */
#define BLE_APPEARANCE_GENERIC_TAG                                              0x0200  /**< Generic Tag. */
#define BLE_APPEARANCE_GENERIC_KEYRING                                          0x0240  /**< Generic Keyring. */
#define BLE_APPEARANCE_GENERIC_MEDIA_PLAYER                                     0x0280  /**< Generic Media Player. */
#define BLE_APPEARANCE_GENERIC_BARCODE_SCANNER                                  0x02C0  /**< Generic Barcode Scanner. */
#define BLE_APPEARANCE_GENERIC_THERMOMETER                                      0x0300  /**< Generic Thermometer. */
#define BLE_APPEARANCE_THERMOMETER_EAR                                          0x0301  /**< Thermometer: Ear. */
#define BLE_APPEARANCE_GENERIC_HEART_RATE_SENSOR                                0x0340  /**< Generic Heart rate Sensor. */
#define BLE_APPEARANCE_HEART_RATE_SENSOR_HEART_RATE_BELT                        0x0341  /**< Heart Rate Sensor: Heart Rate Belt. */
#define BLE_APPEARANCE_GENERIC_BLOOD_PRESSURE                                   0x0380  /**< Generic Blood Pressure. */
#define BLE_APPEARANCE_BLOOD_PRESSURE_ARM                                       0x0381  /**< Blood Pressure: Arm. */
#define BLE_APPEARANCE_BLOOD_PRESSURE_WRIST                                     0x0382  /**< Blood Pressure: Wrist. */
#define BLE_APPEARANCE_GENERIC_HID                                              0x03C0  /**< Human Interface Device (HID). */
#define BLE_APPEARANCE_HID_KEYBOARD                                             0x03C1  /**< Keyboard (HID Subtype). */
#define BLE_APPEARANCE_HID_MOUSE                                                0x03C2  /**< Mouse (HID Subtype). */
#define BLE_APPEARANCE_HID_JOYSTICK                                             0x03C3  /**< Joystick (HID Subtype). */
#define BLE_APPEARANCE_HID_GAMEPAD                                              0x03C4  /**< Gamepad (HID Subtype). */
#define BLE_APPEARANCE_HID_DIGITIZERSUBTYPE                                     0x03C5  /**< Digitizer Tablet (HID Subtype). */
#define BLE_APPEARANCE_HID_CARD_READER                                          0x03C6  /**< Card Reader (HID Subtype). */
#define BLE_APPEARANCE_HID_DIGITAL_PEN                                          0x03C7  /**< Digital Pen (HID Subtype). */
#define BLE_APPEARANCE_HID_BARCODE                                              0x03C8  /**< Barcode Scanner (HID Subtype). */
#define BLE_APPEARANCE_GENERIC_GLUCOSE_METER                                    0x0400  /**< Generic Glucose Meter. */
#define BLE_APPEARANCE_GENERIC_RUNNING_WALKING_SENSOR                           0x0440  /**< Generic Running Walking Sensor. */
#define BLE_APPEARANCE_RUNNING_WALKING_SENSOR_IN_SHOE                           0x0441  /**< Running Walking Sensor: In-Shoe. */
#define BLE_APPEARANCE_RUNNING_WALKING_SENSOR_ON_SHOE                           0x0442  /**< Running Walking Sensor: On-Shoe. */
#define BLE_APPEARANCE_RUNNING_WALKING_SENSOR_ON_HIP                            0x0443  /**< Running Walking Sensor: On-Hip. */
#define BLE_APPEARANCE_GENERIC_CYCLING                                          0x0480  /**< Generic Cycling. */
#define BLE_APPEARANCE_CYCLING_CYCLING_COMPUTER                                 0x0481  /**< Cycling: Cycling Computer. */
#define BLE_APPEARANCE_CYCLING_SPEED_SENSOR                                     0x0482  /**< Cycling: Speed Sensor. */
#define BLE_APPEARANCE_CYCLING_CADENCE_SENSOR                                   0x0483  /**< Cycling: Cadence Sensor. */
#define BLE_APPEARANCE_CYCLING_POWER_SENSOR                                     0x0484  /**< Cycling: Power Sensor. */
#define BLE_APPEARANCE_CYCLING_SPEED_CADENCE_SENSOR                             0x0485  /**< Cycling: Speed and Cadence Sensor. */
#define BLE_APPEARANCE_GENERIC_PULSE_OXIMETER                                   0x0C40  /**< Generic Pulse Oximeter. */
#define BLE_APPEARANCE_PULSE_OXIMETER_FINGERTIP                                 0x0C41  /**< Fingertip (Pulse Oximeter subtype). */
#define BLE_APPEARANCE_PULSE_OXIMETER_WRIST_WORN                                0x0C42  /**< Wrist Worn(Pulse Oximeter subtype). */
#define BLE_APPEARANCE_GENERIC_WEIGHT_SCALE                                     0x0C80  /**< Generic Weight Scale. */
#define BLE_APPEARANCE_GENERIC_OUTDOOR_SPORTS_ACT                               0x1440  /**< Generic Outdoor Sports Activity. */
#define BLE_APPEARANCE_OUTDOOR_SPORTS_ACT_LOC_DISP                              0x1441  /**< Location Display Device (Outdoor Sports Activity subtype). */
#define BLE_APPEARANCE_OUTDOOR_SPORTS_ACT_LOC_AND_NAV_DISP                      0x1442  /**< Location and Navigation Display Device (Outdoor Sports Activity subtype). */
#define BLE_APPEARANCE_OUTDOOR_SPORTS_ACT_LOC_POD                               0x1443  /**< Location Pod (Outdoor Sports Activity subtype). */
#define BLE_APPEARANCE_OUTDOOR_SPORTS_ACT_LOC_AND_NAV_POD                       0x1444  /**< Location and Navigation Pod (Outdoor Sports Activity subtype). */
/** @} */


/**@defgroup ble_gap_conn_range GAP Connection Parameters Range Definitions
 * @{
 * @ingroup ble_gap
 */
typedef uint8_t ble_gap_conn_range_t;
#define BLE_CONN_INTERVAL_MIN             0x0006  /**< The unit is 1.25ms, Minimum connection interval is 7.5ms = (0x0006 * 1.25) ms. */
#define BLE_CONN_INTERVAL_MAX             0x0C80  /**< The unit is 1.25ms, Maximum connection interval is 4s = (0x0C80 * 1.25) ms. */
#define BLE_CONN_LATENCY_MIN              0       /**< Minimum connection latency is 0. */
#define BLE_CONN_LATENCY_MAX              0x01F3  /**< Maximum connection latency is 0x01F3. */
#define BLE_CONN_SUPV_TIMEOUT_MIN         0x000A  /**< The unit is 10ms, Minimum connection supervision timeout is 100ms = (0x000A * 10) ms. */
#define BLE_CONN_SUPV_TIMEOUT_MAX         0x0C80  /**< The unit is 10ms, Maximum connection supervision timeout is 32s = (0x0C80 * 10) ms. */
/** @} */


/**
 * @defgroup ble_gap_init_filter BLE Initiator Filter Policy Definition
 * @{
 * @ingroup ble_gap
 * @details BLE initiator filter policy definition.
 */
typedef uint8_t ble_initiator_filter_t;

/** Filter Accept List is not used to determine which advertiser to connect to. Peer_Address_Type and Peer_Address shall be used. */
#define INIT_FILTER_POLICY_ACCEPT_ALL             0x00

/** Filter Accept List is used to determine which advertiser to connect to. Peer_Address_Type and Peer_Address shall be ignored. */
#define INIT_FILTER_POLICY_FILTER_ACCEPT_LIST     0x01
/** @} */


/**@defgroup ble_cmd_phy BLE PHY Definition
 * @{
 * @ingroup ble_gap
 * @details BLE PHY definition.
 */
typedef uint8_t ble_phy_t;
#define BLE_PHY_1M                    0x01  /**< The transmitter PHY is LE 1M transmitter PHY.*/
#define BLE_PHY_2M                    0x02  /**< The transmitter PHY is LE 2M transmitter PHY.*/
#define BLE_PHY_CODED                 0x04  /**< The transmitter PHY is LE Coded transmitter PHY.*/
/** @} */


/**@defgroup ble_cmd_coded_Phy BLE PHY Definition
 * @{
 * @ingroup ble_gap
 * @details BLE PHY definition.
 */
typedef uint16_t ble_coded_phy_option_t;
#define BLE_CODED_PHY_NO_PREFERRED    0x00  /**< The transmitter PHY is LE 1M transmitter PHY.*/
#define BLE_CODED_PHY_S2              0x01  /**< The transmitter PHY is LE 2M transmitter PHY.*/
#define BLE_CODED_PHY_S8              0x02  /**< The transmitter PHY is LE Coded transmitter PHY.*/
/** @} */


/**@defgroup ble_evt_phy BLE PHY Definition
 * @{
 * @ingroup ble_gap
 * @details BLE PHY definition.
 */
typedef uint8_t ble_evt_phy_t;
#define BLE_EVT_PHY_1M                    0x01  /**< The transmitter PHY is LE 1M transmitter PHY.*/
#define BLE_EVT_PHY_2M                    0x02  /**< The transmitter PHY is LE 2M transmitter PHY.*/
#define BLE_EVT_PHY_CODED                 0x03  /**< The transmitter PHY is LE Coded transmitter PHY.*/
/** @} */


/**
 * @brief Set BLE Device Address parameter.
 * @ingroup ble_gap
*/
typedef struct __attribute__((packed)) ble_gap_addr_s
{
    ble_addr_type_t addr_type;            /**< @ref ble_gap_addr_type "Address type".*/
    uint8_t         addr[BLE_ADDR_LEN];   /**< Address.*/
} ble_gap_addr_t;


/**
 * @brief Get BLE Device Address parameter.
 * @ingroup ble_gap
*/
typedef struct __attribute__((packed)) ble_gap_get_addr_s
{
    ble_gap_addr_t *p_addr_param;        /**< a pointer to the gap address parameter.*/
} ble_gap_get_addr_t;


/**
 * @brief BLE Peer Address parameter.
 * @ingroup ble_gap
*/
typedef struct __attribute__((packed)) ble_gap_peer_addr_s
{
    ble_peer_addr_type_t addr_type;             /**< @ref ble_gap_peer_addr_type "Peer address type".*/
    uint8_t              addr[BLE_ADDR_LEN];    /**< Peer address.*/
} ble_gap_peer_addr_t;


/** @brief BLE connection parameter.
 * @ingroup ble_gap
*/
typedef struct ble_gap_conn_param_s
{
    /** Minimum connection interval.
      *  @note Connection interval = value * 1.25ms \n
      *        Connection interval range = @ref BLE_CONN_INTERVAL_MIN ~ @ref BLE_CONN_INTERVAL_MAX
      */
    uint16_t min_conn_interval;

    /** Maximum connection interval.
    *  @note Connection interval = value * 1.25ms \n
    *        Connection interval range = @ref BLE_CONN_INTERVAL_MIN ~ @ref BLE_CONN_INTERVAL_MAX
    */
    uint16_t max_conn_interval;

    /** Peripheral latency for the connection in number of connection event.
     *  @note Latency range = @ref BLE_CONN_LATENCY_MIN  to @ref BLE_CONN_LATENCY_MAX
     */
    uint16_t periph_latency;

    /** Supervision timeout.
     *  @note Supervision timeout = value * 10ms \n
     *        Supervision timeout range = @ref BLE_CONN_SUPV_TIMEOUT_MIN  ~ @ref BLE_CONN_SUPV_TIMEOUT_MAX
     */
    uint16_t supv_timeout;

} ble_gap_conn_param_t;


/** @brief BLE create connection parameter.
 * @ingroup ble_gap
*/
typedef struct ble_gap_create_conn_param_s
{
    /**< Host id. */
    uint8_t   host_id;

    /**
     * @ref ble_addr_type_t "BLE Address Type"
     */
    ble_addr_type_t   own_addr_type;

    /** Scan interval.
    *  @note Scan interval = value * 0.625ms \n
    *        Scan interval range = @ref SCAN_INTERVAL_MIN ~ @ref SCAN_INTERVAL_MAX
    */
    uint16_t  scan_interval;

    /** Scan window.
     *  @note Scan window = value * 0.625ms \n
     *        Scan window range = @ref SCAN_WINDOW_MIN  ~ @ref SCAN_WINDOW_MAX
     */
    uint16_t  scan_window;

    /** @ref ble_gap_init_filter "BLE initiator filter policy" */
    ble_initiator_filter_t     init_filter_policy;

    /** @ref ble_gap_peer_addr_t Peer address type and address. */
    ble_gap_peer_addr_t        peer_addr;

    /** @ref ble_gap_conn_param_t Connection parameters. */
    ble_gap_conn_param_t       conn_param;

} ble_gap_create_conn_param_t;


/** @brief BLE set terminate parameter.
 * @ingroup ble_gap
*/
typedef struct ble_gap_conn_terminate_param_s
{
    uint8_t               host_id;          /**< Host id. */
} ble_gap_conn_terminate_param_t;


/** @brief BLE connection parameter update parameter.
 * @ingroup ble_gap
*/
typedef struct ble_gap_conn_param_update_param_s
{
    uint8_t               host_id;          /**< Host id. */
    ble_gap_conn_param_t  ble_conn_param;   /**< connection parameter. */
} ble_gap_conn_param_update_param_t;


/** @brief BLE PHY update parameter.
 * @ingroup ble_gap
*/
typedef struct ble_gap_phy_update_param_s
{
    uint8_t                 host_id;          /**< Host id. */
    ble_phy_t               tx_phy;           /**< @ref ble_cmd_phy "TX PHY".*/
    ble_phy_t               rx_phy;           /**< @ref ble_cmd_phy "RX PHY".*/
    ble_coded_phy_option_t  coded_phy_option; /**< @ref ble_cmd_coded_Phy "Coded PHY option".*/
} ble_gap_phy_update_param_t;


/** @brief Read BLE PHY parameter.
 * @ingroup ble_gap
*/
typedef struct ble_gap_phy_read_param_s
{
    uint8_t                 host_id;          /**< Host id. */
} ble_gap_phy_read_param_t;


/** @brief Read BLE RSSI parameter.
 * @ingroup ble_gap
*/
typedef struct ble_gap_rssi_read_param_s
{
    uint8_t                 host_id;          /**< Host id. */
} ble_gap_rssi_read_param_t;


/** @brief Set BLE channel classification parameter.
 * @ingroup ble_gap
*/
typedef struct ble_gap_host_ch_classif_s
{
    uint8_t                 channel_map[5];   /**< channel map value. */
} ble_gap_host_ch_classif_t;


/** @brief Read BLE channel map parameter.
 * @ingroup ble_gap
*/
typedef struct ble_gap_channel_map_read_s
{
    uint8_t                 host_id;          /**< Host id. */
} ble_gap_channel_map_read_t;


/** @brief BLE regenerate resolvable address parameter.
 * @ingroup ble_gap
*/
typedef struct ble_gap_regen_resol_addr_s
{
    uint8_t                 host_id;          /**< Host id. */
    uint8_t                 gen_new_irk;      /**< whether to generate a new IRK. */
} ble_gap_regen_resol_addr_t;


/**
 * @brief Set BLE Device Address Event Parameters.
 * @ingroup ble_gap
*/
typedef struct __attribute__((packed)) ble_evt_gap_set_addr_s
{
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */

} ble_evt_gap_set_addr_t;


/**
 * @brief Get BLE Device Address event Parameters.
 * @ingroup ble_gap
*/
typedef struct __attribute__((packed)) ble_evt_gap_get_addr_s
{
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    ble_gap_addr_t          device_info;     /**< Device address type and address". */
} ble_evt_gap_get_addr_t;


/**
 * @brief BLE Connection Complete Event Parameters.
 * @ingroup ble_gap
*/
typedef struct __attribute__((packed)) ble_evt_gap_conn_complete_s
{
    uint8_t                 host_id;         /**< Host id. */
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    ble_gap_role_t          role;            /**< @ref ble_gap_role "Connection role" */
    ble_gap_peer_addr_t     peer_addr;       /**< Remote device address type and address. */
    uint16_t                conn_interval;   /**< The current link connection interval value. */
    uint16_t                periph_latency;  /**< The current link peripheral latency value. */
    uint16_t                supv_timeout;    /**< The current link connection supervision timeout value. */
} ble_evt_gap_conn_complete_t;


/**
 * @brief Set BLE Device Address Event Parameters.
 * @ingroup ble_gap
*/
typedef struct __attribute__((packed)) ble_evt_gap_create_conn_s
{
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */

} ble_evt_gap_create_conn_t;


/**
 * @brief BLE Connection Update Parameter Event Parameters.
 * @ingroup ble_gap
*/
typedef struct __attribute__((packed)) ble_evt_gap_conn_param_update_s
{
    uint8_t                 host_id;         /**< Host id. */
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint16_t                conn_interval;   /**< The current link connection interval value. */
    uint16_t                periph_latency;  /**< The current link peripheral latency value. */
    uint16_t                supv_timeout;    /**< The current link connection supervision timeout value. */
} ble_evt_gap_conn_param_update_t;


/**
 * @brief BLE Disconnection Complete Event Parameters.
 * @ingroup ble_gap
*/
typedef struct __attribute__((packed)) ble_evt_gap_disconn_complete_s
{
    uint8_t                 host_id;         /**< Host id. */
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t                 reason;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_evt_gap_disconn_complete_t;


/**
 * @brief BLE PHY Related Event Parameters.
 * @ingroup ble_gap
*/
typedef struct __attribute__((packed)) ble_evt_gap_phy_s
{
    uint8_t                 host_id;         /**< Host id. */
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    ble_evt_phy_t           tx_phy;          /**< @ref ble_cmd_phy "TX PHY". */
    ble_evt_phy_t           rx_phy;          /**< @ref ble_cmd_phy "RX PHY". */
} ble_evt_gap_phy_t;


/**
 * @brief Set BLE Device Address Event Parameters.
 * @ingroup ble_gap
*/
typedef struct __attribute__((packed)) ble_evt_gap_rssi_read_s
{
    uint8_t                 host_id;         /**< Host id. */
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    int8_t                  rssi;            /**< RSSI value. */
} ble_evt_gap_rssi_read_t;


/**
 * @brief Read BLE Channel Map Event Parameters.
 * @ingroup ble_gap
*/
typedef struct __attribute__((packed)) ble_evt_gap_ch_map_read_s
{
    uint8_t                 host_id;         /**< Host id. */
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t                 channel_map[5];  /**< channel map value. */
} ble_evt_gap_ch_map_read_t;


/**
 * @brief BLE GAP Event Parameters.
 * @ingroup ble_gap
*/
typedef struct ble_evt_gap_s
{
    union
    {
        ble_evt_gap_set_addr_t           evt_set_addr;          /**< Set device address event parameters. */
        ble_evt_gap_get_addr_t           evt_get_addr;          /**< Get device address event parameters. */
        ble_evt_gap_conn_complete_t      evt_conn_complete;     /**< BLE connection complete event parameters. */
        ble_evt_gap_create_conn_t        evt_create_conn;       /**< BLE create connection event parameters. */
        ble_evt_gap_conn_param_update_t  evt_conn_param_update; /**< BLE connection update parameter event parameters. */
        ble_evt_gap_disconn_complete_t   evt_disconn_complete;  /**< BLE disconnection complete event parameters. */
        ble_evt_gap_phy_t                evt_phy;               /**< BLE PHY related event parameters. */
        ble_evt_gap_rssi_read_t          evt_rssi;              /**< BLE read RSSI event parameters. */
        ble_evt_gap_ch_map_read_t        evt_channel_map;       /**< BLE read channel map event parameters. */
    } param;                                                    /**< Event parameters. */
} ble_evt_gap_t;

/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

/** @brief Get BLE device address and device address type.
 *
 *  This function is used to get BLE Local Device Address and Device Address Type.
 *
 * @ingroup ble_gap
 *
 * @note       BLE Address : Little Endian and the length is @ref BLE_ADDR_LEN \n
 *             If Device BLE Address is set to "01:02:03:04:05:06", addrParam->addr[0] = 0x06
 *
 * @par         BLE_GAP_EVT_GET_DEVICE_ADDR
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_device_addr_get(ble_gap_addr_t *p_addr);


/** @brief Set BLE device address and device address type.
 *
 * This function is used to set BLE Local Device Address and Device Address Type.
 *
 * @ingroup ble_gap
 *
 *
 * @note      BLE Address : Little Endian and the length is @ref BLE_ADDR_LEN \n
 *            If Device BLE Address is set to "01:02:03:04:05:06", addrParam->addr[0] = 0x06
 *
 * @param[in] p_addr : a pointer to the device BLE address and BLE address type.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_device_addr_set(ble_gap_addr_t *p_addr);


/** @brief BLE connection create command.
 *
 * @ingroup ble_gap
 *
 * @param[in] p_param : a pointer to the create connection parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_conn_create(ble_gap_create_conn_param_t *p_param);


/** @brief BLE cancel create connection process command.
 *
 * @ingroup ble_gap
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_conn_create_cancel(void);


/** @brief BLE connection parameter update.
 *
 * @ingroup ble_gap
 *
 * @note    When using the SCAN function for multi-link, ensure that the current connection interval is greater than 10ms x number of links.
 * @param[in] p_param : a pointer to the connection parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_conn_param_update(ble_gap_conn_param_update_param_t *p_param);


/** @brief Terminate the BLE connection link.
 *
 * @ingroup ble_gap_func
 *
 * @param[in] host_id : the link's host id.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_conn_terminate(uint8_t host_id);


/** @brief BLE PHY update.
 *
 * @ingroup ble_gap
 *
 * @param[in] p_param : a pointer to the PHY parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_phy_update(ble_gap_phy_update_param_t *p_param);


/** @brief BLE read PHY.
 *
 * @ingroup ble_gap
 *
 * @param[in] host_id : the link's host id.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_phy_read(uint8_t host_id);


/** @brief BLE read RSSI value.
 *
 * @ingroup ble_gap
 *
 * @param[in] host_id : the link's host id.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_rssi_read(uint8_t host_id);


/** @brief BLE set le channel classification value.
 *
 * @ingroup ble_gap
 *
 * @param[in] p_param : a pointer to channel classification parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_host_ch_classif_set(ble_gap_host_ch_classif_t *p_param);


/** @brief BLE read channel map value.
 *
 * @ingroup ble_gap
 *
 * @param[in] host_id : the link's host id.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_channel_map_read(uint8_t host_id);


/** @brief BLE Init resolvable address.
 *
 * @ingroup ble_gap
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_resolvable_address_init(void);


/** @brief BLE regenerate resolvable address.
 *
 * @ingroup ble_gap
 *
 * @param[in] host_id : the link's host id.
 * @param[in] en_new_irk :  whether to generate a new IRK.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_regenerate_resolvable_address(uint8_t host_id, uint8_t en_new_irk);


#ifdef __cplusplus
};
#endif

#endif /* __BLE_GAP_H__*/

