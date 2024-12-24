/** @file pwm.c
 *
 * @brief PWM driver file.
 *
 */
/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "cm3_mcu.h"

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/

/**************************************************************************************************
*    TYPEDEFS
*************************************************************************************************/

/**************************************************************************************************
 *    GLOBAL VARIABLES
 *************************************************************************************************/
uint32_t pwm_int_status[PWM_ID_MAX];
uint32_t pwm_rdma0_next_address[PWM_ID_MAX];
uint32_t pwm_rdma1_next_address[PWM_ID_MAX];

uint32_t debug_idx;

/**************************************************************************************************
 *    GLOBAL FUNCTIONS
 *************************************************************************************************/


uint32_t Pwm_Init(pwm_seq_para_head_t *pwm_para_config)
{
    pwm_t *pwm;
    pwm_seq_para_t *pwm_seq_set;

    switch (pwm_para_config->pwm_id)
    {
    case PWM_ID_0:
        pwm = PWM0;
        break;
    case PWM_ID_1:
        pwm = PWM1;
        break;
    case PWM_ID_2:
        pwm = PWM2;
        break;
    case PWM_ID_3:
        pwm = PWM3;
        break;
    case PWM_ID_4:
        pwm = PWM4;
        break;
    default:
        return STATUS_INVALID_PARAM;
    }

    pwm_int_status[pwm_para_config->pwm_id] = 0;

    pwm->PWM_INT_CLEAR = PWM_RDMA0_INT_CLR | PWM_RDMA1_INT_CLR | PWM_RDMA0_ERR_INT_CLR | PWM_RDMA1_ERR_INT_CLR | PWM_RSEQ_DONE_INT_CLR | PWM_TSEQ_DONE_INT_CLR | PWM_TRSEQ_DONE_INT_CLR;
    pwm->PWM_CTL1 |= PWM_RESET;
    pwm->PWM_RDMA0_CTL1 = PWM_RDMA_RESET;
    pwm->PWM_RDMA1_CTL1 = PWM_RDMA_RESET;

    pwm->PWM_SET0 = (pwm_para_config->pwm_seq_order     << PWM_CFG0_SEQ_ORDER_SHFT)    |
                    (pwm_para_config->pwm_seq_num       << PWM_CFG0_SEQ_NUM_SEL_SHFT)  |
                    (pwm_para_config->pwm_seq_mode      << PWM_CFG0_SEQ_MODE_SHFT)     |
                    (pwm_para_config->pwm_dma_smp_fmt   << PWM_CFG0_PWM_DMA_FMT_SHFT)  |
                    (pwm_para_config->pwm_counter_mode  << PWM_CFG0_PWM_CNT_MODE_SHFT) |
                    (PWM_DMA_AUTO_ENABLE                << PWM_CFG0_SEQ_DMA_AUTO_SHFT) |
                    (pwm_para_config->pwm_clk_div       << PWM_CFG0_CK_DIV_SHFT)       |
                    (pwm_para_config->pwm_triggered_src << PWM_CFG0_PWM_ENA_TRIG_SHFT);

    if (pwm_para_config->pwm_dma_smp_fmt == PWM_DMA_SMP_FMT_0)
    {
        pwm->PWM_SET1 = pwm_para_config->pwm_count_end_val;
    }

    pwm->PWM_SET2 = pwm_para_config->pwm_play_cnt;

    if (pwm_para_config->pwm_seq_num == PWM_SEQ_NUM_2)
    {
        pwm_seq_set = &(pwm_para_config->pwm_seq0);
        pwm->PWM_SET3 = pwm_seq_set->pwm_element_num;
        pwm->PWM_SET4 = pwm_seq_set->pwm_repeat_num;
        pwm->PWM_SET5 = pwm_seq_set->pwm_delay_num;
        pwm->PWM_RDMA0_SET0 = (pwm_para_config->pwm_dma_smp_fmt == PWM_DMA_SMP_FMT_0) ? ((pwm_seq_set->pwm_element_num >> 1) | ((pwm_seq_set->pwm_element_num >> 2) << 16))
                              : ((pwm_seq_set->pwm_element_num)    | ((pwm_seq_set->pwm_element_num >> 1) << 16));
        pwm->PWM_RDMA0_SET1 = pwm_seq_set->pwm_rdma_addr;

        pwm_seq_set = &(pwm_para_config->pwm_seq1);
        pwm->PWM_SET6 = pwm_seq_set->pwm_element_num;
        pwm->PWM_SET7 = pwm_seq_set->pwm_repeat_num;
        pwm->PWM_SET8 = pwm_seq_set->pwm_delay_num;
        pwm->PWM_RDMA1_SET0 = (pwm_para_config->pwm_dma_smp_fmt == PWM_DMA_SMP_FMT_0) ? ((pwm_seq_set->pwm_element_num >> 1) | ((pwm_seq_set->pwm_element_num >> 2) << 16))
                              : ((pwm_seq_set->pwm_element_num)    | ((pwm_seq_set->pwm_element_num >> 1) << 16));
        pwm->PWM_RDMA1_SET1 = pwm_seq_set->pwm_rdma_addr;
        pwm->PWM_INT_MASK = ~(PWM_RDMA0_INT_MASK_ENABLE  | PWM_RDMA1_INT_MASK_ENABLE  | PWM_RSEQ_DONE_INT_MASK_ENABLE | PWM_TSEQ_DONE_INT_MASK_ENABLE | PWM_TRSEQ_DONE_INT_MASK_ENABLE);
    }
    else if (pwm_para_config->pwm_seq_num == PWM_SEQ_NUM_1 )
    {
        if (pwm_para_config->pwm_seq_order == PWM_SEQ_ORDER_R)
        {
            pwm_seq_set = &(pwm_para_config->pwm_seq0);
            pwm->PWM_SET3 = pwm_seq_set->pwm_element_num;
            pwm->PWM_SET4 = pwm_seq_set->pwm_repeat_num;
            pwm->PWM_SET5 = pwm_seq_set->pwm_delay_num;
            pwm->PWM_RDMA0_SET0 = (pwm_para_config->pwm_dma_smp_fmt == PWM_DMA_SMP_FMT_0) ? ((pwm_seq_set->pwm_element_num >> 1) | ((pwm_seq_set->pwm_element_num >> 2) << 16))
                                  : ((pwm_seq_set->pwm_element_num)    | ((pwm_seq_set->pwm_element_num >> 1) << 16));
            pwm->PWM_RDMA0_SET1 = pwm_seq_set->pwm_rdma_addr;
            pwm->PWM_INT_MASK = ~(PWM_RDMA0_INT_MASK_ENABLE | PWM_RSEQ_DONE_INT_MASK_ENABLE | PWM_TSEQ_DONE_INT_MASK_ENABLE | PWM_TRSEQ_DONE_INT_MASK_ENABLE);
        }
        else if (pwm_para_config->pwm_seq_order == PWM_SEQ_ORDER_T)
        {
            pwm_seq_set = &(pwm_para_config->pwm_seq1);
            pwm->PWM_SET6 = pwm_seq_set->pwm_element_num;
            pwm->PWM_SET7 = pwm_seq_set->pwm_repeat_num;
            pwm->PWM_SET8 = pwm_seq_set->pwm_delay_num;
            pwm->PWM_RDMA1_SET0 = (pwm_para_config->pwm_dma_smp_fmt == PWM_DMA_SMP_FMT_0) ? ((pwm_seq_set->pwm_element_num >> 1) | ((pwm_seq_set->pwm_element_num >> 2) << 16))
                                  : ((pwm_seq_set->pwm_element_num)    | ((pwm_seq_set->pwm_element_num >> 1) << 16));
            pwm->PWM_RDMA1_SET1 = pwm_seq_set->pwm_rdma_addr;
            pwm->PWM_INT_MASK = ~(PWM_RDMA1_INT_MASK_ENABLE | PWM_RSEQ_DONE_INT_MASK_ENABLE | PWM_TSEQ_DONE_INT_MASK_ENABLE | PWM_TRSEQ_DONE_INT_MASK_ENABLE);
        }
    }

    return STATUS_SUCCESS;
}


