/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "AppTask.h"
#include "AppConfig.h"
#include "AppEvent.h"

#include <OTAConfig.h>

#include <app/clusters/identify-server/identify-server.h>

#include <app/server/OnboardingCodesUtil.h>
#include <app/server/Server.h>
#include <app/server/Dnssd.h>
#include <app/util/attribute-storage.h>
#include <app/InteractionModelEngine.h>

#include <assert.h>
#include <DeviceInfoProviderImpl.h>
#include <setup_payload/QRCodeSetupPayloadGenerator.h>
#include <setup_payload/SetupPayload.h>

#include "queue.h"

#include <lib/support/CodeUtils.h>
#include <platform/CHIPDeviceLayer.h>

#include <platform/CommissionableDataProvider.h>

#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>

#include <lib/core/CHIPError.h>
#include <lib/core/CHIPError.h>

#include "uart.h"
#include "util_log.h"
#include "cm3_mcu.h"
#include "init_rt58xPlatform.h"
#include "init_device_environment.h"
#include "bsp.h"
#include "bsp_button.h"
#include "matter_config.h"
#include "fota_define.h"


using namespace chip;
using namespace ::chip::app;
using namespace chip::TLV;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;
using chip::Protocols::InteractionModel::Status;

#define FACTORY_RESET_TRIGGER_TIMEOUT 6000
#define APP_TASK_STACK_SIZE (2 * 1024)
#define APP_TASK_PRIORITY 2
#define APP_EVENT_QUEUE_SIZE 10
#define LIGHT_ENDPOINT_ID (1)

#ifdef CHIP_CONFIG_USE_SUBSCRIPTION_CALLBACKS
SubscriptionCallback RT58xMatterConfig::mSubscriptionHandler;
#endif // CHIP_CONFIG_USE_SUBSCRIPTION_CALLBACKS

namespace {
bool sIsThreadBLEAdvertising = false;
bool sIsThreadProvisioned = false;
bool sIsThreadEnabled     = false;
bool sHaveBLEConnections  = false;
bool sCommissioned        = false;
static TaskHandle_t sAppTaskHandle;
static QueueHandle_t sAppEventQueue;

static uint8_t sAppEventQueueBuffer[APP_EVENT_QUEUE_SIZE * sizeof(AppEvent)];

static StaticQueue_t sAppEventQueueStruct;

static StackType_t appStack[APP_TASK_STACK_SIZE / sizeof(StackType_t)];
static StaticTask_t appTaskStruct;

static DeviceInfoProviderImpl gExampleDeviceInfoProvider;

// Identify gIdentify = {
//     LIGHT_ENDPOINT_ID,
//     AppTask::IdentifyStartHandler,
//     AppTask::IdentifyStopHandler,
//     EMBER_ZCL_IDENTIFY_IDENTIFY_TYPE_VISIBLE_LIGHT,
// };


Clusters::Identify::EffectIdentifierEnum sIdentifyEffect = Clusters::Identify::EffectIdentifierEnum::kStopEffect;

/**********************************************************
 * Identify Callbacks
 *********************************************************/

void IdentifyToggleOnOff(bool onoff)
{
    //turn on/off led indicator
    if(onoff)
    {
        gpio_pin_clear(21);
    }
    else
    {
        gpio_pin_set(21);
    }
}
void IdentifyChannelMinLevel(void)
{
    gpio_pin_set(21);
}
void OnTriggerIdentifyEffectCompleted(chip::System::Layer * systemLayer, void * appState)
{
    ChipLogProgress(Zcl, "Trigger Identify Complete");
    sIdentifyEffect = Clusters::Identify::EffectIdentifierEnum::kStopEffect;
    gpio_pin_set(21);
}

void OnTriggerIdentifyEffectBlink(chip::System::Layer * systemLayer, void * appState)
{
    Identify * identify = static_cast<Identify *>(appState);
    static uint8_t blink_cnt;
    static bool onoff = true;
    if(sIdentifyEffect == Clusters::Identify::EffectIdentifierEnum::kStopEffect) blink_cnt = 2;
    if(blink_cnt < 2)
    {
        IdentifyToggleOnOff(onoff);
        blink_cnt++;
        onoff =!onoff;
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Milliseconds32(500), OnTriggerIdentifyEffectBlink, identify);        
    }
    else
    {
        blink_cnt = 0;
        onoff = true;
        sIdentifyEffect = Clusters::Identify::EffectIdentifierEnum::kStopEffect;
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Milliseconds32(500), OnTriggerIdentifyEffectCompleted, 0);
    }
}

