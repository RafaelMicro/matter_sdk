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
#include <app/util/attribute-storage.h>

#include <app/server/OnboardingCodesUtil.h>
#include <app/server/Server.h>
#include <app/server/Dnssd.h>
#include <app/InteractionModelEngine.h>
#include <data-model-providers/codegen/Instance.h>

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

#define APP_ENABLE_COMMISSIONING_TIMING 1

#if APP_ENABLE_COMMISSIONING_TIMING
#include <inttypes.h>
#include <system/SystemClock.h>
#include <transport/SecureSession.h>
#endif
#include "uart.h"
#include "log.h"
#include "mcu.h"
#include "init_rt58x_platform.h"
#include "init_device_environment.h"

#include "matter_config.h"

using namespace chip;
using namespace ::chip::app;
using namespace chip::TLV;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;
using chip::Protocols::InteractionModel::Status;

#define FACTORY_RESET_TRIGGER_TIMEOUT 6000
#define APP_TASK_STACK_SIZE (4 * 1024)
#define APP_TASK_PRIORITY 2
#define APP_EVENT_QUEUE_SIZE 10

#ifdef CHIP_CONFIG_USE_SUBSCRIPTION_CALLBACKS
SubscriptionCallback mSubscriptionHandler;
#endif // CHIP_CONFIG_USE_SUBSCRIPTION_CALLBACKS

namespace {

bool sIsThreadBLEAdvertising = false;
bool sIsThreadProvisioned    = false;
bool sIsThreadEnabled        = false;
bool sHaveBLEConnections     = false;
bool sSwitchState            = false;
bool sCommissioned           = false;

static TaskHandle_t  sAppTaskHandle;
static QueueHandle_t sAppEventQueue;

static uint8_t       sAppEventQueueBuffer[APP_EVENT_QUEUE_SIZE * sizeof(AppEvent)];
static StaticQueue_t sAppEventQueueStruct;

static StackType_t  appStack[APP_TASK_STACK_SIZE / sizeof(StackType_t)];
static StaticTask_t appTaskStruct;

// NOTE! This key is for test/certification only and should not be available in production devices!
// If CONFIG_CHIP_FACTORY_DATA is enabled, this value is read from the factory data.
static uint8_t sTestEventTriggerEnableKey[TestEventTriggerDelegate::kEnableKeyLength]
    = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

#if APP_ENABLE_COMMISSIONING_TIMING
static chip::System::Clock::Timestamp sCommissioningStartTimestamp;
static bool                           sCommissioningStartValid          = false;
static bool                           sPaseStageInProgress              = false;
static chip::System::Clock::Timestamp sPaseEndTimestamp;
static bool                           sPaseTimingValid                  = false;
static chip::System::Clock::Timestamp sCaseStartTimestamp;
static bool                           sCaseStageInProgress              = false;
static chip::System::Clock::Timestamp sCaseEndTimestamp;
static bool                           sCaseTimingValid                  = false;
static chip::System::Clock::Timestamp sOperationalResumeStartTimestamp;
static bool                           sOperationalResumeTimingValid     = false;
static chip::System::Clock::Timestamp sBootToServerReadyStartTimestamp;
static bool                           sBootToServerReadyTimingValid     = false;
static bool                           sOperationalResumeIndicatorActive = false;
#endif

static DeviceInfoProviderImpl gExampleDeviceInfoProvider;
} // namespace

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

void AppTask::UpdateStatusLED()
{
    if (sCommissioned)
    {
        hosal_gpio_pin_set(20);
    }
    else
    {
        hosal_gpio_pin_clear(20);
    }
}

