/** @file sadc.h
 *
 * @brief SAR ADC driver file.
 *
 */
/**
 * @defgroup sadc_group SADC
 * @ingroup peripheral_group
 * @{
 * @brief Define SADC definitions, structures, and functions.
 */
#ifndef __SADC_H__
#define __SADC_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/

/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define SADC_NEW_DRV 1
#define SADC_NEW_TEMP_CALIBRATION 1

#define SADC_PNSEL_CH_REG_DEFAULT 0x240000FF
#define SADC_SET_CH_REG_DEFAULT 0x80000000
#define SADC_THD_CH_REG_DEFAULT 0x3FFF0000

#define SADC_MONITOR_LOW_THD_DEFAULT 0
#define SADC_MONITOR_HIGH_THD_DEFAULT 0x3FFF

#define SADC_CH_REG_OFFSET 4 /*unit: 4-bytes*/

#define SADC_INT_DISABLE_ALL 0xFFFFFFFF
#define SADC_INT_CLEAR_ALL 0xFFFFFFFF

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
typedef int32_t sadc_value_t;

typedef enum
{
    SADC_RES_8BIT  = 0, /**< 8 bit resolution. */
    SADC_RES_10BIT = 1, /**< 10 bit resolution. */
    SADC_RES_12BIT = 2, /**< 12 bit resolution. */
    SADC_RES_14BIT = 3, /**< 14 bit resolution. */
} sadc_config_resolution_t;

typedef enum
{
    SADC_OVERSAMPLE_0   = 0, /**< No oversampling*/
    SADC_OVERSAMPLE_2   = 1, /**< Oversampling ratio multiple 2*/
    SADC_OVERSAMPLE_4   = 2, /**< Oversampling ratio multiple 4*/
    SADC_OVERSAMPLE_8   = 3, /**< Oversampling ratio multiple 8*/
    SADC_OVERSAMPLE_16  = 4, /**< Oversampling ratio multiple 16*/
    SADC_OVERSAMPLE_32  = 5, /**< Oversampling ratio multiple 32*/
    SADC_OVERSAMPLE_64  = 6, /**< Oversampling ratio multiple 64*/
    SADC_OVERSAMPLE_128 = 7, /**< Oversampling ratio multiple 128*/
    SADC_OVERSAMPLE_256 = 8, /**< Oversampling ratio multiple 256*/
} sadc_config_oversample_t;

typedef enum
{
    SADC_SAMPLE_START = 0, /**< SADC conversion is started by software SADC start in SADC sample one shot mode*/
    SADC_SAMPLE_TIMER = 1, /**< SADC conversion is started by Timer in SADC sample timer mode*/
} sadc_config_sample_t;

typedef enum
{
    SADC_TIMER_SYSTEM_CLK = 0, /**< Select timer clock source to system clock(48MHz/32MHz) in SADC sample timer mode*/
    SADC_TIMER_SLOW_CLK   = 1, /**< Select timer clock source to slow clock (32KHz) in SADC sample timer mode*/
} sadc_config_timer_clk_t;

typedef enum
{
    SADC_CHANNEL_0   = 0,  /**< Channel 0. */
    SADC_CHANNEL_1   = 1,  /**< Channel 1. */
    SADC_CHANNEL_2   = 2,  /**< Channel 2. */
    SADC_CHANNEL_3   = 3,  /**< Channel 3. */
    SADC_CHANNEL_4   = 4,  /**< Channel 4. */
    SADC_CHANNEL_5   = 5,  /**< Channel 5. */
    SADC_CHANNEL_6   = 6,  /**< Channel 6. */
    SADC_CHANNEL_7   = 7,  /**< Channel 7. */
    SADC_CHANNEL_8   = 8,  /**< Channel 8. */
    SADC_CHANNEL_9   = 9,  /**< Channel 9. */
    SADC_CHANNEL_MAX = 10, /**< Max Channel 10. */
} sadc_config_channel_t;

/**
 * @enum sadc_config_input_t
 * @brief Input selection of the analog-to-digital converter.
 */
