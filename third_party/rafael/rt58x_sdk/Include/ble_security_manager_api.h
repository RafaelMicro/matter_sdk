/** @file       ble_security_manager_api.h
 *
 * @addtogroup BLE
 * @{
 * @defgroup ble_sec_api BLE Security APIs
 * @{
 * @brief Define BLE security related definitions, structures and functions.
 * @defgroup ble_sec BLE Security APIs
 * @}
 * @}
*/

#ifndef __BLE_SECURITY_MANAGER_API_H__
#define __BLE_SECURITY_MANAGER_API_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "ble_api.h"
#include "ble_event_app.h"
#include "ble_security_manager.h"

/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** BLE send security request.
 *
 * @ingroup ble_sec
 *
 * @param p_param : a pointer to tthe security request parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_sm_security_request_set(ble_sm_security_request_param_t *p_param);


/** Set BLE pairing passkey value.
 *
 * @ingroup ble_sec
 *
 * @param p_param : a pointer to the passkey parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_sm_passkey_set(ble_sm_passkey_param_t *p_param);


/** Set BLE IO capabilities.
 *
 * @ingroup ble_sec
 *
 * @param p_param : a pointer to the IO capability parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_sm_io_capability_set(ble_evt_sm_io_cap_t *p_param);



/** Set BLE bonding flags.
 *
 * @ingroup ble_sec
 *
 * @param p_param : a pointer to the bonding parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_sm_bonding_flag_set(ble_evt_sm_bonding_flag_t *p_param);


/** Restore BLE bonding CCCD.
 *
 * @ingroup ble_sec
 *
 * @param p_param : a pointer to the restore cccd parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_sm_cccd_restore(ble_sm_restore_cccd_param_t *p_param);


/** BLE bonding space init.
 *
 * @ingroup ble_sec
 *
 * @param none
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_sm_bonding_space_init(void);


/** BLE set identity resolving key.
 *
 * @ingroup ble_sec
 *
 * @param none
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_sm_irk_set(ble_sm_irk_param_t *p_param);


/** BLE security related events handler.
 *
 * @ingroup ble_sec
 *
 * @param p_param :  a pointer to the event parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_evt_sm_handler(void *p_param);


#ifdef __cplusplus
};
#endif

#endif /* __BLE_SECURITY_MANAGER_API_H__*/

