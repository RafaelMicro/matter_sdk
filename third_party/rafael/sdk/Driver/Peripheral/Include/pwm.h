/** @file pwm.h
 *
 * @brief PWM driver header file.
 *
 */
/**
* @defgroup pmw_group PWM
* @ingroup peripheral_group
* @{
* @brief  Define PWM definitions, structures, and functions
*/
#ifndef __PWM_H__
#define __PWM_H__

#ifdef __cplusplus
extern "C"
{
#endif
/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/





/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**@brief PWM ID mapping.
 */
typedef enum
{
    PWM_ID_0,
    PWM_ID_1,
    PWM_ID_2,
    PWM_ID_3,
    PWM_ID_4,
    PWM_ID_MAX,
} pwm_id_t;

/**@brief PWM Clock division table.
 */
typedef enum
{
    PWM_CLK_DIV_1 = 0,
    PWM_CLK_DIV_2,
    PWM_CLK_DIV_4,
    PWM_CLK_DIV_8,
    PWM_CLK_DIV_16,
    PWM_CLK_DIV_32,
    PWM_CLK_DIV_64,
    PWM_CLK_DIV_128,
    PWM_CLK_DIV_256,
} pwm_clk_div_t;

/**@brief PWM Sequence order table.
 * Order_0: S0  /  Order_1: S1  /  Order_2: S0S1  /  Order_3: S1S0
 */
typedef enum
{
    PWM_SEQ_ORDER_R = 0,
    PWM_SEQ_ORDER_T,
    PWM_SEQ_ORDER_MAX,
}   pwm_seq_order_t;

/**@brief PWM sequence selection table.
 */
typedef enum
{
    PWM_SEQ_NUM_1,
    PWM_SEQ_NUM_2,
}   pwm_seq_num_t;

/**@brief PWM sequence playmode table.
 */
typedef enum
{
    PWM_SEQ_MODE_NONCONTINUOUS,
    PWM_SEQ_MODE_CONTINUOUS,
}   pwm_seq_mode_t;

/**@brief PWM trigger source table.
 */
typedef enum
{
    PWM_TRIGGER_SRC_PWM0 = 0,
    PWM_TRIGGER_SRC_PWM1,
    PWM_TRIGGER_SRC_PWM2,
    PWM_TRIGGER_SRC_PWM3,
    PWM_TRIGGER_SRC_PWM4,
    PWM_TRIGGER_SRC_SELF = 7,
}   pwm_trigger_src_t;

/**@brief PWM DMA sample format table.
 */
typedef enum
{
    PWM_DMA_SMP_FMT_0 = 0,
    PWM_DMA_SMP_FMT_1,
}   pwm_dma_smp_fmt_t;

/**@brief PWM counter mode table.
 * UP: Up mode / UD: Up-Down mode
 */
typedef enum
{
    PWM_COUNTER_MODE_UP = 0,
    PWM_COUNTER_MODE_UD,
} pwm_counter_mode_t;

/**@brief PWM DMA auto table.
 */
typedef enum
{
    PWM_DMA_AUTO_DISABLE = 0,
    PWM_DMA_AUTO_ENABLE,
} pwm_dma_auto_t;

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define PWM_ENABLE_PWM                  (0x01UL << PWM_CFG0_PWM_ENA_SHFT)
#define PWM_ENABLE_CLK                  (0x01UL << PWM_CFG0_CK_ENA_SHFT)
#define PWM_RESET                       (0x01UL << PWM_CFG0_PWM_RST_SHFT)
#define PWM_RDMA_ENABLE                 (0x01UL << PWM_CFG0_PWM_RDMA0_CTL0_SHFT)
#define PWM_RDMA_RESET                  (0x01UL << PWM_CFG0_PWM_RDMA0_CTL1_SHFT)

#define PWM_RDMA0_INT_CLR               (0x01UL << PWM_RDMA0_INT_CLR_SHFT)
#define PWM_RDMA0_ERR_INT_CLR           (0x01UL << PWM_RDMA0_ERR_INT_CLR_SHFT)
#define PWM_RDMA1_INT_CLR               (0x01UL << PWM_RDMA1_INT_CLR_SHFT)
#define PWM_RDMA1_ERR_INT_CLR           (0x01UL << PWM_RDMA1_ERR_INT_CLR_SHFT)
#define PWM_RSEQ_DONE_INT_CLR           (0x01UL << PWM_RSEQ_DONE_INT_CLR_SHFT)
#define PWM_TSEQ_DONE_INT_CLR           (0x01UL << PWM_TSEQ_DONE_INT_CLR_SHFT)
#define PWM_TRSEQ_DONE_INT_CLR          (0x01UL << PWM_TRSEQ_DONE_INT_CLR_SHFT)

#define PWM_RDMA0_INT_MASK_ENABLE       (0x01UL << PWM_RDMA0_INT_MASK_SHFT)
#define PWM_RDMA0_ERR_INT_MASK_ENABLE   (0x01UL << PWM_RDMA0_ERR_INT_MASK_SHFT)
#define PWM_RDMA1_INT_MASK_ENABLE       (0x01UL << PWM_RDMA1_INT_MASK_SHFT)
#define PWM_RDMA1_ERR_INT_MASK_ENABLE   (0x01UL << PWM_RDMA1_ERR_INT_MASK_SHFT)
#define PWM_RSEQ_DONE_INT_MASK_ENABLE   (0x01UL << PWM_RSEQ_DONE_INT_MASK_SHFT)
#define PWM_TSEQ_DONE_INT_MASK_ENABLE   (0x01UL << PWM_TSEQ_DONE_INT_MASK_SHFT)
#define PWM_TRSEQ_DONE_INT_MASK_ENABLE  (0x01UL << PWM_TRSEQ_DONE_INT_MASK_SHFT)

/**@brief Convert THD_Value / End_Value / PHA_Value into a 32-bit data
 * Mode0: val1=THD1, val2=THD2
 * Mode1: val0=PHA, val1=THD, val2=end
 */
/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/
#define PWM_FILL_SAMPLE_DATA_MODE0(val0,val1,val2)  ((val0 << 31) | (val2 << 16) | (val0 << 15) | (val1))
#define PWM_FILL_SAMPLE_DATA_MODE1(val0,val1,val2)  ((val2 << 16) | (val0 << 15) | (val1))

/**@brief Structure for each RDMA configurations
 */
typedef struct
{
    uint32_t    pwm_rdma_addr;                 /**< xDMA start address configurations for PWM sequence controller. */
    uint16_t    pwm_element_num;               /**< Element number configurations for PWM sequence controller. */
    uint16_t    pwm_repeat_num;                /**< Repeat number configurations of each element for PWM sequence controller. */
    uint16_t    pwm_delay_num;                 /**< Delay number configurations after PWM sequence is play finish for PWM sequence controller. */
} pwm_seq_para_t;

/**
* @brief Structure for each PWM configurations
*/
typedef struct
{
    pwm_seq_para_t        pwm_seq0;            /**< Handle of PWM sequence controller configurations for R-SEQ. */
    pwm_seq_para_t        pwm_seq1;            /**< Handle of PWM sequence controller configurations for T-SEQ. */
    uint16_t              pwm_play_cnt;        /**< PWM play amount configuration. */
    uint16_t              pwm_count_end_val;   /**< PWM counter end value configuration. */
    pwm_seq_order_t       pwm_seq_order;       /**< PWM sequence play order configuration. */
    pwm_trigger_src_t     pwm_triggered_src;   /**< PWM play trigger source configuration. */
    pwm_seq_num_t         pwm_seq_num;         /**< PWM sequence number configuration. */
    pwm_id_t              pwm_id;              /**< PWM ID designation. */
    pwm_clk_div_t         pwm_clk_div;         /**< PWM gated clock divider value configuration. */
    pwm_counter_mode_t    pwm_counter_mode;    /**< PWM counter mode configuration. */
    pwm_dma_smp_fmt_t     pwm_dma_smp_fmt;     /**< PWM DMA sample format configuration. */
    pwm_seq_mode_t        pwm_seq_mode;        /**< PWM sequence play mode configuration. */
} pwm_seq_para_head_t;

extern uint32_t pwm_int_status[PWM_ID_MAX];

/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/
/**
 * @brief Function to config pwm params
 * @param[in] pwm_para_config
 *            \arg pwm_seq0             Handle of PWM sequence controller configurations for R-SEQ
 *            \arg pwm_seq1             Handle of PWM sequence controller configurations for T-SEQ
 *            \arg pwm_play_cnt         PWM play amount configuration
 *            \arg pwm_count_end_val    PWM counter end value configuration
 *            \arg pwm_seq_order        PWM sequence play order configuration
 *            \arg pwm_triggered_src    PWM play trigger source configuration
 *            \arg pwm_seq_num          PWM sequence number configuration
 *            \arg pwm_id               PWM ID designation
 *            \arg pwm_clk_div          PWM gated clock divider value configuration
 *            \arg pwm_counter_mode     PWM counter mode configuration
 *            \arg pwm_dma_smp_fmt      PWM DMA sample format configuration
 *            \arg pwm_seq_mode         PWM sequence play mode configuration
 * @return None
 */
uint32_t Pwm_Init(pwm_seq_para_head_t *pwm_para_config);

/**
 * @brief Function to enable pwm irq,clock,rdam
 * @param[in] pwm_para_config
 *            \arg pwm_seq_order        PWM sequence play order configuration
 *            \arg pwm_seq_num          PWM sequence number configuration
 *            \arg pwm_id               PWM ID designation
 * @retval STATUS_SUCCESS config pwm irq,clock,rdma registers is vaild
 * @retval STATUS_INVALID_PARAM config pwm irq,clock,rdma registers is invaild
 */
uint32_t Pwm_Start(pwm_seq_para_head_t *pwm_para_config);

#ifdef __cplusplus
}
#endif

#endif /* End of _PWM_H_ */

/** @} */ /* End of Peripheral Group */
