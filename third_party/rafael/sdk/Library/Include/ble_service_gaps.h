#ifndef _BLE_SERVICE_GAPS_H_
#define _BLE_SERVICE_GAPS_H_

/**************************************************************************//**
 * @file  ble_service_gaps.h
 * @brief Provide the Definition of GAPS.
*****************************************************************************/

#include <stdint.h>
#include "ble_service_common.h"
#include "ble_gap.h"
#include "ble_att_gatt.h"


/**************************************************************************
 * GAPS Definitions
 **************************************************************************/
/** @defgroup service_gaps_def BLE GAPS Definitions
 * @{
 * @ingroup service_def
 * @details Here shows the definitions of the GAPS.
 * @}
**************************************************************************/

/**
 * @ingroup service_gaps_def
 * @defgroup service_gaps_UUIDDef BLE GAPS UUID Definitions
 * @{
 * @details Here shows the definitions of the BLE GAPS UUID Definitions.
*/
extern const uint16_t attr_uuid_gaps_primary_service[];                                     /**< GAPS service UUID. */
extern const uint16_t attr_uuid_gaps_charc_device_name[];                                   /**< GAPS characteristic DEVICE_NAME UUID. */
extern const uint16_t attr_uuid_gaps_charc_appearance[];                                    /**< GAPS characteristic APPEARANCE UUID. */
extern const uint16_t attr_uuid_gaps_charc_peripheral_preferred_connection_parameters[];    /**< GAPS characteristic PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS UUID. */
/** @} */

/**
 * @defgroup service_gaps_ServiceChardef BLE GAPS Service and Characteristic Definitions
 * @{
 * @ingroup service_gaps_def
 * @details Here shows the definitions of the GAPS service and characteristic.
 * @}
*/

/**
 * @ingroup service_gaps_ServiceChardef
 * @{
*/
extern const ble_att_param_t att_gaps_primary_service;                                              /**< GAPS primary service. */
extern const ble_att_param_t att_gaps_characteristic_device_name;                                   /**< GAPS characteristic DEVICE_NAME. */
extern const ble_att_param_t att_gaps_device_name;                                                  /**< GAPS DEVICE_NAME value. */
extern const ble_att_param_t att_gaps_characteristic_appearance;                                    /**< GAPS characteristic APPEARANCE. */
extern const ble_att_param_t att_gaps_appearance;                                                   /**< GAPS APPEARANCE value. */
extern const ble_att_param_t att_gaps_characteristic_peripheral_preferred_connection_parameters;    /**< GAPS characteristic PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS. */
extern const ble_att_param_t att_gaps_peripheral_preferred_connection_parameters;                   /**< GAPS PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS value. */
/** @} */


/** GAPS Definition
 * @ingroup service_gaps_ServiceChardef
*/
#define ATT_GAPS_SERVICE                                                          \
    &att_gaps_primary_service,                                                    \
    &att_gaps_characteristic_device_name,                                         \
    &att_gaps_device_name,                                                        \
    &att_gaps_characteristic_appearance,                                          \
    &att_gaps_appearance,                                                         \
    &att_gaps_characteristic_peripheral_preferred_connection_parameters,          \
    &att_gaps_peripheral_preferred_connection_parameters                          \


/**************************************************************************
 * GAPS Application Definitions
 **************************************************************************/
/** @defgroup app_gaps_def BLE GAPS Application Definitions
 * @{
 * @ingroup service_gaps_def
 * @details Here shows the definitions of the GAPS for application.
 * @}
**************************************************************************/
/**
 * @ingroup app_gaps_def
 * @{
 */
#define GAPS_DEVICE_NAME_LENGH_MAX                           248  /**< Maximum length of GAPS server device name. */
#define GAPS_DEVICE_NAME_LENGH                               30   /**< Default length of GAPS server device name. */
/** @} */

/**
 * @ingroup app_gaps_def
 * @defgroup app_gaps_eventDef BLE GAPS Service and Characteristic Definitions
 * @{
 * @details Here shows the event definitions of the GAPS.
*/
#define BLESERVICE_GAPS_DEVICE_NAME_READ_EVENT                                        0x01     /**< GAPS characteristic DEVICE_NAME read event.*/
#define BLESERVICE_GAPS_DEVICE_NAME_READ_RSP_EVENT                                    0x02     /**< GAPS characteristic DEVICE_NAME read response event.*/
#define BLESERVICE_GAPS_APPEARANCE_READ_EVENT                                         0x03     /**< GAPS characteristic APPEARANCE read event.*/
#define BLESERVICE_GAPS_APPEARANCE_READ_RSP_EVENT                                     0x04     /**< GAPS characteristic APPEARANCE read response event.*/
#define BLESERVICE_GAPS_PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS_READ_EVENT         0x05     /**< GAPS characteristic PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS read event.*/
#define BLESERVICE_GAPS_PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS_READ_RSP_EVENT     0x06     /**< GAPS characteristic PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS read response event.*/
/** @} */


