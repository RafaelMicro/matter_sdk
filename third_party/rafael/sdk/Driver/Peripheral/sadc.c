/** @file sadc.c
 *
 * @brief SAR ADC driver file.
 *
 */
/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdio.h>
#include <string.h>

#include "cm3_mcu.h"
#include "mp_sector.h"


#if (SUPPORT_FREERTOS_PORT == 1)
#include "FreeRTOS.h"
#include "timers.h"
#endif


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define SADC_TEST_MODE          0

#if (SADC_TEST_MODE == 1)
#define SADC_TEST_VALUE         0x5AF
#endif

#define SADC_CALIBRATION_VALUE  0

#if (SADC_NEW_DRV == 1)
#define SADC_GAIN_AIN          0x14    /* VGA gain selection, [3:0] +3dB/step, [5:4] +6dB/step */
#define SADC_PULL_AIN          0x00    /* Channel pull selection, [0] P-CH pull high, [1] P-CH pull low, [2] N-CH pull high, [3] N-CH pull low */
#define SADC_GAIN_VBAT         0x14    /* VGA gain selection, [3:0] +3dB/step, [5:4] +6dB/step */
#define SADC_PULL_VBAT         0x03    /* Channel pull selection, [0] P-CH pull high, [1] P-CH pull low, [2] N-CH pull high, [3] N-CH pull low */
#define SADC_GAIN_VCM          0x14    /* VGA gain selection, [3:0] +3dB/step, [5:4] +6dB/step */
#define SADC_PULL_VCM          0x00    /* Channel pull selection, [0] P-CH pull high, [1] P-CH pull low, [2] N-CH pull high, [3] N-CH pull low */
#define SADC_GAIN_TEMPERATURE  0x14    /* VGA gain selection, [3:0] +3dB/step, [5:4] +6dB/step */
#define SADC_PULL_TEMPERATURE  0x00    /* Channel pull selection, [0] P-CH pull high, [1] P-CH pull low, [2] N-CH pull high, [3] N-CH pull low */
#define SADC_PULL_OVADC        0x02    /* Channel pull selection, P-CH pull low, N-CH no pull*/

#if (SADC_NEW_TEMP_CALIBRATION==1)
#define TEMPERATURE_CALIBRATION_CONST     111820
#define TEMPERATURE_CALIBRATION_RATIO     1000
#else
#define TEMPERATURE_CALIBRATION_GOLDEN    2128
#define TEMPERATURE_CALIBRATION_RATIO     100000
#define TEMPERATURE_CALIBRATION_SLOPE     16461
#define TEMPERATURE_CALIBRATION_OFFSET    32522815
#endif

#endif

#define SADC_COMP_DEBUG         0


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/


/**************************************************************************************************
 *    GLOBAL VARIABLES
 *************************************************************************************************/
static sadc_isr_handler_t   sadc_reg_handler = NULL;
static sadc_isr_handler_t   sadc_adcomp_handler = NULL;
static sadc_isr_handler_t   sadc_txcomp_handler = NULL;
static uint32_t sadc_xdma_single_mode = DISABLE;
static sadc_convert_state_t  sadc_convert_state = SADC_CONVERT_IDLE;
static sadc_convert_state_t  sadc_ch_read_convert_state = SADC_CONVERT_IDLE;
static int32_t sadc_compensation_offset = 0;
static int32_t sadc_temperature_calibration_offset = 0;

#if (SADC_NEW_TEMP_CALIBRATION==1)
static int32_t Cvbat = 0;
static int32_t C0vAdc = 0;
static int32_t VbatAdc = 0;
#endif

#if (SADC_NEW_DRV == 1)
static sadc_input_ch_t  sadc_convert_ch = SADC_CH_NC;
static sadc_value_t sadc_ch_value;

sadc_channel_config_t sadc_ch_init[] =
{
    {SADC_CHANNEL_0, SADC_AIN_0, SADC_AIN_DISABLED, SADC_GAIN_AIN, SADC_PULL_AIN, SADC_TACQ_EDLY_TIME_16US, SADC_TACQ_EDLY_TIME_16US, SADC_BURST_ENABLE, SADC_MONITOR_LOW_THD_DEFAULT, SADC_MONITOR_HIGH_THD_DEFAULT},
    {SADC_CHANNEL_0, SADC_AIN_1, SADC_AIN_DISABLED, SADC_GAIN_AIN, SADC_PULL_AIN, SADC_TACQ_EDLY_TIME_16US, SADC_TACQ_EDLY_TIME_16US, SADC_BURST_ENABLE, SADC_MONITOR_LOW_THD_DEFAULT, SADC_MONITOR_HIGH_THD_DEFAULT},
    {SADC_CHANNEL_0, SADC_AIN_2, SADC_AIN_DISABLED, SADC_GAIN_AIN, SADC_PULL_AIN, SADC_TACQ_EDLY_TIME_16US, SADC_TACQ_EDLY_TIME_16US, SADC_BURST_ENABLE, SADC_MONITOR_LOW_THD_DEFAULT, SADC_MONITOR_HIGH_THD_DEFAULT},
    {SADC_CHANNEL_0, SADC_AIN_3, SADC_AIN_DISABLED, SADC_GAIN_AIN, SADC_PULL_AIN, SADC_TACQ_EDLY_TIME_16US, SADC_TACQ_EDLY_TIME_16US, SADC_BURST_ENABLE, SADC_MONITOR_LOW_THD_DEFAULT, SADC_MONITOR_HIGH_THD_DEFAULT},
    {SADC_CHANNEL_0, SADC_AIN_4, SADC_AIN_DISABLED, SADC_GAIN_AIN, SADC_PULL_AIN, SADC_TACQ_EDLY_TIME_16US, SADC_TACQ_EDLY_TIME_16US, SADC_BURST_ENABLE, SADC_MONITOR_LOW_THD_DEFAULT, SADC_MONITOR_HIGH_THD_DEFAULT},
    {SADC_CHANNEL_0, SADC_AIN_5, SADC_AIN_DISABLED, SADC_GAIN_AIN, SADC_PULL_AIN, SADC_TACQ_EDLY_TIME_16US, SADC_TACQ_EDLY_TIME_16US, SADC_BURST_ENABLE, SADC_MONITOR_LOW_THD_DEFAULT, SADC_MONITOR_HIGH_THD_DEFAULT},
    {SADC_CHANNEL_0, SADC_AIN_6, SADC_AIN_DISABLED, SADC_GAIN_AIN, SADC_PULL_AIN, SADC_TACQ_EDLY_TIME_16US, SADC_TACQ_EDLY_TIME_16US, SADC_BURST_ENABLE, SADC_MONITOR_LOW_THD_DEFAULT, SADC_MONITOR_HIGH_THD_DEFAULT},
    {SADC_CHANNEL_0, SADC_AIN_7, SADC_AIN_DISABLED, SADC_GAIN_AIN, SADC_PULL_AIN, SADC_TACQ_EDLY_TIME_16US, SADC_TACQ_EDLY_TIME_16US, SADC_BURST_ENABLE, SADC_MONITOR_LOW_THD_DEFAULT, SADC_MONITOR_HIGH_THD_DEFAULT},
    {SADC_CHANNEL_0, SADC_VBAT, SADC_AIN_DISABLED, SADC_GAIN_VBAT, SADC_PULL_VBAT, SADC_TACQ_EDLY_TIME_16US, SADC_TACQ_EDLY_TIME_16US, SADC_BURST_ENABLE, SADC_MONITOR_LOW_THD_DEFAULT, SADC_MONITOR_HIGH_THD_DEFAULT},
    {SADC_CHANNEL_0, SADC_VBAT, SADC_AIN_DISABLED, SADC_GAIN_VBAT, SADC_PULL_VBAT, SADC_TACQ_EDLY_TIME_16US, SADC_TACQ_EDLY_TIME_16US, SADC_BURST_ENABLE, SADC_MONITOR_LOW_THD_DEFAULT, SADC_MONITOR_HIGH_THD_DEFAULT},
    {SADC_CHANNEL_0, SADC_TEMPERATURE, SADC_TEMPERATURE, SADC_GAIN_TEMPERATURE, SADC_PULL_TEMPERATURE, SADC_TACQ_EDLY_TIME_16US, SADC_TACQ_EDLY_TIME_16US, SADC_BURST_ENABLE, SADC_MONITOR_LOW_THD_DEFAULT, SADC_MONITOR_HIGH_THD_DEFAULT},
    {SADC_CHANNEL_0, SADC_VBAT, SADC_VBAT, SADC_GAIN_VCM, SADC_PULL_VCM, SADC_TACQ_EDLY_TIME_16US, SADC_TACQ_EDLY_TIME_16US, SADC_BURST_ENABLE, SADC_MONITOR_LOW_THD_DEFAULT, SADC_MONITOR_HIGH_THD_DEFAULT},
    /*0V ADC Check for temperature sensor*/
    {SADC_CHANNEL_0, SADC_VBAT_0VADC, SADC_AIN_DISABLED, SADC_GAIN_VBAT, SADC_PULL_OVADC, SADC_TACQ_EDLY_TIME_16US, SADC_TACQ_EDLY_TIME_16US, SADC_BURST_ENABLE, SADC_MONITOR_LOW_THD_DEFAULT, SADC_MONITOR_HIGH_THD_DEFAULT}
};

