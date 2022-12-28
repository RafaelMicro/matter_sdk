/** @file ble_api.h
 *
 * @brief BLE API header file.
 *
 */

/**
 * @defgroup ble_api BLE API Definition
 * @ingroup BLE_group
 * @{
 * @brief Define BLE API definitions, structures, and functions.
 * @}
 */
#ifndef __BLE_API_H__
#define __BLE_API_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include "ble_hci_error.h"

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/** @brief BLE event indication callback definition.
 * @ingroup ble_api
*/
typedef void (pf_evt_indication)(uint32_t data_len);


/** @brief BLE api retrun parameter definition.
 * @ingroup ble_api
*/
typedef enum
{
    BLE_ERR_SEQUENTIAL_PROTOCOL_VIOLATION = -24,  /** @brief Detecting a sequential protocol violation. Usually happens in there is an another GATT request already in progress please wait and retry.*/
    BLE_ERR_DB_PARSING_IN_PROGRESS        = -23,  /** @brief Host peripheral database parsing is still in progress. */
    BLE_ERR_INVALID_HANDLE                = -22,  /** @brief Invalid BLE handle. */
    BLE_ERR_INVALID_HOST_ID               = -21,  /** @brief Invalid Host ID */
    BLE_ERR_CMD_NOT_SUPPORTED             = -20,  /** @brief Command not supported */
    BLE_ERR_INVALID_PARAMETER             = -19,  /** @brief Invalid parameter. */
    BLE_ERR_INVALID_STATE                 = -18,  /** @brief Invalid state. */
    BLE_ERR_TIMER_OP                      = -17,  /** @brief TIMER OP. */
    BLE_ERR_ALLOC_MEMORY_FAIL             = -16,  /** @brief RECVFROM memory fail. */
    BLE_ERR_RECVFROM_LEN_NOT_ENOUGH       = -15,  /** @brief RECVFROM length error. */
    BLE_ERR_RECVFROM_FAIL                 = -14,  /** @brief RECVFROM fail. */
    BLE_ERR_RECVFROM_NO_DATA              = -13,  /** @brief RECVFROM no data. */
    BLE_ERR_RECVFROM_POINTER_NULL         = -12,  /** @brief RECVFROM Pointer NULL. */
    BLE_ERR_SENDTO_FAIL                   = -11,  /** @brief SENDTO fail. */
    BLE_ERR_SENDTO_POINTER_NULL           = -10,  /** @brief SENDTO Pointer NULL. */
    BLE_BUSY                              = -9,   /** @brief Busy. */
    BLE_ERR_WRONG_CONFIG                  = -8,   /** @brief Wrong configuration. */
    BLE_ERR_SEMAPHORE_MALLOC_FAIL         = -7,   /** @brief Semaphore malloc fail. */
    BLE_ERR_THREAD_MALLOC_FAIL            = -6,   /** @brief Thread malloc fail. */
    BLE_ERR_QUEUE_MALLOC_FAIL             = -5,   /** @brief Queue malloc fail. */
    BLE_ERR_DATA_MALLOC_FAIL              = -4,   /** @brief Data malloc fail. */
    BLE_ERR_DUPLICATE_INIT                = -3,   /** @brief Duplicate init. */
    BLE_ERR_NOT_INIT                      = -2,   /** @brief Not initial. */
    BLE_ERR_UNKNOW_TYPE                   = -1,   /** @brief Unknown type. */
    BLE_ERR_OK                            = 0,    /** @brief Success. */
} ble_err_t;


