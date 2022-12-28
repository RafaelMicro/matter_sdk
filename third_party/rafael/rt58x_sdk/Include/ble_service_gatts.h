#ifndef _BLE_SERVICE_GATTS_H_
#define _BLE_SERVICE_GATTS_H_

#ifdef __cplusplus
extern "C" {
#endif
/**************************************************************************//**
 * @file  ble_service_gatts.h
 * @brief Provide the Definition of GATTS.
*****************************************************************************/

#include "ble_service_common.h"
#include "ble_att_gatt.h"


/**************************************************************************
 * GATTS Definitions
 **************************************************************************/
/** @defgroup service_gatts_def BLE GATTS Definitions
 * @{
 * @ingroup service_def
 * @details Here shows the definitions of the GATTS.
 * @}
**************************************************************************/

/**
 * @ingroup service_gatts_def
 * @defgroup service_gatts_UUIDDef BLE GATTS UUID Definitions
 * @{
 * @details Here shows the definitions of the BLE GATTS UUID Definitions.
*/
extern const uint16_t attr_uuid_gatts_primary_service[];          /**< GATTS service UUID. */
extern const uint16_t attr_uuid_gatts_charc_service_changed[];    /**< GATTS characteristic SERVICE_CHANGED UUID. */
/** @} */

/**
 * @defgroup service_gatts_ServiceChardef BLE GATTS Service and Characteristic Definitions
 * @{
 * @ingroup service_gatts_def
 * @details Here shows the definitions of the GATTS service and characteristic.
 * @}
*/

/**
 * @ingroup service_gatts_ServiceChardef
 * @{
*/
extern const ble_att_param_t att_gatts_primary_service;                               /**< GATTS primary service. */
extern const ble_att_param_t att_gatts_characteristic_service_changed;                /**< GATTS characteristic SERVICE_CHANGED. */
extern const ble_att_param_t att_gatts_service_changed;                               /**< GATTS SERVICE_CHANGED value. */
extern const ble_att_param_t att_gatts_service_changed_client_charc_configuration;    /**< GATTS SERVICE_CHANGED client characteristic configuration. */
/** @} */


/** GATTS Definition
 * @ingroup service_gatts_ServiceChardef
*/
#define ATT_GATTS_SERVICE                                          \
    &att_gatts_primary_service,                                    \
    &att_gatts_characteristic_service_changed,                     \
    &att_gatts_service_changed,                                    \
    &att_gatts_service_changed_client_charc_configuration          \


/**************************************************************************
 * GATTS Application Definitions
 **************************************************************************/
/** @defgroup app_gatts_def BLE GATTS Application Definitions
 * @{
 * @ingroup service_gatts_def
 * @details Here shows the definitions of the GATTS for application.
 * @}
**************************************************************************/

/**
 * @ingroup app_gatts_def
 * @defgroup app_gatts_eventDef BLE GATTS Service and Characteristic Definitions
 * @{
 * @details Here shows the event definitions of the GATTS.
*/
#define BLESERVICE_GATTS_SERVICE_CHANGED_INDICATE_CONFIRM_EVENT     0x01     /**< GATTS characteristic SERVICE_CHANGED indicate confirm event.*/
#define BLESERVICE_GATTS_SERVICE_CHANGED_INDICATE_EVENT             0x02     /**< GATTS characteristic SERVICE_CHANGED indicate event.*/
#define BLESERVICE_GATTS_SERVICE_CHANGED_CCCD_READ_EVENT            0x03     /**< GATTS characteristic SERVICE_CHANGED cccd read event.*/
#define BLESERVICE_GATTS_SERVICE_CHANGED_CCCD_READ_RSP_EVENT        0x04     /**< GATTS characteristic SERVICE_CHANGED cccd read response event.*/
#define BLESERVICE_GATTS_SERVICE_CHANGED_CCCD_WRITE_EVENT           0x05     /**< GATTS characteristic SERVICE_CHANGED cccd write event.*/
#define BLESERVICE_GATTS_SERVICE_CHANGED_CCCD_WRITE_RSP_EVENT       0x06     /**< GATTS characteristic SERVICE_CHANGED cccd write response event.*/
/** @} */


/**
 * @ingroup app_gatts_def
 * @defgroup app_gatts_structureDef BLE GATTS Structure Definitions
 * @{
 * @details Here shows the structure definitions of the GATTS.
 * @}
*/

/** GATTS Handles Definition
 * @ingroup app_gatts_structureDef
*/
typedef struct ble_svcs_gatts_handles_s
{
    uint16_t hdl_service_changed;         /**< Handle of SERVICE_CHANGED. */
    uint16_t hdl_service_changed_cccd;    /**< Handle of SERVICE_CHANGED client characteristic configuration. */
} ble_svcs_gatts_handles_t;


/** GATTS Data Definition
 * @ingroup app_gatts_structureDef
 * @note User defined.
*/
typedef struct ble_svcs_gatts_data_s
{
    uint16_t service_changed_cccd;    /**< SERVICE_CHANGED cccd value */
} ble_svcs_gatts_data_t;


/** GATTS Application Data Structure Definition
 * @ingroup app_gatts_structureDef
*/
typedef struct ble_svcs_gatts_subinfo_s
{
    ble_svcs_gatts_handles_t handles;    /**< GAPS attribute handles */
    ble_svcs_gatts_data_t    data;       /**< GAPS attribute data */
} ble_svcs_gatts_subinfo_t;


typedef struct ble_svcs_gatts_info_s
{
    ble_gatt_role_t            role;         /**< BLE GATT role */
    ble_svcs_gatts_subinfo_t   client_info;
    ble_svcs_gatts_subinfo_t   server_info;
} ble_svcs_gatts_info_t;


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

/**
 * @ingroup app_gatts_def
 * @defgroup app_gatts_App BLE GATTS Definitions for Application
 * @{
 * @details Here shows the definitions of the GATTS for application uses.
 * @}
*/

/** ble_svcs_evt_gatts_handler_t
 * @ingroup app_gatts_App
 * @note This callback receives the GATTS events. Each of these events can be associated with parameters.
*/
typedef void (*ble_svcs_evt_gatts_handler_t)(ble_evt_att_param_t *p_param);

/** GATTS Initialization
*
* @ingroup app_gatts_App
*
* @attention There is only one instance of GATTS shall be exposed on a device (if role is @ref BLE_GATT_ROLE_SERVER). \n
*            Callback shall be ignored if role is @ref BLE_GATT_ROLE_SERVER).
*
* @param[in] host_id : the link's host id.
* @param[in] role : @ref ble_gatt_role_t "BLE GATT role".
* @param[in] p_info : a pointer to GATTS information.
* @param[in] callback : a pointer to a callback function that receive the service events.
*
* @retval BLE_ERR_INVALID_HOST_ID : Error host id.
* @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
* @retval BLE_ERR_CMD_NOT_SUPPORTED : Registered services buffer full.
* @retval BLE_ERR_OK  : Setting success.
*/
ble_err_t ble_svcs_gatts_init(uint8_t host_id, ble_gatt_role_t role, ble_svcs_gatts_info_t *p_info, ble_svcs_evt_gatts_handler_t callback);

/** Get GATTS Handle Numbers
*
* @ingroup app_gatts_App
*
* @attention - role = @ref BLE_GATT_ROLE_CLIENT \n
*              MUST call this API to get service information after received @ref BLECMD_EVENT_ATT_DATABASE_PARSING_FINISHED  \n
*            - role = @ref BLE_GATT_ROLE_SERVER \n
*              MUST call this API to get service information before connection established. \n
*
* @param[in] host_id : the link's host id.
* @param[out] p_info : a pointer to GATTS information.
*
* @retval BLE_ERR_INVALID_HOST_ID : Error host id.
* @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
* @retval BLE_ERR_OK : Setting success.
*/
ble_err_t ble_svcs_gatts_handles_get(uint8_t host_id, ble_gatt_role_t role, ble_svcs_gatts_info_t *p_info);

#ifdef __cplusplus
};
#endif
#endif //_BLE_SERVICE_GATTS_H_
