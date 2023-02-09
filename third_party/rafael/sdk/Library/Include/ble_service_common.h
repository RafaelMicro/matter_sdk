#ifndef _BLE_SERVICE_COMMON_H_
#define _BLE_SERVICE_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif
/**************************************************************************//**
 * @file  ble_service_common.h
 * @brief Provide the Common Definition of BLE Profile.
*****************************************************************************/

#include <stdint.h>
#include "FreeRTOS.h"
#include "ble_uuid.h"
#include "ble_att_gatt.h"
#include "ble_api.h"
#include "ble_printf.h"


/**************************************************************************
* BLE Profile Common Definitions
**************************************************************************/
/**
 * @ingroup BLE_group
 * @{
 * @defgroup service_allDef BLE Services
 * @{
 * @brief Define BLE Services definitions, structures, and functions.
 * @details This file shows the BLE defined-services definitions and functions.
 * @defgroup service_basedDef BLE Service Common Definitions
 * @{
 * @details Here shows the definitions of the defined service common definitions.
 * @}
 * @defgroup service_def BLE Defined-Services
 * @{
 * @details Here shows the definitions of the defined services.
 * @}
 * @}
 * @}
**************************************************************************/

/** @ingroup service_basedDef
*/
#define SIZE_STRING(a)                (sizeof((a))/sizeof((a[0])) - 1)    /**< The size of the string.*/


/** Length of @ref Charac_Presentation_Format for decoded data.
 * @ingroup service_basedDef
*/
#define BLE_CHARAC_PRESENTATION_FORMAT_LEN               7


/**
 * @ingroup service_basedDef
 * @defgroup service_basedUUIDDef BLE Common UUID Definitions
 * @{
 * @details Here shows the definitions of the BLE common UUID Definitions.
*/
extern const uint16_t attr_uuid_invalid[];                            /**< Invalid UUID. */
extern const uint16_t attr_uuid_type_primary_service[];               /**< Primary Service UUID. */
extern const uint16_t attr_uuid_type_secondary_service[];             /**< Secondary Service UUID. */
extern const uint16_t attr_uuid_type_include[];                       /**< Included Service UUID. */
extern const uint16_t attr_uuid_type_characteristic[];                /**< Characteristic UUID. */
extern const uint16_t attr_uuid_type_charc_extended_properties[];     /**< Characteristic extended properties UUID. */
extern const uint16_t attr_uuid_type_charc_user_description[];        /**< Characteristic user description UUID. */
extern const uint16_t attr_uuid_type_client_charc_configuration[];    /**< Client characteristic configuration descriptor UUID. */
extern const uint16_t attr_uuid_type_server_charc_configuration[];    /**< Client characteristic configuration UUID. */
extern const uint16_t attr_uuid_type_charc_presentation_format[];     /**< Characteristic presentation format UUID. */
extern const uint16_t attr_uuid_type_charc_aggregate_format[];        /**< Characteristic aggregate format UUID. */
extern const uint16_t attr_uuid_type_valid_range[];                   /**< Valid range UUID. */
extern const uint16_t attr_uuid_type_external_report_reference[];     /**< External report reference UUID. */
extern const uint16_t attr_uuid_type_report_reference[];              /**< Report reference UUID. */
/** @} */


/**
 * @ingroup service_basedDef
 * @defgroup service_nullDef BLE Common Definitions
 * @{
 * @details Here shows the definitions of the BLE common Definitions.
 * @}
*/

/** NULL access function for service defined uses.
 * @ingroup service_nullDef
*/
void attr_null_access(ble_evt_att_param_t *p_param);



/** NULL definition for service defined uses.
 * @ingroup service_nullDef
*/
extern const ble_att_param_t ATT_NULL_INVALID;


/**
 * @ingroup service_basedDef
 * @defgroup appBase_generalDef BLE Based General Structures
 * @{
 * @details Here shows the general definitions of structure.
*/

/** @brief String Format Structure Definition.
 * @ingroup appBase_generalDef
*/
typedef struct string_format_s
{
    uint8_t             *p_data;      /**< String data. */
    uint8_t             length;       /**< String length. */
} string_format_t;


/** @brief Characteristic Presentation Format Structure Definition.
 * @ingroup appBase_generalDef
*/
typedef struct char_presentation_format_s
{
    uint8_t             format;       /**< Format of the value of this characteristic. */
    uint8_t             exponent;     /**< Exponent field to determine how the value of this characteristic is further formatted. */
    uint16_t            unit;         /**< The unit of this characteristic. */
    uint8_t             name_space;   /**< The name space of the description. */
    uint16_t            description;  /**< The description of this characteristic as defined in a higher layer profile. */
} char_presentation_format_t;


/** @brief Service Common Information Structure Definition.
 * @ingroup appBase_generalDef
*/
typedef struct ble_svcs_common_info_s
{
    uint8_t             index;        /**< Service information index. */
    uint8_t             host_id;      /**< Host id. */
    ble_gatt_role_t     role;         /**< GATT role. */
} ble_svcs_common_info_t;

/** @} */


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/
/**
 * @ingroup service_basedDef
 * @defgroup appBase_App BLE Common Functions
 * @{
 * @details Here shows the definitions of functions.
 * @}
*/

/** BLE Service Common Information Initialization
 *
*/
ble_err_t ble_svcs_common_init(uint8_t host_id, ble_gatt_role_t role, uint8_t maxCount, ble_svcs_common_info_t *p_info, uint8_t *p_index, uint8_t *p_count);


/** Query Index from Service Registered Index Mapping Array
 *
*/
ble_err_t ble_svcs_common_info_index_query(uint8_t host_id, ble_gatt_role_t role, uint8_t maxCount, ble_svcs_common_info_t *p_info, uint8_t *p_index);


/** Get BLE Service handle mapping values.
 *
*/
ble_err_t ble_svcs_handles_mapping_get(ble_gatt_handle_table_param_t *p_param);


/** BLE Set Handle CCCD Write Request From the Client
 *
 */
ble_err_t ble_svcs_handle_cccd_write_req(uint8_t *p_data, uint16_t length, uint16_t *p_cccd);



/** BLE Set Automatically Handle CCCD Read Request or Read By Type Request From the Client
 *
 */
ble_err_t ble_svcs_auto_handle_cccd_read_req(ble_evt_att_param_t *p_param, uint16_t cccd_value);



/** BLE Set Automatically Handle Read Request or Read By Type Request From the Client
 *
 */
ble_err_t ble_svcs_auto_handle_read_req(ble_evt_att_param_t *p_param, uint8_t *p_data, uint8_t length);



/** BLE Set Automatically Handle Read Request or Read By Type Request From the Client
 *
 */
ble_err_t ble_svcs_auto_handle_read_blob_req(ble_evt_att_param_t *p_param, uint8_t *p_data, uint8_t length);


/** BLE Set CCCD
 *
 */
ble_err_t ble_svcs_cccd_set(uint8_t host_id, uint16_t handle_num, ble_gatt_cccd_val_t cccd_value);


/** BLE Set data send
 *
 */
ble_err_t ble_svcs_data_send(uint16_t type, ble_gatt_data_param_t *p_param);

#ifdef __cplusplus
};
#endif
#endif // _BLE_SERVICE_COMMON_H_