uint32_t Pwm_Start(pwm_seq_para_head_t *pwm_para_config)
{
    pwm_t *pwm;

    switch (pwm_para_config->pwm_id)
    {
    case PWM_ID_0:
        pwm = PWM0;
        NVIC_EnableIRQ(Pwm0_IRQn);
        break;
    case PWM_ID_1:
        pwm = PWM1;
        NVIC_EnableIRQ(Pwm1_IRQn);
        break;
    case PWM_ID_2:
        pwm = PWM2;
        NVIC_EnableIRQ(Pwm2_IRQn);
        break;
    case PWM_ID_3:
        pwm = PWM3;
        NVIC_EnableIRQ(Pwm3_IRQn);
        break;
    case PWM_ID_4:
        pwm = PWM4;
        NVIC_EnableIRQ(Pwm4_IRQn);
        break;
    default:
        return STATUS_INVALID_PARAM;
    }

    if (pwm_para_config->pwm_seq_num == PWM_SEQ_NUM_2)
    {
        pwm->PWM_RDMA0_CTL0 = PWM_RDMA_ENABLE;
        pwm->PWM_RDMA1_CTL0 = PWM_RDMA_ENABLE;
    }
    else if (pwm_para_config->pwm_seq_num == PWM_SEQ_NUM_1 )
    {
        if (pwm_para_config->pwm_seq_order == PWM_SEQ_ORDER_R)
        {
            pwm->PWM_RDMA0_CTL0 = PWM_RDMA_ENABLE;
        }
        else if (pwm_para_config->pwm_seq_order == PWM_SEQ_ORDER_T)
        {
            pwm->PWM_RDMA1_CTL0 = PWM_RDMA_ENABLE;
        }
    }
    else
    {
        return STATUS_INVALID_PARAM;
    }

    pwm->PWM_CTL0 |= (PWM_ENABLE_PWM | PWM_ENABLE_CLK);
    return STATUS_SUCCESS;

}


