/**************************************************************************//**
 * @file     rtc_reg.h
 * @version
 * @brief    RTC Register defined
 *
 * @copyright
 ******************************************************************************/

#ifndef ___RTC_REG_H__
#define ___RTC_REG_H__

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif



typedef struct
{
    __IO  uint32_t   RTC_SECOND;             //0x0
    __IO  uint32_t   RTC_MINUTE;             //0x4
    __IO  uint32_t   RTC_HOUR;               //0x8
    __IO  uint32_t   RTC_DAY;                //0xC
    __IO  uint32_t   RTC_MONTH;              //0x10
    __IO  uint32_t   RTC_YEAR;               //0x14
    __IO  uint32_t   RTC_CONTROL;            //0x18
    __IO  uint32_t   RTC_CLOCK_DIV;          //0x1C
    __IO  uint32_t   RTC_ALARM_SECOND;       //0x20
    __IO  uint32_t   RTC_ALARM_MINUTE;       //0x24
    __IO  uint32_t   RTC_ALARM_HOUR;         //0x28
    __IO  uint32_t   RTC_ALARM_DAY;          //0x2C
    __IO  uint32_t   RTC_ALARM_MONTH;        //0x30
    __IO  uint32_t   RTC_ALARM_YEAR;         //0x34
    __IO  uint32_t   RTC_INT_CONTROL;        //0x38
    __IO  uint32_t   RTC_INT_STATUS;         //0x3C
    __IO  uint32_t   RTC_INT_CLEAR;          //0x40
    __IO  uint32_t   RTC_LOAD;               //0x44

} RTC_T;



#define  RTC_INT_SEC         (1<<0)
#define  RTC_INT_MIN         (1<<1)
#define  RTC_INT_HOUR        (1<<2)
#define  RTC_INT_DAY         (1<<3)
#define  RTC_INT_MONTH       (1<<4)
#define  RTC_INT_YEAR        (1<<5)
#define  RTC_INT_EVENT       (1<<6)

#define  RTC_CTRL_CLRPLS     (1<<7)

#define  RTC_LOAD_TIME       (1<<0)
#define  RTC_LOAD_ALARM      (1<<1)
#define  RTC_LOAD_DIVISOR    (1<<2)


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

#endif
