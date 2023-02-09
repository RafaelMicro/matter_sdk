/**************************************************************************//**
 * @file     timer_reg.h
 * @version  V1.00
 * @brief    timer definition header file
 *
 * @copyright
 *****************************************************************************/
#ifndef __TIMER_REG_H__
#define __TIMER_REG_H__

#if defined (__CC_ARM)
#pragma anon_unions
#endif


typedef union timern_ctrl_s
{
    struct timern_ctrl_b
    {
        uint32_t RESERVED1   : 2;
        uint32_t PRESCALE    : 3;
        uint32_t INT_ENABLE  : 1;
        uint32_t MODE        : 1;
        uint32_t EN          : 1;
        uint32_t INT_STATUS  : 1;
        uint32_t RESERVED2   : 23;
    } bit;
    uint32_t reg;
} timern_ctrl_t;

typedef union timern_repdly_s
{
    struct timern_repdly_b
    {
        uint32_t INT_REPEAT_DELAY  : 16;
        uint32_t RESERVED          : 16;
    } bit;
    uint32_t reg;
} timern_repdly_t;

typedef struct
{
    __IO uint32_t LOAD;                   /*0x00*/
    __IO uint32_t VALUE;                  /*0x04*/
    __IO timern_ctrl_t CONTROL;           /*0x08*/
    __IO uint32_t CLEAR;                  /*0x0C*/
    __IO timern_repdly_t REPEAT_DELAY;    /*0x10*/
} timern_t;


#define TIMER_INT_STATUS        0x0100
#define TIMER_DISABLE_TIMER     0x0000
#define TIMER_ENABLE_TIMER      0x0080
#define TIMER_MODE_PERODIC      0x0040



#if defined (__CC_ARM)
#pragma no_anon_unions
#endif

#endif

