/** @file       ble_privacy.h
 *
 * @brief BLE privacy header file.
 *
*/

/**
 * @defgroup ble_privacy BLE Privacy
 * @ingroup BLE_group
 * @{
 * @brief Define BLE Privacy definitions, structures, and functions.
 * @}
 */

#ifndef _BLE_PRIVACY_H_
#define _BLE_PRIVACY_H_

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "ble_api.h"
#include "ble_privacy.h"

/**************************************************************************************************
 *    DEFINITIONS
 *************************************************************************************************/
/**
 * @defgroup ble_privacy_param BLE Privacy Parameter Definition
 * @{
 * @ingroup ble_privacy
 * @brief Define BLE privacy parameter definition.
 */
typedef uint8_t ble_privacy_param_t;

/** Enable or Disable */
#define BLE_PRIVACY_PARAM_FLD_ENABLE            0x01
#define BLE_PRIVACY_PARAM_DISABLE               0x00
#define BLE_PRIVACY_PARAM_ENABLE                0x01

/** 0: Network Privacy Mode, 1: Device Privacy Mode */
#define BLE_PRIVACY_PARAM_FLD_MODE              0x02
#define BLE_PRIVACY_PARAM_MODE_NETWORK          0x00
#define BLE_PRIVACY_PARAM_MODE_DEVICE           0x02

/** 0: LL_Privacy is not support, 1: LL_Privacy is support */
#define BLE_PRIVACY_PARAM_FLD_LL_PRIVACY        0x04
#define BLE_PRIVACY_PARAM_LL_PRIVACY_OFF        0x00
#define BLE_PRIVACY_PARAM_LL_PRIVACY_ON         0x04

/** Reserved bits */
#define BLE_PRIVACY_PARAM_FLD_RESERVED          0xF8
/** @} */

#define BLE_ADDR_SUB_TYPE_FLD_RANDOM            0xC0
#define BLE_ADDR_SUB_TYPE_RANDOM                0x00
#define BLE_ADDR_SUB_TYPE_RANDOM_RESOLVABLE     0x40
#define BLE_ADDR_SUB_TYPE_RANDOM_RESERVED       0x80
#define BLE_ADDR_SUB_TYPE_RANDOM_STATIC         0xC0



/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/
/** @brief Set the privacy function enable.
 *
 * @ingroup ble_privacy
 *
 * @note Default "Disable".
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_privacy_enable(ble_set_privacy_cfg_t *p_param);


/** @brief Set the privacy function disable.
 *
 * @ingroup ble_privacy
 *
 * @note Default "Disable".
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_privacy_disable(void);


/** @brief check if the privacy mode set to "enable".
 *
 * @ingroup ble_privacy
 *
 * @note TRUE: enable, FALSE: disable
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
bool ble_privacy_parameter_enable_check(void);


/** @brief check if the privacy mode set to "Device Privacy Mode".
 *
 * @ingroup ble_privacy
 *
 * @note TRUE: Device Privacy Mode, FALSE: Network Privacy Mode
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
bool ble_privacy_parameter_device_mode_check(void);


/** @brief check if the privacy mode set to run "LL_privacy".
 *
 * @ingroup ble_privacy
 *
 * @note TRUE: LL_privacy enable, FALSE: LL_privacy disable
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
bool ble_privacy_parameter_LL_privacy_on_check(void);


/** @brief Get the host id when privacy enable
 *
 * @ingroup ble_privacy
 *
 * @return host_id : the link id.
 */
uint8_t ble_privacy_host_id_get(void);


#ifdef __cplusplus
};
#endif

#endif // _BLE_PRIVACY_H_

