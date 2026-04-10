/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2018 Nest Labs, Inc.
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

#include "LightSwitchMgr.h"

#include "AppConfig.h"
#include "AppEvent.h"

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/cluster-objects.h>
#include <app/clusters/switch-server/switch-server.h>
#include <platform/CHIPDeviceLayer.h>

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;

LightSwitchMgr LightSwitchMgr::sSwitch;

/**
 * @brief Configures LightSwitchMgr
 *        This function needs to be call before using the LightSwitchMgr
 *
 * @param lightSwitchEndpoint endpoint for the light switch device type
 * @param genericSwitchEndpoint endpoint for the generic switch device type
 */
CHIP_ERROR LightSwitchMgr::Init(chip::EndpointId genericSwitchEndpoint)
{
    VerifyOrReturnError(genericSwitchEndpoint != kInvalidEndpointId, CHIP_ERROR_INVALID_ARGUMENT);

    mGenericSwitchEndpoint = genericSwitchEndpoint;
    return CHIP_NO_ERROR;
}

/**
 * @brief Function that triggers a generic switch OnInitialPress event
 */
void LightSwitchMgr::GenericSwitchOnInitialPress()
{
    GenericSwitchEventData * data = Platform::New<GenericSwitchEventData>();

    data->endpoint = mGenericSwitchEndpoint;
    data->event    = Switch::Events::InitialPress::Id;

    DeviceLayer::PlatformMgr().ScheduleWork(GenericSwitchWorkerFunction, reinterpret_cast<intptr_t>(data));
}

/**
 * @brief Function that triggers a generic switch OnShortRelease event
 */
void LightSwitchMgr::GenericSwitchOnShortRelease()
{
    GenericSwitchEventData * data = Platform::New<GenericSwitchEventData>();

    data->endpoint = mGenericSwitchEndpoint;
    data->event    = Switch::Events::ShortRelease::Id;

    DeviceLayer::PlatformMgr().ScheduleWork(GenericSwitchWorkerFunction, reinterpret_cast<intptr_t>(data));
}
/**
 * @brief Function that triggers a generic switch OnLongPress event
 */
void LightSwitchMgr::GenericSwitchOnLongPress()
{
    GenericSwitchEventData * data = Platform::New<GenericSwitchEventData>();

    data->endpoint = mGenericSwitchEndpoint;
    data->event    = Switch::Events::LongPress::Id;

    DeviceLayer::PlatformMgr().ScheduleWork(GenericSwitchWorkerFunction, reinterpret_cast<intptr_t>(data));
}
/**
 * @brief Function that triggers a generic switch OnLongRelease event
 */
void LightSwitchMgr::GenericSwitchOnLongRelease()
{
    GenericSwitchEventData * data = Platform::New<GenericSwitchEventData>();

    data->endpoint = mGenericSwitchEndpoint;
    data->event    = Switch::Events::LongRelease::Id;

    DeviceLayer::PlatformMgr().ScheduleWork(GenericSwitchWorkerFunction, reinterpret_cast<intptr_t>(data));
}

void LightSwitchMgr::GenericSwitchWorkerFunction(intptr_t context)
{
    GenericSwitchEventData * data = reinterpret_cast<GenericSwitchEventData *>(context);

    switch (data->event)
    {
    case Switch::Events::InitialPress::Id: {
        uint8_t currentPosition = 1;

        // Set new attribute value
        Clusters::Switch::Attributes::CurrentPosition::Set(data->endpoint, currentPosition);

        // Trigger event
        Clusters::SwitchServer::Instance().OnInitialPress(data->endpoint, currentPosition);
        break;
    }
    case Switch::Events::ShortRelease::Id: {
        uint8_t previousPosition = 1;
        uint8_t currentPosition  = 0;

        // Set new attribute value
        Clusters::Switch::Attributes::CurrentPosition::Set(data->endpoint, currentPosition);

        // Trigger event
        Clusters::SwitchServer::Instance().OnShortRelease(data->endpoint, previousPosition);
        break;
    }
    case Switch::Events::LongPress::Id: {
        uint8_t currentPosition = 1;

        // Set new attribute value
        Clusters::Switch::Attributes::CurrentPosition::Set(data->endpoint, currentPosition);

        // Trigger event
        Clusters::SwitchServer::Instance().OnLongPress(data->endpoint, currentPosition);
        break;
    }
    case Switch::Events::LongRelease::Id: {
        uint8_t previousPosition = 1;
        uint8_t currentPosition  = 0;

        // Set new attribute value
        Clusters::Switch::Attributes::CurrentPosition::Set(data->endpoint, currentPosition);

        // Trigger event
        Clusters::SwitchServer::Instance().OnLongRelease(data->endpoint, previousPosition);
        break;
    }
    default:
        break;
    }

    Platform::Delete(data);
}