void AppTask::ChipEventHandler(const ChipDeviceEvent * aEvent, intptr_t /* arg */)
{
    switch (aEvent->Type)
    {
    case DeviceEventType::kCHIPoBLEAdvertisingChange:
        sIsThreadBLEAdvertising = true;
        break;
    case DeviceEventType::kCHIPoBLEConnectionClosed:
    case DeviceEventType::kFailSafeTimerExpired:
        sHaveBLEConnections = false;
        break;
    case DeviceEventType::kThreadStateChange:
        sIsThreadProvisioned = ConnectivityMgr().IsThreadProvisioned();
        sIsThreadEnabled     = ConnectivityMgr().IsThreadEnabled();
        break;
    case DeviceEventType::kThreadConnectivityChange:
        break;

#if APP_ENABLE_COMMISSIONING_TIMING
    case DeviceEventType::kCHIPoBLEConnectionEstablished:
        sHaveBLEConnections          = true;
        sCommissioningStartTimestamp = chip::System::SystemClock().GetMonotonicTimestamp();
        sCommissioningStartValid     = true;
        sPaseStageInProgress         = true;
        sCaseStageInProgress         = false;
        sPaseTimingValid             = false;
        sCaseTimingValid             = false;
        sPaseEndTimestamp            = chip::System::Clock::Timestamp();
        sCaseStartTimestamp          = chip::System::Clock::Timestamp();
        sCaseEndTimestamp            = chip::System::Clock::Timestamp();
        ChipLogProgress(NotSpecified, "Commissioning BLE connection established(PASE stage started) (tick=%" PRIu64 " ms)",
                        static_cast<uint64_t>(sCommissioningStartTimestamp.count()));
        break;
#else
    case DeviceEventType::kCHIPoBLEConnectionEstablished:
        sHaveBLEConnections = true;
        break;
#endif
    case DeviceEventType::kServerReady:
        if (sCommissioned)
        {
            UpdateStatusLED();
#if APP_ENABLE_COMMISSIONING_TIMING
            if (sOperationalResumeTimingValid)
            {
                const auto now            = chip::System::SystemClock().GetMonotonicTimestamp();
                const auto resumeDuration = now - sOperationalResumeStartTimestamp;
                ChipLogProgress(NotSpecified,
                                "Operational resume complete (start=%" PRIu64 " ms, end=%" PRIu64 " ms, duration=%" PRIu64 " ms)",
                                static_cast<uint64_t>(sOperationalResumeStartTimestamp.count()),
                                static_cast<uint64_t>(now.count()), static_cast<uint64_t>(resumeDuration.count()));
                sOperationalResumeTimingValid = false;
                if (sOperationalResumeIndicatorActive)
                {
                    sOperationalResumeIndicatorActive = false;
                }
            }
#endif
        }
#if APP_ENABLE_COMMISSIONING_TIMING
        if (sBootToServerReadyTimingValid)
        {
            const auto now          = chip::System::SystemClock().GetMonotonicTimestamp();
            const auto bootDuration = now - sBootToServerReadyStartTimestamp;
            ChipLogProgress(NotSpecified,
                            "Boot->ServerReady timing (start=%" PRIu64 " ms, end=%" PRIu64 " ms, duration=%" PRIu64 " ms)",
                            static_cast<uint64_t>(sBootToServerReadyStartTimestamp.count()), static_cast<uint64_t>(now.count()),
                            static_cast<uint64_t>(bootDuration.count()));
            sBootToServerReadyTimingValid = false;
        }
#endif
        break;
    case DeviceEventType::kCommissioningComplete:
        sCommissioned = true;
        UpdateStatusLED();
#if APP_ENABLE_COMMISSIONING_TIMING
        if (sCommissioningStartValid)
        {
            const auto completion   = chip::System::SystemClock().GetMonotonicTimestamp();
            const auto duration     = completion - sCommissioningStartTimestamp;
            const auto paseDuration =
                sPaseTimingValid ? (sPaseEndTimestamp - sCommissioningStartTimestamp) : chip::System::Clock::Milliseconds64(0);
            const auto caseDuration =
                sCaseTimingValid ? (sCaseEndTimestamp - sCaseStartTimestamp) : chip::System::Clock::Milliseconds64(0);
            ChipLogProgress(NotSpecified, "===Commissioning Timestamp===");
            ChipLogProgress(NotSpecified,
                            "Commissioning completed (overall start=%" PRIu64 " ms, end=%" PRIu64 " ms, duration=%" PRIu64 " ms)",
                            static_cast<uint64_t>(sCommissioningStartTimestamp.count()), static_cast<uint64_t>(completion.count()),
                            static_cast<uint64_t>(duration.count()));
            ChipLogProgress(NotSpecified, "PASE start=%" PRIu64 " ms, end=%" PRIu64 " ms, duration=%" PRIu64 " ms",
                            static_cast<uint64_t>(sCommissioningStartTimestamp.count()),
                            static_cast<uint64_t>(sPaseTimingValid ? sPaseEndTimestamp.count() : completion.count()),
                            static_cast<uint64_t>(paseDuration.count()));
            ChipLogProgress(NotSpecified, "CASE start=%" PRIu64 " ms, end=%" PRIu64 " ms, duration=%" PRIu64 " ms",
                            static_cast<uint64_t>(sCaseTimingValid ? sCaseStartTimestamp.count() : completion.count()),
                            static_cast<uint64_t>(sCaseTimingValid ? sCaseEndTimestamp.count() : completion.count()),
                            static_cast<uint64_t>(caseDuration.count()));
            ChipLogProgress(NotSpecified, "======");
            sCommissioningStartValid = false;
        }
        sCaseStageInProgress = false;
        sPaseStageInProgress = false;
#endif
        break;
#if APP_ENABLE_COMMISSIONING_TIMING
    case DeviceEventType::kSecureSessionEstablished: {
        const auto sessionType =
            static_cast<chip::Transport::SecureSession::Type>(aEvent->SecureSessionEstablished.SecureSessionType);
        const auto now = chip::System::SystemClock().GetMonotonicTimestamp();
        if (sessionType == chip::Transport::SecureSession::Type::kPASE)
        {
            if (sCommissioningStartValid && sPaseStageInProgress)
            {
                const auto paseDuration = now - sCommissioningStartTimestamp;
                ChipLogProgress(NotSpecified,
                                "PASE session complete (start=%" PRIu64 " ms, end=%" PRIu64 " ms, duration=%" PRIu64 " ms)",
                                static_cast<uint64_t>(sCommissioningStartTimestamp.count()), static_cast<uint64_t>(now.count()),
                                static_cast<uint64_t>(paseDuration.count()));
            }
            sPaseEndTimestamp    = now;
            sPaseTimingValid     = true;
            sPaseStageInProgress = false;
            sCaseStartTimestamp  = now;
            sCaseStageInProgress = true;
            ChipLogProgress(NotSpecified, "CASE stage started (tick=%" PRIu64 " ms)", static_cast<uint64_t>(now.count()));
        }
        else if (sessionType == chip::Transport::SecureSession::Type::kCASE)
        {
            if (sCaseStageInProgress)
            {
                const auto caseDuration = now - sCaseStartTimestamp;
                ChipLogProgress(NotSpecified,
                                "CASE session complete (start=%" PRIu64 " ms, end=%" PRIu64 " ms, duration=%" PRIu64 " ms)",
                                static_cast<uint64_t>(sCaseStartTimestamp.count()), static_cast<uint64_t>(now.count()),
                                static_cast<uint64_t>(caseDuration.count()));
            }
            sCaseEndTimestamp    = now;
            sCaseTimingValid     = true;
            sCaseStageInProgress = false;
        }
        break;
    }
#endif
    default:
        break;
    }
}

