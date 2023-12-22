/** @file bsp_uart.c
 *
 * @license
 * @description
 */



#include "project_config.h"

#if (MODULE_ENABLE(SUPPORT_MULTITASKING))
#include "FreeRTOS.h"
#include "task.h"
#endif

#include "bsp.h"
#if (MODULE_ENABLE(USE_BSP_UART_DRV))
#include "bsp_uart_drv.h"
#else
#include "uart_drv.h"
#endif
#include "ringbuffer.h"
#include "cm3_mcu.h"
//=============================================================================
//                  Constant Definition
//=============================================================================
#define BSP_UART_PORT_ID                1
#define BSP_UART_BAUD_RATE              UART_BAUDRATE_115200

#define TX_BUFF_SIZE                    512
#define TX_BUFF_MASK                    (TX_BUFF_SIZE -1)
#define STATE_SEND_IDLE                 0
#define STATE_SEND_BUSY                 1

//=============================================================================
//                  Macro Definition
//=============================================================================

//=============================================================================
//                  Structure Definition
//=============================================================================
typedef struct uart_io
{
    volatile uint32_t wr_idx;
    volatile uint32_t rd_idx;

    volatile uint32_t is_flushing;
    uint8_t uart_cache[TX_BUFF_SIZE];
} uart_io_t;
//=============================================================================
//                  Global Data Definition
//==============================================================================
#if (!MODULE_ENABLE(USE_BSP_UART_DRV))
static bsp_event_callback_t m_callback;
static ring_buf_t         sendbuf;
static volatile uint8_t   send_state;
static uint16_t           send_num;

static uint8_t            rx_temp[4];
static uint8_t            gu8_uart_tx_data[TX_BUFF_SIZE];

#endif


static uart_io_t g_uart_rx_io = { .wr_idx = 0, .rd_idx = 0, };
//=============================================================================
//                  Private Function Definition
//=============================================================================

#if (MODULE_ENABLE(USE_BSP_UART_DRV))
static void uart_isr_event_handle(void)
{
    // receive data
    do
    {
        uint32_t    wr_pos = g_uart_rx_io.wr_idx;
        uint32_t    rd_pos = g_uart_rx_io.rd_idx;
        uint32_t    pos = 0;
        uint8_t     value[4] = {0};
        uint32_t    rx_len = sizeof(value);

        bsp_uart_drv_recv(BSP_UART_PORT_ID, value, &rx_len);

        if ( rx_len )
        {
            pos = (wr_pos + 1) % TX_BUFF_SIZE;
            if ( pos == rd_pos )
            {
                break;
            }

            g_uart_rx_io.uart_cache[wr_pos] = value[0];
            g_uart_rx_io.wr_idx                = pos;
        }
    } while (0);

    return;
}
#else
static void uart_isr_event_handle(uint32_t event, void *p_context)
{
    uint32_t   wr_index, rd_index;
    uint32_t  pos;

    if (event & UART_EVENT_TX_DONE)
    {
        sendbuf.rd_idx += send_num;
        send_num = 0;

        rd_index = sendbuf.rd_idx & TX_BUFF_MASK;
        wr_index = sendbuf.wr_idx & TX_BUFF_MASK;

        if (sendbuf.rd_idx == sendbuf.wr_idx)
        {
            send_state = STATE_SEND_IDLE;
        }
        else
        {
            if (rd_index <= wr_index)
            {
                send_num = wr_index - rd_index;
            }
            else
            {
                send_num = TX_BUFF_SIZE - rd_index;
            }
            uart_tx(BSP_UART_PORT_ID, &(sendbuf.ring_buf[rd_index]), (uint32_t) send_num);
        }
    }
    if (event & UART_EVENT_RX_DONE)
    {
        wr_index = g_uart_rx_io.wr_idx;
        rd_index = g_uart_rx_io.rd_idx;
        pos = (wr_index + 1) % TX_BUFF_SIZE;
        if ( pos != rd_index )
        {
            g_uart_rx_io.uart_cache[wr_index] = rx_temp[0];
            g_uart_rx_io.wr_idx = pos;
            if (m_callback != NULL)
            {
                m_callback(BSP_EVENT_UART_RX_DONE);
            }
        }
        uart_rx(BSP_UART_PORT_ID, rx_temp, 1);
    }
    if (event & UART_EVENT_RX_BREAK)
    {
        if (m_callback)
        {
            m_callback(BSP_EVENT_UART_BREAK);
        }
    }
}
#endif
//=============================================================================
//                  Public Function Definition
//=============================================================================
int bsp_uart_init(bsp_event_callback_t callback)
{
    int     rval = 0;
#if (MODULE_ENABLE(USE_BSP_UART_DRV))
    bsp_uart_config_t   debug_console_drv_config;

    do
    {

        /*uart1 pinmux*/
        pin_set_mode(28, MODE_UART);     /*GPIO28 as UART0 RX*/
        pin_set_mode(29, MODE_UART);     /*GPIO29 as UART0 TX*/

        /*init debug console uart0, 8bits 1 stopbit, none parity, no flow control.*/
        debug_console_drv_config.baud_rate = BSP_UART_BAUD_RATE;
        debug_console_drv_config.word_length = UART_DATA_BITS_8;
        debug_console_drv_config.hwfc     = UART_HWFC_DISABLED;
        debug_console_drv_config.parity   = UART_PARITY_NONE;
        debug_console_drv_config.stop_bits  = UART_STOPBIT_ONE;
        debug_console_drv_config.irq_priority = 0x06;

        rval = bsp_uart_drv_init(BSP_UART_PORT_ID, &debug_console_drv_config, uart_isr_event_handle);

        if (rval != 0)
        {
            break;
        }
        //m_callback = callback;
    } while (0);

#else
    uart_config_t debug_console_drv_config;

    do
    {

        /*init ringbuf for TX*/
        sendbuf.ring_buf = gu8_uart_tx_data;
        sendbuf.bufsize_mask = TX_BUFF_SIZE;
        sendbuf.wr_idx = sendbuf.rd_idx = 0;
        send_state = STATE_SEND_IDLE;

        send_num = 0;

        /*uart1 pinmux*/
        pin_set_mode(28, MODE_UART);     /*GPIO28 as UART0 RX*/
        pin_set_mode(29, MODE_UART);     /*GPIO29 as UART0 TX*/

        /*init debug console uart0, 8bits 1 stopbit, none parity, no flow control.*/
        debug_console_drv_config.baudrate = BSP_UART_BAUD_RATE;
        debug_console_drv_config.databits = UART_DATA_BITS_8;
        debug_console_drv_config.hwfc     = UART_HWFC_DISABLED;
        debug_console_drv_config.parity   = UART_PARITY_NONE;
        debug_console_drv_config.stopbit  = UART_STOPBIT_ONE;
        debug_console_drv_config.interrupt_priority = 0x06;

        rval = uart_init(BSP_UART_PORT_ID, &debug_console_drv_config, uart_isr_event_handle);

        if (rval != 0)
        {
            break;
        }

        m_callback = callback;

        uart_rx(BSP_UART_PORT_ID, rx_temp, 1);

    } while (0);
#endif
    return rval;
}
#if (!MODULE_ENABLE(USE_BSP_UART_DRV))
static void __uart_tx(uint8_t ch)
{
    uint16_t   data_in_fifo, free_space_in_fifo;

wait_free_space:
#if (MODULE_ENABLE(SUPPORT_MULTITASKING))
    portENTER_CRITICAL();
#endif
    data_in_fifo = (sendbuf.wr_idx - sendbuf.rd_idx) & TX_BUFF_MASK;
    free_space_in_fifo = TX_BUFF_MASK - data_in_fifo;
    if (free_space_in_fifo)
    {
        sendbuf.ring_buf[(sendbuf.wr_idx & TX_BUFF_MASK)] = (uint8_t) ch;
        sendbuf.wr_idx ++;
    }
    else
    {
#if (MODULE_ENABLE(SUPPORT_MULTITASKING))
        portEXIT_CRITICAL();
#endif
        if (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk)
        {
            return ;
        }
        else
        {
            //vTaskDelay(2);
            goto wait_free_space;
        }
    }

    if (send_state == STATE_SEND_IDLE)
    {
        send_state = STATE_SEND_BUSY;
        send_num = 1;
#if (MODULE_ENABLE(SUPPORT_MULTITASKING))
        portEXIT_CRITICAL();
#endif
        //uart_tx(BSP_UART_PORT_ID, &sendbuf.ring_buf[rd_index], send_num);
    }
    else
    {
#if (MODULE_ENABLE(SUPPORT_MULTITASKING))
        portEXIT_CRITICAL();
#endif
    }
}
#endif

