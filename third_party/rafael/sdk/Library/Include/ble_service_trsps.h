#ifndef _BLE_SERVICE_TRSPS_H_
#define _BLE_SERVICE_TRSPS_H_

/**************************************************************************//**
 * @file  ble_service_trsps.h
 * @brief Provide the Definition of TRSPS.
*****************************************************************************/

#include "ble_service_common.h"
#include "ble_att_gatt.h"


/**************************************************************************
 * TRSPS Definitions
 **************************************************************************/
/** @defgroup service_trsps_def BLE TRSPS Definitions
 * @{
 * @ingroup service_def
 * @details Here shows the definitions of the TRSPS.
 * @}
**************************************************************************/

/**
 * @ingroup service_trsps_def
 * @defgroup service_trsps_UUIDDef BLE TRSPS UUID Definitions
 * @{
 * @details Here shows the definitions of the BLE TRSPS UUID Definitions.
*/
extern const uint16_t attr_uuid_trsps_primary_service[];    /**< TRSPS service UUID. */
extern const uint16_t attr_uuid_trsps_charc_udatr01[];      /**< TRSPS characteristic UDATR01 UUID. */
extern const uint16_t attr_uuid_trsps_charc_udatni01[];     /**< TRSPS characteristic UDATNI01 UUID. */
extern const uint16_t attr_uuid_trsps_charc_udatrw01[];     /**< TRSPS characteristic UDATRW01 UUID. */
/** @} */

/**
 * @defgroup service_trsps_ServiceChardef BLE TRSPS Service and Characteristic Definitions
 * @{
 * @ingroup service_trsps_def
 * @details Here shows the definitions of the TRSPS service and characteristic.
 * @}
*/

/**
 * @ingroup service_trsps_ServiceChardef
 * @{
*/
extern const ble_att_param_t att_trsps_primary_service;                        /**< TRSPS primary service. */
extern const ble_att_param_t att_trsps_characteristic_udatr01;                 /**< TRSPS characteristic UDATR01. */
extern const ble_att_param_t att_trsps_udatr01;                                /**< TRSPS UDATR01 value. */
extern const ble_att_param_t att_trsps_characteristic_udatni01;                /**< TRSPS characteristic UDATNI01. */
extern const ble_att_param_t att_trsps_udatni01;                               /**< TRSPS UDATNI01 value. */
extern const ble_att_param_t att_trsps_udatni01_client_charc_configuration;    /**< TRSPS UDATNI01 client characteristic configuration. */
extern const ble_att_param_t att_trsps_characteristic_udatrw01;                /**< TRSPS characteristic UDATRW01. */
extern const ble_att_param_t att_trsps_udatrw01;                               /**< TRSPS UDATRW01 value. */
/** @} */


/** TRSPS Definition
 * @ingroup service_trsps_ServiceChardef
*/
#define ATT_TRSPS_SERVICE                                    \
    &att_trsps_primary_service,                              \
    &att_trsps_characteristic_udatr01,                       \
    &att_trsps_udatr01,                                      \
    &att_trsps_characteristic_udatni01,                      \
    &att_trsps_udatni01,                                     \
    &att_trsps_udatni01_client_charc_configuration,          \
    &att_trsps_characteristic_udatrw01,                      \
    &att_trsps_udatrw01                                      \


/**************************************************************************
 * TRSPS Application Definitions
 **************************************************************************/
/** @defgroup app_trsps_def BLE TRSPS Application Definitions
 * @{
 * @ingroup service_trsps_def
 * @details Here shows the definitions of the TRSPS for application.
 * @}
**************************************************************************/

