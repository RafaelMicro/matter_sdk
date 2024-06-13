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

#include "SmokeManager.h"
#include "AppConfig.h"
#include "AppEvent.h"
#include "AppTask.h"

#include <app/clusters/smoke-co-alarm-server/smoke-co-alarm-server.h>
/**********************************************************
 * Defines and Constants
 *********************************************************/

using namespace chip;
using namespace ::chip::app::Clusters::SmokeCoAlarm;
using namespace ::chip::DeviceLayer;
constexpr EndpointId kContactEndpoint = 1;

TimerHandle_t sSmokeTimer;
StaticTimer_t sStaticSmokeTimerStruct;


static std::array<ExpressedStateEnum, SmokeCoAlarmServer::kPriorityOrderLength> sPriorityOrder = {
    ExpressedStateEnum::kSmokeAlarm,     ExpressedStateEnum::kInterconnectSmoke, ExpressedStateEnum::kCOAlarm,
    ExpressedStateEnum::kInterconnectCO, ExpressedStateEnum::kHardwareFault,     ExpressedStateEnum::kTesting,
    ExpressedStateEnum::kEndOfService,   ExpressedStateEnum::kBatteryAlert
};

/**********************************************************
 * Variable declarations
 *********************************************************/


SmokeManager SmokeManager::sSmokeManager;

CHIP_ERROR SmokeManager::Init()
{
    ChipLogProgress(NotSpecified, "SmokeManager::Init");
    mStartSelfTesting = false;
    mSmokeAlarmState = AlarmStateEnum::kNormal;
    mCOAlarmState = AlarmStateEnum::kNormal;

    SmokeCoAlarmServer::Instance().SetExpressedStateByPriority(1, sPriorityOrder);

    sSmokeTimer = xTimerCreateStatic("sensorTmr", pdMS_TO_TICKS(10000), false, nullptr, SmokeTimerEventHandler,
                                      &sStaticSmokeTimerStruct);
    if(sSmokeTimer == NULL)
    {
        ChipLogProgress(NotSpecified, "sSmokeTimer timer create failed");
        return CHIP_ERROR_NO_MEMORY;
    }

    return CHIP_NO_ERROR;
}

void SmokeManager::StartSelfTesting()
{
    ChipLogProgress(Zcl, "Start self-testing");
    if(sSmokeTimer)
    {
        SmokeMgr().mStartSelfTesting = true;
        xTimerStart(sSmokeTimer,0);
    }
}

void SmokeManager::SmokeTimerEventHandler(TimerHandle_t xTimer)
{
    SmokeMgr().mStartSelfTesting = false;

    chip::DeviceLayer::PlatformMgr().LockChipStack();
    SmokeCoAlarmServer::Instance().SetTestInProgress(1, false);
    SmokeCoAlarmServer::Instance().SetExpressedStateByPriority(1, sPriorityOrder);
    chip::DeviceLayer::PlatformMgr().UnlockChipStack();
    ChipLogProgress(Zcl, "End self-testing");

}
void SmokeManager::ToggleSmokeState()
{
    mSmokeAlarmState = (mSmokeAlarmState == AlarmStateEnum::kNormal)  ? AlarmStateEnum::kWarning  :
                       (mSmokeAlarmState == AlarmStateEnum::kWarning) ? AlarmStateEnum::kCritical :
                                                                        AlarmStateEnum::kNormal;
    PlatformMgr().LockChipStack();
    Attributes::SmokeState::Set(1, mSmokeAlarmState);
    PlatformMgr().UnlockChipStack();
}

void SmokeManager::ToggleCOState()
{
    mCOAlarmState = (mCOAlarmState == AlarmStateEnum::kNormal)  ? AlarmStateEnum::kWarning  :
                    (mCOAlarmState == AlarmStateEnum::kWarning) ? AlarmStateEnum::kCritical :
                                                                  AlarmStateEnum::kNormal;
    PlatformMgr().LockChipStack();
    Attributes::COState::Set(1, mCOAlarmState);
    PlatformMgr().UnlockChipStack();
}
