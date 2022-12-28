/**
 * @file bsp_button.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
* @defgroup BSP_BUTTON BSP Button handler API Definition
* Define BSP Button handler definitions, structures, and functions.
* @ingroup BSP
* @{
*/



#ifndef __BSP_BUTTON_H__
#define __BSP_BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"

#if (BOARD==A047)
#define BSP_BUTTON_COUNT        4
#define BSP_BUTTON_0            14
#define BSP_BUTTON_1            15
#define BSP_BUTTON_2            30
#define BSP_BUTTON_3            31
#else
#define BSP_BUTTON_COUNT        5
#define BSP_BUTTON_0            0
#define BSP_BUTTON_1            1
#define BSP_BUTTON_2            2
#define BSP_BUTTON_3            3
#define BSP_BUTTON_4            4
#endif


/**
 * @brief BSP button instance.
 *
 */
typedef struct
{
    uint32_t pin_no;
    uint32_t active_state;
} bsp_button_cfg_t;

/**
 * @brief               Initial the button.
 *
 * @param callback      The function to call when the button trigger.
 * @return int          0 will be returned if the initialized was successfully.
 */
int bsp_button_init(bsp_event_callback_t callback);

/**
 * @brief               Get the button state.
 *
 * @param button        Button to get the state.
 * @return int          The state of the button.
 */
int bsp_button_state_get(uint32_t button);


/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
