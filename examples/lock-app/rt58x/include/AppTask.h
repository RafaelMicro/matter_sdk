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

#pragma once

/**********************************************************
 * Includes
 *********************************************************/

#include <stdbool.h>
#include <stdint.h>

#include "AppEvent.h"
#include "BaseApplication.h"
#include "FreeRTOS.h"

#include "BoltLockManager.h"
// #include "sl_simple_button_instances.h"
#include "timers.h" // provides FreeRTOS timer support
#include <app/clusters/identify-server/identify-server.h>
#include <ble/BLEEndPoint.h>
#include <lib/core/CHIPError.h>
#include <platform/CHIPDeviceLayer.h>

#include "bsp.h"
#include "FactoryDataProvider.h"

/**********************************************************
 * Defines
 *********************************************************/

// Application-defined error codes in the CHIP_ERROR space.
#define APP_ERROR_EVENT_QUEUE_FAILED CHIP_APPLICATION_ERROR(0x01)
#define APP_ERROR_CREATE_TASK_FAILED CHIP_APPLICATION_ERROR(0x02)
#define APP_ERROR_UNHANDLED_EVENT CHIP_APPLICATION_ERROR(0x03)
#define APP_ERROR_CREATE_TIMER_FAILED CHIP_APPLICATION_ERROR(0x04)
#define APP_ERROR_START_TIMER_FAILED CHIP_APPLICATION_ERROR(0x05)
#define APP_ERROR_STOP_TIMER_FAILED CHIP_APPLICATION_ERROR(0x06)

/**********************************************************
 * AppTask Declaration
 *********************************************************/

class AppTask
{

public:
    CHIP_ERROR StartAppTask();
    static void AppTaskMain(void * pvParameter);
    static AppTask & GetAppTask() { return sAppTask; }
    void PostEvent(const AppEvent * event);
    void ActionRequest(int32_t aActor, BoltLockManager::Action_t aAction);

private:
    static void UpdateClusterState(intptr_t arg);
    CHIP_ERROR Init();
    static void InitServer(intptr_t arg);
    static void ActionInitiated(BoltLockManager::Action_t aAction, int32_t aActor);
    static void ActionCompleted(BoltLockManager::Action_t aAction);    
    static void OpenCommissioning(intptr_t arg);
    static void ChipEventHandler(const chip::DeviceLayer::ChipDeviceEvent *, intptr_t);
    static void LockActionEventHandler(AppEvent * aEvent);
    static void UpdateStatusLED();
    void DispatchEvent(AppEvent * event);

    static void ButtonEventHandler(bsp_event_t event);

    static void FunctionTimerEventHandler(AppEvent * aEvent);
    static void FunctionHandler(AppEvent * aEvent);

    static void TimerEventHandler(chip::System::Layer * aLayer, void * aAppState);

    void StartTimer(uint32_t aTimeoutMs);
    void CancelTimer(void);

    enum Function_t
    {
        kFunction_NoneSelected   = 0,
        kFunction_FactoryReset   = 1,

        kFunction_Invalid
    } Function;

    Function_t mFunction;
    bool mFunctionTimerActive;
    bool mSyncClusterToButtonAction = false;

    static AppTask sAppTask;   

#if RAFAEL_CERTS_ENABLED
    chip::DeviceLayer::FactoryDataProvider<chip::DeviceLayer::InternalFlashFactoryData> mFactoryDataProvider;
#endif
};