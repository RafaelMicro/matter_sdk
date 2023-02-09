/** @file ble_security_manager.h
 *
 * @brief BLE security manager header file.
 *
 */

/**
 * @defgroup ble_sec BLE Security Management
 * @ingroup BLE_group
 * @{
 * @brief Define BLE Security definitions, structures, and functions.
 * @}
 */

#ifndef __BLE_SECURITY_MANAGER_H__
#define __BLE_SECURITY_MANAGER_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include "ble_api.h"

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/**
 * @defgroup ble_sec_key_gen_method BLE Security key generation method Definition
 * @{
 * @ingroup ble_sec
 * @brief  Define BLE security key generation method.
 */
typedef uint8_t ble_sm_key_gen_method_t;
#define PASSKEY_ENTRY                               0x01    /**< Entry only.   */
#define PASSKEY_DISPLAY                             0x02    /**< Display only. */
/** @} */


/**
 * @defgroup ble_sec_io_caps BLE Security IO Capabilities Definition
 * @{
 * @ingroup ble_sec
 * @brief  Define BLE IO capabilities.
 */
typedef uint8_t io_caps_t;
#define DISPLAY_ONLY                                 0x00  /**< Display only.              */
#define DISPLAY_YESNO                                0x01  /**< Display and Yes/No entry.  */
#define KEYBOARD_ONLY                                0x02  /**< Keyboard only.             */
#define NOINPUT_NOOUTPUT                             0x03  /**< No IO capabilities.        */
#define KEYBOARD_DISPLAY                             0x04  /**< Keyboard and display.      */
/** @} */


/**
 * @defgroup ble_sec_flag BLE Bonding Flag Definition
 * @{
 * @ingroup ble_sec
 * @brief  Define BLE bonding flags.
 */
typedef uint8_t bonding_flag_t;
#define NO_BONDING                                   0x00  /**< NO Bonding.  */
#define BONDING                                      0x01  /**< Bonding.     */
/** @} */


/**
 * @brief BLE bonding flags parameter.
 * @ingroup ble_sec
 */
typedef struct
{
    uint8_t   host_id;
} ble_sm_security_request_param_t;


/**
 * @brief BLE restore cccd parameter.
 * @ingroup ble_sec
 */
typedef struct
{
    uint8_t   host_id;
} ble_sm_restore_cccd_param_t;


/**
 * @brief BLE passkey parameter.
 * @ingroup ble_sec
 */
typedef struct
{
    uint8_t   host_id;
    uint32_t  passkey;
} ble_sm_passkey_param_t;


/**
 * @brief BLE io capabilities parameter.
 * @ingroup ble_sec
 */
typedef struct
{
    io_caps_t          io_caps_param;           /**< @ref io_caps_t "IO capabilities". */

} ble_sm_io_cap_param_t;


/**
 * @brief BLE bonding flag parameter.
 * @ingroup ble_sec
 */
typedef struct
{
    bonding_flag_t     bonding_flag;            /**< @ref bonding_flag_t "Bonding flags". */

} ble_sm_bonding_flag_param_t;


/**
 * @brief BLE identity resolving key parameter.
 * @ingroup ble_sec
 */
typedef struct
{
    uint8_t   irk[16];
} ble_sm_irk_param_t;


/**
 * @brief STK Generator Method Event Parameters.
 * @ingroup ble_sec
*/
typedef struct __attribute__((packed)) ble_evt_sm_stk_gen_method_s
{
    uint8_t                   host_id;          /**< Host id. */
    ble_sm_key_gen_method_t   key_gen_method;   /**< @ref PASSKEY_ENTRY or @ref PASSKEY_DISPLAY . */
} ble_evt_sm_stk_gen_method_t;


/**
 * @brief Passkey Confirmation Event Parameters.
 * @ingroup ble_sec
*/
typedef struct __attribute__((packed)) ble_evt_sm_passkey_confirm_param_s
{
    uint8_t      host_id;               /**< Host id. */
} ble_evt_sm_passkey_confirm_param_t;


/**
 * @brief Authentication Event Parameters.
 * @ingroup ble_sec
*/
typedef struct __attribute__((packed)) ble_evt_sm_auth_status_s
{
    uint8_t      host_id;               /**< Host id. */
    uint8_t      status;                /**< Status. */
} ble_evt_sm_auth_status_t;


/**
 * @brief Resolving Private Address Fail Status Event Parameters.
 * @ingroup ble_sec
*/
typedef struct __attribute__((packed)) ble_evt_sm_irk_resolving_fail_s
{
    uint8_t      host_id;               /**< Host id. */
} ble_evt_sm_irk_resolving_fail_t;

/**
 * @brief BLE IO Capabilities event parameter.
 * @ingroup ble_sec
*/
typedef struct __attribute__((packed)) ble_evt_sm_io_cap_s
{
    io_caps_t          io_caps_param;           /**< @ref io_caps_t "IO capabilities". */

} ble_evt_sm_io_cap_t;


/** @brief BLE Bonding Flags event parameter.
 * @ingroup ble_sec
*/
typedef struct __attribute__((packed)) ble_evt_sm_bonding_flag_s
{
    bonding_flag_t     bonding_flag;            /**< @ref bonding_flag_t "Bonding flags". */

} ble_evt_sm_bonding_flag_t;


/**
 * @brief BLE Security Manager Event Parameters.
 * @ingroup ble_sec
*/
typedef struct ble_evt_sm_s
{
    union
    {
        ble_evt_sm_stk_gen_method_t          evt_stk_gen_method;         /**< STK generation method event parameters. */
        ble_evt_sm_passkey_confirm_param_t   evt_passkey_confirm_param;  /**< Passkey confirmation event parameters. */
        ble_evt_sm_auth_status_t             evt_auth_status;            /**< Authentication event parameters. */
        ble_evt_sm_irk_resolving_fail_t      evt_irk_resolving_fail;     /**< Resolving private address fail status event parameters. */
        ble_evt_sm_io_cap_t                  evt_io_caps;                /**< IO Capabilities event parameters. */
        ble_evt_sm_bonding_flag_t            evt_bonding_flag;           /**< Bonding flag event parameters. */
    } param;                                                          /**< Event parameters. */

} ble_evt_sm_t;

/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

/** BLE send security request.
 *
 * @ingroup ble_sec
 *
 * @param host_id : The link's host ID.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_security_request_set(uint8_t host_id);


/** Set BLE pairing passkey value.
 *
 * @ingroup ble_sec
 *
 * @param p_param : a pointer to the passkey parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_passkey_set(ble_sm_passkey_param_t *p_param);


/** Set BLE IO capabilities.
 *
 * @ingroup ble_sec
 *
 * @param p_param : a pointer to the IO capability parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_io_capability_set(ble_sm_io_cap_param_t *p_param);


/** Set BLE bonding flags.
 *
 * @ingroup ble_sec
 *
 * @param p_param : a pointer to the bonding parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_bonding_flag_set(ble_sm_bonding_flag_param_t *p_param);


/** Restore BLE bonding CCCD.
 *
 * @ingroup ble_sec
 *
 * @param host_id : The link's host ID.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_cccd_restore(uint8_t host_id);


/** Initialization Bonding Space command.
 *
 * @ingroup ble_sec
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_bonding_space_init(void);


/** Set BLE Identity Resolving Key command.
 *
 * @ingroup ble_sec
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_cmd_write_identity_resolving_key(ble_sm_irk_param_t *p_param);


#ifdef __cplusplus
};
#endif

#endif /* __BLE_SECURITY_MANAGER_H__*/

