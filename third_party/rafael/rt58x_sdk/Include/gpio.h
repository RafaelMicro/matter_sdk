/**************************************************************************//**
 * @file     gpio.h
 * @version
 * @brief    GPIO driver header file
 *
 * @copyright
 ******************************************************************************/
/** @defgroup GPIO_Driver GPIO
 *  @ingroup  peripheral_group
 *  @{
 *  @brief  GPIO_Driver header information
*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define  MAX_NUMBER_OF_PINS    (32)           /*!< Specify Maximum Pins of GPIO   */


/**
 *  @Brief GPIO_DEBOUNCE CLOCK Constant Definitions
 */

#define  DEBOUNCE_SLOWCLOCKS_32     (0)       /*!< setting for sampling cycle = 32 clocks   */
#define  DEBOUNCE_SLOWCLOCKS_64     (1)       /*!< setting for sampling cycle = 64 clocks   */
#define  DEBOUNCE_SLOWCLOCKS_128    (2)       /*!< setting for sampling cycle = 128 clocks  */
#define  DEBOUNCE_SLOWCLOCKS_256    (3)       /*!< setting for sampling cycle = 256 clocks  */
#define  DEBOUNCE_SLOWCLOCKS_512    (4)       /*!< setting for sampling cycle = 512 clocks  */
#define  DEBOUNCE_SLOWCLOCKS_1024   (5)       /*!< setting for sampling cycle = 1024 clocks */
#define  DEBOUNCE_SLOWCLOCKS_2048   (6)       /*!< setting for sampling cycle = 2048 clocks */
#define  DEBOUNCE_SLOWCLOCKS_4096   (7)       /*!< setting for sampling cycle = 4096 clocks */

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**
 * @brief Pin direction definitions.
 */
typedef enum
{
    GPIO_PIN_DIR_INPUT,           /*!< GPIO Input Mode   */
    GPIO_PIN_DIR_OUTPUT,          /*!< GPIO Output Mode  */
    GPIO_PIN_DIR_INVALID
} gpio_pin_dir_t;

/**
 * @brief Selecting the pin to sense high or low level, edge for pin input.
 */

typedef enum
{
    GPIO_PIN_NOINT,              /*!< GPIO Interrupt mode disable  */
    GPIO_PIN_INT_LEVEL_LOW,      /*!< GPIO Interrupt enable for Level-Low */
    GPIO_PIN_INT_LEVEL_HIGH,     /*!< GPIO Interrupt enable for Level-High  */
    GPIO_PIN_INT_EDGE_RISING,    /*!< GPIO Interrupt enable for Rising Edge */
    GPIO_PIN_INT_EDGE_FALLING,   /*!< GPIO Interrupt enable for Falling Edge */
    GPIO_PIN_INT_BOTH_EDGE,      /*!< GPIO Interrupt enable for both Rising and Falling Edge  */
} gpio_pin_int_mode_t;


/**
 * @brief GPIO interrupt service routine callback for user application.
 *
 * @param[in]   pin        Interrupt pin number.
 * @param[in]   isr_param  isr_param passed to user interrupt handler.
 *                         The parameter is set by user called in function gpio_register_isr
 *                          "*param". This register could be NULL, if user does NOT require this context
 *                         parameter.
 *
 *
 * @details    This callback function is still running in interrupt mode, so this function
 *              should be as short as possible. It can NOT call any block function in this
 *              callback service routine.
 *
 */
typedef void (* gpio_isr_handler_t)(uint32_t pin, void *isr_param);


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/
/**
 * @brief Pin configuration function.
 *
 * @param[in]   pin_number    Specifies the pin number.
 * @param[in]   dir           Pin direction.
 * @param[in]   int_mode      Pin interrupt mode. Only available when dir is GPIO_PIN_DIR_INPUT
 *
 * @retval     none
 */

extern void gpio_cfg(
    uint32_t         pin_number,
    gpio_pin_dir_t   dir,
    gpio_pin_int_mode_t int_mode);

/**
 * @brief Register user interrupt ISR callback function.
 *
 * @param[in]    pin    Specifies the pin number.
 * @param[in]    gpio_isr_handler_t  Specifies user callback function when the pin interrupt generated.
 * @param[in]    param  Paramter passed to user interrupt handler "gpio_isr_handler_t"
 *
 * @retval      none
 */
extern void gpio_register_isr(
    uint32_t            pin,
    gpio_isr_handler_t  app_gpio_callback,
    void                *param);


/**
 * @brief  Set gpio pin to output mode.
 *
 * @param[in]    pin   Specifies the pin number to output mode.
 *
 * @retval      none
 * @remark     set pin to output mode will also set the pin driving to be
 *               none-pull driving mode.
 */

__STATIC_INLINE void gpio_cfg_output(uint32_t pin_number)
{
    gpio_cfg(pin_number, GPIO_PIN_DIR_OUTPUT, GPIO_PIN_NOINT);
}

/**
 * @brief  Set gpio pin to input mode.
 *
 * @param[in]  pin      Specifies the pin number to input mode.
 * @param[in]  int_mode  Specifies the pin number interrupt if this pin need to be gpio interrupt source.
 *
 * @retval    none
 * @details   set pin to input mode with interrupt mode if required. Default input pin driving
 *             is 100K pull-up, user can change this by calling function
 *             pin_set_pullopt(uint32_t pin_number, uint32_t mode)
 */

