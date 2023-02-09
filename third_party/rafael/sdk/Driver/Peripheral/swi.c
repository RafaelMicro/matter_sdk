/** @file swi.c
 *
 * @brief Software interrupt driver file.
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
static swi_isr_handler_t   swi_reg_handler[MAX_NUMBER_OF_SWI];

/**************************************************************************************************
 *    LOCAL FUNCTIONS
 *************************************************************************************************/


/**************************************************************************************************
 *    GLOBAL FUNCTIONS
 *************************************************************************************************/


/**
 * @ingroup swi_group
 * @brief Software interrupt handler
 * @details Clear the software interrupt status and handle the software interrupt routine
 * @param None
 * @return None
 */
void Sw_Handler(void)
{
    swi_id_sel_t i;
    uint32_t swi_int_reg;

    swi_int_reg = SWI_INT_STATUS_GET();
    SWI_INT_CLEAR(swi_int_reg);

    if (SWI_INT_STATUS_GET())
    {
        ASSERT();
    }

    if (swi_int_reg)
    {
        for (i = SWI_ID_0; i < MAX_NUMBER_OF_SWI; i++)
        {
            if ((swi_int_reg & (1 << i)) && (swi_reg_handler[i] != NULL))
            {
                swi_reg_handler[i](i);
            }
        }
    }

    return;
}


/**
 * @ingroup swi_group
 * @brief SWI interrupt service routine callback for user application.
 * @param[in] swi_id Software interrupt ID
 * @param[in] swi_int_callback Software interrupt callback handler
 * @return None
 */
static void Swi_Int_Callback_Register(swi_id_sel_t swi_id, swi_isr_handler_t swi_int_callback)
{
    swi_reg_handler[swi_id] = swi_int_callback;

    return;
}


/**
 * @ingroup swi_group
 * @brief Unregister SWI interrupt service routine callback.
 * @param[in] swi_id Software interrupt ID
             \arg SWI_ID_0 ~ SWI_ID_31
 * @return None
 */
static void Swi_Int_Callback_Unregister(swi_id_sel_t swi_id)
{
    swi_reg_handler[swi_id] = NULL;

    return;
}


void Swi_Int_Callback_Clear(void)
{
    swi_id_sel_t i;

    for (i = SWI_ID_0; i < MAX_NUMBER_OF_SWI; i++)
    {
        Swi_Int_Callback_Unregister(i);
    }

    return;
}


void Swi_Int_Enable(swi_id_sel_t swi_id, swi_isr_handler_t swi_int_callback)
{
    uint32_t swi_mask;

    if (swi_id < MAX_NUMBER_OF_SWI)
    {
        Swi_Int_Callback_Register(swi_id, swi_int_callback);

        swi_mask = (1 << swi_id);

        //if (SWI_INT_ENABLE_GET() == NULL) {
        NVIC_EnableIRQ((IRQn_Type)(Soft_IRQn));
        //}

        SWI_INT_CLEAR(swi_mask);
        SWI_INT_ENABLE(swi_mask);
    }

    return;
}


void Swi_Int_Disable(swi_id_sel_t swi_id)
{
    uint32_t swi_mask;

    if (swi_id < MAX_NUMBER_OF_SWI)
    {
        Swi_Int_Callback_Unregister(swi_id);

        swi_mask = (1 << swi_id);

        SWI_INT_DISABLE(swi_mask);
        SWI_INT_CLEAR(swi_mask);

        if (SWI_INT_ENABLE_GET() == NULL)
        {
            NVIC_DisableIRQ((IRQn_Type)(Soft_IRQn));
        }
    }

    return;
}


void Swi_Int_Trigger(swi_id_sel_t swi_id)
{
    uint32_t swi_mask;

    if (swi_id < MAX_NUMBER_OF_SWI)
    {
        swi_mask = (1 << swi_id);

        SWI_INT_STATUS_SET(swi_mask);
    }

    return;
}

