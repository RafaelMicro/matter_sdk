/** @file ble_privacy.h
 *
 *  * @brief BLE Privacy header file.
 *
 */

/**
 * @defgroup ble_privacy BLE Privacy
 * @ingroup BLE_group
 * @{
 * @brief Define BLE Privacy definitions, structures, and functions.
 * @}
 */

#ifndef __BLE_PRIVACY_H__
#define __BLE_PRIVACY_H__

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

/** @defgroup ble_privacy_mode BLE Privacy mode Definition
 * @{
 * @ingroup ble_privacy
 * @details  Define different BLE Privacy mode.
 */
typedef uint8_t ble_privacy_mode_t;
#define NETWORK_PRIVACY_MODE                    0x00  /**< Use network privacy mode.  */
#define DEVICE_PRIVACY_MODE                     0x01  /**< Use device privacy mode.  */
/** @} */



/** @brief Set BLE Privacy mode parameter.
 * @ingroup ble_privacy
*/
typedef struct ble_set_privacy_cfg_s
{
    uint8_t                 host_id;            /**< Host id.*/
    ble_privacy_mode_t      privacy_mode;       /**< Privacy mode selection. */
} ble_set_privacy_cfg_t;


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

/** @brief BLE privacy enable function.
 *
 * @ingroup ble_privacy
 *
 * @param[in] p_param : a pointer to the privacy configuration value.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_privacy_enable(ble_set_privacy_cfg_t *p_param);

/** @brief BLE privacy disable function.
 *
 * @ingroup ble_privacy
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_privacy_disable(void);



#ifdef __cplusplus
};
#endif

#endif /* __BLE_PRIVACY_H__*/