typedef enum
{
    SADC_AIN_0        = 0,  /**< Input 0. */
    SADC_AIN_1        = 1,  /**< Input 1. */
    SADC_AIN_2        = 2,  /**< Input 2. */
    SADC_AIN_3        = 3,  /**< Input 3. */
    SADC_AIN_4        = 4,  /**< Input 4. */
    SADC_AIN_5        = 5,  /**< Input 5. */
    SADC_AIN_6        = 6,  /**< Input 6. */
    SADC_AIN_7        = 7,  /**< Input 7. */
    SADC_AIN_8        = 8,  /**< Input 8. Temperature Sensor */
    SADC_TEMPERATURE  = 8,  /**< Input 8. Temperature Sensor */
    SADC_AIN_9        = 9,  /**< Input 9. Bypass VGA */
    SADC_BYPASS_VGA   = 9,  /**< Input 9. Bypass VGA */
    SADC_AIN_10       = 10, /**< Input 10. VBAT */
    SADC_VBAT         = 10, /**< Input 10. VBAT */
    SADC_VBAT_0VADC   = 10, /**< Input 10. VBAT */
    SADC_AIN_DISABLED = 11, /**< No input selected. */
} sadc_config_input_t;

#if (SADC_NEW_DRV == 1)
typedef enum
{
    SADC_CH_AIN0          = 0,  /**< Input AIO0. */
    SADC_CH_AIN1          = 1,  /**< Input AIO1. */
    SADC_CH_AIN2          = 2,  /**< Input AIO2. */
    SADC_CH_AIN3          = 3,  /**< Input AIO3. */
    SADC_CH_AIN4          = 4,  /**< Input AIO4. */
    SADC_CH_AIN5          = 5,  /**< Input AIO5. */
    SADC_CH_AIN6          = 6,  /**< Input AIO6. */
    SADC_CH_AIN7          = 7,  /**< Input AIO7. */
    SADC_CH_VBAT          = 8,  /**< Input VBAT. */
    SADC_CH_NC            = 13, /**< Input No Connect. */
    SADC_COMP_VBAT        = 9,  /**< Input VBAT. */
    SADC_COMP_TEMPERATURE = 10, /**< Input TEMPERATURE. */
    SADC_COMP_VCM         = 11, /**< Input VCM. */
    SADC_0VADC            = 12, /**< Input VBAT 0V. */
} sadc_input_ch_t;
#endif

typedef enum
{
    SADC_TACQ_EDLY_TIME_0P3US = 0,
    SADC_TACQ_EDLY_TIME_1US   = 1,
    SADC_TACQ_EDLY_TIME_2US   = 2,
    SADC_TACQ_EDLY_TIME_3US   = 3,
    SADC_TACQ_EDLY_TIME_4US   = 4,
    SADC_TACQ_EDLY_TIME_8US   = 5,
    SADC_TACQ_EDLY_TIME_12US  = 6,
    SADC_TACQ_EDLY_TIME_16US  = 7,
} sadc_config_tacq_edly_t;
/**
 * @brief SADC burst enablg types.
 */
typedef enum
{
    SADC_BURST_DISABLE = 0,
    SADC_BURST_ENABLE  = 1,
} sadc_config_burst_t;

/**
 * @brief SADC callback types.
 */
typedef enum
{
    SADC_CB_DONE,   /**< CB generated when the buffer is filled with samples. */
    SADC_CB_SAMPLE, /**< CB generated when the requested channel is sampled. */
} sadc_cb_type_t;

/**
 * @brief SADC convert status types.
 */
typedef enum
{
    SADC_CONVERT_IDLE = 0,
    SADC_CONVERT_START,
    SADC_CONVERT_DONE,
} sadc_convert_state_t;

/**
 * @brief SADC calibration types.
 */
typedef enum
{
    SADC_CALIBRATION_VBAT = 0,
    SADC_CALIBRATION_AIO,
} sadc_cal_type_t;

typedef struct
{
    uint32_t enable;
    uint32_t start_addr;
    uint16_t seg_size;
    uint16_t blk_size;
} sadc_config_xdma_t;
/**
 * @brief SADC timer config struct
 */
