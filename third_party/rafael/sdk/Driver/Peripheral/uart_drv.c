/**************************************************************************//**
 * @file     uart_drv.c
 * @version
 * @brief    uart driver file
 *
 * @copyright
 ******************************************************************************/


#include "cm3_mcu.h"
#include "uart_drv.h"
#include "project_config.h"


#define MAX_NUMBER_OF_UART        3

#define UART0_BASE_CLK            16


// UART flags
#define UART_FLAG_INITIALIZED        (1U << 0)
#define UART_FLAG_POWERED            (1U << 1)
#define UART_FLAG_CONFIGURED         (UART_FLAG_POWERED | UART_FLAG_INITIALIZED)

/*private structure for uart driver used.*/

// UART Transfer Information (Run-Time)
typedef struct _UART_TRANSFER_INFO
{
    uint8_t                *rx_buf;        // Pointer to in data buffer
    uint8_t                *tx_buf;        // Pointer to out data buffer
    uint32_t                rx_num;        // Total number of data to be received
    uint32_t                tx_num;        // Total number of data to be send
    uint32_t                rx_cnt;        // Number of data received
    uint32_t                tx_cnt;        // Number of data sent
} uart_transfer_info_t;

/* Remark: Please don't change uart_rx_status_t structure bit setting
 *
 */
typedef struct
{

    uint8_t resv1       : 3;               // Reserver bits.
    uint8_t rx_overflow : 1;               // Receive data overflow detected (cleared on start of next receive operation)
    uint8_t rx_break    : 1;               // Break detected on receive (cleared on start of next receive operation)
    uint8_t rx_framing_error : 1;          // Framing error detected on receive (cleared on start of next receive operation)
    uint8_t rx_parity_error  : 1;          // Parity error detected on receive (cleared on start of next receive operation)
    uint8_t resv2       : 1;               // Reserver bits.

} uart_rx_status_t;


typedef union
{
    uart_rx_status_t   uart_rx_status;
    uint32_t           init_byte;
} status_byte;

// UART Information (Run-Time)
typedef struct _UART_INFO
{

    uart_event_handler_t     cb_event;      // Event callback

    void                     *p_context;

    uart_transfer_info_t     xfer;          // Transfer information

    status_byte              status;        // UART　status flags

    /* "send_active" and "rx_busy" is single byte to avoid
       "race condition" issue */
    uint8_t                  send_active;   // TX Send active flag
    uint8_t                  rx_busy;       // Receiver busy flag

    uint8_t                  flags;         // UART driver flags

    uint8_t                  resv;

    uint8_t                 cache_buf[4];

    uint8_t                 *next_rx_address;
    uint8_t                 *rx_address;
    uint32_t                remain_length;
    uint32_t                rx_cache_length;


} uart_info_t;


typedef struct
{

    UART_T          *uart;       /*based hardware address*/

    uart_info_t     *info;       // Run-Time Information

    /*IRQ number*/
    IRQn_Type       irq_num;     // UART IRQ Number

    uint8_t         uart_id;     // uart id, 0 for uart0, 1 for uart1

    /*DMA information*/
    uint8_t         dma_tx;      // DMA channel for uart tx, if 0 doesn't use DMA for tx
    uint8_t         dma_rx;      // DMA channel for uart rx, if 0 doesn't use DMA for rx

    uint8_t         user_def_recv;    // user has itself recv function handler..

} uart_handle_t;


static uart_info_t uart0_info = {0};
static uart_info_t uart1_info = {0};
static uart_info_t uart2_info = {0};


static const uart_handle_t  m_uart_handle[3] =
{
    {   /*UART0 instatnce*/
        UART0,
        &uart0_info,
        Uart0_IRQn,
        0,

#if (MODULE_ENABLE(UART0_USER_HANDLE_RECV_RX))
        1,        // 1 for enable RX DMA, 0 for PIO DMA...
        0,        // 0 for PIO RX.
        1         // 1 for using user RECV Rx Handler.

#else
        1,        // 1 for enable TX DMA
        1,        // 1 for enable RX DMA
        0         // Enable RX DMA can not call user user defined RECV handler.
#endif

    },
    {   /*UART1 instatnce*/
        UART1,
        &uart1_info,
        Uart1_IRQn,
        1,
        1,
        1,
        0
    },
    {   /*UART2 instatnce*/
        UART2,
        &uart2_info,
        Uart2_IRQn,
        2,
        1,                   // DMA channel for uart tx, if 0 doesn't use DMA for tx
        1,                   // DMA channel for uart tx, if 0 doesn't use DMA for tx
        0
    }
};

