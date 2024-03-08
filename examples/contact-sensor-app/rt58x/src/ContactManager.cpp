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

#include "ContactManager.h"
#include "AppConfig.h"
#include "AppEvent.h"
#include "AppTask.h"
#include "semphr.h"

/**********************************************************
 * Defines and Constants
 *********************************************************/

using namespace chip;
using namespace ::chip::DeviceLayer;

constexpr EndpointId kContactEndpoint = 1;

//namespace ThermAttr = chip::app::Clusters::Thermostat::Attributes;
namespace ContactAttr = chip::app::Clusters::BooleanState::Attributes;
/**********************************************************
 * Variable declarations
 *********************************************************/

ContactManager ContactManager::sContMgr;

CHIP_ERROR ContactManager::Init()
{
    bool temp;

    ContactAttr::StateValue::Get(kContactEndpoint, &temp);

    mStateValue =  temp;
    return CHIP_NO_ERROR;
}

void ContactManager::AttributeChangeHandler(EndpointId endpointId, AttributeId attributeId, uint8_t * value, uint16_t size)
{

}

CHIP_ERROR ContactManager::ToggleStateValue()
{
    if(mStateValue == true)
      mStateValue = false;
    else
      mStateValue = true;

    PlatformMgr().LockChipStack();
    ContactAttr::StateValue::Set(kContactEndpoint, mStateValue);
    PlatformMgr().UnlockChipStack();

    ChipLogProgress(NotSpecified, "contact sensor status : %d", mStateValue);

    return CHIP_NO_ERROR;
}