typedef struct
{
    sadc_config_timer_clk_t timer_clk_src;
    uint32_t timer_clk_div;
} sadc_config_timer_t;

/**
 * @brief ADC configuration.
 */
typedef struct
{
    // uint8_t interrupt_priority;                 /**< Priority of SADC interrupt*/
    sadc_int_t sadc_int_mask;
    sadc_config_resolution_t sadc_resolution; /**< SADC resolution*/
    sadc_config_oversample_t sadc_oversample; /**< SADC oversample*/
    sadc_config_xdma_t sadc_xdma;             /**< SADC XDMA*/
    sadc_config_sample_t sadc_sample_mode;    /**< SADC sample mode*/
    sadc_config_timer_t sadc_timer;           /**< SADC timer*/
} sadc_config_t;

typedef struct
{
    sadc_config_channel_t ch_sel; /**< SADC AIN0~AIN9 channel Selection*/
    sadc_config_input_t pi_sel;   /**< SADC AIN0~AIN9 P channel  Selection*/
    sadc_config_input_t ni_sel;   /**< SADC AIN0~AIN9 N channel  Selection*/
    uint32_t gain;                /**< SADC AIN0~AIN9 Gain settings*/
    uint32_t pull;                /**< SADC AIN0~AIN9 Pull high or low*/
    sadc_config_tacq_edly_t tacq; /**< SADC result acquisition time for system clock 32M*/
    sadc_config_tacq_edly_t edly; /**< SADC end delay time for system clock 32M */
    sadc_config_burst_t burst;    /**< SADC Burst mode selection*/
    uint32_t low_thd;             /**< SADC Low threshold*/
    uint32_t high_thd;            /**< SADC High threshold*/
} sadc_channel_config_t;

/**
 * @brief Analog-to-digital converter driver DONE cb.
 */
typedef struct
{
    sadc_value_t * p_buffer; /**< Pointer to buffer with converted samples. */
    uint16_t size;           /**< Number of samples in the buffer. */
} sadc_done_cb_t;

/**
 * @brief Analog-to-digital converter driver SAMPLE cb.
 */
#if (SADC_NEW_DRV == 1)
typedef struct
{
    sadc_value_t conversion_value;   /**< Converted sample. */
    sadc_value_t compensation_value; /**< Compensation sample. */
    sadc_value_t calibration_value;  /**< Calibration sample. */
} sadc_raw_cb_t;
#endif

typedef struct
{
    sadc_value_t value; /**< Converted sample. */
    uint32_t channel;
} sadc_sample_cb_t;

/**
 * @brief Analog-to-digital converter driver cb.
 */
typedef struct
{
    sadc_cb_type_t type; /**< CB type. */
#if (SADC_NEW_DRV == 1)
    sadc_raw_cb_t raw;
#endif
    union
    {
        sadc_done_cb_t done;     /**< Data for DONE cb. */
        sadc_sample_cb_t sample; /**< Data for SAMPLE cb. */
    } data;
} sadc_cb_t;

/**
 * @brief User cb handler prototype.
 *
 * This function is called when the requested number of samples has been processed.
 *
 * @param p_cb CB.
 */
typedef void (*sadc_isr_handler_t)(sadc_cb_t * p_cb);

/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/
#define SADC_RESET() (SADC->SADC_CTL1.bit.CFG_SADC_RST = ENABLE)             /**< Reset the SADC module*/
#define SADC_ENABLE() (SADC->SADC_CTL0.bit.CFG_SADC_ENA = ENABLE)            /**< Enable the SADC module*/
#define SADC_DISABLE() (SADC->SADC_CTL0.bit.CFG_SADC_ENA = DISABLE)          /**< Disable the SADC module*/
#define SADC_START() (SADC->SADC_CTL2.bit.CFG_SADC_START = ENABLE)           /**< Start the SADC module to trigger SADC conversion*/
#define SADC_RES_BIT(para_set) (SADC->SADC_SET1.bit.CFG_SADC_BIT = para_set) /**< Configure the SADC resolution*/
#define SADC_OVERSAMPLE_RATE(para_set) (SADC->SADC_SET1.bit.CFG_SADC_OSR = para_set) /**< Configure the SADC oversample time       \
                                                                                        value*/