#if (SUPPORT_FREERTOS_PORT == 1)
TimerHandle_t sadc_comp_timer_handle = NULL;
#endif

static sadc_config_resolution_t sadc_config_res = SADC_RES_12BIT;
static sadc_config_oversample_t sadc_config_os = SADC_OVERSAMPLE_256;
static sadc_isr_handler_t sadc_config_int_callback = NULL;

#if (LPWR_FLASH_PROTECT_ENABLE==1) &&  (LPWR_FLASH_VBAT_PROTECT_ENABLE==1)

static sadc_config_resolution_t sadc_config_vbat_res = SADC_RES_12BIT;
static sadc_config_oversample_t sadc_config_vbat_os = SADC_OVERSAMPLE_0;
static sadc_isr_handler_t sadc_config_int_vbat_callback = NULL;
#endif
#endif


/**************************************************************************************************
 *    LOCAL FUNCTIONS
 *************************************************************************************************/


/**************************************************************************************************
 *    GLOBAL FUNCTIONS
 *************************************************************************************************/

/**
 * @brief SADC interrupt handler
 * @details Clear the SADC interrupt status and handle the SADC interrupt routine
 * @param None
 * @return None
 */
void Sadc_Handler(void)
{
    sadc_cb_t cb;
    sadc_int_t reg_sadc_int_status;
    sadc_value_t  sadc_value;
    sadc_cal_type_t cal_type;

    reg_sadc_int_status.reg = SADC->SADC_INT_STATUS.reg;
    SADC->SADC_INT_CLEAR.reg = reg_sadc_int_status.reg;

    if (reg_sadc_int_status.reg != 0)
    {
        if (reg_sadc_int_status.bit.DONE == 1)
        {
        }
        if (reg_sadc_int_status.bit.VALID == 1)
        {
            if (sadc_convert_ch <= SADC_CH_VBAT)
            {
                if (sadc_reg_handler != NULL)
                {
                    cb.type = SADC_CB_SAMPLE;
                    sadc_value = SADC_GET_ADC_VALUE();

                    Sadc_Resolution_Compensation(&sadc_value);
                    cb.raw.conversion_value = sadc_value;

                    sadc_value = Sadc_Compensation(sadc_value);
                    cb.raw.compensation_value = sadc_value;

                    if (sadc_convert_ch <= SADC_CH_AIN7)
                    {
                        cal_type = SADC_CALIBRATION_AIO;
                    }
                    else if (sadc_convert_ch <= SADC_CH_VBAT)
                    {
                        cal_type = SADC_CALIBRATION_VBAT;
                    }
                    sadc_value = Sadc_Calibration(cal_type, sadc_value);
                    cb.raw.calibration_value = sadc_value;

                    if (sadc_convert_ch == SADC_CH_VBAT)
                    {
                        VbatAdc = sadc_value;   /*vbat value for Temperature sensor*/
                    }

                    cb.data.sample.value = sadc_value;
                    cb.data.sample.channel = sadc_convert_ch;

                    sadc_reg_handler(&cb);
                }
            }
            else if (sadc_convert_ch <= SADC_COMP_TEMPERATURE)
            {
                if (sadc_txcomp_handler != NULL)
                {
                    cb.type = SADC_CB_SAMPLE;
                    sadc_value = SADC_GET_ADC_VALUE();

                    if (sadc_convert_ch == SADC_COMP_VBAT)
                    {
                        Sadc_Resolution_Compensation(&sadc_value);
                        cb.raw.conversion_value = sadc_value;

                        Cvbat = sadc_value;     /*cvbat for temperature sensor*/

                        sadc_value = Sadc_Compensation(sadc_value);
                        cb.raw.compensation_value = sadc_value;

                        sadc_value = Sadc_Calibration(SADC_CALIBRATION_VBAT, sadc_value);
                        cb.raw.calibration_value = sadc_value;
                    }
                    else if (sadc_convert_ch == SADC_COMP_TEMPERATURE)
                    {
                        Sadc_Temperature_Disable();

                        Sadc_Resolution_Compensation(&sadc_value);
                        cb.raw.conversion_value = sadc_value;

#if (1)
                        sadc_value = Sadc_Compensation(sadc_value);
#endif

                        cb.raw.compensation_value = sadc_value;

                        sadc_value = Sadc_Temperature_Calibration(sadc_value);

                        cb.raw.calibration_value = sadc_value;
                    }

                    cb.data.sample.value = sadc_value;
                    cb.data.sample.channel = sadc_convert_ch;

                    sadc_txcomp_handler(&cb);
                }
            }
            else if (sadc_convert_ch <= SADC_COMP_VCM)
            {
                if (sadc_adcomp_handler != NULL)
                {
                    cb.type = SADC_CB_SAMPLE;

                    sadc_value = SADC_GET_ADC_VALUE();

                    if (sadc_convert_ch == SADC_COMP_VCM)
                    {
                        //Sadc_Resolution_Compensation(&sadc_value);
                        sadc_value >>= 2;          // fix ADC resolution 12-bit for VCM temperature compensation
                        cb.raw.conversion_value = sadc_value;

                        cb.raw.compensation_value = sadc_value;

                        cb.raw.calibration_value = sadc_value;
                    }

                    cb.data.sample.value = sadc_value;
                    cb.data.sample.channel = sadc_convert_ch;

                    sadc_adcomp_handler(&cb);
                }
            }
            else if (sadc_convert_ch <= SADC_0VADC) /*0V ADC Value for temperature senor calcuation*/
            {
                if (sadc_txcomp_handler != NULL)
                {
                    cb.type = SADC_CB_SAMPLE;
                    sadc_value = SADC_GET_ADC_VALUE();

                    Sadc_Resolution_Compensation(&sadc_value);
                    cb.raw.conversion_value = sadc_value;

                    sadc_value = Sadc_Compensation(sadc_value);
                    cb.raw.compensation_value = sadc_value;

                    /*0V ADC Value for temperature senor calcuation*/
                    C0vAdc = sadc_value;

                    cb.data.sample.value = sadc_value;
                    cb.data.sample.channel = sadc_convert_ch;

                    //sadc_txcomp_handler(&cb);
                }
            }

            Lpm_Low_Power_Unmask(LOW_POWER_MASK_BIT_TASK_ADC);
            Sadc_Disable();

            sadc_ch_read_convert_state = SADC_CONVERT_DONE;
        }
        if (reg_sadc_int_status.bit.MODE_DONE == 1)
        {
            if (SADC_GET_SAMPLE_MODE() == SADC_SAMPLE_START)
            {
                sadc_convert_state = SADC_CONVERT_DONE;
            }

            /*
            if(sadc_reg_handler != NULL)
            {
                cb.type = SADC_CB_DONE;
                cb.data.done.p_buffer = (sadc_value_t *)(SADC_GET_XDMA_START_ADDRESS());
                cb.data.done.size = SADC_GET_XDMA_RESULT_NUMBER();
                sadc_reg_handler(&cb);
            }
            */
        }
        if (reg_sadc_int_status.bit.WDMA == 1)
        {
        }
        if (reg_sadc_int_status.bit.WDMA_ERROR == 1)
        {
        }
    }

    return;
}


