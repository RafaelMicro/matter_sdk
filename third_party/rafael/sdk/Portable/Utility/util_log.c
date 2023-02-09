/** @file util_log.c
 *
 * @license
 * @description
 */

#include <stdarg.h>
#include <string.h>
#include "util.h"
#include "util_log.h"
#include "util_printf.h"
#include "util_string.h"
//=============================================================================
//                  Constant Definition
//=============================================================================
#define CONFIG_LOG_BUFFER_SIZE      512
//=============================================================================
//                  Macro Definition
//=============================================================================
#define LOG_TYPE_STATE(a)           util_log(UTIL_LOG_INFO, "%s= %d\n", util_to_string(a), BFIELD_IS_SET(&g_log_type_set, a))

//=============================================================================
//                  Structure Definition
//=============================================================================
DEFINE_BFIELD(UTIL_LOG_MAX);
//=============================================================================
//                  Global Data Definition
//=============================================================================
static bit_field_t      g_log_type_set;
#if defined(CONFIG_ENABLE_LOG_TEE)
static cb_log_rec_t     g_cb_log_recorder = 0;

static char             g_log_buf[CONFIG_LOG_BUFFER_SIZE] = {0};
#endif
//=============================================================================
//                  Private Function Definition
//=============================================================================

//=============================================================================
//                  Public Function Definition
//=============================================================================
void
util_log_init(void)
{
    BFIELD_ZERO(&g_log_type_set);
    BFIELD_SET(&g_log_type_set, UTIL_LOG_ERR);
    BFIELD_SET(&g_log_type_set, UTIL_LOG_INFO);
    return;
}

void
util_log_register_recorder(
    cb_log_rec_t    cb_recorder)
{
#if defined(CONFIG_ENABLE_LOG_TEE)
    g_cb_log_recorder = cb_recorder;
#endif
    return;
}

void
util_log(util_log_type_t type, const char *fmt, ...)
{
    if ( BFIELD_IS_SET(&g_log_type_set, type) )
    {
        va_list     va;
        va_start(va, fmt);

#if defined(CONFIG_ENABLE_LOG_TEE)
        {
            memset(g_log_buf, 0x0, CONFIG_LOG_BUFFER_SIZE);
            utility_vsprintf(g_log_buf, sizeof(g_log_buf), fmt, va);

            utility_puts(g_log_buf);
            if ( g_cb_log_recorder )
            {
                g_cb_log_recorder(type, g_log_buf, utility_strlen(g_log_buf));
            }
        }
#else
        utility_vsprintf(0, 0, fmt, va);
#endif

        va_end(va);
    }
    return;
}

void
util_log_on(util_log_type_t type)
{
    BFIELD_SET(&g_log_type_set, type);
    return;
}

void
util_log_off(util_log_type_t type)
{
    BFIELD_CLR(&g_log_type_set, type);
    return;
}

void
util_log_status(void)
{
    LOG_TYPE_STATE(UTIL_LOG_ERR);
    LOG_TYPE_STATE(UTIL_LOG_INFO);
    LOG_TYPE_STATE(UTIL_LOG_LEVEL_1);
    LOG_TYPE_STATE(UTIL_LOG_LEVEL_2);
    LOG_TYPE_STATE(UTIL_LOG_LEVEL_3);
    LOG_TYPE_STATE(UTIL_LOG_LEVEL_4);
    LOG_TYPE_STATE(UTIL_LOG_PROTOCOL);
    LOG_TYPE_STATE(UTIL_LOG_DEBUG);

    return;
}

void
util_log_mem(
    util_log_type_t   type,
    char                *prefix,
    uint8_t             *pAddr,
    int                 bytes,
    unsigned int        has_out_u32le)
{
    if ( has_out_u32le )
    {
        uintptr_t   addr = (uintptr_t)pAddr;
        uint32_t    cnt = (bytes + 0x3) >> 2;
        uint32_t    *pCur = (uint32_t *)pAddr;

        for (int i = 0; i < cnt; i++)
        {
            if ( (i & 0x3) == 2 )
            {
                util_log(type, " -");
            }
            else if ( !(i & 0x3) )
            {
                util_log(type, "\n%s%08X |", prefix, addr);
                addr += 16;
            }

            util_log(type, " %08X", pCur[i]);
        }
        util_log(type, "\n\n");
    }
    else
    {
        uintptr_t   addr = (uintptr_t)pAddr;
        uint8_t     *pCur = pAddr;

        for (int i = 0; i < bytes; i++)
        {
            if ( (i & 0xF) == 8 )
            {
                util_log(type, " -");
            }
            else if ( !(i & 0xF) )
            {
                util_log(type, "\n%s%08X |", prefix, addr);
                addr += 16;
            }

            util_log(type, " %02X", pCur[i]);
        }
        util_log(type, "\n\n");
    }
    return;
}
