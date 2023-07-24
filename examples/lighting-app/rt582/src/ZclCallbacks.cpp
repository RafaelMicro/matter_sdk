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
#include "init_rt582Platform.h"
#include "init_lighting_rt582Platform.h"

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>
#include <app/util/af-types.h>
#include <assert.h>
#include <lib/support/logging/CHIPLogging.h>

using namespace ::chip;
using namespace ::chip::app::Clusters;

void MatterPostAttributeChangeCallback(const chip::app::ConcreteAttributePath & attributePath, uint8_t type, uint16_t size,
                                       uint8_t * value)
{
    EndpointId endpoint     = attributePath.mEndpointId;
    ClusterId clusterId     = attributePath.mClusterId;
    AttributeId attributeId = attributePath.mAttributeId;

    DeviceLayer::ChipDeviceEvent event;

    static HsvColor_t hsv = {.h = 0, .s = 0, .v = 0};

    static bool getH = false;
    static bool getS = false;
    // static bool powerOnZCLInitOnOff = true;
    static bool powerOnZCLInitLevelControl = true;
    static bool powerOnZCLInitColortemperature = true;

    switch (clusterId)
    {
    case OnOff::Id:
        if (attributeId == OnOff::Attributes::OnOff::Id) 
        {
            event.Type = DeviceLayer::DeviceEventType::kOnOffAttributeChanged;
            event.OnOffChanged.value = *value;

            DeviceLayer::PlatformMgr().PostEvent(&event);
        }
        break;
    case LevelControl::Id:
        if (attributeId == LevelControl::Attributes::CurrentLevel::Id && size == 1)
        {
            event.Type = DeviceLayer::DeviceEventType::kLevelControlAttributeChanged;
            event.LevelControlChanged.level = *value;
            if (powerOnZCLInitLevelControl) 
            {
                powerOnZCLInitLevelControl = false;
                break;
            }
            DeviceLayer::PlatformMgr().PostEvent(&event);
        }
        break;
    case ColorControl::Id:
        if (attributeId == ColorControl::Attributes::RemainingTime::Id)
        {
            uint16_t RemainingTime = *reinterpret_cast<uint16_t *>(value);
            return;
        }
        else if (attributeId == ColorControl::Attributes::ColorMode::Id)
        {
            uint8_t ColorMode = *reinterpret_cast<uint8_t *>(value);
            return;
        }
        else if (attributeId == ColorControl::Attributes::EnhancedColorMode::Id)
        {
            uint8_t EnhancedColorMode = *reinterpret_cast<uint8_t *>(value);
            return;
        }
        else if (attributeId == ColorControl::Attributes::CurrentX::Id || 
                 attributeId == ColorControl::Attributes::CurrentY::Id)
        {
            event.Type = DeviceLayer::DeviceEventType::kColorControlAttributeXYChanged;
            DeviceLayer::PlatformMgr().PostEvent(&event);
        }
        else if (attributeId == ColorControl::Attributes::CurrentHue::Id         ||
                 attributeId == ColorControl::Attributes::CurrentSaturation::Id  ||
                 attributeId == ColorControl::Attributes::EnhancedCurrentHue::Id)
        {
            if (attributeId == ColorControl::Attributes::EnhancedCurrentHue::Id)
            {
                // We only support 8-bit hue. Assuming hue is linear, normalize 16-bit to 8-bit.
                hsv.h = (uint8_t)((*reinterpret_cast<uint16_t *>(value)) >> 8);
                // get saturation from cluster value storage
                EmberAfStatus status = ColorControl::Attributes::CurrentSaturation::Get(endpoint, &hsv.s);
                // assert(status == EMBER_ZCL_STATUS_SUCCESS);

                // getH = true;
            }
            else if (attributeId == ColorControl::Attributes::CurrentHue::Id)
            {
                hsv.h = *value;
                // get saturation from cluster value storage
                EmberAfStatus status = ColorControl::Attributes::CurrentSaturation::Get(endpoint, &hsv.s);
                // assert(status == EMBER_ZCL_STATUS_SUCCESS);

                event.Type = DeviceLayer::DeviceEventType::kColorControlAttributeHSVChanged;
                event.ColorControlHSVChanged.hue = hsv.h;
                event.ColorControlHSVChanged.saturation = hsv.s;
                DeviceLayer::PlatformMgr().PostEvent(&event);

                // getH = true;
            }
            else if (attributeId == ColorControl::Attributes::CurrentSaturation::Id)
            {
                hsv.s = *value;
                // get hue from cluster value storage
                EmberAfStatus status = ColorControl::Attributes::CurrentHue::Get(endpoint, (uint8_t *)&hsv.h);
                // assert(status == EMBER_ZCL_STATUS_SUCCESS);

                event.Type = DeviceLayer::DeviceEventType::kColorControlAttributeHSVChanged;
                event.ColorControlHSVChanged.hue = hsv.h;
                event.ColorControlHSVChanged.saturation = hsv.s;
                DeviceLayer::PlatformMgr().PostEvent(&event);

                // getS = true;
            }

            // if (getH && getS) 
            // {
            //     event.Type = DeviceLayer::DeviceEventType::kColorControlAttributeHSVChanged;
            //     event.ColorControlHSVChanged.hue = hsv.h;
            //     event.ColorControlHSVChanged.saturation = hsv.s;

            //     getH = getS = false;
            //     DeviceLayer::PlatformMgr().PostEvent(&event);
            // }
        }
        else if (attributeId == ColorControl::Attributes::ColorTemperatureMireds::Id)
        {
            event.Type = DeviceLayer::DeviceEventType::kColorControlAttributeCTChanged;
            event.ColorControlCTChanged.ctMireds = *reinterpret_cast<uint16_t *>(value);   
            if (powerOnZCLInitColortemperature) 
            {
                powerOnZCLInitColortemperature = false;
                break;
            }
            DeviceLayer::PlatformMgr().PostEvent(&event);
        }
        break;
    default:
        break;
    }

    if (clusterId == Identify::Id)
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