static void IdentifyToggleOnOff(uint8_t onoff)
{
    // turn on/off led indicator
    if (onoff)
    {
        hosal_gpio_pin_clear(20);
    }
    else
    {
        hosal_gpio_pin_set(20);
    }
}

static void IdentifyStop(void)
{
    ChipLogProgress(Zcl, "Identify Complete");
    hosal_gpio_pin_set(20);
}

void AppTask::InitServer(intptr_t arg)
{
    CHIP_ERROR err;
    static chip::CommonCaseDeviceServerInitParams initParams;
    (void) initParams.InitializeStaticResourcesBeforeServerInit();
    initParams.dataModelProvider = app::CodegenDataModelProviderInstance(initParams.persistentStorageDelegate);

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
        ChipLogError(NotSpecified, "chip::Server::init failed %s", ErrorStr(err));
    }
#ifdef CHIP_CONFIG_USE_SUBSCRIPTION_CALLBACKS
    // Register ICD subscription callback to match subscription max intervals to its idle time interval
    chip::app::InteractionModelEngine::GetInstance()->RegisterReadHandlerAppCallback(&mSubscriptionHandler);
#endif // CHIP_CONFIG_
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
#if APP_ENABLE_COMMISSIONING_TIMING
        sOperationalResumeStartTimestamp  = chip::System::SystemClock().GetMonotonicTimestamp();
        sOperationalResumeTimingValid     = true;
        sOperationalResumeIndicatorActive = true;
#endif
    }

    err = IdentifyInit(IdentifyToggleOnOff, IdentifyStop);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "IdentifyInit failed");
    }

    err = LightSwitchMgr::GetInstance().Init(1);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "LightSwitchMgr Init failed!");
    }
#if RT58x_OTA_ENABLED
    OTAConfig::Init();
#endif
}

CHIP_ERROR AppTask::Init()
{
    MatterFotaInit();
    CHIP_ERROR err;
    ChipLogProgress(NotSpecified, "Current Software Version: %s", CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING);
    MatterNetworkInit();

    err = ThreadStackMgr().InitThreadStack();
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "ThreadStackMgr().InitThreadStack() failed");
    }

    ChipLogProgress(NotSpecified, "Device Type : 0x%04X", CHIP_DEVICE_CONFIG_DEVICE_TYPE);

