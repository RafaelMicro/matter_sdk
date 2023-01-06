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

#include "TemperatureManager.h"
#include "AppConfig.h"
#include "AppEvent.h"
#include "AppTask.h"
#include "semphr.h"

/**********************************************************
 * Defines and Constants
 *********************************************************/

using namespace chip;
using namespace ::chip::DeviceLayer;

constexpr EndpointId kThermostatEndpoint = 1;

namespace ThermAttr = chip::app::Clusters::Thermostat::Attributes;
/**********************************************************
 * Variable declarations
 *********************************************************/

TemperatureManager TemperatureManager::sTempMgr;

CHIP_ERROR TemperatureManager::Init()
{
    app::DataModel::Nullable<int16_t> temp;
    int16_t heatingSetpoint, coolingSetpoint;
    uint8_t systemMode;

    //PlatformMgr().LockChipStack();
    ThermAttr::LocalTemperature::Get(kThermostatEndpoint, temp);
    ThermAttr::OccupiedCoolingSetpoint::Get(kThermostatEndpoint, &coolingSetpoint);
    ThermAttr::OccupiedHeatingSetpoint::Get(kThermostatEndpoint, &heatingSetpoint);
    ThermAttr::SystemMode::Get(kThermostatEndpoint, &systemMode);
    //PlatformMgr().UnlockChipStack();

    mCurrentTempCelsius     = ConvertToPrintableTemp(temp.Value());
    mHeatingCelsiusSetPoint = ConvertToPrintableTemp(coolingSetpoint);
    mCoolingCelsiusSetPoint = ConvertToPrintableTemp(heatingSetpoint);
    mThermMode              = systemMode;


    GetAppTask().UpdateThermoStatUI();
    return CHIP_NO_ERROR;
}

int16_t TemperatureManager::ConvertToPrintableTemp(int16_t temperature)
{
    return static_cast<int16_t>(temperature);
}

void TemperatureManager::AttributeChangeHandler(EndpointId endpointId, AttributeId attributeId, uint8_t * value, uint16_t size)
{
    switch (attributeId)
    {
    case ThermAttr::LocalTemperature::Id: {
        int16_t Temp = ConvertToPrintableTemp(*((int16_t *) value));
        ChipLogProgress(NotSpecified, "Local temp %d", Temp);
        mCurrentTempCelsius = Temp;
    }
    break;

    case ThermAttr::OccupiedCoolingSetpoint::Id: {
        int16_t coolingTemp = ConvertToPrintableTemp(*((int16_t *) value));
        ChipLogProgress(NotSpecified, "CoolingSetpoint %d", coolingTemp);
        mCoolingCelsiusSetPoint = coolingTemp;
    }
    break;

    case ThermAttr::OccupiedHeatingSetpoint::Id: {
        int16_t heatingTemp = ConvertToPrintableTemp(*((int16_t *) value));
        ChipLogProgress(NotSpecified, "HeatingSetpoint %d", heatingTemp);
        mHeatingCelsiusSetPoint = heatingTemp;
    }
    break;

    case ThermAttr::SystemMode::Id: {
        ChipLogProgress(NotSpecified, "SystemMode %d", static_cast<uint8_t>(*value));
        uint8_t mode = static_cast<uint8_t>(*value);
        if (mThermMode != mode)
        {
            mThermMode = mode;
        }
    }
    break;

    default: {
        ChipLogProgress(NotSpecified, "Unhandled thermostat attribute %x", attributeId);
        return;
    }
    break;
    }

    GetAppTask().UpdateThermoStatUI();
}

uint8_t TemperatureManager::GetMode()
{
    return mThermMode;
}

int16_t TemperatureManager::GetCurrentTemp()
{
    return mCurrentTempCelsius;
}
int16_t TemperatureManager::GetHeatingSetPoint()
{
    return mHeatingCelsiusSetPoint;
}

int16_t TemperatureManager::GetCoolingSetPoint()
{
    return mCoolingCelsiusSetPoint;
}