/**
 * @ingroup app_trsps_def
 * @defgroup app_trsps_eventDef BLE TRSPS Service and Characteristic Definitions
 * @{
 * @details Here shows the event definitions of the TRSPS.
*/
#define BLESERVICE_TRSPS_UDATR01_READ_EVENT                   0x01     /**< TRSPS characteristic UDATR01 read event.*/
#define BLESERVICE_TRSPS_UDATR01_READ_RSP_EVENT               0x02     /**< TRSPS characteristic UDATR01 read response event.*/
#define BLESERVICE_TRSPS_UDATNI01_NOTIFY_EVENT                0x03     /**< TRSPS characteristic UDATNI01 notify event.*/
#define BLESERVICE_TRSPS_UDATNI01_INDICATE_CONFIRM_EVENT      0x04     /**< TRSPS characteristic UDATNI01 indicate confirm event.*/
#define BLESERVICE_TRSPS_UDATNI01_INDICATE_EVENT              0x05     /**< TRSPS characteristic UDATNI01 indicate event.*/
#define BLESERVICE_TRSPS_UDATNI01_CCCD_READ_EVENT             0x06     /**< TRSPS characteristic UDATNI01 cccd read event.*/
#define BLESERVICE_TRSPS_UDATNI01_CCCD_READ_RSP_EVENT         0x07     /**< TRSPS characteristic UDATNI01 cccd read response event.*/
#define BLESERVICE_TRSPS_UDATNI01_CCCD_WRITE_EVENT            0x08     /**< TRSPS characteristic UDATNI01 cccd write event.*/
#define BLESERVICE_TRSPS_UDATNI01_CCCD_WRITE_RSP_EVENT        0x09     /**< TRSPS characteristic UDATNI01 cccd write response event.*/
#define BLESERVICE_TRSPS_UDATRW01_READ_EVENT                  0x0a     /**< TRSPS characteristic UDATRW01 read event.*/
#define BLESERVICE_TRSPS_UDATRW01_READ_RSP_EVENT              0x0b     /**< TRSPS characteristic UDATRW01 read response event.*/
#define BLESERVICE_TRSPS_UDATRW01_WRITE_EVENT                 0x0c     /**< TRSPS characteristic UDATRW01 write event.*/
#define BLESERVICE_TRSPS_UDATRW01_WRITE_RSP_EVENT             0x0d     /**< TRSPS characteristic UDATRW01 write response event.*/
#define BLESERVICE_TRSPS_UDATRW01_WRITE_WITHOUT_RSP_EVENT     0x0e     /**< TRSPS characteristic UDATRW01 write without response event.*/
/** @} */


/**
 * @ingroup app_trsps_def
 * @defgroup app_trsps_structureDef BLE TRSPS Structure Definitions
 * @{
 * @details Here shows the structure definitions of the TRSPS.
 * @}
*/

/** TRSPS Handles Definition
 * @ingroup app_trsps_structureDef
*/
typedef struct ble_svcs_trsps_handles_s
{
    uint16_t hdl_udatr01;          /**< Handle of UDATR01. */
    uint16_t hdl_udatni01;         /**< Handle of UDATNI01. */
    uint16_t hdl_udatni01_cccd;    /**< Handle of UDATNI01 client characteristic configuration. */
    uint16_t hdl_udatrw01;         /**< Handle of UDATRW01. */
} ble_svcs_trsps_handles_t;


/** TRSPS Data Definition
 * @ingroup app_trsps_structureDef
 * @note User defined.
*/
typedef struct ble_svcs_trsps_data_s
{
    uint16_t udatni01_cccd;    /**< UDATNI01 cccd value */
} ble_svcs_trsps_data_t;


/** TRSPS Application Data Structure Definition
 * @ingroup app_trsps_structureDef
*/
typedef struct ble_svcs_trsps_subinfo_s
{
    ble_svcs_trsps_handles_t handles;    /**< TRSPS attribute handles */
    ble_svcs_trsps_data_t    data;       /**< TRSPS attribute data */
} ble_svcs_trsps_subinfo_t;

typedef struct ble_svcs_trsps_info_s
{
    ble_gatt_role_t          role;           /**< BLE GATT role */
    ble_svcs_trsps_subinfo_t client_info;    /**< Client Information */
    ble_svcs_trsps_subinfo_t server_info;    /**< Server Information */
} ble_svcs_trsps_info_t;


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

/**
 * @ingroup app_trsps_def
 * @defgroup app_trsps_App BLE TRSPS Definitions for Application
 * @{
 * @details Here shows the definitions of the TRSPS for application uses.
 * @}
*/

/** ble_svcs_evt_trsps_handler_t
 * @ingroup app_trsps_App
 * @note This callback receives the TRSPS events. Each of these events can be associated with parameters.
*/
typedef void (*ble_svcs_evt_trsps_handler_t)(ble_evt_att_param_t *p_param);

