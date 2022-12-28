/**************************************************************************//**
 * @file     i2s_reg.h
 * @version  v1.00
 * @brief    I2S register definition header
 *
 * @copyright
 ******************************************************************************/

#ifndef __I2S_REG_H__
#define __I2S_REG_H__

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif




typedef struct
{
    __IO  uint32_t  I2S_MS_CTL0;      //offset: 0x00
    __IO  uint32_t  I2S_MS_CTL1;      //offset: 0x04
    __IO  uint32_t  I2S_MCLK_SET0;    //offset: 0x08
    __IO  uint32_t  I2S_MCLK_SET1;    //offset: 0x0C
    __IO  uint32_t  I2S_RESVD_0;      //offset: 0x10
    __IO  uint32_t  I2S_MS_SET0;      //offset: 0x14
    __IO  uint32_t  I2S_RESVD_1;      //offset: 0x18
    __IO  uint32_t  I2S_RESVD_2;      //offset: 0x1C
    __IO  uint32_t  I2S_RESVD_3;      //offset: 0x20
    __IO  uint32_t  I2S_RESVD_4;      //offset: 0x24
    __IO  uint32_t  I2S_RESVD_5;      //offset: 0x28
    __IO  uint32_t  I2S_RESVD_6;      //offset: 0x2C
    __IO  uint32_t  I2S_RESVD_7;      //offset: 0x30
    __IO  uint32_t  I2S_RESVD_8;      //offset: 0x34
    __IO  uint32_t  I2S_RESVD_9;      //offset: 0x38
    __IO  uint32_t  I2S_RESVD_10;     //offset: 0x3C
    __IO  uint32_t  I2S_RDMA_CTL0;    //offset: 0x40
    __IO  uint32_t  I2S_RDMA_CTL1;    //offset: 0x44
    __IO  uint32_t  I2S_RDMA_SET0;    //offset: 0x48
    __IO  uint32_t  I2S_RDMA_SET1;    //offset: 0x4C
    __IO  uint32_t  I2S_RESVD_11;     //offset: 0x50
    __IO  uint32_t  I2S_RESVD_12;     //offset: 0x54
    __I   uint32_t  I2S_RDMA_R0;      //offset: 0x58
    __I   uint32_t  I2S_RDMA_R1;      //offset: 0x5C
    __IO  uint32_t  I2S_WDMA_CTL0;    //offset: 0x60
    __IO  uint32_t  I2S_WDMA_CTL1;    //offset: 0x64
    __IO  uint32_t  I2S_WDMA_SET0;    //offset: 0x68
    __IO  uint32_t  I2S_WDMA_SET1;    //offset: 0x6C
    __IO  uint32_t  I2S_RESVD_13;     //offset: 0x70
    __IO  uint32_t  I2S_RESVD_14;     //offset: 0x74
    __I   uint32_t  I2S_WDMA_R0;      //offset: 0x78
    __I   uint32_t  I2S_WDMA_R1;      //offset: 0x7C
    __IO  uint32_t  I2S_RESVD_15;     //offset: 0x80
    __IO  uint32_t  I2S_RESVD_16;     //offset: 0x84
    __IO  uint32_t  I2S_RESVD_17;     //offset: 0x88
    __IO  uint32_t  I2S_RESVD_18;     //offset: 0x8C
    __IO  uint32_t  I2S_RESVD_19;     //offset: 0x90
    __IO  uint32_t  I2S_RESVD_20;     //offset: 0x94
    __IO  uint32_t  I2S_RESVD_21;     //offset: 0x98
    __IO  uint32_t  I2S_RESVD_22;     //offset: 0x9C
    __IO  uint32_t  I2S_INT_CLEAR;    //offset: 0xA0
    __IO  uint32_t  I2S_INT_MASK;     //offset: 0xA4
    __I   uint32_t  I2S_INT_STATUS;   //offset: 0xA8
} i2s_t;


/* offset 0x00 */
#define I2S_MS_ENABLE_I2S_SHFT        0
#define I2S_MS_ENABLE_I2S_MASK        (0x01UL << I2S_MS_ENABLE_I2S_SHFT)
#define I2S_MS_ENABLE_MCLK_SHFT       1
#define I2S_MS_ENABLE_MCLK_MASK       (0x01UL << I2S_MS_ENABLE_MCLK_SHFT)

/* offset 0x04 */
#define I2S_MS_RESET_SHFT             0
#define I2S_MS_RESET_MASK             (0x01UL << I2S_MS_RESET_SHFT)


/* offset 0x08 */
#define I2S_CFG_MCK_ISEL_SHFT         0

/* offset 0x0C */
#define I2S_CFG_MCLK_DIV_SHFT         0

