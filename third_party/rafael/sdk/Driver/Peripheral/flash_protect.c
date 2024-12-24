/**************************************************************************//**
 * @file     flashctl.c
 * @version
 * @brief
 *
 * @copyright
*****************************************************************************/

#include "project_config.h"
#include "cm3_mcu.h"
#include "retarget.h"
#include "uart_drv.h"

#if LPWR_FLASH_PROTECT_ENABLE==1

#if LPWR_FLASH_VBAT_PROTECT_ENABLE==1

#define VBAT_VALUE                  2500

static sadc_convert_state_t sadc_convert_vbat_status = SADC_CONVERT_IDLE;
static sadc_value_t                     sadc_convert_vbat_value;
uint32_t Sadc_VBAT_value(void);
void Sadc_Int_vbat_Callback_Handler(sadc_cb_t *p_cb);
#endif

#if LPWR_FLASH_CMP_PROTECT_ENABLE==1
volatile uint32_t flash_protect = FALSE;
uint32_t Vbat_Comp_Config(comp_config_t *p_config);
void Vbat_Comp_Int_Callback_Handler(void);
#endif


#if LPWR_FLASH_CMP_PROTECT_ENABLE==1

void Vbat_Comp_Int_Callback_Handler(void)
{
    if (COMP_OUT_GET())
    {
        flash_protect = FALSE;
    }
    else
    {
        flash_protect = TRUE;
    }

    COMP_INT_CLEAR();
}

uint32_t Vbat_Comp_Config(comp_config_t *p_config)
{
    if (p_config == NULL)
    {
        return STATUS_INVALID_PARAM;
    }

    p_config->comp_selref = COMP_CONFIG_SELREF_INTERNAL;                    /*Select the Comparator input N source: BG 0.6V*/
    p_config->comp_ref_sel = COMP_CONFIG_REFSEL_CHANNEL_0;                  /*Select the Comparator N input for comparision: Channel 0*/
    p_config->comp_selinput = COMP_CONFIG_SELINPUT_INTERNAL;                /*Select the Comparator input P source: Internal vdd div*/
    p_config->comp_ch_sel = COMP_CONFIG_CHSEL_CHANNEL_0;                    /*Select the Comparator P input for comparision: Channel 0*/
    p_config->comp_swdiv = COMP_CONFIG_SWDIV_RES;                           /*Switch the Comparator vdd div type: RES div*/
    p_config->comp_v_sel = COMP_CONFIG_VOLTAGE_7;                                           /*Select the Comparator internal vdd div voltage for Power Fail: By MP setting*/
    p_config->comp_int_pol = COMP_CONFIG_INT_POL_BOTH;                      /*Set the Comparator interrupt polarity: Both edge*/
    p_config->comp_ds_wakeup = COMP_CONFIG_DS_WAKEUP_ENABLE;                /*Select the Comparator wakeup in DeepSleep: Enable*/
    p_config->comp_ds_inv = COMP_CONFIG_DS_INVERT_DISABLE;                  /*Select the invert of the Comparator output for waking up from DeepSleep: Disable*/
    p_config->comp_pw = COMP_CONFIG_PW_LARGEST;                             /*Select the Comparator current: Largest*/
    p_config->comp_hys_sel = COMP_CONFIG_HYS_80;                            /*Select the Comparator hys window: 130mV*/

    return STATUS_SUCCESS;
}

void flash_cmp_protect_init(void)
{
    comp_config_t p_comp_config;
    Vbat_Comp_Config(&p_comp_config);
    Comp_Init(&p_comp_config, Vbat_Comp_Int_Callback_Handler);          /*Init Comparator*/
    Comp_Enable();                                                                      /*Enable Comparator*/
}
#endif

#if LPWR_FLASH_VBAT_PROTECT_ENABLE==1

void Sadc_Int_vbat_Callback_Handler(sadc_cb_t *p_cb)
{
    if (p_cb->type == SADC_CB_SAMPLE)
    {
        sadc_convert_vbat_value = p_cb->data.sample.value;
        sadc_convert_vbat_status = SADC_CONVERT_DONE;
    }
}

uint32_t Sadc_VBAT_value()
{
    uint32_t value = 0;

    sadc_convert_vbat_status = SADC_CONVERT_IDLE;

    while (1)
    {
        if (sadc_convert_vbat_status == SADC_CONVERT_DONE)
        {
            if (sadc_convert_vbat_value < VBAT_VALUE)
            {
                sadc_convert_vbat_status = SADC_CONVERT_IDLE;
            }
            else
            {
                break;
            }
        }
        else if (sadc_convert_vbat_status == SADC_CONVERT_IDLE)
        {
            sadc_convert_vbat_status = SADC_CONVERT_START;

            if (Sadc_Channel_Read_Vbat(SADC_CH_VBAT) != STATUS_SUCCESS)
            {
                sadc_convert_vbat_status = SADC_CONVERT_IDLE;
            }
        }
    }

    sadc_convert_vbat_status = SADC_CONVERT_IDLE;

    return value;
}
void flash_vbat_protect_init(void)
{
    /*Enable SADC Read VBAT*/
    Sadc_Config_Enable_Vbat(SADC_RES_12BIT, SADC_OVERSAMPLE_0, Sadc_Int_vbat_Callback_Handler);
    Sadc_Compensation_Init(1);
    Sadc_VBAT_value();
}
#endif


#endif
