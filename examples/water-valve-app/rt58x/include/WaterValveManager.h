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

#include <app/data-model/Nullable.h>
#include <app/clusters/valve-configuration-and-control-server/valve-configuration-and-control-delegate.h>
#include <app/clusters/valve-configuration-and-control-server/valve-configuration-and-control-server.h>

#include "init_rt58xPlatform.h"
#include "init_device_environment.h"  
#include <lib/core/CHIPError.h>

class WaterValveManager : public chip::app::Clusters::ValveConfigurationAndControl::Delegate
{
public:
    CHIP_ERROR Init(uint8_t ep);

    // shall return current level if supported, otherwise null
    chip::app::DataModel::Nullable<chip::Percent> HandleOpenValve(chip::app::DataModel::Nullable<chip::Percent> level) override;
    CHIP_ERROR HandleCloseValve() override;
    void HandleRemainingDurationTick(uint32_t duration) override;
    
    void OpenValve(void);
    void CloseValve(void);

private:
    uint8_t mEndpoint;
    uint8_t mCurrentLevelPercent;
    uint8_t mCurrentState;
    friend WaterValveManager & ValveMgr(void);
    static WaterValveManager sValve;
};

inline WaterValveManager & ValveMgr(void)
{
    return WaterValveManager::sValve;
}
