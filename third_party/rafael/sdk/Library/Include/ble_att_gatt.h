/** @file ble_att_gatt.h
 *
 * @brief BLE attribute protocol and generic attribute profile header file.
 *
 */
/**
 * @defgroup ble_att_gatt Attribute Protocol
 * @ingroup BLE_group
 * @{
 * @brief Define BLE attribute protocol definitions, structures, and functions.
 * @}
 */

#ifndef _BLE_ATT_GATT_H_
#define _BLE_ATT_GATT_H_

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
/** @ingroup ble_att_gatt
 * @{
*/
#define BLE_HOSTID_RESERVED                                             0xFF /**< Reserved BLE host id.*/

#define BLE_GATT_ATT_MTU_MIN                                            23   /**< Minimum ATT MTU size in bytes.*/
#define BLE_GATT_ATT_MTU_MAX                                            247  /**< Maximum ATT MTU size in bytes.*/

#define BLE_GATT_DATA_LENGTH_MIN                                        27   /**< Minimum data length size in bytes.*/
#define BLE_GATT_DATA_LENGTH_MAX                                        251  /**< Maximum data length size in bytes.*/
/** @} */


/**
 * @defgroup ble_att_uuid_type BLE Attribute Type UUID Format Definition
 * @{
 * @ingroup ble_att_gatt
 * @details  BLE attribute type UUID format definition for "db_permission_format" in @ref ble_att_param_t.
 */
#define ATT_TYPE_FORMAT_128UUID                                         0x00  /**< 128-bits UUID. */
#define ATT_TYPE_FORMAT_16UUID                                          0x01  /**< 16-bits UUID. */
/** @} */


/**
 * @defgroup ble_att_bond_opt BLE Attribute Value Bond or Not Option Definition
 * @{
 * @ingroup ble_att_gatt
 * @details  BLE attribute data bond option definition for "db_permission_format" in @ref ble_att_param_t.
 */
#define ATT_VALUE_BOND_DISABLE                                          0x00  /**< The attribute value will be bond when writing. */
#define ATT_VALUE_BOND_ENABLE                                           0x02  /**< The attribute value will be bond when writing. */
/** @} */


/**
 * @defgroup ble_att_permission BLE Attribute Permission Definition
 * @{
 * @ingroup ble_att_gatt
 * @details  BLE attribute permission definition for "property_value" in @ref ble_att_param_t.
 */
#define ATT_PERMISSION_ENC_READ                                         0x04  /**< The attribute is encryption required for the remote read access. */
#define ATT_PERMISSION_ENC_WRITE                                        0x08  /**< The attribute is encryption required for the remote write access. */
#define ATT_PERMISSION_AUTHE_READ                                       0x10  /**< The attribute is authentication required for the remote read access. */
#define ATT_PERMISSION_AUTHE_WRITE                                      0x20  /**< The attribute is authentication required for the remote write access. */
#define ATT_PERMISSION_AUTHO_READ                                       0x40  /**< The attribute is authorization required for the remote read access. */
#define ATT_PERMISSION_AUTHO_WRITE                                      0x80  /**< The attribute is authorization required for the remote write access. */
/** @} */


/**
 * @defgroup ble_att_char_propertie BLE Characteristic Properties Definition
 * @{
 * @ingroup ble_att_gatt
 * @details  BLE characteristic properties definition for "property_value" in @ref ble_att_param_t.
 * @note The characteristic properties bit field determines how the characteristic value can be used, or how the characteristic descriptors can be accessed.
 */
#define GATT_DECLARATIONS_PROPERTIES_BROADCAST                          0x01  /**< Broadcasting of the value permitted. */
#define GATT_DECLARATIONS_PROPERTIES_READ                               0x02  /**< Reading the value permitted. */
#define GATT_DECLARATIONS_PROPERTIES_WRITE_WITHOUT_RESPONSE             0x04  /**< Writing the value with write command permitted. */
#define GATT_DECLARATIONS_PROPERTIES_WRITE                              0x08  /**< Writing the value with write request permitted. */
#define GATT_DECLARATIONS_PROPERTIES_NOTIFY                             0x10  /**< Notification of the value permitted. */
#define GATT_DECLARATIONS_PROPERTIES_INDICATE                           0x20  /**< Indications of the value permitted. */
#define GATT_DECLARATIONS_PROPERTIES_AUTHENTICATED_SIGNED_WRITES        0x40  /**< Writing the value with signed write command permitted. */
#define GATT_DECLARATIONS_PROPERTIES_EXTENDED_PROPERTIES                0x80  /**< Addional characteristic properties are defined in the characteristic extended properties descriptor. */
/** @} */


