/**************************************************************************//**
 * @file     pwm_reg.h
 * @version  v1.00
 * @brief    PWM register definition header
 *
 * @copyright
 ******************************************************************************/

#ifndef __PWM_REG_H__
#define __PWM_REG_H__

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


typedef struct
{
    __IO  uint32_t  PWM_CTL0;                //offset: 0x00
    __IO  uint32_t  PWM_CTL1;                //offset: 0x04
    __IO  uint32_t  PWM_SET0;                //offset: 0x08
    __IO  uint32_t  PWM_SET1;                //offset: 0x0C
    __IO  uint32_t  PWM_SET2;                //offset: 0x10
    __IO  uint32_t  PWM_SET3;                //offset: 0x14
    __IO  uint32_t  PWM_SET4;                //offset: 0x18
    __IO  uint32_t  PWM_SET5;                //offset: 0x1C
    __IO  uint32_t  PWM_SET6;                //offset: 0x20
    __IO  uint32_t  PWM_SET7;                //offset: 0x24
    __IO  uint32_t  PWM_SET8;                //offset: 0x28
    __IO  uint32_t  PWM_RSVD_0;              //offset: 0x2C
    __IO  uint32_t  PWM_RSVD_1;              //offset: 0x30
    __IO  uint32_t  PWM_RSVD_2;              //offset: 0x34
    __IO  uint32_t  PWM_RSVD_3;              //offset: 0x38
    __IO  uint32_t  PWM_RSVD_4;              //offset: 0x3C
    __IO  uint32_t  PWM_RDMA0_CTL0;          //offset: 0x40
    __IO  uint32_t  PWM_RDMA0_CTL1;          //offset: 0x44
    __IO  uint32_t  PWM_RDMA0_SET0;          //offset: 0x48
    __IO  uint32_t  PWM_RDMA0_SET1;          //offset: 0x4C
    __IO  uint32_t  PWM_RSVD_5;              //offset: 0x50
    __IO  uint32_t  PWM_RSVD_6;              //offset: 0x54
    __I   uint32_t  PWM_RDMA0_R0;            //offset: 0x58
    __I   uint32_t  PWM_RDMA0_R1;            //offset: 0x5C
    __IO  uint32_t  PWM_RDMA1_CTL0;          //offset: 0x60
    __IO  uint32_t  PWM_RDMA1_CTL1;          //offset: 0x64
    __IO  uint32_t  PWM_RDMA1_SET0;          //offset: 0x68
    __IO  uint32_t  PWM_RDMA1_SET1;          //offset: 0x6C
    __IO  uint32_t  PWM_RSVD_7;              //offset: 0x70
    __IO  uint32_t  PWM_RSVD_8;              //offset: 0x74
    __I   uint32_t  PWM_RDMA1_R0;            //offset: 0x78
    __I   uint32_t  PWM_RDMA1_R1;            //offset: 0x7C
    __IO  uint32_t  PWM_RSVD_9;              //offset: 0x80
    __IO  uint32_t  PWM_RSVD_10;             //offset: 0x84
    __IO  uint32_t  PWM_RSVD_11;             //offset: 0x88
    __IO  uint32_t  PWM_RSVD_12;             //offset: 0x8C
    __IO  uint32_t  PWM_RSVD_13;             //offset: 0x90
    __IO  uint32_t  PWM_RSVD_14;             //offset: 0x94
    __IO  uint32_t  PWM_RSVD_15;             //offset: 0x98
    __IO  uint32_t  PWM_RSVD_16;             //offset: 0x9C
    __IO  uint32_t  PWM_INT_CLEAR;           //offset: 0xA0
    __IO  uint32_t  PWM_INT_MASK;            //offset: 0xA4
    __I   uint32_t  PWM_INT_STATUS;          //offset: 0xA8

} pwm_t;


// offset: 0x00[31:0]
#define PWM_CFG0_PWM_ENA_SHFT                0
#define PWM_CFG0_PWM_ENA_MASK                (0x01UL << PWM_CFG0_PWM_ENA_SHFT)
#define PWM_CFG0_CK_ENA_SHFT                 1
#define PWM_CFG0_CK_ENA_MASK                 (0x01UL << PWM_CFG0_CK_ENA_SHFT)

