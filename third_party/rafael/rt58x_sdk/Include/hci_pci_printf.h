#ifndef __HCI_PCI_PRINTF_H__
#define __HCI_PCI_PRINTF_H__

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
#define HCI_PCI_DEBUG_ALL            0xFFFFFFFF
#define HCI_PCI_DEBUG_NONE           0

#define COMMON_INFO                  0x00000001
#define COMMON_DEBUG_INFO            0x00000002
#define COMMON_DEBUG_ERR             0x00000004

#define HCI_TASK                     0x00000010
#define HCI_DEBUG_CMD_EVT            0x00000020
#define HCI_DEBUG_DATA               0x00000040

#define HCI_DEBUG_INFO               0x00000100
#define HCI_DEBUG_ERR                0x00000200
#define HCI_DEBUG_LOG                0x00000400

#define PCI_TASK                     0x00010000
#define PCI_DEBUG_CMD_EVT            0x00020000
#define PCI_DEBUG_DATA               0x00040000

#define PCI_DEBUG_INFO               0x00100000
#define PCI_DEBUG_ERR                0x00200000
#define PCI_DEBUG_LOG                0x00400000

#define HPCI_FORCE_PRINT             (PCI_DEBUG_ERR | PCI_DEBUG_INFO | PCI_DEBUG_LOG | \
                                      HCI_DEBUG_ERR | HCI_DEBUG_INFO | HCI_DEBUG_LOG | HCI_DEBUG_CMD_EVT | HCI_DEBUG_DATA | \
                                      COMMON_DEBUG_ERR | COMMON_DEBUG_INFO)


/**************************************************************************************************
 *    DEBUG FUNCTIONS
 *************************************************************************************************/
#define HPCI_PRINTF(type, fmt, ...) \
    do { \
        if(HPCI_FORCE_PRINT && (type & HPCI_FORCE_PRINT)) {\
            if((type == HCI_DEBUG_ERR) || (type == PCI_DEBUG_ERR)) \
                err(fmt, ##__VA_ARGS__);\
            else \
                util_log(UTIL_LOG_HCI_PCI, fmt, ##__VA_ARGS__); } \
    } while(0)

#define HPCI_BDUMP(type, addr, length)         \
    do { \
        if(HPCI_FORCE_PRINT && (type & HPCI_FORCE_PRINT))  { \
            util_log_mem(UTIL_LOG_HCI_PCI, "  ", (uint8_t *)addr, length, 0); } \
    } while(0)



#ifdef __cplusplus
};
#endif

#endif /* __HCI_PCI_PRINTF_H__*/
