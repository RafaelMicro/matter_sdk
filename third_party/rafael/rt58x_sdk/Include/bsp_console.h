/**
 * @file bsp_console.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
* @defgroup BSP_Console Console handler API Definition
* Define Console handler definitions, structures, and functions.
* @ingroup BSP
* @{
*/

#ifndef __BSP_CONSOLE_H__
#define __BSP_CONSOLE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"


#define __cli_cmd_pool          __attribute__ ((used, section("cli_cmd_pool")))


/**
 * @brief               Initial the console.
 *
 * @param callback      Allway set NULL.
 * @return int          0 will be returned if the initialized was successfully.
 */
int bsp_console_init(bsp_event_callback_t callback);

/**
 * @brief               Get the input data form console by standard I/O of BSP.
 *
 * @param pBuf          The receiving data buffer.
 * @param length        The receiving length.
 *                          It should be assigned receiving buffer length and
 *                          this API will report the real receiving data length
 * @return int          Return the bytes of receiving.
 */
int bsp_console_stdin_str(char *pBuf, int length);

/**
 * @brief               Sending string to console by standard I/O of BSP.
 *
 * @param str           The sending string buffer.
 * @param length        The length of the string.
 * @return int          0:fail
 */
int bsp_console_stdout_string(char *str, int length);

/**
 * @brief               Sending charter to console by standard I/O of BSP.
 *
 * @param ch            The sending charter.
 * @return int          0:fail
 */
int bsp_console_stdout_char(int ch);

/**
 * @brief               Flushing the console
 *
 * @return int          0:Success
 */
int bsp_console_stdio_flush(void);


/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