uint32_t uart_init(uint32_t uart_id,
                   uart_config_t const   *p_config,
                   uart_event_handler_t  event_handler)
{
    const uart_handle_t  *uart_dev;
    uart_info_t          *uart_info;
    UART_T               *uart;

    uint32_t             cval;
    uint32_t             temp   __attribute__((unused));

    if ((uart_id >= MAX_NUMBER_OF_UART) || (p_config == NULL))
    {
        return STATUS_INVALID_PARAM;
    }

    uart_info = m_uart_handle[uart_id].info;

    if (uart_info->flags & UART_FLAG_INITIALIZED)
    {
        // Driver is already initialized
        // Please uninit the uart then init if APP wants to resetting
        return STATUS_INVALID_REQUEST;
    }

    uart_dev = &m_uart_handle[uart_id];
    uart = uart_dev->uart;

    // Initialize UART Run-time Resources

    uart_info->cb_event = event_handler;
    uart_info->p_context = p_config->p_context;

    /*Set rx_busy, rx_overflow, rx_break, rx_framing_error,
      and rx_parity_error as 0．　
      It also set tx_send_active as 0 */
    uart_info->status.init_byte = 0;
    uart_info->send_active = 0;
    uart_info->rx_busy = 0;

    /*Notice: We don't set config pin here. */

    /*set uart config*/
    // Clear UART IRQ
    NVIC_ClearPendingIRQ(uart_dev->irq_num);

    // Enable UART peripheral clock
    enable_perclk((UART0_BASE_CLK + uart_dev->uart_id));

    /*clear FIFO, REMAKR: FCR is write-only*/
    uart->FCR = 0;
    uart->FCR = FCR_DEFVAL;     /*reset FIFO*/

    // Disable interrupts
    uart->IER = 0;

    /*bits mode only use two bits.*/
    cval = p_config->databits & 0x03;

    /*set baudrate*/
    uart->LCR = (LCR_DLAB | cval);
    /*set baudrate*/
    uart->DLL = p_config->baudrate & 0xFF ;
    uart->DLM = p_config->baudrate >> 8;
    uart->LCR = (cval);

    /*set stop bits*/
    if (p_config->stopbit == UART_STOPBIT_TWO)
    {
        cval |= LCR_STOP;
    }

    /*set parity*/
    if (p_config->parity & PARENB)
    {
        cval |= LCR_PARITY;
    }
    if (!(p_config->parity & PARODD))
    {
        cval |= LCR_EPAR;
    }
    if (p_config->parity & CMSPAR)
    {
        cval |= LCR_SPAR;
    }

    uart->LCR = cval;

    temp = uart->RBR;     /*read temp to clear Timeout IIR noise... only happen when clear RX FIFO and data income..*/

    uart->MCR = 0;         /*Initial default modem control register.*/

    /*init native DMA architecture setting*/
    uart->xDMA_IER = 0;         /*disable xDMA interrupt*/

    uart->xDMA_RX_ENABLE = xDMA_Stop;
    uart->xDMA_TX_ENABLE = xDMA_Stop;

    uart->xDMA_TX_LEN = 0;
    uart->xDMA_RX_LEN = 0;

    uart->xDMA_INT_STATUS = xDMA_ISR_RX | xDMA_ISR_TX;      /*write 1 to clear*/

    uart->xDMA_IER = xDMA_IER_TX | xDMA_IER_RX ;    /*Enable DMA interrupt here*/


#if MODULE_ENABLE(SUPPORT_UART1_FLOWCNTL)
    if ((uart_id == 1) && (p_config->hwfc))
    {
        /*only uart1 support HW flow control pin*/
        uart->MCR = MCR_ENABLE_CTS;     /*Enable hardware CTS to block transmit data*/
        /*when CTS change, and rx line status error, interrupt event generated */
        uart->IER = UART_IER_RLSI | UART_IER_MSI;
    }
    else
#endif
        /*rx line status error, interrupt event generated*/
        uart->IER = UART_IER_RLSI;

    if (uart_dev->user_def_recv)
    {
        uart->FCR = (FCR_FIFO_EN | FCR_TRIGGER_1);      /*FCR Trigger 1, so RX interrrupt can happen ASAP. */
        uart->IER |= UART_IER_RDI;                            /*default enable RX interrupt for user recv handler mode*/
    }
    else
    {
        uart->FCR = (FCR_FIFO_EN | FCR_TRIGGER_4);
    }


    NVIC_SetPriority(uart_dev->irq_num, p_config->interrupt_priority);
    NVIC_EnableIRQ(uart_dev->irq_num);

    /*this function auto turn-on the uart device power.*/
    uart_info->flags = (UART_FLAG_POWERED | UART_FLAG_INITIALIZED);

    return STATUS_SUCCESS;
}

