/**
 * @file util_log.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
* @defgroup LOG Log API Definition
* Define Log definitions, structures, and functions.
* @ingroup Utility
* @{
*/


#ifndef __UTIL_LOG_H__
#define __UTIL_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "util.h"
//=============================================================================
//                  Constant Definition
//=============================================================================
/**
 *  color of text
 */
#define LOG_BLACK               "\033[30m"
#define LOG_RED                 "\033[31m"
#define LOG_GREEN               "\033[32m"
#define LOG_YELLOW              "\033[33m"
#define LOG_BLUE                "\033[34m"
#define LOG_MAGENTA             "\033[35m"
#define LOG_CYAN                "\033[36m"
#define LOG_WHITE               "\033[37m"
#define LOG_DEFAULT             "\033[39m"
#define LOG_RESET               "\033[m"

typedef enum util_log_type
{
    UTIL_LOG_ERR         = 0,
    UTIL_LOG_INFO,
    UTIL_LOG_LEVEL_1,
    UTIL_LOG_LEVEL_2,
    UTIL_LOG_LEVEL_3,
    UTIL_LOG_LEVEL_4,
    UTIL_LOG_PROTOCOL,
    UTIL_LOG_HCI_PCI,
    UTIL_LOG_DEBUG,

    UTIL_LOG_MAX

} util_log_type_t;

/**
 * @brief callback for recording log by user
 *
 */
typedef int (*cb_log_rec_t)(util_log_type_t type, char *pStr, long length);
//=============================================================================
//                  Macro Definition
//=============================================================================
#if 1
#ifdef COLOR_OFF
#define msg(type, str, ...)                 util_log(type, str, ##__VA_ARGS__)
#define info(str, ...)                      util_log(UTIL_LOG_INFO, str, ##__VA_ARGS__)
#define log_color(type, color, str, ...)    util_log(type,str, ##__VA_ARGS__)
#define info_color(color, str, ...)         util_log(UTIL_LOG_INFO,str, ##__VA_ARGS__)
#define err(str, ...)                       util_log(UTIL_LOG_ERR,str, ##__VA_ARGS__)
#else
#define msg(type, str, ...)                 util_log(type, str, ##__VA_ARGS__)
#define info(str, ...)                      util_log(UTIL_LOG_INFO, str, ##__VA_ARGS__)
#define log_color(type, color, str, ...)    util_log(type, color str LOG_RESET, ##__VA_ARGS__)
#define info_color(color, str, ...)         util_log(UTIL_LOG_INFO, color str LOG_RESET, ##__VA_ARGS__)
#define err(str, ...)                       util_log(UTIL_LOG_ERR, LOG_RED str LOG_RESET, ##__VA_ARGS__)
#endif
#else
#define msg(type, str, ...)
#define info(str, ...)
#define log_color(type, color, str, ...)
#define info_color(color, str, ...)
#define err(str, ...)
#endif
//=============================================================================
//                  Structure Definition
//=============================================================================

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
 *  @brief  initialize log system
 *              This function MUST be executed before the other log API
 *  @return
 *      none
 */
void
util_log_init(void);

/**
 *  @brief  register recorder to save log message if necessary
 *
 *  @param [in] cb_recorder
 *  @return
 *      none
 */
void
util_log_register_recorder(
    cb_log_rec_t    cb_recorder);

/**
 *  @brief  log message
 *
 *  @param [in] type    log type, reference enum vtutil_log_type
 *  @param [in] fmt     C string that contains the text to be written to stdout.
 *  @param [in] ...     Depending on the format string
 *  @return
 *      none
 */
void
util_log(
    util_log_type_t   type,
    const char          *fmt,
    ...);

/**
 *  @brief  enable log with type
 *
 *  @param [in] type        log type, reference enum vtutil_log_type
 *  @return
 *      none
 */
void
util_log_on(
    util_log_type_t   type);

/**
 *  @brief  disable log with type
 *
 *  @param [in] type        log type, reference enum vtutil_log_type
 *  @return
 *      none
 */
void
util_log_off(
    util_log_type_t   type);

/**
 *  @brief  show all log types are enable or not
 *
 *  @return
 *      none
 */
void
util_log_status(void);

/**
 *  @brief  log memory data
 *
 *  @param [in] type            log type, reference enum vtutil_log_type
 *  @param [in] prefix          the prefix of log message
 *  @param [in] pAddr           start memory address
 *  @param [in] bytes           byte length
 *  @param [in] has_out_u32le   display with uint32 little endian or byte by byte
 *  @return
 *      none
 *
 */
void
util_log_mem(
    util_log_type_t   type,
    char                *prefix,
    uint8_t             *pAddr,
    int                 bytes,
    unsigned int        has_out_u32le);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
