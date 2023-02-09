/** @file comparator.c
 *
 * @brief Comparator driver file.
 *
 */

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "cm3_mcu.h"


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/


/**************************************************************************************************
 *    GLOBAL VARIABLES
 *************************************************************************************************/
static comp_isr_handler_t   comp_reg_handler = NULL;


/**************************************************************************************************
 *    LOCAL FUNCTIONS
 *************************************************************************************************/


/**************************************************************************************************
 *    GLOBAL FUNCTIONS
 *************************************************************************************************/

/**
 * @ingroup comp_group
 * @brief Comparator interrupt handler
 * @details Clear the Comparator interrupt status and handle the Comparator interrupt routine
 * @param None
 * @return None
 */
void Comp_Handler(void)
{
    if (COMP_INT_STATUS_GET())
    {
        COMP_INT_CLEAR();

        if (COMP_INT_STATUS_GET())
        {
            ASSERT();
        }

        if (comp_reg_handler != NULL)
        {
            comp_reg_handler();
        }
    }
    return;
}


void Comp_Register_Int_Callback(comp_isr_handler_t comp_int_callback)
{
    comp_reg_handler = comp_int_callback;

    return;
}


void Comp_Int_Enable(void)
{
    COMP_INT_CLEAR();
    COMP_INT_ENABLE();
    NVIC_EnableIRQ((IRQn_Type)(Comp_IRQn));

    return;
}


void Comp_Int_Disable(void)
{
    NVIC_DisableIRQ((IRQn_Type)(Comp_IRQn));
    COMP_INT_DISABLE();
    COMP_INT_CLEAR();

    return;
}


uint32_t Comp_Init(comp_config_t *p_config, comp_isr_handler_t comp_int_callback)
{
    if (p_config == NULL)
    {
        return STATUS_INVALID_PARAM;
    }

    COMP_SELREF_SEL(p_config->comp_selref);       /*Select the Comparator input N source*/
    COMP_INT_POL(p_config->comp_int_pol);         /*Set the Comparator interrupt polarity*/
    COMP_SELINPUT_SEL(p_config->comp_selinput);   /*Select the Comparator input P source*/
    COMP_V_SEL(p_config->comp_v_sel);             /*Select the Comparator internal vdd div voltage*/
    COMP_REF_SEL(p_config->comp_ref_sel);         /*Select the Comparator N input for comparision*/
    COMP_CH_SEL(p_config->comp_ch_sel);           /*Select the Comparator P input for comparision*/
    COMP_PW_SEL(p_config->comp_pw);               /*Select the Comparator current*/
    COMP_HYS_SEL(p_config->comp_hys_sel);         /*Select the Comparator hys window*/
    COMP_SWDIV_SEL(p_config->comp_swdiv);         /*Switch the Comparator vdd div type*/
    COMP_DS_WAKEUP(p_config->comp_ds_wakeup);     /*Select the Comparator wakeup in DeepSleep*/
    COMP_DS_INV(p_config->comp_ds_inv);           /*Select the invert of the Comparator output for waking up from DeepSleep*/

    if (comp_int_callback != NULL)
    {
        Comp_Register_Int_Callback(comp_int_callback);
    }
    Comp_Int_Enable();

    return STATUS_SUCCESS;
}


void Comp_Enable(void)
{
    COMP_ENABLE();       /*Enable Comparator*/

    return;
}


void Comp_Enable_At_Sleep(void)
{
    COMP_ENABLE_SP();       /*Enable Comparator at Sleep*/

    return;
}


void Comp_Enable_At_DeepSleep(void)
{
    COMP_ENABLE_DS();       /*Enable Comparator at DeepSleep*/

    return;
}