typedef enum
{
    //====================================================================
    //     BLE COMMON API
    //====================================================================
    TYPE_BLE_COMMON_API_START                   = 0,

    /** @brief BLE common controller setting initial function (BLE stack initialization).
     *  @par   BLE_COMMON_EVT_SET_CONTROLLER_INFO
     *  @par   BLE_COMMON_EVT_SET_EVENT_MASK
     *  @par   BLE_COMMON_EVT_READ_LOCAL_VER
     *  @par   BLE_COMMON_EVT_READ_BUFFER_SIZE
     *
     *  @note  do
     *         - BLE phy init
     *         - Set vendor command (set BLE version, company ID and default public address)
     *         - Set LE event mask
     *         - Read local version
     *         - Read buffer size
     *  @param @ref ble_common_controller_info_t
     */
    TYPE_BLE_COMMON_CONTROLLER_INIT             = TYPE_BLE_COMMON_API_START,

    /** @brief BLE Read Filter accept list size command.
     *  @par   BLE_COMMON_EVT_READ_FILTER_ACCEPT_LIST_SIZE
     *
     *  @param none
     */
    TYPE_BLE_COMMON_READ_FILTER_ACCEPT_LIST_SIZE,

    /** @brief BLE Clear filter accept list command.
     *  @par   BLE_COMMON_EVT_CLEAR_FILTER_ACCEPT_LIST
     *
     *  @param none
     */
    TYPE_BLE_COMMON_CLEAR_FILTER_ACCEPT_LIST,

    /** @brief BLE Add device to filter accept list command.
     *  @par   BLE_COMMON_EVT_ADD_FILTER_ACCEPT_LIST
     *
     *  @param @ref ble_filter_accept_list_t
     */
    TYPE_BLE_COMMON_ADD_DEVICE_TO_FILTER_ACCEPT_LIST,

    /** @brief BLE Remove device from filter accept list command.
     *  @par   BLE_COMMON_EVT_REMOVE_FILTER_ACCEPT_LIST
     *
     *  @param @ref ble_filter_accept_list_t
     */
    TYPE_BLE_COMMON_REMOVE_DEVICE_FROM_FILTER_ACCEPT_LIST,

    /** @brief BLE Read antenna information command.
     *  @par   BLE_COMMON_EVT_READ_ANTENNA_INFO
     *
     *  @param none
     */
    TYPE_BLE_COMMON_ANTENNA_INFO_READ,


    //====================================================================
    //     BLE GAP API
    //====================================================================
    TYPE_BLE_GAP_API_START, //1

    /** @brief Get BLE device address and device address type.
     *  @par   BLE_GAP_EVT_GET_DEVICE_ADDR
     *
     *  @note  BLE Address : Little Endian and the length is @ref BLE_ADDR_LEN \n
     *         If Device BLE Address is set to "01:02:03:04:05:06", addrParam->addr[0] = 0x06
     *  @param @ref none
     */
    TYPE_BLE_GAP_DEVICE_ADDR_GET                = TYPE_BLE_GAP_API_START,

    /** @brief Set BLE device address and device address type.
     *  @par   BLE_GAP_EVT_SET_RANDOM_ADDR with parameter @ref ble_evt_gap_get_addr_t
     *
     *  @note  BLE Address : Little Endian and the length is @ref BLE_ADDR_LEN \n
     *         If Device BLE Address is set to "01:02:03:04:05:06", addrParam->addr[0] = 0x06
     *  @param @ref ble_gap_addr_t
     */
    TYPE_BLE_GAP_DEVICE_ADDR_SET,

    /** @brief BLE connection create command.
     *  @par   BLE_GAP_EVT_CONN_COMPLETE
     *
     *  @param @ref ble_gap_create_conn_param_t
     */
    TYPE_BLE_GAP_CONNECTION_CREATE,

    /** @brief BLE cancel create connection process command.
     *  @par   BLE_GAP_EVT_CONN_CANCEL
     *
     *  @param @ref none
     */
    TYPE_BLE_GAP_CONNECTION_CANCEL,

    /** @brief Terminate the BLE connection link.
     *  @par   BLE_GAP_EVT_DISCONN_COMPLETE
     *
     *  @param @ref ble_gap_conn_terminate_param_t
     */
    TYPE_BLE_GAP_CONNECTION_TERMINATE,

    /** @brief BLE connection parameter update.
     *  @par   BLE_GAP_EVT_CONN_PARAM_UPDATE
     *
     *  @param @ref ble_gap_conn_param_update_param_t
     */
    TYPE_BLE_GAP_CONNECTION_UPDATE,

    /** @brief BLE PHY update.
     *  @par   BLE_GAP_EVT_PHY_UPDATE
     *
     *  @param @ref ble_gap_phy_update_param_t
     */
    TYPE_BLE_GAP_PHY_UPDATE,

    /** @brief BLE read RSSI value.
     *  @par   BLE_GAP_EVT_PHY_READ
     *
     *  @param @ref ble_gap_phy_read_param_t
     */
    TYPE_BLE_GAP_PHY_READ,

    /** @brief BLE read RSSI value.
     *  @par   BLE_GAP_EVT_RSSI_READ
     *
     *  @param @ref ble_gap_rssi_read_param_t
     */
    TYPE_BLE_GAP_RSSI_READ,

    /** @brief BLE set le host channel classification.
     *  @par   BLE_GAP_EVT_SET_LE_HOST_CH_CLASSIFICATION
     *
     *  @param @ref ble_gap_host_ch_classif_t
     */
    TYPE_BLE_GAP_HOST_CHANNEL_CLASSIFICATION_SET,

    /** @brief BLE read channel map value.
     *  @par   BLE_GAP_EVT_READCHANNEL_MAP
     *
     *  @param @ref ble_gap_channel_map_read_t
     */
    TYPE_BLE_CHANNEL_MAP_READ,

    /** @brief BLE resolvable random address initialize.
     *  @par   none
     *
     *  @param none
     */
    TYPE_BLE_RESOLVABLE_ADDR_INIT,

    /** @brief BLE regenerate resolvable random address.
     *  @par   none
     *
     *  @param @ref ble_gap_regen_resol_addr_t
     */
    TYPE_BLE_GAP_REGEN_RESOLVABLE_ADDRESS,

    //====================================================================
    //     BLE ADV API
    //====================================================================
    TYPE_BLE_ADV_API_START,  //12

    /** @brief Set BLE advertising parameter.
     *  @par   BLE_ADV_EVT_SET_PARAM
     *
     *  @note  Advertising interval Min. and Max. : @ref ADV_INTERVAL_MIN to @ref ADV_INTERVAL_MAX \n
     *         Advertising interval Min. shall be less than or equal to advertising interval Max.
     *  @param @ref ble_adv_param_t
     */
    TYPE_BLE_ADV_PARAMETER_SET                  = TYPE_BLE_ADV_API_START,

    /** @brief Set BLE advertising data.
     *
     *  @par BLE_ADV_EVT_SET_DATA
     *
     *  @note BLE advertising data length shall be less than or equal to 31 bytes.
     *  @note AD Type please refer to @ref ble_adv_type_t
     *  @verbatim
        ||---------------------------------------------------------------------------------------------------------||
        ||                   <length octets = (1+slength_1)+...+(1+slength_N)  octets>                             ||
        ||                                    BLE advertising data                                                 ||
        ||---------------------------------------------------------------------------------------------------------||
        ||              AD Structure 1                 ||  ........ ||               AD Structure N                ||
        ||---------------------------------------------------------------------------------------------------------||
        ||<1 octet>||      <slength_1 octets>          ||           ||<1 octet>||    <slength_N octets>            ||
        ||slength_1||              Data                ||  ........ ||slength_N||              Data                ||
        ||---------------------------------------------------------------------------------------------------------||
        ||         ||<1 octets>||<(slength_1-1) octets>||           ||         ||<1 octets>||<(slength_N-1) octets>||
        ||         || AD Type  ||     AD Data          ||  ........ ||         || AD Type  ||     AD Data          ||
        ||---------------------------------------------------------------------------------------------------------||
        @endverbatim
     *
     *  @param @ref ble_adv_data_param_t
     */
    TYPE_BLE_ADV_DATA_SET,

    /** @brief Set BLE scan response data.
     *
     *  @par BLE_ADV_EVT_SET_SCAN_RSP
     *
     *  @note    BLE scan response data length shall be less than or equal to 31 bytes.
     *
     *  @note AD Type please refer to @ref ble_adv_type_t
     *  @verbatim
        ||---------------------------------------------------------------------------------------------------------||
        ||                   <length octets = (1+slength_1)+...+(1+slength_N)  octets>                             ||
        ||                                    BLE advertising data                                                 ||
        ||---------------------------------------------------------------------------------------------------------||
        ||              AD Structure 1                 ||  ........ ||               AD Structure N                ||
        ||---------------------------------------------------------------------------------------------------------||
        ||<1 octet>||      <slength_1 octets>          ||           ||<1 octet>||    <slength_N octets>            ||
        ||slength_1||              Data                ||  ........ ||slength_N||              Data                ||
        ||---------------------------------------------------------------------------------------------------------||
        ||         ||<1 octets>||<(slength_1-1) octets>||           ||         ||<1 octets>||<(slength_N-1) octets>||
        ||         || AD Type  ||     AD Data          ||  ........ ||         || AD Type  ||     AD Data          ||
        ||---------------------------------------------------------------------------------------------------------||
        @endverbatim
     *
     *  @param @ref ble_adv_data_param_t
     */
    TYPE_BLE_ADV_SCAN_RSP_SET,

    /** @brief Set BLE start advertising.
     *  @par   BLE_ADV_EVT_SET_ENABLE
     *
     *  @param @ref ble_adv_enable_param_t
     */
    TYPE_BLE_ADV_ENABLE,

    /** @brief Set BLE stop advertising.
     *  @par   BLE_ADV_EVT_SET_ENABLE
     *
     *  @param none
     */
    TYPE_BLE_ADV_DISABLE,

    //====================================================================
    //     BLE SCAN API
    //====================================================================
    TYPE_BLE_SCAN_API_START,  //17

    /** @brief Set BLE scan parameter.
     *  @par   BLE_SCAN_EVT_SET_PARAM
     *
     *  @note Scan window can only be less than or equal to the scan interval.
     *  @param @ref ble_scan_param_t
     */
    TYPE_BLE_SCAN_PARAMETER_SET                 = TYPE_BLE_SCAN_API_START,

    /** @brief Set BLE start Scanning.
     *  @par   BLE_SCAN_EVT_SET_ENABLE
     *
     *  @param none
     */
    TYPE_BLE_SCAN_ENABLE,

    /** @brief Set BLE stop Scanning.
     *  @par   BLE_SCAN_EVT_SET_ENABLE
     *
     *  @param none
     */
    TYPE_BLE_SCAN_DISABLE,

    //====================================================================
    //     BLE GATT API
    //====================================================================
    TYPE_BLE_GATT_API_START,  //20

    /** @brief Set suggested default data length.
     *  @par   BLE_ATT_GATT_EVT_WRITE_SUGGESTED_DEFAULT_DATA_LENGTH
     *
     *  @param @ref ble_gatt_suggested_data_len_param_t
     */
    TYPE_BLE_GATT_PREFERRED_DATA_LENGTH_SET     = TYPE_BLE_GATT_API_START,

    /** @brief Set preferred MTU size.
     *  @par   none
     *
     *  @param @ref ble_gatt_mtu_param_t
     */
    TYPE_BLE_GATT_PREFERRED_MTU_SET,

    /** @brief ATT_MTU exchange request.
     *  @par   BLE_ATT_GATT_EVT_MTU_EXCHANGE
     *
     *  @param @ref ble_gatt_mtu_param_t
     */
    TYPE_BLE_GATT_EXCHANGE_MTU_REQ,

    /** @brief Set data length update.
     *  @par   BLE_ATT_GATT_EVT_DATA_LENGTH_CHANGE
     *
     *  @param @ref ble_gatt_data_len_param_t
     */
    TYPE_BLE_GATT_DATA_LENGTH_UPDATE,

    /** @brief Get Get BLE GATT MTU Size
     *  @par   none
     *
     *  @param @ref ble_gatt_get_mtu_param_t
     *  @param[out] ble_gatt_get_mtu_param_t->mtu
     */
    TYPE_BLE_GATT_MTU_GET,

    /** @brief Get BLE GATT Attribute Handles Mapping Table
     *  @par   BLE_ATT_GATT_EVT_GET_ATT_HANDLES_TABLE_COMPLETE
     *
     *  @param[in] @ref ble_gatt_handle_table_param_t
     *  @param[out] ble_gatt_handle_table_param_t
     */
    TYPE_BLE_GATT_ATT_HANDLE_MAPPING_GET,

    /** @brief BLE GATT Read Response.
     *  @par   none
     *
     *  @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
     *  @note The read response is sent in reply to a received Read Request and contains the value of the attribute that has been read.
     *  @param @ref ble_gatt_data_param_t
     */
    TYPE_BLE_GATT_READ_RSP,

    /** @brief BLE GATT Read By Type Response.
     *  @par   none
     *
     *  @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
     *  @note The read by type response is sent in reply to a received Read By Type Request and contains the handle number and value of the attribute that has been read.
     *  @param @ref ble_gatt_data_param_t
     */
    TYPE_BLE_GATT_READ_BY_TYPE_RSP,

    /** @brief BLE GATT Read Blob Response.
     *  @par   none
     *
     *  @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
     *  @note The read blob response is sent in reply to a received Read Blob Request and contains the value of the attribute that has been read.
     *  @param @ref ble_gatt_data_param_t
     */
    TYPE_BLE_GATT_READ_BLOB_RSP,

    /** @brief BLE GATT Error Response.
     *  @par   none
     *
     *  @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
     *  @note The Error Response is used to state that a given request cannot be performed, and to provide the reason.
     *  @param @ref ble_gatt_err_rsp_param_t
     */
    TYPE_BLE_GATT_ERROR_RSP,

    /** @brief BLE GATT notification.
     *  @par   none
     *
     *  @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
     *  @note When a server is configured to notify a Characteristic Value to a client without the acknowledgment that the notification was successfully received.
     *  @param @ref ble_gatt_data_param_t
     */
    TYPE_BLE_GATT_NOTIFICATION,

    /** @brief BLE GATT indication.
     *  @par   none
     *
     *  @attention Only supported if GATT role is @ref BLE_GATT_ROLE_SERVER.
     *  @note When a server is configured to indicate a Characteristic Value to a client and expects the indication was successfully received.
     *  @param @ref ble_gatt_data_param_t
     */
    TYPE_BLE_GATT_INDICATION,

    /** @brief BLE GATT Write Request.
     *  @par   none
     *
     *  @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
     *  @note The Write Request is used to request the server to write the value of an attribute and acknowledge that this has been achieved in a Write Response. \n
     *        A Write Response shall be sent by the server if the write of the Characteristic Value succeeded.
     *  @param @ref ble_gatt_data_param_t
     */
    TYPE_BLE_GATT_WRITE_REQ,

    /** @brief BLE GATT Write Command.
     *  @par   none
     *
     *  @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
     *  @note Write a Characteristic Value to a server when the client knows the Characteristic Value Handle \n
     *        and the client does not need an acknowledgment that the write was successfully performed.
     *  @param @ref ble_gatt_data_param_t
     */
    TYPE_BLE_GATT_WRITE_CMD,

    /** @brief BLE GATT Read Request.
     *  @par   none
     *
     *  @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
     *  @note Read a Characteristic Value from a server when the client knows the Characteristic Value Handle.
     *
     *  @param @ref ble_gatt_read_req_param_t
     */
    TYPE_BLE_GATT_READ_REQ,

    /** @brief BLE GATT Read Blob Request.
     *  @par   none
     *
     *  @attention Only supported if GATT role is @ref BLE_GATT_ROLE_CLIENT.
     *  @note Read part of the value of an attribute at a given offset from a server when the client knows the Characteristic Value Handle.
     *
     *  @param @ref ble_gatt_read_blob_req_param_t
     */
    TYPE_BLE_GATT_READ_BLOB_REQ,

    //====================================================================
    //     BLE SM API
    //====================================================================
    TYPE_BLE_SM_API_START,

    /** @brief Set BLE bonding flags.
     *  @par   none
     *
     *  @param @ref ble_evt_sm_bonding_flag_t
     */
    TYPE_BLE_SM_BONDING_FLAG_SET                = TYPE_BLE_SM_API_START,

    /** @brief Restore BLE bonding CCCD.
     *  @par   none
     *
     *  @param @ref ble_sm_restore_cccd_param_t
     */
    TYPE_BLE_SM_CCCD_RESTORE,

    /** @brief Set BLE IO capabilities.
     *  @par   none
     *
     *  @param @ref ble_evt_sm_io_cap_t
     */
    TYPE_BLE_SM_IO_CAPABILITY_SET,

    /** @brief Set BLE pairing passkey value.
     *  @par   none
     *
     *  @param @ref ble_sm_passkey_param_t
     */
    TYPE_BLE_SM_PASSKEY_SET,

    /** @brief BLE send security request.
     *  @par   none
     *
     *  @param @ref ble_sm_security_request_param_t
     */
    TYPE_BLE_SM_SECURITY_REQ_SET,

    /** @brief BLE bonding space init.
     *  @par   none
     *
     *  @param none
     */
    TYPE_BLE_SM_BOND_SPACE_INIT,

    /** @brief BLE identity resolving key set.
     *  @par   none
     *
     *  @param ble_sm_irk_param_t
     */
    TYPE_BLE_SM_IDENTITY_RESOLVING_KEY_SET,

    //====================================================================
    //     BLE VENDOR API
    //====================================================================
    /** @brief BLE scan request report command.
     *  @par   none
     *
     *  @param ble_vendor_scan_req_rpt_t
     */
    TYPE_BLE_VENDOR_SCAN_REQUEST_REPORT,

    //====================================================================
    //     BLE PRIVACY API
    //====================================================================
    /** @brief BLE set Privacy enable command.
     *  @par   none
     *
     *  @param none
     */
    TYPE_BLE_PRIVACY_ENABLE,

    /** @brief BLE set Privacy disable command.
     *  @par   none
     *
     *  @param none
     */
    TYPE_BLE_PRIVACY_DISABLE,


    //====================================================================
    //     BLE CONN CTE API
    //====================================================================
    /** @brief BLE set connection cte rx parameters.
     *  @par   BLE_CTE_EVT_SET_CONN_CTE_RX_PARAM
     *
     *  @param @ref ble_gap_connection_cte_rx_param_t
     */
    TYPE_BLE_CTE_CONNECTION_CTE_RX_PARAMETERS_SET,

    /** @brief BLE set connection cte tx parameters.
     *  @par   BLE_CTE_EVT_SET_CONN_CTE_TX_PARAM
     *
     *  @param @ref ble_gap_connection_cte_tx_param_t
     */
    TYPE_BLE_CTE_CONNECTION_CTE_TX_PARAMETERS_SET,

    /** @brief BLE set connection cte request enable.
     *  @par   BLE_CTE_EVT_SET_CONN_CTE_REQ
     *
     *  @param @ref ble_gap_connection_cte_req_enable_t
     */
    TYPE_BLE_CTE_CONNECTION_CTE_REQ_SET,

    /** @brief BLE set connection cte response enable.
     *  @par   BLE_CTE_EVT_SET_CONN_CTE_RSP
     *
     *  @param @ref ble_gap_connection_cte_rsp_enable_t
     */
    TYPE_BLE_CTE_CONNECTION_CTE_RSP_SET,

    //====================================================================
    //     END
    //====================================================================
    TYPE_BLE_API_MAX,

} ble_api_type_t;