/** TRSPS Initialization
*
* @ingroup app_trsps_App
*
* @attention There is only one instance of TRSPS shall be exposed on a device (if role is @ref BLE_GATT_ROLE_SERVER). \n
*            Callback shall be ignored if role is @ref BLE_GATT_ROLE_SERVER).
*
* @param[in] host_id : the link's host id.
* @param[in] role : @ref ble_gatt_role_t "BLE GATT role".
* @param[in] p_info : a pointer to TRSPS information.
* @param[in] callback : a pointer to a callback function that receive the service events.
*
* @retval BLE_ERR_INVALID_HOST_ID : Error host id.
* @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
* @retval BLE_ERR_CMD_NOT_SUPPORTED : Registered services buffer full.
* @retval BLE_ERR_OK  : Setting success.
*/
ble_err_t ble_svcs_trsps_init(uint8_t host_id, ble_gatt_role_t role, ble_svcs_trsps_info_t *p_info, ble_svcs_evt_trsps_handler_t callback);

/** Get TRSPS Handle Numbers
*
* @ingroup app_trsps_App
*
* @attention - role = @ref BLE_GATT_ROLE_CLIENT \n
*              MUST call this API to get service information after received @ref BLECMD_EVENT_ATT_DATABASE_PARSING_FINISHED  \n
*            - role = @ref BLE_GATT_ROLE_SERVER \n
*              MUST call this API to get service information before connection established. \n
*
* @param[in] host_id : the link's host id.
* @param[out] p_info : a pointer to TRSPS information.
*
* @retval BLE_ERR_INVALID_HOST_ID : Error host id.
* @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
* @retval BLE_ERR_OK : Setting success.
*/
ble_err_t ble_svcs_trsps_handles_get(uint8_t host_id,  ble_gatt_role_t role, ble_svcs_trsps_info_t *p_info);


/** Send Data to Server (Client ONLY)
 *
 * @ingroup appTRSPS_App
 *
 * @param[in] host_id : the link's host id.
 * @param[in] type : BLE GATT characteristic write type
 * @param[in] handle_num : handle number of write characteristic.
 * @param[in] p_data : a pointer to data to send
 * @param[in] length : length of  the data
 *
 * @retval BLE_ERR_INVALID_HOST_ID : Error host id.
 * @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
 * @retval BLE_ERR_DB_PARSING_IN_PROGRESS : parsing database process has NOT finished.
 * @retval BLE_ERR_INVALID_HANDLE : Invalid attribute handle.
 * @retval BLE_BUSY : Message queue buffer full.
 * @retval BLE_ERR_OK  : Setting success.
*/
ble_err_t ble_svcs_trsps_client_send(uint8_t host_id, ble_gatt_write_t type, uint16_t handle_num, uint8_t *p_data, uint8_t length);


/** Get data from server by reading request (Client ONLY)
 *
 * @ingroup appTRSPS_App
 *
 * @param[in] host_id : the link's host id
 * @param[in] handle_num : handle number of write characteristic.
 *
 * @retval BLE_ERR_INVALID_HOST_ID : Error host id.
 * @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
 * @retval BLE_ERR_DB_PARSING_IN_PROGRESS : parsing database process has NOT finished.
 * @retval BLE_ERR_INVALID_HANDLE : Invalid attribute handle.
 * @retval BLE_BUSY : Message queue buffer full.
 * @retval BLE_ERR_OK  : Setting success.
*/
ble_err_t ble_svcs_trsps_client_read(uint8_t host_id, uint16_t handle_num);



/** Send Data to Client (Server ONLY)
 *
 * @ingroup appTRSPS_App
 *
 * @param[in] host_id : the link's host id.
 * @param[in] cccd : BLE GATT characteristic cccd value to indicated notification or indication @ref ble_gatt_cccd_val_t .
 * @param[in] handle_num : handle number of write characteristic.
 * @param[in] p_data : a pointer to data to send
 * @param[in] length : length of  the data
 *
 * @retval BLE_ERR_INVALID_HOST_ID : Error host id.
 * @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
 * @retval BLE_ERR_DB_PARSING_IN_PROGRESS : parsing database process has NOT finished.
 * @retval BLE_ERR_INVALID_HANDLE : Invalid attribute handle.
 * @retval BLE_BUSY : Message queue buffer full.
 * @retval BLE_ERR_OK  : Setting success.
*/
ble_err_t ble_svcs_trsps_server_send(uint8_t host_id, ble_gatt_cccd_val_t cccd, uint16_t handle_num, uint8_t *p_data, uint8_t length);

#endif //_BLE_SERVICE_TRSPS_H_