int bsp_uart_stdout_char(int ch)
{
    //__uart_tx(ch);
#if (MODULE_ENABLE(USE_BSP_UART_DRV))
    bsp_uart_drv_send(BSP_UART_PORT_ID, (uint8_t *)&ch, 1);
#else
    __uart_tx(ch);
#endif
    return ch;
}

int bsp_uart_stdout_string(char *str, int length)
{
#if (!MODULE_ENABLE(USE_BSP_UART_DRV))
    for (int i = 0; i < length; i++)
    {
        __uart_tx((uint8_t)str[i]);
    }
    return length;
#else
    return bsp_uart_drv_send(BSP_UART_PORT_ID, (uint8_t *)str, length);
#endif

}

uint32_t bsp_uart_send(uint8_t const *p_data, uint32_t length)
{
#if (MODULE_ENABLE(USE_BSP_UART_DRV))
    return bsp_uart_drv_send(BSP_UART_PORT_ID, (uint8_t *)p_data, length);
#else
    for (int i = 0; i < length; i++)
    {
        __uart_tx((uint8_t)p_data[i]);
    }
    return length;
#endif
}
int bsp_uart_flush(void)
{
    int     rval = 0;

    NVIC_DisableIRQ(Uart1_IRQn);

    g_uart_rx_io.wr_idx = 0;
    g_uart_rx_io.rd_idx = 0;
    g_uart_rx_io.is_flushing = true;

    NVIC_EnableIRQ(Uart1_IRQn);
    return rval;
}

uint32_t bsp_uart_recv(uint8_t *p_data, uint32_t length)
{
    int         byte_cnt = 0;
    uint32_t    rd_pos = g_uart_rx_io.rd_idx;
    uint32_t    wr_pos = g_uart_rx_io.wr_idx;

    g_uart_rx_io.is_flushing = 0;

    do
    {
        while ( 1 )
        {
            if ( g_uart_rx_io.is_flushing )
            {
                wr_pos = rd_pos = byte_cnt = 0;
                g_uart_rx_io.rd_idx = 0;
                g_uart_rx_io.wr_idx = 0;
                break;
            }
            if ( rd_pos == wr_pos )
            {
                break;
            }
            if ( length == byte_cnt )
            {
                break;
            }

            p_data[byte_cnt++] = g_uart_rx_io.uart_cache[rd_pos];

            rd_pos = (rd_pos + 1) % TX_BUFF_SIZE;
        }
        g_uart_rx_io.rd_idx = rd_pos;
    } while (0);
    return byte_cnt;
}

uint8_t bsp_uart_rx_getbytes(uint32_t uart_id)
{
    //return uart_rx_getbytes(uart_id);
    return 0;
}