uint32_t Pwm_Stop(pwm_seq_para_head_t *pwm_para_config)
{
    pwm_t *pwm;

    switch (pwm_para_config->pwm_id)
    {
    case PWM_ID_0:
        pwm = PWM0;
        NVIC_DisableIRQ(Pwm0_IRQn);
        break;
    case PWM_ID_1:
        pwm = PWM1;
        NVIC_DisableIRQ(Pwm1_IRQn);
        break;
    case PWM_ID_2:
        pwm = PWM2;
        NVIC_DisableIRQ(Pwm2_IRQn);
        break;
    case PWM_ID_3:
        pwm = PWM3;
        NVIC_DisableIRQ(Pwm3_IRQn);
        break;
    case PWM_ID_4:
        pwm = PWM4;
        NVIC_DisableIRQ(Pwm4_IRQn);
        break;
    default:
        return STATUS_INVALID_PARAM;
    }


    if (pwm_para_config->pwm_seq_num == PWM_SEQ_NUM_2)
    {
        pwm->PWM_RDMA0_CTL1 |= PWM_RDMA_RESET;      //reset xdma
        pwm->PWM_RDMA1_CTL1 |= PWM_RDMA_RESET;      //reset xdma
    }
    else if (pwm_para_config->pwm_seq_num == PWM_SEQ_NUM_1 )
    {
        if (pwm_para_config->pwm_seq_order == PWM_SEQ_ORDER_R)
        {
            pwm->PWM_RDMA0_CTL1 |= PWM_RDMA_RESET;       //reset xdma
        }
        else if (pwm_para_config->pwm_seq_order == PWM_SEQ_ORDER_T)
        {
            pwm->PWM_RDMA1_CTL1 |= PWM_RDMA_RESET;        //reset xdma
        }
    }
    else
    {
        return STATUS_INVALID_PARAM;
    }

    pwm->PWM_CTL0 &= ~ (PWM_ENABLE_PWM | PWM_ENABLE_CLK);

    return STATUS_SUCCESS;
}


