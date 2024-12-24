/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
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

/**
 * @file
 *   This file implements the handler for data model messages.
 */

#include "AppConfig.h"
#include "AppTask.h"
#include "LightingManager.h"
#include "init_rt58xPlatform.h"
#include "init_device_environment.h"

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>
#include <app/util/af-types.h>
#include <assert.h>
#include <lib/support/logging/CHIPLogging.h>

using namespace ::chip;
using namespace chip::DeviceLayer;
using namespace chip::System;
using namespace ::chip::app::Clusters;
using chip::Protocols::InteractionModel::Status;

void MatterPostAttributeChangeCallback(const chip::app::ConcreteAttributePath & attributePath, uint8_t type, uint16_t size,
                                       uint8_t * value)
{
    EndpointId endpoint     = attributePath.mEndpointId;
    ClusterId clusterId     = attributePath.mClusterId;
    AttributeId attributeId = attributePath.mAttributeId;
    switch (clusterId)
    {
    case OnOff::Id:
        if (attributeId == OnOff::Attributes::OnOff::Id) 
        {
            LightMgr().InitiateAction(*value ? LightingManager::ON_ACTION : LightingManager::OFF_ACTION, value);
        }
        break;
    case LevelControl::Id:
        if (attributeId == LevelControl::Attributes::CurrentLevel::Id && size == 1)
        {
            LightMgr().InitiateAction(LightingManager::LEVEL_ACTION, value);
        }
        break;
    case ColorControl::Id:
        if (attributeId == ColorControl::Attributes::ColorMode::Id)
        {
            chip::app::Clusters::ColorControl::ColorModeEnum ColorMode = *reinterpret_cast<chip::app::Clusters::ColorControl::ColorModeEnum *>(value);
            LightMgr().SetColorMode(ColorMode);
            return;
        }
        else if (attributeId == ColorControl::Attributes::CurrentX::Id || 
                 attributeId == ColorControl::Attributes::CurrentY::Id)
        {
            LightMgr().SetColorMode(ColorControl::ColorModeEnum::kCurrentXAndCurrentY);
            SystemLayer().StartTimer(Clock::Milliseconds32(100), LightMgr().DelayedXYAction, nullptr);
        }
        else if (attributeId == ColorControl::Attributes::CurrentHue::Id         ||
                 attributeId == ColorControl::Attributes::CurrentSaturation::Id  ||
                 attributeId == ColorControl::Attributes::EnhancedCurrentHue::Id)
        {
            LightMgr().SetColorMode(ColorControl::ColorModeEnum::kCurrentHueAndCurrentSaturation);
            SystemLayer().StartTimer(Clock::Milliseconds32(100), LightMgr().DelayedHSVAction, nullptr);
        }
        else if (attributeId == ColorControl::Attributes::ColorTemperatureMireds::Id)
        {
            LightMgr().SetColorMode(ColorControl::ColorModeEnum::kColorTemperatureMireds);
            LightMgr().InitiateAction(LightingManager::COLOR_ACTION_CT, value);
        }
        break;
    default:
        break;
    }

    if (clusterId == Identify::Id && attributeId == Identify::Attributes::IdentifyTime::Id && *value > 0)
    {
        GetAppTask().PostAppIdentify();
    }
}
/** @brief OnOff Cluster Init
 *
 * This function is called when a specific cluster is initialized. It gives the
 * application an opportunity to take care of cluster initialization procedures.
 * It is called exactly once for each endpoint where cluster is present.
 *
 * @param endpoint   Ver.: always
 *
 * TODO Issue #3841
 * emberAfOnOffClusterInitCallback happens before the stack initialize the cluster
 * attributes to the default value.
 * The logic here expects something similar to the deprecated Plugins callback
 * emberAfPluginOnOffClusterServerPostInitCallback.
 *
 */
void emberAfOnOffClusterInitCallback(EndpointId endpoint)
{
    // TODO: implement any additional Cluster Server init actions
}