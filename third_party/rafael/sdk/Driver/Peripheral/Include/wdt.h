/** @file wdt.h
 *
 * @brief Watchdog driver header file.
 *
 */
/**
* @defgroup wdt_group WDT
* @ingroup peripheral_group
* @{
* @brief  Define Watchdog Timer definitions, structures, and functions.
*/
#ifndef __WDT_TIMER_H__
#define __WDT_TIMER_H__

#include "cm3_mcu.h"
#ifdef __cplusplus
extern "C"
{
#endif
/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define WDT_PRESCALE_1               0
#define WDT_PRESCALE_16              1
#define WDT_PRESCALE_32              3
#define WDT_PRESCALE_128             4
#define WDT_PRESCALE_256             2
#define WDT_PRESCALE_1024            5
#define WDT_PRESCALE_4096            6
/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**
 * @brief WDT TIMER interrupt service routine callback for user application.
 *
 *
 */
typedef void (*wdt_isr_handler_t)();

/**@brief wdt config structure holding configuration settings for the wdt timer.
 */
typedef struct
{
    uint8_t    int_enable     : 1;
    uint8_t    reset_enable   : 1;
    uint8_t    lock_enable    : 1;
    uint8_t    prescale       : 3;
} wdt_config_mode_t;


/**@brief wdt config structure holding configuration settings for the wdt timer.
 */
typedef struct
{
    uint32_t    wdt_ticks;
    uint32_t    int_ticks;
    uint32_t    wdt_min_ticks;

} wdt_config_tick_t;

/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/
/*Remark:
 *   WDT interrupt is "warning interrupt", that notify system it will reset soon.
 * so int_ticks should be less that wdt_ticks and should have enough time for
 * "reset error" handling.. int_ticks can NOT greater or equal then wdt_ticks.
 *
 *   If you set lock_enable mode, you can NOT change the watchdog setting anymore
 * before reset (after WDT lock_enable bit has been set). That is you must kick
 * the watchdog periodic to prevent reset event.
 *
 * Example:
 * Clock is 32M Hz. Set prescale is 32.. so WDT is 1M Hz.
 * If we set wdt_ticks 500000, and int_ticks 300
 * then we should kick WDT within 0.5 second, otherwise
 * the whole system will reset (if reset enable) after 0.5s
 * from the last kick and before reset. And before reset,
 * a warning reset interrupt will generate, system has 300 us
 * to save some important data or something..
 * We can ignore int_ticks and wdt_handler, if we don't set
 * int_enable mode.
 *
 */
/**
* @ingroup wdt_group
* @brief register Watch dog interrupt callback function
* @param[in] wdt_handler
* @return None
*/
extern void Wdt_Int_Callback_Register(wdt_isr_handler_t wdt_handler);

/**
 * @brief Watch dog Start
 * @param[in] wdt_mode config the watch dog
              \arg int_enable
              \arg reset_enable
              \arg lock_enable
              \arg prescale
 * @param[in] wdt_cfg_ticks config the watch dog ticks
              \arg wdt_ticks        setting the watchdog reset ticks
              \arg int_ticks        setting the watchdog interrupt ticks
              \arg wdt_min_ticks    setting the watchdog window min ticks
 * @param[in] wdt_handler register watch dog interrupt service routine callback
 * @return
 */
extern uint32_t Wdt_Start(
    wdt_config_mode_t wdt_mode,
    wdt_config_tick_t wdt_cfg_ticks,
    wdt_isr_handler_t wdt_handler);

/**
 * @brief Stop the watchdog
 * @retval STATUS_INVALID_REQUEST watchdog control lock mode is true, then can't clear the watchdog enable flag
 * @retval STATUS_SUCCESS clear the watchdog enable flag is successful
 */
extern uint32_t Wdt_Stop(void);

/**
 * @brief Get watchdog reset event value
 * @return Watchdog reset event value
 */
__STATIC_INLINE  uint32_t Wdt_Reset_Event_Get(void)
{
    return WDT->RST_OCCUR.reg;
}

/**
 * @brief Clear Watchdog Reset Event
 * @return None
 */
__STATIC_INLINE void Wdt_Reset_Event_Clear(void)
{
    WDT->RST_OCCUR.bit.RESET_OCCUR = 1;     /*Write clear.*/
}

extern uint32_t Wdt_Stop(void);

/**
 * @brief Reload the watchdog Kick value
 * @return None
 */
__STATIC_INLINE void Wdt_Kick(void)
{
    WDT->WDT_KICK = WDT_KICK_VALUE;
}

/**
 * @brief Clear Watchdog interrupt flag
 * @return Watch dog value
 */
__STATIC_INLINE void Wdt_Interrupt_Clear(void)
{
    WDT->CLEAR = 1;
}
/**
 * @brief Clear Watchdog interrupt flag
 * @return return Watchdog value
 */
__STATIC_INLINE uint32_t Wdt_Current_Get(void)
{
    return (WDT->VALUE);
}

#ifdef __cplusplus
}
#endif

#endif /* End of _WDT_H_ */

/** @} */ /* End of Peripheral Group */