void Sadc_Register_Int_Callback(sadc_isr_handler_t sadc_int_callback)
{
    sadc_reg_handler = sadc_int_callback;

    return;
}

void Sadc_Register_Adcomp_Int_Callback(sadc_isr_handler_t sadc_comp_int_callback)
{
    sadc_adcomp_handler = sadc_comp_int_callback;

    return;
}

void Sadc_Register_Txcomp_Int_Callback(sadc_isr_handler_t sadc_txcomp_int_callback)
{
    sadc_txcomp_handler = sadc_txcomp_int_callback;

    return;
}

void Sadc_Int_Enable(uint32_t int_mask)
{
    SADC->SADC_INT_CLEAR.reg = SADC_INT_CLEAR_ALL;
    SADC->SADC_INT_MASK.reg = int_mask;
    NVIC_EnableIRQ((IRQn_Type)(Sadc_IRQn));
    return;
}

void Sadc_Int_Disable(void)
{
    NVIC_DisableIRQ((IRQn_Type)(Sadc_IRQn));
    SADC->SADC_INT_MASK.reg = SADC_INT_DISABLE_ALL;
    SADC->SADC_INT_CLEAR.reg = SADC_INT_CLEAR_ALL;
    return;
}

void Sadc_Channel_Enable(sadc_channel_config_t *config_channel)
{
    volatile sadc_pnsel_ch_t *sadc_pnsel_ch;
    volatile sadc_set_ch_t *sadc_set_ch;
    volatile sadc_thd_ch_t *sadc_thd_ch;
    //uint32_t gpio_pull_ctrl_offset = 0;
    uint32_t gpio_pull_ctrl_bit = 0;

    sadc_pnsel_ch = &(SADC->SADC_PNSEL_CH0) + (config_channel->ch_sel * SADC_CH_REG_OFFSET);
    sadc_set_ch = &(SADC->SADC_SET_CH0) + (config_channel->ch_sel * SADC_CH_REG_OFFSET);
    sadc_thd_ch = &(SADC->SADC_THD_CH0) + (config_channel->ch_sel * SADC_CH_REG_OFFSET);

    /*
    if (config_channel->pi_sel == config_channel->ni_sel)
    {
        return;
    }
    */

    /*
    SADC->SADC_SET1.bit.CFG_SADC_TST_MANUAL = ENABLE;
    PMU->PMU_COMP0.bit.AUX_TEST_MODE = ENABLE;
    SYSCTRL->GPIO_PULL_CTRL[2] &= (~(0x00000007 << 24));    //no pull
    SYSCTRL->GPIO_PULL_CTRL[2] &= (~(0x00000007 << 28));    //no pull
    */

    /*
    if (config_channel->pi_sel < SADC_AIN_DISABLED)
    {
        gpio_pull_ctrl_offset = (config_channel->pi_sel >> 3);
        gpio_pull_ctrl_bit = (config_channel->pi_sel & 0x07) * 4;
        SYSCTRL->GPIO_PULL_CTRL[gpio_pull_ctrl_offset] &= (~(0x00000007 << gpio_pull_ctrl_bit));    //no pull

        SYSCTRL->GPIO_AIO_CTRL |= (0x00000001 << config_channel->pi_sel);    //enable AIO
    }

    if (config_channel->ni_sel < SADC_AIN_DISABLED)
    {
        gpio_pull_ctrl_offset = (config_channel->ni_sel >> 3);
        gpio_pull_ctrl_bit = (config_channel->ni_sel & 0x07) * 4;
        SYSCTRL->GPIO_PULL_CTRL[gpio_pull_ctrl_offset] &= (~(0x00000007 << gpio_pull_ctrl_bit));    //no pull

        SYSCTRL->GPIO_AIO_CTRL |= (0x00000001 << config_channel->ni_sel);    //enable AIO
    }
    */

    if (config_channel->pi_sel < SADC_AIN_8)
    {
        gpio_pull_ctrl_bit = config_channel->pi_sel * 4;
        SYSCTRL->GPIO_PULL_CTRL[3] &= (~(0x00000007 << gpio_pull_ctrl_bit));    //no pull

        SYSCTRL->GPIO_AIO_CTRL |= (0x00000001 << config_channel->pi_sel);    //enable AIO
    }

    if (config_channel->ni_sel < SADC_AIN_8)
    {
        gpio_pull_ctrl_bit = config_channel->ni_sel * 4;
        SYSCTRL->GPIO_PULL_CTRL[3] &= (~(0x00000007 << gpio_pull_ctrl_bit));    //no pull

        SYSCTRL->GPIO_AIO_CTRL |= (0x00000001 << config_channel->ni_sel);    //enable AIO
    }

    /*
    if ((config_channel->pi_sel == SADC_VBAT) || (config_channel->ni_sel == SADC_VBAT))
    {
        SADC->SADC_ANA_SET1.bit.CFG_AUX_VLDO = 1;
    }
    */

    sadc_pnsel_ch->bit.CFG_SADC_PSEL_CH = config_channel->pi_sel;
    sadc_pnsel_ch->bit.CFG_SADC_NSEL_CH = config_channel->ni_sel;
    sadc_pnsel_ch->bit.CFG_SADC_GAIN_CH = config_channel->gain;

    if ((config_channel->pi_sel == SADC_VBAT) && (config_channel->ni_sel == SADC_VBAT))
    {
        //N channel = P channel = Vbat, VCM ADC
        sadc_pnsel_ch->bit.CFG_SADC_REF_IN_CH = 3;                                           //N/P channel = adc_ldo
    }
    else if ((config_channel->pi_sel == SADC_TEMPERATURE) && (config_channel->ni_sel == SADC_TEMPERATURE))
    {
        //N channel = P channel = Temperature Sensor
        sadc_pnsel_ch->bit.CFG_SADC_REF_IN_CH = 0;                                           //adc ref not from ADC_LDO output
        Sadc_Temperature_Enable();
    }
    else
    {
        sadc_pnsel_ch->bit.CFG_SADC_REF_IN_CH = 1;                                           //adc ref from ADC_LDO output
    }

    sadc_pnsel_ch->bit.CFG_SADC_PULL_CH = config_channel->pull;
    sadc_pnsel_ch->bit.CFG_SADC_TACQ_CH = config_channel->tacq;
    sadc_pnsel_ch->bit.CFG_SADC_EDLY_CH = config_channel->edly;

    sadc_set_ch->bit.CFG_SADC_BURST_CH = config_channel->burst;

    sadc_thd_ch->bit.CFG_SADC_LTHD_CH = config_channel->low_thd;
    sadc_thd_ch->bit.CFG_SADC_HTHD_CH = config_channel->high_thd;

    return;
}


