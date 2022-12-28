/** @file
 *
 * @brief BLE scan header file.
 *
 */

/**
 * @defgroup ble_scan SCAN
 * @ingroup BLE_group
 * @{
 * @brief Define BLE Advertising definitions, structures, and functions.
 * @}
 */

#ifndef __BLE_SCAN_H__
#define __BLE_SCAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include "ble_api.h"
#include "ble_advertising.h"

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**
 * @defgroup ble_scan_type BLE Scan Type Definition
 * @{
 * @ingroup ble_scan
 * @details BLE scan type definition.
 */
typedef uint8_t ble_scan_type_t;
#define SCAN_TYPE_PASSIVE             0x00  /**< Passive scanning. */
#define SCAN_TYPE_ACTIVE              0x01  /**< Active scanning. */
/** @} */


/**
 * @defgroup ble_scan_int BLE Scan Interval Minimum and Maximum Definition
 * @{
 * @ingroup ble_scan
 * @details BLE scan interval range definition.
 */
typedef uint8_t ble_scan_interval_range_t;

/** The unit of scan interval is 0.625ms \n
 * Minimum scan interval is 2.5ms = (0x0004 * 0.625) ms
*/
#define SCAN_INTERVAL_MIN             0x0004

/** The unit of scan interval is 0.625ms \n
 * Maximum scan interval is 10.24s = (0x4000 * 0.625) ms
*/
#define SCAN_INTERVAL_MAX             0x4000
/** @} */


/**
 * @defgroup ble_scan_window BLE Scan Window Minimum and Maximum Definition
 * @{
 * @ingroup ble_scan
 * @details BLE scan window range definition.
 */
typedef uint8_t ble_scan_window_range_t;

/** The unit of scan window is 0.625ms \n
 * Minimum scan window is 2.5ms = (0x0004 * 0.625) ms
*/
#define SCAN_WINDOW_MIN               0x0004

/** The unit of scan window is 0.625ms \n
 * Maximum scan window is 10.24s = (0x4000 * 0.625) ms
*/
#define SCAN_WINDOW_MAX               0x4000
/** @} */


/**
 * @defgroup ble_scan_filter BLE Scan Filter Policy Definition
 * @{
 * @ingroup ble_scan
 * @details BLE scan filter policy definition.
 */
typedef uint8_t ble_scan_filter_t;

/** The Link Layer shall process all advertising and scan response PDUs (i.e., the Filter Accept List is not used),
    and a directed advertising PDU shall be ignored. */
#define SCAN_FILTER_POLICY_BASIC_UNFILTERED         0x00

/** The Link Layer shall process advertising and scan response PDUs only from devices in the Filter Accept List,
    and a directed advertising PDU shall be ignored. */
#define SCAN_FILTER_POLICY_BASIC_FILTERED           0x01

/** The Link Layer shall process all advertising and scan response PDUs (i.e., the Filter Accept List is not used),
    and the TargetA field is identical to the scanner's device address,
    or the TargetA field is a resolvable private address, address resolution is enabled, and the address is resolved successfully. */
#define SCAN_FILTER_POLICY_EXTENED_UNFILTERED       0x02

/** The Link Layer shall process advertising and scan response PDUs only from devices in the Filter Accept List,
    and the TargetA field is identical to the scanner's device address,
    or the TargetA field is a resolvable private address, address resolution is enabled, and the address is resolved successfully. */
#define SCAN_FILTER_POLICY_EXTENED_FILTERED         0x03
/** @} */


/** @brief BLE scan parameter.
 * @ingroup ble_scan
*/
typedef struct ble_scan_param_s
{
    /**
     * BLE scan type, refer to @ref ble_scan_type "ble_scan_type_t"
     */
    ble_scan_type_t       scan_type;

    /**
     * @ref ble_addr_type_t "BLE Address Type"
     */
    ble_addr_type_t       own_addr_type;

    /** Scan interval.
     *  @note Scan interval = value * 0.625ms \n
     *        Scan interval range = @ref SCAN_INTERVAL_MIN ~ @ref SCAN_INTERVAL_MAX
     */
    uint16_t              scan_interval;

    /** Scan window.
     *  @note Scan window = value * 0.625ms \n
     *        Scan window range = @ref SCAN_WINDOW_MIN  ~ @ref SCAN_WINDOW_MAX
     */
    uint16_t              scan_window;

    /**
     * BLE scan filter policy, refer to @ref ble_scan_filter "ble_scan_filter_t"
     */
    ble_scan_filter_t     scan_filter_policy;

} ble_scan_param_t;

/**
 * @brief Set Scan Parameter Event Parameters.
 * @ingroup ble_scan
*/
typedef struct __attribute__((packed)) ble_evt_scan_set_scan_param_s
{
    uint8_t             status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */

} ble_evt_scan_set_scan_param_t;


/**
 * @brief Advertising Report Event Parameters.
 * @ingroup ble_scan
*/
typedef struct __attribute__((packed)) ble_evt_scan_adv_report_s
{
    ble_adv_type_t        adv_type;               /**< @ref ble_adv_type "BLE advertising type". */
    ble_gap_peer_addr_t   peer_addr;              /**< Remote device address type and address. */
    uint8_t               length;                 /**< Received advertising or scan response data length. */
    uint8_t               data[31];               /**< Received advertising or scan response data. */
    int8_t                rssi;                   /**< Received Signal Strength Indication in dBm. */
} ble_evt_scan_adv_report_t;


/**
 * @brief Set Scan Enable Event Parameters.
 * @ingroup ble_scan
*/
typedef struct __attribute__((packed)) ble_evt_scan_set_scan_enable_s
{
    uint8_t             status;                 /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
    bool                scan_enabled;           /**< false : disabled, true: enabled. */
} ble_evt_scan_set_scan_enable_t;


/**
 * @brief BLE Scan Event Parameters.
 * @ingroup ble_scan
*/
typedef struct ble_evt_scan_s
{
    union
    {
        ble_evt_scan_set_scan_param_t   evt_set_scan_param;   /**< Set scan parameter event parameters. */
        ble_evt_scan_set_scan_enable_t  evt_set_scan_enable;  /**< Set scan enable event parameters. */
        ble_evt_scan_adv_report_t       evt_adv_report;       /**< Advertising report event parameters. */
    } param;                                                  /**< Event parameters. */
} ble_evt_scan_t;


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

/** @brief Set BLE scan parameter.
 *
 * @ingroup ble_scan
 *
 * @note Scan window can only be less than or equal to the scan interval.
 *
 * @param[in] p_param : a pointer to scanning parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_scan_param_set(ble_scan_param_t *p_param);


/** @brief Set BLE start Scanning.
 *
 * @ingroup ble_scan
 *
 * @note    When using the SCAN function for multi-link, ensure that the current connection interval is greater than 10ms x number of links.
 *
 * @par BLE Event
 * Wait for @ref BLECMD_EVENT_SCAN_COMPLETE event which indicates the scan has been started. \n
 * Wait for @ref BLECMD_EVENT_SCAN_REPORT event to receive scanned devices information.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_scan_enable(void);


/** @brief Set BLE stop scanning.
 *
 * @ingroup ble_scan
 *
 * @par BLE Event
 * Wait for @ref BLECMD_EVENT_SCAN_COMPLETE event which indicates the scan has been stopped.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_scan_disable(void);


/** @brief Get BLE advertising parsing data by advertising data type.
 *
 * @ingroup ble_scan
 *
 * @param[in]  p_rpt_data     : a pointer to scanned advertising data to parse.
 * @param[in]  ad_type        : parsing advertising type.
 * @param[out] p_data         : a pointer to the parsing data.
 * @param[out] p_data_length  : a pointer to the length of parsing data.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_scan_report_adv_data_parsing(ble_evt_scan_adv_report_t *p_rpt_data, ble_gap_ad_type_t ad_type, uint8_t *p_data, uint8_t *p_data_length);


#ifdef __cplusplus
};
#endif


#endif /* __BLE_SCAN_H__*/