/**
 * @defgroup ble_att_char_format BLE Characteristic Format Field Definition
 * @{
 * @ingroup ble_att_gatt
 * @details  The format field determines how a single value contained in the characteristic value is formatted.
 *
 */
#define GATT_CHARACTERISTIC_FORMAT_RFU                                  0x00  /**< Reserved for future use. */
#define GATT_CHARACTERISTIC_FORMAT_BOOLEAN                              0x01  /**< Boolean. */
#define GATT_CHARACTERISTIC_FORMAT_2BIT                                 0x02  /**< Unsigned 2-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_NIBBLE                               0x03  /**< Unsigned 4-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_UINT8                                0x04  /**< Unsigned 8-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_UINT12                               0x05  /**< Unsigned 12-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_UINT16                               0x06  /**< Unsigned 16-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_UINT24                               0x07  /**< Unsigned 24-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_UINT32                               0x08  /**< Unsigned 32-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_UINT48                               0x09  /**< Unsigned 48-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_UINT64                               0x0A  /**< Unsigned 64-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_UINT128                              0x0B  /**< Unsigned 128-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_SINT8                                0x0C  /**< Signed 2-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_SINT12                               0x0D  /**< Signed 12-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_SINT16                               0x0E  /**< Signed 16-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_SINT24                               0x0F  /**< Signed 24-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_SINT32                               0x10  /**< Signed 32-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_SINT48                               0x11  /**< Signed 48-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_SINT64                               0x12  /**< Signed 64-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_SINT128                              0x13  /**< Signed 128-bit integer. */
#define GATT_CHARACTERISTIC_FORMAT_FLOAT32                              0x14  /**< IEEE-754 32-bit floating point. */
#define GATT_CHARACTERISTIC_FORMAT_FLOAT64                              0x15  /**< IEEE-754 64-bit floating point. */
#define GATT_CHARACTERISTIC_FORMAT_SFLOAT                               0x16  /**< IEEE-11073 16-bit sfloat. */
#define GATT_CHARACTERISTIC_FORMAT_FLOAT                                0x17  /**< IEEE-11073 32-bit float. */
#define GATT_CHARACTERISTIC_FORMAT_DUINT16                              0x18  /**< IEEE-20601 format. */
#define GATT_CHARACTERISTIC_FORMAT_UTF8S                                0x19  /**< UTF-8 string. */
#define GATT_CHARACTERISTIC_FORMAT_UTF16S                               0x1A  /**< UTF-16 string. */
#define GATT_CHARACTERISTIC_FORMAT_STRUCT                               0x1B  /**< Opaque structure. */
/** @} */


/**
 * @defgroup ble_att_name_space BLE Characteristic Namespace Field Definition
 * @{
 * @ingroup ble_att_gatt
 * @details  The Name Space field is used to identify the organization as defined in the Assigned Numbers document.
 *
 * @note The name space of the description as defined in Assigned Numbers Specification: \n
 *       https://www.bluetooth.org/en-us/specification/assigned-numbers
 */