uint32_t uart_uninit(uint32_t uart_id)
{
    uart_info_t          *uart_info;

    if (uart_id >= MAX_NUMBER_OF_UART)
    {
        return STATUS_INVALID_PARAM;
    }

    /*auto turn off the power*/
    uart_power(uart_id, UART_POWER_OFF);

    uart_info = m_uart_handle[uart_id].info;

    // Reset UART status flags
    uart_info->flags = 0;

    return STATUS_SUCCESS;
}

uint32_t uart_power(uint32_t uart_id, uint32_t enable)
{
    const uart_handle_t  *uart_dev;
    uart_info_t          *uart_info;
    UART_T               *uart;

    if (uart_id >= MAX_NUMBER_OF_UART)
    {
        return STATUS_INVALID_PARAM;
    }

    uart_dev = &m_uart_handle[uart_id];
    uart_info = m_uart_handle[uart_id].info;

    uart = uart_dev->uart;

    if (enable)
    {
        /*power-on*/
        if (uart_info->flags & UART_FLAG_POWERED)
        {
            /*already power-on*/
            return STATUS_SUCCESS;
        }

        // Enable UART peripheral clock
        enable_perclk((UART0_BASE_CLK + uart_dev->uart_id));

        /*clear FIFO, REMAKR: FCR is write-only*/
        uart->FCR = 0;
        uart->FCR = FCR_DEFVAL;     /*reset FIFO*/

        /*Remark: we don't change IER setting here if initial*/

        // Clear driver variables
        uart_info->status.init_byte = 0;
        uart_info->send_active = 0;
        uart_info->rx_busy = 0;

        NVIC_ClearPendingIRQ(uart_dev->irq_num);

        if (uart_info->flags & UART_FLAG_INITIALIZED)
        {
            // Clear pending UART interrupts in NVIC
            NVIC_EnableIRQ(uart_dev->irq_num);
        }

        uart_info->flags |= UART_FLAG_POWERED;

    }
    else
    {
        /*power-off*/

        // Disable UART IRQ,
        NVIC_DisableIRQ(uart_dev->irq_num);

        uart->xDMA_IER = 0;         /*disable xDMA interrupt*/

        uart->xDMA_RX_ENABLE = xDMA_Stop;
        uart->xDMA_TX_ENABLE = xDMA_Stop;

        uart->xDMA_INT_STATUS = xDMA_ISR_RX | xDMA_ISR_TX;      /*write 1 to clear*/

        /*reset uart FIFO*/
        uart->FCR =  0;
        uart->FCR =  FCR_CLEAR_XMIT | FCR_CLEAR_RCVR;

        // Disable UART peripheral clock
        disable_perclk((UART0_BASE_CLK + uart_dev->uart_id));

        // Clear driver variables
        uart_info->status.init_byte = 0;
        uart_info->send_active = 0;
        uart_info->rx_busy = 0;

        uart_info->flags &= ~UART_FLAG_POWERED;

        // Clear pending UART interrupts in NVIC
        NVIC_ClearPendingIRQ(uart_dev->irq_num);
    }

    return STATUS_SUCCESS;
}

