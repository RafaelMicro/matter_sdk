/** @file shell.c
 *
 * @license
 * @description
 */

#include <string.h>
#include <stdarg.h>
#include "project_config.h"

#if (MODULE_ENABLE(SUPPORT_DEBUG_CONSOLE_CLI))
#include "types.h"
#include "util_printf.h"
#include "shell.h"
#include "bsp_console.h"
//=============================================================================
//                  Constant Definition
//=============================================================================
#define CONFIG_SH_CMD_ARG_MAX_NUM           24


#define SH_LF       '\n'
#define SH_CR       '\r'

/**
 *  escape code
 */
#define ESC_START_CODE              0x1b
#define ESC_CODE_HOME               "\x1b[1~"
#define ESC_CODE_INSERT             "\x1b[2~"
#define ESC_CODE_DEL                "\x1b[3~"
#define ESC_CODE_END                "\x1b[4~"
#define ESC_CODE_PAGE_UP            "\x1b[5~"
#define ESC_CODE_PAGE_DOWN          "\x1b[6~"

#define ESC_CODE_UP                 "\x1b[A"
#define ESC_CODE_DOWN               "\x1b[B"
#define ESC_CODE_RIGHT              "\x1b[C"
#define ESC_CODE_LEFT               "\x1b[D"

#define ESC_CODE_VT100_DELETE       "\x1b[P"
#define ESC_CODE_VT100_INSTER       "\x1b[4h"

#define ESC_CODE_BACKSPACE          "\x08"
#define ESC_CODE_TAB                "\x09"
#define ESC_CODE_CTRL_C             "\x03"

typedef enum sh_state
{
    SH_STATE_GET_LINE   =  1,
    SH_STATE_OK         =  0,
    SH_STATE_ERR        = -1,
} sh_state_t;

typedef enum sh_esc_code
{
    SH_ESC_CODE_UNKNOWN    = 0,
    SH_ESC_CODE_HOME,
    SH_ESC_CODE_INSERT,
    SH_ESC_CODE_DELETE,
    SH_ESC_CODE_END,
    SH_ESC_CODE_PAGE_UP,
    SH_ESC_CODE_PAGE_DOWN,
    SH_ESC_CODE_UP,
    SH_ESC_CODE_DOWN,
    SH_ESC_CODE_LEFT,
    SH_ESC_CODE_RIGHT,
    SH_ESC_CODE_BACKSPACE,
    SH_ESC_CODE_TAB,
    SH_ESC_CODE_CTRL_C,

    SH_ESC_CODE_TOTAL

} sh_esc_code_t;
//=============================================================================
//                  Macro Definition
//=============================================================================
#ifndef __unused
#define __unused            __attribute__ ((unused))
#endif

#if 0
#define err(str, ...)       do { utility_printf("[%s:%u] " str, __func__, __LINE__, ##__VA_ARGS__); while(1); } while(0)
#else
#define err(str, ...)
#endif


#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern uint32_t Image$$RW_IROM_CLI_CMD_POOL_START$$RO$$Base;
extern uint32_t Image$$RW_IROM_CLI_CMD_POOL_START$$RO$$Limit;
#define SHELL_CLI_CMD_POOL_START    ((void*)&Image$$RW_IROM_CLI_CMD_POOL_START$$RO$$Base)
#define SHELL_CLI_CMD_POOL_END    ((void*)&Image$$RW_IROM_CLI_CMD_POOL_START$$RO$$Limit)
#else
extern uint32_t     __cli_cmd_pool_start__;
extern uint32_t     __cli_cmd_pool_end__;
#define SHELL_CLI_CMD_POOL_START    ((void*)&__cli_cmd_pool_start__)
#define SHELL_CLI_CMD_POOL_END    ((void*)&__cli_cmd_pool_end__)
#endif

//=============================================================================
//                  Structure Definition
//=============================================================================
typedef struct sh_esc_item
{
    sh_esc_code_t   esc_code;
    uint16_t        esc_code_len;
    char            *pEsc_ascii;

} sh_esc_item_t;

