/** @file i2s.c
 *
 * @brief I2s driver header file.
 *
 */
/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/
#include "cm3_mcu.h"

/**************************************************************************************************
 *    GLOBAL FUNCTIONS
 *************************************************************************************************/


static i2s_isr_handler_t  i2s_reg_handler = NULL;

void i2s_Register_Int_Callback(i2s_isr_handler_t i2s_int_callback)
{
    i2s_reg_handler = i2s_int_callback;

    return;
}
/*
*If no multitask to acess i2c device, maybe we can forget "critical_section".
*/

uint32_t I2s_Start(i2s_para_set_t *i2s_para)
{
    i2s_t *i2s = I2S0;
    if (i2s_para->trx_mode == I2S_TRX_MODE_TXRX)
    {
        i2s->I2S_WDMA_CTL0  |= I2S_WDMA_ENABLE;
        i2s->I2S_RDMA_CTL0  |= I2S_RDMA_ENABLE;
    }
    else if (i2s_para->trx_mode == I2S_TRX_MODE_TX)
    {
        i2s->I2S_RDMA_CTL0  = I2S_RDMA_ENABLE;
    }
    else if (i2s_para->trx_mode == I2S_TRX_MODE_RX)
    {
        i2s->I2S_WDMA_CTL0  = I2S_WDMA_ENABLE;
    }
    else
    {
        return STATUS_INVALID_PARAM;
    }
    // Enable I2S
    i2s->I2S_MS_CTL0 = I2S_MS_ENABLE_I2S | I2S_MS_ENABLE_MCLK;

    return STATUS_SUCCESS;
}

uint32_t I2s_Stop(void)
{
    i2s_t *i2s = I2S0;

    // Disable I2S
    i2s->I2S_MS_CTL0 &= ~(I2S_MS_ENABLE_I2S | I2S_MS_ENABLE_MCLK);

    return STATUS_SUCCESS;
}

