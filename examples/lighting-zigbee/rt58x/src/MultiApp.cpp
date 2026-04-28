#include <AppTask.h>
#include "MultiApp.h"
#include "AppConfig.h"
#include "init_rt58x_platform.h"
#include "zigbee_platform.h"
#include "zigbee_zcl_msg_handler.h"
#include "zigbee_api.h"
#include "matter_config.h"
#include <platform/RT58x/BLEManagerImpl.h>

#define BUTTON0_HOLD_MS 6000

MultiApp MultiApp::sInstance;
using namespace chip::DeviceLayer;
using namespace chip::DeviceLayer::Internal;

void MultiApp::Button0HoldTimerCallback(TimerHandle_t xTimer)
{
    ChipLogProgress(NotSpecified, "Trigger factory reset");
    if(GetMultiApp().mCurrentProtocol == MultiApp::CurrentProtocol::kZigbee)
    {
        ZIGBEE_APP_NOTIFY(ZB_APP_EVENT_FACTORY_RESET);
    }
    else if(GetMultiApp().mCurrentProtocol == MultiApp::CurrentProtocol::kMatter)
    {
        PlatformMgr().ScheduleWork(DoFactoryReset, 0);
    }
    else
    {
        efd_env_set_default();
        sys_software_reset();
    }
}
void MultiApp::SendBleMsgToZigbeeApp(uint8_t* buf, uint8_t len)
{
    uint8_t *copy = (uint8_t *)pvPortMalloc(len);
    if (copy != NULL)
    {
        memcpy(copy, buf, len);
        ble_msg_t msg = { copy, len };
        if (xQueueSend(g_ble_msg_queue, &msg, 0) != pdTRUE)
        {
            vPortFree(copy);
            ChipLogError(NotSpecified, "BLE message queue full, dropping message");
            return;
        }
    }
    else
    {
        ChipLogError(NotSpecified, "BLE message: malloc failed");
        return;
    }
    ZIGBEE_APP_NOTIFY(ZB_APP_EVENT_HANDLE_BLE_MESSAGE);
}

void MultiApp::SetCurrentProtocol(CurrentProtocol protocol)
{
    mCurrentProtocol = protocol;
    efd_set_env_blob("prot", &mCurrentProtocol, sizeof(CurrentProtocol));
}
void MultiApp::ButtonIsrHandler(uint32_t pin, void * isr_param)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t pin_state;
    hosal_gpio_pin_get(pin, &pin_state);
    switch (pin)
    {
    case 0:
        if (pin_state == 0) /* pressed */
        {
            if (GetMultiApp().mButton0HoldTimer != NULL && xTimerIsTimerActive(GetMultiApp().mButton0HoldTimer) == pdFALSE)
            {
                log_info("Hold Button 0 for 6 seconds to trigger factory reset");
                xTimerStartFromISR(GetMultiApp().mButton0HoldTimer, &xHigherPriorityTaskWoken);
            }
        }
        else /* released */
        {
            if (GetMultiApp().mButton0HoldTimer != NULL && xTimerIsTimerActive(GetMultiApp().mButton0HoldTimer) == pdTRUE)
            {
                log_info("Factory reset timer stopped");
                xTimerStopFromISR(GetMultiApp().mButton0HoldTimer, &xHigherPriorityTaskWoken);
            }
        }
        break;
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
void MultiApp::SuspendMatterTask(void)
{
    log_info("Deleting Matter task");
    if(mMatterTaskRunning)
    {
        mMatterTaskRunning = 0;
        if (ThreadStackMgr().IsThreadAttached())
        {
            CHIP_ERROR srpErr = ThreadStackMgr().ClearAllSrpHostAndServices();
            if (srpErr != CHIP_NO_ERROR)
            {
                ChipLogError(NotSpecified, "ClearAllSrpHostAndServices failed: %s", ErrorStr(srpErr));
            }
            else
            {
                ChipLogProgress(NotSpecified, "SRP host/services cleared");
            }
        }
        ConnectivityMgr().ErasePersistentInfo();
        PlatformMgr().StopEventLoopTask();
        PlatformMgr().Shutdown();
    }
}
void MultiApp::SuspendZigbeeTask(void)
{
    if(mZigbeeTaskRunning)
    {
        zboss_suspend();
        mZigbeeTaskRunning = 0;
    }
}
void MultiApp::ResumeZigbeeTask(void)
{
    if(!mZigbeeTaskRunning)
    {
        zboss_resume();
        mZigbeeTaskRunning = 1;
    }
}
void MultiApp::Init()
{
    /* Read protocol from flash */
    size_t actual_len;
    efd_get_env_blob("prot", (void *) &mCurrentProtocol, sizeof(CurrentProtocol), &actual_len);
    if(actual_len == 0) {
        log_info("Protocol not found, set to default");
        mCurrentProtocol = CurrentProtocol::kNone;
        efd_set_env_blob("prot", &mCurrentProtocol, sizeof(CurrentProtocol));
    }
    else
    {
        log_info("Protocol: %s", mCurrentProtocol == CurrentProtocol::kNone ? "None" : 
        mCurrentProtocol == CurrentProtocol::kZigbee ? "Zigbee" : "Matter");
    }
    if(mCurrentProtocol == CurrentProtocol::kNone)
    {
        mZigbeeTaskRunning = 1;
        mMatterTaskRunning = 1;
    }
    else if(mCurrentProtocol == CurrentProtocol::kZigbee)
    {
        mZigbeeTaskRunning = 1;
    }
    else if(mCurrentProtocol == CurrentProtocol::kMatter)
    {
        mMatterTaskRunning = 1;
    }
    /* gpio pin setting */
    hosal_gpio_input_config_t pin_cfg;
    pin_cfg.param        = NULL;
    pin_cfg.pin_int_mode = HOSAL_GPIO_PIN_INT_BOTH_EDGE;
    pin_cfg.usr_cb       = (void *) ButtonIsrHandler;

    hosal_gpio_set_debounce_time(DEBOUNCE_SLOWCLOCKS_1024);
    NVIC_SetPriority(Gpio_IRQn, 7);
    NVIC_EnableIRQ(Gpio_IRQn);
    for (uint8_t i = 0; i < 5; i++)
    {
        hosal_pin_set_pullopt(i, HOSAL_PULL_UP_100K);
        hosal_gpio_cfg_input(i, pin_cfg);
        hosal_gpio_debounce_enable(i);
        hosal_gpio_int_enable(i);
    }
    if(mCurrentProtocol == CurrentProtocol::kZigbee)
    {
        CHIP_ERROR err = BLEMgr().Init();
        if (err != CHIP_NO_ERROR)
        {
            ChipLogError(DeviceLayer, "BLEManager initialization failed: %" CHIP_ERROR_FORMAT, err.Format());
        }
    }
    mButton0HoldTimer = xTimerCreate("btn",
                                     pdMS_TO_TICKS(BUTTON0_HOLD_MS),
                                     pdFALSE, /* one-shot */
                                     NULL,
                                     Button0HoldTimerCallback);
    if (mButton0HoldTimer == NULL)
    {
        ChipLogError(NotSpecified, "Failed to create button 0 hold timer");
    }
}