typedef struct sh_dev
{
    sh_io_desc_t    *pIO;

    char            *pLine_buf;
    uint32_t        line_buf_len;
    uint32_t        rd_pos;
    uint32_t        wr_pos;
    uint32_t        cursor_pos;

    union
    {
        uint32_t        esc_u32_buf[2];
        char            esc_op_buf[8];
    } esc_buf;

    int             esc_input_cumulation;

    void            *pUser_data;

#if (MODULE_ENABLE(SUPPORT_SHELL_CMD_HISTORY))
    char            *pHistory_buf;
    uint16_t        line_size;
    uint16_t        cmd_deep;
    char            **ppLine_list;
#endif // defined

    int             row_index;
} sh_dev_t;
//=============================================================================
//                  Global Data Definition
//=============================================================================
static sh_dev_t         g_sh_dev = { .pIO = 0, };
static sh_cmd_t         *g_pCmd_head = 0;

static sh_esc_item_t    g_esc_code_table[] =
{
    { .esc_code = SH_ESC_CODE_HOME,      .pEsc_ascii = ESC_CODE_HOME,      .esc_code_len = 4 },
    { .esc_code = SH_ESC_CODE_INSERT,    .pEsc_ascii = ESC_CODE_INSERT,    .esc_code_len = 4 },
    { .esc_code = SH_ESC_CODE_DELETE,    .pEsc_ascii = ESC_CODE_DEL,       .esc_code_len = 4 },
    { .esc_code = SH_ESC_CODE_DELETE,    .pEsc_ascii = ESC_CODE_VT100_DELETE, .esc_code_len = 3 },
    { .esc_code = SH_ESC_CODE_END,       .pEsc_ascii = ESC_CODE_END,       .esc_code_len = 4 },
    { .esc_code = SH_ESC_CODE_PAGE_UP,   .pEsc_ascii = ESC_CODE_PAGE_UP,   .esc_code_len = 4 },
    { .esc_code = SH_ESC_CODE_PAGE_DOWN, .pEsc_ascii = ESC_CODE_PAGE_DOWN, .esc_code_len = 4 },
    { .esc_code = SH_ESC_CODE_UP,        .pEsc_ascii = ESC_CODE_UP,        .esc_code_len = 3 },
    { .esc_code = SH_ESC_CODE_DOWN,      .pEsc_ascii = ESC_CODE_DOWN,      .esc_code_len = 3 },
    { .esc_code = SH_ESC_CODE_LEFT,      .pEsc_ascii = ESC_CODE_LEFT,      .esc_code_len = 3 },
    { .esc_code = SH_ESC_CODE_RIGHT,     .pEsc_ascii = ESC_CODE_RIGHT,     .esc_code_len = 3 },
    { .esc_code = SH_ESC_CODE_BACKSPACE, .pEsc_ascii = ESC_CODE_BACKSPACE, .esc_code_len = 1 },
    { .esc_code = SH_ESC_CODE_TAB,       .pEsc_ascii = ESC_CODE_TAB,       .esc_code_len = 1 },
    { .esc_code = SH_ESC_CODE_CTRL_C,    .pEsc_ascii = ESC_CODE_CTRL_C,    .esc_code_len = 1 },
    { .esc_code = SH_ESC_CODE_TOTAL, }
};
//=============================================================================
//                  Private Function Definition
//=============================================================================
static int
_shell_log(const char *fmt, ...)
{
    static char     log_buf[512] = {0};
    int             rval = 0;
    sh_io_desc_t    *pIO = g_sh_dev.pIO;
    va_list         args;


    va_start(args, fmt);
    utility_vsprintf(log_buf, sizeof(log_buf), fmt, args);
    va_end(args);
    if (pIO && pIO->cb_write )
    {
        rval = pIO->cb_write((uint8_t *)log_buf, strlen(log_buf), g_sh_dev.pUser_data);
    }

    return rval;
}

#if (MODULE_ENABLE(SUPPORT_SHELL_CMD_HISTORY))
static int
_shell_push_to_history(char *pLine)
{
    int     rval = 0;
    do
    {
        char    *pAct_line = g_sh_dev.ppLine_list[g_sh_dev.cmd_deep - 1];

        for (int i = g_sh_dev.cmd_deep - 1; i > 0; i--)
        {
            g_sh_dev.ppLine_list[i] = g_sh_dev.ppLine_list[i - 1];
        }

        g_sh_dev.ppLine_list[0] = pAct_line;

        utility_snprintf(pAct_line, g_sh_dev.line_size, "%s", pLine);

    } while (0);

    g_sh_dev.row_index = 0;

    return rval;
}

