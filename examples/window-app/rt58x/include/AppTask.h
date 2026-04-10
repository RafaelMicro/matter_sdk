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

#include <stdbool.h>
#include <stdint.h>

#include "AppEvent.h"
#include "BaseApplication.h"
#include "EnhancedFlashDataset.h"
#include "FactoryDataProvider.h"
#include "FreeRTOS.h"
#include "WindowManager.h"
#include "hosal_gpio.h"
#include "hosal_sysctrl.h"
#include "timers.h"

#include <app/clusters/identify-server/identify-server.h>
#include <ble/BLEEndPoint.h>
#include <lib/core/CHIPError.h>
#include <platform/CHIPDeviceLayer.h>

class AppTask
{
public:
    CHIP_ERROR StartAppTask();
    static void AppTaskMain(void * pvParameter);

    void PostLightActionRequest(int32_t aActor, WindowManager::Action_t aAction);
    void PostEvent(const AppEvent * event);

private:
    friend AppTask & GetAppTask(void);

    CHIP_ERROR Init();
    static void InitServer(intptr_t arg);
    static void OpenCommissioning(intptr_t arg);
    static void ChipEventHandler(const chip::DeviceLayer::ChipDeviceEvent *, intptr_t);
    static void ActionEventHandler(AppEvent * aEvent);
    static void ActionInitiated(WindowManager::Action_t aAction);
    static void ActionCompleted(WindowManager::Action_t aAction);
    static void UpdateStatusLED();
    static void UpdateClusterState(intptr_t arg);
    void DispatchEvent(AppEvent * event);

    static void ButtonEventHandler(uint32_t pin, void* isr_param) ;

    static void FunctionTimerEventHandler(AppEvent * aEvent);
    static void FunctionHandler(AppEvent * aEvent);

    static void TimerEventHandler(chip::System::Layer * aLayer, void * aAppState);

    void StartTimer(uint32_t aTimeoutMs);
    void CancelTimer(void);

    enum Function_t
    {
        kFunction_NoneSelected = 0,
        kFunction_FactoryReset,
        kFunction_Switch_1,
        kFunction_Switch_2,
        kFunction_Switch_3,
        kFunction_Switch_4,
        kFunction_Invalid
    };

    Function_t mFunction;
    bool mFunctionTimerActive;
    bool mFunctionSwitchActive;
    bool mSyncClusterToButtonAction;

    static AppTask sAppTask;

#if RAFAEL_CERTS_ENABLED
    chip::DeviceLayer::FactoryDataProvider<chip::DeviceLayer::InternalFlashFactoryData> mFactoryDataProvider;
#endif
};

inline AppTask & GetAppTask(void)
{
    return AppTask::sAppTask;
}
