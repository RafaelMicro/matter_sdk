/**************************************************************************//**
 * @file     remap_reg.h
 * @version  v1.00
 * @brief    Remap register definition header
 *
 * @copyright
 ******************************************************************************/

#ifndef __REMAP_REG_H__
#define __REMAP_REG_H__

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif




typedef struct
{
    __IO  uint32_t  RESERVED1;                 //offset: 0x00
    __IO  uint32_t  RESERVED2;                 //offset: 0x04
    __IO  uint32_t  RESERVED3;                 //offset: 0x08
    __IO  uint32_t  RESERVED4;                 //offset: 0x0C
    __IO  uint32_t  SW_IRQ_SET;                //offset: 0x10
    __IO  uint32_t  SW_IRQ_CLR;                //offset: 0x14
    __IO  uint32_t  SW_IRQ_EN_SET;             //offset: 0x18
    __IO  uint32_t  SW_IRQ_EN_CLR;             //offset: 0x1C
} remap_t;


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

#endif

