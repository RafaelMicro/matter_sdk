/*
 *
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

#include "FreeRTOS.h"
#include "timers.h" // provides FreeRTOS timer support
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/cluster-objects.h>
#include <lib/core/CHIPError.h>
#include "bsp_led.h"
using namespace ::chip::app::Clusters::SmokeCoAlarm;

class SmokeManager
{
public:
    CHIP_ERROR Init();
    void ToggleSmokeState(AlarmStateEnum  AlarmState);
    void HandleSmokeState(uint8_t SmokeAlarmState);
    void HandleOnoffValue(uint8_t onoff);

    void StartSelfTesting();

    enum AlarmState: uint8_t
    {
        kAlarmStateNormal = 0,
        kAlarmStateWarning,
        kAlarmStateCritical,
    };
private:
    bool mStartSelfTesting;
    AlarmStateEnum mSmokeAlarmState;
    AlarmStateEnum mCOAlarmState;
    friend SmokeManager & SmokeMgr();
    static void SelfTestTimerEventHandler(TimerHandle_t xTimer);
    static void AlarmLedTimerHandler(TimerHandle_t xTimer);

    static SmokeManager sSmokeManager;
};

inline SmokeManager & SmokeMgr()
{
    return SmokeManager::sSmokeManager;
}