/* offset 0x14 */
#define I2S_CFG_BCK_OSR_SHFT          0
#define I2S_CFG_BCK_OSR_MASK          (0x03UL << I2S_CFG_BCK_OSR_SHFT)
#define I2S_CFG_I2S_MOD_SHFT          2
#define I2S_CFG_I2S_MOD_MASK          (0x03UL << I2S_CFG_I2S_MOD_SHFT)
#define I2S_CFG_I2S_FMT_SHFT          4
#define I2S_CFG_I2S_FMT_MASK          (0x03UL << I2S_CFG_I2S_FMT_SHFT)
#define I2S_CFG_BCK_LEN_SHFT          6
#define I2S_CFG_BCK_LEN_MASK          (0x03UL << I2S_CFG_BCK_LEN_SHFT)
#define I2S_CFG_TXD_WID_SHFT          8
#define I2S_CFG_TXD_WID_MASK          (0x03UL << I2S_CFG_TXD_WID_SHFT)
#define I2S_CFG_RXD_WID_SHFT          10
#define I2S_CFG_RXD_WID_MASK          (0x03UL << I2S_CFG_RXD_WID_SHFT)
#define I2S_CFG_TXD_CHN_SHFT          12
#define I2S_CFG_TXD_CHN_MASK          (0x03UL << I2S_CFG_TXD_CHN_SHFT)
#define I2S_CFG_RXD_CHN_SHFT          14
#define I2S_CFG_RXD_CHN_MASK          (0x03UL << I2S_CFG_RXD_CHN_SHFT)
#define I2S_CFG_I2S_TST_SHFT          16
#define I2S_CFG_I2S_TST_MASK          (0x0FUL << I2S_CFG_I2S_TST_SHFT)

/* offset 0x40 */
#define I2S_RDMA_ENABLE_SHFT          0
#define I2S_RDMA_ENABLE_MASK          (0x01UL << I2S_RDMA_ENABLE_SHFT)

/* offset 0x44 */
#define I2S_RDMA_RESET_SHFT           0
#define I2S_RDMA_RESET_MASK           (0x01UL << I2S_CFG_I2S_TST_SHFT)

/* offset 0x48 */
#define I2S_RDMA_SEG_SIZE_SHFT        0
#define I2S_RDMA_SEG_SIZE_MASK        (0x0000FFFFUL << I2S_RDMA_SEG_SIZE_SHFT)
#define I2S_RDMA_BLK_SIZE_SHFT        16
#define I2S_RDMA_BLK_SIZE_MASK        (0x0000FFFFUL << I2S_RDMA_BLK_SIZE_SHFT)

/* offset 0x4C */
#define I2S_WDMA_ENABLE_SHFT          0
#define I2S_WDMA_ENABLE_MASK          (0x01UL << I2S_WDMA_ENABLE_SHFT)

/* offset 0x64 */
#define I2S_WDMA_RESET_SHFT           0
#define I2S_WDMA_RESET_MASK           (0x01UL << I2S_WDMA_RESET_SHFT)

/* offset 0x68 */
#define I2S_WDMA_SEG_SIZE_SHFT        0
#define I2S_WDMA_SEG_SIZE_MASK        (0x0000FFFFUL << I2S_WDMA_SEG_SIZE_SHFT)
#define I2S_WDMA_BLK_SIZE_SHFT        16
#define I2S_WDMA_BLK_SIZE_MASK        (0x0000FFFFUL << I2S_WDMA_BLK_SIZE_SHFT)

/* offset 0xA0 */
#define I2S_RDMA_IRQ_CLR_SHFT         0
#define I2S_RDMA_IRQ_CLR_MASK         (0x01UL << I2S_RDMA_IRQ_CLR_SHFT)
#define I2S_RDMA_ERR_IRQ_CLR_SHFT     1
#define I2S_RDMA_ERR_IRQ_CLR_MASK     (0x01UL << I2S_RDMA_ERR_IRQ_CLR_SHFT)
#define I2S_WDMA_IRQ_CLR_SHFT         2
#define I2S_WDMA_IRQ_CLR_MASK         (0x01UL << I2S_WDMA_IRQ_CLR_SHFT)
#define I2S_WDMA_ERR_IRQ_CLR_SHFT     3
#define I2S_WDMA_ERR_IRQ_CLR_MASK     (0x01UL << I2S_WDMA_ERR_IRQ_CLR_SHFT)

/* offset 0xA4 */
#define I2S_RDMA_IRQ_MASK_SHFT        0
#define I2S_RDMA_IRQ_MASK_MASK        (0x01UL << I2S_RDMA_IRQ_MASK_SHFT)
#define I2S_RDMA_ERR_IRQ_MASK_SHFT    1
#define I2S_RDMA_ERR_IRQ_MASK_MASK    (0x01UL << I2S_RDMA_ERR_IRQ_MASK_SHFT)
#define I2S_WDMA_IRQ_MASK_SHFT        2
#define I2S_WDMA_IRQ_MASK_MASK        (0x01UL << I2S_WDMA_IRQ_MASK_SHFT)
#define I2S_WDMA_ERR_IRQ_MASK_SHFT    3
#define I2S_WDMA_ERR_IRQ_MASK_MASK    (0x01UL << I2S_WDMA_ERR_IRQ_MASK_SHFT)

/* offset 0xA8 */
#define I2S_RDMA_IRQ_STATUS_SHFT      0
#define I2S_RDMA_IRQ_STATUS_MASK      (0x01UL << I2S_RDMA_IRQ_STATUS_SHFT)
#define I2S_RDMA_ERR_IRQ_STATUS_SHFT  1
#define I2S_RDMA_ERR_IRQ_STATUS_MASK  (0x01UL << I2S_RDMA_ERR_IRQ_STATUS_SHFT)
#define I2S_WDMA_IRQ_STATUS_SHFT      0
#define I2S_WDMA_IRQ_STATUS_MASK      (0x01UL << I2S_WDMA_IRQ_STATUS_SHFT)
#define I2S_WDMA_ERR_IRQ_STATUS_SHFT  1
#define I2S_WDMA_ERR_IRQ_STATUS_MASK  (0x01UL << I2S_WDMA_ERR_IRQ_STATUS_SHFT)


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

#endif

