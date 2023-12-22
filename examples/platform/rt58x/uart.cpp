/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "AppConfig.h"
#include "matter_shell.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "uart.h"
#include <stddef.h>
#include <stdio.h>

#include "uart_retarget.h"
#include "bsp_uart_drv.h"

#include "util_log.h"
#include "util_printf.h"

#define UART_CACHE_SIZE 256
#define UART_CACHE_MASK (UART_CACHE_SIZE - 1)

typedef struct execption_ctxt
{
    uint32_t exp_vect;
    uint32_t exp_addr;

    uint32_t spsr;
    uint32_t cpsr;

    uint32_t r[16];
} exception_ctxt_t;

typedef struct uart_io
{
    volatile uint32_t wr_idx;
    volatile uint32_t rd_idx;

    volatile uint32_t is_flushing;
    uint8_t uart_cache[UART_CACHE_SIZE];
} uart_io_t;

static uart_io_t g_uart_rx_io = {
    .wr_idx = 0,
    .rd_idx = 0,
};

static char bsp_c_g_msg[64] __attribute__((aligned(4))) = {0};

exception_ctxt_t __exi_ctxt;

extern void otSysEventSignalPending(void);

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

        bsp_uart_drv_recv(0, value, &rx_len);

        if (rx_len)
        {
            pos = (wr_pos + 1) % UART_CACHE_SIZE;
            if (pos == rd_pos)
            {
                break;
            }

            g_uart_rx_io.uart_cache[wr_pos] = value[0];
            g_uart_rx_io.wr_idx = pos;
#if ENABLE_CHIP_SHELL
            chip::NotifyShellProcessFromISR();
#endif
            //otSysEventSignalPending();
        }
    } while (0);
    return;
}

void uartConsoleInit(void)
{
    int rval = 0;
    bsp_uart_config_t debug_console_drv_config;
    uart_retarget_desc_t t_retarget_desc;
    do
    {
        /*uart0 pinmux*/
        pin_set_mode(16, MODE_UART); /*GPIO16 as UART0 RX*/
        pin_set_mode(17, MODE_UART); /*GPIO17 as UART0 TX*/

        /*init debug console uart0, 8bits 1 stopbit, none parity, no flow control.*/
        debug_console_drv_config.baud_rate = UART_BAUDRATE_115200;
        debug_console_drv_config.word_length = UART_DATA_BITS_8;
        debug_console_drv_config.hwfc = UART_HWFC_DISABLED;
        debug_console_drv_config.parity = UART_PARITY_NONE;
        debug_console_drv_config.stop_bits = UART_STOPBIT_ONE;
        debug_console_drv_config.irq_priority = 6;

        rval = bsp_uart_drv_init(0, &debug_console_drv_config, uart_isr_event_handle);

        if (rval != 0)
        {
            break;
        }
        t_retarget_desc.uart_id = 0;

        uart_retarget_init(&t_retarget_desc);
    } while (0);

    utility_register_stdout(uart_retarget_stdout_char, uart_retarget_stdout_string);

    util_log_init();
}

int16_t uartConsoleWrite(const char *Buf, uint16_t BufLength)
{
    uart_retarget_stdout_string((char *)Buf, BufLength);
    return (int16_t)BufLength;
}

int16_t uartConsoleRead(char *Buf, uint16_t BufLength)
{

    int16_t byte_cnt = 0;
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
            if (BufLength == byte_cnt)
            {
                break;
            }

            Buf[byte_cnt++] = g_uart_rx_io.uart_cache[rd_pos];

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

                        strcpy(&bsp_c_g_msg[strlen(bsp_c_g_msg)], "0x");
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

void HardFault_Handler(void)
{
    __asm volatile(
        "   tst lr, #4\n"
        "   ite eq\n"
        "   mrseq r0, msp\n" 
        "   mrsne r0, psp\n"
        "   b my_fault_handler_c\n"
        );
}


#ifdef __cplusplus
}
#endif
