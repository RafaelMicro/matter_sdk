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

#include "SmartPlugManager.h"

#include "AppConfig.h"
#include "AppTask.h"
#include <FreeRTOS.h>

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Clusters.h>

using namespace chip;
using namespace chip::DeviceLayer;

SmartPlugManager SmartPlugManager::sPlug;

CHIP_ERROR SmartPlugManager::Init()
{
    bool currentLedState;
    // read current on/off value on endpoint one.

    OnOffServer::Instance().getOnOffValue(1, &currentLedState);

    mState = currentLedState ? kState_On : kState_Off;

    return CHIP_NO_ERROR;
}

void SmartPlugManager::SetCallbacks(SmartPlugCallback_fn aActionInitiated_CB, SmartPlugCallback_fn aActionCompleted_CB)
{
    mActionInitiated_CB = aActionInitiated_CB;
    mActionCompleted_CB = aActionCompleted_CB;
}

bool SmartPlugManager::IsTurnedOn()
{
    return mState == kState_On;
}


bool SmartPlugManager::InitiateAction(Action_t aAction, int32_t aActor, uint16_t size, uint8_t * value)
{
    bool action_initiated = false;
    State_t new_state;
    switch (aAction)
    {
    case ON_ACTION:
        ChipLogProgress(NotSpecified, "PlugMgr:ON: %s->ON", mState == kState_On ? "ON" : "OFF");
        break;
    case OFF_ACTION:
        ChipLogProgress(NotSpecified, "PlugMgr:OFF: %s->OFF", mState == kState_On ? "ON" : "OFF");
        break;    
    default:
        ChipLogProgress(NotSpecified, "PlugMgr:Unknown");
        break;
    }

    // Initiate Turn On/Off Action only when the previous one is complete.
    if (mState == kState_Off && aAction == ON_ACTION)
    {
        action_initiated = true;

        mState = kState_On;
    }
    else if (mState == kState_On && aAction == OFF_ACTION)
    {
        action_initiated = true;

        mState = kState_Off;
    }
    if (action_initiated)
    {        
        if (mActionInitiated_CB)
        {
            mActionInitiated_CB(aAction);
        }
        else
        {
            Set(mState == kState_On);
        }

        if (mActionCompleted_CB)
        {
            mActionCompleted_CB(aAction);
        }
    }

    return action_initiated;
}

void SmartPlugManager::Set(bool aOn)
{
    if (aOn)
    {
        mState = kState_On;
    }
    else
    {
        mState = kState_Off;
    }
}