static char *
_shell_pop_history(int *pRow_index)
{
    char    *pLine = 0;
    do
    {
        int     row_index = *pRow_index;
        if ( row_index < 0 )
        {
            row_index += g_sh_dev.cmd_deep;
        }
        else if ( row_index >= g_sh_dev.cmd_deep )
        {
            row_index -= g_sh_dev.cmd_deep;
        }

        *pRow_index = row_index;

        pLine = g_sh_dev.ppLine_list[row_index];
    } while (0);
    return pLine;
}
#else
#define _shell_push_to_history(pLine)
#define _shell_pop_history(pRow_index)       0
#endif

static int
_shell_erase_line(
    uint32_t        *pWr_pos,
    uint32_t        *pCursor_pos,
    int (*cb_write)(uint8_t *, uint32_t, void *))
{
    int     rval = 0;
    do
    {
        // move to end of line
        char        *pLine_buf = g_sh_dev.pLine_buf;
        uint32_t    wr_pos = *pWr_pos;
        uint32_t    cursor_pos = *pCursor_pos;

        if ( cursor_pos < wr_pos )
        {
            char    str_buf[12] = {0};
            utility_snprintf(str_buf, 12, "\x1b[%dC", (int)(wr_pos - cursor_pos));
            cb_write((uint8_t *)str_buf, strlen(str_buf), g_sh_dev.pUser_data);
        }

        // delete characters
        while ( wr_pos-- )
        {
            cb_write((uint8_t *)ESC_CODE_LEFT ESC_CODE_VT100_DELETE, 6, g_sh_dev.pUser_data);
        }

        // reset parameters
        *pCursor_pos = *pWr_pos = 0;
        pLine_buf[0] = '\0';
    } while (0);
    return rval;
}

static int
_shell_get_esc_code(
    char            character,
    sh_esc_code_t   *pEsc_code)
{
    int     rval = 0;
    do
    {
        sh_esc_item_t   *pEsc_item_cur = g_esc_code_table;

        if ( (uint8_t)character == ESC_START_CODE ||
                g_sh_dev.esc_input_cumulation >= sizeof(g_sh_dev.esc_buf.esc_op_buf) )
        {
            g_sh_dev.esc_input_cumulation = 0;
            g_sh_dev.esc_buf.esc_u32_buf[0]       = 0;
            g_sh_dev.esc_buf.esc_u32_buf[1]       = 0;
        }

        g_sh_dev.esc_buf.esc_op_buf[g_sh_dev.esc_input_cumulation++] = character;

        while ( pEsc_item_cur->esc_code != SH_ESC_CODE_TOTAL )
        {
            if ( g_sh_dev.esc_input_cumulation < pEsc_item_cur->esc_code_len )
            {
                pEsc_item_cur++;
                continue;
            }

            if ( !strncmp(pEsc_item_cur->pEsc_ascii,
                          g_sh_dev.esc_buf.esc_op_buf,
                          pEsc_item_cur->esc_code_len) )
            {
                *pEsc_code = pEsc_item_cur->esc_code;

                g_sh_dev.esc_input_cumulation = 0;
                g_sh_dev.esc_buf.esc_u32_buf[0]       = 0;
                g_sh_dev.esc_buf.esc_u32_buf[1]       = 0;
                break;
            }

            pEsc_item_cur++;
        }

        if ( g_sh_dev.esc_input_cumulation == 1 &&
                (uint8_t)character != ESC_START_CODE )
        {
            *pEsc_code = SH_ESC_CODE_TOTAL;
            g_sh_dev.esc_input_cumulation = 0;
            g_sh_dev.esc_buf.esc_u32_buf[0]       = 0;
            g_sh_dev.esc_buf.esc_u32_buf[1]       = 0;
        }

    } while (0);
    return rval;
}

