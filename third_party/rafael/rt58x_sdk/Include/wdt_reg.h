/**************************************************************************//**
 * @file     wdt_reg.h
 * @version  V1.00
 * @brief    WacthDog Timer register definition header file
 *
 * @copyright
 *****************************************************************************/
#ifndef __WDT_REG_H__
#define __WDT_REG_H__

#if defined (__CC_ARM)
#pragma anon_unions
#endif



typedef union wdt_ctrl_s
{
    struct wdt_ctrl_b
    {
        uint32_t LOCKOUT   : 1;
        uint32_t RESERVED1 : 1;
        uint32_t PRESCALE  : 3;
        uint32_t RESET_EN  : 1;
        uint32_t INT_EN    : 1;
        uint32_t WDT_EN    : 1;
        uint32_t RESERVED2 : 24;
    } bit;
    uint32_t reg;
} wdt_ctrl_t;

typedef union wdt_rst_s
{
    struct wdt_rst_b
    {
        uint32_t RESET_OCCUR : 1;
        uint32_t RESERVED    : 31;
    } bit;
    uint32_t reg;
} wdt_rst_t;



typedef struct
{
    __IO uint32_t LOAD;        /*0x00*/
    __I  uint32_t VALUE;       /*0x04*/
    __IO wdt_ctrl_t CONTROL;   /*0x08*/
    __IO uint32_t WDT_KICK;    /*0x0C*/
    __IO wdt_rst_t RST_OCCUR;  /*0x10*/
    __O  uint32_t CLEAR;       /*0x14*/
    __IO uint32_t INT_VALUE;   /*0x18*/
    __IO uint32_t WINDOW_MIN;  /*0x1C*/
} wdt_t;



// control register bit field definitions:

#define WDT_DISABLE            0x0000
#define WDT_ENABLE             0x0080

#define WDT_ENABLE_INT         0x0040
#define WDT_ENABLE_RESET       0x0020
#define WDT_ENABLE_LOCK        0x0001

#define WDT_PRESCALE_MASK      0x001c

#define WDT_PRESCALE_DIV1      0x0000
#define WDT_PRESCALE_DIV16     0x0004
#define WDT_PRESCALE_DIV256    0x0008
#define WDT_PRESCALE_DIV32     0x000C
#define WDT_PRESCALE_DIV128    0x0010
#define WDT_PRESCALE_DIV1024   0x0014
#define WDT_PRESCALE_DIV4096   0x0018
#define WDT_PRESCALE_DIV4096A  0x001C

#define WDT_KICK_VALUE         0xA5A5

#if defined (__CC_ARM)
#pragma no_anon_unions
#endif

#endif