/**
 * @brief Function for handling the PWM0 interrupt.
 * @details Checks for PWM interrupt status, and executes PWM handlers for the corresponding status.
 * @param[in] None
 * @return None
 */
void Pwm0_Handler(void)
{
    uint32_t pwm_int_buf;
    pwm_t *pwm = PWM0;

    pwm_int_buf = pwm->PWM_INT_STATUS;
    pwm->PWM_INT_CLEAR = pwm_int_buf;

    //pwm_int_status[PWM_ID_0] = pwm_int_buf;

    if (pwm_int_buf & PWM_RDMA0_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_0] |= PWM_RDMA0_STATUS_INT_MASK;
        pwm_rdma0_next_address[PWM_ID_0] = pwm->PWM_RDMA0_R0;
    }

    if (pwm_int_buf & PWM_RDMA0_STATUS_ERR_INT_MASK)
    {
        pwm->PWM_INT_MASK |= PWM_RDMA0_ERR_INT_MASK_ENABLE;

        pwm_int_status[PWM_ID_0] |= PWM_RDMA0_STATUS_ERR_INT_MASK;
    }

    if (pwm_int_buf & PWM_RDMA1_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_0] |= PWM_RDMA1_STATUS_INT_MASK;
        pwm_rdma1_next_address[PWM_ID_0] = pwm->PWM_RDMA1_R0;
    }

    if (pwm_int_buf & PWM_RDMA1_STATUS_ERR_INT_MASK)
    {
        pwm->PWM_INT_MASK |= PWM_RDMA1_ERR_INT_MASK_ENABLE;

        pwm_int_status[PWM_ID_0] |= PWM_RDMA1_STATUS_ERR_INT_MASK;
    }

    if (pwm_int_buf & PWM_RSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_0] |= PWM_RSEQ_DONE_STATUS_INT_MASK;
        pwm_rdma0_next_address[PWM_ID_0] = pwm->PWM_RDMA0_R0;
    }

    if (pwm_int_buf & PWM_TSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_0] |= PWM_TSEQ_DONE_STATUS_INT_MASK;
        pwm_rdma1_next_address[PWM_ID_0] = pwm->PWM_RDMA1_R0;
    }

    if (pwm_int_buf & PWM_TRSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_0] |= PWM_TRSEQ_DONE_STATUS_INT_MASK;
    }
}


/**
 * @brief Function for handling the PWM1 interrupt.
 * @details Checks for PWM interrupt status, and executes PWM handlers for the corresponding status.
 * @param[in] None
 * @return None
 */
void Pwm1_Handler(void)
{
    uint32_t pwm_int_buf;
    pwm_t *pwm = PWM1;

    pwm_int_buf = pwm->PWM_INT_STATUS;
    pwm->PWM_INT_CLEAR = pwm_int_buf;

    //pwm_int_status[PWM_ID_1] = pwm_int_buf;

    if (pwm_int_buf & PWM_RDMA0_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_1] |= PWM_RDMA0_STATUS_INT_MASK;
        pwm_rdma0_next_address[PWM_ID_1] = pwm->PWM_RDMA0_R0;
    }

    if (pwm_int_buf & PWM_RDMA0_STATUS_ERR_INT_MASK)
    {
        pwm->PWM_INT_MASK |= PWM_RDMA0_ERR_INT_MASK_ENABLE;

        pwm_int_status[PWM_ID_1] |= PWM_RDMA0_STATUS_ERR_INT_MASK;
    }

    if (pwm_int_buf & PWM_RDMA1_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_1] |= PWM_RDMA1_STATUS_INT_MASK;
        pwm_rdma1_next_address[PWM_ID_1] = pwm->PWM_RDMA1_R0;
    }

    if (pwm_int_buf & PWM_RDMA1_STATUS_ERR_INT_MASK)
    {
        pwm->PWM_INT_MASK |= PWM_RDMA1_ERR_INT_MASK_ENABLE;

        pwm_int_status[PWM_ID_1] |= PWM_RDMA1_STATUS_ERR_INT_MASK;
    }

    if (pwm_int_buf & PWM_RSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_1] |= PWM_RSEQ_DONE_STATUS_INT_MASK;
        pwm_rdma0_next_address[PWM_ID_1] = pwm->PWM_RDMA0_R0;
    }

    if (pwm_int_buf & PWM_TSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_1] |= PWM_TSEQ_DONE_STATUS_INT_MASK;
        pwm_rdma1_next_address[PWM_ID_1] = pwm->PWM_RDMA1_R0;
    }

    if (pwm_int_buf & PWM_TRSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_1] |= PWM_TRSEQ_DONE_STATUS_INT_MASK;
    }
}


