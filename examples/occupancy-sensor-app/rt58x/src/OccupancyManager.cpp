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

#include "OccupancyManager.h"
#include "AppConfig.h"
#include "AppEvent.h"
#include "AppTask.h"
#include "semphr.h"

/**********************************************************
 * Defines and Constants
 *********************************************************/

using namespace chip;
using namespace ::chip::DeviceLayer;

constexpr EndpointId kOccupancyEndpoint = 1;

//namespace ThermAttr = chip::app::Clusters::Thermostat::Attributes;
namespace OccupancyAttr = chip::app::Clusters::OccupancySensing::Attributes;
/**********************************************************
 * Variable declarations
 *********************************************************/

OccupancyManager OccupancyManager::sOccuMgr;

CHIP_ERROR OccupancyManager::Init()
{
    chip::BitMask<chip::app::Clusters::OccupancySensing::OccupancyBitmap> temp;

    OccupancyAttr::Occupancy::Get(kOccupancyEndpoint, &temp);

    mOccupancy =  temp.Raw();
    return CHIP_NO_ERROR;
}

void OccupancyManager::AttributeChangeHandler(EndpointId endpointId, AttributeId attributeId, uint8_t * value, uint16_t size)
{

}

CHIP_ERROR OccupancyManager::ToggleOccupancy()
{
    if(mOccupancy == 1)
      mOccupancy = 0;
    else
      mOccupancy = 1;

    PlatformMgr().LockChipStack();
    OccupancyAttr::Occupancy::Set(kOccupancyEndpoint, mOccupancy);
    PlatformMgr().UnlockChipStack();

    ChipLogProgress(NotSpecified, "occupancy status : %d", mOccupancy);

    return CHIP_NO_ERROR;
}

