/** @file bsp_console.c
 *
 * @license
 * @description
 */

#include "stdint.h"
#include "bsp.h"
#include "cm3_mcu.h"

#include "stdarg.h"
#include "string.h"

#if (MODULE_ENABLE(USE_BSP_UART_DRV))
#include "bsp_uart_drv.h"
#else
#include "uart_drv.h"
#endif

#include "uart_retarget.h"
//=============================================================================
//                  Constant Definition
//=============================================================================
#ifndef DEBUG_CONSOLE_UART_ID
#define UART_ACT_DBG_PORT_ID 0
#else
#define UART_ACT_DBG_PORT_ID DEBUG_CONSOLE_UART_ID
#endif

#ifndef DEBUG_CONSOLE_UART_BAUDRATE
#define UART_ACT_BAUD_RATE UART_BAUDRATE_115200
#else
#define UART_ACT_BAUD_RATE DEBUG_CONSOLE_UART_BAUDRATE
#endif

#ifndef UART0_USER_HANDLE_RECV_RX
#define UART0_RX_PIO_MODE 0
#else
#define UART0_RX_PIO_MODE UART0_USER_HANDLE_RECV_RX
#endif

#define UART_CACHE_SIZE 256
#define UART_CACHE_MASK (UART_CACHE_SIZE - 1)
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
    uint8_t uart_cache[UART_CACHE_SIZE];
} uart_io_t;
//=============================================================================
//                  Global Data Definition
//==============================================================================
static uart_io_t g_uart_rx_io = {
    .wr_idx = 0,
    .rd_idx = 0,
};

#if (UART0_RX_PIO_MODE == 0 && !MODULE_ENABLE(USE_BSP_UART_DRV))
static uint8_t gu8_uart_rx_cache[4];
#endif

#if (!MODULE_ENABLE(USE_BSP_UART_DRV))
static bsp_event_callback_t m_callback = NULL;
#endif

static char bsp_c_g_msg[64] __attribute__((aligned(4))) = {0};
//=============================================================================
//                  Private Function Definition
//=============================================================================
#if (MODULE_ENABLE(USE_BSP_UART_DRV))
void uart_isr_event_handle(void)
{
    // receive data
    do
    {
        uint32_t wr_pos = g_uart_rx_io.wr_idx;
        uint32_t rd_pos = g_uart_rx_io.rd_idx;
        uint32_t pos = 0;
        uint8_t value[4] = {0};
        uint32_t rx_len = sizeof(value);

        bsp_uart_drv_recv(UART_ACT_DBG_PORT_ID, value, &rx_len);

        if (rx_len)
        {
            pos = (wr_pos + 1) % UART_CACHE_SIZE;
            if (pos == rd_pos)
            {
                break;
            }

            g_uart_rx_io.uart_cache[wr_pos] = value[0];
            g_uart_rx_io.wr_idx = pos;
        }
    } while (0);
    return;
}
#else
static void uart_isr_event_handle(uint32_t event, void *p_context)
{
    uint32_t pos;
#if (UART0_RX_PIO_MODE == 1)
    if (event & UART_EVENT_RX_RECV)
    {
        pos = (g_uart_rx_io.wr_idx + 1) % UART_CACHE_SIZE;
        if (pos != g_uart_rx_io.rd_idx)
        {
            g_uart_rx_io.uart_cache[g_uart_rx_io.wr_idx] = uart_rx_getbytes(DEBUG_CONSOLE_UART_ID);
            g_uart_rx_io.wr_idx = pos;
            if (m_callback != NULL)
            {
                m_callback(BSP_EVENT_UART_RX_RECV);
            }
        }
    }
#else
    if (event & UART_EVENT_RX_DONE)
    {
        pos = (g_uart_rx_io.wr_idx + 1) % UART_CACHE_SIZE;
        if (pos != g_uart_rx_io.rd_idx)
        {
            g_uart_rx_io.uart_cache[g_uart_rx_io.wr_idx] = gu8_uart_rx_cache[0];
            g_uart_rx_io.wr_idx = pos;
            if (m_callback != NULL)
            {
                m_callback(BSP_EVENT_UART_RX_DONE);
            }
        }
        uart_rx(UART_ACT_DBG_PORT_ID, gu8_uart_rx_cache, 1);
    }
#endif
    if (event & UART_EVENT_RX_BREAK)
    {
#if (UART0_RX_PIO_MODE == 1)
        uart_rx_getbytes(DEBUG_CONSOLE_UART_ID);
        uart_rx_getbytes(DEBUG_CONSOLE_UART_ID);
#endif
        if (m_callback != NULL)
        {
            m_callback(BSP_EVENT_UART_BREAK);
        }
    }
}
#endif

