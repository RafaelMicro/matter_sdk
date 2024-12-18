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
TimerHandle_t sLedTimer;
StaticTimer_t sStaticLedTimerStruct;


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

    sSmokeTimer = xTimerCreateStatic("sensorTmr", pdMS_TO_TICKS(10000), false, nullptr, SelfTestTimerEventHandler,
                                      &sStaticSmokeTimerStruct);
    if(sSmokeTimer == NULL)
    {
        ChipLogProgress(NotSpecified, "sSmokeTimer timer create failed");
        return CHIP_ERROR_NO_MEMORY;
    }
    sLedTimer = xTimerCreateStatic("ledTmr", pdMS_TO_TICKS(100), true, nullptr, AlarmLedTimerHandler,
                                      &sStaticLedTimerStruct);
    if(sLedTimer == NULL)
    {
        ChipLogProgress(NotSpecified, "sSmokeTimer timer create failed");
        return CHIP_ERROR_NO_MEMORY;
    }

    return CHIP_NO_ERROR;
}

void SmokeManager::StartSelfTesting()
{
    ChipLogProgress(Zcl, "Start self-testing");
    if(sSmokeTimer && SmokeMgr().mStartSelfTesting == false)
    {
        SmokeMgr().mStartSelfTesting = true;
        xTimerStart(sSmokeTimer,0);
    }
}

void SmokeManager::SelfTestTimerEventHandler(TimerHandle_t xTimer)
{
    SmokeMgr().mStartSelfTesting = false;

    chip::DeviceLayer::PlatformMgr().LockChipStack();
    SmokeCoAlarmServer::Instance().SetTestInProgress(1, false);
    SmokeCoAlarmServer::Instance().SetExpressedStateByPriority(1, sPriorityOrder);
    chip::DeviceLayer::PlatformMgr().UnlockChipStack();
    ChipLogProgress(Zcl, "End self-testing");

}
void SmokeManager::ToggleSmokeState(AlarmStateEnum  AlarmState)
{
    switch(AlarmState)
    {
        case AlarmStateEnum::kNormal:
        {
            mSmokeAlarmState = AlarmStateEnum::kNormal;
            ChipLogProgress(NotSpecified,"Smoke State: Normal");
        }
        break;
        case AlarmStateEnum::kWarning:
        {
            mSmokeAlarmState = AlarmStateEnum::kWarning;
            ChipLogProgress(NotSpecified,"Smoke State: Warning");
        }
        break;
        case AlarmStateEnum::kCritical:
        {
            mSmokeAlarmState = AlarmStateEnum::kCritical;
            ChipLogProgress(NotSpecified,"Smoke State: Critical");
        }
        break;
    }
    PlatformMgr().LockChipStack();
    SmokeCoAlarmServer::Instance().SetSmokeState(1, mSmokeAlarmState);
    SmokeCoAlarmServer::Instance().SetExpressedStateByPriority(1, sPriorityOrder);
    PlatformMgr().UnlockChipStack();
}
void SmokeManager::AlarmLedTimerHandler(TimerHandle_t xTimer)
{
    bsp_led_toggle(BSP_LED_1);
}
void SmokeManager::HandleSmokeState(uint8_t SmokeAlarmState)
{
    if(SmokeAlarmState == 0 && sLedTimer && xTimerIsTimerActive(sLedTimer))
    {
        xTimerStop(sLedTimer, 0);
        bsp_led_Off(BSP_LED_1);
    }
    else if(sLedTimer && !xTimerIsTimerActive(sLedTimer))
    {
        xTimerStart(sLedTimer, 0);
    }
}