// offset: 0x04[31:0]
#define PWM_CFG0_PWM_RST_SHFT                0
#define PWM_CFG0_PWM_RST_MASK                (0x00000001UL << PWM_CFG0_PWM_RST_SHFT)
#define PWM_CFG0_CK_UPD_SHFT                 8
#define PWM_CFG0_CK_UPD_MASK                 (0x00000001UL << PWM_CFG0_CK_UPD_SHFT)

// offset: 0x08[31:0]
#define PWM_CFG0_SEQ_ORDER_SHFT              0
#define PWM_CFG0_SEQ_ORDER_MASK              (0x00000003UL << PWM_CFG0_SEQ_ORDER_SHFT)
#define PWM_CFG0_SEQ_NUM_SEL_SHFT            1
#define PWM_CFG0_SEQ_NUM_SEL_MASK            (0x00000001UL << PWM_CFG0_SEQ_NUM_SEL_SHFT)
#define PWM_CFG0_SEQ_MODE_SHFT               2
#define PWM_CFG0_SEQ_MODE_MASK               (0x00000001UL << PWM_CFG0_SEQ_MODE_SHFT)
#define PWM_CFG0_PWM_DMA_FMT_SHFT            3
#define PWM_CFG0_PWM_DMA_FMT_MASK            (0x00000001UL << PWM_CFG0_PWM_DMA_FMT_SHFT)
#define PWM_CFG0_PWM_CNT_MODE_SHFT           4
#define PWM_CFG0_PWM_CNT_MODE_MASK           (0x00000001UL << PWM_CFG0_PWM_CNT_MODE_SHFT)
#define PWM_CFG0_PWM_CNT_TRIG_SHFT           5
#define PWM_CFG0_PWM_CNT_TRIG_MASK           (0x00000001UL << PWM_CFG0_PWM_CNT_TRIG_SHFT)
#define PWM_CFG0_SEQ_DMA_AUTO_SHFT           6
#define PWM_CFG0_SEQ_DMA_AUTO_MASK           (0x00000001UL << PWM_CFG0_SEQ_DMA_AUTO_SHFT)
#define PWM_CFG0_CK_DIV_SHFT                 8
#define PWM_CFG0_CK_DIV_MASK                 (0x0000000FUL << PWM_CFG0_CK_DIV_SHFT)
#define PWM_CFG0_PWM_ENA_TRIG_SHFT           12
#define PWM_CFG0_PWM_ENA_TRIG_MASK           (0x00000007UL << PWM_CFG0_PWM_ENA_TRIG_SHFT)

// offset: 0x0C[31:0]
#define PWM_CFG0_PWM_CNT_END_SHFT            0
#define PWM_CFG0_PWM_CNT_END_MASK            (0x00007FFFUL << PWM_CFG0_PWM_CNT_END_SHFT)

// offset: 0x10[31:0]
#define PWM_CFG0_SEQX_PCNT_SHFT              0
#define PWM_CFG0_SEQX_PCNT_MASK              (0x0000FFFFUL << PWM_CFG0_SEQX_PCNT_SHFT)

// offset: 0x14[31:0]
#define PWM_CFG0_SEQ0_NUM_SHFT               0
#define PWM_CFG0_SEQ0_NUM_MASK               (0x0000FFFFUL << PWM_CFG0_SEQ0_NUM_SHFT)

// offset: 0x18[31:0]
#define PWM_CFG0_SEQ0_RPT_SHFT               0
#define PWM_CFG0_SEQ0_RPT_MASK               (0x00FFFFFFUL << PWM_CFG0_SEQ0_RPT_SHFT)

// offset: 0x1C[31:0]
#define PWM_CFG0_SEQ0_DLY_SHFT               0
#define PWM_CFG0_SEQ0_DLY_MASK               (0x00FFFFFFUL << PWM_CFG0_SEQ0_DLY_SHFT)

// offset: 0x20[31:0]
#define PWM_CFG0_SEQ1_NUM_SHFT               0
#define PWM_CFG0_SEQ1_NUM_MASK               (0x0000FFFFUL << PWM_CFG0_SEQ1_NUM_SHFT)

// offset: 0x24[31:0]
#define PWM_CFG0_SEQ1_RPT_SHFT               0
#define PWM_CFG0_SEQ1_RPT_MASK               (0x00FFFFFFUL << PWM_CFG0_SEQ1_RPT_SHFT)

// offset: 0x28[31:0]
#define PWM_CFG0_SEQ1_DLY_SHFT               0
#define PWM_CFG0_SEQ1_DLY_MASK               (0x00FFFFFFUL << PWM_CFG0_SEQ1_DLY_SHFT)