void Sadc_Channel_Disable(sadc_config_channel_t ch_sel)
{
    volatile sadc_pnsel_ch_t *sadc_pnsel_ch;
    volatile sadc_set_ch_t *sadc_set_ch;
    volatile sadc_thd_ch_t *sadc_thd_ch;

    sadc_pnsel_ch = &(SADC->SADC_PNSEL_CH0) + (ch_sel * SADC_CH_REG_OFFSET);
    sadc_set_ch = &(SADC->SADC_SET_CH0) + (ch_sel * SADC_CH_REG_OFFSET);
    sadc_thd_ch = &(SADC->SADC_THD_CH0) + (ch_sel * SADC_CH_REG_OFFSET);

    sadc_pnsel_ch->reg = SADC_PNSEL_CH_REG_DEFAULT;
    sadc_set_ch->reg = SADC_SET_CH_REG_DEFAULT;
    sadc_thd_ch->reg = SADC_THD_CH_REG_DEFAULT;

    return;
}


void Sadc_Xdma_Config(uint32_t xdma_start_addr,
                      uint16_t xdma_seg_size,
                      uint16_t xdma_blk_size)
{
    /*Reset XDMA*/
    SADC->SADC_WDMA_CTL1.bit.CFG_SADC_WDMA_CTL1 = ENABLE;

    /*Clear XDMA IRQ*/
    SADC->SADC_INT_CLEAR.bit.WDMA = ENABLE;
    SADC->SADC_INT_CLEAR.bit.WDMA_ERROR = ENABLE;

    /*Enable XDMA IRQ*/
    /*
    SADC->SADC_INT_MASK.bit.WDMA = 0;
    SADC->SADC_INT_MASK.bit.WDMA_ERROR = 0;
    */

    /*Set XDMA buffer address*/
    SADC->SADC_WDMA_SET1 = xdma_start_addr;

    /*Set XDMA buffer size*/
    SADC->SADC_WDMA_SET0.bit.CFG_SADC_SEG_SIZE = xdma_seg_size;
    SADC->SADC_WDMA_SET0.bit.CFG_SADC_BLK_SIZE = xdma_blk_size;

    /*Start XDMA for memory access*/
    SADC_SET_XDMA_START();

    return;
}

/*
 * @ingroup sadc_group
 * @brief Sadc Analog Initial configuration
 * @return None
 */
void Sadc_Analog_Init(void)
{
    SADC->SADC_ANA_SET0.bit.CFG_AUX_ANA_SET0 = 0x7F708;

    SADC->SADC_ANA_SET1.bit.CFG_AUX_CMSEL = 0;
    SADC->SADC_ANA_SET1.bit.CFG_AUX_CMSEL = 5;
    SADC->SADC_ANA_SET1.bit.CFG_AUX_CMSEL = 0;
    SADC->SADC_ANA_SET1.bit.CFG_AUX_CMSEL = 1;

    SADC->SADC_ANA_SET1.bit.CFG_AUX_COMP = 3;
    SADC->SADC_ANA_SET1.bit.CFG_AUX_ADC_OUTPUTSTB = 0;
    SADC->SADC_ANA_SET1.bit.CFG_AUX_TEST_MODE = 0;
    SADC->SADC_ANA_SET1.bit.CFG_AUX_VLDO = 3;
    SADC->SADC_ANA_SET1.bit.CFG_AUX_CLK_SEL = 0;

    SADC->SADC_ANA_SET1.bit.CFG_AUX_PW = 0;
    SADC->SADC_ANA_SET1.bit.CFG_AUX_PW = 36;
    SADC->SADC_ANA_SET1.bit.CFG_AUX_PW = 0;
    SADC->SADC_ANA_SET1.bit.CFG_AUX_PW = 36;

    SADC->SADC_ANA_SET1.bit.CFG_EN_CLKAUX = ENABLE;
}

