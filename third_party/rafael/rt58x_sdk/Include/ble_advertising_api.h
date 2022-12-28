/** @file ble_advertising_api.h
 *
 */
#ifndef __BLE_ADV_API_H__
#define __BLE_ADV_API_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "ble_api.h"
#include "ble_advertising.h"

/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** @brief Set BLE advertising parameter.
 *
 * @ingroup ble_adv_api
 *
 * @note    Advertising interval Min. and Max. : @ref ADV_INTERVAL_MIN to @ref ADV_INTERVAL_MAX \n
 *          Advertising interval Min. shall be less than or equal to advertising interval Max.
 *
 * @param[in] p_param : a pointer to advertising parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_adv_param_set(ble_adv_param_t *p_param);


/** @brief Set BLE advertising data.
 *
 * @ingroup ble_adv_api
 *
 * @note BLE advertising data length shall be less than or equal to 31 bytes.
 * @note AD Type please refer to @ref ble_adv_type_t
 * @verbatim
   ||---------------------------------------------------------------------------------------------------------||
   ||                   <length octets = (1+slength_1)+...+(1+slength_N)  octets>                             ||
   ||                                    BLE advertising data                                                 ||
   ||---------------------------------------------------------------------------------------------------------||
   ||              AD Structure 1                 ||  ........ ||               AD Structure N                ||
   ||---------------------------------------------------------------------------------------------------------||
   ||<1 octet>||      <slength_1 octets>          ||           ||<1 octet>||    <slength_N octets>            ||
   ||slength_1||              Data                ||  ........ ||slength_N||              Data                ||
   ||---------------------------------------------------------------------------------------------------------||
   ||         ||<1 octets>||<(slength_1-1) octets>||           ||         ||<1 octets>||<(slength_N-1) octets>||
   ||         || AD Type  ||     AD Data          ||  ........ ||         || AD Type  ||     AD Data          ||
   ||---------------------------------------------------------------------------------------------------------||
   @endverbatim
 *
 * @param[in] p_param : a pointer to advertising data parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_adv_data_set(ble_adv_data_param_t *p_param);


/** @brief Set BLE scan response data.
 *
 * @ingroup ble_adv_api
 *
 * @note    BLE scan response data length shall be less than or equal to 31 bytes.
 *
 * @note AD Type please refer to @ref ble_adv_type_t
 * @verbatim
   ||---------------------------------------------------------------------------------------------------------||
   ||                   <length octets = (1+slength_1)+...+(1+slength_N)  octets>                             ||
   ||                                    BLE advertising data                                                 ||
   ||---------------------------------------------------------------------------------------------------------||
   ||              AD Structure 1                 ||  ........ ||               AD Structure N                ||
   ||---------------------------------------------------------------------------------------------------------||
   ||<1 octet>||      <slength_1 octets>          ||           ||<1 octet>||    <slength_N octets>            ||
   ||slength_1||              Data                ||  ........ ||slength_N||              Data                ||
   ||---------------------------------------------------------------------------------------------------------||
   ||         ||<1 octets>||<(slength_1-1) octets>||           ||         ||<1 octets>||<(slength_N-1) octets>||
   ||         || AD Type  ||     AD Data          ||  ........ ||         || AD Type  ||     AD Data          ||
   ||---------------------------------------------------------------------------------------------------------||
   @endverbatim
 *
 * @param[in] p_param : a pointer to scan response data parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_adv_scan_rsp_set(ble_adv_data_param_t *p_param);



/** @brief Set BLE start advertising.
 *
 * @ingroup ble_adv_api
 *
 * @par BLE Event
 * Wait for @ref BLECMD_EVENT_ADV_COMPLETE event which indicates the advertisement has been started.
 *
 * @param[in] p_param : a pointer to adv enable parameter.
 *                      the link's host ID or set to @ref BLE_HOSTID_RESERVED to enable @ref ADV_TYPE_ADV_NONCONN_IND or @ref ADV_TYPE_SCAN_IND advertisement.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_adv_enable(ble_adv_enable_param_t *p_param);



/** @brief Set BLE stop advertising.
 *
 * @ingroup ble_adv_api
 *
 * @par BLE Event
 * Wait for @ref BLECMD_EVENT_ADV_COMPLETE event which indicates the advertisement has been stopped.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_adv_disable(void);



/** @brief Check the advertising state is in idle mode (advertising disabled).
 *
 * @ingroup ble_adv_api
 *
 * @return true : advertising disabled.
 * @return false : advertising enabled.
 */
bool ble_adv_idle_state_check(void);



/** @brief BLE advertising events handler.
 *
 * @ingroup ble_adv_api
 *
 * @param p_param :  a pointer to the event parameter.
 *
 * @return @ref BLE_ERR_OK is success or an @ref ble_err_t "error".
 */
ble_err_t ble_evt_adv_handler(void *p_param);



#ifdef __cplusplus
};
#endif

#endif /* __BLE_ADV_API_H__*/

