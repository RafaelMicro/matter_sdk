/** @file ble_connect_cte.h
 *
 *  * @brief BLE CONN CTE header file.
 *
 */

/**
 * @defgroup ble_cte Generic access profile
 * @ingroup BLE_group
 * @{
 * @brief Define BLE CONN CTE definitions, structures, and functions.
 * @}
 */

#ifndef __BLE_CONNECT_CTE_H__
#define __BLE_CONNECT_CTE_H__

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


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/** @brief BLE Set connection CTE receive parameter.
 * @ingroup ble_cte
*/
typedef struct ble_connection_cte_rx_param_s
{
    uint8_t                 host_id;          /**< Host id. */
    uint8_t                 sampling_enable;  /**< Connection IQ sampling is enable or not setting. */
    uint8_t                 slot_durations;   /**< Switching and sampling slots. */
    uint8_t                 sw_pattern_length;/**< The number of Antenna IDs in the pattern. */
    uint8_t                 antenna_ids[];    /**< List of Antenna IDs in the pattern. */
} ble_connection_cte_rx_param_t;


/** @brief BLE Set connection CTE transmit parameter.
 * @ingroup ble_cte
*/
typedef struct ble_connection_cte_tx_param_s
{
    uint8_t                 host_id;          /**< Host id. */
    uint8_t                 cte_types;        /**< type of Constant Tone Extension. */
    uint8_t                 sw_pattern_length;/**< The number of Antenna IDs in the pattern. */
    uint8_t                 antenna_ids[];    /**< List of Antenna IDs in the pattern. */
} ble_connection_cte_tx_param_t;


/** @brief BLE Set connection cte request enable.
 * @ingroup ble_cte
*/
typedef struct ble_connection_cte_req_enable_s
{
    uint8_t                 host_id;          /**< Host id. */
    uint8_t                 enable;           /**< CTE request setting. */
    uint16_t                cte_req_interval; /**< The Constant Tone Extension Request procedure is initiated only once or periodically.. */
    uint8_t                 req_cte_length;   /**< The minimum length of the Constant Tone Extension. */
    uint8_t                 req_cte_type;     /**< The type of Constant Tone Extension that the Controller shall request from the remote device. */
} ble_connection_cte_req_enable_t;


/** @brief BLE Set connection CTE response parameter.
 * @ingroup ble_cte
*/
typedef struct ble_connection_cte_rsp_enable_s
{
    uint8_t                 host_id;          /**< Host id. */
    uint8_t                 enable;           /**< CTE response setting. */
} ble_connection_cte_rsp_enable_t;


/**
 * @brief Set CTE RX Event Parameters.
 * @ingroup ble_cte
*/
typedef struct __attribute__((packed)) ble_evt_conn_cte_rx_param_s
{
    uint8_t                 host_id;         /**< Host id. */
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_evt_conn_cte_rx_param_t;


/**
 * @brief Set CTE TX Map Event Parameters.
 * @ingroup ble_cte
*/
typedef struct __attribute__((packed)) ble_evt_conn_cte_tx_param_s
{
    uint8_t                 host_id;         /**< Host id. */
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_evt_conn_cte_tx_param_t;


/**
 * @brief Set CTE REQ Event Parameters.
 * @ingroup ble_cte
*/
typedef struct __attribute__((packed)) ble_evt_conn_cte_req_s
{
    uint8_t                 host_id;         /**< Host id. */
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_evt_conn_cte_req_t;


/**
 * @brief Set CTE RSP Event Parameters.
 * @ingroup ble_cte
*/
typedef struct __attribute__((packed)) ble_evt_conn_cte_rsp_s
{
    uint8_t                 host_id;         /**< Host id. */
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_evt_conn_cte_rsp_t;


/**
 * @brief Set CTE RSP Event Parameters.
 * @ingroup ble_cte
*/
typedef struct __attribute__((packed)) ble_evt_conn_iq_report_s
{
    uint8_t                 host_id;        /**< Host id. */
    uint8_t                 rx_phy;         /**< Receiver PHY. */
    uint8_t                 data_ch_idx;    /**< The index of the data channel. */
    int16_t                 rssi;           /**< RSSI of packet. */
    uint8_t                 rssi_antenna_id;    /**< ID of the antenna on which the rssi. */
    uint8_t                 cte_type;       /**< CTE Type. */
    uint8_t                 slot_durations; /**< Switching and sampling slots. */
    uint8_t                 packet_status;  /**< Packet status. */
    uint16_t                conn_evt_cnt;   /**< the value of connection event counter. */
    uint8_t                 sample_cnt;     /**< Sample counter. */
    uint8_t                 sample_iq[];    /**< pointer of iq value. */
} ble_evt_conn_iq_report_t;


/**
 * @brief CTE REQ Failed Event Parameters.
 * @ingroup ble_cte
*/
typedef struct __attribute__((packed)) ble_evt_cte_req_failed_s
{
    uint8_t                 host_id;        /**< Host id. */
    uint8_t                 status;          /**< @ref hci_cmd_param_error_code "ble_hci_error_code_t". */
} ble_evt_cte_req_failed_t;


/**
 * @brief BLE CONN CTE Event Parameters.
 * @ingroup ble_cte
*/
typedef struct ble_evt_cte_s
{
    union
    {
        ble_evt_conn_cte_rx_param_t      evt_conn_cte_rx_param; /**< BLE set connection cte rx parameters event. */
        ble_evt_conn_cte_tx_param_t      evt_conn_cte_tx_param; /**< BLE set connection cte tx parameters event. */
        ble_evt_conn_cte_req_t           evt_conn_cte_req;      /**< BLE set connection cte request event. */
        ble_evt_conn_cte_rsp_t           evt_conn_cte_rsp;      /**< BLE set connection cte response event. */
        ble_evt_conn_iq_report_t         evt_conn_iq_report;    /**< BLE connection IQ report event. */
        ble_evt_cte_req_failed_t         evt_cte_req_failed;    /**< BLE CTE request failed event. */
    } param;                                                    /**< Event parameters. */
} ble_evt_cte_t;

/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

/** @brief BLE Set connection CTE receiver parameters.
 *
 * @ingroup ble_cte
 *
 * @param[in] p_param : a pointer of the CTE receiver parameters structure.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_connection_cte_receive_parameters_set(ble_connection_cte_rx_param_t *p_param);


/** @brief BLE Set connection CTE transmit parameters.
 *
 * @ingroup ble_cte
 *
 * @param[in] p_param : a pointer of the CTE transmit parameters structure.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_connection_cte_transmit_parameters_set(ble_connection_cte_tx_param_t *p_param);


/** @brief BLE Set connection CTE request enable.
 *
 * @ingroup ble_cte
 *
 * @param[in] p_param : a pointer of the CTE request enable structure.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_connection_cte_request_enable(ble_connection_cte_req_enable_t *p_param);

/** @brief BLE Set connection CTE response enable.
 *
 * @ingroup ble_cte
 *
 * @param[in] p_param : a pointer of the CTE response enable structure.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_connection_cte_response_enable(ble_connection_cte_rsp_enable_t *p_param);


#ifdef __cplusplus
};
#endif

#endif /* __BLE_CONNECT_CTE_H__*/

