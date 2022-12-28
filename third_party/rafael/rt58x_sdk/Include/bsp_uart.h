/**
 * @file bsp_uart.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
* @defgroup BSP_UART Uart handler API Definition
* Define Uart handler definitions, structures, and functions.
* @ingroup BSP
* @{
*/


#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "bsp.h"

/**
 * @brief               Initial the button.
 *
 * @param callback      The function to call when the UART event trigger.
 * @return int          0 will be returned if the initialized was successfully.
 */
int bsp_uart_init(bsp_event_callback_t callback);

/**
 * @brief               Send data to target UART.
 *
 * @param p_data        The sending data buffer.
 * @param length        Data length of sending.
 * @return uint32_t
 */
uint32_t bsp_uart_send(uint8_t const *p_data, uint32_t length);

/**
 * @brief               Receive data form target UART.
 *
 * @param p_data        The receiving data buffer.
 * @param length        The receiving length.
 * @return uint32_t
 */
uint32_t bsp_uart_recv(uint8_t *p_data, uint32_t length);
uint8_t bsp_uart_rx_getbytes(uint32_t uart_id);

/**
 * @brief           Send string to uart by standard I/O of BSP
 *
 * @param str       The sending string of the buffer.
 * @param length    The length of the string.
 * @return int      0: fail
 */
int bsp_uart_stdout_string(char *str, int length);

/**
 * @brief           Send charater to uart by standard I/O of BSP
 *
 * @param ch        The sending charater.
 * @return int      1: ok, 0: fail
 */
int bsp_uart_stdout_char(int ch);


int bsp_uart_flush(void);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
