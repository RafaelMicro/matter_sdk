#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdio.h>

#include "mcu.h"

extern uint32_t SystemCoreClock;
#define configCPU_CLOCK_HZ (SystemCoreClock)
#define configTICK_RATE_HZ (1000)

#define configKERNEL_INTERRUPT_PRIORITY (224)
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY (64)

#define configUSE_PREEMPTION			1
#define configUSE_IDLE_HOOK				0
#define configUSE_TICK_HOOK				0
#define configUSE_MALLOC_FAILED_HOOK	0
#define configUSE_TICKLESS_IDLE			CONFIG_HOSAL_SOC_IDLE_SLEEP

#define configMAX_PRIORITIES			( 32 )


#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 256 ) /* SIZE-1-1-12-16-30-34>=4 */
#define configMAX_TASK_NAME_LEN			( 16 )
#define configUSE_TRACE_FACILITY		1
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
#define configUSE_16_BIT_TICKS			0
#define configIDLE_SHOULD_YIELD			1
#define configUSE_MUTEXES				1
#define configQUEUE_REGISTRY_SIZE		8
#define configCHECK_FOR_STACK_OVERFLOW	1
#define configUSE_RECURSIVE_MUTEXES		1
#define configUSE_APPLICATION_TASK_TAG	0
#define configUSE_COUNTING_SEMAPHORES	1
#define configGENERATE_RUN_TIME_STATS	0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configSUPPORT_STATIC_ALLOCATION 1


#define configMINIMAL_SECURE_STACK_SIZE                 ( 1024 )

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 			0
#define configMAX_CO_ROUTINE_PRIORITIES ( 1 )

/* Software timer definitions. */
#define configUSE_TIMERS				1
#define configTIMER_TASK_PRIORITY		( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH		10
#define configTIMER_TASK_STACK_DEPTH	( 512 )

#define configENABLE_FPU                    0
#define configENABLE_MPU                    0

#define configENABLE_TRUSTZONE              0

#define configRUN_FREERTOS_SECURE_ONLY      1

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
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

//extern void rt584_utick_set_clear();
//#define Timer_25us_Tick     ((0xFFFFFFFF - (*((volatile uint32_t *)(TIMER32K0+4)))))
//#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()    rt584_utick_set_clear()
//#define portGET_RUN_TIME_COUNTER_VALUE()       Timer_25us_Tick

