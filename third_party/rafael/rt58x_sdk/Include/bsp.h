/**
 * @file bsp.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
* @ingroup BSP
* @{
*/

#ifndef __BSP_H__
#define __BSP_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>


//=============================================================================
//                  Constant Definition
//=============================================================================
#define BSP_INIT_NONE           0
#define BSP_INIT_LEDS           (1 << 0)
#define BSP_INIT_BUTTONS        (1 << 1)
#define BSP_INIT_DEBUG_CONSOLE  (1 << 2)
#define BSP_INIT_UART           (1 << 3)


#define A015  0U
#define A048  1U
#define A047  2U

#ifndef BOARD
#define BOARD   A048
#endif

typedef enum
{
    BSP_EVENT_NOTING = 0,
    BSP_EVENT_UART_TX_DONE,
    BSP_EVENT_UART_RX_RECV,
    BSP_EVENT_UART_RX_DONE,
    BSP_EVENT_UART_BREAK,
    BSP_EVENT_BUTTONS_0,
    BSP_EVENT_BUTTONS_1,
    BSP_EVENT_BUTTONS_2,
    BSP_EVENT_BUTTONS_3,
    BSP_EVENT_BUTTONS_4,
} bsp_event_t;


struct bsp_desc;

typedef int (*cb_stdout_char_t)(int c);
typedef int (*cb_stdout_string_t)(char *pBuf, int length);
typedef int (*cb_stdin_string_t)(char *pBuf, int length);
typedef int (*cb_stdio_flush_t)(void);
typedef void (* bsp_event_callback_t)(bsp_event_t);
//=============================================================================
//                  Macro Definition
//=============================================================================

//=============================================================================
//                  Structure Definition
//=============================================================================
typedef struct bsp_stdio
{
    cb_stdout_char_t      pf_stdout_char;
    cb_stdout_string_t    pf_stdout_string;
    cb_stdin_string_t     pf_stdin_str;
    cb_stdio_flush_t      pf_stdio_flush;
} bsp_stdio_t;

/**
 * @brief the description of BSP
 *
 */
typedef struct bsp_desc
{
    int (*pf_init)(void);

    // standard I/O methods in the BSP
    cb_stdout_char_t      pf_stdout_char;
    cb_stdout_string_t    pf_stdout_string;
    cb_stdin_string_t     pf_stdin_str;
    cb_stdio_flush_t      pf_stdio_flush;

    // private data
    void                  *pPriv_data;
} bsp_desc_t;
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
 *  @brief  initialize board (Non thread-safe)
 *
 *  @param [in] u32_type    Type of peripherals used.
    @param [in] callback    Function to be called when buttor/event detected(in ISR)
 *  @return
 *      0: ok, other: fail
 */
int bsp_init(uint32_t u32_type, bsp_event_callback_t callback);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