#if CONFIG_HOSAL_SOC_IDLE_SLEEP
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
        ChipLogError(NotSpecified, "ThreadStackMgr().StartThreadTask() failed");
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
    int        error;
    hosal_gpio_input_config_t pin_cfg;

    /* gpio0 pin setting */
    pin_cfg.param        = NULL;
    pin_cfg.pin_int_mode = HOSAL_GPIO_PIN_INT_BOTH_EDGE;
    pin_cfg.usr_cb       = (void *) ButtonEventHandler;

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

    sAppEventQueue = xQueueCreateStatic(APP_EVENT_QUEUE_SIZE, sizeof(AppEvent),
                                        sAppEventQueueBuffer, &sAppEventQueueStruct);
    if (sAppEventQueue == nullptr)
    {
        ChipLogError(NotSpecified, "Failed to allocate app event queue");
        return CHIP_ERROR_NO_MEMORY;
    }

    // Start App task.
    sAppTaskHandle = xTaskCreateStatic(AppTaskMain, APP_TASK_NAME,
                                       MATTER_ARRAY_SIZE(appStack), nullptr, 1, appStack, &appTaskStruct);
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
            ChipLogError(NotSpecified, "Failed to post event to app event queue");
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
    if (sAppTask.mFunctionTimerActive && sAppTask.mFunction == kFunction_FactoryReset)
    {
        // Actually trigger Factory Reset
        sAppTask.mFunction            = kFunction_NoneSelected;
        sAppTask.mFunctionTimerActive = false;
        ChipLogProgress(NotSpecified, "Performing Factory Reset");
        sCommissioned = false;
        UpdateStatusLED();
        chip::DeviceLayer::PlatformMgr().ScheduleWork(DoFactoryReset, 0);
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
            AppEvent event;
            event.Type               = AppEvent::kEventType_Button_Func_Pressed;
            event.ButtonEvent.Action = kButtonPushEvent;
            event.Handler            = SwitchActionEventHandler;
            sAppTask.PostEvent(&event);

            sAppTask.mFunction = kFunction_NoneSelected;
        }
        else
        {
            AppEvent event;
            event.Type               = AppEvent::kEventType_Button_Func_Released;
            event.ButtonEvent.Action = kButtonPushEvent;
            event.Handler            = SwitchActionEventHandler;
            sAppTask.PostEvent(&event);
        }
        break;

    default:
        break;
    }
}

void AppTask::SwitchActionEventHandler(AppEvent * aEvent)
{
    if (aEvent->Type == AppEvent::kEventType_Button_Func_Pressed)
    {
        LightSwitchMgr::GetInstance().GenericSwitchOnInitialPress();
    }
    else if (aEvent->Type == AppEvent::kEventType_Button_Func_Released)
    {
        LightSwitchMgr::GetInstance().GenericSwitchOnShortRelease();
    }
}

void AppTask::ButtonEventHandler(uint32_t pin, void * isr_param)
{
    uint32_t pin_status;
    hosal_gpio_pin_get(pin, &pin_status);
    ChipLogProgress(NotSpecified, "ButtonEventHandler pin %ld %ld", pin, pin_status);
    switch (pin)
    {
    case (0):
        {
            AppEvent button_event              = {};
            button_event.Type                  = AppEvent::kEventType_Button;
            button_event.ButtonEvent.ButtonIdx = AppEvent::AppActionTypes::kActionTypes_FactoryReset;
            button_event.ButtonEvent.Action    = (pin_status) ? 0 : 1;
            // Hand off to Functionality handler - depends on duration of press
            button_event.Handler = FunctionHandler;
            xQueueSendFromISR(sAppEventQueue, &button_event, NULL);
        }
        break;
    case (1):
        {
            AppEvent button_event              = {};
            button_event.Type                  = AppEvent::kEventType_Button;
            button_event.ButtonEvent.ButtonIdx = AppEvent::AppActionTypes::kActionTypes_Switch_1;
            button_event.ButtonEvent.Action    = (pin_status) ? 0 : 1;
            button_event.Handler               = FunctionHandler;
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
        return;
    }

    while (true)
    {
        BaseType_t eventReceived = xQueueReceive(sAppEventQueue, &event, portMAX_DELAY);

        while (eventReceived == pdTRUE)
        {
            sAppTask.DispatchEvent(&event);
            eventReceived = xQueueReceive(sAppEventQueue, &event, 0);
        }
    }
}
