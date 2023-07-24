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
#include "ColorFormat.h"
#include "FreeRTOS.h"
#include "timers.h" // provides FreeRTOS timer support
#include <app/clusters/on-off-server/on-off-server.h>


#include "init_rt582Platform.h"
#include "init_lighting_rt582Platform.h"  
#include <lib/core/CHIPError.h>

class LightingManager
{
public:
    enum Action_t
    {
        ON_ACTION = 0,
        OFF_ACTION,
        LEVEL_ACTION,
        COLOR_ACTION_XY,
        COLOR_ACTION_HSV,
        COLOR_ACTION_CT,
        INVALID_ACTION
    } Action;

    enum State_t
    {
        kState_On = 0,
        kState_Off,
    } State;

    CHIP_ERROR Init();
    bool IsTurnedOn();
    uint8_t GetLevel();
    RgbColor_t GetRgb();
    bool InitiateAction(Action_t aAction, int32_t aActor, uint16_t size, uint8_t * value);

    using LightingCallback_fn = std::function<void(Action_t)>;

    void SetCallbacks(LightingCallback_fn aActionInitiated_CB, LightingCallback_fn aActionCompleted_CB);


private:
    friend LightingManager & LightMgr(void);
    State_t mState;
    uint8_t mLevel;
    XyColor_t mXY;
    CW_t mCW;
    HsvColor_t mHSV;
    RgbColor_t mRGB;
    CtColor_t mCT;

    LightingCallback_fn mActionInitiated_CB;
    LightingCallback_fn mActionCompleted_CB;

    void Set(bool aOn);
    void SetLevel(uint8_t aLevel);
    void SetColor(uint16_t x, uint16_t y);
    void SetColor(uint8_t hue, uint8_t saturation);
    void SetColorTemperature(CtColor_t ct);

    void UpdateLight();

    static void TimerEventHandler(TimerHandle_t xTimer);
    static LightingManager sLight;
};

inline LightingManager & LightMgr(void)
{
    return LightingManager::sLight;
}