__unused static int
_shell_esc_code_proc(
    sh_esc_code_t   esc_code,
    uint32_t        *pWr_pos,
    uint32_t        *pCursor_pos,
    int (*cb_write)(uint8_t *, uint32_t, void *))
{
    int     rval = 0;

    switch ( esc_code )
    {
    case SH_ESC_CODE_BACKSPACE:
        if ( *pWr_pos > 0 && *pCursor_pos > 0 )
        {
            char        *pLine_buf = g_sh_dev.pLine_buf;
            uint32_t    len = 0;
            uint32_t    cursor_pos = *pCursor_pos;

            cb_write((uint8_t *)ESC_CODE_LEFT ESC_CODE_VT100_DELETE, 6, g_sh_dev.pUser_data);

            len = strlen(pLine_buf);
            if ( cursor_pos == len )
            {
                // at line end
                pLine_buf[len - 1] = '\0';
                *pCursor_pos = *pCursor_pos - 1;
            }
            else
            {
                len++;
                for (int i = --cursor_pos; i < len; i++)
                {
                    pLine_buf[i] = pLine_buf[i + 1];
                }

                *pCursor_pos = cursor_pos;
            }

            *pWr_pos = *pWr_pos - 1;
        }
        break;
    case SH_ESC_CODE_HOME:
        if ( *pCursor_pos > 0 )
        {
            char    str_buf[12] = {0};
            utility_snprintf(str_buf, 12, "\x1b[%dD", (int)(*pCursor_pos));
            cb_write((uint8_t *)str_buf, strlen(str_buf), g_sh_dev.pUser_data);
            *pCursor_pos = 0;
        }
        break;
    case SH_ESC_CODE_END:
        if ( *pCursor_pos < *pWr_pos )
        {
            char    str_buf[12] = {0};
            utility_snprintf(str_buf, 12, "\x1b[%dC", (int)((*pWr_pos) - (*pCursor_pos)));
            cb_write((uint8_t *)str_buf, strlen(str_buf), g_sh_dev.pUser_data);
            *pCursor_pos = *pWr_pos;
        }
        break;
    case SH_ESC_CODE_UP:
    case SH_ESC_CODE_DOWN:
    {
        char        *pLine = 0;
        int         line_len = 0;

        _shell_erase_line(pWr_pos, pCursor_pos, cb_write);

        pLine    = _shell_pop_history(&g_sh_dev.row_index);
        line_len = (pLine) ? strlen(pLine) : 0;

        if ( pLine && line_len )
        {
            strncpy(g_sh_dev.pLine_buf, pLine, line_len);
            cb_write((uint8_t *)pLine, line_len, g_sh_dev.pUser_data);
            *pCursor_pos = *pWr_pos = line_len;
        }

        if ( esc_code == SH_ESC_CODE_UP )
        {
            g_sh_dev.row_index++;
        }
        else
        {
            g_sh_dev.row_index--;
        }
    }
    break;
    case SH_ESC_CODE_LEFT:
        if ( *pCursor_pos > 0 )
        {
            cb_write((uint8_t *)ESC_CODE_LEFT, 3, g_sh_dev.pUser_data);
            (*pCursor_pos)--;
        }
        break;
    case SH_ESC_CODE_RIGHT:
        if ( *pCursor_pos < *pWr_pos )
        {
            cb_write((uint8_t *)ESC_CODE_RIGHT, 3, g_sh_dev.pUser_data);
            (*pCursor_pos)++;
        }
        break;
    case SH_ESC_CODE_INSERT:
#if 0
        cb_write((uint8_t *)ESC_CODE_VT100_INSTER, 3, g_sh_dev.pUser_data);
#endif
        break;
    case SH_ESC_CODE_DELETE:
        if ( *pCursor_pos < *pWr_pos )
        {
            char        *pLine_buf = g_sh_dev.pLine_buf;
            uint32_t    len = 0;
            uint32_t    cursor_pos = *pCursor_pos;

            cb_write((uint8_t *)ESC_CODE_VT100_DELETE, 3, g_sh_dev.pUser_data);

            len = strlen(pLine_buf);
            for (int i = cursor_pos; i < len; i++)
            {
                pLine_buf[i] = pLine_buf[i + 1];
            }

            *pCursor_pos = cursor_pos;

            *pWr_pos = *pWr_pos - 1;
        }
        break;

    case SH_ESC_CODE_PAGE_UP:
    case SH_ESC_CODE_PAGE_DOWN:
    case SH_ESC_CODE_TAB:
    case SH_ESC_CODE_CTRL_C:
    default:
        break;
    }
    return rval;
}

