/**
 * @file shell.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
* @defgroup Shell Shell API Definition
* Define Shell definitions, structures, and functions.
* @ingroup Utility
* @{
*
*/

#ifndef __shell_H_waEXYsOz_lBwV_Hy1X_s6yW_u8zxP70lVh3i__
#define __shell_H_waEXYsOz_lBwV_Hy1X_s6yW_u8zxP70lVh3i__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//=============================================================================
//                  Constant Definition
//=============================================================================
/*! \name VT100 Common Commands (VT100 escape codes)
 */

/**
 *  VT100 color of text
 */
#define VT100_COLOR_BLACK             "\033[30m"
#define VT100_COLOR_RED               "\033[31m"
#define VT100_COLOR_GREEN             "\033[32m"
#define VT100_COLOR_YELLOW            "\033[33m"
#define VT100_COLOR_BLUE              "\033[34m"
#define VT100_COLOR_MAGENTA           "\033[35m"
#define VT100_COLOR_CYAN              "\033[36m"
#define VT100_COLOR_WHITE             "\033[37m"
#define VT100_COLOR_DEFAULT           "\033[39m"
#define VT100_COLOR_RESET             "\033[m"

/**
 *  VT100 background color
 */
#define VT100_BG_BLACK                "\033[40m"
#define VT100_BG_RED                  "\033[41m"
#define VT100_BG_GREEN                "\033[42m"
#define VT100_BG_YELLOW               "\033[43m"
#define VT100_BG_BLUE                 "\033[44m"
#define VT100_BG_MAGENTA              "\033[45m"
#define VT100_BG_CYAN                 "\033[46m"
#define VT100_BG_WHITE                "\033[47m"
#define VT100_BG_DEFAULT              "\033[49m"

#define SHELL_PROMPT                  "~# "

#define SH_CMD_REGISTER_PRIORITY_NORMAL     150

typedef int (*cb_shell_out_t)(const char *str, ...);
//=============================================================================
//                  Macro Definition
//=============================================================================
#define SHELL_EXPAND2(a, b)     a ## b
#define SHELL_EXPAND(a, b)      SHELL_EXPAND2(a, b)
#define SHELL_CMD_UID(what)     SHELL_EXPAND(what, __LINE__)

#define sh_cmd_add(cmd_name, desc_, cmd_func, priority)                                     \
    static void __attribute__((constructor(priority))) SHELL_CMD_UID(cmd_reg_helper_)() {   \
        static sh_cmd_t SHELL_CMD_UID(cmd_desc_) = {                                        \
            .pCmd_name    = cmd_name,                                                       \
            .pDescription = desc_,                                                          \
            .cmd_exec     = &cmd_func,                                                      \
        };                                                                                  \
        shell_register_cmd(&SHELL_CMD_UID(cmd_desc_));                                      \
    }


/**
 *  command history buffer usage
 *
 *      +-------------+   ^
 *      | line_size   |   |
 *      +-------------+   |
 *      |             |   | cmd_deep
 *      |  ...        |   |
 *      +-------------+   |
 *      | line_size   |   |
 *      +-------------+   v
 *
 */
#define SHELL_CALC_HISTORY_BUFFER(line_size, cmd_deep)        (((line_size) + 4) * (cmd_deep))
//=============================================================================
//                  Structure Definition
//=============================================================================

/**
 * @brief   The settings description of shell module
 *
 */
typedef struct sh_set
{
    char        *pLine_buf;
    uint32_t    line_buf_len;

    char        *pIO_cache;
    uint32_t    io_cache_len;

    /**
     *  pHistory_buf MUST be 4-bytes alignment
     */
    char        *pHistory_buf;
    uint16_t    line_size;
    uint16_t    cmd_deep;
    uint32_t    history_buf_size;  // assign with SHELL_CALC_HISTORY_BUFFER()

    void        *pUser_data;

} sh_set_t;

/**
 * @brief   The I/O description of shell module
 *
 */
typedef struct sh_io_desc
{
    int     (*cb_init)(sh_set_t *pSet_info);
    int     (*cb_deinit)(void *pInfo);

    int     (*cb_read)(uint8_t *pBuf, uint32_t length, void *pExtra);
    int     (*cb_write)(uint8_t *pBuf, uint32_t length, void *pExtra);

    int     (*cb_flush)(void);
} sh_io_desc_t;

/**
 * @brief   The command description of shell module
 *
 */
typedef struct sh_cmd
{
    struct sh_cmd   *next;

    char        *pCmd_name;
    uint32_t    cmd_name_len;
    char        *pDescription;

    void        *pExtra;

    /**
     *  @brief  cmd_exec
     *              executable program of shell command
     *
     *  @param [in] argc        count of arguments
     *  @param [in] argv        values of arguments
     *  @param [in] out_func    function of shell log output
     *  @param [in] pExtra      extra data from (struct sh_cmd)->pExtra
     *  @return
     *      0: ok, other fail
     */
    int         (*cmd_exec)(int argc, char **argv, cb_shell_out_t out_func, void *pExtra);

} sh_cmd_t;

/**
 * @brief   shell arguments, it MUST has a static instance for shell_proc()
 *
 */
typedef struct sh_args
{
    uint32_t    is_blocking;
    uint32_t    delay_time_ms;

    /**
     *  exec_result is used to report the result of shell commands.
     *  The shell command is not handled if exec_result is SH_EXEC_RVAL_UNKNOWN
     */
#define SH_EXEC_RVAL_UNKNOWN    0xdeadbeef
    uint32_t    exec_result;

    int         (*cb_regular_alarm)(struct sh_args *pArg);

    void        *pTunnel_info;

} sh_args_t;
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
 * @brief Initial the shell module
 *
 * @param pDesc  Pointer of the I/O description.
 * @param pSet_info Pointer of the settings description.
 * @return int 0 will be returned if the initialate was successfully
 */
int
shell_init(
    sh_io_desc_t *pDesc,
    sh_set_t     *pSet_info);


/**
 * @brief Register the CLI command to shell module.
 *
 * @param pCmd The pointer of command description
 * @return int 0 will be returned if the register was successfully
 */
int
shell_register_cmd(sh_cmd_t *pCmd);

/**
 * @brief Proces of the shell module
 *
 * @param pArg Pointer of the shell arguments.
 */
void *
shell_proc(sh_args_t *pArg);


/** @} */
#endif
#ifdef __cplusplus
}
#endif