/**
 * @brief Function for handling the PWM2 interrupt.
 * @details Checks for PWM interrupt status, and executes PWM handlers for the corresponding status.
 * @param[in] None
 * @return None
 */
void Pwm2_Handler(void)
{
    uint32_t pwm_int_buf;
    pwm_t *pwm = PWM2;

    pwm_int_buf = pwm->PWM_INT_STATUS;
    pwm->PWM_INT_CLEAR = pwm_int_buf;

    //pwm_int_status[PWM_ID_2] = pwm_int_buf;

    if (pwm_int_buf & PWM_RDMA0_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_2] |= PWM_RDMA0_STATUS_INT_MASK;
        pwm_rdma0_next_address[PWM_ID_2] = pwm->PWM_RDMA0_R0;
    }

    if (pwm_int_buf & PWM_RDMA0_STATUS_ERR_INT_MASK)
    {
        pwm->PWM_INT_MASK |= PWM_RDMA0_ERR_INT_MASK_ENABLE;

        pwm_int_status[PWM_ID_2] |= PWM_RDMA0_STATUS_ERR_INT_MASK;
    }

    if (pwm_int_buf & PWM_RDMA1_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_2] |= PWM_RDMA1_STATUS_INT_MASK;
        pwm_rdma1_next_address[PWM_ID_2] = pwm->PWM_RDMA1_R0;
    }

    if (pwm_int_buf & PWM_RDMA1_STATUS_ERR_INT_MASK)
    {
        pwm->PWM_INT_MASK |= PWM_RDMA1_ERR_INT_MASK_ENABLE;

        pwm_int_status[PWM_ID_2] |= PWM_RDMA1_STATUS_ERR_INT_MASK;
    }

    if (pwm_int_buf & PWM_RSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_2] |= PWM_RSEQ_DONE_STATUS_INT_MASK;
        pwm_rdma0_next_address[PWM_ID_2] = pwm->PWM_RDMA0_R0;
    }

    if (pwm_int_buf & PWM_TSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_2] |= PWM_TSEQ_DONE_STATUS_INT_MASK;
        pwm_rdma1_next_address[PWM_ID_2] = pwm->PWM_RDMA1_R0;
    }

    if (pwm_int_buf & PWM_TRSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_2] |= PWM_TRSEQ_DONE_STATUS_INT_MASK;
    }
}


/**
 * @brief Function for handling the PWM3 interrupt.
 * @details Checks for PWM interrupt status, and executes PWM handlers for the corresponding status.
 * @param[in] None
 * @return None
 */
