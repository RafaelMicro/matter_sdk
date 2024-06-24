#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdio.h>
#define configTICK_RATE_HZ (1000)
/* Definition used by Keil to replace default system clock source. */
#define configOVERRIDE_DEFAULT_TICK_CONFIGURATION 0

/* Hook function related definitions. */
#define configUSE_TICK_HOOK (0)
#define configCHECK_FOR_STACK_OVERFLOW (2)
#define configUSE_MALLOC_FAILED_HOOK (1)
#define configUSE_IDLE_HOOK (1)

/* Main functions*/
/* Run time stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS (0)

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES (0)
#define configMAX_CO_ROUTINE_PRIORITIES (1)

/* Software timer related definitions. */
#define configUSE_TIMERS (1)
#define configTIMER_TASK_PRIORITY (40) /* Highest priority */
#define configTIMER_QUEUE_LENGTH (10)
#define configTIMER_TASK_STACK_DEPTH (1024)

#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 4

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY (224)
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 128
#define configENABLE_FPU 0
#define configENABLE_MPU 0
/* FreeRTOS Secure Side Only and TrustZone Security Extension */
#define configRUN_FREERTOS_SECURE_ONLY 0
#define configENABLE_TRUSTZONE 0
/* FreeRTOS MPU specific definitions. */
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS (0)

extern uint32_t SystemCoreClock;
#define configCPU_CLOCK_HZ (SystemCoreClock)
#define configUSE_PREEMPTION (1)
#define configUSE_TIME_SLICING (0)
#define configUSE_PORT_OPTIMISED_TASK_SELECTION (0)
#define configUSE_TICKLESS_IDLE (1)

#define configMAX_PRIORITIES (56)
#define configMINIMAL_STACK_SIZE (128) /* Number of words to use for Idle and Timer stacks */
#define configMAX_TASK_NAME_LEN (10)
#define configUSE_16_BIT_TICKS (0)
#define configIDLE_SHOULD_YIELD (1)
#define configUSE_MUTEXES (1)
#define configUSE_RECURSIVE_MUTEXES (1)
#define configUSE_COUNTING_SEMAPHORES (1)
#define configUSE_TASK_NOTIFICATIONS 1
#define configUSE_TRACE_FACILITY 0
#define configQUEUE_REGISTRY_SIZE (10)
#define configUSE_QUEUE_SETS (0)
#define configUSE_NEWLIB_REENTRANT (1)
#define configENABLE_BACKWARD_COMPATIBILITY (1)
#define configSUPPORT_STATIC_ALLOCATION (1)
#define configSUPPORT_DYNAMIC_ALLOCATION (1)

// #ifndef configTOTAL_HEAP_SIZE
#define configTOTAL_HEAP_SIZE ((size_t) (31 * 1024))
// #endif // configTOTAL_HEAP_SIZE

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet (1)
#define INCLUDE_uxTaskPriorityGet (1)
#define INCLUDE_vTaskDelete (1)
#define INCLUDE_vTaskSuspend (1)
#define INCLUDE_xResumeFromISR (1)
#define INCLUDE_vTaskDelayUntil (1)
#define INCLUDE_vTaskDelay (1)
#define INCLUDE_xTaskGetSchedulerState (1)
#define INCLUDE_xTaskGetCurrentTaskHandle (1)
#define INCLUDE_uxTaskGetStackHighWaterMark (1)
#define INCLUDE_xTaskGetIdleTaskHandle (1)
#define INCLUDE_xTimerGetTimerDaemonTaskHandle (1)
#define INCLUDE_pcTaskGetTaskName (1)
#define INCLUDE_eTaskGetState (1)
#define INCLUDE_xEventGroupSetBitFromISR (1)
#define INCLUDE_xEventGroupSetBitsFromISR (1)
#define INCLUDE_xSemaphoreGetMutexHolder (1)
#define INCLUDE_xTimerPendFunctionCall (1)
#define INCLUDE_xTaskGetHandle (1)

/* Stop if an assertion fails. */
#define configASSERT(x)                                                                                                            \
    if ((x) == 0)                                                                                                                  \
    {                                                                                                                              \
        taskDISABLE_INTERRUPTS();                                                                                                  \
        printf("\nFREERTOS ASSERT ( %s ) %s %s %d\n", #x, __FILE__, __func__, __LINE__);                                           \
        for (;;)                                                                                                                   \
            ;                                                                                                                      \
    }
#define configASSERTNULL(x)                                                                                                        \
    if ((x) == NULL)                                                                                                               \
    {                                                                                                                              \
        taskDISABLE_INTERRUPTS();                                                                                                  \
        for (;;)                                                                                                                   \
            ;                                                                                                                      \
    }

typedef enum task_priority
{
    TASK_PRIORITY_IDLE = 0, /* lowest, special for idle task */

    TASK_PRIORITY_APP = (configTIMER_TASK_PRIORITY - 5),
    TASK_PRIORITY_PROTOCOL_LOW,
    TASK_PRIORITY_PROTOCOL_NORMAL,
    TASK_PRIORITY_PROTOCOL_MEDIUM,
    TASK_PRIORITY_PROTOCOL_HIGH,

    /* highest */
    TASK_PRIORITY_SVC = configMAX_PRIORITIES - 1,
} task_priority_t;

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define SVC_Handler vPortSVCHandler
#define PendSV_Handler xPortPendSVHandler
/* Ensure Cortex-M port compatibility. */
#define SysTick_Handler xPortSysTickHandler

/* Thread local storage pointers used by the SDK */
#ifndef configNUM_SDK_THREAD_LOCAL_STORAGE_POINTERS
#define configNUM_SDK_THREAD_LOCAL_STORAGE_POINTERS 0
#endif

#if defined(__GNUC__)
/* For the linker. */
#define fabs __builtin_fabs
#endif

#ifdef __cplusplus
}
#endif
