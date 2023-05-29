/* See Project CHIP LICENSE file for licensing information. */
#include <platform/logging/LogV.h>

#include <lib/core/CHIPConfig.h>
#include <platform/CHIPDeviceConfig.h>

#include <lib/support/SafeString.h>
#include <lib/support/logging/CHIPLogging.h>

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include <openthread/platform/logging.h>
#endif

#include <FreeRTOS.h>
#include <queue.h>
#include <stdio.h>
#include <string.h>
#include <task.h>

#include "util_log.h"

#define LOG_ERROR "<error > "
#define LOG_WARN "<warn  > "
#define LOG_INFO "<info  > "
#define LOG_DETAIL "<detail> "
#if CHIP_SYSTEM_CONFIG_USE_LWIP
#define LOG_LWIP "<lwip  > "
#endif // CHIP_SYSTEM_CONFIG_USE_LWIP
#define LOG_RT582 "<rt582 > "
// If a new category string LOG_* is created, add it in the MaxStringLength arguments below
#if CHIP_SYSTEM_CONFIG_USE_LWIP
static constexpr size_t kMaxCategoryStrLen = chip::MaxStringLength(LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DETAIL, LOG_LWIP, LOG_RT582);
#else
static constexpr size_t kMaxCategoryStrLen = chip::MaxStringLength(LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DETAIL, LOG_RT582);
#endif // CHIP_SYSTEM_CONFIG_USE_LWIP

#if LOG_RTT_BUFFER_INDEX != 0
static uint8_t sLogBuffer[LOG_RTT_BUFFER_SIZE];
static uint8_t sCmdLineBuffer[LOG_RTT_BUFFER_SIZE];
#endif

static void PrintLog(const char * msg)
{
    info("%s\r\n", msg);
}


namespace chip {
namespace DeviceLayer {

/**
 * Called whenever a log message is emitted by Chip or LwIP.
 *
 * This function is intended be overridden by the application to, e.g.,
 * schedule output of queued log entries.
 */
void __attribute__((weak)) OnLogOutput(void) {}

} // namespace DeviceLayer
} // namespace chip

namespace chip {
namespace Logging {
namespace Platform {

/**
 * CHIP log output functions.
 */
void LogV(const char * module, uint8_t category, const char * aFormat, va_list v)
{
    #if _CHIP_USE_LOGGING
    if (IsCategoryEnabled(category))
    {
        char formattedMsg[CHIP_CONFIG_LOG_MESSAGE_MAX_SIZE];
        size_t formattedMsgLen;

        // len for Category string + "[" + Module name + "] " (Brackets and space =3)
        constexpr size_t maxPrefixLen = kMaxCategoryStrLen + kMaxModuleNameLen + 3;
        static_assert(sizeof(formattedMsg) > maxPrefixLen); // Greater than to at least accommodate a ending Null Character

        switch (category)
        {
        case kLogCategory_Error:
            strcpy(formattedMsg, LOG_ERROR);
            break;
        case kLogCategory_Progress:
        default:
            strcpy(formattedMsg, LOG_INFO);
            break;
        case kLogCategory_Detail:
            strcpy(formattedMsg, LOG_DETAIL);
            break;
        }

        formattedMsgLen = strlen(formattedMsg);

        // Form the log prefix, e.g. "[DL] "
        snprintf(formattedMsg + formattedMsgLen, sizeof(formattedMsg) - formattedMsgLen, "[%s] ", module);
        formattedMsg[sizeof(formattedMsg) - 1] = 0;
        formattedMsgLen                        = strlen(formattedMsg);

        size_t len = vsnprintf(formattedMsg + formattedMsgLen, sizeof formattedMsg - formattedMsgLen, aFormat, v);

        if (len >= sizeof formattedMsg - formattedMsgLen)
        {
            formattedMsg[sizeof formattedMsg - 1] = '\0';
        }

        PrintLog(formattedMsg);
    }
    // Let the application know that a log message has been emitted.
    chip::DeviceLayer::OnLogOutput();
    #endif
}

} // namespace Platform
} // namespace Logging
} // namespace chip

/**
 * Platform logging function for OpenThread
 */
#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
extern "C" void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion, const char * aFormat, ...)
{
    (void) aLogRegion;
    va_list v;

    va_start(v, aFormat);
    char formattedMsg[CHIP_CONFIG_LOG_MESSAGE_MAX_SIZE];

    switch (aLogLevel)
    {
    case OT_LOG_LEVEL_CRIT:
        strcpy(formattedMsg, LOG_ERROR "[ot] ");
        break;
    case OT_LOG_LEVEL_WARN:
        strcpy(formattedMsg, LOG_WARN "[ot] ");
        break;
    case OT_LOG_LEVEL_NOTE:
        strcpy(formattedMsg, LOG_INFO "[ot] ");
        break;
    case OT_LOG_LEVEL_INFO:
        strcpy(formattedMsg, LOG_INFO "[ot] ");
        break;
    case OT_LOG_LEVEL_DEBG:
        strcpy(formattedMsg, LOG_DETAIL "[ot] ");
        break;
    default:
        strcpy(formattedMsg, LOG_DETAIL "[ot] ");
        break;
    }

    size_t prefixLen = strlen(formattedMsg);
    size_t len       = vsnprintf(formattedMsg + prefixLen, sizeof(formattedMsg) - prefixLen, aFormat, v);

    if (len >= sizeof formattedMsg - prefixLen)
    {
        formattedMsg[sizeof formattedMsg - 1] = '\0';
    }

    PrintLog(formattedMsg);

#if configCHECK_FOR_STACK_OVERFLOW
    // Force a stack overflow check.
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
        taskYIELD();
#endif

    // Let the application know that a log message has been emitted.
    chip::DeviceLayer::OnLogOutput();
    va_end(v);
}
#endif // CHIP_ENABLE_OPENTHREAD

//#if HARD_FAULT_LOG_ENABLE && EFR32_LOG_ENABLED

/**
 * Log register contents to UART when a hard fault occurs.
 */
extern "C" void debugHardfault(uint32_t * sp)
{
    while (1)
        ;
}

/**
 * Override default hard-fault handler
 */
#if 0
extern "C" __attribute__((naked)) void HardFault_Handler(void)
{
    __asm volatile("tst lr, #4                                    \n"
                   "ite eq                                        \n"
                   "mrseq r0, msp                                 \n"
                   "mrsne r0, psp                                 \n"
                   "ldr r1, debugHardfault_address                \n"
                   "bx r1                                         \n"
                   "debugHardfault_address: .word debugHardfault  \n");
}
#endif
/*-----------------------------------------------------------*/



/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
extern "C" void vApplicationGetIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer, StackType_t ** ppxIdleTaskStackBuffer,
                                              uint32_t * pulIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
extern "C" void vApplicationGetTimerTaskMemory(StaticTask_t ** ppxTimerTaskTCBBuffer, StackType_t ** ppxTimerTaskStackBuffer,
                                               uint32_t * pulTimerTaskStackSize)
{
    /* If the buffers to be provided to the Timer task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
//#endif // HARD_FAULT_LOG_ENABLE && EFR32_LOG_ENABLED
