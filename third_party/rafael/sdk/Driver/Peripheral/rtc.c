/**************************************************************************//**
 * @file     rtc.c
 * @version
 * @brief
 *
 * @copyright
*****************************************************************************/
#include "cm3_mcu.h"

/**
 * In this driver design, we don't consider multithread issue. So we don't
 * any context-switch protect. If your system is multi-thread OS, you should
 * add some prtotect mechanism.
 *
 */

static rtc_alarm_proc_cb  rtc_notify_cb;

__STATIC_INLINE uint32_t bcd2bin(uint32_t val)
{
    return ((val & 0xF) + ((val & 0xFF) >> 4) * 10);
}

__STATIC_INLINE uint32_t bin2bcd(uint32_t val)
{
    return (((val / 10) << 4) | (val % 10));
}

uint32_t rtc_get_time(rtc_time_t *tm)
{
    uint32_t temp;

    if (tm == NULL)
    {
        return STATUS_INVALID_PARAM;
    }

read_again:

    tm->tm_sec  = bcd2bin(RTC->RTC_SECOND);
    tm->tm_min  = bcd2bin(RTC->RTC_MINUTE);
    tm->tm_hour = bcd2bin(RTC->RTC_HOUR);
    tm->tm_day  = bcd2bin(RTC->RTC_DAY);
    tm->tm_mon  = bcd2bin(RTC->RTC_MONTH);
    tm->tm_year = bcd2bin(RTC->RTC_YEAR);

    temp = bcd2bin(RTC->RTC_SECOND);    /*recheck second again.*/

    if (temp != tm->tm_sec)
    {
        /* maybe HH:MM:59 to become HH:(MM+1):00
         * so we read again.*/
        if (temp == 0)
        {
            goto read_again;
        }
        else
        {
            tm->tm_sec = temp;    /*just second update one second.*/
        }
    }

    return STATUS_SUCCESS;
}

uint32_t rtc_set_time(rtc_time_t *tm)
{
    if (tm == NULL)
    {
        return STATUS_INVALID_PARAM;
    }

    /*we don't check input data is valid or not here.
      Caller should ensure it*/

    RTC->RTC_SECOND = bin2bcd(tm->tm_sec);
    RTC->RTC_MINUTE = bin2bcd(tm->tm_min);
    RTC->RTC_HOUR   = bin2bcd(tm->tm_hour);
    RTC->RTC_DAY    = bin2bcd(tm->tm_day);
    RTC->RTC_MONTH  = bin2bcd(tm->tm_mon);
    RTC->RTC_YEAR   = bin2bcd(tm->tm_year);

    RTC->RTC_LOAD  = RTC_LOAD_TIME;

    /*wait this take effect, wait RTC_LOAD bcome 0*/
    while (RTC->RTC_LOAD)
    {

    }

    return STATUS_SUCCESS;
}

uint32_t rtc_get_alarm(rtc_time_t *tm)
{
    if (tm == NULL)
    {
        return STATUS_INVALID_PARAM;
    }

    tm->tm_sec  = bcd2bin(RTC->RTC_ALARM_SECOND);
    tm->tm_min  = bcd2bin(RTC->RTC_ALARM_MINUTE);
    tm->tm_hour = bcd2bin(RTC->RTC_ALARM_HOUR);
    tm->tm_day  = bcd2bin(RTC->RTC_ALARM_DAY);
    tm->tm_mon  = bcd2bin(RTC->RTC_ALARM_MONTH);
    tm->tm_year = bcd2bin(RTC->RTC_ALARM_YEAR);

    return STATUS_SUCCESS;
}

uint32_t rtc_set_alarm(rtc_time_t *tm, uint32_t rtc_int_mode, rtc_alarm_proc_cb rtc_usr_isr)
{
    uint32_t  temp_int_reg;

    /*We don't check rtc_int_mode is correct or not...
      caller should use correct setting.*/
    if (tm == NULL)
    {
        return STATUS_INVALID_PARAM;
    }

    if (rtc_usr_isr == NULL)
    {
        return STATUS_INVALID_PARAM;
    }

    /*
     * IRQ Event and IRQ Event Repeat bit in hardware register is bit8 and bit9
     * But In our software flag, each event flag in different position, so we need
     * adjust the shift number depends on the flag type.
     */
    RTC->RTC_ALARM_SECOND = bin2bcd(tm->tm_sec) | ((rtc_int_mode & RTC_SEC_MASK) << RTC_IRQEVENT_SEC_SHIFT);
    RTC->RTC_ALARM_MINUTE = bin2bcd(tm->tm_min) | ((rtc_int_mode & RTC_MIN_MASK) << RTC_IRQEVENT_MIN_SHIFT);
    RTC->RTC_ALARM_HOUR   = bin2bcd(tm->tm_hour) | ((rtc_int_mode & RTC_HOUR_MASK) << RTC_IRQEVENT_HOUR_SHIFT);

    RTC->RTC_ALARM_DAY    = bin2bcd(tm->tm_day) | ((rtc_int_mode & RTC_DAY_MASK) << RTC_IRQEVENT_DAY_SHIFT);

    RTC->RTC_ALARM_MONTH  = bin2bcd(tm->tm_mon) | ((rtc_int_mode & RTC_MONTH_MASK) << RTC_IRQEVENT_MONTH_SHIFT);

    RTC->RTC_ALARM_YEAR   = bin2bcd(tm->tm_year) | ((rtc_int_mode & RTC_YEAR_MASK) >> RTC_IRQEVENT_YEAR_RSHIFT);

    temp_int_reg = (rtc_int_mode >> RTC_INTERRUPT_MASK_SHIFT) & RTC_INTERRUPT_MASK;

    /*clear all interrupt source first*/
    RTC->RTC_INT_CLEAR = RTC_INTERRUPT_MASK;

    RTC->RTC_INT_CONTROL = temp_int_reg;

    if (temp_int_reg)
    {
        /*Enable Cortex-M3 interrupt*/
        NVIC_EnableIRQ(Rtc_IRQn);
    }

    /*remember user callback function*/
    rtc_notify_cb = rtc_usr_isr;

    RTC->RTC_LOAD  = RTC_LOAD_ALARM;

    while (RTC->RTC_LOAD)
    {

    }

    return STATUS_SUCCESS;
}


uint32_t rtc_disable_alarm(void)
{
    /*Disable Cortex-M3 interrupt*/
    NVIC_DisableIRQ(Rtc_IRQn);

    RTC->RTC_INT_CONTROL = 0;   /*set control register to disable all interrpt*/

    /**clear all interrupt source first**/
    RTC->RTC_INT_CLEAR = RTC_INTERRUPT_MASK;

    return STATUS_SUCCESS;
}

void rtc_reset(void)
{

    RTC->RTC_CONTROL = RTC_CTRL_CLRPLS;

    while (RTC->RTC_CONTROL & RTC_CTRL_CLRPLS)
        ;            /*clear in progress*/

}

void rtc_handler(void)
{
    uint32_t status;

    status = RTC->RTC_INT_STATUS;
    RTC->RTC_INT_CLEAR = status;     /*clear interrupt status.*/

    if (rtc_notify_cb != NULL)
    {
        /*call RTC user isr*/
        rtc_notify_cb(status);
    }

}

void rtc_set_clk(uint32_t clk)
{
    RTC->RTC_CLOCK_DIV = (clk - 1) & 0xFFFF; /*only 16bits.*/
    RTC->RTC_LOAD = RTC_LOAD_DIVISOR;

    /*wait this take effect, wait RTC_LOAD bcome 0*/
    while (RTC->RTC_LOAD)
    {

    }

}