uint32_t I2s_Init(i2s_para_set_t *i2s_para)
{
    i2s_t *i2s = I2S0;
    i2s_xdma_ctrl_ptr_t *i2s_xdma_config;
    i2s_mclk_isel_t cfg_imck = I2S_MCLK_ISEL_MAX;

    if ((i2s_para->fmt >= I2S_FMT_MAX) |
            (i2s_para->sr >= I2S_SR_MAX))
    {
        //Fail Cases
        return STATUS_INVALID_PARAM;
    }

    // Make I2S reset
    I2S_RESET();

    // Check PLL first
    // Set iMCLK
    switch (SystemCoreClock)
    {
    case (32000000UL): //32MHz
        if (i2s_para->imck_rate == I2S_IMCLK_12P288M)
        {
            cfg_imck = I2S_MCLK_ISEL_0;
        }
        else if (i2s_para->imck_rate == I2S_IMCLK_8P192M)
        {
            cfg_imck = I2S_MCLK_ISEL_1;
        }
        break;
    case (48000000UL): //48MHz
        if (i2s_para->imck_rate == I2S_IMCLK_12P288M)
        {
            cfg_imck = I2S_MCLK_ISEL_2;
        }
        else if (i2s_para->imck_rate == I2S_IMCLK_8P192M)
        {
            cfg_imck = I2S_MCLK_ISEL_3;
        }
        break;
    case (64000000UL): //64MHz
        if (i2s_para->imck_rate == I2S_IMCLK_24P576M)
        {
            cfg_imck = I2S_MCLK_ISEL_4;
        }
        else if (i2s_para->imck_rate == I2S_IMCLK_16P384M)
        {
            cfg_imck = I2S_MCLK_ISEL_5;
        }

        break;
    default:
        break;
    }

    if (cfg_imck == I2S_MCLK_ISEL_MAX)
    {
        return STATUS_INVALID_PARAM;
    }
    i2s->I2S_MCLK_SET0 = cfg_imck;

    i2s->I2S_MCLK_SET1 = i2s_para->mck_div;

    i2s->I2S_MS_SET0 = ((i2s_para->bck_osr << I2S_CFG_BCK_OSR_SHFT)& I2S_CFG_BCK_OSR_MASK) |
                       ((i2s_para->trx_mode << I2S_CFG_I2S_MOD_SHFT)& I2S_CFG_I2S_MOD_MASK) |
                       ((i2s_para->fmt << I2S_CFG_I2S_FMT_SHFT)& I2S_CFG_I2S_FMT_MASK) |
                       ((i2s_para->bck_ratio << I2S_CFG_BCK_LEN_SHFT)& I2S_CFG_BCK_LEN_MASK) |
                       ((i2s_para->width << I2S_CFG_TXD_WID_SHFT)& I2S_CFG_TXD_WID_MASK) |
                       ((i2s_para->width << I2S_CFG_RXD_WID_SHFT)& I2S_CFG_RXD_WID_MASK) |
                       ((i2s_para->ch << I2S_CFG_TXD_CHN_SHFT)& I2S_CFG_TXD_CHN_MASK) |
                       ((i2s_para->ch << I2S_CFG_RXD_CHN_SHFT)& I2S_CFG_RXD_CHN_MASK);

    // Reset XDMA
    i2s->I2S_RDMA_CTL1 = I2S_RDMA_RESET;
    i2s->I2S_WDMA_CTL1 = I2S_WDMA_RESET;
    // Clear XDMA IRQ
    i2s->I2S_INT_CLEAR = I2S_RDMA_IRQ_CLR | I2S_RDMA_ERR_IRQ_CLR | I2S_WDMA_IRQ_CLR | I2S_WDMA_ERR_IRQ_CLR;

    if (i2s_para->trx_mode == I2S_TRX_MODE_TXRX)
    {
        // Enable XDMA irq
        i2s->I2S_INT_MASK = ~(I2S_RDMA_IRQ_MASK_ENABLE | I2S_RDMA_ERR_IRQ_MASK_ENABLE | I2S_WDMA_IRQ_MASK_ENABLE | I2S_WDMA_ERR_IRQ_MASK_ENABLE);
        NVIC_EnableIRQ(I2s0_IRQn);

        i2s_xdma_config = i2s_para->rdma_config;
        i2s->I2S_RDMA_SET0  =   (((uint32_t)i2s_xdma_config->i2s_xdma_seg_size << I2S_RDMA_SEG_SIZE_SHFT) & I2S_RDMA_SEG_SIZE_MASK) |
                                (((uint32_t)i2s_xdma_config->i2s_xdma_blk_size << I2S_RDMA_BLK_SIZE_SHFT) & I2S_RDMA_BLK_SIZE_MASK);
        i2s->I2S_RDMA_SET1  = i2s_xdma_config->i2s_xdma_start_addr;

        i2s_xdma_config = i2s_para->wdma_config;
        i2s->I2S_WDMA_SET0  =   (((uint32_t)i2s_xdma_config->i2s_xdma_seg_size << I2S_RDMA_SEG_SIZE_SHFT) & I2S_RDMA_SEG_SIZE_MASK) |
                                (((uint32_t)i2s_xdma_config->i2s_xdma_blk_size << I2S_RDMA_BLK_SIZE_SHFT) & I2S_RDMA_BLK_SIZE_MASK);
        i2s->I2S_WDMA_SET1  = i2s_xdma_config->i2s_xdma_start_addr;
    }
    else if (i2s_para->trx_mode == I2S_TRX_MODE_TX)
    {
        // Enable XDMA irq
        i2s->I2S_INT_MASK = ~(I2S_RDMA_IRQ_MASK_ENABLE | I2S_RDMA_ERR_IRQ_MASK_ENABLE);
        NVIC_EnableIRQ(I2s0_IRQn);

        i2s_xdma_config = i2s_para->rdma_config;
        i2s->I2S_RDMA_SET0  =   (((uint32_t)i2s_xdma_config->i2s_xdma_seg_size << I2S_RDMA_SEG_SIZE_SHFT) & I2S_RDMA_SEG_SIZE_MASK) |
                                (((uint32_t)i2s_xdma_config->i2s_xdma_blk_size << I2S_RDMA_BLK_SIZE_SHFT) & I2S_RDMA_BLK_SIZE_MASK);
        i2s->I2S_RDMA_SET1  = i2s_xdma_config->i2s_xdma_start_addr;
    }
    else if (i2s_para->trx_mode == I2S_TRX_MODE_RX)
    {
        // Enable XDMA irq
        i2s->I2S_INT_MASK = ~(I2S_WDMA_IRQ_MASK_ENABLE | I2S_WDMA_ERR_IRQ_MASK_ENABLE);
        NVIC_EnableIRQ(I2s0_IRQn);

        i2s_xdma_config = i2s_para->wdma_config;
        i2s->I2S_WDMA_SET0  =   (((uint32_t)i2s_xdma_config->i2s_xdma_seg_size << I2S_RDMA_SEG_SIZE_SHFT) & I2S_RDMA_SEG_SIZE_MASK) |
                                (((uint32_t)i2s_xdma_config->i2s_xdma_blk_size << I2S_RDMA_BLK_SIZE_SHFT) & I2S_RDMA_BLK_SIZE_MASK);
        i2s->I2S_WDMA_SET1  = i2s_xdma_config->i2s_xdma_start_addr;
    }
    else
    {
        return STATUS_INVALID_PARAM;
    }

    return STATUS_SUCCESS;
}