void OnTriggerIdentifyEffectBreathe(chip::System::Layer * systemLayer, void * appState)
{
    Identify * identify = static_cast<Identify *>(appState);
    static uint8_t breath_cnt;
    static bool onoff = true;
    if(sIdentifyEffect == Clusters::Identify::EffectIdentifierEnum::kStopEffect) breath_cnt = 30;
    if(breath_cnt < 30)
    {
        IdentifyToggleOnOff(onoff);
        onoff =!onoff;
        breath_cnt++;
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Milliseconds32(500), OnTriggerIdentifyEffectBreathe, identify);        
    }
    else
    {
        breath_cnt = 0;
        onoff = true;
        sIdentifyEffect = Clusters::Identify::EffectIdentifierEnum::kStopEffect;
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Milliseconds32(500), OnTriggerIdentifyEffectCompleted, 0);
    }
}

void OnTriggerIdentifyEffectOk(chip::System::Layer * systemLayer, void * appState)
{
    Identify * identify = static_cast<Identify *>(appState);
    static uint8_t okay_cnt;
    static bool onoff = true;
    if(sIdentifyEffect == Clusters::Identify::EffectIdentifierEnum::kStopEffect) okay_cnt = 4;
    if(okay_cnt < 4)
    {
        IdentifyToggleOnOff(onoff);
        onoff =!onoff;
        okay_cnt++;
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Milliseconds32(500), OnTriggerIdentifyEffectOk, identify);        
    }
    else
    {
        okay_cnt = 0;
        onoff = true;
        sIdentifyEffect = Clusters::Identify::EffectIdentifierEnum::kStopEffect;
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Milliseconds32(500), OnTriggerIdentifyEffectCompleted, 0);
    }
}

void OnTriggerIdentifyEffectChannel(chip::System::Layer * systemLayer, void * appState)
{
    Identify * identify = static_cast<Identify *>(appState);
    static uint8_t channel_cnt;
    if(sIdentifyEffect == Clusters::Identify::EffectIdentifierEnum::kStopEffect) channel_cnt = 16;
    if(channel_cnt < 16)
    {
        if(channel_cnt == 0)
        {
            IdentifyToggleOnOff(1);
        }
        else
        {
            IdentifyChannelMinLevel();
        }
        channel_cnt++;
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Milliseconds32(500), OnTriggerIdentifyEffectChannel, identify);        
    }
    else
    {
        channel_cnt = 0;
        sIdentifyEffect = Clusters::Identify::EffectIdentifierEnum::kStopEffect;
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Milliseconds32(500), OnTriggerIdentifyEffectCompleted, 0);
    }    
}

void OnTriggerIdentifyEffect(Identify * identify)
{
    sIdentifyEffect = identify->mCurrentEffectIdentifier;

    switch (sIdentifyEffect)
    {
    case Clusters::Identify::EffectIdentifierEnum::kBlink:
        ChipLogProgress(Zcl, "EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_BLINK");
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Seconds16(1), OnTriggerIdentifyEffectBlink, identify);
        break;
    case Clusters::Identify::EffectIdentifierEnum::kBreathe:
        ChipLogProgress(Zcl, "EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_BREATHE");
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Seconds16(1), OnTriggerIdentifyEffectBreathe, identify);
        break;
    case Clusters::Identify::EffectIdentifierEnum::kOkay:
        ChipLogProgress(Zcl, "EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_OKAY");
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Seconds16(1), OnTriggerIdentifyEffectOk, identify);
        break;
    case Clusters::Identify::EffectIdentifierEnum::kFinishEffect:
        ChipLogProgress(Zcl, "EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_FINISH_EFFECT");
        sIdentifyEffect = Clusters::Identify::EffectIdentifierEnum::kStopEffect;
        break;
    case Clusters::Identify::EffectIdentifierEnum::kChannelChange:
        ChipLogProgress(Zcl, "EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_CHANNEL_CHANGE");
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Seconds16(1), OnTriggerIdentifyEffectChannel, identify);
        break;
    case Clusters::Identify::EffectIdentifierEnum::kStopEffect:
        ChipLogProgress(Zcl, "EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_STOP_EFFECT");
        sIdentifyEffect = Clusters::Identify::EffectIdentifierEnum::kStopEffect;
        break;    
    default:
        ChipLogProgress(Zcl, "No identifier effect");
    }
}

