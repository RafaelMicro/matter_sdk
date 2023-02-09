/**************************************************************************//**
 * @file     dma_reg.h
 * @version
 * @brief    DMA Register defined
 *
 * @copyright
 ******************************************************************************/

#ifndef ___DMA_REG_H__
#define ___DMA_REG_H__

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


typedef struct
{
    __IO  uint32_t   DMA_CTRL;               //0x0
    __I   uint32_t   DMA_STATUS;             //0x4
    __IO  uint32_t   DMA_SOURCE_DESC;        //0x8
    __IO  uint32_t   DMA_DEST_DESC;          //0xC
    __IO  uint32_t   DMA_Xfer_LEN;           //0x10
    __I   uint32_t   DMA_CUR_SRC_ADDR;       //0x14
    __I   uint32_t   DMA_CUR_DEST_ADDR;      //0x18
    __IO  uint32_t   DMA_INTR_CTRL;          //0x1C
    __O   uint32_t   DMA_INTR_CLEAR;         //0x20
    __IO  uint32_t   DMA_ENABLE;             //0x24
    __I   uint32_t   DMA_Lli_NEXT_Ptr;       //0x28
    __IO  uint32_t   DMA_REPEAT_VALUE;       //0x2C
    __I   uint32_t   DMA_REPEAT_STATUS;      //0x30
} DMA_T;




#define  DMA_STATUS_ENABLE          (1<<7)

#define  DMA_DMACLK_GATE            (1<<1)

#define  DMA_ENABLE_MASK            (1<<0)
#define  DMA_ENABLE_BIT             (1<<0)



#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

#endif