#define SADC_SAMPLE_MODE(para_set)                                                                                                 \
    (SADC->SADC_SET0.bit.CFG_SADC_SMP_MODE = para_set)  /**< Configure the SADC sample rate mode (one shot mode and timer mode)*/
#define SADC_GET_SAMPLE_MODE()                                                                                                     \
    (SADC->SADC_SET0.bit.CFG_SADC_SMP_MODE)             /**< Return the SADC sample rate mode (one shot mode and timer mode)*/
#define SADC_TIMER_CLOCK(para_set)                                                                                                 \
    (SADC->SADC_SET0.bit.CFG_SADC_TMR_CKSEL = para_set) /**< Select timer clock source in SADC sample timer mode*/
#define SADC_TIMER_CLOCK_DIV(para_set)                                                                                             \
    (SADC->SADC_SET0.bit.CFG_SADC_TMR_CKDIV = para_set) /**< Configure timer clock divisor (1~65535) in SADC sample timer mode*/
#define SADC_GET_ADC_VALUE() (SADC->SADC_R0.bit.SADC_O) /**< Return the last SADC conversion result data for regular channel*/
#define SADC_GET_ADC_CHANNEL() (SADC->SADC_R0.bit.SADC_O_CHX) /**< Return the last SADC conversion channel*/
#define SADC_GET_XDMA_START_ADDRESS() (SADC->SADC_WDMA_SET1)  /**< Return the XDMA buffer address*/
#define SADC_GET_XDMA_NEXT_ADDRESS() (SADC->SADC_WDMA_R0)     /**< Return the status of XDMA next pointer address*/
#define SADC_GET_XDMA_RESULT_NUMBER()                                                                                              \
    (SADC->SADC_R1.bit.SADC_NUM_RES) /**< Return the number of SADC result write into WDMA since last SADC start*/
#define SADC_GET_RES_BIT() (SADC->SADC_SET1.bit.CFG_SADC_BIT)                        /**< Return the SADC resolution*/
#define SADC_SET_XDMA_START() (SADC->SADC_WDMA_CTL0.bit.CFG_SADC_WDMA_CTL0 = ENABLE) /**< Set the XDMA start*/

#define SADC_TEST_CONTROL(para_set) (SADC->SADC_SET1.bit.CFG_SADC_TST = para_set)    /**< Configure the SADC test mode*/
#define SADC_MANUAL_MODE() (SADC_TEST_CONTROL(8))                                    /**< Set the SADC to manual mode*/
#define SADC_VGA_ENABLE() (SADC->SADC_CTL0.bit.CFG_SADC_VGA_ENA = ENABLE)            /**< Enable the SADC VGA*/
#define SADC_VGA_DISABLE() (SADC->SADC_CTL0.bit.CFG_SADC_VGA_ENA = DISABLE)          /**< Disable the SADC VGA*/
#define SADC_LDO_ENABLE() (SADC->SADC_CTL0.bit.CFG_SADC_LDO_ENA = ENABLE)            /**< Enable the SADC LDO*/
#define SADC_LDO_DISABLE() (SADC->SADC_CTL0.bit.CFG_SADC_LDO_ENA = DISABLE)          /**< Disable the SADC LDO*/

#define SADC_TEST_ENABLE() (SADC->SADC_SET1.bit.CFG_SADC_TST = 1)                    /**< Enable the SADC test mode*/
#define SADC_TEST_ADJUST_VALUE(para_set)                                                                                           \
    (SADC->SADC_SET1.bit.CFG_SADC_VAL_TST = para_set)                                /**< Set the SADC test mode adjust value*/

/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/
/*
 * @brief Register the sadc interrupt callback service routine function
 * @param[in] sadc_int_callback config the interrupt callback service routine function
 * @return None
 */
void Sadc_Register_Int_Callback(sadc_isr_handler_t sadc_int_callback);
void Sadc_Register_Adcomp_Int_Callback(sadc_isr_handler_t sadc_comp_int_callback);
void Sadc_Register_Txcomp_Int_Callback(sadc_isr_handler_t sadc_txcomp_int_callback);

