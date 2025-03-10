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

#include <app/clusters/on-off-server/on-off-server.h>

#include "init_rt58xPlatform.h"
#include "init_device_environment.h"  
#include <lib/core/CHIPError.h>

class SmartPlugManager
{
public:
    enum Action_t
    {
        ON_ACTION = 0,
        OFF_ACTION,
        INVALID_ACTION
    } Action;

    enum State_t
    {
        kState_On = 0,
        kState_Off,
    } State;

    CHIP_ERROR Init();
    bool IsTurnedOn();
    bool InitiateAction(Action_t aAction, int32_t aActor, uint16_t size, uint8_t * value);

    using SmartPlugCallback_fn = std::function<void(Action_t)>;

    void SetCallbacks(SmartPlugCallback_fn aActionInitiated_CB, SmartPlugCallback_fn aActionCompleted_CB);


private:
    friend SmartPlugManager & PlugMgr(void);
    State_t mState;

    SmartPlugCallback_fn mActionInitiated_CB;
    SmartPlugCallback_fn mActionCompleted_CB;

    void Set(bool aOn);

    static void TimerEventHandler(TimerHandle_t xTimer);
    static SmartPlugManager sPlug;
};

inline SmartPlugManager & PlugMgr(void)
{
    return SmartPlugManager::sPlug;
}
