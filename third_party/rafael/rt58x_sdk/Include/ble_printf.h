#ifndef __BLE_PRINTF_H__
#define __BLE_PRINTF_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "util_log.h"

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define BLE_DEBUG_ALL                0xFFFFFFFF
#define BLE_DEBUG_NONE               0

#define BLE_DEBUG_HCI_CMD_EVT        0x00100000
#define BLE_DEBUG_HCI_DATA           0x00200000

#define BLE_DEBUG_CMD_INFO           0x01000000
#define BLE_DEBUG_INFO               0x02000000
#define BLE_DEBUG_ERR                0x04000000
#define BLE_DEBUG_LOG                0x08000000

#define BLE_FORCE_PRINT              (BLE_DEBUG_INFO | BLE_DEBUG_ERR | BLE_DEBUG_LOG | BLE_DEBUG_HCI_CMD_EVT | BLE_DEBUG_HCI_DATA)


/**************************************************************************************************
 *    DEBUG FUNCTIONS
 *************************************************************************************************/
#define BLE_PRINTF(type, fmt, ...) \
    do { \
        if(BLE_FORCE_PRINT && (type & BLE_FORCE_PRINT)) {\
            if(type == BLE_DEBUG_ERR) \
                err(fmt, ##__VA_ARGS__);\
            else \
                util_log(UTIL_LOG_PROTOCOL, fmt, ##__VA_ARGS__); } \
    } while(0)

#define BLE_BDUMP(type, addr, length)         \
    do { \
        if(BLE_FORCE_PRINT && (type & BLE_FORCE_PRINT))  { \
            util_log_mem(UTIL_LOG_PROTOCOL, "  ", (uint8_t *)addr, length, 0); } \
    } while(0)



#ifdef __cplusplus
};
#endif

#endif /* __BLE_PRINTF_H__*/