static sh_state_t
_shell_read_line(
    sh_io_desc_t    *pIO)
{
    sh_state_t      rval = SH_STATE_OK;

    do
    {
        int (*cb_read)(uint8_t *, uint32_t, void *);
        int (*cb_write)(uint8_t *, uint32_t, void *);
        int (*cb_flush)(void);
        char    c = 0;

        if ( !pIO->cb_read || !pIO->cb_write )
        {
            break;
        }

        cb_read  = pIO->cb_read;
        cb_write = pIO->cb_write;
        cb_flush = pIO->cb_flush;

        if (g_sh_dev.cursor_pos == (g_sh_dev.line_buf_len - 1))
        {
            cb_flush();
            g_sh_dev.cursor_pos = 0;
            g_sh_dev.wr_pos = 0;
            g_sh_dev.rd_pos = 0;
        }

        if ( cb_read((uint8_t *)&c, 1, g_sh_dev.pUser_data) == 0 )
        {
            break;
        }

        {
            // read to line buffer
            uint32_t        pos = 0;
            uint32_t        wr_pos = g_sh_dev.wr_pos;
            uint32_t        rd_pos = g_sh_dev.rd_pos;
            sh_esc_code_t   esc_code = SH_ESC_CODE_UNKNOWN;

            pos = (wr_pos + 1) % g_sh_dev.line_buf_len;
            if ( pos == rd_pos )
            {
                break;
            }

            if ( c == '\n' || c == '\r' )
            {
                //                if( (wr_pos + 1) < g_sh_dev.line_buf_len )
                //                {
                //                    g_sh_dev.pLine_buf[wr_pos++] = '\n';
                //                    pos = (wr_pos + 1) % g_sh_dev.line_buf_len;
                //                }

                g_sh_dev.pLine_buf[wr_pos] = '\0';
                g_sh_dev.wr_pos            = pos;

                _shell_push_to_history(g_sh_dev.pLine_buf);

                rval = SH_STATE_GET_LINE;
                break;
            }

            _shell_get_esc_code(c, &esc_code);

            if ( esc_code == SH_ESC_CODE_TOTAL )
            {
                // non-escape text
                char        *pLine_buf = g_sh_dev.pLine_buf;
                uint32_t    len = 0;

                if ( g_sh_dev.cursor_pos > wr_pos )
                {
                    rval = SH_STATE_ERR;
                    break;
                }

                len = strlen(pLine_buf);
                pLine_buf[len + 1] = '\0';

                for (int i = len; i > g_sh_dev.cursor_pos; i--)
                {
                    pLine_buf[i] = pLine_buf[i - 1];
                }

                g_sh_dev.wr_pos = pos;

                pLine_buf[g_sh_dev.cursor_pos] = c;

                // show the word uart receive
#if (MODULE_ENABLE(SUPPORT_SHELL_CMD_HISTORY))
                cb_write((uint8_t *)&pLine_buf[g_sh_dev.cursor_pos],
                         g_sh_dev.wr_pos - g_sh_dev.cursor_pos,
                         g_sh_dev.pUser_data);

                if ( g_sh_dev.cursor_pos < wr_pos )
                {
                    char        str_buf[12] = {0};
                    utility_snprintf(str_buf, 12, "\x1b[%dD", (int)(g_sh_dev.wr_pos - g_sh_dev.cursor_pos - 1));
                    cb_write((uint8_t *)str_buf, strlen(str_buf), g_sh_dev.pUser_data);
                }
#endif
                g_sh_dev.cursor_pos += 1;
            }
            else
            {
                // handle escape control code
                _shell_esc_code_proc(esc_code, &g_sh_dev.wr_pos, &g_sh_dev.cursor_pos, cb_write);
            }
        }

    } while (0);

    return rval;
}

