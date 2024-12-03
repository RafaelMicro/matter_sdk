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
#include "WindowControl.h"
#include "WindowManager.h"
#include "init_rt58xPlatform.h"
#include "init_window_rt58xPlatform.h"

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/callback.h>
#include <app-common/zap-generated/cluster-objects.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>
#include <app/CommandHandler.h>
#include <app/ConcreteCommandPath.h>
#include <app/clusters/window-covering-server/window-covering-server.h>
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
    switch (clusterId)
    {
    case Identify::Id:
        if (attributeId == Identify::Attributes::IdentifyTime::Id && *value > 0)
        {
            GetAppTask().PostAppIdentify();
        }
        break;
    case WindowCovering::Id:
        ChipLogProgress(Zcl, "Window covering cluster ID: " ChipLogFormatMEI " Type: %u Value: %u, length: %u",
                        ChipLogValueMEI(attributeId), type, *value, size);
        break;
    default:
        break;
    }
}

/* Forwards all attributes changes */
void MatterWindowCoveringClusterServerAttributeChangedCallback(const app::ConcreteAttributePath & attributePath)
{
    if (attributePath.mEndpointId == WindowControl::Endpoint())
    {
        switch (attributePath.mAttributeId)
        {
        case Attributes::TargetPositionLiftPercent100ths::Id:
            WindowControl::Instance().StartMove(WindowControl::MoveType::LIFT);
            break;
        case Attributes::TargetPositionTiltPercent100ths::Id:
            WindowControl::Instance().StartMove(WindowControl::MoveType::TILT);
            break;
        case Attributes::CurrentPositionLiftPercent100ths::Id:
            WindowControl::Instance().PositionLEDUpdate(WindowControl::MoveType::LIFT);
            break;
        case Attributes::CurrentPositionTiltPercent100ths::Id:
            WindowControl::Instance().PositionLEDUpdate(WindowControl::MoveType::TILT);
            break;
        default:
            WindowControl::Instance().SchedulePostAttributeChange(attributePath.mEndpointId, attributePath.mAttributeId);
            break;
        };
    }
}