__STATIC_INLINE void gpio_cfg_input(uint32_t pin_number, gpio_pin_int_mode_t int_mode)
{
    gpio_cfg(pin_number, GPIO_PIN_DIR_INPUT, int_mode);
}

/**
 * @brief  Set gpio pin output high.
 *
 * @param[in]  pin      Specifies the pin number to output high.
 *
 * @retval    none
 */

__STATIC_INLINE void gpio_pin_set(uint32_t pin_number)
{
    assert_param(pin_number < MAX_NUMBER_OF_PINS);

    GPIO->OUTPUT_HIGH = (1 << pin_number);
}

/**
 * @brief  Set gpio pin output low.
 *
 * @param[in]  pin      Specifies the pin number to output low.
 *
 * @retval    none
 */

__STATIC_INLINE void gpio_pin_clear(uint32_t pin_number)
{
    assert_param(pin_number < MAX_NUMBER_OF_PINS);

    GPIO->OUTPUT_LOW = (1 << pin_number);
}


/**
 * @brief  Set gpio pin output value.
 *
 * @param[in]  pin      Specifies the pin number.
 * @param[in]  value    value 0 for output low, value 1 for output high.
 *
 * @retval    none
 */

__STATIC_INLINE void gpio_pin_write(uint32_t pin_number, uint32_t value)
{
    if (value == 0)
    {
        gpio_pin_clear(pin_number);
    }
    else
    {
        gpio_pin_set(pin_number);
    }
}


/**
 * @brief  Toggle gpio pin output value.
 *
 * @param[in]  pin      Specifies the pin number.
 *
 * @retval    none
 */

__STATIC_INLINE void gpio_pin_toggle(uint32_t pin_number)
{
    uint32_t state, MASK;

    assert_param(pin_number < MAX_NUMBER_OF_PINS);

    MASK = (1 << pin_number);
    state = GPIO->STATE & MASK;

    if (state)
    {
        GPIO->OUTPUT_LOW = MASK;
    }
    else
    {
        GPIO->OUTPUT_HIGH = MASK;
    }

}

/**
 * @brief  Get gpio pin input value.
 *
 * @param[in]  pin      Specifies the pin number.
 *
 * @retval    1 for input pin is high, 0 for input is low.
 */

__STATIC_INLINE uint32_t gpio_pin_get(uint32_t pin_number)
{
    assert_param(pin_number < MAX_NUMBER_OF_PINS);

    return ((GPIO->STATE & (1 << pin_number)) ? 1 : 0);
}


/**
 * @brief  Enable gpio pin interrupt
 *
 * @param[in]  pin      Specifies the pin number that enable interrupt.
 *
 * @retval    None
 */

__STATIC_INLINE void gpio_int_enable(uint32_t pin)
{
    assert_param(pin < MAX_NUMBER_OF_PINS);
    GPIO->ENABLE_INT = (1 << pin);
}

/**
 * @brief  Disable gpio pin interrupt
 *
 * @param[in]  pin      Specifies the pin number that disable interrupt.
 *
 * @retval    None
 */

__STATIC_INLINE void gpio_int_disable(uint32_t pin)
{
    assert_param(pin < MAX_NUMBER_OF_PINS);
    GPIO->DISABLE_INT = (1 << pin);
}

/**
 * @brief  Enable gpio pin debounce function.
 *
 * @param[in]  pin      Specifies the pin number that enable gpio debounce when interrupt happened.
 *
 * @retval    None
 *
 * @remark    ONLY enable this debounce function in those pins for "human input" pin, like
 *             button pressed/released, card insert/remove detected. If the pin interrupt trigger by another
 *             IC with stable high/low source, then that pin should NOT enable debounce function.
 */

__STATIC_INLINE void gpio_debounce_enable(uint32_t pin)
{
    assert_param(pin < MAX_NUMBER_OF_PINS);
    GPIO->DEBOUCE_EN = (1 << pin);
}

/**
 * @brief  Disable gpio pin debounce function.
 *
 * @param[in]  pin      Specifies the pin number that disable gpio debounce when interrupt happened.
 *
 * @retval    None
 *
 *
 */

__STATIC_INLINE void gpio_debounce_disable(uint32_t pin)
{
    assert_param(pin < MAX_NUMBER_OF_PINS);
    GPIO->DEBOUCE_DIS = (1 << pin);
}

/**
 * @brief  Set GPIO debounce clock.
 *
 * @param[in]  mode   Specifies the sampling clock of debounce function.
 *
 * @retval    None
 */

__STATIC_INLINE void gpio_set_debounce_time(uint32_t mode)
{
    assert_param(mode < DEBOUNCE_SLOWCLOCKS_4096);

    if (mode > DEBOUNCE_SLOWCLOCKS_4096)
    {
        mode = DEBOUNCE_SLOWCLOCKS_4096;
    }

    GPIO->DEBOUNCE_TIME = mode;
}

/*@}*/ /* end of peripheral_group GPIO_Driver */

#ifdef __cplusplus
}
#endif


#endif /* end of __GPIO_H__ */