//=============================================================================
//                  Public Function Definition
//=============================================================================
int bsp_console_init(bsp_event_callback_t callback)
{
    int rval = 0;

#if (MODULE_ENABLE(USE_BSP_UART_DRV))
    bsp_uart_config_t debug_console_drv_config;
    uart_retarget_desc_t t_retarget_desc;
    do
    {
        /*uart0 pinmux*/
        pin_set_mode(16, MODE_UART); /*GPIO16 as UART0 RX*/
        pin_set_mode(17, MODE_UART); /*GPIO17 as UART0 TX*/

        /*init debug console uart0, 8bits 1 stopbit, none parity, no flow control.*/
        debug_console_drv_config.baud_rate = UART_ACT_BAUD_RATE;
        debug_console_drv_config.word_length = UART_DATA_BITS_8;
        debug_console_drv_config.hwfc = UART_HWFC_DISABLED;
        debug_console_drv_config.parity = UART_PARITY_NONE;
        debug_console_drv_config.stop_bits = UART_STOPBIT_ONE;
        debug_console_drv_config.irq_priority = 0x06;

        rval = bsp_uart_drv_init(UART_ACT_DBG_PORT_ID, &debug_console_drv_config, uart_isr_event_handle);

        if (rval != 0)
        {
            break;
        }
        // m_callback = callback;

        t_retarget_desc.uart_id = UART_ACT_DBG_PORT_ID;

        uart_retarget_init(&t_retarget_desc);
    } while (0);

#else

    uart_config_t debug_console_drv_config;
    uart_retarget_desc_t t_retarget_desc;

    do
    {

        /*uart0 pinmux*/
        pin_set_mode(16, MODE_UART); /*GPIO16 as UART0 RX*/
        pin_set_mode(17, MODE_UART); /*GPIO17 as UART0 TX*/

        /*init debug console uart0, 8bits 1 stopbit, none parity, no flow control.*/
        debug_console_drv_config.baudrate = (uart_baudrate_t)UART_ACT_BAUD_RATE;
        debug_console_drv_config.databits = UART_DATA_BITS_8;
        debug_console_drv_config.hwfc = UART_HWFC_DISABLED;
        debug_console_drv_config.parity = UART_PARITY_NONE;
        debug_console_drv_config.stopbit = UART_STOPBIT_ONE;
        debug_console_drv_config.interrupt_priority = 0x06;

        rval = uart_init(UART_ACT_DBG_PORT_ID, &debug_console_drv_config, uart_isr_event_handle);

        if (rval != 0)
        {
            break;
        }

        m_callback = callback;

        t_retarget_desc.uart_id = UART_ACT_DBG_PORT_ID;

        uart_retarget_init(&t_retarget_desc);
#if (UART0_RX_PIO_MODE == 0)
        uart_rx(UART_ACT_DBG_PORT_ID, gu8_uart_rx_cache, 1);
#endif
    } while (0);
#endif
    return rval;
}

int bsp_console_stdio_flush(void)
{
    int rval = 0;

    NVIC_DisableIRQ(Uart0_IRQn);

    g_uart_rx_io.wr_idx = 0;
    g_uart_rx_io.rd_idx = 0;
    g_uart_rx_io.is_flushing = true;

    NVIC_EnableIRQ(Uart0_IRQn);
    return rval;
}

int bsp_console_deinit(void)
{
    return 0;
}

int bsp_console_stdout_char(int ch)
{
    return uart_retarget_stdout_char(ch);
}

int bsp_console_stdout_string(char *str, int length)
{
    int rval = 0;
    uart_retarget_stdout_string(str, length);
    return (rval == 0) ? length : 0;
}

