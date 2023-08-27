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

#include <math.h>
#include "LightingManager.h"

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Clusters.h>

#include "AppConfig.h"
#include "AppTask.h"
#include <FreeRTOS.h>

#include <lib/support/TypeTraits.h>

#include "util_log.h"

// initialization values for Blue in XY color space
constexpr XyColor_t kBlueXY = { 9830, 3932 };

// initialization values for Blue in HSV color space
constexpr HsvColor_t kHSV = { 60, 0, 254 };

using namespace chip;
using namespace chip::DeviceLayer;
using namespace chip::app::Clusters::LevelControl;
using namespace chip::app::Clusters;

LightingManager LightingManager::sLight;

CHIP_ERROR LightingManager::Init()
{
    bool currentLedState;
    uint8_t currentLedHue;
    uint8_t currentLedSaturation;
    uint8_t currentLedColorMode;
    uint16_t currentLedColorTemperature;
    app::DataModel::Nullable<uint8_t> nullableCurrentLevel;

    EmberAfStatus status;

    OnOffServer::Instance().getOnOffValue(1, &currentLedState);
    status = Attributes::CurrentLevel::Get(1, nullableCurrentLevel);
    if (status == EMBER_ZCL_STATUS_SUCCESS)
    {
        mLevel = nullableCurrentLevel.Value();
    }
    else
    {
        mLevel = kHSV.v;
    }
    mHSV.v = mLevel;
    status = ColorControl::Attributes::CurrentHue::Get(1, &currentLedHue);
    if (status == EMBER_ZCL_STATUS_SUCCESS)
    {
        mHSV.h = currentLedHue;
    }
    else
    {
        mHSV.h = kHSV.h;
    }
    status = ColorControl::Attributes::CurrentSaturation::Get(1, &currentLedSaturation);
    if (status == EMBER_ZCL_STATUS_SUCCESS)
    {
        mHSV.s = currentLedSaturation;
    }
    else
    {
        mHSV.s = kHSV.s;
    }
    status = ColorControl::Attributes::ColorMode::Get(1, &currentLedColorMode);
    if (status == EMBER_ZCL_STATUS_SUCCESS)
    {
    }
    else
    {
        currentLedColorMode = 0x0;
    }
    status = ColorControl::Attributes::ColorTemperatureMireds::Get(1, &currentLedColorTemperature);
    if (status == EMBER_ZCL_STATUS_SUCCESS)
    {
        mCT.ctMireds = currentLedColorTemperature;
    }
    // err("color mode: %d\r\n", currentLedColorMode);
    // err("level: %d\r\n", mHSV.v);
    // err("hue: %d\r\n", mHSV.h);
    // err("saturation: %d\r\n", mHSV.s);
    // err("color temperature: %d\r\n", mCT.ctMireds);

    mXY    = kBlueXY;
    // mHSV   = kHSV;
    if (currentLedColorMode == 0x0)
        mRGB = HsvToRgb(mHSV);
    else if (currentLedColorMode == 0x2)
        mRGB = CctToRgb(mCT); 
    mState = currentLedState ? kState_On : kState_Off;

    return CHIP_NO_ERROR;
}

void LightingManager::SetCallbacks(LightingCallback_fn aActionInitiated_CB, LightingCallback_fn aActionCompleted_CB)
{
    mActionInitiated_CB = aActionInitiated_CB;
    mActionCompleted_CB = aActionCompleted_CB;
}

bool LightingManager::IsTurnedOn()
{
    return mState == kState_On;
}

uint8_t LightingManager::GetLevel()
{
    return mLevel;
}

RgbColor_t LightingManager::GetRgb()
{
    return mRGB;
}

