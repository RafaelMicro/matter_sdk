/**************************************************************************//**
 * @file     DMA.c
 * @version
 * @brief    DMA driver file
 *
 * @copyright
 ******************************************************************************/

#include "cm3_mcu.h"

#include "project_config.h"

#define MAX_NUMBER_OF_DMA       4         /*RT58x has 4 DMA channel*/

const static uint32_t dma_addr[MAX_NUMBER_OF_DMA] =
{
    DMA0_CH0_BASE, DMA0_CH1_BASE, DMA0_CH2_BASE, DMA0_CH3_BASE,
};

static dma_proc_cb_t   dma_usr_isr_cb[MAX_NUMBER_OF_DMA];


void dma_register_isr(uint32_t  dma_channel_id, dma_proc_cb_t app_dma_callback)
{
    DMA_T   *DMA;

    if ((dma_channel_id >= MAX_NUMBER_OF_DMA) || (app_dma_callback == NULL))
    {
        return ;
    }

    DMA = (DMA_T *) dma_addr[dma_channel_id];

    DMA->DMA_INTR_CLEAR = DMA_INT_ALL;    /*clear all interrupt*/
    DMA->DMA_INTR_CTRL = DMA_INT_ALL;     /*should we enable DestERR and SrcErr?*/
    dma_usr_isr_cb[dma_channel_id] = app_dma_callback;      /*remember dma isr callback.*/

    NVIC_EnableIRQ( (IRQn_Type) (Dma0_C0_IRQn + dma_channel_id));

    return;
}

uint32_t dma_config(uint32_t  dma_channel_id, const dma_config_t *mode)
{
    DMA_T   *DMA;

    if ((dma_channel_id >= MAX_NUMBER_OF_DMA) || (mode == NULL))
    {
        return STATUS_INVALID_PARAM;
    }

    DMA = (DMA_T *) dma_addr[dma_channel_id];

    DMA->DMA_CTRL = mode->dma_control_setting;
    DMA->DMA_SOURCE_DESC = mode->dma_src_addr;
    DMA->DMA_DEST_DESC = mode->dma_dest_addr;
    DMA->DMA_Xfer_LEN = mode->dma_xfer_length;
    DMA->DMA_REPEAT_VALUE = mode->dma_repeat_setting;

    DMA->DMA_INTR_CTRL = mode->dma_int_enable;

    return STATUS_SUCCESS;

}

uint32_t dma_enable(uint32_t  dma_channel_id)
{
    DMA_T   *DMA;

    if (dma_channel_id >= MAX_NUMBER_OF_DMA)
    {
        return STATUS_INVALID_PARAM;
    }

    DMA = (DMA_T *) dma_addr[dma_channel_id];

    DMA->DMA_ENABLE |= DMA_ENABLE_BIT;

    return STATUS_SUCCESS;
}

uint32_t dma_set_link_list(
    uint32_t dma_channel_id,
    dma_linklist_entry_t *dma_lli,
    uint32_t dma_int_enable
)
{
    DMA_T   *DMA;

    if ((dma_channel_id >= MAX_NUMBER_OF_DMA) || (dma_lli == NULL))
    {
        return STATUS_INVALID_PARAM;
    }

    DMA = (DMA_T *) dma_addr[dma_channel_id];

    DMA->DMA_SOURCE_DESC = (uint32_t) dma_lli;

    DMA->DMA_CTRL = dma_lli->control_set;

    /*Interrupt enable mode*/
    DMA->DMA_INTR_CTRL = dma_int_enable;

    return STATUS_SUCCESS;
}


/**
 *  dma_linklist_request is the "dma move instruction" array which has entry_number item.
 *  And DMA_link_list is the link_list entry used by dma hardware. It should
 *  have entry_number item space too.
 *
 */
