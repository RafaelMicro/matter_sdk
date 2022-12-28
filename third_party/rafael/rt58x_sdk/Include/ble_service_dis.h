#ifndef _BLE_SERVICE_DIS_H_
#define _BLE_SERVICE_DIS_H_

#ifdef __cplusplus
extern "C" {
#endif
/**************************************************************************//**
 * @file  ble_service_dis.h
 * @brief Provide the Definition of DIS.
*****************************************************************************/

#include "ble_service_common.h"
#include "ble_att_gatt.h"


/**************************************************************************
 * DIS Definitions
 **************************************************************************/
/** @defgroup service_dis_def BLE DIS Definitions
 * @{
 * @ingroup service_def
 * @details Here shows the definitions of the DIS.
 * @}
**************************************************************************/

/**
 * @ingroup service_dis_def
 * @defgroup service_dis_UUIDDef BLE DIS UUID Definitions
 * @{
 * @details Here shows the definitions of the BLE DIS UUID Definitions.
*/
extern const uint16_t attr_uuid_dis_primary_service[];                   /**< DIS service UUID. */
extern const uint16_t attr_uuid_dis_charc_serial_number_string[];        /**< DIS characteristic SERIAL_NUMBER_STRING UUID. */
extern const uint16_t attr_uuid_dis_charc_manufacturer_name_string[];    /**< DIS characteristic MANUFACTURER_NAME_STRING UUID. */
extern const uint16_t attr_uuid_dis_charc_system_id[];                   /**< DIS characteristic SYSTEM_ID UUID. */
extern const uint16_t attr_uuid_dis_charc_firmware_revision_string[];    /**< DIS characteristic FIRMWARE_REVISION_STRING UUID. */
extern const uint16_t attr_uuid_dis_charc_model_number_string[];         /**< DIS characteristic MODEL_NUMBER_STRING UUID. */
extern const uint16_t attr_uuid_dis_charc_hardware_revision_string[];    /**< DIS characteristic HARDWARE_REVISION_STRING UUID. */
extern const uint16_t attr_uuid_dis_charc_software_revision_string[];    /**< DIS characteristic SOFTWARE_REVISION_STRING UUID. */
extern const uint16_t attr_uuid_dis_charc_pnp_id[];                      /**< DIS characteristic PNP_ID UUID. */
/** @} */

/**
 * @defgroup serviceDIS_dataDef BLE DIS Value Definitions
 * @{
 * @ingroup service_dis_def
 * @details Here shows the value definitions of the DIS service.
 * @note Return the "Read data" when central send "Read Request".
*/

extern const uint8_t ATTR_VALUE_DIS_SERIAL_NUMBER_STRING_PRESENTATION_FORMAT[];     /**< DIS serial number string presentation format value. */
extern const uint8_t ATTR_VALUE_DIS_COMMON_PRESENTATION_FORMAT[];                   /**< DIS common presentation format value. */
extern const uint8_t ATTR_VALUE_DIS_MANUFACTURER_NAME_STRING_PRESENTATION_FORMAT[]; /**< DIS manufacturer name string presentation format value. */
extern const uint8_t ATTR_VALUE_DIS_SYSTEM_ID[];                                    /**< DIS system ID string value. */
extern const uint8_t ATTR_VALUE_DIS_FIRMWARE_REVISION_STRING_PRESENTATION_FORMAT[]; /**< DIS firmware revision presentation string value. */
extern const uint8_t ATTR_VALUE_DIS_MODEL_NUMBER_STRING_PRESENTATION_FORMAT[];      /**< DIS model number string presentation format value. */
extern const uint8_t ATTR_VALUE_DIS_HARDWARE_REVISION_STRING_PRESENTATION_FORMAT[]; /**< DIS hardware revision string presentation format value. */
extern const uint8_t ATTR_VALUE_DIS_SOFTWARE_REVISION_STRING_PRESENTATION_FORMAT[]; /**< DIS software revision string presentation format value. */
extern const uint8_t ATTR_VALUE_DIS_PNP_ID[];                                       /**< DIS PnP ID value. */
/** @} */


/**
 * @defgroup service_dis_ServiceChardef BLE DIS Service and Characteristic Definitions
 * @{
 * @ingroup service_dis_def
 * @details Here shows the definitions of the DIS service and characteristic.
 * @}
*/

/**
 * @ingroup service_dis_ServiceChardef
 * @{
*/
extern const ble_att_param_t att_dis_primary_service;                                 /**< DIS primary service. */
extern const ble_att_param_t att_dis_characteristic_serial_number_string;             /**< DIS characteristic SERIAL_NUMBER_STRING. */
extern const ble_att_param_t att_dis_serial_number_string;                            /**< DIS SERIAL_NUMBER_STRING value. */
extern const ble_att_param_t att_dis_serial_number_string_presentation_format;        /**< DIS SERIAL_NUMBER_STRING presentation format. */
extern const ble_att_param_t att_dis_characteristic_manufacturer_name_string;         /**< DIS characteristic MANUFACTURER_NAME_STRING. */
extern const ble_att_param_t att_dis_manufacturer_name_string;                        /**< DIS MANUFACTURER_NAME_STRING value. */
extern const ble_att_param_t att_dis_manufacturer_name_string_presentation_format;    /**< DIS MANUFACTURER_NAME_STRING presentation format. */
extern const ble_att_param_t att_dis_characteristic_system_id;                        /**< DIS characteristic SYSTEM_ID. */
extern const ble_att_param_t att_dis_system_id;                                       /**< DIS SYSTEM_ID value. */
extern const ble_att_param_t att_dis_characteristic_firmware_revision_string;         /**< DIS characteristic FIRMWARE_REVISION_STRING. */
extern const ble_att_param_t att_dis_firmware_revision_string;                        /**< DIS FIRMWARE_REVISION_STRING value. */
extern const ble_att_param_t att_dis_firmware_revision_string_presentation_format;    /**< DIS FIRMWARE_REVISION_STRING presentation format. */
extern const ble_att_param_t att_dis_characteristic_model_number_string;              /**< DIS characteristic MODEL_NUMBER_STRING. */
extern const ble_att_param_t att_dis_model_number_string;                             /**< DIS MODEL_NUMBER_STRING value. */
extern const ble_att_param_t att_dis_model_number_string_presentation_format;         /**< DIS MODEL_NUMBER_STRING presentation format. */
extern const ble_att_param_t att_dis_characteristic_hardware_revision_string;         /**< DIS characteristic HARDWARE_REVISION_STRING. */
extern const ble_att_param_t att_dis_hardware_revision_string;                        /**< DIS HARDWARE_REVISION_STRING value. */
extern const ble_att_param_t att_dis_hardware_revision_string_presentation_format;    /**< DIS HARDWARE_REVISION_STRING presentation format. */
extern const ble_att_param_t att_dis_characteristic_software_revision_string;         /**< DIS characteristic SOFTWARE_REVISION_STRING. */
extern const ble_att_param_t att_dis_software_revision_string;                        /**< DIS SOFTWARE_REVISION_STRING value. */
extern const ble_att_param_t att_dis_software_revision_string_presentation_format;    /**< DIS SOFTWARE_REVISION_STRING presentation format. */
extern const ble_att_param_t att_dis_characteristic_pnp_id;                           /**< DIS characteristic PNP_ID. */
extern const ble_att_param_t att_dis_pnp_id;                                          /**< DIS PNP_ID value. */
/** @} */


/** DIS Definition
 * @ingroup service_dis_ServiceChardef
*/
#define ATT_DIS_SERVICE                                             \
    &att_dis_primary_service,                                       \
    &att_dis_characteristic_serial_number_string,                   \
    &att_dis_serial_number_string,                                  \
    &att_dis_serial_number_string_presentation_format,              \
    &att_dis_characteristic_manufacturer_name_string,               \
    &att_dis_manufacturer_name_string,                              \
    &att_dis_manufacturer_name_string_presentation_format,          \
    &att_dis_characteristic_system_id,                              \
    &att_dis_system_id,                                             \
    &att_dis_characteristic_firmware_revision_string,               \
    &att_dis_firmware_revision_string,                              \
    &att_dis_firmware_revision_string_presentation_format,          \
    &att_dis_characteristic_model_number_string,                    \
    &att_dis_model_number_string,                                   \
    &att_dis_model_number_string_presentation_format,               \
    &att_dis_characteristic_hardware_revision_string,               \
    &att_dis_hardware_revision_string,                              \
    &att_dis_hardware_revision_string_presentation_format,          \
    &att_dis_characteristic_software_revision_string,               \
    &att_dis_software_revision_string,                              \
    &att_dis_software_revision_string_presentation_format,          \
    &att_dis_characteristic_pnp_id,                                 \
    &att_dis_pnp_id                                                 \


/**
 * @defgroup serviceDIS_pnpdef BLE DIS Characteristic PnP ID Vendor ID Source Field Definitions
 * @{
 * @ingroup service_dis_def
 * @details Here shows the definitions of the DIS PnP ID Vendor ID source field.
 *
 * @note The Bluetooth Special Interest Group assigns Device ID Vendor ID, and the USB Implementer Forum assigns Vendor IDs, either of which can be used for the Vendor ID field value. \n
 *       Device providers should procure the Vendor ID from the USB Implementer Forum or the Company Identifier from the Bluetooth SIG.
 * @note http://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.pnp_id.xml
*/
#define BLE_GATT_DIS_PNPID_VID_SOURCE_BLUETOOTH_SIG     0x01      /**< BLE SIG-assigned Device ID Vendor ID value form the Assigned Number Document. */
#define BLE_GATT_DIS_PNPID_VID_SOURCE_USB_FORUM         0x02      /**< USB Implement's Forum assigned Vendor ID value. */
/** @} */




/**************************************************************************
 * DIS Application Definitions
 **************************************************************************/
/** @defgroup app_dis_def BLE DIS Application Definitions
 * @{
 * @ingroup service_dis_def
 * @details Here shows the definitions of the DIS for application.
 * @}
**************************************************************************/

/**
 * @ingroup app_dis_def
 * @defgroup app_dis_eventDef BLE DIS Service and Characteristic Definitions
 * @{
 * @details Here shows the event definitions of the DIS.
*/
#define BLESERVICE_DIS_SERIAL_NUMBER_STRING_READ_EVENT                    0x01     /**< DIS characteristic SERIAL_NUMBER_STRING read event.*/
#define BLESERVICE_DIS_SERIAL_NUMBER_STRING_READ_RSP_EVENT                0x02     /**< DIS characteristic SERIAL_NUMBER_STRING read response event.*/
#define BLESERVICE_DIS_SERIAL_NUMBER_STRING_FORMAT_READ_EVENT             0x03     /**< DIS characteristic SERIAL_NUMBER_STRING presentation format read event.*/
#define BLESERVICE_DIS_SERIAL_NUMBER_STRING_FORMAT_READ_RSP_EVENT         0x04     /**< DIS characteristic SERIAL_NUMBER_STRING presentation format read response event.*/
#define BLESERVICE_DIS_MANUFACTURER_NAME_STRING_READ_EVENT                0x05     /**< DIS characteristic MANUFACTURER_NAME_STRING read event.*/
#define BLESERVICE_DIS_MANUFACTURER_NAME_STRING_READ_RSP_EVENT            0x06     /**< DIS characteristic MANUFACTURER_NAME_STRING read response event.*/
#define BLESERVICE_DIS_MANUFACTURER_NAME_STRING_FORMAT_READ_EVENT         0x07     /**< DIS characteristic MANUFACTURER_NAME_STRING presentation format read event.*/
#define BLESERVICE_DIS_MANUFACTURER_NAME_STRING_FORMAT_READ_RSP_EVENT     0x08     /**< DIS characteristic MANUFACTURER_NAME_STRING presentation format read response event.*/
#define BLESERVICE_DIS_SYSTEM_ID_READ_EVENT                               0x09     /**< DIS characteristic SYSTEM_ID read event.*/
#define BLESERVICE_DIS_SYSTEM_ID_READ_RSP_EVENT                           0x0a     /**< DIS characteristic SYSTEM_ID read response event.*/
#define BLESERVICE_DIS_FIRMWARE_REVISION_STRING_READ_EVENT                0x0b     /**< DIS characteristic FIRMWARE_REVISION_STRING read event.*/
#define BLESERVICE_DIS_FIRMWARE_REVISION_STRING_READ_RSP_EVENT            0x0c     /**< DIS characteristic FIRMWARE_REVISION_STRING read response event.*/
#define BLESERVICE_DIS_FIRMWARE_REVISION_STRING_FORMAT_READ_EVENT         0x0d     /**< DIS characteristic FIRMWARE_REVISION_STRING presentation format read event.*/
#define BLESERVICE_DIS_FIRMWARE_REVISION_STRING_FORMAT_READ_RSP_EVENT     0x0e     /**< DIS characteristic FIRMWARE_REVISION_STRING presentation format read response event.*/
#define BLESERVICE_DIS_MODEL_NUMBER_STRING_READ_EVENT                     0x0f     /**< DIS characteristic MODEL_NUMBER_STRING read event.*/
#define BLESERVICE_DIS_MODEL_NUMBER_STRING_READ_RSP_EVENT                 0x10     /**< DIS characteristic MODEL_NUMBER_STRING read response event.*/
#define BLESERVICE_DIS_MODEL_NUMBER_STRING_FORMAT_READ_EVENT              0x11     /**< DIS characteristic MODEL_NUMBER_STRING presentation format read event.*/
#define BLESERVICE_DIS_MODEL_NUMBER_STRING_FORMAT_READ_RSP_EVENT          0x12     /**< DIS characteristic MODEL_NUMBER_STRING presentation format read response event.*/
#define BLESERVICE_DIS_HARDWARE_REVISION_STRING_READ_EVENT                0x13     /**< DIS characteristic HARDWARE_REVISION_STRING read event.*/
#define BLESERVICE_DIS_HARDWARE_REVISION_STRING_READ_RSP_EVENT            0x14     /**< DIS characteristic HARDWARE_REVISION_STRING read response event.*/
#define BLESERVICE_DIS_HARDWARE_REVISION_STRING_FORMAT_READ_EVENT         0x15     /**< DIS characteristic HARDWARE_REVISION_STRING presentation format read event.*/
#define BLESERVICE_DIS_HARDWARE_REVISION_STRING_FORMAT_READ_RSP_EVENT     0x16     /**< DIS characteristic HARDWARE_REVISION_STRING presentation format read response event.*/
#define BLESERVICE_DIS_SOFTWARE_REVISION_STRING_READ_EVENT                0x17     /**< DIS characteristic SOFTWARE_REVISION_STRING read event.*/
#define BLESERVICE_DIS_SOFTWARE_REVISION_STRING_READ_RSP_EVENT            0x18     /**< DIS characteristic SOFTWARE_REVISION_STRING read response event.*/
#define BLESERVICE_DIS_SOFTWARE_REVISION_STRING_FORMAT_READ_EVENT         0x19     /**< DIS characteristic SOFTWARE_REVISION_STRING presentation format read event.*/
#define BLESERVICE_DIS_SOFTWARE_REVISION_STRING_FORMAT_READ_RSP_EVENT     0x1a     /**< DIS characteristic SOFTWARE_REVISION_STRING presentation format read response event.*/
#define BLESERVICE_DIS_PNP_ID_READ_EVENT                                  0x1b     /**< DIS characteristic PNP_ID read event.*/
#define BLESERVICE_DIS_PNP_ID_READ_RSP_EVENT                              0x1c     /**< DIS characteristic PNP_ID read response event.*/
/** @} */


/**
 * @ingroup app_dis_def
 * @defgroup app_dis_structureDef BLE DIS Structure Definitions
 * @{
 * @details Here shows the structure definitions of the DIS.
 * @}
*/

/** DIS Handles Definition
 * @ingroup app_dis_structureDef
*/
typedef struct ble_svcs_dis_handles_s
{
    uint16_t hdl_serial_number_string;                            /**< Handle of SERIAL_NUMBER_STRING. */
    uint16_t hdl_serial_number_string_presentation_format;        /**< Handle of SERIAL_NUMBER_STRING presentation format. */
    uint16_t hdl_manufacturer_name_string;                        /**< Handle of MANUFACTURER_NAME_STRING. */
    uint16_t hdl_manufacturer_name_string_presentation_format;    /**< Handle of MANUFACTURER_NAME_STRING presentation format. */
    uint16_t hdl_system_id;                                       /**< Handle of SYSTEM_ID. */
    uint16_t hdl_firmware_revision_string;                        /**< Handle of FIRMWARE_REVISION_STRING. */
    uint16_t hdl_firmware_revision_string_presentation_format;    /**< Handle of FIRMWARE_REVISION_STRING presentation format. */
    uint16_t hdl_model_number_string;                             /**< Handle of MODEL_NUMBER_STRING. */
    uint16_t hdl_model_number_string_presentation_format;         /**< Handle of MODEL_NUMBER_STRING presentation format. */
    uint16_t hdl_hardware_revision_string;                        /**< Handle of HARDWARE_REVISION_STRING. */
    uint16_t hdl_hardware_revision_string_presentation_format;    /**< Handle of HARDWARE_REVISION_STRING presentation format. */
    uint16_t hdl_software_revision_string;                        /**< Handle of SOFTWARE_REVISION_STRING. */
    uint16_t hdl_software_revision_string_presentation_format;    /**< Handle of SOFTWARE_REVISION_STRING presentation format. */
    uint16_t hdl_pnp_id;                                          /**< Handle of PNP_ID. */
} ble_svcs_dis_handles_t;


/** DIS System ID Definition
 * @ingroup appDIS_structureDef
*/
typedef struct SystemId_Format
{
    uint64_t                        manufacturer_id;              /**< Manufacturer ID, Only 5 bytes shall be used. */
    uint32_t                        organizationally_unique_id;   /**< Organizationally unique ID, Only 3 bytes shall be used. */
} SystemId_Format;


/** DIS PnP ID Definition
 * @ingroup appDIS_structureDef
*/
typedef struct PnPId_Format
{
    uint8_t                         vendor_id_source;             /**< Vendor ID Source. see @ref serviceDIS_pnpdef. */
    uint16_t                        vendor_id;                    /**< Vendor ID. */
    uint16_t                        product_id;                   /**< Product ID. */
    uint16_t                        product_version;              /**< Product Version. */
} PnPId_Format;


/** DIS Data Definition
 * @ingroup app_dis_structureDef
 * @note User defined.
*/
typedef struct ble_svcs_dis_data_s
{
    string_format_t                   *serial_number_str;           /**< Serial number string. */
    char_presentation_format_t        *serial_number_format;        /**< Serial number string characteristic presentation format. */
    string_format_t                   *manufacturer_name_str;       /**< Manufacturer name string. */
    char_presentation_format_t        *manufacturer_name_format;    /**< Manufacturer name string characteristic presentation format. */
    SystemId_Format                   *system_id;                   /**< System ID */
    string_format_t                   *firmware_rev_str;            /**< Firmware revision string */
    char_presentation_format_t        *firmware_rev_format;         /**< Firmware revision string characteristic presentation format. */
    string_format_t                   *model_number_str;            /**< Model number string */
    char_presentation_format_t        *model_number_format;         /**< Model number string characteristic presentation format. */
    string_format_t                   *hardware_rev_str;            /**< Hardware revision string */
    char_presentation_format_t        *hardware_rev_format;         /**< Hardware revision string characteristic presentation format. */
    string_format_t                   *software_rev_str;            /**< Software revision string */
    char_presentation_format_t        *software_rev_format;         /**< Software revision string characteristic presentation format. */
    PnPId_Format                      *pnp_id;                      /**< PnP ID */
} ble_svcs_dis_data_t;


/** DIS Application Data Structure Definition
 * @ingroup app_dis_structureDef
*/
typedef struct ble_svcs_dis_subinfo_s
{
    ble_svcs_dis_handles_t handles;    /**< DIS attribute handles */
    ble_svcs_dis_data_t    data;       /**< DIS attribute data */
} ble_svcs_dis_subinfo_t;


typedef struct ble_svcs_dis_info_s
{
    ble_gatt_role_t           role;         /**< BLE GATT role */
    ble_svcs_dis_subinfo_t    client_info;
    ble_svcs_dis_subinfo_t    server_info;
} ble_svcs_dis_info_t;


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

/**
 * @ingroup app_dis_def
 * @defgroup app_dis_App BLE DIS Definitions for Application
 * @{
 * @details Here shows the definitions of the DIS for application uses.
 * @}
*/

/** ble_svcs_evt_dis_handler_t
 * @ingroup app_dis_App
 * @note This callback receives the DIS events. Each of these events can be associated with parameters.
*/
typedef void (*ble_svcs_evt_dis_handler_t)(ble_evt_att_param_t *p_param);

/** DIS Initialization
*
* @ingroup app_dis_App
*
* @attention There is only one instance of DIS shall be exposed on a device (if role is @ref BLE_GATT_ROLE_SERVER). \n
*            Callback shall be ignored if role is @ref BLE_GATT_ROLE_SERVER).
*
* @param[in] host_id : the link's host id.
* @param[in] role : @ref ble_gatt_role_t "BLE GATT role".
* @param[in] p_info : a pointer to DIS information.
* @param[in] callback : a pointer to a callback function that receive the service events.
*
* @retval BLE_ERR_INVALID_HOST_ID : Error host id.
* @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
* @retval BLE_ERR_CMD_NOT_SUPPORTED : Registered services buffer full.
* @retval BLE_ERR_OK  : Setting success.
*/
ble_err_t ble_svcs_dis_init(uint8_t host_id, ble_gatt_role_t role, ble_svcs_dis_info_t *p_info, ble_svcs_evt_dis_handler_t callback);

/** Get DIS Handle Numbers
*
* @ingroup app_dis_App
*
* @attention - role = @ref BLE_GATT_ROLE_CLIENT \n
*              MUST call this API to get service information after received @ref BLECMD_EVENT_ATT_DATABASE_PARSING_FINISHED  \n
*            - role = @ref BLE_GATT_ROLE_SERVER \n
*              MUST call this API to get service information before connection established. \n
*
* @param[in] host_id : the link's host id.
* @param[out] p_info : a pointer to DIS information.
*
* @retval BLE_ERR_INVALID_HOST_ID : Error host id.
* @retval BLE_ERR_INVALID_PARAMETER : Invalid parameter.
* @retval BLE_ERR_OK  : Setting success.
*/
ble_err_t ble_svcs_dis_handles_get(uint8_t host_id, ble_gatt_role_t role, ble_svcs_dis_info_t *p_info);


#ifdef __cplusplus
};
#endif
#endif //_BLE_SERVICE_DIS_H_
