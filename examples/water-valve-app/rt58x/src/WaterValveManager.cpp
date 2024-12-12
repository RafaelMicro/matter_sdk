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

#include "WaterValveManager.h"

#include "AppConfig.h"
#include "AppTask.h"
#include <FreeRTOS.h>

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app-common/zap-generated/cluster-enums.h>
using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::DeviceLayer;
WaterValveManager WaterValveManager::sValve;

CHIP_ERROR WaterValveManager::Init(uint8_t ep)
{
    mEndpoint = ep;
    SetDefaultDelegate(ep, this);

    return CHIP_NO_ERROR;
}
DataModel::Nullable<chip::Percent> WaterValveManager::HandleOpenValve(DataModel::Nullable<chip::Percent> level)
{
    ChipLogProgress(Zcl, "Valve open at level %d%%", level.Value());

    mCurrentLevelPercent = level.Value();
    if(mCurrentLevelPercent > 0)
    {
        gpio_pin_clear(21);
    }
    
    ValveConfigurationAndControl::UpdateCurrentState(mEndpoint, ValveConfigurationAndControl::ValveStateEnum::kOpen);
    return level;
}
CHIP_ERROR WaterValveManager::HandleCloseValve()
{
    ChipLogProgress(Zcl, "Valve closed");

    mCurrentLevelPercent = 0;
    gpio_pin_set(21);

    ValveConfigurationAndControl::UpdateCurrentState(mEndpoint, ValveConfigurationAndControl::ValveStateEnum::kClosed);
    return CHIP_NO_ERROR;
}
void WaterValveManager::HandleRemainingDurationTick(uint32_t duration)
{
    ChipLogProgress(Zcl, "Valve will close in %ds", duration);
}
void WaterValveManager::OpenValve(void)
{
    ChipLogProgress(Zcl, "OpenValve");
    DataModel::Nullable<Percent> openLevel;
    DataModel::Nullable<uint32_t> duration;
    openLevel.SetNonNull(100);
    duration.SetNull();
    PlatformMgr().LockChipStack();
    ValveConfigurationAndControl::SetValveLevel(mEndpoint, openLevel, duration);
    PlatformMgr().UnlockChipStack();
}
void WaterValveManager::CloseValve(void)
{
    ChipLogProgress(Zcl, "CloseValve");
    DataModel::Nullable<Percent> openLevel;
    DataModel::Nullable<uint32_t> duration;
    openLevel.SetNonNull(0);
    duration.SetNull();
    PlatformMgr().LockChipStack();
    ValveConfigurationAndControl::CloseValve(mEndpoint);
    PlatformMgr().UnlockChipStack();
}