/*native uart DMA architecture*/
uint32_t uart_tx(uint32_t uart_id,
                 uint8_t const *p_data,
                 uint32_t length)
{

    const uart_handle_t  *uart_dev;
    uart_info_t          *uart_info;
    UART_T               *uart;
    int32_t              val;

    if (uart_id >= MAX_NUMBER_OF_UART)
    {
        return STATUS_INVALID_PARAM;
    }

    if ((p_data == NULL) || (length == 0) || (length > 65535))
    {
        // Invalid parameters
        return STATUS_INVALID_PARAM;
    }

    uart_dev = &m_uart_handle[uart_id];
    uart_info = uart_dev->info;
    uart = uart_dev->uart;

    if ((uart_info->flags & UART_FLAG_CONFIGURED) != UART_FLAG_CONFIGURED)
    {
        // UART is not configured (mode not selected)
        return STATUS_NO_INIT;
    }

    if (uart_info->send_active != 0)
    {
        // Send is not completed yet
        return STATUS_EBUSY;
    }

    // Set Send active flag
    uart_info->send_active = 1;

    // Save transmit buffer info
    uart_info->xfer.tx_buf = (uint8_t *)p_data;
    uart_info->xfer.tx_num = length;
    uart_info->xfer.tx_cnt = 0;

    if (uart_dev->dma_tx)
    {

        uart->xDMA_TX_ADDR = (uint32_t) p_data;
        uart->xDMA_TX_LEN  = length;
        uart->xDMA_TX_ENABLE = xDMA_Enable;

    }
    else
    {
        /*interrupt mode*/
        // Fill TX FIFO
        uart = uart_dev->uart;

        if (uart->LSR & UART_LSR_THRE)
        {
            val = 16;
            while ((val--) && (uart_info->xfer.tx_cnt != uart_info->xfer.tx_num))
            {
                uart->THR = uart_info->xfer.tx_buf[uart_info->xfer.tx_cnt++];
            }
        }

        /* It is POSSIBLE that send and receive are different task (in multitasking OS)
         * To avoid this situation, we add critical section protect.
         */
        enter_critical_section();
        // Enable transmit holding register empty interrupt
        uart->IER |= UART_IER_THRI;
        leave_critical_section();
    }

    if (uart_info->cb_event == NULL)
    {
        /*For block mode*/
        while (uart_info->send_active)
            ;
    }

    return STATUS_SUCCESS;

}

bool uart_tx_in_progress(uint32_t uart_id)
{
    return ((m_uart_handle[uart_id].info->send_active) ? true : false);
}


bool uart_trx_complete(uint32_t uart_id)
{
    const uart_handle_t  *uart_dev;
    UART_T               *uart;
    bool trx_done = FALSE;

    uart_dev = &m_uart_handle[uart_id];
    uart = uart_dev->uart;

    if (((m_uart_handle[uart_id].info->send_active) == 0) && ((uart->LSR & UART_LSR_TEMT) != 0))
    {
        trx_done = TRUE;
    }

    return trx_done;
}


/*
 * Please ONLY call this function when
 * (uart->LSR & UART_LSR_DR) is true.
 *
 */
uint8_t uart_rx_getbytes(uint32_t uart_id)
{
    const uart_handle_t  *uart_dev;
    UART_T               *uart;

    uart_dev = &m_uart_handle[uart_id];
    uart = uart_dev->uart;
    return  (uart->RBR & 0xFF);
}

