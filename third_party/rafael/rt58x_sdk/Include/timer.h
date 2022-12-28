/**************************************************************************//**
 * @file     timer.h
 * @version
 * @brief    TIMER driver header file
 *
 * @copyright
 ******************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief TIMER interrupt service routine callback for user application.
 *
 * @param timer_id   is timer id for the interrupt source
 *
 */
typedef void (*timer_isr_handler_t)(uint32_t timer_id);


#define TIMER_PERIODIC_MODE            1
#define TIMER_FREERUN_MODE             0

#define TIMER_PRESCALE_1               0
#define TIMER_PRESCALE_2               3
#define TIMER_PRESCALE_8               4
#define TIMER_PRESCALE_16              1
#define TIMER_PRESCALE_32              5
#define TIMER_PRESCALE_128             6
#define TIMER_PRESCALE_256             2
#define TIMER_PRESCALE_1024            7

/**@brief timer config structure holding configuration settings for the timer.
 */
typedef struct
{
    uint8_t    mode     : 1;
    uint8_t    prescale : 3;
    uint8_t    int_en   : 1;
} timer_config_mode_t;


/*Notice:
 * Freerun mode will NOT generated interupt. Freerun mode can ONLY
 * use polling to get timer counter...
 * If timer_handler is not NULL, system will call timer_handler
 *when timeout happen (periodic interrupt mode).
 */
extern void Timer_Int_Callback_Register(uint32_t timer_id,
                                        timer_isr_handler_t timer_handler);

extern uint32_t Timer_Int_Priority(uint32_t timer_id,
                                   uint32_t timer_priority);

extern uint32_t Timer_Open(uint32_t timer_id,
                           timer_config_mode_t mode,
                           timer_isr_handler_t timer_handler);

extern uint32_t Timer_Load(uint32_t timer_id, uint32_t timeout_ticks);

/*  For example: if timer running in 32MHz. And prescale is 1
 * timeout_ticks as 320000 in predioc mode,
 * it will generate 10 ms interrupt.
 */
extern uint32_t Timer_Start(uint32_t timer_id,
                            uint32_t timeout_ticks);

extern uint32_t Timer_Close(uint32_t timer_id);

extern uint32_t Timer_Stop(uint32_t timer_id);

/*
 * This function will return the current tick.
 * Please note: the tick is down counter.
 */
extern uint32_t Timer_Current_Get(uint32_t timer_id);

#ifdef __cplusplus
}
#endif

#endif