static int
_sh_cmd_help(int argc, char **argv, cb_shell_out_t log_out, void *pExtra)
{
    uint32_t     cli_cmd_pool_start = (uint32_t)SHELL_CLI_CMD_POOL_START;
    uint32_t     cli_cmd_pool_end = (uint32_t)SHELL_CLI_CMD_POOL_END;
    sh_cmd_t            *pCur = g_pCmd_head;

    if (argc == 2)
    {
        pCur = (sh_cmd_t *)cli_cmd_pool_start;
        while ( (uintptr_t)pCur < (uintptr_t)cli_cmd_pool_end )
        {
            if (strcmp(pCur->pCmd_name, argv[1]) == 0)
            {
                log_out("\ncommand: '%s'\n%s\n", pCur->pCmd_name, pCur->pDescription);
            }
            pCur++;
            if ((uintptr_t)pCur->pCmd_name == 0xFFFFFFFF)
            {
                break;
            }
        }
        return 0;
    }

    pCur = (sh_cmd_t *)cli_cmd_pool_start;
    while ( (uintptr_t)pCur < (uintptr_t)cli_cmd_pool_end  )
    {
        log_out("\ncommand: '%s'\n  %s\n", pCur->pCmd_name, pCur->pDescription);
        pCur++;
        if ((uintptr_t)pCur->pCmd_name == 0xFFFFFFFF)
        {
            break;
        }
    }

    pCur = g_pCmd_head;
    while ( pCur )
    {
        log_out("\ncommand: '%s'\n  %s\n", pCur->pCmd_name, pCur->pDescription);
        pCur = pCur->next;
        if ((uintptr_t)pCur->pCmd_name == 0xFFFFFFFF)
        {
            break;
        }
    }

    return 0;
}

const sh_cmd_t   g_sh_cmd_desc_help __cli_cmd_pool =
{
    .pCmd_name    = "help",
    .pDescription = "list commands",
    .cmd_exec     = &_sh_cmd_help,
};
//=============================================================================
//                  Public Function Definition
//=============================================================================
int
shell_init(
    sh_io_desc_t *pDesc,
    sh_set_t     *pSet_info)
{
    int     rval = 0;

    do
    {
        if ( !pDesc )
        {
            rval = ERRNO_NULL_POINTER;
            err("input NULL data\n");
            break;
        }

        memset(&g_sh_dev, 0x0, sizeof(sh_dev_t));

        g_sh_dev.pIO          = pDesc;
        g_sh_dev.pLine_buf    = pSet_info->pLine_buf;
        g_sh_dev.line_buf_len = pSet_info->line_buf_len;
        g_sh_dev.pUser_data   = pSet_info->pUser_data;

#if (MODULE_ENABLE(SUPPORT_SHELL_CMD_HISTORY))
        if ( pSet_info->history_buf_size <
                SHELL_CALC_HISTORY_BUFFER(pSet_info->line_size, pSet_info->cmd_deep) )
        {
            rval = ERRNO_WRONG_PARAM;
            err("Wrong cmd history buffer size !\n");
            break;
        }

        g_sh_dev.pHistory_buf = pSet_info->pHistory_buf;
        g_sh_dev.line_size    = pSet_info->line_size;
        g_sh_dev.cmd_deep     = pSet_info->cmd_deep;
        g_sh_dev.ppLine_list  = (char **)pSet_info->pHistory_buf;
        memset(g_sh_dev.pHistory_buf, 0x0, pSet_info->history_buf_size);

        for (int i = 0; i < g_sh_dev.cmd_deep; i++)
        {
            g_sh_dev.ppLine_list[i] = (char *)((uintptr_t)pSet_info->pHistory_buf
                                               + sizeof(uintptr_t) * g_sh_dev.cmd_deep
                                               + i * g_sh_dev.line_size);
        }
#endif // defined

        if ( g_sh_dev.pLine_buf )
        {
            memset(g_sh_dev.pLine_buf, 0x0, g_sh_dev.line_buf_len);
        }

        if ( g_sh_dev.pIO && g_sh_dev.pIO->cb_init )
        {
            rval = g_sh_dev.pIO->cb_init(pSet_info);
        }

        if ( rval )
        {
            break;
        }

    } while (0);

    return rval;
}

int
shell_deinit(void *pInfo)
{
    int     rval = 0;

    if ( g_sh_dev.pIO && g_sh_dev.pIO->cb_deinit )
    {
        rval = g_sh_dev.pIO->cb_deinit(pInfo);
    }

    return rval;
}

int
shell_register_cmd(sh_cmd_t *pCmd)
{
    int     rval = 0;

    if ( g_pCmd_head )
    {
        sh_cmd_t    *pCur = g_pCmd_head;

        while ( pCur->next )
        {
            pCur = pCur->next;
        }

        pCur->next = pCmd;
    }
    else
    {
        g_pCmd_head = pCmd;
    }
    return rval;
}

