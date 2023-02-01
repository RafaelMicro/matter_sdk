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

#include <functional>

#include "AppEvent.h"
#include "FreeRTOS.h"
#include "timers.h" // provides FreeRTOS timer support

#include "init_rt582Platform.h"
#include "init_window_rt582Platform.h"  
#include <lib/core/CHIPError.h>

class WindowManager
{
public:
    enum Action_t : uint8_t
    {
        ON_ACTION = 0,
        OFF_ACTION,
        LEVEL_ACTION,
        COLOR_ACTION_XY,
        COLOR_ACTION_HSV,
        COLOR_ACTION_CT,
        INVALID_ACTION
    } Action;

    enum State_t : uint8_t
    {
        kState_On = 0,
        kState_Off,
    } State;

    enum MoveType_t : uint8_t
    {
        LIFT_TYPE = 0,
        TILT_TYPE,
        NONE
    } MoveType;

    CHIP_ERROR Init();
    bool InitiateAction(MoveType_t aMoveType, Action_t aAction, int32_t aActor, uint8_t * value);
    bool IsTurnedOn() { return mState == kState_On; }
    uint8_t GetLIFILevel() { return lifi_level; }
    uint8_t GetTILILevel() { return tili_level; }
    uint8_t GetMinLevel() const { return mMinLevel; }
    uint8_t GetMaxLevel() const { return mMaxLevel; }

    using WindowCallback_fn = std::function<void(Action_t)>;

    void SetCallbacks(WindowCallback_fn aActionInitiated_CB, WindowCallback_fn aActionCompleted_CB){
        mActionInitiated_CB = aActionInitiated_CB;
        mActionCompleted_CB = aActionCompleted_CB;
    }

private:
    friend WindowManager & WindowMgr(void);
    uint8_t mMinLevel = 0;
    uint8_t mMaxLevel = 200;
    // default initialization value for the light level after start
    const uint8_t kDefaultLIFILevel = 200;
    const uint8_t kDefaultTILILevel = 200;
    State_t mState;
    uint8_t lifi_level;
    uint8_t tili_level;
    WindowCallback_fn mActionInitiated_CB;
    WindowCallback_fn mActionCompleted_CB;

    void SetLIFILevel(uint8_t aLevel) { lifi_level = aLevel; }
    void SetTILILevel(uint8_t aLevel) { tili_level = aLevel; }
    void SetState(State_t aOn){ mState = (aOn == kState_On)? kState_On: kState_Off; }

    void UpdateLight();
    static void TimerEventHandler(TimerHandle_t xTimer);
    static WindowManager sWindow;
};

inline WindowManager & WindowMgr(void)
{
    return WindowManager::sWindow;
}
