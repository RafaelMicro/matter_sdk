/** @file uart_retarget.h
 * @license
 * @description
 */

#ifndef __UART_RETARGET_H___
#define __UART_RETARGET_H___

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "project_config.h"

//=============================================================================
//                  Constant Definition
//=============================================================================

//=============================================================================
//                  Macro Definition
//=============================================================================
typedef int (*cb_stdin_str_t)(char *pBuf, int length);
//=============================================================================
//                  Structure Definition
//=============================================================================
/**
 *  the description of uart retarget
 */
typedef struct retarget_desc
{
    uint32_t uart_id;

    // If you need to retarget getc or read, must assigne the callback
    cb_stdin_str_t pf_stdin_str;
} uart_retarget_desc_t;
//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================

//=============================================================================
//                  Public Function Definition
//=============================================================================
/**
 *  @brief  initialize uart retarget for printf
 *
 *  @param [in] uart_retarget_desc_t     assign the user description of retarget
 *  @return
 *      0: ok, other: fail
 */
int uart_retarget_init(uart_retarget_desc_t *pt_retarget_desc);

/**
 *  @brief  stdout for bsp stdout
 *
 *  @param [in] ch output ch
 *  @return
 *      0: ok, other: fail
 */
int uart_retarget_stdout_char(int ch);

int uart_retarget_stdout_string(char *str, int length);

#ifdef __cplusplus
}
#endif

#endif