int bsp_console_stdin_str(char *pBuf, int length)
{
    int byte_cnt = 0;
    uint32_t rd_pos = g_uart_rx_io.rd_idx;
    uint32_t wr_pos = g_uart_rx_io.wr_idx;

    g_uart_rx_io.is_flushing = 0;

    do
    {
        while (1)
        {
            if (g_uart_rx_io.is_flushing)
            {
                wr_pos = rd_pos = byte_cnt = 0;
                break;
            }
            if (rd_pos == wr_pos)
            {
                break;
            }
            if (length == byte_cnt)
            {
                break;
            }

            pBuf[byte_cnt++] = g_uart_rx_io.uart_cache[rd_pos];

            rd_pos = (rd_pos + 1) % UART_CACHE_SIZE;
        }
        g_uart_rx_io.rd_idx = rd_pos;
    } while (0);
    return byte_cnt;
}

static void
_uint2strhex(char *pStr, unsigned int number, const char nibbles_to_print)
{
#define MAX_NIBBLES (8)
    int nibble = 0;
    char nibbles = nibbles_to_print;

    if ((unsigned)nibbles > MAX_NIBBLES)
    {
        nibbles = MAX_NIBBLES;
    }

    while (nibbles > 0)
    {
        nibbles--;
        nibble = (int)(number >> (nibbles * 4)) & 0x0F;
        if (nibble <= 9)
        {
            pStr[strlen(pStr)] = (char)('0' + nibble);
        }
        else
        {
            pStr[strlen(pStr)] = (char)('A' - 10 + nibble);
        }
    }
    return;
}
static int
_exp_dump_out(char *pMsg, int len)
{
    UART_T *pCSR = (UART_T *)UART0;

    while (len)
    {
        while ((pCSR->LSR & 0x20) == 0)
        {
        }

        pCSR->THR = *pMsg++;
        len--;
    }
    return 0;
}
static void
_exp_log_out(const char *format, ...)
{
    char *pch = (char *)format;
    va_list va;
    va_start(va, format);

    do
    {
        if (!pch)
        {
            break;
        }

        while (*pch)
        {
            /* format identification character */
            if (*pch == '%')
            {
                pch++;

                if (pch)
                {
                    switch (*pch)
                    {
                    case 'x':
                    {
                        const unsigned int number = va_arg(va, unsigned int);

                        for (int j = 0; j < sizeof(bsp_c_g_msg); j += 4)
                        {
                            *(uint32_t *)&bsp_c_g_msg[j] = 0;
                        }

                        strcpy((void *)&bsp_c_g_msg[strlen(bsp_c_g_msg)], (void *)"0x");
                        _uint2strhex(bsp_c_g_msg, number, 8);
                        _exp_dump_out(bsp_c_g_msg, strlen(bsp_c_g_msg));
                    }
                    break;
                    case 's':
                    {
                        char *string = va_arg(va, char *);
                        _exp_dump_out(string, strlen(string));
                    }
                    break;
                    default:
                        break;
                    }
                }
            }
            else
            {
                _exp_dump_out(pch, 1);
            }

            pch++;
        }
    } while (0);

    va_end(va);
    return;
}

static int
_exp_dump_init(void)
{
    UART_T *pCSR = (UART_T *)UART0;

    pCSR->LCR |= (0x1 << 7);
    pCSR->DLL = UART_BAUDRATE_Baud115200 & 0xFF;
    pCSR->DLM = UART_BAUDRATE_Baud115200 >> 8;
    pCSR->LCR &= ~(0x1 << 7);
    pCSR->LCR = 0x3;
    return 0;
}

void my_fault_handler_c(uint32_t *sp)
{
    _exp_dump_init();
    _exp_log_out(" [HardFaultHandler]\r\n");
    _exp_log_out("R0= %x, R1= %x R2= %x R3= %x\r\n",
                 sp[0], sp[1], sp[2], sp[3]);

    _exp_log_out("R12= %x, LR= %x, PC= %x, PSR= %x\r\n",
                 sp[4], sp[5], sp[6], sp[7]);

    while (1)
        ;
}
#if defined(__CC_ARM) || defined(__CLANG_ARM)
__asm void HardFault_Handler(void)
{
    tst lr, #4 ite eq

                mrseq r0,
        msp
            mrsne r0,
        psp

            b
            __cpp(my_fault_handler_c)
}
#endif

void HardFault_Handler(void)
{
    __asm volatile(
        "stmdb sp!, {lr}\n");
}
