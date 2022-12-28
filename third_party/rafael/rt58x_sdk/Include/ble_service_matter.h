#ifndef _BLE_SERVICE_MATTER_H_
#define _BLE_SERVICE_MATTER_H_

#ifdef __cplusplus
extern "C" {
#endif
/**************************************************************************//**
 * @file  ble_service_matter.h
 * @brief Provide the Definition of MATTER.
*****************************************************************************/

#include "ble_service_common.h"
#include "ble_att_gatt.h"


/**************************************************************************
 * MATTER Definitions
 **************************************************************************/
/** @defgroup service_matter_def BLE MATTER Definitions
 * @{
 * @ingroup service_def
 * @details Here shows the definitions of the MATTER.
 * @}
**************************************************************************/

/**
 * @ingroup service_matter_def
 * @defgroup service_matter_UUIDDef BLE MATTER UUID Definitions
 * @{
 * @details Here shows the definitions of the BLE MATTER UUID Definitions.
*/
extern const uint16_t attr_uuid_matter_primary_service[];                                /**< MATTER service UUID. */
extern const uint16_t attr_uuid_matter_charc_client_tx_buffer[];                         /**< MATTER characteristic CLIENT_TX_BUFFER UUID. */
extern const uint16_t attr_uuid_matter_charc_client_rx_buffer[];                         /**< MATTER characteristic CLIENT_RX_BUFFER UUID. */
extern const uint16_t attr_uuid_matter_charc_additional_commissioning_related_data[];    /**< MATTER characteristic ADDITIONAL_COMMISSIONING_RELATED_DATA UUID. */
/** @} */

/**
 * @defgroup service_matter_ServiceChardef BLE MATTER Service and Characteristic Definitions
 * @{
 * @ingroup service_matter_def
 * @details Here shows the definitions of the MATTER service and characteristic.
 * @}
*/

/**
 * @ingroup service_matter_ServiceChardef
 * @{
*/
extern const ble_att_param_t att_matter_primary_service;                                         /**< MATTER primary service. */
extern const ble_att_param_t att_matter_characteristic_client_tx_buffer;                         /**< MATTER characteristic CLIENT_TX_BUFFER. */
extern const ble_att_param_t att_matter_client_tx_buffer;                                        /**< MATTER CLIENT_TX_BUFFER value. */
extern const ble_att_param_t att_matter_characteristic_client_rx_buffer;                         /**< MATTER characteristic CLIENT_RX_BUFFER. */
extern const ble_att_param_t att_matter_client_rx_buffer;                                        /**< MATTER CLIENT_RX_BUFFER value. */
extern const ble_att_param_t att_matter_client_rx_buffer_client_charc_configuration;             /**< MATTER CLIENT_RX_BUFFER client characteristic configuration. */
extern const ble_att_param_t att_matter_characteristic_additional_commissioning_related_data;    /**< MATTER characteristic ADDITIONAL_COMMISSIONING_RELATED_DATA. */
extern const ble_att_param_t att_matter_additional_commissioning_related_data;                   /**< MATTER ADDITIONAL_COMMISSIONING_RELATED_DATA value. */
/** @} */


/** MATTER Definition
 * @ingroup service_matter_ServiceChardef
*/
#define ATT_MATTER_SERVICE                                                     \
    &att_matter_primary_service,                                               \
    &att_matter_characteristic_client_tx_buffer,                               \
    &att_matter_client_tx_buffer,                                              \
    &att_matter_characteristic_client_rx_buffer,                               \
    &att_matter_client_rx_buffer,                                              \
    &att_matter_client_rx_buffer_client_charc_configuration,                   \
    &att_matter_characteristic_additional_commissioning_related_data,          \
    &att_matter_additional_commissioning_related_data                          \


/**************************************************************************
 * MATTER Application Definitions
 **************************************************************************/
/** @defgroup app_matter_def BLE MATTER Application Definitions
 * @{
 * @ingroup service_matter_def
 * @details Here shows the definitions of the MATTER for application.
 * @}
**************************************************************************/

/**
 * @ingroup app_matter_def
 * @defgroup app_matter_eventDef BLE MATTER Service and Characteristic Definitions
 * @{
 * @details Here shows the event definitions of the MATTER.
*/
#define BLESERVICE_MATTER_CLIENT_TX_BUFFER_WRITE_EVENT                             0x01     /**< MATTER characteristic CLIENT_TX_BUFFER write event.*/
#define BLESERVICE_MATTER_CLIENT_TX_BUFFER_WRITE_RSP_EVENT                         0x02     /**< MATTER characteristic CLIENT_TX_BUFFER write response event.*/
#define BLESERVICE_MATTER_CLIENT_RX_BUFFER_INDICATE_CONFIRM_EVENT                  0x03     /**< MATTER characteristic CLIENT_RX_BUFFER indicate confirm event.*/
#define BLESERVICE_MATTER_CLIENT_RX_BUFFER_INDICATE_EVENT                          0x04     /**< MATTER characteristic CLIENT_RX_BUFFER indicate event.*/
#define BLESERVICE_MATTER_CLIENT_RX_BUFFER_CCCD_READ_EVENT                         0x05     /**< MATTER characteristic CLIENT_RX_BUFFER cccd read event.*/
#define BLESERVICE_MATTER_CLIENT_RX_BUFFER_CCCD_READ_RSP_EVENT                     0x06     /**< MATTER characteristic CLIENT_RX_BUFFER cccd read response event.*/
#define BLESERVICE_MATTER_CLIENT_RX_BUFFER_CCCD_WRITE_EVENT                        0x07     /**< MATTER characteristic CLIENT_RX_BUFFER cccd write event.*/
#define BLESERVICE_MATTER_CLIENT_RX_BUFFER_CCCD_WRITE_RSP_EVENT                    0x08     /**< MATTER characteristic CLIENT_RX_BUFFER cccd write response event.*/
#define BLESERVICE_MATTER_ADDITIONAL_COMMISSIONING_RELATED_DATA_READ_EVENT         0x09     /**< MATTER characteristic ADDITIONAL_COMMISSIONING_RELATED_DATA read event.*/
#define BLESERVICE_MATTER_ADDITIONAL_COMMISSIONING_RELATED_DATA_READ_RSP_EVENT     0x0a     /**< MATTER characteristic ADDITIONAL_COMMISSIONING_RELATED_DATA read response event.*/
/** @} */


/**
 * @ingroup app_matter_def
 * @defgroup app_matter_structureDef BLE MATTER Structure Definitions
 * @{
 * @details Here shows the structure definitions of the MATTER.
 * @}
*/

/** MATTER Handles Definition
 * @ingroup app_matter_structureDef
*/
typedef struct ble_svcs_matter_handles_s
{
    uint16_t hdl_client_tx_buffer;                         /**< Handle of CLIENT_TX_BUFFER. */
    uint16_t hdl_client_rx_buffer;                         /**< Handle of CLIENT_RX_BUFFER. */
    uint16_t hdl_client_rx_buffer_cccd;                    /**< Handle of CLIENT_RX_BUFFER client characteristic configuration. */
    uint16_t hdl_additional_commissioning_related_data;    /**< Handle of ADDITIONAL_COMMISSIONING_RELATED_DATA. */
} ble_svcs_matter_handles_t;


/** MATTER Data Definition
 * @ingroup app_matter_structureDef
 * @note User defined.
*/
typedef struct ble_svcs_matter_data_s
{
    uint16_t client_rx_buffer_cccd;    /**< CLIENT_RX_BUFFER cccd value */
} ble_svcs_matter_data_t;


/** MATTER Application Data Structure Definition
 * @ingroup app_matter_structureDef
*/
typedef struct ble_svcs_matter_subinfo_s
{
    ble_svcs_matter_handles_t handles;    /**< MATTER attribute handles */
    ble_svcs_matter_data_t    data;       /**< MATTER attribute data */
} ble_svcs_matter_subinfo_t;

typedef struct ble_svcs_matter_info_s
{
    ble_gatt_role_t           role;           /**< BLE GATT role */
    ble_svcs_matter_subinfo_t client_info;    /**< Client Information */
    ble_svcs_matter_subinfo_t server_info;    /**< Server Information */
} ble_svcs_matter_info_t;


/**
 * @ingroup app_matter_def
 * @defgroup app_matter_App BLE MATTER Definitions for Application
 * @{
 * @details Here shows the definitions of the MATTER for application uses.
 * @}
*/

/** ble_svcs_evt_matter_handler_t
 * @ingroup app_matter_App
 * @note This callback receives the MATTER events. Each of these events can be associated with parameters.
*/
typedef void (*ble_svcs_evt_matter_handler_t)(ble_evt_att_param_t *p_param);

/** MATTER Initialization
*
* @ingroup app_matter_App
*
* @attention There is only one instance of MATTER shall be exposed on a device (if role is @ref BLE_GATT_ROLE_SERVER). \n
*            Callback shall be ignored if role is @ref BLE_GATT_ROLE_SERVER).
*
* @param[in] host_id : the link's host id.
* @param[in] role : @ref ble_gatt_role_t "BLE GATT role".
* @param[in] p_info : a pointer to MATTER information.
* @param[in] callback : a pointer to a callback function that receive the service events.
*
* @retval BLE_ERR_INVALID_HOST_ID : Error host id.
* @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
* @retval BLE_ERR_CMD_NOT_SUPPORTED : Registered services buffer full.
* @retval BLE_ERR_OK  : Setting success.
*/
ble_err_t ble_svcs_matter_init(uint8_t host_id, ble_gatt_role_t role, ble_svcs_matter_info_t *p_info, ble_svcs_evt_matter_handler_t callback);

/** Get MATTER Handle Numbers
*
* @ingroup app_matter_App
*
* @attention - role = @ref BLE_GATT_ROLE_CLIENT \n
*              MUST call this API to get service information after received @ref BLECMD_EVENT_ATT_DATABASE_PARSING_FINISHED  \n
*            - role = @ref BLE_GATT_ROLE_SERVER \n
*              MUST call this API to get service information before connection established. \n
*
* @param[in] host_id : the link's host id.
* @param[out] p_info : a pointer to MATTER information.
*
* @retval BLE_ERR_INVALID_HOST_ID : Error host id.
* @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
* @retval BLE_ERR_OK : Setting success.
*/
ble_err_t ble_svcs_matter_handles_get(uint8_t host_id, ble_gatt_role_t role, ble_svcs_matter_info_t *p_info);

#ifdef __cplusplus
};
#endif
#endif //_BLE_SERVICE_MATTER_H_