bool LightingManager::InitiateAction(Action_t aAction, int32_t aActor, uint16_t size, uint8_t * value)
{
    bool action_initiated = false;
    State_t new_state;
    XyColor_t xy;
    HsvColor_t hsv;
    CtColor_t ct;

    switch (aAction)
    {
    case ON_ACTION:
        ChipLogProgress(NotSpecified, "LightMgr:ON: %s->ON", mState == kState_On ? "ON" : "OFF");
        break;
    case OFF_ACTION:
        ChipLogProgress(NotSpecified, "LightMgr:OFF: %s->OFF", mState == kState_On ? "ON" : "OFF");
        break;
    case LEVEL_ACTION:
        ChipLogProgress(NotSpecified, "LightMgr:LEVEL: lev:%u->%u", mLevel, *value);
        break;
    case COLOR_ACTION_XY:
        xy = *reinterpret_cast<XyColor_t *>(value);
        ChipLogProgress(NotSpecified, "LightMgr:COLOR: xy:%u|%u->%u|%u", mXY.x, mXY.y, xy.x, xy.y);
        break;
    case COLOR_ACTION_HSV:
        hsv = *reinterpret_cast<HsvColor_t *>(value);
        ChipLogProgress(NotSpecified, "LightMgr:COLOR: hsv:%u|%u->%u|%u", mHSV.h, mHSV.s, hsv.h, hsv.s);
        break;
    case COLOR_ACTION_CT:
        ct.ctMireds = *reinterpret_cast<uint16_t *>(value);
        ChipLogProgress(NotSpecified, "LightMgr:COLOR: ct:%u->%u", mCT.ctMireds, ct.ctMireds);
        break;        
    default:
        ChipLogProgress(NotSpecified, "LightMgr:Unknown");
        break;
    }

    // Initiate Turn On/Off Action only when the previous one is complete.
    if (mState == kState_Off && aAction == ON_ACTION)
    {
        action_initiated = true;

        new_state = kState_On;
    }
    else if (mState == kState_On && aAction == OFF_ACTION)
    {
        action_initiated = true;

        new_state = kState_Off;
    }
    else if (aAction == LEVEL_ACTION && *value != mLevel)
    {
        action_initiated = true;
        if (*value == 0)
        {
            new_state = kState_Off;
        }
        else
        {
            new_state = kState_On;
        }
    }
    else if (aAction == COLOR_ACTION_XY)
    {
        action_initiated = true;
        if (xy.x == 0 && xy.y == 0)
        {
            new_state = kState_Off;
        }
        else
        {
            new_state = kState_On;
        }
    }
    else if (aAction == COLOR_ACTION_HSV)
    {
        action_initiated = true;
        if (hsv.h == 0 && hsv.s == 0)
        {
            new_state = kState_Off;
        }
        else
        {
           new_state = kState_On;
        }
    }
    else if (aAction == COLOR_ACTION_CT)
    {
        action_initiated = true;
        if (ct.ctMireds == 0)
        {
            new_state = kState_Off;
        }
        else
        {
           new_state = kState_On;
        }
    }
    if (action_initiated)
    {        
        if (mActionInitiated_CB)
        {
            mActionInitiated_CB(aAction);
        }
        if (aAction == LEVEL_ACTION)
        {
            SetLevel(*value);
        }
        else if (aAction == COLOR_ACTION_XY)
        {
            SetColor(xy.x, xy.y);
        }
        else if (aAction == COLOR_ACTION_HSV)
        {
            SetColor(hsv.h, hsv.s);
        }
        else if (aAction == COLOR_ACTION_CT)
        {
            SetColorTemperature(ct);
        }
        else
        {
            Set(new_state == kState_On);
        }

        if (mActionCompleted_CB)
        {
            mActionCompleted_CB(aAction);
        }
    }

    return action_initiated;
}

void LightingManager::SetLevel(uint8_t aLevel)
{
    mLevel = aLevel;
    mHSV.v = mLevel;
    //mRGB   = XYToRgb(mLevel, mXY.x, mXY.y);
    mRGB   = HsvToRgb(mHSV);
    UpdateLight();
}

void LightingManager::SetColor(uint16_t x, uint16_t y)
{
    mXY.x = x;
    mXY.y = y;
    mRGB  = XYToRgb(mLevel, mXY.x, mXY.y);
    UpdateLight();
}

void LightingManager::SetColor(uint8_t hue, uint8_t saturation)
{
    mHSV.h = hue;
    mHSV.s = saturation;
    mHSV.v = mLevel; // use level from Level Cluster as Vibrance parameter

    // ChipLogProgress(NotSpecified, "Sync HSV: %d %d %d", mHSV.h, mHSV.s, mHSV.v);

    mRGB = HsvToRgb(mHSV);

    // info("===> H: %d, S: %d\r\n", hue, saturation);
    // info("===> R: %d, G: %d, B: %d\r\n", mRGB.r, mRGB.g, mRGB.b);

    UpdateLight();
}

void LightingManager::SetColorTemperature(CtColor_t ct)
{
    mCT.ctMireds = ct.ctMireds;
    // info("===> mCT.ctMireds: %d\r\n", mCT.ctMireds);
    // info("===> Kelvin: %d\r\n", 1000000 / mCT.ctMireds);
    mRGB = CctToRgb(mCT);
    UpdateLight();
}

void LightingManager::Set(bool aOn)
{
    if (aOn)
    {
        mState = kState_On;
    }
    else
    {
        mState = kState_Off;
    }
    UpdateLight();
}

void LightingManager::UpdateLight()
{
    // ChipLogProgress(NotSpecified, "UpdateLight: %d L:%d R:%d G:%d B:%d", mState, mLevel, mRGB.r, mRGB.g, mRGB.b);

    if (mState == kState_On && mLevel > 1)
    {
        // err("R: %d, G: %d, B: %d\r\n", mRGB.r, mRGB.g, mRGB.b);

        rt582_led_level_ctl(2, mRGB.b);
        rt582_led_level_ctl(3, mRGB.r);
        rt582_led_level_ctl(4, mRGB.g);
    }
}