Identify gIdentify = {
    LIGHT_ENDPOINT_ID,
    AppTask::IdentifyStartHandler,
    AppTask::IdentifyStopHandler,
    Clusters::Identify::IdentifyTypeEnum::kVisibleIndicator,
    OnTriggerIdentifyEffect,
};

} //namespace

constexpr EndpointId kNetworkCommissioningEndpointSecondary = 0xFFFE;
using namespace chip::TLV;
using namespace ::chip::DeviceLayer;
AppTask AppTask::sAppTask;

void LockOpenThreadTask(void)
{
    chip::DeviceLayer::ThreadStackMgr().LockThreadStack();
}

void UnlockOpenThreadTask(void)
{
    chip::DeviceLayer::ThreadStackMgr().UnlockThreadStack();
}

void MatterFotaInit(void)
{
    fota_information_t  *p_fota_info = (fota_information_t *)(FOTA_UPDATE_BANK_INFO_ADDRESS);

    if (p_fota_info->fotabank_ready == FOTA_IMAGE_READY)
    { 
        if (p_fota_info->fota_result == FOTA_RESULT_SUCCESS)
        {
            // err("sw ver: %d\r\n", p_fota_info->reserved[0]);
            chip::DeviceLayer::ConfigurationMgr().StoreSoftwareVersion(p_fota_info->reserved[0]);
        } 
        else
        {
            err("fota result: %d\r\n", p_fota_info->fota_result);
        }  
        while (flash_check_busy());
        taskENTER_CRITICAL();
        flash_erase(FLASH_ERASE_SECTOR, FOTA_UPDATE_BANK_INFO_ADDRESS);
        taskEXIT_CRITICAL();
    }
}
void AppTask::IdentifyStartHandler(Identify *)
{
    AppEvent event;
    event.Type               = AppEvent::kEventType_Identify_Start;
    event.Handler            = IdentifyHandleOp;
    sAppTask.PostEvent(&event);
}

void AppTask::IdentifyStopHandler(Identify *)
{
    AppEvent event;
    event.Type               = AppEvent::kEventType_Identify_Stop;
    event.Handler            = IdentifyHandleOp;
    sAppTask.PostEvent(&event);
}

void AppTask::PostAppIdentify()
{
    AppEvent event;
    event.Type               = AppEvent::kEventType_Identify_Identify;
    event.Handler            = IdentifyHandleOp;
    sAppTask.PostEvent(&event);    
}

void AppTask::IdentifyHandleOp(AppEvent * aEvent)
{
    static uint32_t identifyState = 0;
    static bool identify_onoff = 0;

    // ChipLogProgress(NotSpecified, "identify effect = %x", aEvent->Type);

    if (aEvent->Type == AppEvent::kEventType_Identify_Start)
    {
        identifyState = 1;
        identify_onoff = 0;
    }

    else if (aEvent->Type == AppEvent::kEventType_Identify_Identify && identifyState)
    {
        identify_onoff = !identify_onoff;
        IdentifyToggleOnOff(identify_onoff);
    }

    else if (aEvent->Type == AppEvent::kEventType_Identify_Stop)
    {
        identifyState = 0;
        IdentifyToggleOnOff(0);
        ChipLogProgress(NotSpecified, "identify stop");
    }
}

void AppTask::InitServer(intptr_t arg)
{
    CHIP_ERROR err;
    static chip::CommonCaseDeviceServerInitParams initParams;
    (void) initParams.InitializeStaticResourcesBeforeServerInit();
    gExampleDeviceInfoProvider.SetStorageDelegate(initParams.persistentStorageDelegate);
    SetDeviceInfoProvider(&gExampleDeviceInfoProvider);

    chip::Inet::EndPointStateOpenThread::OpenThreadEndpointInitParam nativeParams;
    nativeParams.lockCb                = LockOpenThreadTask;
    nativeParams.unlockCb              = UnlockOpenThreadTask;
    nativeParams.openThreadInstancePtr = chip::DeviceLayer::ThreadStackMgrImpl().OTInstance();
    initParams.endpointNativeParams    = static_cast<void *>(&nativeParams);

    err = chip::Server::GetInstance().Init(initParams);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "chip::Server::init faild %s", ErrorStr(err));
    }