/*native DMA architecture*/
uint32_t  uart_rx(uint32_t uart_id,
                  uint8_t  *p_data,
                  uint32_t  length)
{
    const uart_handle_t  *uart_dev;
    uart_info_t          *uart_info;
    UART_T               *uart;


    if (uart_id >= MAX_NUMBER_OF_UART)
    {
        return STATUS_INVALID_PARAM;
    }

    if ((p_data == NULL) || (length == 0U) || (length > 65535))
    {
        // Invalid parameters
        return STATUS_INVALID_PARAM;
    }

    uart_dev = &m_uart_handle[uart_id];
    uart_info = uart_dev->info;
    uart = uart_dev->uart;

    if ((uart_info->flags & UART_FLAG_CONFIGURED) != UART_FLAG_CONFIGURED)
    {
        // UART is not configured (mode not selected)
        return STATUS_NO_INIT;
    }

    // Check if receiver is busy
    if (uart_info->rx_busy == 1)
    {
        return STATUS_EBUSY;
    }

    // Set RX busy flag
    uart_info->rx_busy = 1;

    // Save number of data to be received
    uart_info->xfer.rx_num = length;

    /*  Clear RX statuses, clear break, framing_error
     * overflow and parity_error.. almost
     *  Almost IMPOSSIBLE to happen those error.
     */
    uart_info->status.init_byte = 0;

    // Save receive buffer info
    uart_info->xfer.rx_buf = (uint8_t *)p_data;
    uart_info->xfer.rx_cnt =  0;


    if (uart_dev->dma_rx)
    {
        // DMA mode
        /*check recived  p_data alignment*/
        if ((((uint32_t)p_data) & 3) == 0)
        {
            /*buffer is alignment*/

            if ((length & 3) == 0)
            {
                /*length is 4N, including 4 bytes*/
                uart_info->remain_length = 0;
                uart_info->rx_cache_length = 0;

                /*ignore *rx_address */

                uart->xDMA_RX_ADDR = (uint32_t) p_data;
                uart->xDMA_RX_LEN  = length;

            }
            else
            {
                /*length is not 4N*/
                if (length < 4)
                {
                    /*special case for length is less than  4 bytes*/
                    uart_info->remain_length = 0;
                    uart_info->rx_cache_length = length;
                    uart_info->rx_address = p_data ;

                    /*ignore *next_rx_address */
                    uart->xDMA_RX_ADDR = (uint32_t) (uart_info->cache_buf);
                    uart->xDMA_RX_LEN  = length;

                }
                else
                {
                    uart_info->remain_length = length & 3;
                    uart_info->rx_cache_length = 0;
                    uart_info->next_rx_address = p_data + (length & ~3);
                    uart_info->rx_address = p_data ;        /*this address will be ignored*/

                    uart->xDMA_RX_ADDR = (uint32_t) p_data;
                    uart->xDMA_RX_LEN  = (length & ~3);
                }
            }

        }
        else
        {

            if (length <= 4)
            {

                /*special case for length is less than or equal 4 bytes*/
                uart_info->remain_length = 0;
                uart_info->rx_cache_length = length;
                uart_info->rx_address = p_data ;
                /*ignore *next_rx_address */
                uart->xDMA_RX_ADDR = (uint32_t) (uart_info->cache_buf);
                uart->xDMA_RX_LEN  = length;

            }
            else
            {
                /*address is not 4N alignment and length > 4 bytes*/

                uart_info->rx_cache_length = 4 - (((uint32_t)p_data) & 3);
                uart_info->remain_length = length - uart_info->rx_cache_length;

                uart_info->rx_address = p_data ;
                uart_info->next_rx_address = p_data + uart_info->rx_cache_length ;

                /*ignore *next_rx_address */
                uart->xDMA_RX_ADDR = (uint32_t) (uart_info->cache_buf);
                uart->xDMA_RX_LEN  = uart_info->rx_cache_length;

            }

        }

        uart->xDMA_RX_ENABLE = xDMA_Enable;


    }
    else
    {
        // Enable receive data available interrupt
        /* It is POSSIBLE that send and receive are different task! */
        enter_critical_section();
        uart->IER |= UART_IER_RDI;
        leave_critical_section();
    }

    if (uart_info->cb_event == NULL)
    {
        /*For block mode*/
        while (uart_info->rx_busy)
            ;
    }

    return STATUS_SUCCESS;

}

void uart_rx_abort(uint32_t uart_id)
{
    const uart_handle_t  *uart_dev;
    uart_info_t          *uart_info;
    UART_T               *uart;

    if (uart_id >= MAX_NUMBER_OF_UART)
    {
        return;
    }

    uart_dev = &m_uart_handle[uart_id];
    uart_info = uart_dev->info;

    if ((uart_info->flags & UART_FLAG_CONFIGURED) != UART_FLAG_CONFIGURED)
    {
        // UART is not configured (mode not selected)
        return;
    }

    uart = uart_dev->uart;

    enter_critical_section();
    // Disable receive data available interrupt
    uart->IER &= ~UART_IER_RDI;
    leave_critical_section();

    // If DMA mode - disable DMA channel
    if ((uart_dev->dma_rx) && (uart_info->rx_busy))
    {
        uart->xDMA_RX_ENABLE = xDMA_Stop;
        uart->xDMA_INT_STATUS = xDMA_ISR_RX;
    }

    // Receive FIFO reset
    uart->FCR = FCR_TRIGGER_8 | FCR_CLEAR_RCVR | FCR_FIFO_EN;

    // Clear RX busy status
    uart_info->rx_busy = 0;

}

void uart_tx_abort(uint32_t uart_id)
{
    const uart_handle_t  *uart_dev;
    uart_info_t          *uart_info;
    UART_T               *uart;

    if (uart_id >= MAX_NUMBER_OF_UART)
    {
        return;
    }

    uart_dev = &m_uart_handle[uart_id];
    uart_info = uart_dev->info;

    if ((uart_info->flags & UART_FLAG_CONFIGURED) != UART_FLAG_CONFIGURED)
    {
        // UART is not configured (mode not selected)
        return;
    }

    uart = uart_dev->uart;

    enter_critical_section();
    // Disable transmit holding register empty interrupt
    uart->IER &= ~UART_IER_THRI;

    // If DMA mode - disable DMA channel
    if ((uart_dev->dma_tx) && (uart_info->send_active))
    {

        uart->xDMA_TX_ENABLE = xDMA_Stop;
        uart->xDMA_INT_STATUS = xDMA_ISR_TX;
    }

    // Clear Send active flag
    uart_info->send_active = 0;

    // FCR is write-only.
    // Transmit FIFO reset
    uart->FCR = 0;                              /*diable FIFO */
    uart->FCR = FCR_CLEAR_XMIT ;        /*Reset FIFO*/

    uart->FCR = FCR_TRIGGER_8 | FCR_FIFO_EN;

    leave_critical_section();

    /*if data in tranmist holding register, wait it empty.*/
    while ((uart->LSR & UART_LSR_TEMT) == 0)
        ;

}