#define GATT_CHARACTERISTIC_BLUETOOTH_NAMESPACE_BLUETOOTH_SIG           0x01  /**< Bluetooth SIG defined namespace. */
/** @} */


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**
 * @defgroup ble_att_opcode BLE Attribute Operation Code Definition
 * @{
 * @details  BLE attribute operation code definition.
 * @ingroup ble_att_gatt
 */
typedef uint8_t ble_att_opcode_t;
#define OPCODE_ATT_ERROR_RESPONSE                                       0x01     /**< Error response. */
#define OPCODE_ATT_EXCHANGE_MTU_REQUEST                                 0x02     /**< Exchange MTU request. */
#define OPCODE_ATT_EXCHANGE_MTU_RESPONSE                                0x03     /**< Exchange MTU response. */
#define OPCODE_ATT_FIND_INFORMATION_REQUEST                             0x04     /**< Find information request. */
#define OPCODE_ATT_FIND_INFORMATION_RESPONSE                            0x05     /**< Find information response. */
#define OPCODE_ATT_FIND_BY_TYPE_VALUE_REQUEST                           0x06     /**< Find by type value request. */
#define OPCODE_ATT_FIND_BY_TYPE_VALUE_RESPONSE                          0x07     /**< Find by type value response. */
#define OPCODE_ATT_READ_BY_TYPE_REQUEST                                 0x08     /**< Read by type request. */
#define OPCODE_ATT_READ_BY_TYPE_RESPONSE                                0x09     /**< Read by type response. */
#define OPCODE_ATT_READ_REQUEST                                         0x0A     /**< Read request. */
#define OPCODE_ATT_READ_RESPONSE                                        0x0B     /**< Read response. */
#define OPCODE_ATT_READ_BLOB_REQUEST                                    0x0C     /**< Read blob request. */
#define OPCODE_ATT_READ_BLOB_RESPONSE                                   0x0D     /**< Read blob response. */
#define OPCODE_ATT_READ_MULTIPLE_REQUEST                                0x0E     /**< Read multiple request. */
#define OPCODE_ATT_READ_MULTIPLE_RESPONSE                               0x0F     /**< Read multiple response. */
#define OPCODE_ATT_READ_BY_GROUP_TYPE_REQUEST                           0x10     /**< Read by group type request. */
#define OPCODE_ATT_READ_BY_GROUP_TYPE_RESPONSE                          0x11     /**< Read by group type response. */
#define OPCODE_ATT_WRITE_REQUEST                                        0x12     /**< Write request. */
#define OPCODE_ATT_WRITE_RESPONSE                                       0x13     /**< Write response. */
#define OPCODE_ATT_WRITE_COMMAND                                        0x52     /**< Write command. */
#define OPCODE_ATT_PREPARE_WRITE_REQUEST                                0x16     /**< Prepare write request. */
#define OPCODE_ATT_PREPARE_WRITE_RESPONSE                               0x17     /**< Prepare write response. */
#define OPCODE_ATT_EXECUTE_WRITE_REQUEST                                0x18     /**< Execute write request. */
#define OPCODE_ATT_EXECUTE_WRITE_RESPONSE                               0x19     /**< Execute write response. */
#define OPCODE_ATT_HANDLE_VALUE_NOTIFICATION                            0x1B     /**< Handle value notification. */
#define OPCODE_ATT_HANDLE_VALUE_INDICATION                              0x1D     /**< Handle value indication. */
#define OPCODE_ATT_HANDLE_VALUE_CONFIRMATION                            0x1E     /**< Handle value confirmation. */
#define OPCODE_ATT_SIGNED_WRITE_COMMAND                                 0xD2     /**< Signed write command. */
#define OPCODE_ATT_RESTORE_BOND_DATA_COMMAND                            0xFF     /**< VENDOR DEFINE: restore bond data Command. */
/** @} */


/**
 * @defgroup ble_att_error_code BLE Attribute Error Code Definition
 * @{
 * @details  BLE attribute error code definition.
 * @ingroup ble_att_gatt
 */
typedef uint8_t ble_att_error_code_t;
#define ERR_CODE_ATT_NO_ERROR                                           0x00  /**< The procedure finished without errors. */
#define ERR_CODE_ATT_INVALID_HANDLE                                     0x01  /**< Invalid attribute handle. */
#define ERR_CODE_ATT_READ_NOT_PERMITTED                                 0x02  /**< Read not permitted. */
#define ERR_CODE_ATT_WRITE_NOT_PERMITTED                                0x03  /**< Write not permitted. */
#define ERR_CODE_ATT_INVALID_PDU                                        0x04  /**< Invalid PDU. */
#define ERR_CODE_ATT_INSUFFICIENT_AUTHENTICATION                        0x05  /**< Authenticated link required. */
#define ERR_CODE_ATT_REQUEST_NOT_SUPPORTED                              0x06  /**< Request not supported. */
#define ERR_CODE_ATT_INVALID_OFFSET                                     0x07  /**< Invalid offset*/
#define ERR_CODE_ATT_INSUFFICIENT_AUTHORIZATION                         0x08  /**< Insufficient authorization. */
#define ERR_CODE_ATT_PREPARE_QUEUE_FULL                                 0x09  /**< Prepare queue full. */
#define ERR_CODE_ATT_ATTRIBUTE_NOT_FOUND                                0x0A  /**< Attribute not found. */
#define ERR_CODE_ATT_ATTRIBUTE_NOT_LONG                                 0x0B  /**< Attribute cannot be read or written using read/write blob requests. */
#define ERR_CODE_ATT_INSUFFICIENT_ENCRYPTION_KEY_SIZE                   0x0C  /**< Encryption key size is insufficient. */
#define ERR_CODE_ATT_INVALID_ATTRIBUTE_VALUE_LENGTH                     0x0D  /**< Invalid value length. */
#define ERR_CODE_ATT_UNLIKELY_ERROR                                     0x0E  /**< Unlikely error. */
#define ERR_CODE_ATT_INSUFFICIENT_ENCRYPTION                            0x0F  /**< Encrypted link required. */
#define ERR_CODE_ATT_UNSUPPORTED_GROUP_TYPE                             0x10  /**< Attribute type is not a supported grouping attribute. */
#define ERR_CODE_ATT_INSUFFICIENT_RESOURCES                             0x11  /**< Encrypted link required. */
#define ERR_CODE_ATT_DATABASE_OUT_OF_SYNC                               0x12  /**< The server requests the client to rediscover the database. */
#define ERR_CODE_ATT_VALUE_NOT_ALLOWED                                  0x13  /**< The attribute parameter value was not allowed. */
#define ERR_CODE_ATT_RESERVED                                           0x14  /**< Reserved for future use. */
#define ERR_CODE_ATT_APPLICATION_ERROR                                  0x80  /**< Attribute application error. */
#define ERR_CODE_ATT_WRITE_REQUEST_REJECTED                             0xFC  /**< Write request rejected. */
#define ERR_CODE_ATT_CLIENT_CHAR_CONFIG_DESCRTR_IMPROPERLY_CONFIGURED   0xFD  /**< Client characteristic configuration descriptor improperly configured. */
#define ERR_CODE_ATT_PROCEDURE_ALREADY_IN_PROGRESS                      0xFE  /**< Procedure already in progress. */
#define ERR_CODE_ATT_OUT_OF_RANGE                                       0xFF  /**< Out of range. */
/** @} */


/**
 * @defgroup ble_att_gatt_role BLE GATT Role Definition
 * @{
 * @ingroup ble_att_gatt
 * @details BLE GATT role.
 */
typedef uint8_t ble_gatt_role_t;
#define BLE_GATT_ROLE_CLIENT                                            (0x01u)  /**< GATT client is reading or writing data from or to the GATT server. */
#define BLE_GATT_ROLE_SERVER                                            (0x02u)  /**< GATT server contains the characteristic database that is being read or written by a GATT client. */
/** @} */


/**
 * @defgroup ble_att_cccd BLE GATT Client Characteristic Configuration Value Definition
 * @{
 * @details BLE GATT client characteristic configuration value definition.
 * @ingroup ble_att_gatt
 * @{
 */
typedef uint16_t ble_gatt_cccd_val_t;
#define BLEGATT_CCCD_NONE                                               0x0000  /**< Disabled notification/ indication.  */
#define BLEGATT_CCCD_NOTIFICATION                                       0x0001  /**< The Characteristic shall be notified.  */
#define BLEGATT_CCCD_INDICATION                                         0x0002  /**< The Characteristic shall be indicated.  */
#define BLEGATT_CCCD_NOTIFY_INDICATE                                    0x0003  /**< The Characteristic shall be both notified and indicated. */
/** @} */
/** @} */


/**
 * @defgroup ble_att_write_type BLE GATT Characteristic Value Write Definition
 * @{
 * @details There are five sub-procedures that can be used to write a Characteristic Value.
 * @ingroup ble_att_gatt
 * @{
 */
typedef uint8_t ble_gatt_write_t;
#define BLEGATT_WRITE                                                   0x00  /**< GATT write (Write request).  */
#define BLEGATT_WRITE_WITHOUT_RSP                                       0x01  /**< GATT write without response (Write command). */
/** @} */
/** @} */


/** @brief BLE Services GATT Event Parameter Definition
 * @ingroup ble_att_gatt
*/
typedef struct ble_evt_att_param_s
{
    uint8_t         host_id;                      /**< Host id.*/
    uint8_t         gatt_role;                    /**< @ref ble_att_gatt_role "ble_gatt_role_t".*/
    uint16_t        cb_index;                     /**< Callback index.*/
    uint16_t        handle_num;                   /**< Characteristic value handle.*/
    uint8_t         opcode;                       /**< Event opcode @ref ble_att_opcode .*/
    uint8_t         event;                        /**< User defined event identification .*/
    uint16_t        length;                       /**< The length of BLE attribute event data.*/
    uint8_t         data[];                       /**< BLE attribute event data.*/
} ble_evt_att_param_t;


/** @brief BLE Attribute Handle Parameter Definition
 * @ingroup ble_att_gatt
*/
typedef struct ble_att_handle_param_s
{
    uint16_t        handle_num;                     /**< Attribute handle number.*/
    uint8_t         property_value;                 /**< characteristic property value.*/
    union
    {
        uint8_t     cfg_client_charc;               /**< Client characteristic configuration descriptor value.*/
        uint8_t     cfg_server_charc;               /**< Server characteristic configuration descriptor value*/
    } value;                                        /**< Client/Server characteristic configuration descriptor value.*/
} ble_att_handle_param_t;


/** @brief BLE Attribute Database Mapping By Host ID
 * @ingroup ble_att_gatt
*/
typedef struct ble_att_db_mapping_by_id_s
{
    ble_att_handle_param_t *map_client_db;          /**< Mapping to client attribute database.*/
    ble_att_handle_param_t *map_server_db;          /**< Mapping to server attribute database.*/
} ble_att_db_mapping_by_id_t;


/** @brief BLE Attribute Database Mapping By Host ID Size
 * @ingroup ble_att_gatt
*/
typedef struct ble_att_db_mapping_by_id_size_s
{
    uint16_t        size_map_client_db;             /**< Mapping to size of client attribute database.*/
    uint16_t        size_map_server_db;             /**< Mapping to size of server attribute database.*/
} ble_att_db_mapping_by_id_size_t;


/** @brief BLE Service Declaration With An Attribute.
 * @ingroup ble_att_gatt
*/
typedef struct ble_att_param_s
{
    void            *p_uuid_type;                                 /**< Attribute type which defined by a UUID, an UUID is used to identify every attribute type. */
    void            *p_uuid_value;                                /**< Attribute Value shall be the 16-bit Bluetooth UUID or 128-bit UUID for the service/ characteristic, known as the service/ characteristic UUID.*/
    uint16_t        att_len;                                      /**< The length of attribute value.*/
    uint8_t         property_value;                               /**< Characteristic properties.*/
    uint8_t         db_permission_format;                         /**< UUID format and characteristic permission.*/
    void            (*att_handler)(ble_evt_att_param_t *p_param); /**< Register callback function. */
} ble_att_param_t;


/** @brief BLE Pre-defined Services Definition
 * @ingroup ble_att_gatt
*/
typedef struct ble_att_role_by_id_s
{
    const ble_att_param_t *const *p_client_db;              /**< The table of GATT database describes the pre-defined attributes. */
    const ble_att_param_t *const *p_server_db;              /**< The table of GATT database describes the server and attributes contained on the server. */
} ble_att_role_by_id_t;


/** @brief BLE Write Suggested default data length parameter
 * @ingroup ble_att_gatt
*/
typedef struct
{
    uint16_t        tx_octets;                              /**< The Host's suggested value for the Controller's maximum transmitted number of payload octets. @ref BLE_GATT_DATA_LENGTH_MIN to @ref BLE_GATT_DATA_LENGTH_MAX */
} ble_gatt_suggested_data_len_param_t;


/** @brief BLE set data length parameter
 * @ingroup ble_att_gatt
*/
typedef struct
{
    uint8_t         host_id;                                /**< Host id.*/
    uint16_t        tx_octets;                              /**< The value for the Controller's maximum transmitted number of payload octets. @ref BLE_GATT_DATA_LENGTH_MIN to @ref BLE_GATT_DATA_LENGTH_MAX */
} ble_gatt_data_len_param_t;


/** @brief BLE set mtu length parameter
 * @ingroup ble_att_gatt
*/
typedef struct
{
    uint8_t         host_id;                                /**< Host id.*/
    uint16_t        mtu;                                    /**< The maximum size of any packet sent between a client and a server.. @ref BLE_GATT_ATT_MTU_MIN to @ref BLE_GATT_ATT_MTU_MAX */
} ble_gatt_mtu_param_t;


/** @brief BLE get mtu length parameter
 * @ingroup ble_att_gatt
*/
typedef struct
{
    uint8_t         host_id;                                /**< Host id.*/
    uint16_t        *p_mtu;                                 /**< A pointer to the read mtu size buffer. */
} ble_gatt_get_mtu_param_t;


/** @brief BLE attribute data parameter
 * @ingroup ble_att_gatt
*/
typedef struct
{
    uint8_t         host_id;                                /**< Host id.*/
    uint16_t        handle_num;                             /**< Characteristic value handle.*/
    uint16_t        length;                                 /**< The length of attribute data.*/
    uint8_t         *p_data;                                /**< A pointer to the attribute data. */
} ble_gatt_data_param_t;


/** @brief BLE attribute error response parameter
 * @ingroup ble_att_gatt
*/
typedef struct
{
    uint8_t                 host_id;                        /**< Host id.*/
    uint16_t                handle_num;                     /**< Characteristic value handle.*/
    ble_att_opcode_t        opcode;                         /**< The request that generated this ATT_ERROR_RSP PDU.*/
    ble_att_error_code_t    err_rsp;                        /**< The reason why the request has generated an ATT_ERROR_RSP PDU. */
} ble_gatt_err_rsp_param_t;


/** @brief BLE service handle table parameter
 * @ingroup ble_att_gatt
*/
typedef struct
{
    uint8_t                 host_id;                        /**< Host id.*/
    ble_gatt_role_t         gatt_role;                      /**< @ref ble_att_gatt_role "ble_gatt_role_t". */
    ble_att_param_t         *p_element;                     /**< A pinter to the service declaration element. */
    void                    *p_handle_num_addr;             /**< A pinter to the service handle value. */
} ble_gatt_handle_table_param_t;


/** @brief BLE Read request parameter
 * @ingroup ble_att_gatt
*/
typedef struct
{
    uint8_t                 host_id;                        /**< Host id.*/
    uint16_t                handle_num;                     /**< Characteristic value handle.*/
} ble_gatt_read_req_param_t;


/** @brief BLE Read blob request parameter
 * @ingroup ble_att_gatt
*/
typedef struct
{
    uint8_t                 host_id;                        /**< Host id. */
    uint16_t                handle_num;                     /**< Characteristic value handle. */
    uint16_t                offset;                         /**< The offset of the first octet to be read. */
} ble_gatt_read_blob_req_param_t;


/**
 * @brief BLE MTU Exchange Event Parameters.
 * @ingroup ble_att_gatt
*/
typedef struct __attribute__((packed)) ble_evt_mtu_s
{
    uint8_t                 host_id;                        /**< Host id. */
    uint16_t                mtu;                            /**< MTU size */
} ble_evt_mtu_t;


/**
 * @brief BLE Database Parsing Complete Event Parameters.
 * @ingroup ble_att_gatt
*/
typedef struct __attribute__((packed)) ble_evt_att_db_parse_complete_s
{
    uint8_t                 host_id;                        /**< Host id. */
    uint8_t                 result;                         /**< The result of parsing attribute database operation, refer to @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_evt_att_db_parse_complete_t;


/**
 * @brief BLE Get handle table Complete Event Parameters.
 * @ingroup ble_att_gatt
*/
typedef struct __attribute__((packed)) ble_evt_att_handle_table_complete_s
{
    uint8_t                 host_id;                        /**< Host id. */
    ble_att_param_t         *p_element;                     /**< A pinter to the service declaration element. */
} ble_evt_att_handle_table_complete_t;



/** @brief BLE Data Length Change Event Parameters.
 * @ingroup ble_att_gatt
*/
typedef struct __attribute__((packed)) ble_evt_data_length_change_s
{
    /**< Host id. */
    uint8_t                 host_id;

    /** Preferred maximum number of payload octets that the Controller for reception in a single Link Layer packet on this connection.*/
    uint16_t                max_tx_octets;

    /** The maximum time that the local Controller will take to send a Link Layer packet on this connection. */
    uint16_t                max_tx_time;

    /** Preferred maximum number of payload octets that the Controller for transmission in a single Link Layer packet on this connection.*/
    uint16_t                max_rx_octets;

    /** The maximum time that the local Controller expects to take to receive a Link Layer packet on this connection. */
    uint16_t                max_rx_time;

} ble_evt_data_length_change_t;



/** @brief BLE Set Data Length Event Parameters.
 * @ingroup ble_att_gatt
*/
typedef struct __attribute__((packed)) ble_evt_data_length_set_s
{
    uint8_t                 status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_evt_data_length_set_t;


/** @brief BLE Write Suggested Default Data Length Event Parameters.
 * @ingroup ble_att_gatt
*/
typedef struct __attribute__((packed)) ble_evt_suggest_data_length_set_s
{
    uint8_t                 status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_evt_suggest_data_length_set_t;


/**
 * @brief BLE ATT/GATT Event Parameters.
 * @ingroup ble_att_gatt
*/
typedef struct ble_evt_att_gatt_s
{
    union
    {
        ble_evt_mtu_t                       ble_evt_mtu;                        /**< BLE MTU exchange event parameters. */
        ble_evt_data_length_change_t        ble_evt_data_length_change;         /**< BLE data length change event parameters. */
        ble_evt_att_db_parse_complete_t     ble_evt_att_db_parse_complete;      /**< BLE database parsing complete event parameters. */
        ble_evt_att_handle_table_complete_t ble_evt_att_handle_table_complete;  /**< BLE Get service handle table complete event parameters. */
        ble_evt_data_length_set_t           ble_evt_data_length_set;            /**< BLE set data length complete event parameters. */
        ble_evt_suggest_data_length_set_t   ble_evt_suggest_data_length_set;    /**< BLE write suggested default data length complete event parameters. */
    } param;                                                                    /**< Event parameters. */
} ble_evt_att_gatt_t;


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

/** @brief Set suggested data length.
 *
 * @ingroup ble_att_gatt
 *
 * @note The range of tx_octets is @ref BLE_GATT_DATA_LENGTH_MIN to @ref BLE_GATT_DATA_LENGTH_MAX bytes.
 *
 * @param[in] tx_octets : The preferred maximum number of payload octets.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_suggest_data_len_set(uint16_t tx_octets);


/** @brief Set preferred MTU size.
 *
 * @ingroup ble_att_gatt
 *
 * @note The range of mtu is @ref BLE_GATT_ATT_MTU_MIN to @ref BLE_GATT_ATT_MTU_MAX bytes.
 *
 * @param[in] host_id : the link's host id.
 * @param[in] mtu     : mtu size.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_default_mtu_size_set(uint8_t host_id, uint16_t mtu);


/** @brief ATT_MTU exchange request.
 *
 * @ingroup ble_att_gatt
 *
 * @par BLE Event
 * Wait for @ref BLE_ATT_GATT_EVT_MTU_EXCHANGE event to get exchanged MTU size. \n
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
 *
 * @note The range of mtu is @ref BLE_GATT_ATT_MTU_MIN to @ref BLE_GATT_ATT_MTU_MAX bytes.
 *
 * @param[in] host_id : the link's host id.
 * @param[in] mtu     : mtu size.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_mtu_size_update(uint8_t host_id, uint16_t mtu);


/** @brief Set data length update.
 *
 * @ingroup ble_att_gatt
 *
 * @par BLE Event
 * Wait for @ref BLE_ATT_GATT_EVT_DATA_LENGTH_CHANGE event to get updated BLE data length.
 *
 * @note The range of tx_octets is @ref BLE_GATT_DATA_LENGTH_MIN to @ref BLE_GATT_DATA_LENGTH_MAX bytes.
 *
 * @param[in] host_id   : the link's host id.
 * @param[in] tx_octets : The preferred maximum number of TX payload octets.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_data_len_update(uint8_t host_id, uint16_t tx_octets);


/** @brief Get BLE GATT MTU Size.
 *
 * @ingroup ble_att_gatt
 *
 * @param[in] host_id : the link's host id.
 * @param[in] p_param : the pointer to p_param.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_mtu_size_get(uint8_t host_id, uint16_t *mtu);


/** @brief BLE GATT Read Response.
 *
 * @ingroup ble_att_gatt
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
 *
 * @note The read response is sent in reply to a received Read Request and contains the value of the attribute that has been read.
 *
 * @param[in] p_param : a pointer to the read response parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_gatt_read_rsp(ble_gatt_data_param_t *p_param);


/** @brief BLE GATT Read By Type Response.
 *
 * @ingroup ble_att_gatt
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
 *
 * @note The read by type response is sent in reply to a received Read By Type Request and contains the handle number and value of the attribute that has been read.
 *
 * @param[in] p_param : a pointer to the read by type parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_gatt_read_by_type_rsp(ble_gatt_data_param_t *p_param);


/** @brief BLE GATT Read Blob Response.
 *
 * @ingroup ble_att_gatt
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
 *
 * @note The read blob response is sent in reply to a received Read Blob Request and contains the value of the attribute that has been read.
 *
 * @param[in] p_param : a pointer to the read blob response parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_gatt_read_blob_rsp(ble_gatt_data_param_t *p_param);


/** @brief BLE GATT Error Response.
 *
 * @ingroup ble_att_gatt
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
 *
 * @note The Error Response is used to state that a given request cannot be performed, and to provide the reason.
 *
 * @param[in] p_param : a pointer to the error response parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_gatt_error_rsp(ble_gatt_err_rsp_param_t *p_param);


/** @brief BLE GATT Notification.
 *
 * @ingroup ble_att_gatt
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
 *
 * @note When a server is configured to notify a Characteristic Value to a client without the acknowledgment that the notification was successfully received.
 *
 * @param[in] p_param : a pointer to the notification parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_gatt_notification(ble_gatt_data_param_t *p_param);


/** @brief BLE GATT Indication.
 *
 * @ingroup ble_att_gatt
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
 *
 * @note When a server is configured to indicate a Characteristic Value to a client and expects the indication was successfully received.
 *
 * @param[in] p_param : a pointer to the indication parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_gatt_indication(ble_gatt_data_param_t *p_param);


/** @brief BLE GATT Write Request.
 *
 * @ingroup ble_att_gatt
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
 *
 * @note The Write Request is used to request the server to write the value of an attribute and acknowledge that this has been achieved in a Write Response. \n
 * A Write Response shall be sent by the server if the write of the Characteristic Value succeeded.
 *
 * @param[in] p_param : a pointer to the the write request parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_gatt_write_req(ble_gatt_data_param_t *p_param);


/** @brief BLE GATT Write Command.
 *
 * @ingroup ble_att_gatt
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
 *
 * @note Write a Characteristic Value to a server when the client knows the Characteristic Value Handle \n
 *       and the client does not need an acknowledgment that the write was successfully performed.
 *
 * @param[in] p_param : a pointer to the the write command parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_gatt_write_cmd(ble_gatt_data_param_t *p_param);


/** @brief BLE GATT Read Request.
 *
 * @ingroup ble_att_gatt
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
 *
 * @note Read a Characteristic Value from a server when the client knows the Characteristic Value Handle.
 *
 * @param[in] p_param : a pointer to the the read request parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_gatt_read_req(ble_gatt_read_req_param_t *p_param);


/** @brief BLE GATT Read Blob Request.
 *
 * @ingroup ble_att_gatt
 *
 * @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
 *
 * @note Read part of the value of an attribute at a given offset from a server when the client knows the Characteristic Value Handle.
 *
 * @param[in] p_param : a pointer to the the read blob request parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_gatt_read_blob_req(ble_gatt_read_blob_req_param_t *p_param);


#ifdef __cplusplus
};
#endif

#endif // _BLE_ATT_GATT_H_