typedef enum
{
    BLE_APP_RETURN_PARAMETER_EVENT,   /**< Queue type: BLE command request. */
    BLE_APP_GENERAL_EVENT,            /**< Queue type: BLE event. */
    BLE_APP_SERVICE_EVENT,            /**< Queue type: BLE service data. */
} ble_event_t;


typedef struct
{
    pf_evt_indication   *pf_evt_indication;    /**< a pointer to the evt indication callback function. */
} ble_cfg_t;


typedef struct
{
    uint16_t    type;       /**< Payload Identifier : defined by each module */
    uint16_t    length;     /**< Payload Length : the length of the payload data */
    uint8_t     value[];    /**< Payload Data */
} ble_tlv_t;


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

/** @brief Set BLE host stack initialization.
 *
 * @ingroup ble_api
 *
 * @param[in] pt_cfg : a pointer to the event indication callback function.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
int ble_host_stack_init(ble_cfg_t *pt_cfg);


/** @brief Set to BLE host stack delete.
 *
 * @ingroup ble_api
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
int ble_host_stack_deinit(void);


/** @brief BLE event message receive function.
 *
 * @ingroup ble_api
 *
 * @param[in] pu8_buf : a pointer to the received data buffer.
 * @param[in] pu32_buf_len : a pointer to the data length.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
int ble_event_msg_recvfrom(uint8_t *pu8_buf, uint32_t *pu32_buf_len);


/** @brief BLE cmd send to function.
 *
 * @ingroup ble_api
 *
 * @param[in] pt_tlv : a pointer to ble command message.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
int ble_event_msg_sendto(ble_tlv_t *pt_tlv);


#ifdef __cplusplus
};
#endif

#endif /* __BLE_API_H__*/