#ifdef CHIP_CONFIG_USE_SUBSCRIPTION_CALLBACKS
    // Register ICD subscription callback to match subscription max intervals to its idle time interval
    chip::app::InteractionModelEngine::GetInstance()->RegisterReadHandlerAppCallback(&RT58xMatterConfig::mSubscriptionHandler);
#endif // CHIP_CONFIG_USE_SUBSCRIPTION_CALLBACKS

    if (chip::Server::GetInstance().GetFabricTable().FabricCount() == 0)
    {
        vTaskSuspendAll();
        PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));
        xTaskResumeAll();
    }
    else
    {
        chip::app::DnssdServer::Instance().StartServer();   
        sCommissioned = true;
    }

    // Setup light
    err = ValveMgr().Init(1);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "ValveMgr().Init() failed");
    }


#if RT58x_OTA_ENABLED
    OTAConfig::Init();
#endif
}

void AppTask::UpdateStatusLED()
{
    if (sCommissioned)
    {
        gpio_pin_set(20);
    }
    else
    {
        gpio_pin_clear(20);
    }
}

void AppTask::ChipEventHandler(const ChipDeviceEvent * aEvent, intptr_t /* arg */)
{
    //ChipLogProgress(NotSpecified, "ChipEventHandler: %x", aEvent->Type);
    switch (aEvent->Type)
    {
    case DeviceEventType::kCHIPoBLEAdvertisingChange:
        sIsThreadBLEAdvertising = true;
        UpdateStatusLED();   
        break;
    case DeviceEventType::kCHIPoBLEConnectionClosed:
    case DeviceEventType::kFailSafeTimerExpired:
        sHaveBLEConnections = false;
        UpdateStatusLED();
        break;
    case DeviceEventType::kThreadStateChange:
        sIsThreadProvisioned = ConnectivityMgr().IsThreadProvisioned();
        sIsThreadEnabled     = ConnectivityMgr().IsThreadEnabled();
        UpdateStatusLED();    
        break;
    case DeviceEventType::kThreadConnectivityChange:
        break;

    case DeviceEventType::kCHIPoBLEConnectionEstablished:
        sHaveBLEConnections = true;
        UpdateStatusLED(); 
        break;

    case DeviceEventType::kCommissioningComplete:
        sCommissioned = true;
        UpdateStatusLED();
        break;
    default:
        break;
    }
}

CHIP_ERROR AppTask::Init()
{
    MatterFotaInit();
    CHIP_ERROR err;
    // ChipLogProgress(NotSpecified, "Current Software Version: %s", CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING);
    
    // chip::DeviceLayer::ConfigurationMgr().StoreSoftwareVersion(CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION);

    err = ThreadStackMgr().InitThreadStack();
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "ThreadStackMgr().InitThreadStack() failed");
    }

    ChipLogError(NotSpecified, "Device Type : 0x%04X", CHIP_DEVICE_CONFIG_DEVICE_TYPE);

#if CHIP_CONFIG_ENABLE_ICD_SERVER
    err = ConnectivityMgr().SetThreadDeviceType(ConnectivityManager::kThreadDeviceType_SleepyEndDevice);
#else
    err = ConnectivityMgr().SetThreadDeviceType(ConnectivityManager::kThreadDeviceType_Router);
#endif
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "ConnectivityMgr().SetThreadDeviceType() failed");
    }

    ChipLogError(NotSpecified, "Start Thread Task\n");
    err = ThreadStackMgr().StartThreadTask();
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "ThreadStackMgr().InitThreadStack() failed");
    }

    if (PlatformMgr().StartEventLoopTask() != CHIP_NO_ERROR)
    {
       ChipLogError(NotSpecified, "Error during PlatformMgr().StartEventLoopTask();");
    }
    PlatformMgr().ScheduleWork(InitServer, 0);
    PlatformMgr().AddEventHandler(ChipEventHandler, 0);
    UpdateStatusLED();

    return CHIP_NO_ERROR;
}