/* Stop if an assertion fails. */
#define configASSERT(x)                                                                                                            \
    if ((x) == 0)                                                                                                                  \
    {                                                                                                                              \
        printf("\nFREERTOS ASSERT ( %s ) %s %s %d\n", #x, __FILE__, __func__, __LINE__);                                           \
        __disable_irq();                                                                                                           \
        for (;;)                                                                                                                   \
            ;                                                                                                                      \
    }
#define configASSERTNULL(x)                                                                                                        \
    if ((x) == NULL)                                                                                                               \
    {                                                                                                                              \
        for (;;)                                                                                                                   \
    }

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
// #define SVC_Handler vPortSVCHandler 
// #define PendSV_Handler xPortPendSVHandler 
// /* Ensure Cortex-M port compatibility. */
// #define SysTick_Handler xPortSysTickHandler

#if (CONFIG_BUILD_COMPONENT_BLUETOOTH_HCI_BRIDGE == 1)
#ifndef CONFIG_BLUETOOTH_HCI_BRIDGE_TASK_PRIORITY
#define CONFIG_BLUETOOTH_HCI_BRIDGE_TASK_PRIORITY (configMAX_PRIORITIES - 3)
#endif /* CONFIG_BLUETOOTH_HCI_BRIDGE_TASK_PRIORITY */
#endif /* CONFIG_BUILD_COMPONENT_BLUETOOTH_HCI_BRIDGE */

#if (CONFIG_BUILD_COMPONENT_BLUETOOTH_LE_HOST == 1)
#ifndef CONFIG_BLUETOOTH_LE_HOST_TASK_PRIORITY
#define CONFIG_BLUETOOTH_LE_HOST_TASK_PRIORITY (configMAX_PRIORITIES - 3)
#endif /* CONFIG_BLUETOOTH_LE_HOST_TASK_PRIORITY */
#endif /* CONFIG_BUILD_COMPONENT_BLUETOOTH_LE_HOST */

#if (CONFIG_BUILD_COMPONENT_ZIGBEE == 1)
#ifndef CONFIG_ZIGBEE_TASK_PRIORITY
#define CONFIG_ZIGBEE_TASK_PRIORITY (configMAX_PRIORITIES - 3)
#endif /* CONFIG_ZIGBEE_TASK_PRIORITY */
#endif /* CONFIG_BUILD_COMPONENT_ZIGBEE */

#if (CONFIG_BUILD_COMPONENT_OPENTHREAD == 1)
#ifndef CONFIG_OPENTHREAD_TASK_PRIORITY
#define CONFIG_OPENTHREAD_TASK_PRIORITY (configMAX_PRIORITIES - 3)
#endif /* CONFIG_OPENTHREAD_TASK_PRIORITY */
#endif /* CONFIG_BUILD_COMPONENT_OPENTHREAD */

#if (CONFIG_BUILD_COMPONENT_CLI == 1)
#ifndef CONFIG_CLI_TASK_PRIORITY
#define CONFIG_CLI_TASK_PRIORITY (3)
#endif /* CONFIG_CLI_TASK_PRIORITY */
#endif /* CONFIG_BUILD_COMPONENT_CLI */

#ifndef CONFIG_APPLICATION_MAIN_TASK_PRIORITY
#define CONFIG_APPLICATION_MAIN_TASK_PRIORITY (2)
#endif /* CONFIG_APPLICATION_MAIN_TASK_PRIORITY */

typedef enum {
    E_TASK_PRIORITY_MIN = 0,
    E_TASK_PRIORITY_IDLE = 1,
    E_TASK_PRIORITY_APP = CONFIG_APPLICATION_MAIN_TASK_PRIORITY,

#if (CONFIG_BUILD_COMPONENT_CLI == 1)
    E_TASK_PRIORITY_CLI = CONFIG_CLI_TASK_PRIORITY,
#endif /* CONFIG_BUILD_COMPONENT_CLI */
    E_TASK_PRIORITY_LOWEST = 5,

    E_TASK_PRIORITY_NORMAL = 10,

    E_TASK_PRIORITY_HIGHEST = 15,

#if (CONFIG_BUILD_COMPONENT_BLUETOOTH_HCI_BRIDGE == 1)
    E_TASK_PRIORITY_BLUETOOTH_LE_HCI_BRIDGE =
        CONFIG_BLUETOOTH_HCI_BRIDGE_TASK_PRIORITY,
#endif /* CONFIG_BUILD_COMPONENT_BLUETOOTH_HCI_BRIDGE */

#if (CONFIG_BUILD_COMPONENT_BLUETOOTH_LE_HOST == 1)
    E_TASK_PRIORITY_BLUETOOTH_LE_HOST = CONFIG_BLUETOOTH_LE_HOST_TASK_PRIORITY,
#endif /* CONFIG_BUILD_COMPONENT_BLUETOOTH_LE_HOST */

#if (CONFIG_BUILD_COMPONENT_ZIGBEE == 1)
    E_TASK_PRIORITY_ZIGBEE = CONFIG_ZIGBEE_TASK_PRIORITY,
#endif /* CONFIG_BUILD_COMPONENT_ZIGBEE */

#if (CONFIG_BUILD_COMPONENT_OPENTHREAD == 1)
    E_TASK_PRIORITY_OPENTHREAD = CONFIG_OPENTHREAD_TASK_PRIORITY,
#endif /* CONFIG_BUILD_COMPONENT_OPENTHREAD */
    E_TASK_PRIORITY_SW_TIMER = 29,
    E_TASK_PRIORITY_HOSAL = 30,
    E_TASK_PRIORITY_MAX = (configMAX_PRIORITIES - 1),
} e_task_priority;


#ifdef __cplusplus
}
#endif
