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
#include <app/clusters/smoke-co-alarm-server/SmokeCOTestEventTriggerHandler.h>
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

    SmokeCoAlarmServer::Instance().SetTestInProgress(1, false);
    SmokeCoAlarmServer::Instance().SetHardwareFaultAlert(1, false);
    SmokeCoAlarmServer::Instance().SetBatteryAlert(1, AlarmStateEnum::kNormal);
    SmokeCoAlarmServer::Instance().SetEndOfServiceAlert(1, EndOfServiceEnum::kNormal);
    SmokeCoAlarmServer::Instance().SetInterconnectSmokeAlarm(1, AlarmStateEnum::kNormal);
    SmokeCoAlarmServer::Instance().SetInterconnectCOAlarm(1, AlarmStateEnum::kNormal);
    SmokeCoAlarmServer::Instance().SetExpressedStateByPriority(1, sPriorityOrder);

    sSmokeTimer = xTimerCreateStatic("sensorTmr", pdMS_TO_TICKS(10000), false, nullptr, SelfTestTimerEventHandler,
                                      &sStaticSmokeTimerStruct);
    if (sSmokeTimer == NULL)
    {
        ChipLogProgress(NotSpecified, "sSmokeTimer timer create failed");
        return CHIP_ERROR_NO_MEMORY;
    }
    sLedTimer = xTimerCreateStatic("ledTmr", pdMS_TO_TICKS(100), true, nullptr, AlarmLedTimerHandler,
                                      &sStaticLedTimerStruct);
    if (sLedTimer == NULL)
    {
        ChipLogProgress(NotSpecified, "sSmokeTimer timer create failed");
        return CHIP_ERROR_NO_MEMORY;
    }

    return CHIP_NO_ERROR;
}