bool uart_rx_ready(uint32_t uart_id)
{
    const uart_handle_t  *uart_dev;
    uart_info_t          *uart_info;
    UART_T               *uart;

    if (uart_id >= MAX_NUMBER_OF_UART)
    {
        return false;   /*Invalid Id*/
    }

    uart_dev = &m_uart_handle[uart_id];
    uart_info = uart_dev->info;

    if ((uart_info->flags & UART_FLAG_CONFIGURED) != UART_FLAG_CONFIGURED)
    {
        // UART is not configured (mode not selected)
        return false;
    }

    uart = uart_dev->uart;

    return ((uart->LSR & UART_LSR_DR) ? true : false);

}

__STATIC_INLINE void uart_get_status_warp(uint32_t uart_id, uart_status *state)
{
    uint32_t             temp;

    const uart_handle_t  *uart_dev;
    uart_info_t          *uart_info;
    UART_T               *uart;

    if (uart_id >= MAX_NUMBER_OF_UART)
    {
        temp = 0;      /*Invalid device id*/
    }
    else
    {

        uart_dev = &m_uart_handle[uart_id];
        uart_info = uart_dev->info;
        uart = uart_dev->uart;

        temp = (uint32_t) uart_info->status.init_byte;

        if (!(uart->LSR & UART_LSR_TEMT))
        {
            temp |= BIT0;
        }

        if (uart_info->rx_busy)
        {
            temp |= BIT1;
        }

        /* modem status for CTS, CTS is active low
         * so when CTS is low, data transfer,
         * when CTS is high, data stop to transfer
         * (auto by hardware if flow control enable)
         * this is only available for UART1
         */
        if ((uart_id == 1) && (uart->MSR & BIT4))
        {
            temp |= BIT8;
        }
    }

    *((uint32_t *) state) = temp;

    return;
}

uart_status uart_status_get(uint32_t uart_id)
{
    uart_status          temp;

    /* It does not check uart device power state here.
     * this function should be called in power state
     */
    uart_get_status_warp(uart_id, &temp);

    return temp;
}


uint32_t uart_set_break(uint32_t uart_id, uint32_t state)
{
    const uart_handle_t  *uart_dev;
    uart_info_t          *uart_info;
    UART_T               *uart;

    if (uart_id >= MAX_NUMBER_OF_UART)
    {
        return STATUS_INVALID_PARAM;    /*Invalid device id*/
    }

    uart_dev = &m_uart_handle[uart_id];
    uart_info = uart_dev->info;


    if ((uart_info->flags & UART_FLAG_POWERED) == 0)
    {
        // UART not powered
        return STATUS_INVALID_REQUEST;
    }

    uart = uart_dev->uart;

    if (state)
    {
        /*Set Break*/
        if ((uart_info->send_active != 0) || !(uart->LSR & UART_LSR_TEMT))
        {
            return STATUS_EBUSY;
        }

        uart->LCR |= LCR_SBC;
        uart_info->send_active = 1;

    }
    else
    {
        /*Clear Break*/
        uart->LCR &= ~LCR_SBC;
        // Clear Send active flag
        uart_info->send_active = 0;
    }

    return STATUS_SUCCESS;

}

uint32_t uart_set_modem_status(uint32_t uart_id, uint32_t state)
{
    if (uart_id != 1)
    {
        return STATUS_INVALID_PARAM;    /*Only UART1 support modem RTS*/
    }

    if (( m_uart_handle[1].info->flags & UART_FLAG_POWERED) == 0)
    {
        // UART not powered
        return STATUS_INVALID_REQUEST;
    }

    if (state)
    {
        /*
         * set RTS, nRTS will be low (nRTS is low active)
         * UART other  side can send data to us.
         */
        UART1->MCR |= MCR_RTS;
    }
    else
    {
        /*
         * clear RTS, nRTS will be high (nRTS is low active)
         * UART other side SHOULD STOP to send data.
         */
        UART1->MCR &= ~MCR_RTS;
    }

    return STATUS_SUCCESS;
}

