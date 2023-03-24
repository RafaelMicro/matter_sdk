#include "sys_arch.h"

/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook(size_t xWantedSize)
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    //malloc_info_printf();
    err("xPortGetFreeHeapSize: %d bytes\r\n", xPortGetFreeHeapSize());
    err("MallocFailed (%d)\n", xWantedSize);
    for (;;);
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(xTaskHandle pxTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;
    err("stack overflow in task %08x %s", (unsigned int)pxTask, (portCHAR *)pcTaskName);
    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for (;;);
}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void)
{
    /* This function will be called by each tick interrupt if
    configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
    added here, but the tick hook is called from an interrupt context, so
    code must not attempt to block, and only the interrupt safe FreeRTOS API
    functions can be used (those that end in FromISR()).  */

#if ( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY == 0 )
    {
        /* In this case the tick hook is used as part of the queue set test. */
        //        vQueueSetAccessQueueSetFromISR();
    }
#endif /* mainCREATE_SIMPLE_BLINKY_DEMO_ONLY */
}
void vAssertCalled(const char *const pcFileName, unsigned long ulLine)
{
    char *current_task_name = (char *)sys_current_thread_name();
    err("assert: [%s] %s:%ld\n", current_task_name, pcFileName, ulLine);
    while (1) {}
}
/*-----------------------------------------------------------*/