// offset: 0x40[31:0]
#define PWM_CFG0_PWM_RDMA0_CTL0_SHFT         0
#define PWM_CFG0_PWM_RDMA0_CTL0_MASK         (0x00000001UL << PWM_CFG0_PWM_RDMA0_CTL0_SHFT)

// offset: 0x44[31:0]
#define PWM_CFG0_PWM_RDMA0_CTL1_SHFT         0
#define PWM_CFG0_PWM_RDMA0_CTL1_MASK         (0x00000001UL << PWM_CFG0_PWM_RDMA0_CTL1_SHFT)

// offset: 0x48[31:0]
#define PWM_CFG0_PWM_RDMA0_SEG_SHFT          0
#define PWM_CFG0_PWM_RDMA0_SEG_MASK          (0x0000FFFFUL << PWM_CFG0_PWM_RDMA0_SEG_SHFT)
#define PWM_CFG0_PWM_RDMA0_BLK_SHFT          16
#define PWM_CFG0_PWM_RDMA0_BLK_MASK          (0x0000FFFFUL << PWM_CFG0_PWM_RDMA0_BLK_SHFT)

// offset: 0x4C[31:0]
#define PWM_CFG0_PWM_RDMA0_SET1_SHFT         0
#define PWM_CFG0_PWM_RDMA0_SET1_MASK         (0xFFFFFFFFUL << PWM_CFG0_PWM_RDMA0_SET1_SHFT)

// offset: 0x58[31:0]
#define PWM_CFG0_PWM_RDMA0_R0_SHFT           0
#define PWM_CFG0_PWM_RDMA0_R0_MASK           (0xFFFFFFFFUL << PWM_CFG0_PWM_RDMA0_R0_SHFT)

// offset: 0x5C[31:0]
#define PWM_CFG0_PWM_RDMA0_R1_SHFT           0
#define PWM_CFG0_PWM_RDMA0_R1_MASK           (0xFFFFFFFFUL << PWM_CFG0_PWM_RDMA0_R1_SHFT)

// offset: 0x60[31:0]
#define PWM_CFG0_PWM_RDMA1_CTL0_SHFT         0
#define PWM_CFG0_PWM_RDMA1_CTL0_MASK         (0x0000000FUL << PWM_CFG0_PWM_RDMA1_CTL0_SHFT)

// offset: 0x64[31:0]
#define PWM_CFG0_PWM_RDMA1_CTL1_SHFT         0
#define PWM_CFG0_PWM_RDMA1_CTL1_MASK         (0x00000001UL << PWM_CFG0_PWM_RDMA1_CTL1_SHFT)

// offset: 0x68[31:0]
#define PWM_CFG0_PWM_RDMA1_SEG_SHFT          0
#define PWM_CFG0_PWM_RDMA1_SEG_MASK          (0x0000FFFFUL << PWM_CFG0_PWM_RDMA1_SEG_SHFT)
#define PWM_CFG0_PWM_RDMA1_BLK_SHFT          16
#define PWM_CFG0_PWM_RDMA1_BLK_MASK          (0x0000FFFFUL << PWM_CFG0_PWM_RDMA1_BLK_SHFT)

// offset: 0x6C[31:0]
#define PWM_CFG0_PWM_RDMA1_SET1_SHFT         0
#define PWM_CFG0_PWM_RDMA1_SET1_MASK         (0xFFFFFFFFUL << PWM_CFG0_PWM_RDMA1_SET1_SHFT)

// offset: 0x78[31:0]
#define PWM_CFG0_PWM_RDMA1_R0_SHFT           0
#define PWM_CFG0_PWM_RDMA1_R0_MASK           (0xFFFFFFFFUL << PWM_CFG0_PWM_RDMA1_R0_SHFT)

// offset: 0x7C[31:0]
#define PWM_CFG0_PWM_RDMA1_R1_SHFT           0
#define PWM_CFG0_PWM_RDMA1_R1_MASK           (0xFFFFFFFFUL << PWM_CFG0_PWM_RDMA1_R1_SHFT)