CHIP_ERROR AppTask::StartAppTask()
{
    CHIP_ERROR err;

    bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, ButtonEventHandler);

    sAppEventQueue = xQueueCreateStatic(APP_EVENT_QUEUE_SIZE, sizeof(AppEvent), 
                                    sAppEventQueueBuffer, &sAppEventQueueStruct);
    if (sAppEventQueue == nullptr)
    {
        ChipLogError(NotSpecified, "Failed to allocate app event queue");
        return CHIP_ERROR_NO_MEMORY;
    }

    // Start App task.
    sAppTaskHandle = xTaskCreateStatic(AppTaskMain, APP_TASK_NAME, 
                                    ArraySize(appStack), nullptr, 1, appStack, &appTaskStruct);
    if (sAppTaskHandle == nullptr)
    {
        return CHIP_ERROR_NO_MEMORY;
    }

#if RAFAEL_CERTS_ENABLED
    ReturnErrorOnFailure(mFactoryDataProvider.Init());
    SetDeviceInstanceInfoProvider(&mFactoryDataProvider);
    SetCommissionableDataProvider(&mFactoryDataProvider);
    SetDeviceAttestationCredentialsProvider(&mFactoryDataProvider);    
#else
    SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());
#endif

    return CHIP_NO_ERROR;
}

void AppTask::TimerEventHandler(chip::System::Layer * aLayer, void * aAppState)
{
    AppEvent event;
    event.Type               = AppEvent::kEventType_Timer;
    event.TimerEvent.Context = aAppState;
    event.Handler            = FunctionTimerEventHandler;
    sAppTask.PostEvent(&event);
}


void AppTask::CancelTimer()
{
    PlatformMgr().LockChipStack();
    chip::DeviceLayer::SystemLayer().CancelTimer(TimerEventHandler, this);
    PlatformMgr().UnlockChipStack();
    mFunctionTimerActive = false;
}

void AppTask::StartTimer(uint32_t aTimeoutInMs)
{
    CHIP_ERROR err;

    //chip::DeviceLayer::SystemLayer().CancelTimer(TimerEventHandler, this);
    PlatformMgr().LockChipStack();
    err = chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Milliseconds32(aTimeoutInMs), TimerEventHandler, this);
    PlatformMgr().UnlockChipStack();
    SuccessOrExit(err);

    mFunctionTimerActive = true;
exit:
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "StartTimer failed %s: ", chip::ErrorStr(err));
    }
}

void AppTask::PostEvent(const AppEvent * aEvent)
{
    if (sAppEventQueue != NULL)
    {
        if (!xQueueSend(sAppEventQueue, aEvent, 1))
        {
        }
    }
}

void AppTask::DispatchEvent(AppEvent * aEvent)
{
    if (aEvent->Handler)
    {
        aEvent->Handler(aEvent);
    }
    else
    {
        ChipLogError(NotSpecified, "Event received with no handler. Dropping event.");
    }
}

void AppTask::FunctionTimerEventHandler(AppEvent * aEvent)
{
    if (aEvent->Type != AppEvent::kEventType_Timer)
    {
        return;
    }

    // If we reached here, the button was held past FACTORY_RESET_TRIGGER_TIMEOUT,
    // initiate factory reset
    else if (sAppTask.mFunctionTimerActive && sAppTask.mFunction == kFunction_FactoryReset)
    {
        // Actually trigger Factory Reset
        sAppTask.mFunction = kFunction_NoneSelected;
        chip::Server::GetInstance().ScheduleFactoryReset();
    }
}