uint32_t create_dma_link_list(
    dma_linklist_request_t  *dma_linklist_request,
    uint32_t                entry_number,
    dma_linklist_entry_t    *DMA_link_list,
    uint32_t                control_mode
)
{
    dma_linklist_entry_t     *cur_DMA_link_list;
    dma_linklist_request_t   *cur_dma_list;
    int                      i;

    if ((dma_linklist_request == NULL) || (DMA_link_list == NULL) || (entry_number == 0))
    {
        return STATUS_INVALID_PARAM;
    }

    cur_DMA_link_list = DMA_link_list;
    cur_dma_list =      dma_linklist_request;

    for (i = 0; i < entry_number; i++)
    {
        cur_DMA_link_list->src_addr  = (uint32_t) cur_dma_list->src_ptr;
        cur_DMA_link_list->dest_addr = (uint32_t) cur_dma_list->dest_ptr;
        cur_DMA_link_list->transfer_length = cur_dma_list->size;
        cur_DMA_link_list->control_set = control_mode;

        /*next linked list element */
        cur_DMA_link_list->next_ptr_addr = (uint32_t) (cur_DMA_link_list + 1);

        cur_DMA_link_list->status = 0x0;
        cur_DMA_link_list->repeat_set = 0x0;
        cur_DMA_link_list->resv = 0x0;         /*set resv as zero */

        /*next dma_linklist_entry_t and dma_list_test_t */
        cur_DMA_link_list++;
        cur_dma_list++;
    }

    /*back to the last dma_link_list_t element*/
    cur_DMA_link_list--;
    /*set the Final element in the linked list */
    cur_DMA_link_list->next_ptr_addr = DMA_CNTL_LINKLIST_END;

    return STATUS_SUCCESS;
}

uint32_t dma_get_status(uint32_t  dma_channel_id, uint32_t *status)
{
    DMA_T   *DMA;

    if ((dma_channel_id >= MAX_NUMBER_OF_DMA) || (status == NULL))
    {
        return STATUS_INVALID_PARAM;
    }

    DMA = (DMA_T *) dma_addr[dma_channel_id];

    *status = (DMA->DMA_STATUS & DMA_INT_ALL);

    return STATUS_SUCCESS;
}

uint32_t dma_get_repeatstatus(uint32_t  dma_channel_id, uint32_t *status)
{
    DMA_T   *DMA;

    if ((dma_channel_id >= MAX_NUMBER_OF_DMA) || (status == NULL))
    {
        return STATUS_INVALID_PARAM;
    }

    DMA = (DMA_T *) dma_addr[dma_channel_id];

    *status = (DMA->DMA_REPEAT_STATUS);

    return STATUS_SUCCESS;
}

uint32_t dma_finish(uint32_t  dma_channel_id)
{
    DMA_T   *DMA;

    if (dma_channel_id >= MAX_NUMBER_OF_DMA)
    {
        return STATUS_INVALID_PARAM;
    }

    DMA = (DMA_T *) dma_addr[dma_channel_id];

    DMA->DMA_INTR_CLEAR = DMA_INT_ALL;  /*clear all interrupt flag*/

    DMA->DMA_ENABLE &= ~DMA_ENABLE_MASK;

    return STATUS_SUCCESS;
}

/*do some basic dma init function.*/
uint32_t dma_init(void)
{
    int i;
    DMA_T   *DMA;

    for (i = 0; i < MAX_NUMBER_OF_DMA; i++)
    {
        DMA = (DMA_T *) dma_addr[i];
        /*Enable dmaClkGate to save some power*/
        DMA->DMA_ENABLE |= DMA_DMACLK_GATE;
    }

    return STATUS_SUCCESS;
}

static void dma_isr_handle(uint32_t dma_channel_id)
{
    DMA_T   *DMA;
    uint32_t   status;

    DMA = (DMA_T *) dma_addr[dma_channel_id];

    status = DMA->DMA_STATUS;
    DMA->DMA_INTR_CLEAR = status;  /*clear all interrupt flag*/

    if (status & DMA_INT_XFER_DONE)
    {
        /* we only stop DMA when all dma finish
         * It is possible that some application enable DMA_INT_LL_ELEMENT_Done
         * It will interrupt for each element finish.
         */
        DMA->DMA_ENABLE &= ~DMA_ENABLE_MASK;
    }

    if (dma_usr_isr_cb[dma_channel_id] != NULL)
    {
        dma_usr_isr_cb[dma_channel_id](dma_channel_id, status);
    }

    return;
}

/*The following is interrupt ISR.*/
void dma0_c0_handler(void)
{
    dma_isr_handle(0);
    return;
}

void dma0_c1_handler(void)
{
    dma_isr_handle(1);
    return;
}

void dma0_c2_handler(void)
{
    dma_isr_handle(2);
    return;
}

void dma0_c3_handler(void)
{
    dma_isr_handle(3);
    return;
}

