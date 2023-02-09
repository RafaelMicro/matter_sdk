/** @file bsp_button.c
 *
 * @license
 * @description
 */

#include <stdint.h>
#include "bsp.h"
#include "cm3_mcu.h"
#include "bsp_button.h"
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
static bsp_event_callback_t m_callback;
static bsp_button_cfg_t m_button_cfg[BSP_BUTTON_COUNT] =
{
#ifdef BSP_BUTTON_0
    {BSP_BUTTON_0, GPIO_PIN_INT_BOTH_EDGE},
#endif
#ifdef BSP_BUTTON_1
    {BSP_BUTTON_1, GPIO_PIN_INT_BOTH_EDGE},
#endif
#ifdef BSP_BUTTON_2
    {BSP_BUTTON_2, GPIO_PIN_INT_BOTH_EDGE},
#endif
#ifdef BSP_BUTTON_3
    {BSP_BUTTON_3, GPIO_PIN_INT_BOTH_EDGE},
#endif
#ifdef BSP_BUTTON_4
    {BSP_BUTTON_4, GPIO_PIN_INT_BOTH_EDGE},
#endif
};
//=============================================================================
//                  Private Function Definition
//=============================================================================
static void bsp_button_isr_event(uint32_t pin, void *isr_param)
{
    if (m_callback == NULL)
    {
        return ;
    }
    if (pin == BSP_BUTTON_0)
    {
        m_callback(BSP_EVENT_BUTTONS_0);
    }
    else if (pin == BSP_BUTTON_1)
    {
        m_callback(BSP_EVENT_BUTTONS_1);
    }
    else if (pin == BSP_BUTTON_2)
    {
        m_callback(BSP_EVENT_BUTTONS_2);
    }
    else if (pin == BSP_BUTTON_3)
    {
        m_callback(BSP_EVENT_BUTTONS_3);
    }
#ifdef BSP_BUTTON_4
    else if (pin == BSP_BUTTON_4)
    {
        m_callback(BSP_EVENT_BUTTONS_4);
    }
#endif
}
//=============================================================================
//                  Public Function Definition
//=============================================================================
int bsp_button_init(bsp_event_callback_t callback)
{
    int     rval = 0;
    int i;
    for (i = 0; i < BSP_BUTTON_COUNT; i++)
    {
        gpio_cfg_input((uint32_t)m_button_cfg[i].pin_no, (gpio_pin_int_mode_t)m_button_cfg[i].active_state);
        gpio_register_isr(m_button_cfg[i].pin_no, bsp_button_isr_event, NULL);
        gpio_debounce_enable(m_button_cfg[i].pin_no);
        gpio_int_enable(m_button_cfg[i].pin_no);
    }
    NVIC_SetPriority(Gpio_IRQn, 6);

    m_callback = callback;

    return rval;
}

int bsp_button_state_get(uint32_t button)
{
    return gpio_pin_get(button);
}