void AppTask::FunctionHandler(AppEvent * aEvent)
{
    switch (aEvent->ButtonEvent.ButtonIdx)
    {
    case (AppEvent::AppActionTypes::kActionTypes_FactoryReset):
        if (aEvent->ButtonEvent.Action == true)
        {
            if (!sAppTask.mFunctionTimerActive && sAppTask.mFunction == kFunction_NoneSelected)
            {
                ChipLogProgress(NotSpecified, "[BTN] Hold to select function:");
                ChipLogProgress(NotSpecified, "[BTN] - Factory Reset (>6s)");

                sAppTask.StartTimer(FACTORY_RESET_TRIGGER_TIMEOUT);

                sAppTask.mFunction = kFunction_FactoryReset;
            }
        }
        else
        {
            if (sAppTask.mFunctionTimerActive && sAppTask.mFunction == kFunction_FactoryReset)
            {
                sAppTask.CancelTimer();

                // Change the function to none selected since factory reset has been
                // canceled.
                sAppTask.mFunction = kFunction_NoneSelected;

                ChipLogProgress(NotSpecified, "[BTN] Factory Reset has been Canceled");
            }
        }
        break;
    case (AppEvent::AppActionTypes::kActionTypes_Switch_1):
        if (aEvent->ButtonEvent.Action == true)
        {
            if (!sAppTask.mFunctionSwitchActive && sAppTask.mFunction == kFunction_NoneSelected)
            {
                sAppTask.mFunction = kFunction_Switch_1;
                sAppTask.mFunctionSwitchActive = true;
            }
        }
        else
        {
            if (sAppTask.mFunctionSwitchActive && sAppTask.mFunction == kFunction_Switch_1)
            {
                ValveMgr().OpenValve();
                sAppTask.mFunction = kFunction_NoneSelected;
                sAppTask.mFunctionSwitchActive = false;                
            }
        }
        break;
    case (AppEvent::AppActionTypes::kActionTypes_Switch_2):
        if (aEvent->ButtonEvent.Action == true)
        {
            if (!sAppTask.mFunctionSwitchActive && sAppTask.mFunction == kFunction_NoneSelected)
            {
                sAppTask.mFunction = kFunction_Switch_2;
                sAppTask.mFunctionSwitchActive = true;
            }
        }
        else
        {
            if (sAppTask.mFunctionSwitchActive && sAppTask.mFunction == kFunction_Switch_2)
            {
                ValveMgr().CloseValve();
                sAppTask.mFunction = kFunction_NoneSelected;
                sAppTask.mFunctionSwitchActive = false;
            }
        }
        break;
    default:
        break;
    }
}

void AppTask::ButtonEventHandler(bsp_event_t event)
{
    ChipLogProgress(NotSpecified, "ButtonEventHandler %d %d", (event), bsp_button_state_get(event-5));
    switch (event)
    {
    case (BSP_EVENT_BUTTONS_0):
        {
            AppEvent button_event              = {};
            button_event.Type                  = AppEvent::kEventType_Button;
            button_event.ButtonEvent.ButtonIdx = AppEvent::AppActionTypes::kActionTypes_FactoryReset;
            button_event.ButtonEvent.Action    = bsp_button_state_get(BSP_BUTTON_0)?0:1;
            // Hand off to Functionality handler - depends on duration of press
            button_event.Handler = FunctionHandler;
            xQueueSendFromISR(sAppEventQueue, &button_event, NULL);
        }
        break;
    case (BSP_EVENT_BUTTONS_1):
        {
            AppEvent button_event              = {};
            button_event.Type                  = AppEvent::kEventType_Button;
            button_event.ButtonEvent.ButtonIdx = AppEvent::AppActionTypes::kActionTypes_Switch_1;
            button_event.ButtonEvent.Action    = bsp_button_state_get(BSP_BUTTON_1)?0:1;
            // Hand off to Functionality handler - depends on duration of press
            button_event.Handler = FunctionHandler;
            xQueueSendFromISR(sAppEventQueue, &button_event, NULL);
        }
        break;
    case (BSP_EVENT_BUTTONS_2):
        {
            AppEvent button_event              = {};
            button_event.Type                  = AppEvent::kEventType_Button;
            button_event.ButtonEvent.ButtonIdx = AppEvent::AppActionTypes::kActionTypes_Switch_2;
            button_event.ButtonEvent.Action    = bsp_button_state_get(BSP_BUTTON_2)?0:1;
            // Hand off to Functionality handler - depends on duration of press
            button_event.Handler = FunctionHandler;
            xQueueSendFromISR(sAppEventQueue, &button_event, NULL);
        }
        break;
    default:
        break;
    }
}


void AppTask::AppTaskMain(void * pvParameter)
{
    AppEvent event;
    CHIP_ERROR err = sAppTask.Init();
    if (err != CHIP_NO_ERROR)
    {
        return ;
    }

    while (true)
    {       
        BaseType_t eventReceived = xQueueReceive(sAppEventQueue, &event, portMAX_DELAY);
       
        while (eventReceived == pdTRUE)
        {;
            sAppTask.DispatchEvent(&event);
            eventReceived = xQueueReceive(sAppEventQueue, &event, 0);
        }
    }
}