void Sadc_Temperature_Enable(void)
{
    PMU->PMU_TS.bit.TS_VX = 4;
    PMU->PMU_TS.bit.TS_S = 5;
    PMU->PMU_TS.bit.TS_EN = ENABLE;
    Delay_ms(1);
    PMU->PMU_TS.bit.TS_CLK_SEL = 1;
    PMU->PMU_TS.bit.TS_CLK_EN = ENABLE;
    Delay_ms(1);
    PMU->PMU_TS.bit.TS_RST = ENABLE;
    Delay_ms(1);
    PMU->PMU_TS.bit.TS_RST = DISABLE;
    Delay_ms(5);
}

void Sadc_Temperature_Disable(void)
{
    PMU->PMU_TS.bit.TS_EN = DISABLE;
    PMU->PMU_TS.bit.TS_CLK_EN = DISABLE;
}

uint32_t Sadc_Init(sadc_config_t *p_config, sadc_isr_handler_t sadc_int_callback)
{
    if (p_config == NULL)
    {
        return STATUS_INVALID_PARAM;
    }

    sadc_xdma_single_mode = DISABLE;
    sadc_convert_state = SADC_CONVERT_IDLE;

    SADC_RESET();                                       /*Reset SADC*/

    Sadc_Analog_Init();

    SADC_RES_BIT(p_config->sadc_resolution);            /*Set SADC resolution bit*/

    SADC_OVERSAMPLE_RATE(p_config->sadc_oversample);    /*Set SADC oversample rate*/

    if (p_config->sadc_xdma.enable == ENABLE)
    {
        Sadc_Xdma_Config(p_config->sadc_xdma.start_addr, p_config->sadc_xdma.seg_size, p_config->sadc_xdma.blk_size);

        if (p_config->sadc_xdma.blk_size == 0)
        {
            sadc_xdma_single_mode = ENABLE;
        }
    }

    if (sadc_int_callback != NULL)
    {
        Sadc_Register_Int_Callback(sadc_int_callback);
    }
    Sadc_Int_Enable(p_config->sadc_int_mask.reg);

    SADC_SAMPLE_MODE(p_config->sadc_sample_mode);                    /*Sample rate depends on timer rate*/
    if (p_config->sadc_sample_mode == SADC_SAMPLE_TIMER)
    {
        SADC_TIMER_CLOCK(p_config->sadc_timer.timer_clk_src);        /*Timer clock source = system clock*/
        SADC_TIMER_CLOCK_DIV(p_config->sadc_timer.timer_clk_div);    /*Timer rate configuration*/
    }

    SADC_MANUAL_MODE();                                 /*Set the SADC to manual mode*/
    SADC_VGA_ENABLE();                                  /*Enable the SADC VGA*/
    SADC_LDO_ENABLE();                                  /*Enable the SADC LDO*/

    Delay_ms(1);                                        /*Delay 1ms for VGA settle*/

#if (SADC_TEST_MODE == 1)
    SADC_TEST_ENABLE();
    SADC_TEST_ADJUST_VALUE(SADC_TEST_VALUE);
#elif (SADC_CALIBRATION_VALUE != 0)
    SADC_TEST_ADJUST_VALUE((uint32_t)SADC_CALIBRATION_VALUE);
#endif

    return STATUS_SUCCESS;
}

void Sadc_Disable(void)
{
    SADC_DISABLE();       /*Disable SADC*/
    SADC_LDO_DISABLE();   /*Disable the SADC LDO*/
    SADC_VGA_DISABLE();   /*Disable the SADC VGA*/

    Sadc_Int_Disable();
    Sadc_Register_Int_Callback(NULL);

    return;
}

void Sadc_Enable(void)
{
    SADC_ENABLE();       /*Enable SADC*/

    return;
}

void Sadc_Start(void)
{
    if (sadc_xdma_single_mode == ENABLE)
    {
        SADC_SET_XDMA_START();
    }

    sadc_convert_state = SADC_CONVERT_START;

    SADC_START();        /*Start to trigger SADC*/

    return;
}

uint32_t Sadc_Resolution_Compensation(sadc_value_t *p_data)
{
    uint32_t compensation_bit = 0;

    if (p_data == NULL)
    {
        return STATUS_INVALID_PARAM;
    }

    switch (SADC_GET_RES_BIT())
    {
    case SADC_RES_8BIT:
        compensation_bit = 6;
        break;

    case SADC_RES_10BIT:
        compensation_bit = 4;
        break;

    case SADC_RES_12BIT:
        compensation_bit = 2;
        break;

    case SADC_RES_14BIT:
        break;

    default:
        break;
    }

    (*p_data) >>= compensation_bit;

    return STATUS_SUCCESS;
}

sadc_convert_state_t Sadc_Convert_State_Get(void)
{
    return sadc_convert_state;
}

void Sadc_Calibration_Init(void)
{
    MpCalAdcInit();
}

#if (SADC_NEW_DRV == 1)
uint32_t Sadc_Temperature_Calibration(sadc_value_t adc_val)
{
    int32_t cal_vol = 0;
    int32_t cal_m, cal_v, cal_k;
    int32_t cts = adc_val;

    Sadc_Temperature_Calibration_Offset_Update(adc_val);

    if (sadc_temperature_calibration_offset != 0)
    {

        cal_m = VbatAdc >> 1;

        cal_m = (cal_m * TEMPERATURE_CALIBRATION_RATIO) / (Cvbat - C0vAdc);

        cal_k = 0 - ((cal_m * C0vAdc) / TEMPERATURE_CALIBRATION_RATIO);

        cal_v = (cal_m * cts) / TEMPERATURE_CALIBRATION_RATIO;

        cal_v = (cal_v + cal_k);

        cal_vol = (cal_v * TEMPERATURE_CALIBRATION_CONST) / TEMPERATURE_CALIBRATION_RATIO;

        cal_vol = cal_vol / TEMPERATURE_CALIBRATION_RATIO;

        cal_vol = cal_vol + sadc_temperature_calibration_offset;

    }

    return cal_vol;
}

void Sadc_Temperature_Calibration_Offset_Update(sadc_value_t adc_val)
{
    uint32_t read_status;

    mp_temp_k_t  mp_cal_tempk;

    read_status =    MpCalKtRead(&mp_cal_tempk);

    if (read_status == STATUS_SUCCESS)
    {
        sadc_temperature_calibration_offset = mp_cal_tempk.ktvalue * (-1);
    }
    else
    {
        sadc_temperature_calibration_offset = 0;
    }
}
#endif

