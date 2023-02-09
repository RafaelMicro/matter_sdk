/**************************************************************************//**
 * @file     gpio.c
 * @version
 * @brief
 *
 * @copyright
*****************************************************************************/

#include "cm3_mcu.h"


/*should we allocate the space for every pin??
  since most pins are not used...
  To save space, maybe we need to remember gpio number */
typedef struct
{
    gpio_isr_handler_t   gpio_handler;           /**< user application ISR handler. */
    void                 *p_context;             /**< . */
} gpio_config_t;

static gpio_config_t   user_isr[MAX_NUMBER_OF_PINS];

void gpio_cfg(
    uint32_t             pin_number,
    gpio_pin_dir_t       dir,
    gpio_pin_int_mode_t  int_mode)
{
    uint32_t  MASK;

    assert_param(pin_number < MAX_NUMBER_OF_PINS);
    assert_param(dir < GPIO_PIN_DIR_INVALID);

    MASK = (1 << pin_number);

    if (dir == GPIO_PIN_DIR_INPUT)
    {
        GPIO->INPUT_EN = MASK;
    }
    else
    {
        GPIO->OUTPUT_EN = MASK;

        /*   For output we should set the pin to no pull mode
         * Because for pull high mode if system enter to sleep/deep sleep,
         * and this pin output low will cause power leakage.
         */
        pin_set_pullopt(pin_number, MODE_PULL_NONE);
    }

    /*should we set interrupt mode to default first?*/
    user_isr[pin_number].gpio_handler = NULL;

    switch (int_mode)
    {
    case GPIO_PIN_INT_LEVEL_LOW:
        GPIO->NEGATIVE   = MASK;
        GPIO->LEVEL_TRIG = MASK;
        break;

    case GPIO_PIN_INT_LEVEL_HIGH:
        GPIO->POSTITIVE  = MASK;
        GPIO->LEVEL_TRIG = MASK;
        break;

    case GPIO_PIN_INT_EDGE_RISING:
        GPIO->POSTITIVE  = MASK;
        GPIO->EDGE_TRIG  = MASK;
        break;

    case GPIO_PIN_INT_EDGE_FALLING:
        GPIO->NEGATIVE   = MASK;
        GPIO->EDGE_TRIG  = MASK;
        break;

    case GPIO_PIN_INT_BOTH_EDGE:
        GPIO->BOTH_EDGE_EN = MASK;
        GPIO->EDGE_TRIG  = MASK;
        break;

    case GPIO_PIN_NOINT:
    default:
        GPIO->DISABLE_INT = MASK;
        break;
    }

    return;
}


void gpio_register_isr(
    uint32_t pin,
    gpio_isr_handler_t  app_gpio_callback,
    void    *param)
{
    assert_param(pin < MAX_NUMBER_OF_PINS);
    assert_param(app_gpio_callback);

    user_isr[pin].gpio_handler = app_gpio_callback;
    user_isr[pin].p_context    = param;

    /*we enable Global intterup here.*/
    NVIC_EnableIRQ(Gpio_IRQn);
    return;
}
/**
 * @ingroup GPIO_Driver
 * @brief GPIO interrupt
 * @details
 * @return
 */
void gpio_handler(void)
{
    uint32_t  irq_state;
    uint32_t  i = 0, Mask = 1;

    gpio_isr_handler_t   app_isr;

    irq_state = GPIO->INT_STATUS;
    //dprintf("get irq_state %08x, pin state %8x \n", irq_state, GPIO->STATE);

    for (i = 0; i < MAX_NUMBER_OF_PINS; i++, Mask <<= 1)
    {
        if (irq_state & Mask)
        {
            app_isr = user_isr[i].gpio_handler;

            assert_param(app_isr);      /*it should not be NULL!*/

            /*clear Edgeinterrupt status..
             * if the interrupt source is level trigger, this clear
             * does NOT have change...
             */
            GPIO->EDGE_INT_CLR = Mask;

            if (app_isr != NULL)
            {
                app_isr(i, user_isr[i].p_context);
            }
        }
    }

    return;
}



