/** @file bsp_button.c
 *
 * @license
 * @description
 */

#include <stdint.h>
#include "cm3_mcu.h"
#include "bsp.h"

#include "bsp_led.h"
//=============================================================================
//                  Constant Definition
//=============================================================================

//=============================================================================
//                  Macro Definition
//=============================================================================

//=============================================================================
//                  Structure Definition
//=============================================================================

//=============================================================================
//                  Global Data Definition
//==============================================================================
//static bsp_event_callback_t m_callback;
static bsp_led_cfg_t m_led_cfg[2] =
{
#ifdef BSP_LED_0
    {BSP_LED_0},
#endif
#ifdef BSP_LED_1
    {BSP_LED_1},
#endif

};
//=============================================================================
//                  Private Function Definition
//=============================================================================

//=============================================================================
//                  Public Function Definition
//=============================================================================
int bsp_led_init(bsp_event_callback_t callback)
{
    int rval = 0;
    int i;
    for (i = 0; i < BSP_LED_COUNT; i++)
    {
        gpio_cfg_output(m_led_cfg[i].pin_no);
        bsp_led_Off(m_led_cfg[i].pin_no);
    }

    //m_callback = callback;

    return rval;
}
void bsp_led_on(uint32_t led)
{
    gpio_pin_write(led, BSP_LED_ACTIVE_STATE);
}
void bsp_led_Off(uint32_t led)
{
    gpio_pin_write(led, !BSP_LED_ACTIVE_STATE);
}
void bsp_led_toggle(uint32_t led)
{
    gpio_pin_toggle(led);
}