uint32_t I2s_Uninit(void)
{
    i2s_t *i2s = I2S0;

    // Make I2S reset
    I2S_RESET();

    // Reset XDMA
    i2s->I2S_RDMA_CTL1 = I2S_RDMA_RESET;
    i2s->I2S_WDMA_CTL1 = I2S_WDMA_RESET;

    // Clear XDMA IRQ
    i2s->I2S_INT_CLEAR = I2S_RDMA_IRQ_CLR | I2S_RDMA_ERR_IRQ_CLR | I2S_WDMA_IRQ_CLR | I2S_WDMA_ERR_IRQ_CLR;

    // Disable XDMA irq
    i2s->I2S_INT_MASK = (I2S_RDMA_IRQ_MASK_ENABLE | I2S_RDMA_ERR_IRQ_MASK_ENABLE | I2S_WDMA_IRQ_MASK_ENABLE | I2S_WDMA_ERR_IRQ_MASK_ENABLE);
    NVIC_DisableIRQ(I2s0_IRQn);

    return STATUS_SUCCESS;
}

/**
 * @ingroup i2s_group
 * @brief i2s interrupt service rouine
 * @retval None
 */
void I2s_Handler(void)
{
    i2s_t *i2s = I2S0;
    i2s_cb_t cb;

    if (i2s->I2S_INT_STATUS & I2S_RDMA_IRQ_MASK_MASK)
    {
        i2s->I2S_INT_CLEAR |= I2S_RDMA_IRQ_CLR;

        if (((i2s->I2S_RDMA_SET0 & 0xFFFF0000) >> 16) == 0)
        {
            //If one-shot mode happened, this condition  fuction is valid.
            i2s->I2S_RDMA_CTL0 = I2S_RDMA_ENABLE;
        }

        cb.type = I2S_CB_RDMA;
        cb.blk_size = I2S_GET_RDMA_BLK_SIZE;
        cb.seg_size = I2S_GET_RDMA_SEG_SIZE;

        i2s_reg_handler(&cb);

        i2s->I2S_INT_CLEAR |= I2S_RDMA_IRQ_CLR;

    }
    else if (i2s->I2S_INT_STATUS & I2S_RDMA_ERR_IRQ_MASK_MASK)
    {
        i2s->I2S_INT_CLEAR |= I2S_RDMA_ERR_IRQ_CLR;
    }
    else if (i2s->I2S_INT_STATUS & I2S_WDMA_IRQ_MASK_MASK)
    {
        i2s->I2S_INT_CLEAR |= I2S_WDMA_IRQ_CLR;

        if (((i2s->I2S_WDMA_SET0 & 0xFFFF0000) >> 16) == 0)
        {
            //If one-shot mode happened, this condition  fuction is valid.
            i2s->I2S_WDMA_CTL0 = I2S_WDMA_ENABLE;
        }

        cb.type = I2S_CB_WDMA;
        cb.blk_size = I2S_GET_WDMA_BLK_SIZE;
        cb.seg_size = I2S_GET_WDMA_SEG_SIZE;

        i2s_reg_handler(&cb);

        i2s->I2S_INT_CLEAR |= I2S_WDMA_IRQ_CLR;

    }
    else if (i2s->I2S_INT_STATUS & I2S_WDMA_ERR_IRQ_MASK_MASK)
    {
        i2s->I2S_INT_CLEAR |= I2S_WDMA_ERR_IRQ_CLR;
    }

    return;
}


