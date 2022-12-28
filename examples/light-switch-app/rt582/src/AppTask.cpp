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

#include <app-common/zap-generated/attribute-id.h>
#include <app-common/zap-generated/attribute-type.h>
#include <app-common/zap-generated/cluster-id.h>
#include <app/clusters/identify-server/identify-server.h>
#include <app/util/attribute-storage.h>

#include <app/server/OnboardingCodesUtil.h>
#include <app/server/Server.h>
#include <app/server/Dnssd.h>

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

#include "bsp.h"
#include "bsp_button.h"
#include "bsp_led.h"


using namespace chip;
using namespace chip::TLV;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;

#define FACTORY_RESET_TRIGGER_TIMEOUT 6000
#define APP_TASK_STACK_SIZE (2 * 1024)
#define APP_TASK_PRIORITY 2
#define APP_EVENT_QUEUE_SIZE 10
#define LIGHT_ENDPOINT_ID (1)

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

// NOTE! This key is for test/certification only and should not be available in production devices!
// If CONFIG_CHIP_FACTORY_DATA is enabled, this value is read from the factory data.
static uint8_t sTestEventTriggerEnableKey[TestEventTriggerDelegate::kEnableKeyLength] 
                = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

static EmberAfIdentifyEffectIdentifier sIdentifyEffect = EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_STOP_EFFECT;
static DeviceInfoProviderImpl gExampleDeviceInfoProvider;
/**********************************************************
 * Identify Callbacks
 *********************************************************/

namespace {
void OnTriggerIdentifyEffectCompleted(chip::System::Layer * systemLayer, void * appState)
{
    ChipLogProgress(Zcl, "Trigger Identify Complete");
    sIdentifyEffect = EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_STOP_EFFECT;
}
} // namespace

void OnTriggerIdentifyEffect(Identify * identify)
{
    sIdentifyEffect = identify->mCurrentEffectIdentifier;

    if (identify->mCurrentEffectIdentifier == EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_CHANNEL_CHANGE)
    {
        ChipLogProgress(Zcl, "IDENTIFY_EFFECT_IDENTIFIER_CHANNEL_CHANGE - Not supported, use effect variant %d",
                        identify->mEffectVariant);
        sIdentifyEffect = static_cast<EmberAfIdentifyEffectIdentifier>(identify->mEffectVariant);
    }

    switch (sIdentifyEffect)
    {
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_BLINK:
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_BREATHE:
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_OKAY:
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Seconds16(5), OnTriggerIdentifyEffectCompleted,
                                                           identify);
        break;
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_FINISH_EFFECT:
        (void) chip::DeviceLayer::SystemLayer().CancelTimer(OnTriggerIdentifyEffectCompleted, identify);
        (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Seconds16(1), OnTriggerIdentifyEffectCompleted,
                                                           identify);
        break;
    case EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_STOP_EFFECT:
        (void) chip::DeviceLayer::SystemLayer().CancelTimer(OnTriggerIdentifyEffectCompleted, identify);
        sIdentifyEffect = EMBER_ZCL_IDENTIFY_EFFECT_IDENTIFIER_STOP_EFFECT;
        break;
    default:
        ChipLogProgress(Zcl, "No identifier effect");
    }
}
Identify gIdentify = {
    chip::EndpointId{ 1 },
    [](Identify *) { ChipLogProgress(Zcl, "onIdentifyStart"); },
    [](Identify *) { ChipLogProgress(Zcl, "onIdentifyStop"); },
    EMBER_ZCL_IDENTIFY_IDENTIFY_TYPE_VISIBLE_LED,
    OnTriggerIdentifyEffect,
};

} // namespace

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
void AppTask::OpenCommissioning(intptr_t arg)
{
    // Enable BLE advertisements
    chip::Server::GetInstance().GetCommissioningWindowManager().OpenBasicCommissioningWindow();
    ChipLogProgress(NotSpecified, "BLE advertising started. Waiting for Pairing.");
}

/**
 * Update cluster status after application level changes
 */
void AppTask::UpdateClusterState(void) {}

void AppTask::ActionInitiated(AppTask::Action_t aAction, int32_t aActor) {}

