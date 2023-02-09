/**************************************************************************//**
 * @file     rtc.h
 * @version
 * @brief    RTC driver header file
 *
 * @copyright
 ******************************************************************************/
/** @defgroup RTC_Driver RTC
 *  @ingroup  peripheral_group
 *  @{
 *  @brief  RTC Driver header information
 *
 *  @details
 *  RTC_MODE_EVERY_SEC_INTERRUPT mode will trigger rtc "second interrupt" for
 *  every "second" if RTC_MODE_EN_SEC_INTERRUPT mode set.
 *
 *  RTC_MODE_MATCH_SEC_INTERRUPT mode will only trigger rtc "second interrupt"
 * "current second" = "alarm second" once per minute, if RTC_MODE_EN_SEC_INTERRUPT
 *  mode set.
 *
 *  RTC_MODE_SECOND_EVENT_INTERRUPT will generate "event interupt" every SS matched,
 *  if RTC_MODE_EVENT_INTERRUPT should be enable.
 *
 *  RTC_MODE_EVERY_MIN_INTERRUPT mode will trigger rtc "min interrupt" for every
 * "minute" if RTC_MODE_EN_MIN_INTERRUPT mode set.
 *
 *  RTC_MODE_MATCH_MIN_INTERRUPT mode will only trigger rtc "minute interrupt"
 * "current minute" = "alarm minute" once per hour, if RTC_MODE_EN_MIN_INTERRUPT
 *  mode set.
 *
 *  RTC_MODE_MINUTE_EVENT_INTERRUPT will generate "event interupt" for MM:SS matched.
 *  (RTC_MODE_SECOND_EVENT_INTERRUPT should NOT set for this option. And
 *  RTC_MODE_EVENT_INTERRUPT should be enable.)
 *
 *  RTC_MODE_EVERY_HOUR_INTERRUPT mode will trigger rtc "hour interrupt" for every
 * "HOUR" if RTC_MODE_EN_HOUR_INTERRUPT mode set.
 *
 *  RTC_MODE_MATCH_HOUR_INTERRUPT mode will only trigger rtc "hour interrupt"
 * "current hour" = "alarm hour" once per day, if RTC_MODE_EN_HOUR_INTERRUPT mode set.
 *
 *  RTC_MODE_HOUR_EVENT_INTERRUPT will generate "event interupt"for HH:MM:SS matched.
 *  (RTC_MODE_MIN_EVENT_INTERRUPT and  RTC_MODE_SEC_EVENT_INTERRUPT should
 *   NOT set for this option.  And RTC_MODE_EVENT_INTERRUPT should be enable.)
 *
 *  RTC_MODE_EVERY_DAY_INTERRUPT mode will trigger rtc "day interrupt" for every
 * "DAY" if RTC_MODE_EN_DAY_INTERRUPT mode set.
 *
 *  RTC_MODE_MATCH_DAY_INTERRUPT mode will only trigger rtc "day interrupt"
 * "current day" = "alarm day" once per month, if RTC_MODE_EN_DAY_INTERRUPT mode set.
 *
 *  RTC_MODE_DAY_EVENT_INTERRUPT will generate "event interupt" for DAY HH:MM:SS matched
 *  (RTC_MODE_HOUR_EVENT_INTERRUPT, RTC_MODE_MIN_EVENT_INTERRUPT and
 *   RTC_MODE_SEC_EVENT_INTERRUPT should NOT set for this option.
 *   And RTC_MODE_EVENT_INTERRUPT should be enable.)
 *
 *  RTC_MODE_EVERY_MONTH_INTERRUPT mode will trigger rtc "month interrupt" for every
 * "month" if RTC_MODE_EN_MONTH_INTERRUPT mode set.
 *
 *  RTC_MODE_MATCH_MONTH_INTERRUPT mode will only trigger rtc "month interrupt"
 * "current month" = "alarm month" once per year, if RTC_MODE_EN_MONTH_INTERRUPT mode set.
 *
 *  RTC_MODE_MONTH_EVENT_INTERRUPT will generate "event interupt" for MON:DAY HH:MM:SS matched.
 *  (RTC_MODE_DAY_EVENT_INTERRUPT, RTC_MODE_HOUR_EVENT_INTERRUPT,
 *   RTC_MODE_MIN_EVENT_INTERRUPT and RTC_MODE_SEC_EVENT_INTERRUPT should
 *   NOT set for this option. And RTC_MODE_EVENT_INTERRUPT should be enable.)
 *
 *  RTC_MODE_EVERY_YEAR_INTERRUPT mode will trigger rtc "year interrupt" for every
 * "year" if RTC_MODE_EN_YEAR_INTERRUPT mode set.
 *
 *  RTC_MODE_MATCH_YEAR_INTERRUPT mode will only trigger rtc "year interrupt"
 * "current year" = "alarm year" once per century, if RTC_MODE_EN_YEAR_INTERRUPT mode set.
 *
 *  RTC_MODE_MONTH_EVENT_INTERRUPT will generate "event interupt" for YEAR:MON:DAY HH:MM:SS matched.
 *  (RTC_MODE_MONTH_EVENT_INTERRUPT, RTC_MODE_DAY_EVENT_INTERRUPT, RTC_MODE_HOUR_EVENT_INTERRUPT,
 *   RTC_MODE_MIN_EVENT_INTERRUPT and RTC_MODE_SEC_EVENT_INTERRUPT should
 *   NOT set for this option.)
 *
 *
*/
#ifndef ___RTC_H__
#define ___RTC_H__

