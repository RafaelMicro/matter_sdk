/** @file bsp_uart.c
 * @license
 * @description
 */


#include "cm3_mcu.h"
#include "bsp_uart_drv.h"
#include "stdint.h"

#define MAX_NUMBER_OF_UART        3
#define UART0_BASE_CLK            16

#define UART_ReadLineStatus(pReg)      ((pReg)->LSR)

typedef struct
{

    UART_T          *uart;       /*based hardware address*/
    /*IRQ number*/
    IRQn_Type       irq_num;     // UART IRQ Number
    uint8_t         uart_id;     // uart id, 0 for uart0, 1 for uart1
    uint8_t         reserved;

} uart_handle_t;

static const uart_handle_t  m_uart_handle[3] =
{
    {   /*UART0 instatnce*/
        UART0,
        Uart0_IRQn,
        0,
    },
    {   /*UART1 instatnce*/
        UART1,
        Uart1_IRQn,
        1
    },
    {   /*UART2 instatnce*/
        UART2,
        Uart2_IRQn,
        2,
    }
};

static uart_isr_cb pf_isr_cb[3];

int bsp_uart_drv_init(uint32_t target_id, bsp_uart_config_t *pConfig, uart_isr_cb isr_cb)
{
    int      rval = 0;
    const uart_handle_t  *uart_dev;
    UART_T               *uart;
    uint32_t             cval;

    do
    {
        uart_dev = &m_uart_handle[target_id];
        uart = uart_dev->uart;

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
        cval = pConfig->word_length & 0x03;

        /*set baudrate*/
        uart->LCR = (LCR_DLAB | cval);
        /*set baudrate*/
        uart->DLL = pConfig->baud_rate & 0xFF ;
        uart->DLM = pConfig->baud_rate >> 8;
        uart->LCR = (cval);

        /*set stop bits*/
        if (pConfig->stop_bits == UART_STOPBIT_TWO)
        {
            cval |= LCR_STOP;
        }

        /*set parity*/
        if (pConfig->parity & PARENB)
        {
            cval |= LCR_PARITY;
        }
        if (!(pConfig->parity & PARODD))
        {
            cval |= LCR_EPAR;
        }
        if (pConfig->parity & CMSPAR)
        {
            cval |= LCR_SPAR;
        }

        uart->LCR = cval;

        uart->MCR = 0;         /*Initial default modem control register.*/
        uart->FCR = (FCR_FIFO_EN | FCR_TRIGGER_8);

        uart->IER = UART_IER_RLSI;

        uart->IER |= UART_IER_RDI;
        pf_isr_cb[target_id] = isr_cb;

        NVIC_SetPriority(uart_dev->irq_num, pConfig->irq_priority);
        NVIC_EnableIRQ(uart_dev->irq_num);

    } while (0);

    return rval;
}

int bsp_uart_drv_send(uint32_t target_id, uint8_t *p_data, uint32_t length)
{
    int     rval = 0;
    const uart_handle_t  *uart_dev;
    UART_T               *uart;

    do
    {

        if ((p_data == NULL) || (length == 0) || (length > 4096))
        {
            // Invalid parameters
            return -1;
        }
        uart_dev = &m_uart_handle[target_id];
        uart = uart_dev->uart;

        NVIC_DisableIRQ(m_uart_handle[target_id].irq_num);
        while ( length )
        {
            while ( (UART_ReadLineStatus(uart) & UART_LSR_THRE) == 0 ) {}

            uart->THR = *p_data++;
            length--;
        }
        NVIC_EnableIRQ(m_uart_handle[target_id].irq_num);

    } while (0);
    return rval;
}

int bsp_uart_drv_recv(uint32_t target_id, uint8_t *p_data, uint32_t *pLength)
{
    int     rval = 0;
    const uart_handle_t  *uart_dev;
    UART_T               *uart;

    do
    {
        if ((p_data == NULL) || (pLength == NULL))
        {
            rval = -1;
            break;
        }

        uart_dev = &m_uart_handle[target_id];
        uart = uart_dev->uart;

        *pLength = 0;

        if ( uart->LSR & UART_LSR_DR )
        {
            *p_data = uart->RBR & 0xFF;
            *pLength = 1;
        }
    } while (0);

    return rval;
}

void uart0_handler(void)
{
    //UART_T *uart;

    NVIC_ClearPendingIRQ(Uart0_IRQn);
    pf_isr_cb[0]();
}
void uart1_handler(void)
{
    //UART_T *uart;

    NVIC_ClearPendingIRQ(Uart1_IRQn);
    pf_isr_cb[1]();
}
void uart2_handler(void)
{
    //UART_T *uart;

    NVIC_ClearPendingIRQ(Uart2_IRQn);
    pf_isr_cb[2]();
}