void Pwm3_Handler(void)
{
    uint32_t pwm_int_buf;
    pwm_t *pwm = PWM3;

    pwm_int_buf = pwm->PWM_INT_STATUS;
    pwm->PWM_INT_CLEAR = pwm_int_buf;

    //pwm_int_status[PWM_ID_3] = pwm_int_buf;

    if (pwm_int_buf & PWM_RDMA0_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_3] |= PWM_RDMA0_STATUS_INT_MASK;
        pwm_rdma0_next_address[PWM_ID_3] = pwm->PWM_RDMA0_R0;
    }

    if (pwm_int_buf & PWM_RDMA0_STATUS_ERR_INT_MASK)
    {
        pwm->PWM_INT_MASK |= PWM_RDMA0_ERR_INT_MASK_ENABLE;

        pwm_int_status[PWM_ID_3] |= PWM_RDMA0_STATUS_ERR_INT_MASK;
    }

    if (pwm_int_buf & PWM_RDMA1_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_3] |= PWM_RDMA1_STATUS_INT_MASK;
        pwm_rdma1_next_address[PWM_ID_3] = pwm->PWM_RDMA1_R0;
    }

    if (pwm_int_buf & PWM_RDMA1_STATUS_ERR_INT_MASK)
    {
        pwm->PWM_INT_MASK |= PWM_RDMA1_ERR_INT_MASK_ENABLE;

        pwm_int_status[PWM_ID_3] |= PWM_RDMA1_STATUS_ERR_INT_MASK;
    }

    if (pwm_int_buf & PWM_RSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_3] |= PWM_RSEQ_DONE_STATUS_INT_MASK;
        pwm_rdma0_next_address[PWM_ID_3] = pwm->PWM_RDMA0_R0;
    }

    if (pwm_int_buf & PWM_TSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_3] |= PWM_TSEQ_DONE_STATUS_INT_MASK;
        pwm_rdma1_next_address[PWM_ID_3] = pwm->PWM_RDMA1_R0;
    }

    if (pwm_int_buf & PWM_TRSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_3] |= PWM_TRSEQ_DONE_STATUS_INT_MASK;
    }
}


/**
 * @brief Function for handling the PWM4 interrupt.
 * @details Checks for PWM interrupt status, and executes PWM handlers for the corresponding status.
 * @param[in] None
 * @return None
 */
void Pwm4_Handler(void)
{
    uint32_t pwm_int_buf;
    pwm_t *pwm = PWM4;

    pwm_int_buf = pwm->PWM_INT_STATUS;
    pwm->PWM_INT_CLEAR = pwm_int_buf;

    //pwm_int_status[PWM_ID_4] = pwm_int_buf;

    if (pwm_int_buf & PWM_RDMA0_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_4] |= PWM_RDMA0_STATUS_INT_MASK;
        pwm_rdma0_next_address[PWM_ID_4] = pwm->PWM_RDMA0_R0;
    }

    if (pwm_int_buf & PWM_RDMA0_STATUS_ERR_INT_MASK)
    {
        pwm->PWM_INT_MASK |= PWM_RDMA0_ERR_INT_MASK_ENABLE;

        pwm_int_status[PWM_ID_4] |= PWM_RDMA0_STATUS_ERR_INT_MASK;
    }

    if (pwm_int_buf & PWM_RDMA1_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_4] |= PWM_RDMA1_STATUS_INT_MASK;
        pwm_rdma1_next_address[PWM_ID_4] = pwm->PWM_RDMA1_R0;
    }

    if (pwm_int_buf & PWM_RDMA1_STATUS_ERR_INT_MASK)
    {
        pwm->PWM_INT_MASK |= PWM_RDMA1_ERR_INT_MASK_ENABLE;

        pwm_int_status[PWM_ID_4] |= PWM_RDMA1_STATUS_ERR_INT_MASK;
    }

    if (pwm_int_buf & PWM_RSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_4] |= PWM_RSEQ_DONE_STATUS_INT_MASK;
        pwm_rdma0_next_address[PWM_ID_4] = pwm->PWM_RDMA0_R0;
    }

    if (pwm_int_buf & PWM_TSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_4] |= PWM_TSEQ_DONE_STATUS_INT_MASK;
        pwm_rdma1_next_address[PWM_ID_4] = pwm->PWM_RDMA1_R0;
    }

    if (pwm_int_buf & PWM_TRSEQ_DONE_STATUS_INT_MASK)
    {
        pwm_int_status[PWM_ID_4] |= PWM_TRSEQ_DONE_STATUS_INT_MASK;
    }
}



