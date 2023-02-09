/**************************************************************************//**
 * @file     qspi_reg.h
 * @version
 * @brief    qspi Register defined
 *
 * @copyright
 ******************************************************************************/

#ifndef ___QSPI_REG_H__
#define ___QSPI_REG_H__

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


typedef struct
{
    __IO  uint32_t   QSPI_TX_FIFO;           //0x0
    __I   uint32_t   QSPI_RX_FIFO;           //0x4
    __I   uint32_t   RESERVE_1;              //0x8
    __IO  uint32_t   QSPI_CONTROL;           //0xC
    __IO  uint32_t   QSPI_AUX;               //0x10
    __I   uint32_t   QSPI_STATUS;            //0x14
    __IO  uint32_t   QSPI_SLAVE_SEL;         //0x18
    __IO  uint32_t   QSPI_SLAVE_SEL_POL;     //0x1C
    __IO  uint32_t   QSPI_INT_EN;            //0x20
    __I   uint32_t   QSPI_INT_STATUS;        //0x24
    __IO  int32_t    QSPI_INT_CLR;           //0x28
    __I   uint32_t   QSPI_TX_FIFO_LVL;       //0x2C
    __I   uint32_t   QSPI_RX_FIFO_LVL;       //0x30
    __I   uint32_t   RESERVE_2;              //0x34
    __IO  uint32_t   QSPI_M_WAIT;            //0x38
    __IO  uint32_t   QSPI_EN;                //0x3C
    __IO  uint32_t   reserved[4];            //0x40~0x4C
    __IO  uint32_t   QSPI_CLKDIV;            //0x50

    __IO  uint32_t   reserved2[3];           //0x54~0x5C

    __IO  uint32_t   DMA_RX_ADDR;            //0x60
    __IO  uint32_t   DMA_RX_LEN;             //0x64
    __IO  uint32_t   DMA_TX_ADDR;            //0x68
    __IO  uint32_t   DMA_TX_LEN;             //0x6C
    __I   uint32_t   DMA_RX_RLEN;            //0x70
    __I   uint32_t   DMA_TX_RLEN;            //0x74
    __IO  uint32_t   DMA_IER;                //0x78
    __IO  uint32_t   DMA_INT_STATUS;         //0x7C
    __IO  uint32_t   DMA_RX_ENABLE;          //0x80
    __IO  uint32_t   DMA_TX_ENABLE;          //0x84
} QSPI_T;


#define  QSPI_TX_FIFO_OFFSET          0
#define  QSPI_RX_FIFO_OFFSET          4

#define  QSPI_CNTL_NODMA_MASK       (0xFF)



#define  QSPI_BITSIZE_8             (1<<4)
#define  QSPI_BITSIZE_32            (7<<4)

#define  QSPI_DISABLE_OUT           (1<<2)
#define  QSPI_DISABLE_IN            (1<<3)

#define  QSPI_Xfer_Extend           (1<<7)

#define  QSPI_CNTL_contXfer_SHIFT    (0)
#define  QSPI_CNTL_contXfer_En       (1<<QSPI_CNTL_contXfer_SHIFT)

#define  QSPI_CNTL_EDIAN_SHIFT       (1)
#define  QSPI_CNTL_LITTLE_ENDIAN     (1<<QSPI_CNTL_EDIAN_SHIFT)

#define  QSPI_CNTL_MSB_SHIFT         (2)
#define  QSPI_CNTL_CPHA_SHIFT        (3)
#define  QSPI_CNTL_CPOL_SHIFT        (4)

#define  QSPI_CNTL_MASTER_SHIFT      (5)
#define  QSPI_CNTL_MASTER            (1<<5)
#define  QSPI_CNTL_SLAVE             (0<<5)

#define  SPI_CNTL_SLAVE_SDATA_SHIFT  (6)
#define  SPI_CNTL_SLAVE_SDATA_OUT    (1<<6)

#define  QSPI_CNTL_DMA_SHIFT         (10)

#define  QSPI_CNTL_mWaitEn_SHIFT     (11)

#define  QSPI_CNTL_rxWmark_SHIFT     (12)
#define  QSPI_CNTL_txWmark_SHIFT     (14)

#define  QSPI_CNTL_DMA_ENABLE        (1<<QSPI_CNTL_DMA_SHIFT)

#define  QSPI_CNTL_TX_1_8_WATERMARK     (00<<QSPI_CNTL_txWmark_SHIFT)
#define  QSPI_CNTL_TX_1_4_WATERMARK     (01<<QSPI_CNTL_txWmark_SHIFT)
#define  QSPI_CNTL_TX_HALF_WATERMARK    (10<<QSPI_CNTL_txWmark_SHIFT)
#define  QSPI_CNTL_TX_3_4_WATERMARK     (11<<QSPI_CNTL_txWmark_SHIFT)

#define  QSPI_CNTL_RX_1_8_WATERMARK     (00<<QSPI_CNTL_rxWmark_SHIFT)
#define  QSPI_CNTL_RX_1_4_WATERMARK     (01<<QSPI_CNTL_rxWmark_SHIFT)
#define  QSPI_CNTL_RX_HALF_WATERMARK    (10<<QSPI_CNTL_rxWmark_SHIFT)
#define  QSPI_CNTL_RX_3_4_WATERMARK     (11<<QSPI_CNTL_rxWmark_SHIFT)



#define  QSPI_STATUS_xferIP           (1<<0)            /*transfer in progress*/
#define  QSPI_STATUS_AllCmdDone       (1<<1)
#define  QSPI_STATUS_txEmpty          (1<<2)
#define  QSPI_STATUS_txWmark          (1<<3)
#define  QSPI_STATUS_txFull           (1<<4)
#define  QSPI_STATUS_rxEmpty          (1<<5)
#define  QSPI_STATUS_rxWmark          (1<<6)
#define  QSPI_STATUS_rxFull           (1<<7)

#define  QSPI_INT_txEmpty             (1<<0)
#define  QSPI_INT_txWmark             (1<<1)
#define  QSPI_INT_rxWmark             (1<<2)
#define  QSPI_INT_rxFull              (1<<3)
#define  QSPI_INT_xferDone            (1<<4)
#define  QSPI_INT_rxNotEmpty          (1<<5)


#define  QSPI_MST_CLKDIV_EN           (1<<8)

#define  QSPI_MST_CLKDIV_16MHZ        (0)
#define  QSPI_MST_CLKDIV_8MHZ         (1)
#define  QSPI_MST_CLKDIV_4MHZ         (3)
#define  QSPI_MST_CLKDIV_2MHZ         (7)
#define  QSPI_MST_CLKDIV_1MHZ         (15)


#define  QSPI_DMA_ISR_TX              (1<<1)
#define  QSPI_DMA_ISR_RX              (1<<0)

#define  QSPI_DMA_ISR_CLEARALL        (QSPI_DMA_ISR_TX|QSPI_DMA_ISR_RX)


#define  QSPI_DMA_IER_TX              QSPI_DMA_ISR_TX
#define  QSPI_DMA_IER_RX              QSPI_DMA_ISR_RX

#define  QSPI_DMA_ENABLE              (1<<0)
#define  QSPI_DMA_Dummy_ENABLE        (1<<1)


#define  SPI0_CLK_INDEX               (20)



#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

#endif