#ifdef __cplusplus
extern "C"
{
#endif


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
/**

 */
#define RTC_MODE_EVERY_SEC_INTERRUPT         (1<<0)         /*!< Geneate second interrupt for every new seconds   */
#define RTC_MODE_MATCH_SEC_INTERRUPT         (0<<0)         /*!< Geneate second interrupt only for matched second of every minutes   */
#define RTC_MODE_SECOND_EVENT_INTERRUPT      (1<<1)         /*!< Geneate event interrupt for matched second of every munutes  */

#define RTC_MODE_EVERY_MIN_INTERRUPT         (1<<2)         /*!< Geneate minute interrupt for every new minutes  (**:00)   */
#define RTC_MODE_MATCH_MIN_INTERRUPT         (0<<2)         /*!< Geneate minute interrupt only for matched minutes  (mm:00) of every hours, mm is the value set in alarm minute  */
#define RTC_MODE_MINUTE_EVENT_INTERRUPT      (1<<3)         /*!< Geneate event interrupt for matched minutes  (mm:ss) of every hours  */

#define RTC_MODE_EVERY_HOUR_INTERRUPT        (1<<4)         /*!< Geneate hour interrupt for every new hours (**:00:00)  */
#define RTC_MODE_MATCH_HOUR_INTERRUPT        (0<<4)         /*!< Geneate hour interrupt only for matched hours (hh:00:00) of every days, HH is the value set in alarm hour */
#define RTC_MODE_HOUR_EVENT_INTERRUPT        (1<<5)         /*!< Geneate event interrupt only for matched hours (hh:mm:ss) of every days  */

#define RTC_MODE_EVERY_DAY_INTERRUPT         (1<<6)         /*!< Geneate day interrupt for every new day (00:00:00)   */
#define RTC_MODE_MATCH_DAY_INTERRUPT         (0<<6)         /*!< Geneate day interrupt only for match day of months ( dd 00:00:00)   */
#define RTC_MODE_DAY_EVENT_INTERRUPT         (1<<7)         /*!< Geneate event interrupt for matched day (dd hh:mm:ss)   */

#define RTC_MODE_EVERY_MONTH_INTERRUPT       (1<<8)         /*!< Geneate month interrupt for every new month ( **:00 00:00:00)   */
#define RTC_MODE_MATCH_MONTH_INTERRUPT       (0<<8)         /*!< Geneate month interrupt only for matched month of years ( mm:00 00:00:00)   */
#define RTC_MODE_MONTH_EVENT_INTERRUPT       (1<<9)         /*!< Geneate event interrupt for matched month (mm:dd hh:mm:ss)  */

#define RTC_MODE_EVERY_YEAR_INTERRUPT        (1<<10)        /*!< Geneate year interrupt for every new year ( **:00:00 00:00:00)   */
#define RTC_MODE_MATCH_YEAR_INTERRUPT        (0<<10)        /*!< Geneate year interrupt only for matched year of century ( YY:00:00 00:00:00)  */
#define RTC_MODE_YEAR_EVENT_INTERRUPT        (1<<11)        /*!< Geneate event interrupt for matched year:month:day hour:minute:second (yy:mm:dd hh:mm:ss)  */

#define RTC_INTERRUPT_MASK_SHIFT             (16)
#define RTC_INTERRUPT_MASK                   (0x7F)

#define RTC_MODE_EN_SEC_INTERRUPT            (1<< RTC_INTERRUPT_MASK_SHIFT)             /*!< Geneate Second interrupt  */
#define RTC_MODE_EN_MIN_INTERRUPT            (1<<(RTC_INTERRUPT_MASK_SHIFT+1))          /*!< Geneate Minute interrupt  */
#define RTC_MODE_EN_HOUR_INTERRUPT           (1<<(RTC_INTERRUPT_MASK_SHIFT+2))          /*!< Geneate Hour   interrupt  */
#define RTC_MODE_EN_DAY_INTERRUPT            (1<<(RTC_INTERRUPT_MASK_SHIFT+3))          /*!< Geneate Day    interrupt  */
#define RTC_MODE_EN_MONTH_INTERRUPT          (1<<(RTC_INTERRUPT_MASK_SHIFT+4))          /*!< Geneate Month  interrupt  */
#define RTC_MODE_EN_YEAR_INTERRUPT           (1<<(RTC_INTERRUPT_MASK_SHIFT+5))          /*!< Geneate Year   interrupt  */
#define RTC_MODE_EVENT_INTERRUPT             (1<<(RTC_INTERRUPT_MASK_SHIFT+6))          /*!< Geneate Event  interrupt  */

#define RTC_IRQEVENT_SEC_SHIFT               (8)
#define RTC_IRQEVENT_MIN_SHIFT               (6)
#define RTC_IRQEVENT_HOUR_SHIFT              (4)
#define RTC_IRQEVENT_DAY_SHIFT               (2)
#define RTC_IRQEVENT_MONTH_SHIFT             (0)
#define RTC_IRQEVENT_YEAR_RSHIFT             (2)

#define RTC_SEC_MASK                         (0x3<<0)
#define RTC_MIN_MASK                         (0x3<<2)
#define RTC_HOUR_MASK                        (0x3<<4)
#define RTC_DAY_MASK                         (0x3<<6)
#define RTC_MONTH_MASK                       (0x3<<8)
#define RTC_YEAR_MASK                        (0x3<<10)


/*Define the status of interrupt */

#define RTC_INT_SECOND_BIT                   (1<<0)             /*!< Indicate second interrupt flag  */
#define RTC_INT_MINUTE_BIT                   (1<<1)             /*!< Indicate minute interrupt flag  */
#define RTC_INT_HOUR_BIT                     (1<<2)             /*!< Indicate hour interrupt flag    */
#define RTC_INT_DAY_BIT                      (1<<3)             /*!< Indicate day interrupt flag  */
#define RTC_INT_MONTH_BIT                    (1<<4)             /*!< Indicate month interrupt flag  */
#define RTC_INT_YEAR_BIT                     (1<<5)             /*!< Indicate year interrupt flag  */
#define RTC_INT_EVENT_BIT                    (1<<6)             /*!< Indicate event interrupt flag  */



/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**
 * @brief RTC alarm routine callback to notify user that alarm timing event happen.
 *
 * @param rtc_status  rtc_status passed to user rtc alarm routine for the reason
 *                     RTC alarm routine called.
 *
 */
typedef void (*rtc_alarm_proc_cb)(uint32_t rtc_status);


/**
 * @brief RTC timer structure for RTC setting
 *
 *        Please Note each member of rtc_time_t is binary value.
 *        Invalid input parameter will make rtc to enter unknown/undetermine state.
 *        So please input valid date and time number.
 */
typedef struct
{
    uint32_t   tm_sec;              /*!< rtc second in binary  */
    uint32_t   tm_min;              /*!< rtc minute in binary  */
    uint32_t   tm_hour;             /*!< rtc hour in binary  */
    uint32_t   tm_day;              /*!< rtc day  in binary */
    uint32_t   tm_mon;              /*!< rtc month in binary  */
    uint32_t   tm_year;             /*!< rtc year in binary  */
} rtc_time_t;

/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/
/**
 * @brief rtc_get_time. Return the current time read from RTC.
 *
 * @param tm:     specify pointer of rtc_time_t to get current rtc value.
 *
 * @return
 * @retval    STATUS_SUCCESS
 * @retval    STATUS_INVALID_PARAM --- tm is null pointer.
 *
 *
 */
extern uint32_t rtc_get_time(rtc_time_t *tm);

/**
 * @brief rtc_set_time. Set the time in the RTC.
 *
 * @param tm:   specify pointer to rtc_time_t to set current rtc time.
 *
 * @return
 * @retval    STATUS_SUCCESS
 * @retval    STATUS_INVALID_PARAM --- tm is null pointer.
 *
 * @details     Because RTC timing domain is running in 32K clock domain, calling this function may
 *             block maxium 1ms for take the value effect.
 *
 */
extern uint32_t rtc_set_time(rtc_time_t *tm);

/**
 * @brief rtc_get_alarm. Use to get rtc alarm time.
 *
 * @param tm: Get Alarm time information.
 *
 * @return
 * @retval    STATUS_SUCCESS
 * @retval    STATUS_INVALID_PARAM --- tm is null pointer.
 *
 *
 */
extern uint32_t rtc_get_alarm(rtc_time_t *tm);

/**
 * @brief rtc_set_alarm. Use to set rtc alarm time.
 *
 * @param    tm:             set Alarm time information.
 * @param    rtc_int_mode:   set mode for interrupt generated.
 * @param    rtc_usr_isr:    when rtc interrupt happen it will call rtc_usr_isr to
 *            notify the interrupt happen.
 *
 * @return
 * @retval    STATUS_SUCCESS
 * @retval    STATUS_INVALID_PARAM --- tm is NULL pointer or rtc_usr_isr is NULL.
 *
 * @details    Because RTC timing domain is running in 32K clock, calling this function may
 *            block maximum 1ms  for take the value effect.
 *            Calling this function will also enable RTC interrupt of cortex-m3.
 *
 */
extern uint32_t rtc_set_alarm(
    rtc_time_t *tm,
    uint32_t rtc_int_mode,
    rtc_alarm_proc_cb rtc_usr_isr
);


/**
 * @brief rtc_disable_alarm. Use to disable rtc alarm time.
 *
 * @return
 * @retval    STATUS_SUCCESS
 *
 */
extern uint32_t rtc_disable_alarm(void);


/**
 * @brief rtc_set_clk. Use to set rtc ticks for second .
 *
 * @param clk: Set ticks for one second used for RTC counter.
 *
 * @details: Set rtc ticks will only take effect in next second start. It will not
 *         change current ticks in counting rtc. Adjust this value to match rtc clock.
 *
 */

extern void rtc_set_clk(uint32_t clk);


/**
 * @brief rtc_reset. Use to reset the RTC to default setting.
 *
 */

extern void rtc_reset(void);

/*@}*/ /* end of peripheral_group RTC_Driver */

#ifdef __cplusplus
}
#endif

#endif /* end of ___RTC_H__ */


