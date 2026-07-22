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

#include "OccupancyManager.h"
#include "AppConfig.h"
#include "AppEvent.h"
#include "AppTask.h"
#include "semphr.h"

using namespace chip;
using namespace ::chip::DeviceLayer;

constexpr EndpointId kOccupancyEndpoint = 1;

// Default HoldTime: 10 seconds (valid range 1~300)
constexpr uint16_t kHoldTimeDefault = 10;
constexpr uint16_t kHoldTimeMin     = 1;
constexpr uint16_t kHoldTimeMax     = 300;

namespace OccupancyAttr = chip::app::Clusters::OccupancySensing::Attributes;
using namespace chip::app::Clusters::OccupancySensing;

// OccupancySensing::Instance handles FeatureMap, HoldTime, HoldTimeLimits via AAI
static Instance gOccupancySensingInstance(BitMask<Feature>(Feature::kPassiveInfrared));

OccupancyManager OccupancyManager::sOccuMgr;

CHIP_ERROR OccupancyManager::Init()
{
    // Init OccupancySensing AAI instance (handles FeatureMap, HoldTime, HoldTimeLimits)
    ReturnErrorOnFailure(gOccupancySensingInstance.Init());

    // Set HoldTimeLimits
    Structs::HoldTimeLimitsStruct::Type holdTimeLimits;
    holdTimeLimits.holdTimeMin     = kHoldTimeMin;
    holdTimeLimits.holdTimeMax     = kHoldTimeMax;
    holdTimeLimits.holdTimeDefault = kHoldTimeDefault;
    ReturnErrorOnFailure(SetHoldTimeLimits(kOccupancyEndpoint, holdTimeLimits));

    // Set initial HoldTime
    ReturnErrorOnFailure(SetHoldTime(kOccupancyEndpoint, kHoldTimeDefault));

    // Initialize OccupancySensorType: kPir (0)
    OccupancyAttr::OccupancySensorType::Set(kOccupancyEndpoint,
        OccupancySensorTypeEnum::kPir);

    // Initialize OccupancySensorTypeBitmap: kPir (0x1)
    chip::BitMask<OccupancySensorTypeBitmap> bitmap;
    bitmap.Set(OccupancySensorTypeBitmap::kPir);
    OccupancyAttr::OccupancySensorTypeBitmap::Set(kOccupancyEndpoint, bitmap);

    // Ensure occupancy starts as unoccupied
    mOccupancy = 0;
    OccupancyAttr::Occupancy::Set(kOccupancyEndpoint, mOccupancy);

    // Create HoldTime auto-unoccupy timer (one-shot)
    mHoldTimer = xTimerCreateStatic("HoldTmr", pdMS_TO_TICKS(kHoldTimeDefault * 1000),
                                    pdFALSE, nullptr, HoldTimerEventHandler,
                                    &mStaticHoldTimerStruct);
    if (mHoldTimer == NULL)
    {
        ChipLogError(NotSpecified, "HoldTimer create failed");
        return CHIP_ERROR_NO_MEMORY;
    }

    ChipLogProgress(NotSpecified, "OccupancyManager::Init done");
    return CHIP_NO_ERROR;
}

void OccupancyManager::AttributeChangeHandler(EndpointId endpointId, AttributeId attributeId, uint8_t * value, uint16_t size)
{
}

CHIP_ERROR OccupancyManager::ToggleOccupancy()
{
    mOccupancy ^= 0x1;

    if (mOccupancy & 0x1)
    {
        ChipLogProgress(NotSpecified, "Occupied");
        hosal_gpio_pin_clear(21);

        // Start hold timer - read current HoldTime
        uint16_t * holdTime = GetHoldTimeForEndpoint(kOccupancyEndpoint);
        uint16_t   holdMs   = (holdTime != nullptr) ? (*holdTime * 1000) : (kHoldTimeDefault * 1000);
        xTimerChangePeriod(mHoldTimer, pdMS_TO_TICKS(holdMs), 0);
        xTimerStart(mHoldTimer, 0);
    }
    else
    {
        ChipLogProgress(NotSpecified, "Unoccupied");
        hosal_gpio_pin_set(21);
        xTimerStop(mHoldTimer, 0);
    }

    PlatformMgr().LockChipStack();
    OccupancyAttr::Occupancy::Set(kOccupancyEndpoint, mOccupancy);
    PlatformMgr().UnlockChipStack();

    ChipLogProgress(NotSpecified, "Occupancy: %d", mOccupancy);
    return CHIP_NO_ERROR;
}

void OccupancyManager::HoldTimerEventHandler(TimerHandle_t xTimer)
{
    // HoldTime expired: auto-clear to unoccupied
    ChipLogProgress(NotSpecified, "HoldTime expired, clearing occupancy");

    PlatformMgr().LockChipStack();
    OccuMgr().mOccupancy = 0;
    hosal_gpio_pin_set(21);
    OccupancyAttr::Occupancy::Set(kOccupancyEndpoint, static_cast<uint8_t>(0));
    PlatformMgr().UnlockChipStack();
}