static uint32_t uart_rx_line_int_handler(const uart_handle_t *uart_dev)
{
    uint32_t     lsr, event;

    uart_rx_status_t  *status;

    event = 0;
    lsr   = uart_dev->uart->LSR;

    status = (uart_rx_status_t *) & (uart_dev->info->status);

    // OverRun error
    if (lsr & UART_LSR_OE)
    {
        status->rx_overflow = 1;
        event |= UART_EVENT_RX_OVERFLOW;
    }

    // Parity error
    if (lsr & UART_LSR_PE)
    {
        status->rx_parity_error = 1;
        event |= UART_EVENT_RX_PARITY_ERROR;
    }

    // Break detected
    if (lsr & UART_LSR_BI)
    {
        status->rx_break = 1;
        event |= UART_EVENT_RX_BREAK;
    }

    // Framing error
    if (lsr & UART_LSR_FE)
    {
        status->rx_framing_error = 1;
        event |= UART_EVENT_RX_FRAMING_ERROR;
    }

    return event;
}

/**
 * @ingroup UART_Driver
 * @brief   UART Interrupt handler.
 * @param[in]   uart_dev     Pointer to uart_handle_t
 * @return None
 */
static void uart_irqhandler(const uart_handle_t  *uart_dev)
{
    uint32_t     iir, event, val, event1;
    UART_T       *uart;
    uart_info_t  *uart_info;

    uint32_t    i, length;
    uint8_t      *ptr;


    event = 0;
    uart  = uart_dev->uart;
    iir   = uart->IIR &  IIR_INTID_MSK;   /*Mask IIR FIFO enabled Bits*/
    uart_info = uart_dev->info;

    /*check DMA complete or not, it can not use IIR to check.*/

    if ((uart->xDMA_INT_STATUS & xDMA_ISR_RX) == xDMA_ISR_RX)
    {

        uart->xDMA_INT_STATUS = xDMA_ISR_RX;     /*clear RX interrupt flag*/
        uart->xDMA_RX_ENABLE = xDMA_Stop;        /*stop dma*/

        /*check rx dma status.*/

        length = uart_info->rx_cache_length;

        if (length != 0)
        {
            ptr = uart_info->rx_address;

            for (i = 0; i < length; i++)
            {
                *ptr++ = uart_info->cache_buf[i];
            }

            uart_info->rx_cache_length = 0;
        }

        length = uart_info->remain_length;

        if (length != 0)
        {

            if (length < 4)
            {
                /*special case for length is less than  4 bytes*/
                uart_info->rx_cache_length = length;
                uart_info->remain_length = 0;
                /*update address buf pointer.*/
                uart_info->rx_address = uart_info->next_rx_address  ;
                /*ignore *next_rx_address */
                uart->xDMA_RX_ADDR = (uint32_t) (uart_info->cache_buf);
                uart->xDMA_RX_LEN  = length;

            }
            else
            {

                if ((length & 3) == 0)
                {
                    /*remain length is 4N, including 4 bytes*/
                    uart_info->remain_length = 0 ;
                    uart_info->rx_cache_length = 0;

                    /*ignore *rx_address */

                    uart->xDMA_RX_ADDR = (uint32_t) (uart_info->next_rx_address);
                    uart->xDMA_RX_LEN  = length;

                }
                else
                {
                    /*remain length is not 4N*/
                    uart_info->remain_length = length & 3;
                    uart_info->rx_cache_length = 0;
                    uart_info->rx_address = uart_info->next_rx_address ;
                    uart_info->next_rx_address = uart_info->next_rx_address + (length & ~3);

                    uart->xDMA_RX_ADDR = (uint32_t) (uart_info->rx_address);
                    uart->xDMA_RX_LEN  = (length & ~3);
                }

            }

            uart->xDMA_RX_ENABLE = xDMA_Enable;

        }
        else
        {
            // Clear RX busy flag and set receive transfer complete event
            uart_info->xfer.rx_cnt = uart_info->xfer.rx_num;
            uart_info->rx_busy = 0;
            event |= UART_EVENT_RX_DONE;
        }

    }

    if ((uart->xDMA_INT_STATUS & xDMA_ISR_TX) == xDMA_ISR_TX)
    {
        /*clear TX interrupt*/
        uart->xDMA_INT_STATUS = xDMA_ISR_TX;
        /*stop xDMA for RX*/
        uart->xDMA_TX_ENABLE = xDMA_Stop;

        uart_info->xfer.tx_cnt = uart_info->xfer.tx_num;
        uart_info->send_active = 0;

        // Set send complete event
        event |= UART_EVENT_TX_DONE;
    }

    /*BIT0 of IIR can be tested to determine if there is an interrupt pending*/
    if ((iir & IIR_INTSTATUS) == 0)
    {

        /*priority check according to 16550 Spec.*/

        // Receive line status
        if (iir == IIR_INTID_RLS)
        {
            event |= uart_rx_line_int_handler(uart_dev);
        }

        // only check rx data when no rx-dma
        // Receive data available and Character time-out indicator interrupt

        if (uart_dev->dma_rx == 0)
        {
            if ((iir == IIR_INTID_RDA) || (iir == IIR_INTID_CTI))
            {

                // Get available data from RX FIFO
                while ((uart->LSR & UART_LSR_DR))
                {
                    /*2021/11/22 add: patch for user isr to handle uart input message*/
                    if (uart_dev->user_def_recv)
                    {
                        event1 = UART_EVENT_RX_RECV;          /*specail event notify to user AP ASAP*/
                        uart_info->cb_event(event1, uart_info->p_context);
                    }
                    else
                    {

                        // Read data from RX FIFO into receive buffer
                        uart_info->xfer.rx_buf[uart_info->xfer.rx_cnt++] = uart->RBR;

                        // Check if requested amount of data is received
                        if (uart_info->xfer.rx_cnt == uart_info->xfer.rx_num)
                        {
                            // Disable RDA interrupt,
                            /* we don't enter_critical_section here.
                            because here is in ISR */
                            uart->IER &= ~UART_IER_RDI;

                            // Clear RX busy flag and set receive transfer complete event
                            uart_info->rx_busy = 0;
                            event |= UART_EVENT_RX_DONE;
                            break;
                        }
                    }
                }
            }

            // Character time-out indicator
            if (iir == IIR_INTID_CTI)
            {
                // Signal RX Time-out event, if not all requested data received
                if (uart_info->xfer.rx_cnt != uart_info->xfer.rx_num)
                {
                    event |= UART_EVENT_RX_TIMEOUT;
                }
            }
        }

        // only check tx data when no tx-dma
        // Transmit holding register empty
        if (uart_dev->dma_tx == 0)
        {
            if (iir == IIR_INTID_THRE)
            {
                val = 16;

                while ((val--) && (uart_info->xfer.tx_num != uart_info->xfer.tx_cnt))
                {
                    // Write data to Tx FIFO
                    uart->THR = uart_info->xfer.tx_buf[uart_info->xfer.tx_cnt++];
                }

                // Check if all data is transmitted
                if (uart_info->xfer.tx_num == uart_info->xfer.tx_cnt)
                {
                    // Disable THRE interrupt
                    uart->IER &= ~UART_IER_THRI;

                    // Clear TX busy flag
                    uart_info->send_active = 0;

                    // Set send complete event
                    event |= UART_EVENT_TX_DONE;
                }
            }
        }


#if (MODULE_ENABLE(SUPPORT_UART1_FLOWCNTL))
        // Modem interrupt (UART1 only), and only CTS
        if (uart_dev->uart_id == 1)
        {
            if (iir == IIR_INTID_MS)
            {
                // Save modem status register
                val = uart->MSR;
                // Check CTS state changed
                if (val & UART_MSR_DCTS)
                {
                    event |= UART_EVENT_CTS;
                }
            }
        }
#endif

    }

    if ((uart_info->cb_event != NULL) && (event != 0))
    {
        uart_info->cb_event(event, uart_info->p_context);
    }

}

/*uart interrupt handler */
/**
 * @ingroup UART_Driver
 * @brief   UART0 Interrupt handler.
 * @param[in] None
 * @return None
 */
void uart0_handler(void)
{
    uart_irqhandler(&m_uart_handle[0]);
}
/**
 * @ingroup UART_Driver
 * @brief   UART1 Interrupt handler.
 * @param[in] None
 * @return None
 */
void uart1_handler(void)
{
    uart_irqhandler(&m_uart_handle[1]);
}
/**
 * @ingroup UART_Driver
 * @brief   UART2 Interrupt handler.
 * @param[in] None
 * @return None
 */
void uart2_handler(void)
{
    uart_irqhandler(&m_uart_handle[2]);
}