/*
 * @ingroup sadc_group
 * @brief Enable the specified SADC interrupts
 * @param[in] int_mask Specifies the SADC interrupt sources to be enabled
 *   This parameter can be any combination of the following values:
 *     @arg SADC_WDMA: SADC WDMA interrupt mask
 *     @arg SADC_WDMA_ERROR: SADC WDMA error interrupt mask
 *     @arg SADC_DONE: SADC conversion done interrupt mask
 *     @arg SADC_VALID: SADC conversion valid interrupt mask
 *     @arg SADC_MODE_DONE: SADC scan mode done interrupt mask
 *     @arg SADC_MONITOR_LOW: SADC monitor low interrupt mask for CH0 ~ CH9
 *     @arg SADC_MONITOR_HIGH: SADC monitor high interrupt mask for CH0 ~ CH9
 * @return None
 */
void Sadc_Int_Enable(uint32_t int_mask);

/*
 * @brief SADC interrupt disable
 * @return None
 */
void Sadc_Int_Disable(void);

/*
 * @brief Config the sadc channel
 * @param[in] config_channel
 *            \arg ch_sel   SADC AIN0~AIN9 channel Selection
 *            \arg pi_sel   SADC AIN0~AIN9 P Selection
 *            \arg ni_sel   SADC AIN0~AIN9 N Selection
 *            \arg gain     SADC AIN0~AIN9 Gain settings
 *            \arg pull     SADC AIN0~AIN9 Pull high or low
 *            \arg tacq     SADC result acquisition time for system clock 32M
 *            \arg edly     SADC end delay time for system clock 32M
 *            \arg burst    SADC Burst mode selection
 *            \arg low_thd  SADC Low threshold
 *            \arg high_thd SADC High threshold
 * @return None
 */
void Sadc_Channel_Enable(sadc_channel_config_t * config_channel);

/*
 * @brief SADC Channel Enable
 * @param[in] ch_sel SADC Channel Select (0~9)
 *            \arg SADC_CHANNEL_0 ~ SADC_CHANNEL_9
 * @return None
 */
void Sadc_Channel_Disable(sadc_config_channel_t ch_sel);

/*
 * @brief Config sadc xdma start address, segment size, block size
 * @param[in] xdma_start_addr config xdma start address
 * @param[in] xdma_seg_size   config xdma segment size
 * @param[in] xdma_blk_size   config xdma block size
 * @return None
 */
void Sadc_Xdma_Config(uint32_t xdma_start_addr, uint16_t xdma_seg_size, uint16_t xdma_blk_size);

/*
 * @brief SADC Temperature Enable
 * @return None
 */
void Sadc_Temperature_Enable(void);

/*
 * @brief SADC Temperature Disable
 * @return None
 */
void Sadc_Temperature_Disable(void);

/*
 * @brief Initinal SADC
 * @param[in] p_config config the sadc params
 *              \arg sadc_int
 *              \arg sadc_config_resolution
 *              \arg sadc_config_oversample
 *              \arg sadc_config_xdma
 *              \arg sadc_config_sample
 *              \arg sadc_config_timer
 * @param[in] sadc_int_callback config the interrupt service routine callback function
 * @return None
 */
uint32_t Sadc_Init(sadc_config_t * p_config, sadc_isr_handler_t sadc_int_callback);

/*
 * @brief SADC Disable
 * @param None
 * @return None
 */
void Sadc_Disable(void);

/*
 * @brief SADC Enable
 * @param None
 * @return None
 */
void Sadc_Enable(void);

/*
 * @brief SADC start convert
 * @param None
 * @return None
 */
void Sadc_Start(void);

/*
 * @ingroup sadc_group
 * @brief SADC resolution compensation
 * @param[in] p_data SADC Channel Value
 * @retval STATUS_SUCCESS SADC value resolution compensation is successful
 * @retval STATUS_INVALID_PARAM Wrong parameter, SADC value is invalid
 */
uint32_t Sadc_Resolution_Compensation(sadc_value_t * p_data);