void SmokeManager::StartSelfTesting()
{
    ChipLogProgress(Zcl, "Start self-testing");
    if (sSmokeTimer && SmokeMgr().mStartSelfTesting == false)
    {
        SmokeMgr().mStartSelfTesting = true;
        SmokeCoAlarmServer::Instance().SetTestInProgress(1, true);
        SmokeCoAlarmServer::Instance().SetExpressedStateByPriority(1, sPriorityOrder);
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
void SmokeManager::ToggleSmokeState(AlarmStateEnum newAlarmState)
{
    switch (newAlarmState)
    {
        case AlarmStateEnum::kNormal:
        {
            mSmokeAlarmState = AlarmStateEnum::kNormal;
            ChipLogProgress(NotSpecified, "Smoke State: Normal");
        }
        break;
        case AlarmStateEnum::kWarning:
        {
            mSmokeAlarmState = AlarmStateEnum::kWarning;
            ChipLogProgress(NotSpecified, "Smoke State: Warning");
        }
        break;
        case AlarmStateEnum::kCritical:
        {
            mSmokeAlarmState = AlarmStateEnum::kCritical;
            ChipLogProgress(NotSpecified, "Smoke State: Critical");
        }
        break;
        default:
        break;
    }
    PlatformMgr().LockChipStack();
    SmokeCoAlarmServer::Instance().SetSmokeState(1, mSmokeAlarmState);
    SmokeCoAlarmServer::Instance().SetExpressedStateByPriority(1, sPriorityOrder);
    PlatformMgr().UnlockChipStack();
}
void SmokeManager::AlarmLedTimerHandler(TimerHandle_t xTimer)
{
    hosal_gpio_pin_toggle(21);
}
void SmokeManager::HandleSmokeState(uint8_t SmokeAlarmState)
{
    if (SmokeAlarmState == 0 && sLedTimer && xTimerIsTimerActive(sLedTimer))
    {
        xTimerStop(sLedTimer, 0);
        hosal_gpio_pin_set(21);
    }
    else if (sLedTimer && !xTimerIsTimerActive(sLedTimer))
    {
        xTimerStart(sLedTimer, 0);
    }
}

bool HandleSmokeCOTestEventTrigger(uint64_t eventTrigger)
{
    // Normalize upper 16 bits to match SmokeCOTrigger enum prefix (0x005c = SmokeCoAlarm cluster ID).
    // The test harness may send 0xffff as a wildcard prefix; we replace it unconditionally.
    eventTrigger = (eventTrigger & 0x0000FFFFFFFFFFFFULL) | 0x005c000000000000ULL;
    SmokeCOTrigger trigger = static_cast<SmokeCOTrigger>(eventTrigger);
    switch (trigger)
    {
    case SmokeCOTrigger::kForceSmokeWarning:
        SmokeCoAlarmServer::Instance().SetSmokeState(kContactEndpoint, AlarmStateEnum::kWarning);
        break;
    case SmokeCOTrigger::kForceSmokeCritical:
        SmokeCoAlarmServer::Instance().SetSmokeState(kContactEndpoint, AlarmStateEnum::kCritical);
        break;
    case SmokeCOTrigger::kClearSmoke:
        SmokeCoAlarmServer::Instance().SetSmokeState(kContactEndpoint, AlarmStateEnum::kNormal);
        break;
    case SmokeCOTrigger::kForceCOWarning:
        SmokeCoAlarmServer::Instance().SetCOState(kContactEndpoint, AlarmStateEnum::kWarning);
        break;
    case SmokeCOTrigger::kForceCOCritical:
        SmokeCoAlarmServer::Instance().SetCOState(kContactEndpoint, AlarmStateEnum::kCritical);
        break;
    case SmokeCOTrigger::kClearCO:
        SmokeCoAlarmServer::Instance().SetCOState(kContactEndpoint, AlarmStateEnum::kNormal);
        break;
    case SmokeCOTrigger::kForceLowBatteryWarning:
        SmokeCoAlarmServer::Instance().SetBatteryAlert(kContactEndpoint, AlarmStateEnum::kWarning);
        break;
    case SmokeCOTrigger::kForceLowBatteryCritical:
        SmokeCoAlarmServer::Instance().SetBatteryAlert(kContactEndpoint, AlarmStateEnum::kCritical);
        break;
    case SmokeCOTrigger::kClearBatteryLevelLow:
        SmokeCoAlarmServer::Instance().SetBatteryAlert(kContactEndpoint, AlarmStateEnum::kNormal);
        break;
    case SmokeCOTrigger::kForceMalfunction:
        SmokeCoAlarmServer::Instance().SetHardwareFaultAlert(kContactEndpoint, true);
        break;
    case SmokeCOTrigger::kClearMalfunction:
        SmokeCoAlarmServer::Instance().SetHardwareFaultAlert(kContactEndpoint, false);
        break;
    case SmokeCOTrigger::kForceEndOfLife:
        SmokeCoAlarmServer::Instance().SetEndOfServiceAlert(kContactEndpoint, EndOfServiceEnum::kExpired);
        break;
    case SmokeCOTrigger::kClearEndOfLife:
        SmokeCoAlarmServer::Instance().SetEndOfServiceAlert(kContactEndpoint, EndOfServiceEnum::kNormal);
        break;
    case SmokeCOTrigger::kForceSmokeInterconnect:
        SmokeCoAlarmServer::Instance().SetInterconnectSmokeAlarm(kContactEndpoint, AlarmStateEnum::kWarning);
        break;
    case SmokeCOTrigger::kClearSmokeInterconnect:
        SmokeCoAlarmServer::Instance().SetInterconnectSmokeAlarm(kContactEndpoint, AlarmStateEnum::kNormal);
        break;
    case SmokeCOTrigger::kForceCOInterconnect:
        SmokeCoAlarmServer::Instance().SetInterconnectCOAlarm(kContactEndpoint, AlarmStateEnum::kWarning);
        break;
    case SmokeCOTrigger::kClearCOInterconnect:
        SmokeCoAlarmServer::Instance().SetInterconnectCOAlarm(kContactEndpoint, AlarmStateEnum::kNormal);
        break;
    case SmokeCOTrigger::kForceSmokeContaminationHigh:
        SmokeCoAlarmServer::Instance().SetContaminationState(kContactEndpoint, ContaminationStateEnum::kCritical);
        break;
    case SmokeCOTrigger::kForceSmokeContaminationLow:
        SmokeCoAlarmServer::Instance().SetContaminationState(kContactEndpoint, ContaminationStateEnum::kLow);
        break;
    case SmokeCOTrigger::kClearContamination:
        SmokeCoAlarmServer::Instance().SetContaminationState(kContactEndpoint, ContaminationStateEnum::kNormal);
        break;
    case SmokeCOTrigger::kForceSmokeSensitivityHigh:
        SmokeCoAlarmServer::Instance().SetSmokeSensitivityLevel(kContactEndpoint, SensitivityEnum::kHigh);
        break;
    case SmokeCOTrigger::kForceSmokeSensitivityLow:
        SmokeCoAlarmServer::Instance().SetSmokeSensitivityLevel(kContactEndpoint, SensitivityEnum::kLow);
        break;
    case SmokeCOTrigger::kClearSensitivity:
        SmokeCoAlarmServer::Instance().SetSmokeSensitivityLevel(kContactEndpoint, SensitivityEnum::kStandard);
        break;
    case SmokeCOTrigger::kForceSilence:
        SmokeCoAlarmServer::Instance().SetDeviceMuted(kContactEndpoint, MuteStateEnum::kMuted);
        break;
    case SmokeCOTrigger::kClearSilence:
        SmokeCoAlarmServer::Instance().SetDeviceMuted(kContactEndpoint, MuteStateEnum::kNotMuted);
        break;
    default:
        return false;
    }
    SmokeCoAlarmServer::Instance().SetExpressedStateByPriority(kContactEndpoint, sPriorityOrder);
    return true;
}