uint32_t Sadc_Calibration(sadc_cal_type_t cal_type, sadc_value_t adc_val)
{
    uint32_t read_status;
    mp_cal_adc_t mp_cal_adc;
    int32_t cal_vol = 0;

    if (cal_type == SADC_CALIBRATION_VBAT)
    {
        read_status = MpCalVbatAdcRead(&mp_cal_adc);
    }
    else if (cal_type == SADC_CALIBRATION_AIO)
    {
        read_status = MpCalAioAdcRead(&mp_cal_adc);
    }
    else
    {
        read_status = STATUS_INVALID_PARAM;
    }

    if (read_status == STATUS_SUCCESS)
    {
        cal_vol = (adc_val - mp_cal_adc.adc_1);
        cal_vol *= (mp_cal_adc.voltage_2 - mp_cal_adc.voltage_1);
        cal_vol /= (mp_cal_adc.adc_2 - mp_cal_adc.adc_1);
        cal_vol += mp_cal_adc.voltage_1;
    }

    return cal_vol;
}

void Sadc_Compensation_Offset_Update(sadc_value_t adc_val)
{
    uint32_t read_status;
    mp_cal_vcm_adc_t mp_cal_vcmadc;

    read_status = MpCalVcmAdcRead(&mp_cal_vcmadc);

    if ((read_status == STATUS_SUCCESS) && (mp_cal_vcmadc.enable))
    {
        sadc_compensation_offset = adc_val - mp_cal_vcmadc.adc_1;
    }
    else
    {
        sadc_compensation_offset = 0;
    }

#if (SADC_COMP_DEBUG == 1)
    printf("\nOffset = %d, ADC = %d, VCMADC = %d\n", sadc_compensation_offset, adc_val, mp_cal_vcmadc.adc_1);
#endif
}

sadc_value_t Sadc_Compensation(sadc_value_t adc_val)
{
    sadc_value_t comp_vol = 0;

    comp_vol = adc_val - sadc_compensation_offset;

    return comp_vol;
}

#if (SADC_NEW_DRV == 1)
void Sadc_Config_Enable(sadc_config_resolution_t res, sadc_config_oversample_t os, sadc_isr_handler_t sadc_int_callback)
{
    sadc_config_t p_sadc_config;

    Sadc_Calibration_Init();

    //=== Sadc config backup ===
    sadc_config_res = res;
    sadc_config_os = os;
    sadc_config_int_callback = sadc_int_callback;

    //=== Sadc_Config(&p_sadc_config); start ===
    p_sadc_config.sadc_int_mask.bit.DONE = 1;                         /*Set SADC interrupt mask*/
    p_sadc_config.sadc_int_mask.bit.MODE_DONE = 1;
    p_sadc_config.sadc_int_mask.bit.MONITOR_HIGH = 0x3FF;
    p_sadc_config.sadc_int_mask.bit.MONITOR_LOW = 0x3FF;
    p_sadc_config.sadc_int_mask.bit.VALID = 0;
    p_sadc_config.sadc_int_mask.bit.WDMA = 1;
    p_sadc_config.sadc_int_mask.bit.WDMA_ERROR = 1;

    p_sadc_config.sadc_resolution = res;                              /*Set SADC resolution bit*/

    p_sadc_config.sadc_oversample = os;                               /*Set SADC oversample rate*/

    p_sadc_config.sadc_xdma.enable = DISABLE;
    p_sadc_config.sadc_xdma.start_addr = (uint32_t)&sadc_ch_value;
    p_sadc_config.sadc_xdma.seg_size = 1;
    p_sadc_config.sadc_xdma.blk_size = 0;

    p_sadc_config.sadc_sample_mode = SADC_SAMPLE_START;               /*Sample rate depends on start trigger*/
    //=== Sadc_Config(&p_sadc_config); end ===

    Sadc_Init(&p_sadc_config, sadc_int_callback);

    Sadc_Enable();       /*Enable SADC*/
}

uint32_t Sadc_Channel_Read(sadc_input_ch_t ch)
{
    uint32_t read_status;

    enter_critical_section();

    if (sadc_ch_read_convert_state != SADC_CONVERT_START)
    {
        sadc_ch_read_convert_state = SADC_CONVERT_START;

        leave_critical_section();

        Lpm_Low_Power_Mask(LOW_POWER_MASK_BIT_TASK_ADC);
        sadc_convert_ch = ch;
        Sadc_Config_Enable(sadc_config_res, sadc_config_os, sadc_config_int_callback);
        Sadc_Channel_Enable(&sadc_ch_init[ch]);
        Sadc_Start();        /*Start to trigger SADC*/

        read_status = STATUS_SUCCESS;
    }
    else
    {
        leave_critical_section();

        read_status = STATUS_EBUSY;
    }

    return read_status;
}

#if LPWR_FLASH_PROTECT_ENABLE==1 && LPWR_FLASH_VBAT_PROTECT_ENABLE==1
uint32_t Sadc_Init_Vbat(sadc_config_t *p_config, sadc_isr_handler_t sadc_int_callback)
{
    if (p_config == NULL)
    {
        return STATUS_INVALID_PARAM;
    }

    sadc_xdma_single_mode = DISABLE;
    sadc_convert_state = SADC_CONVERT_IDLE;

    SADC_RESET();                                       /*Reset SADC*/

    Sadc_Analog_Init();

    SADC_RES_BIT(p_config->sadc_resolution);            /*Set SADC resolution bit*/

    SADC_OVERSAMPLE_RATE(p_config->sadc_oversample);    /*Set SADC oversample rate*/

    if (p_config->sadc_xdma.enable == ENABLE)
    {
        Sadc_Xdma_Config(p_config->sadc_xdma.start_addr, p_config->sadc_xdma.seg_size, p_config->sadc_xdma.blk_size);

        if (p_config->sadc_xdma.blk_size == 0)
        {
            sadc_xdma_single_mode = ENABLE;
        }
    }


    if (sadc_int_callback != NULL)
    {
        Sadc_Register_Int_Callback(sadc_int_callback);
    }

    //Sadc_Int_Enable(p_config->sadc_int_mask.reg);
    SADC->SADC_INT_CLEAR.reg = SADC_INT_CLEAR_ALL;
    SADC->SADC_INT_MASK.reg = p_config->sadc_int_mask.reg;

    SADC_SAMPLE_MODE(p_config->sadc_sample_mode);                    /*Sample rate depends on timer rate*/
    if (p_config->sadc_sample_mode == SADC_SAMPLE_TIMER)
    {
        SADC_TIMER_CLOCK(p_config->sadc_timer.timer_clk_src);        /*Timer clock source = system clock*/
        SADC_TIMER_CLOCK_DIV(p_config->sadc_timer.timer_clk_div);    /*Timer rate configuration*/
    }

    SADC_MANUAL_MODE();                                                             /*Set the SADC to manual mode*/
    SADC_VGA_ENABLE();                                                              /*Enable the SADC VGA*/
    SADC_LDO_ENABLE();                                                              /*Enable the SADC LDO*/

    Delay_us(15);                                                           /*Delay 1ms for VGA settle*/

#if (SADC_TEST_MODE == 1)
    SADC_TEST_ENABLE();
    SADC_TEST_ADJUST_VALUE(SADC_TEST_VALUE);
#elif (SADC_CALIBRATION_VALUE != 0)
    SADC_TEST_ADJUST_VALUE((uint32_t)SADC_CALIBRATION_VALUE);
#endif

    return STATUS_SUCCESS;
}


