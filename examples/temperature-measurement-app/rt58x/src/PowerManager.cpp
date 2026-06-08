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

#include "PowerManager.h"
#include "AppConfig.h"
#include "AppEvent.h"
#include "AppTask.h"

/**********************************************************
 * Defines and Constants
 *********************************************************/

using namespace chip;
using namespace chip::DeviceLayer;
using namespace chip::app::Clusters;

constexpr EndpointId kPowerSourceEndpoint = 1;


/**********************************************************
 * Variable declarations
 *********************************************************/

TimerHandle_t sPowerTimer;
StaticTimer_t sPowerTimerStruct;
PowerManager PowerManager::sPowerManager;

CHIP_ERROR PowerManager::Init()
{
    ChipLogProgress(NotSpecified, "PowerManager::Init");
    /* Init Battery Percentage Remaining, unit: 0.5%. e.g. 200 means 100% */
    PowerSource::Attributes::BatPercentRemaining::Set(kPowerSourceEndpoint, 200);
    /* simulate battery power decrease by 5% every minutes */
    sPowerTimer = xTimerCreateStatic("pow", pdMS_TO_TICKS(600000), true, nullptr, PowerTimerEventHandler,
                                      &sPowerTimerStruct);
    xTimerStart(sPowerTimer, 10);
    return CHIP_NO_ERROR;
}

void PowerManager::PowerTimerEventHandler(TimerHandle_t xTimer)
{
    static uint8_t BatteryPercentRemaining = 200;
    if (BatteryPercentRemaining > 10)
    {
        BatteryPercentRemaining -= 10;
    }
    else
    {
        BatteryPercentRemaining = 200;
    }
    ChipLogProgress(NotSpecified, "Battery power Remaining: %d%%", BatteryPercentRemaining/2);
    PlatformMgr().LockChipStack();
    PowerSource::Attributes::BatPercentRemaining::Set(kPowerSourceEndpoint, BatteryPercentRemaining);
    PlatformMgr().UnlockChipStack();
}
