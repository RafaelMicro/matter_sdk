/**************************************************************************//**
 * @file     cachectl_reg.h
 * @version  V1.00
 * @brief    cache controller register definition header file
 *
 * @copyright
 *****************************************************************************/
#ifndef ___CACHECTL_REG_H__
#define ___CACHECTL_REG_H__

#if defined (__CC_ARM)
#pragma anon_unions
#endif


typedef union cache_ccr_s
{
    struct cache_ccr_b
    {
        uint32_t CACHE_EN         : 1;
        uint32_t CACHE_WAY_1_EN   : 1;
        uint32_t RESERVED2        : 6;
        uint32_t CACHE_WAY_0_CLR  : 1;
        uint32_t CACHE_WAY_1_CLR  : 1;
        uint32_t RESERVED10       : 22;
    } bit;
    uint32_t reg;
} cache_ccr_t;


typedef struct
{
    __IO cache_ccr_t CCR;        /*0x00*/
} CACHECTL_T;


#if defined (__CC_ARM)
#pragma no_anon_unions
#endif

#endif