/*
 * @brief Get SADC convert state
 * @param None
 * @return sadc_convert_state_t return SADC convert status
 *         \arg SADC_CONVERT_IDLE
 *         \arg SADC_CONVERT_START
 *         \arg SADC_CONVERT_DONE
 */
sadc_convert_state_t Sadc_Convert_State_Get(void);

/*
 * @brief SADC Calibration Initialization to read SADC related MP Sector
 * @param None
 * @return None
 *
 */
void Sadc_Calibration_Init(void);

/*
 * @brief SADC temperature value calibration
 * @param[in] adc_val SADC value before calibration
 * @return return SADC value after calibration
 */
uint32_t Sadc_Temperature_Calibration(sadc_value_t adc_val);

/*
 * @brief Update SADC temperature calibration offset value
 * @param[in] adc_val SADC value for updating calibration offset
 * @return None
 */
void Sadc_Temperature_Calibration_Offset_Update(sadc_value_t adc_val);

/*
 * @brief SADC value calibration
 * @param[in] cal_type SADC calibration type
 * @param[in] adc_val SADC value before calibration
 * @return return SADC value after calibration
 */
uint32_t Sadc_Calibration(sadc_cal_type_t cal_type, sadc_value_t adc_val);

/*
 * @brief Update SADC compensation offset value
 * @param[in] adc_val SADC value for updating compensation offset
 * @return None
 */
void Sadc_Compensation_Offset_Update(sadc_value_t adc_val);

/*
 * @brief SADC value compensation
 * @param[in] adc_val SADC value before compensation
 * @return return SADC value after compensation
 */
sadc_value_t Sadc_Compensation(sadc_value_t adc_val);

#if (SADC_NEW_DRV == 1)
/**
 * @brief SADC channel configuration, enable, and register the sadc interrupt callback service routine function
 * @param[in] res SADC resolution configuration
 * @param[in] os SADC oversample configuration
 * @param[in] sadc_int_callback SADC interrupt callback service routine function configuration
 * @return None
 */
void Sadc_Config_Enable(sadc_config_resolution_t res, sadc_config_oversample_t os, sadc_isr_handler_t sadc_int_callback);

/**
 * @brief Trigger to read SADC channel
 * @param[in] ch SADC channel assignment
 *         \arg SADC_CH_AIN0 Read SADC channel AIO0
 *         \arg SADC_CH_AIN1 Read SADC channel AIO1
 *         \arg SADC_CH_AIN2 Read SADC channel AIO2
 *         \arg SADC_CH_AIN3 Read SADC channel AIO3
 *         \arg SADC_CH_AIN4 Read SADC channel AIO4
 *         \arg SADC_CH_AIN5 Read SADC channel AIO5
 *         \arg SADC_CH_AIN6 Read SADC channel AIO6
 *         \arg SADC_CH_AIN7 Read SADC channel AIO7
 *         \arg SADC_CH_VBAT Read SADC channel VBAT
 * @return return SADC channel read status
 *         \arg STATUS_EBUSY Read SADC channel is busy and invalid
 *         \arg STATUS_SUCCESS Read SADC channel is successful
 */
uint32_t Sadc_Channel_Read(sadc_input_ch_t ch);

/**
 * @brief SADC Compensation Initialization to enable SW timer for periodic compensation
 * @param[in] xPeriodicTimeInSec Periodic time in second for SADC compensation
 * @return None
 *
 */
void Sadc_Compensation_Init(uint32_t xPeriodicTimeInSec);

/**
 * @brief SADC Compensation De-initialization to disable SW timer for periodic compensation
 * @param None
 * @return None
 *
 */
void Sadc_Compensation_Deinit(void);

void Sadc_Config_Enable_Vbat(sadc_config_resolution_t res, sadc_config_oversample_t os, sadc_isr_handler_t sadc_int_callback);
uint32_t Sadc_Channel_Read_Vbat(sadc_input_ch_t ch);
uint32_t Sadc_Init_vBAT(sadc_config_t * p_config, sadc_isr_handler_t sadc_int_callback);

#endif

#endif    /* end of _SADC_H_ */

/** @} */ /* End of Peripheral Group */