// offset: 0xA0[3:0]
#define PWM_RDMA0_INT_CLR_SHFT               0
#define PWM_RDMA0_INT_CLR_MASK               (0x01UL << PWM_RDMA0_INT_CLR_SHFT)
#define PWM_RDMA0_ERR_INT_CLR_SHFT           1
#define PWM_RDMA0_ERR_INT_CLR_MASK           (0x01UL << PWM_RDMA0_ERR_INT_CLR_SHFT)
#define PWM_RDMA1_INT_CLR_SHFT               2
#define PWM_RDMA1_INT_CLR_MASK               (0x01UL << PWM_RDMA1_INT_CLR_SHFT)
#define PWM_RDMA1_ERR_INT_CLR_SHFT           3
#define PWM_RDMA1_ERR_INT_CLR_MASK           (0x01UL << PWM_RDMA1_ERR_INT_CLR_SHFT)
#define PWM_RSEQ_DONE_INT_CLR_SHFT           4
#define PWM_RSEQ_DONE_INT_CLR_MASK           (0x01UL << PWM_RSEQ_DONE_INT_CLR_SHFT)
#define PWM_TSEQ_DONE_INT_CLR_SHFT           5
#define PWM_TSEQ_DONE_INT_CLR_MASK           (0x01UL << PWM_TSEQ_DONE_INT_CLR_MASK)
#define PWM_TRSEQ_DONE_INT_CLR_SHFT          6
#define PWM_TRSEQ_DONE_INT_CLR_MASK          (0x01UL << PWM_TRSEQ_DONE_INT_CLR_SHFT)

// offset: 0xA4[3:0]
#define PWM_RDMA0_INT_MASK_SHFT              0
#define PWM_RDMA0_INT_MASK_MASK              (0x01UL << PWM_RDMA0_INT_MASK_SHFT)
#define PWM_RDMA0_ERR_INT_MASK_SHFT          1
#define PWM_RDMA0_ERR_INT_MASK_MASK          (0x01UL << PWM_RDMA0_ERR_INT_MASK_SHFT)
#define PWM_RDMA1_INT_MASK_SHFT              2
#define PWM_RDMA1_INT_MASK_MASK              (0x01UL << PWM_RDMA1_INT_MASK_SHFT)
#define PWM_RDMA1_ERR_INT_MASK_SHFT          3
#define PWM_RDMA1_ERR_INT_MASK_MASK          (0x01UL << PWM_RDMA1_ERR_INT_MASK_SHFT)
#define PWM_RSEQ_DONE_INT_MASK_SHFT          4
#define PWM_RSEQ_DONE_INT_MASK_MASK          (0x01UL << PWM_RSEQ_DONE_INT_MASK_SHFT)
#define PWM_TSEQ_DONE_INT_MASK_SHFT          5
#define PWM_TSEQ_DONE_INT_MASK_MASK          (0x01UL << PWM_TSEQ_DONE_INT_MASK_SHFT)
#define PWM_TRSEQ_DONE_INT_MASK_SHFT         6
#define PWM_TRSEQ_DONE_INT_MASK_MASK         (0x01UL << PWM_TRSEQ_DONE_INT_MASK_SHFT)

// offset: 0xA8[3:0]
#define PWM_RDMA0_STATUS_INT_SHFT            0
#define PWM_RDMA0_STATUS_INT_MASK            (0x01UL << PWM_RDMA0_STATUS_INT_SHFT)
#define PWM_RDMA0_STATUS_ERR_INT_SHFT        1
#define PWM_RDMA0_STATUS_ERR_INT_MASK        (0x01UL << PWM_RDMA0_STATUS_ERR_INT_SHFT)
#define PWM_RDMA1_STATUS_INT_SHFT            2
#define PWM_RDMA1_STATUS_INT_MASK            (0x01UL << PWM_RDMA1_STATUS_INT_SHFT)
#define PWM_RDMA1_STATUS_ERR_INT_SHFT        3
#define PWM_RDMA1_STATUS_ERR_INT_MASK        (0x01UL << PWM_RDMA1_STATUS_ERR_INT_SHFT)
#define PWM_RSEQ_DONE_STATUS_INT_SHFT        4
#define PWM_RSEQ_DONE_STATUS_INT_MASK        (0x01UL << PWM_RSEQ_DONE_STATUS_INT_SHFT)
#define PWM_TSEQ_DONE_STATUS_INT_SHFT        5
#define PWM_TSEQ_DONE_STATUS_INT_MASK        (0x01UL << PWM_TSEQ_DONE_STATUS_INT_SHFT)
#define PWM_TRSEQ_DONE_STATUS_INT_SHFT       6
#define PWM_TRSEQ_DONE_STATUS_INT_MASK       (0x01UL << PWM_TRSEQ_DONE_STATUS_INT_SHFT)



#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

#endif

