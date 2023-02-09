/** @file
 *
 * @brief BLE advertising header file.
 *
 */

/**
 * @defgroup ble_advertising Advertising
 * @ingroup BLE_group
 * @{
 * @brief Define BLE Advertising definitions, structures, and functions.
 * @}
 */

#ifndef __BLE_ADVERTISING_H__
#define __BLE_ADVERTISING_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "ble_gap.h"
#include "ble_api.h"


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define BLE_ADV_DATA_SIZE_MAX         31    /*!< Maximun data size of advertising data.*/


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**
 * @defgroup ble_adv_type BLE Advertising Type Definition
 * @{
 * @ingroup ble_advertising
 * @brief  Define advertising type.
 */
typedef uint8_t ble_adv_type_t;
#define ADV_TYPE_ADV_IND              0x00  /**< Connectable and scannable undirected advertising. */
#define ADV_TYPE_ADV_DIRECT_IND       0x01  /**< Connectable directed advertising. */
#define ADV_TYPE_SCAN_IND             0x02  /**< Scanable undirected advertising. */
#define ADV_TYPE_ADV_NONCONN_IND      0x03  /**< Non-Connectable undirected advertising. */
#define ADV_TYPE_SCAN_RSP             0x04  /**< Scan response. */
/** @} */


/**
 * @defgroup ble_adv_int BLE Advertising Interval Minimum and Maximum Definition
 * @{
 * @ingroup ble_advertising
 * @brief  Define advertising interval range.
 */
typedef uint8_t ble_adv_interval_range_t;

/** The unit of advertising interval is 0.625ms \n
 * Minimum advertising interval is 20ms = (0x20 * 0.625) ms
*/
#define ADV_INTERVAL_MIN              0x0020

/** The unit of advertising interval is 0.625ms \n
 * Maximum advertising interval is 10.24s = (0x4000 * 0.625) ms
*/
#define ADV_INTERVAL_MAX              0x4000
/** @} */


/**
 * @defgroup ble_adv_channel BLE Advertising Channel Definition
 * @{
 * @ingroup ble_advertising
 * @brief Define BLE advertising channel.
 */
typedef uint8_t ble_adv_channel_t;

/** Channel 37 */
#define ADV_CHANNEL_37         0x01

/** Channel 38 */
#define ADV_CHANNEL_38         0x02

/** Channel 39 */
#define ADV_CHANNEL_39         0x04

/** Channel 37 and Channel 38 */
#define ADV_CHANNEL_37_38      (ADV_CHANNEL_37 | ADV_CHANNEL_38)

/** Channel 37 and Channel 39 */
#define ADV_CHANNEL_37_39      (ADV_CHANNEL_37 | ADV_CHANNEL_39)

/** Channel 38 and Channel 39 */
#define ADV_CHANNEL_38_39      (ADV_CHANNEL_38 | ADV_CHANNEL_39)

/** Channel 37 and Channel 38 and channel 39 */
#define ADV_CHANNEL_ALL        (ADV_CHANNEL_37 | ADV_CHANNEL_38 | ADV_CHANNEL_39)
/** @} */


/**
 * @defgroup ble_adv_filter BLE Advertising Filter Policy Definition
 * @{
 * @ingroup ble_advertising
 * @brief Define BLE advertising filter policy.
 */
typedef uint8_t ble_adv_filter_t;

/** Process scan and connection requests from all devices (i.e., the Filter Accept List is not in use). */
#define ADV_FILTER_POLICY_ACCEPT_ALL                      0x00

/** Process connection requests from all devices and scan requests only from devices that are in the Filter Accept List. */
#define ADV_FILTER_POLICY_ACCEPT_SCAN_REQ_FROM_FAL        0x01

/** Process scan requests from all devices and connection requests only from devices that are in the Filter Accept List.  */
#define ADV_FILTER_POLICY_ACCEPT_CONN_REQ_FROM_FAL        0x02

/** Process scan and connection requests only from devices in the Filter Accept List. */
#define ADV_FILTER_POLICY_ACCEPT_SCAN_CONN_REQ_FROM_FAL   0x03
/** @} */


/** @brief BLE advertising parameters.
 * @ingroup ble_advertising
*/
typedef struct
{
    /**
     * @ref ble_adv_type "BLE advertising type"
     */
    ble_adv_type_t      adv_type;

    /**
     * @ref ble_addr_type_t "BLE GAP Address Type"
     */
    ble_addr_type_t     own_addr_type;

    /** Minimum advertising interval. \n
      *  @note Advertising interval = value * 0.625ms \n
      *        Advertising interval range = @ref ADV_INTERVAL_MIN ~ @ref ADV_INTERVAL_MAX
      */
    uint16_t            adv_interval_min;

    /** Maximum advertising  interval.
      *  @note Advertising interval = value * 0.625ms \n
      *        Advertising interval range = @ref ADV_INTERVAL_MIN ~ @ref ADV_INTERVAL_MAX
      */
    uint16_t            adv_interval_max;


    /** The BLE address and BLE address type for directed advertising
     * @note MUST set advDirectAddrParam if BLE advertising type is set to @ref ADV_TYPE_ADV_DIRECT_IND.
    */
    ble_gap_peer_addr_t adv_peer_addr_param;

    /**
     * @ref ble_adv_channel "Advertising channel map"
     */
    ble_adv_channel_t   adv_channel_map;

    /**
     * @ref ble_adv_filter "Advertising filter policy"
     */
    ble_adv_filter_t    adv_filter_policy;

} ble_adv_param_t;


/** @brief BLE advertising data parameters.
 * @ingroup ble_advertising
*/
typedef struct
{
    /** The BLE advertising data length. \n
     * @note Advertising data length range = 0 ~ @ref BLE_ADV_DATA_SIZE_MAX
    */
    uint8_t     length;

    /** The BLE advertising data array. \n
    */
    uint8_t     data[BLE_ADV_DATA_SIZE_MAX];

} ble_adv_data_param_t;


/** @brief BLE advertising enable parameters.
 * @ingroup ble_advertising
*/
typedef struct
{
    /** The link's host ID or set to @ref BLE_HOSTID_RESERVED to enable @ref ADV_TYPE_ADV_NONCONN_IND or @ref ADV_TYPE_SCAN_IND advertisement. */
    uint8_t     host_id;

} ble_adv_enable_param_t;


/**
 * @brief Advertising Parameter Event Parameters.
 * @ingroup ble_advertising
*/
typedef struct __attribute__((packed)) ble_evt_adv_set_adv_param_s
{
    /** @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t     status;

} ble_evt_adv_set_adv_param_t;


/**
 * @brief Advertising Data Event Parameters.
 * @ingroup ble_advertising
*/
typedef struct __attribute__((packed)) ble_evt_adv_set_adv_data_s
{
    /** @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t     status;

} ble_evt_adv_set_adv_data_t;


/**
 * @brief Scan Response Data Event Parameters.
 * @ingroup ble_advertising
*/
typedef struct __attribute__((packed)) ble_evt_adv_set_scan_rsp_s
{
    /** @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t     status;

} ble_evt_adv_set_scan_rsp_t;


/**
 * @brief Advertising Enable Event Parameters.
 * @ingroup ble_advertising
*/
typedef struct __attribute__((packed)) ble_evt_adv_set_adv_enable_s
{
    /** @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    uint8_t     status;

    /** false : disabled, true: enabled. */
    bool        adv_enabled;

} ble_evt_adv_set_adv_enable_t;


/**
 * @brief BLE Advertising Event Parameters.
 * @ingroup ble_advertising
*/
typedef struct
{
    union
    {
        /** Set advertising parameter event parameters. */
        ble_evt_adv_set_adv_param_t   evt_set_adv_param;

        /** Set advertising data event parameters. */
        ble_evt_adv_set_adv_data_t    evt_set_adv_data;

        /** Set advertising scan response data event parameters. */
        ble_evt_adv_set_scan_rsp_t    evt_set_scan_rsp;

        /** Set advertising enable event parameters. */
        ble_evt_adv_set_adv_enable_t  evt_set_adv_enable;

    } param;/** Event parameters. */

} ble_evt_adv_t;


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

/** @brief Set BLE advertising parameters.
 *
 * @ingroup ble_advertising
 *
 * @note    Advertising interval Min. and Max. : @ref ADV_INTERVAL_MIN to @ref ADV_INTERVAL_MAX \n
 *          Advertising interval Min. shall be less than or equal to advertising interval Max.
 *
 * @param[in] p_param : a pointer to advertising parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_adv_param_set(ble_adv_param_t *p_param);


/** @brief Set BLE advertising data.
 *
 * @ingroup ble_advertising
 *
 * @note BLE advertising data length shall be less than or equal to 31 bytes.
 * @note AD Type please refer to @ref ble_adv_type_t
 * @verbatim
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
 * @param[in] p_param : a pointer to the advertising data parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_adv_data_set(ble_adv_data_param_t *p_param);


/** @brief Set BLE scan response data.
 *
 * @ingroup ble_advertising
 *
 * @note    BLE scan response data length shall be less than or equal to 31 bytes.
 *
 * @note AD Type please refer to @ref ble_adv_type_t
 * @verbatim
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
 * @param[in] p_param : a pointer to the scan response data parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_adv_scan_rsp_set(ble_adv_data_param_t *p_param);


/** @brief Set BLE start advertising.
 *
 * @ingroup ble_advertising
 *
 * @par BLE Event
 * Wait for @ref BLE_ADV_EVT_SET_ENABLE event which indicates the advertisement has been started.
 *
 * @param[in] host_id : he link's host ID
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_adv_enable(uint8_t host_id);


/** @brief Set BLE stop advertising.
 *
 * @ingroup ble_advertising
 *
 * @par BLE Event
 * Wait for @ref BLE_ADV_EVT_SET_ENABLE event which indicates the advertisement has been stopped.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_adv_disable(void);


#ifdef __cplusplus
};
#endif

#endif /* __BLE_ADVERTISING_H__*/

