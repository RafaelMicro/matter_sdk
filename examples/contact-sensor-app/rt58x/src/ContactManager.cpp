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

#include "ContactManager.h"
#include "AppConfig.h"
#include "AppEvent.h"
#include "AppTask.h"

using namespace chip;
using namespace chip::DeviceLayer;
using namespace chip::app::Clusters;

constexpr EndpointId kContactEndpoint = 1;

ContactManager ContactManager::sContMgr;

CHIP_ERROR ContactManager::Init()
{
    auto booleanState = BooleanState::FindClusterOnEndpoint(kContactEndpoint);
    ChipLogProgress(NotSpecified, "Contact Sensor State: Contact");
    booleanState->SetStateValue(true);
    hosal_gpio_pin_set(21);
    return CHIP_NO_ERROR;
}

void ContactManager::AttributeChangeHandler(EndpointId endpointId, AttributeId attributeId, uint8_t * value, uint16_t size)
{
}

void ContactManager::ToggleStateValue(bool val)
{
    auto booleanState = BooleanState::FindClusterOnEndpoint(kContactEndpoint);
    if (booleanState != nullptr)
    {
        if (val)
        {
            ChipLogProgress(NotSpecified, "Contact Sensor State: Contact");
            hosal_gpio_pin_set(21);
        }
        else
        {
            ChipLogProgress(NotSpecified, "Contact Sensor State: Not contact");
            hosal_gpio_pin_clear(21);
        }
        PlatformMgr().LockChipStack();
        booleanState->SetStateValue(val);
        PlatformMgr().UnlockChipStack();
    }
}