void AppTask::ActionCompleted(AppTask::Action_t aAction)
{
    if (aAction == AppEvent::kEventType_Button)
    {
        sAppTask.UpdateClusterState();
        sAppTask.mSyncClusterToButtonAction = false;
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
    if(err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "chip::Server::init faild %s", ErrorStr(err));
    }
    if (chip::Server::GetInstance().GetFabricTable().FabricCount() == 0)
    {
        vTaskSuspendAll();
        PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));
        xTaskResumeAll();
    }
}

CHIP_ERROR AppTask::Init()
{
    CHIP_ERROR err;
    ChipLogProgress(NotSpecified, "Current Software Version: %s", CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING);

    bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, ButtonEventHandler);
    bsp_led_init(NULL);
    // Setup light
    err = InitBindingHandler();
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "InitBindingHandler() failed");
        return err;
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR AppTask::StartAppTask()
{
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
#if CONFIG_CHIP_FACTORY_DATA
    ReturnErrorOnFailure(mFactoryDataProvider.Init());
    SetDeviceInstanceInfoProvider(&mFactoryDataProvider);
    SetCommissionableDataProvider(&mFactoryDataProvider);
    SetDeviceAttestationCredentialsProvider(&mFactoryDataProvider);    
#else
    SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());
#endif

    PlatformMgr().ScheduleWork(InitServer, 0);
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
    chip::DeviceLayer::SystemLayer().CancelTimer(TimerEventHandler, this);
    mFunctionTimerActive = false;
}

void AppTask::StartTimer(uint32_t aTimeoutInMs)
{
    CHIP_ERROR err;

    chip::DeviceLayer::SystemLayer().CancelTimer(TimerEventHandler, this);
    err = chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Milliseconds32(aTimeoutInMs), TimerEventHandler, this);
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
                AppEvent event;
                event.Type               = AppEvent::kEventType_Button;
                event.ButtonEvent.Action = kButtonPushEvent;
                event.Handler            = SwitchActionEventHandler;
                sAppTask.PostEvent(&event);

                sAppTask.mFunction = kFunction_NoneSelected;
                sAppTask.mFunctionSwitchActive = false;
            }
        }
        break;
    
    default:
        break;
    }
}

void AppTask::SwitchActionEventHandler(AppEvent * aEvent)
{
    if (aEvent->Type == AppEvent::kEventType_Button)
    {
        BindingCommandData * data = chip::Platform::New<BindingCommandData>();
        data->commandId           = chip::app::Clusters::OnOff::Commands::Toggle::Id;
        data->clusterId           = chip::app::Clusters::OnOff::Id;
        bsp_led_toggle(BSP_LED_0);
        PlatformMgr().ScheduleWork(SwitchWorkerFunction, reinterpret_cast<intptr_t>(data));
    }
}

void AppTask::ButtonEventHandler(bsp_event_t event)
{
    // ChipLogProgress(NotSpecified, "ButtonEventHandler %d %d", (event), bsp_button_state_get(event-5));
    switch (event)
    {
    case (BSP_EVENT_BUTTONS_0):
        {
            AppEvent button_event              = {};
            button_event.Type                  = AppEvent::kEventType_Button;
            button_event.ButtonEvent.ButtonIdx = AppEvent::AppActionTypes::kActionTypes_FactoryReset;
            button_event.ButtonEvent.Action    = bsp_button_state_get(BSP_EVENT_BUTTONS_0-5)?0:1;
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
            button_event.ButtonEvent.Action    = bsp_button_state_get(BSP_EVENT_BUTTONS_1-5)?0:1;
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
    //QueueHandle_t sAppEventQueue = *(static_cast<QueueHandle_t *>(pvParameter));

    CHIP_ERROR err = sAppTask.Init();
    if (err != CHIP_NO_ERROR)
    {
        return ;
    }

    while (true)
    {       
        BaseType_t eventReceived = xQueueReceive(sAppEventQueue, &event, pdMS_TO_TICKS(10));
       
        while (eventReceived == pdTRUE)
        {
            sAppTask.DispatchEvent(&event);
            eventReceived = xQueueReceive(sAppEventQueue, &event, 0);
        }    
    }
}