void *
shell_proc(sh_args_t *pArg)
{
    sh_io_desc_t    *pIO = g_sh_dev.pIO;
    static uint32_t        is_line_head = 1;


    do
    {
        sh_state_t      rval = SH_STATE_OK;

        pArg->exec_result = SH_EXEC_RVAL_UNKNOWN;

        if ( pIO )
        {
            if ( is_line_head )
            {
#if (MODULE_ENABLE(SUPPORT_SHELL_CMD_HISTORY))
                if ( pIO->cb_write )
                {
                    pIO->cb_write((uint8_t *)SHELL_PROMPT, sizeof(SHELL_PROMPT), g_sh_dev.pUser_data);
                }
#endif

                is_line_head = 0;
            }

            rval = _shell_read_line(pIO);
            if ( rval == SH_STATE_GET_LINE )
            {
                int         arg_cnt = 0;
                char        *pCmd_args[CONFIG_SH_CMD_ARG_MAX_NUM] = {0};
                char        *pCur = g_sh_dev.pLine_buf;
                char        *pEnd = g_sh_dev.pLine_buf + strlen(g_sh_dev.pLine_buf);
                uint32_t    is_arg_head = 0;

                // -------------------------------
                // parse arguments from line buffer
                is_arg_head = 1;
                while ( pCur < pEnd )
                {
                    char    c = *pCur;

                    if ( c == ' ' || c == '\t' || c == '\n' )
                    {
                        *pCur++ = '\0';
                        is_arg_head = 1;
                        continue;
                    }

                    if ( is_arg_head && (arg_cnt < CONFIG_SH_CMD_ARG_MAX_NUM) &&
                            ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') ||
                             (c >= 'A' && c <= 'Z') || c == '_' || c == '?' || c == '-' || c == '='))
                    {
                        pCmd_args[arg_cnt++] = pCur;
                        is_arg_head = 0;
                    }

                    pCur++;
                }

                // show the word uart receive
#if (MODULE_ENABLE(SUPPORT_SHELL_CMD_HISTORY))
                if ( pIO->cb_write )
                {
                    pIO->cb_write((uint8_t *)"\n", 1, g_sh_dev.pUser_data);
                }
#endif

                //-----------------------
                // execute command
                if ( arg_cnt )
                {
                    sh_cmd_t            *pCmd_cur = 0;
                    sh_cmd_t            *pCmd_act = 0;
                    uint32_t     cli_cmd_pool_start = (uint32_t)SHELL_CLI_CMD_POOL_START;
                    uint32_t     cli_cmd_pool_end = (uint32_t)SHELL_CLI_CMD_POOL_END;

                    pCmd_cur = (sh_cmd_t *)cli_cmd_pool_start;
                    while ( (uintptr_t)pCmd_cur < (uintptr_t)cli_cmd_pool_end  )
                    {
                        if ( !strncmp(pCmd_cur->pCmd_name, pCmd_args[0],
                                      strlen(pCmd_cur->pCmd_name)) )
                        {
                            pCmd_act = pCmd_cur;
                            break;
                        }
                        pCmd_cur++;
                    }

                    if ( !pCmd_act )
                    {
                        pCmd_cur = g_pCmd_head;
                        while ( pCmd_cur )
                        {
                            if ( !strncmp(pCmd_cur->pCmd_name, pCmd_args[0],
                                          3) )
                            {
                                pCmd_act = pCmd_cur;
                                break;
                            }
                            pCmd_cur = pCmd_cur->next;
                        }
                    }

                    if ( pCmd_act )
                    {
                        pArg->exec_result = pCmd_act->cmd_exec(arg_cnt, pCmd_args, _shell_log, pCmd_act->pExtra);
                    }
                }

                //-----------------------
                // reset parameters
                is_line_head = 1;

                g_sh_dev.wr_pos     = 0;
                g_sh_dev.cursor_pos = 0;
                memset(g_sh_dev.pLine_buf, 0x0, g_sh_dev.line_buf_len);
            }
        }

        if ( pArg->cb_regular_alarm )
        {
            pArg->cb_regular_alarm(pArg);
        }
    } while ( pArg->is_blocking );
    return 0;
}
#endif
