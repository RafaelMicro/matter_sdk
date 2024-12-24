// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
#ifndef __SYS_PRINTF_H__
#define __SYS_PRINTF_H__

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
//                Include (Better to prevent)
//=============================================================================
#include "util_log.h"
//=============================================================================
//                Public Definitions of const value
//=============================================================================
#define VASSERT             configASSERT

#define DEBUG_ALL                   0xFFFFFFFF
#define DEBUG_NONE                  0
#define DEBUG_SYS                   0x00000001
#define DEBUG_TIMER                 0x00000002

#define BLE_HCI_TASK                0x00000004
#define BLE_HOST_TASK               0x00000008

#define DEBUG_PROVISION             0x00001000
#define DEBUG_NETWORK               0x00002000
#define DEBUG_TRANS                 0x00004000
#define DEBUG_SEG                   0x00008000
#define DEBUG_FRIEND                0x00010000
#define DEBUG_SEC_BCN               0x00020000
#define DEBUG_LPN                   0x00040000
#define DEBUG_BEARER                0x00080000

#define DEBUG_HCI_CMD_EVT           0x00100000
#define DEBUG_HCI_DATA              0x00200000

#define DEBUG_DEMO_TASK             0x01000000
#define DEBUG_INFO                  0x02000000
#define DEBUG_ERR                   0x04000000
#define DEBUG_LOG                   0x08000000

#define FORCE_PRINT                 (DEBUG_INFO | DEBUG_ERR | DEBUG_LOG| DEBUG_PROVISION)
//=============================================================================
//                Public ENUM
//=============================================================================

//=============================================================================
//                Public Struct
//=============================================================================

//=============================================================================
//                Public Function Declaration
//=============================================================================

//==================================
//     Debug functions
//==================================
#define SYS_PRINTF(type, fmt, ...) \
    do { \
        if(FORCE_PRINT && (type & FORCE_PRINT)) {\
            if(type == DEBUG_ERR) \
                err(fmt, ##__VA_ARGS__);\
            else \
                info(fmt, ##__VA_ARGS__); } \
    } while(0)

#define SYS_BDUMP(type, addr, length)         \
    do { \
        if(FORCE_PRINT && (type & FORCE_PRINT))  { \
            util_log_mem(UTIL_LOG_INFO, "  ", (uint8_t *)addr, length, 0); } \
    } while(0)


#ifdef __cplusplus
};
#endif
#endif /* __SYS_PRINTF_H__ */