/**
 * @ingroup app_gaps_def
 * @defgroup app_gaps_structureDef BLE GAPS Structure Definitions
 * @{
 * @details Here shows the structure definitions of the GAPS.
 * @}
*/

/** GAPS Handles Definition
 * @ingroup app_gaps_structureDef
*/
typedef struct ble_svcs_gaps_handles_s
{
    uint16_t hdl_device_name;                                   /**< Handle of DEVICE_NAME. */
    uint16_t hdl_appearance;                                    /**< Handle of APPEARANCE. */
    uint16_t hdl_peripheral_preferred_connection_parameters;    /**< Handle of PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS. */
} ble_svcs_gaps_handles_t;


/** GAPS Data Definition
 * @ingroup app_gaps_structureDef
 * @note User defined.
*/
typedef struct ble_svcs_gaps_data_s
{
    uint8_t               device_name[GAPS_DEVICE_NAME_LENGH];    /**< GAPS device name. */
    uint8_t               device_name_len;                        /**< GAPS the length of device name. */
    uint16_t              appearance;                             /**< GAPS appearance. */
    ble_gap_conn_param_t  periph_conn_param;                      /**< GAPS peripheral preferred connection parameter. */
} ble_svcs_gaps_data_t;


/** GAPS Application Data Structure Definition
 * @ingroup app_gaps_structureDef
*/
typedef struct ble_svcs_gaps_subinfo_s
{
    ble_svcs_gaps_handles_t handles;    /**< GAPS attribute handles */
    ble_svcs_gaps_data_t    data;       /**< GAPS attribute data */
} ble_svcs_gaps_subinfo_t;


typedef struct ble_svcs_gaps_info_s
{
    ble_gatt_role_t           role;         /**< BLE GATT role */
    ble_svcs_gaps_subinfo_t   client_info;
    ble_svcs_gaps_subinfo_t   server_info;
} ble_svcs_gaps_info_t;

/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

/**
 * @ingroup app_gaps_def
 * @defgroup app_gaps_App BLE GAPS Definitions for Application
 * @{
 * @details Here shows the definitions of the GAPS for application uses.
 * @}
*/

/** ble_svcs_evt_gaps_handler_t
 * @ingroup app_gaps_App
 * @note This callback receives the GAPS events. Each of these events can be associated with parameters.
*/
typedef void (*ble_svcs_evt_gaps_handler_t)(ble_evt_att_param_t *p_param);

/** GAPS Initialization
*
* @ingroup app_gaps_App
*
* @attention There is only one instance of GAPS shall be exposed on a device (if role is @ref BLE_GATT_ROLE_SERVER). \n
*            Callback shall be ignored if role is @ref BLE_GATT_ROLE_SERVER).
*
* @param[in] host_id : the link's host id.
* @param[in] role : @ref ble_gatt_role_t "BLE GATT role".
* @param[in] p_info : a pointer to GAPS information.
* @param[in] callback : a pointer to a callback function that receive the service events.
*
* @retval BLE_ERR_INVALID_HOST_ID : Error host id.
* @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
* @retval BLE_ERR_CMD_NOT_SUPPORTED : Registered services buffer full.
* @retval BLE_ERR_OK  : Setting success.
*/
ble_err_t ble_svcs_gaps_init(uint8_t host_id, ble_gatt_role_t role, ble_svcs_gaps_info_t *p_info, ble_svcs_evt_gaps_handler_t callback);

/** Get GAPS Handle Numbers
 *
 * @param[in] p_data : a pointer to the device name.
 * @param[in] length : the length of the device name.
 *
 * @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
 * @retval BLE_ERR_OK  : Setting success.
*/
ble_err_t ble_svcs_gaps_device_name_set(uint8_t *p_data, uint8_t length);


/** Set GAPS Appearance
 *
 * @param[in] appearance : @ref bleGapAppearance.
*/
void ble_svcs_gaps_appearance_set(uint16_t appearance);



/** Set GAPS Peripheral Preferred Connection Parameters
 *
 * @param[in] p_param : a pointer to the preferred peripheral connection parameters.
 *
 * @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
 * @retval BLE_ERR_OK  : Setting success.
*/
ble_err_t ble_svcs_gaps_peripheral_conn_param_set(ble_gap_conn_param_t *p_param);



/** Get GAPS Handle Numbers
*
* @ingroup app_gaps_App
*
* @attention - role = @ref BLE_GATT_ROLE_CLIENT \n
*              MUST call this API to get service information after received @ref BLECMD_EVENT_ATT_DATABASE_PARSING_FINISHED  \n
*            - role = @ref BLE_GATT_ROLE_SERVER \n
*              MUST call this API to get service information before connection established. \n
*
* @param[in] host_id : the link's host id.
* @param[out] p_info : a pointer to GAPS information.
*
* @retval BLE_ERR_INVALID_HOST_ID : Error host id.
* @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
* @retval BLE_ERR_OK  : Setting success.
*/
ble_err_t ble_svcs_gaps_handles_get(uint8_t host_id, ble_gatt_role_t role, ble_svcs_gaps_info_t *p_info);

#endif //_BLE_SERVICE_GAPS_H_

