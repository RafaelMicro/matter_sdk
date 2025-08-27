#include <assert.h>
#include <errno.h>
#include <stdio.h>

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
// #include <openthread-core-config.h>
#include <openthread/cli.h>
#include <openthread/diag.h>
#include <openthread/ncp.h>
#include <openthread/tasklet.h>

// #include "mbedtls/platform.h"
#include "openthread_port.h"

// #include <mbedtls/platform.h>

#include "log.h"

static otInstance* ot_instance = NULL;
ot_system_event_t ot_system_event_var = OT_SYSTEM_EVENT_NONE;

otInstance* otrGetInstance() {
    if(ot_instance == NULL)
    {
        ot_instance = otInstanceInitSingle();
    }
    return ot_instance;
}
//=============================================================================
//                Functions
//=============================================================================
extern int mbedtls_platform_set_calloc_free(void* (*calloc_func)(size_t,
                                                                 size_t),
                                            void (*free_func)(void*));
void otSysInit(int argc, char * argv[])
{
    OT_UNUSED_VARIABLE(argc);
    OT_UNUSED_VARIABLE(argv);
    ot_entropy_init();
    ot_alarmInit();
    ot_radioInit();
    mbedtls_platform_set_calloc_free(pvPortCalloc, vPortFree);
}

bool otSysPseudoResetWasRequested(void)
{
    return false;
}

void otSysDeinit(void) {}

void otSysProcessDrivers(otInstance * aInstance)
{
    ot_system_event_t sevent = OT_SYSTEM_EVENT_NONE;
    OT_GET_NOTIFY(sevent);
    ot_alarmTask(sevent);
    ot_radioTask(sevent);
}

// void otSysEventSignalPending(void)
// {
//     //gpio_pin_toggle(22);
// }