void Sadc_Config_Enable_Vbat(sadc_config_resolution_t res, sadc_config_oversample_t os, sadc_isr_handler_t sadc_int_callback)
{
    sadc_config_t p_sadc_config;

    Sadc_Calibration_Init();

    //=== Sadc config backup ===
    sadc_config_vbat_res = res;
    sadc_config_vbat_os = os;
    sadc_config_int_vbat_callback = sadc_int_callback;

    //=== Sadc_Config(&p_sadc_config); start ===
    p_sadc_config.sadc_int_mask.bit.DONE = 1;                         /*Set SADC interrupt mask*/
    p_sadc_config.sadc_int_mask.bit.MODE_DONE = 1;
    p_sadc_config.sadc_int_mask.bit.MONITOR_HIGH = 0x3FF;
    p_sadc_config.sadc_int_mask.bit.MONITOR_LOW = 0x3FF;
    p_sadc_config.sadc_int_mask.bit.VALID = 0;
    p_sadc_config.sadc_int_mask.bit.WDMA = 1;
    p_sadc_config.sadc_int_mask.bit.WDMA_ERROR = 1;

    p_sadc_config.sadc_resolution = res;                              /*Set SADC resolution bit*/

    p_sadc_config.sadc_oversample = os;                               /*Set SADC oversample rate*/

    p_sadc_config.sadc_xdma.enable = DISABLE;
    p_sadc_config.sadc_xdma.start_addr = (uint32_t)&sadc_ch_value;
    p_sadc_config.sadc_xdma.seg_size = 1;
    p_sadc_config.sadc_xdma.blk_size = 0;

    p_sadc_config.sadc_sample_mode = SADC_SAMPLE_START;               /*Sample rate depends on start trigger*/
    //=== Sadc_Config(&p_sadc_config); end ===

    Sadc_Init_Vbat(&p_sadc_config, sadc_int_callback);

    Sadc_Enable();       /*Enable SADC*/
}

uint32_t Sadc_Channel_Read_Vbat(sadc_input_ch_t ch)
{
    uint32_t read_status;
    sadc_cb_t cb;
    sadc_int_t reg_sadc_int_status;
    sadc_value_t  sadc_value;
    sadc_cal_type_t cal_type;
    enter_critical_section();

    if (sadc_ch_read_convert_state != SADC_CONVERT_START)
    {
        leave_critical_section();
        sadc_ch_read_convert_state = SADC_CONVERT_START;
        Lpm_Low_Power_Mask(LOW_POWER_MASK_BIT_TASK_ADC);
        sadc_convert_ch = ch;
        Sadc_Config_Enable_Vbat(sadc_config_vbat_res, sadc_config_vbat_os, sadc_config_int_vbat_callback);
        Sadc_Channel_Enable(&sadc_ch_init[ch]);
        Sadc_Start();

        while (SADC->SADC_INT_STATUS.reg == 0);

        reg_sadc_int_status.reg = SADC->SADC_INT_STATUS.reg;
        SADC->SADC_INT_CLEAR.reg = reg_sadc_int_status.reg;

        if (reg_sadc_int_status.reg != 0)
        {
            if (reg_sadc_int_status.bit.DONE == 1)
            {
            }
            if (reg_sadc_int_status.bit.VALID == 1)
            {
                if (sadc_convert_ch <= SADC_CH_VBAT)
                {
                    if (sadc_reg_handler != NULL)
                    {
                        cb.type = SADC_CB_SAMPLE;
                        sadc_value = SADC_GET_ADC_VALUE();

                        Sadc_Resolution_Compensation(&sadc_value);
                        cb.raw.conversion_value = sadc_value;

                        sadc_value = Sadc_Compensation(sadc_value);
                        cb.raw.compensation_value = sadc_value;

                        if (sadc_convert_ch <= SADC_CH_AIN7)
                        {
                            cal_type = SADC_CALIBRATION_AIO;
                        }
                        else if (sadc_convert_ch <= SADC_CH_VBAT)
                        {
                            cal_type = SADC_CALIBRATION_VBAT;
                        }
                        sadc_value = Sadc_Calibration(cal_type, sadc_value);
                        cb.raw.calibration_value = sadc_value;

                        if (sadc_convert_ch == SADC_CH_VBAT)
                        {
                            VbatAdc = sadc_value;   /*vbat value for Temperature sensor*/
                        }

                        cb.data.sample.value = sadc_value;
                        cb.data.sample.channel = sadc_convert_ch;

                        sadc_reg_handler(&cb);
                    }
                }
                else if (sadc_convert_ch <= SADC_COMP_TEMPERATURE)
                {
                    if (sadc_txcomp_handler != NULL)
                    {
                        cb.type = SADC_CB_SAMPLE;
                        sadc_value = SADC_GET_ADC_VALUE();

                        if (sadc_convert_ch == SADC_COMP_VBAT)
                        {
                            Sadc_Resolution_Compensation(&sadc_value);
                            cb.raw.conversion_value = sadc_value;

                            Cvbat = sadc_value;     /*cvbat for temperature sensor*/

                            sadc_value = Sadc_Compensation(sadc_value);
                            cb.raw.compensation_value = sadc_value;

                            sadc_value = Sadc_Calibration(SADC_CALIBRATION_VBAT, sadc_value);
                            cb.raw.calibration_value = sadc_value;
                        }
                        else if (sadc_convert_ch == SADC_COMP_TEMPERATURE)
                        {
                            Sadc_Temperature_Disable();

                            Sadc_Resolution_Compensation(&sadc_value);
                            cb.raw.conversion_value = sadc_value;

#if (1)
                            sadc_value = Sadc_Compensation(sadc_value);
#endif

                            cb.raw.compensation_value = sadc_value;

                            sadc_value = Sadc_Temperature_Calibration(sadc_value);

                            cb.raw.calibration_value = sadc_value;
                        }

                        cb.data.sample.value = sadc_value;
                        cb.data.sample.channel = sadc_convert_ch;

                        sadc_txcomp_handler(&cb);
                    }
                }
                else if (sadc_convert_ch <= SADC_COMP_VCM)
                {
                    if (sadc_adcomp_handler != NULL)
                    {
                        cb.type = SADC_CB_SAMPLE;

                        sadc_value = SADC_GET_ADC_VALUE();

                        if (sadc_convert_ch == SADC_COMP_VCM)
                        {
                            //Sadc_Resolution_Compensation(&sadc_value);
                            sadc_value >>= 2;          // fix ADC resolution 12-bit for VCM temperature compensation
                            cb.raw.conversion_value = sadc_value;

                            cb.raw.compensation_value = sadc_value;

                            cb.raw.calibration_value = sadc_value;
                        }

                        cb.data.sample.value = sadc_value;
                        cb.data.sample.channel = sadc_convert_ch;

                        sadc_adcomp_handler(&cb);
                    }
                }
                else if (sadc_convert_ch <= SADC_0VADC) /*0V ADC Value for temperature senor calcuation*/
                {
                    if (sadc_txcomp_handler != NULL)
                    {
                        cb.type = SADC_CB_SAMPLE;
                        sadc_value = SADC_GET_ADC_VALUE();

                        Sadc_Resolution_Compensation(&sadc_value);
                        cb.raw.conversion_value = sadc_value;

                        sadc_value = Sadc_Compensation(sadc_value);
                        cb.raw.compensation_value = sadc_value;

                        /*0V ADC Value for temperature senor calcuation*/
                        C0vAdc = sadc_value;

                        cb.data.sample.value = sadc_value;
                        cb.data.sample.channel = sadc_convert_ch;

                        //sadc_txcomp_handler(&cb);
                    }
                }

                Lpm_Low_Power_Unmask(LOW_POWER_MASK_BIT_TASK_ADC);
                Sadc_Disable();

                sadc_ch_read_convert_state = SADC_CONVERT_DONE;
            }
            if (reg_sadc_int_status.bit.MODE_DONE == 1)
            {
                if (SADC_GET_SAMPLE_MODE() == SADC_SAMPLE_START)
                {
                    sadc_convert_state = SADC_CONVERT_DONE;
                }

                /*
                if(sadc_reg_handler != NULL)
                {
                        cb.type = SADC_CB_DONE;
                        cb.data.done.p_buffer = (sadc_value_t *)(SADC_GET_XDMA_START_ADDRESS());
                        cb.data.done.size = SADC_GET_XDMA_RESULT_NUMBER();
                        sadc_reg_handler(&cb);
                }
                */
            }
            if (reg_sadc_int_status.bit.WDMA == 1)
            {
            }
            if (reg_sadc_int_status.bit.WDMA_ERROR == 1)
            {
            }
        }
        read_status = STATUS_SUCCESS;
    }
    else
    {
        leave_critical_section();

        read_status = STATUS_EBUSY;
    }




    return read_status;
}


