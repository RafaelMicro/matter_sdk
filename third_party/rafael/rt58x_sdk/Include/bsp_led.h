/**
 * @file bsp_led.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */


/**
* @defgroup BSP_LED LED handler API Definition
* Define LED handler definitions, structures, and functions.
* @ingroup BSP
* @{
*/



#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "bsp.h"

#define BSP_LED_COUNT   2
#define BSP_LED_0       20
#define BSP_LED_1       21
#define BSP_LED_ACTIVE_STATE    0

typedef struct
{
    uint32_t pin_no;
} bsp_led_cfg_t;


/**
 * @brief               Initial the LED.
 *
 * @param callback      Allway set NULL.
 * @return int          0 will be returned if the initialized was successfully.
 */
int bsp_led_init(bsp_event_callback_t callback);

/**
 * @brief           Set the target LED state ON.
 *
 * @param led       Target LED.
 */
void bsp_led_on(uint32_t led);

/**
 * @brief           Set the target LED state OFF.
 *
 * @param led       Target LED.
 */
void bsp_led_Off(uint32_t led);

/**
 * @brief           Set the target LED state toggle.
 *
 * @param led       Target LED.
 */
void bsp_led_toggle(uint32_t led);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
