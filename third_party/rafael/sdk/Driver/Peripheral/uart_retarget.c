/** @file uart_retarget.c
 *
 * @license
 * @description
 */

#include "stdio.h"

#include "cm3_mcu.h"

#include "project_config.h"

#include "uart_drv.h"
#include "uart_retarget.h"
//=============================================================================
//                  Constant Definition
//=============================================================================

//=============================================================================
//                  Macro Definition
//=============================================================================
#define UART_ReadLineStatus(pReg)      ((pReg)->LSR)

//=============================================================================
//                  Structure Definition
//=============================================================================
typedef struct
{
    UART_T          *uart;       /*based hardware address*/
} uart_handle_t;
//=============================================================================
//                  Global Data Definition
//=============================================================================
static uint32_t sUartId;
static cb_stdin_str_t pf_stdin_str;
//=============================================================================
//                  Private Function Definition
//=============================================================================
static uart_handle_t sUartHandle[3] = { {UART0}, {UART1}, {UART2} };
//=============================================================================
//                  Public Function Definition
//=============================================================================
int uart_retarget_init(uart_retarget_desc_t *pt_retarget_desc)
{
    int ret = 0;
    do
    {
        sUartId = pt_retarget_desc->uart_id;
        pf_stdin_str = pt_retarget_desc->pf_stdin_str;

    } while (0);
    return ret;
}
int retarget_stdout_string(char *str, int length, UART_T *p_csr)
{
    while ( length )
    {
        while ( (UART_ReadLineStatus(p_csr) & UART_LSR_THRE) == 0 );

        p_csr->THR = *str++;
        length--;
    }
    return length;
}
int retarget_stdout_char(int ch, UART_T *p_csr)
{
    while ( (UART_ReadLineStatus(p_csr) & UART_LSR_THRE) == 0 );

    p_csr->THR = ch;

    return ch;
}
int uart_retarget_stdout_string(char *str, int length)
{
    return retarget_stdout_string(str, length, sUartHandle[sUartId].uart);
}
int uart_retarget_stdout_char(int ch)
{
    return retarget_stdout_char(ch, sUartHandle[sUartId].uart);
}
#if defined(__CC_ARM) || defined(__CLANG_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
int fputc(int ch, FILE *p_file)
{
    if (ch == 0x0a)
    {
        uart_retarget_stdout_char(0x0d);
    }
    uart_retarget_stdout_char(ch);
    return ch;
}

int fgetc(FILE *p_file)
{
    char pBuf;

    if (pf_stdin_str)
    {
        pf_stdin_str((char *)&pBuf, 1);
    }

    return pBuf;
}
#elif defined(__GNUC__)

int _write(int fd, char *ptr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        if (ptr[i] == 0x0a)
        {
            uart_retarget_stdout_char(0x0d);
        }
        uart_retarget_stdout_char(ptr[i]);
    }

    return len;
}

int _read(int fd, char *ptr, int len)
{
    char pBuf;

    if (pf_stdin_str)
    {
        pf_stdin_str((char *)&pBuf, 1);
    }

    return 1;
}

#endif