#endif

void Sadc_Compensation_Int_Handler(sadc_cb_t *p_cb)
{
    uint32_t             sadc_comp_input;
    sadc_value_t         sadc_comp_value;

    if (p_cb->type == SADC_CB_SAMPLE)
    {
        sadc_comp_input = p_cb->data.sample.channel;
        sadc_comp_value = p_cb->data.sample.value;

        if (sadc_comp_input == SADC_COMP_VCM)
        {
#if (SADC_COMP_DEBUG == 1)
            gpio_pin_toggle(0);
            printf("\nVCM ADC = %d\n", sadc_comp_value);
#endif

            Sadc_Compensation_Offset_Update(sadc_comp_value);
        }
    }
}
#if (SUPPORT_FREERTOS_PORT == 1)
void Sadc_Compensation_Periodic_Callback(TimerHandle_t pxTimer)
{
    /* Optionally do something if the pxTimer parameter is NULL. */
    configASSERT(pxTimer);

    Sadc_Channel_Read(SADC_COMP_VCM);
}
#endif

void Sadc_Compensation_Init(uint32_t xPeriodicTimeInSec)
{
    Sadc_Register_Adcomp_Int_Callback(Sadc_Compensation_Int_Handler);

#if (SUPPORT_FREERTOS_PORT == 1)
    sadc_comp_timer_handle = xTimerCreate
                             ( /* Just a text name, not used by the RTOS kernel. */
                                 "Timer",
                                 /* The timer period in ticks, must be greater than 0. */
                                 pdMS_TO_TICKS(xPeriodicTimeInSec * 1000),
                                 /* The timers will auto-reload themselves when they expire. */
                                 pdTRUE,
                                 /* The ID is used to store a count of the number of times the timer has expired, which is initialised to 0. */
                                 (void *) 0,
                                 /* Each timer calls the same callback when it expires. */
                                 Sadc_Compensation_Periodic_Callback
                             );

    if (sadc_comp_timer_handle == NULL)
    {
        /* The timer was not created. */
    }
    else
    {
        /* Start the timer.  No block time is specified, and
        even if one was it would be ignored because the RTOS
        scheduler has not yet been started. */
        if (xTimerStart(sadc_comp_timer_handle, 0) != pdPASS)
        {
            /* The timer could not be set into the Active state. */
        }
    }
#else
    Sadc_Channel_Read(SADC_COMP_VCM);
#endif
}

void Sadc_Compensation_Deinit(void)
{
#if (SUPPORT_FREERTOS_PORT == 1)
    if (sadc_comp_timer_handle != NULL)
    {
        if (xTimerDelete(sadc_comp_timer_handle, 0) == pdPASS)
        {
            /* The delete command was successfully sent to the timer command queue. */
            sadc_comp_timer_handle = NULL;
        }
    }
#endif

    Sadc_Register_Adcomp_Int_Callback(NULL);
}


#if LPWR_FLASH_PROTECT_ENABLE==1 && LPWR_FLASH_VBAT_PROTECT_ENABLE==1

void Sadc_Compensation_vbat_Init(uint32_t xPeriodicTimeInSec)
{
    Sadc_Register_Adcomp_Int_Callback(Sadc_Compensation_Int_Handler);

#if (SUPPORT_FREERTOS_PORT == 1)
    sadc_comp_timer_handle = xTimerCreate
                             ( /* Just a text name, not used by the RTOS kernel. */
                                 "Timer",
                                 /* The timer period in ticks, must be greater than 0. */
                                 pdMS_TO_TICKS(xPeriodicTimeInSec * 1000),
                                 /* The timers will auto-reload themselves when they expire. */
                                 pdTRUE,
                                 /* The ID is used to store a count of the number of times the timer has expired, which is initialised to 0. */
                                 (void *) 0,
                                 /* Each timer calls the same callback when it expires. */
                                 Sadc_Compensation_Periodic_Callback
                             );

    if (sadc_comp_timer_handle == NULL)
    {
        /* The timer was not created. */
    }
    else
    {
        /* Start the timer.  No block time is specified, and
        even if one was it would be ignored because the RTOS
        scheduler has not yet been started. */
        if (xTimerStart(sadc_comp_timer_handle, 0) != pdPASS)
        {
            /* The timer could not be set into the Active state. */
        }
    }
#else

    Sadc_Channel_Read(SADC_COMP_VCM);

#endif
}

#endif

#endif

/** @} */
