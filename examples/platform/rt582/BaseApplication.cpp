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

/**********************************************************
 * Includes
 *********************************************************/

#include "AppConfig.h"
// #include "AppEvent.h"
#include "AppTask.h"
// #include "LEDWidget.h"

#ifdef DISPLAY_ENABLED
#include "lcd.h"
#ifdef QR_CODE_ENABLED
#include "qrcodegen.h"
#endif // QR_CODE_ENABLED
#endif // DISPLAY_ENABLED

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/attribute-type.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/server/OnboardingCodesUtil.h>
#include <app/server/Server.h>
#include <app/util/attribute-storage.h>
#include <assert.h>
#include <lib/support/CodeUtils.h>
#include <platform/CHIPDeviceLayer.h>
// #include <platform/RT582/freertos_bluetooth.h>
#include <setup_payload/QRCodeSetupPayloadGenerator.h>
#include <setup_payload/SetupPayload.h>

#if CHIP_ENABLE_OPENTHREAD
#include <platform/RT582/ThreadStackManagerImpl.h>
#include <platform/OpenThread/OpenThreadUtils.h>
#include <platform/ThreadStackManager.h>
#endif // CHIP_ENABLE_OPENTHREAD


#include "util_log.h"

/**********************************************************
 * Defines and Constants
 *********************************************************/

#define FACTORY_RESET_TRIGGER_TIMEOUT 3000
#define FACTORY_RESET_CANCEL_WINDOW_TIMEOUT 3000
#ifndef APP_TASK_STACK_SIZE
#define APP_TASK_STACK_SIZE (4096)
#endif
#define APP_TASK_PRIORITY 2
#define APP_EVENT_QUEUE_SIZE 10
#define EXAMPLE_VENDOR_ID 0xcafe

// #define SYSTEM_STATE_LED &sl_led_led0

// #define APP_FUNCTION_BUTTON &sl_button_btn0

using namespace chip;
using namespace ::chip::app;
using namespace ::chip::DeviceLayer;

namespace {

/**********************************************************
 * Variable declarations
 *********************************************************/

TimerHandle_t sFunctionTimer; // FreeRTOS app sw timer.
TimerHandle_t sLightTimer;

TaskHandle_t sAppTaskHandle;
QueueHandle_t sAppEventQueue;


#if !(defined(CHIP_DEVICE_CONFIG_ENABLE_SED) && CHIP_DEVICE_CONFIG_ENABLE_SED)

bool sIsProvisioned      = false;
bool sIsEnabled          = false;
bool sIsAttached         = false;
bool sHaveBLEConnections = false;

#endif // CHIP_DEVICE_CONFIG_ENABLE_SED

Clusters::Identify::EffectIdentifierEnum sIdentifyEffect = Clusters::Identify::EffectIdentifierEnum::kStopEffect;

uint8_t sAppEventQueueBuffer[APP_EVENT_QUEUE_SIZE * sizeof(AppEvent)];
StaticQueue_t sAppEventQueueStruct;

StackType_t appStack[APP_TASK_STACK_SIZE / sizeof(StackType_t)];
StaticTask_t appTaskStruct;

BaseApplication::Function_t mFunction;
bool mFunctionTimerActive;

Identify * gIdentifyptr = nullptr;

} // namespace

/**********************************************************
 * AppTask Definitions
 *********************************************************/

CHIP_ERROR BaseApplication::StartAppTask(TaskFunction_t taskFunction)
{

    info("BaseApplication::StartAppTask\n");
    sAppEventQueue = xQueueCreateStatic(
                    APP_EVENT_QUEUE_SIZE, 
                    sizeof(AppEvent), 
                    sAppEventQueueBuffer, 
                    &sAppEventQueueStruct);
    if (sAppEventQueue == NULL)
    {
        err("Failed to allocate app event queue");
        //appError(APP_ERROR_EVENT_QUEUE_FAILED);
    }

    // Start App task.
    sAppTaskHandle = xTaskCreateStatic(
                    taskFunction, 
                    APP_TASK_NAME, 
                    ArraySize(appStack), 
                    &sAppEventQueue, 
                    1, 
                    appStack, 
                    &appTaskStruct);
    if (sAppTaskHandle == nullptr)
    {
        err("Failed to create app task");
        //appError(APP_ERROR_CREATE_TASK_FAILED);
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR BaseApplication::Init(Identify * identifyObj)
{
    PrintOnboardingCodes(chip::RendezvousInformationFlag(chip::RendezvousInformationFlag::kBLE));

    return CHIP_NO_ERROR;
}

void BaseApplication::FunctionTimerEventHandler(TimerHandle_t xTimer)
{
    AppEvent event;
    event.Type               = AppEvent::kEventType_Timer;
    event.TimerEvent.Context = (void *) xTimer;
    event.Handler            = FunctionEventHandler;
    PostEvent(&event);
}

void BaseApplication::FunctionEventHandler(AppEvent * aEvent)
{
    
}

void BaseApplication::LightEventHandler()
{
    
}

void BaseApplication::ButtonHandler(AppEvent * aEvent)
{
}

void BaseApplication::CancelFunctionTimer()
{
}

void BaseApplication::StartFunctionTimer(uint32_t aTimeoutInMs)
{
}

void BaseApplication::StartStatusLEDTimer()
{
}

void BaseApplication::StopStatusLEDTimer()
{
    
}

void BaseApplication::LightTimerEventHandler(TimerHandle_t xTimer)
{
}

#ifdef DISPLAY_ENABLED
SilabsLCD & BaseApplication::GetLCD(void)
{
}
#endif

void BaseApplication::PostEvent(const AppEvent * aEvent)
{
}

void BaseApplication::DispatchEvent(AppEvent * aEvent)
{

}
