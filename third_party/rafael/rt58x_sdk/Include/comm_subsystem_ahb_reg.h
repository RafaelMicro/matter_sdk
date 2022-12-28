/**************************************************************************//**
 * @file     COMM_SUBSYSTEM_AHB_REG.h
 * @version
 * @brief    dma Register defined
 *

 ******************************************************************************/

#ifndef __COMM_SUBSYSTEM_AHB_REG_H__
#define __COMM_SUBSYSTEM_AHB_REG_H__

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


typedef struct
{
    __IO  uint32_t  COMM_SUBSYSTEM_AHB_ADDR;//0x0
    __IO     uint32_t  COMM_SUBSYSTEM_DMA1;//0x4
    __IO  uint32_t  COMM_SUBSYSTEM_DMA_TYPE;//0x8
    __O  uint32_t  COMM_SUBSYSTEM_HOST;//0xC
    __IO  uint32_t  COMM_SUBSYSTEM_INTR_EN;//0x10
    __O     uint32_t  COMM_SUBSYSTEM_INTR_CLR;//0x14
    __I     uint32_t  COMM_SUBSYSTEM_INTR_STATUS;//0x18
    __I  uint32_t  COMM_SUBSYSTEM_RX_INFO;//0x1C
    __I   uint32_t  COMM_SUBSYSTEM_TX_INFO;//0x20
    __I  uint32_t  COMM_SUBSYSTEM_INFO;//0x24
} COMM_SUBSYSTEM_AHB_T;


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

#endif
