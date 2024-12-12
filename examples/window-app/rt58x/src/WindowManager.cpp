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

#include "WindowControl.h"
#include "WindowManager.h"

#include "AppConfig.h"
#include "AppTask.h"
#include <FreeRTOS.h>

#include <app/clusters/window-covering-server/window-covering-server.h>

using namespace chip;
using namespace ::chip::DeviceLayer;

WindowManager WindowManager::sWindow;

CHIP_ERROR WindowManager::Init()
{
    bool currentLedState = kState_On;
    lifi_level = kDefaultLIFILevel;
    tili_level = kDefaultTILILevel;
    mState = currentLedState ? kState_On : kState_Off;

    return CHIP_NO_ERROR;
}

bool WindowManager::InitiateAction(MoveType_t aMoveType, Action_t aAction, int32_t aActor, uint8_t * value)
{
    bool action_initiated = false;
    State_t new_state;

    if (mActionInitiated_CB)
    {
        mActionInitiated_CB(aAction);
    }

    switch (aMoveType)
    {
    case LIFT_TYPE:
        (aAction == LEVEL_ACTION)? SetLIFILevel(*value): SetState(new_state);
        break;
    case TILT_TYPE:
        (aAction == LEVEL_ACTION)? SetTILILevel(*value): SetState(new_state);
        break;
    default:
        break;
    }        

    if (mActionCompleted_CB)
    {
        mActionCompleted_CB(aAction);
    }

    return action_initiated;
}

void WindowManager::UpdateWindow()
{
    ChipLogProgress(NotSpecified, "UpdateWindow Level: LIFT: %d TILT: %d", lifi_level, tili_level);
}
