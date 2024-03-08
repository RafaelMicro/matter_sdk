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

#include "SensorManager.h"
#include "AppConfig.h"
#include "AppEvent.h"
#include "AppTask.h"

/**********************************************************
 * Defines and Constants
 *********************************************************/

using namespace chip;
using namespace ::chip::DeviceLayer;

constexpr EndpointId kTemperatureMeasurementEndpoint = 1;
constexpr uint16_t kSensorTImerPeriodMs  = 30000; // 30s timer period
constexpr uint16_t kMinTemperatureDelta  = 50;    // 0.5 degree Celcius

/**********************************************************
 * Variable declarations
 *********************************************************/

TimerHandle_t sSensorTimer;
StaticTimer_t sStaticSensorTimerStruct;

SensorManager SensorManager::sSensorManager;

constexpr uint16_t kSimulatedReadingFrequency = (60000 / kSensorTImerPeriodMs); // Change Simulated number at each minutes
static int16_t mSimulatedTemp[]               = { 2300, 2400, 2800, 2550, 2200, 2125, 2100, 2600, 1800, 2700 };

CHIP_ERROR SensorManager::Init()
{
    // Create FreeRTOS sw timer for temp sensor timer.
    sSensorTimer = xTimerCreateStatic("sensorTmr", pdMS_TO_TICKS(kSensorTImerPeriodMs), true, nullptr, SensorTimerEventHandler,
                                      &sStaticSensorTimerStruct);

    if (sSensorTimer == NULL)
    {
        ChipLogProgress(NotSpecified, "sSensorTimer timer create failed");
        return APP_ERROR_CREATE_TIMER_FAILED;
    }

    // Update Temp immediatly at bootup
    //SensorTimerEventHandler(sSensorTimer);

    // Trigger periodic update
    xTimerStart(sSensorTimer, 10);

    ChipLogProgress(NotSpecified, "SensorManager::Init");

    return CHIP_NO_ERROR;
}

void SensorManager::SensorTimerEventHandler(TimerHandle_t xTimer)
{
    int16_t temperature            = 0;
    static int16_t lastTemperature = 0;

    static uint8_t nbOfRepetition = 0;
    static uint8_t simulatedIndex = 0;
    if (simulatedIndex >= 9)
    {
        simulatedIndex = 0;
    }
    temperature = mSimulatedTemp[simulatedIndex];

    nbOfRepetition++;
    if (nbOfRepetition >= kSimulatedReadingFrequency)
    {
        simulatedIndex++;
        nbOfRepetition = 0;
    }
    ChipLogProgress(NotSpecified, "Sensor Temp is : %d", temperature);

    if ((temperature >= (lastTemperature + kMinTemperatureDelta)) || temperature <= (lastTemperature - kMinTemperatureDelta))
    {
        lastTemperature = temperature;
        PlatformMgr().LockChipStack();
        // The SensorMagager shouldn't be aware of the Endpoint ID TODO Fix this.
        // TODO Per Spec we should also apply the Offset stored in the same cluster before saving the temp

        app::Clusters::TemperatureMeasurement::Attributes::MeasuredValue::Set(kTemperatureMeasurementEndpoint, temperature);
        PlatformMgr().UnlockChipStack();
    }
}
