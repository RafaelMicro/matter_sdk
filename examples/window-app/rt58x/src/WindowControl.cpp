/*
 *    Copyright (c) 2022 Project CHIP Authors
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

#include "WindowControl.h"
#include "WindowManager.h"

#include "AppConfig.h"

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app/util/af.h>
#include <platform/CHIPDeviceLayer.h>

using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;
using namespace chip::app::Clusters::WindowCovering;
using chip::Protocols::InteractionModel::Status;

static constexpr uint32_t sMoveTimeoutMs{ 200 };

WindowControl::WindowControl()
{
    // Init Light state
}

WindowControl::WindowControl(WindowManager::WindowCallback_fn aActionInitiated_CB, WindowManager::WindowCallback_fn aActionCompleted_CB)
{
    // Init Light state
    WindowMgr().Init();
    WindowMgr().SetCallbacks(aActionInitiated_CB, aActionCompleted_CB);
}

void WindowControl::DriveCurrentLiftPosition(intptr_t)
{
    NPercent100ths current{};
    NPercent100ths target{};
    NPercent100ths positionToSet{};

    VerifyOrReturn(Attributes::CurrentPositionLiftPercent100ths::Get(Endpoint(), current) == Status::Success);
    VerifyOrReturn(Attributes::TargetPositionLiftPercent100ths::Get(Endpoint(), target) == Status::Success);

    UpdateOperationalStatus(MoveType::LIFT, ComputeOperationalState(target, current));

    positionToSet.SetNonNull(CalculateSingleStep(MoveType::LIFT));
    LiftPositionSet(Endpoint(), positionToSet);

    // assume single move completed
    Instance().mInLiftMove = false;

    VerifyOrReturn(Attributes::CurrentPositionLiftPercent100ths::Get(Endpoint(), current) == Status::Success);

    if (!TargetCompleted(MoveType::LIFT, current, target))
    {
        // continue to move
        StartTimer(MoveType::LIFT, sMoveTimeoutMs);
    }
    else
    {
        // the OperationalStatus should indicate no-lift-movement after the target is completed
        UpdateOperationalStatus(MoveType::LIFT, ComputeOperationalState(target, current));
    }
}

chip::Percent100ths WindowControl::CalculateSingleStep(MoveType aMoveType)
{
    Status status{};
    chip::Percent100ths percent100ths{};
    chip::Percent100ths sPercentDelta{};
    NPercent100ths current{};
    NPercent100ths target{};
    OperationalState opState{};

    if (aMoveType == MoveType::LIFT)
    {
        status  = Attributes::CurrentPositionLiftPercent100ths::Get(Endpoint(), current);        
        if (status == Status::Success)
        {
            status  = Attributes::TargetPositionLiftPercent100ths::Get(Endpoint(), target);
        }
        opState = OperationalStateGet(Endpoint(), OperationalStatus::kLift);
    }
    else if (aMoveType == MoveType::TILT)
    {
        status  = Attributes::CurrentPositionTiltPercent100ths::Get(Endpoint(), current);
        if (status == Status::Success)
        {
            status  = Attributes::TargetPositionTiltPercent100ths::Get(Endpoint(), target);
        }
        opState = OperationalStateGet(Endpoint(), OperationalStatus::kTilt);
    }

    if ((status == Status::Success) && !current.IsNull() && !target.IsNull())
    {
        if(target.Value() != current.Value())
        {
            if(opState == OperationalState::MovingDownOrClose)
            {
                sPercentDelta = chip::min(WC_PERCENT100THS_MAX_CLOSED / 10, target.Value() - current.Value());
            }
            else if(opState == OperationalState::MovingUpOrOpen)
            {
                sPercentDelta = chip::min(WC_PERCENT100THS_MAX_CLOSED / 10, current.Value() - target.Value());
            }
        }
        else
        {
            sPercentDelta = WC_PERCENT100THS_MAX_CLOSED / 10;
        }
        percent100ths = ComputePercent100thsStep(opState, current.Value(), sPercentDelta);
    }
    else
    {
        ChipLogError(NotSpecified, "Cannot read the current lift position. Error: %d", static_cast<uint8_t>(status));
    }

    return percent100ths;
}

bool WindowControl::TargetCompleted(MoveType aMoveType, NPercent100ths aCurrent, NPercent100ths aTarget)
{
    return (OperationalState::Stall == ComputeOperationalState(aTarget, aCurrent));
}

void WindowControl::StartTimer(MoveType aMoveType, uint32_t aTimeoutMs)
{
    MoveType * moveType = chip::Platform::New<MoveType>();
    VerifyOrReturn(moveType != nullptr);

    *moveType = aMoveType;
    (void) chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Milliseconds32(aTimeoutMs), MoveTimerTimeoutCallback,
                                                       reinterpret_cast<void *>(moveType));
}

void WindowControl::MoveTimerTimeoutCallback(chip::System::Layer * systemLayer, void * appState)
{
    MoveType * moveType = reinterpret_cast<MoveType *>(appState);
    VerifyOrReturn(moveType != nullptr);

    if (*moveType == MoveType::LIFT)
    {
        chip::DeviceLayer::PlatformMgr().ScheduleWork(WindowControl::DriveCurrentLiftPosition);
    }
    else if (*moveType == MoveType::TILT)
    {
        chip::DeviceLayer::PlatformMgr().ScheduleWork(WindowControl::DriveCurrentTiltPosition);
    }

    chip::Platform::Delete(moveType);
}

void WindowControl::DriveCurrentTiltPosition(intptr_t)
{
    NPercent100ths current{};
    NPercent100ths target{};
    NPercent100ths positionToSet{};

    VerifyOrReturn(Attributes::CurrentPositionTiltPercent100ths::Get(Endpoint(), current) == Status::Success);
    VerifyOrReturn(Attributes::TargetPositionTiltPercent100ths::Get(Endpoint(), target) == Status::Success);

    UpdateOperationalStatus(MoveType::TILT, ComputeOperationalState(target, current));

    positionToSet.SetNonNull(CalculateSingleStep(MoveType::TILT));
    TiltPositionSet(Endpoint(), positionToSet);

    // assume single move completed
    Instance().mInTiltMove = false;

    VerifyOrReturn(Attributes::CurrentPositionTiltPercent100ths::Get(Endpoint(), current) == Status::Success);

    if (!TargetCompleted(MoveType::TILT, current, target))
    {
        // continue to move
        StartTimer(MoveType::TILT, sMoveTimeoutMs);
    }
    else
    {
        // the OperationalStatus should indicate no-tilt-movement after the target is completed
        UpdateOperationalStatus(MoveType::TILT, ComputeOperationalState(target, current));
    }
}

void WindowControl::StartMove(MoveType aMoveType)
{
    switch (aMoveType)
    {
    case MoveType::LIFT:
        if (!mInLiftMove)
        {
            mInLiftMove = true;
            StartTimer(aMoveType, sMoveTimeoutMs);
        }
        break;
    case MoveType::TILT:
        if (!mInTiltMove)
        {
            mInTiltMove = true;
            StartTimer(aMoveType, sMoveTimeoutMs);
        }
        break;
    default:
        break;
    };
}

void WindowControl::SetSingleStepTarget(OperationalState aDirection)
{
    UpdateOperationalStatus(mCurrentUIMoveType, aDirection);
    SetTargetPosition(aDirection, CalculateSingleStep(mCurrentUIMoveType));
}

void WindowControl::UpdateOperationalStatus(MoveType aMoveType, OperationalState aDirection)
{
    switch (aMoveType)
    {
    case MoveType::LIFT:
        OperationalStateSet(Endpoint(), OperationalStatus::kLift, aDirection);
        break;
    case MoveType::TILT:
        OperationalStateSet(Endpoint(), OperationalStatus::kTilt, aDirection);
        break;
    case MoveType::NONE:
        break;
    default:
        break;
    }
}

void WindowControl::SetTargetPosition(OperationalState aDirection, chip::Percent100ths aPosition)
{
    Status status{};
    if (Instance().mCurrentUIMoveType == MoveType::LIFT)
    {
        status = Attributes::TargetPositionLiftPercent100ths::Set(Endpoint(), aPosition);
    }
    else if (Instance().mCurrentUIMoveType == MoveType::TILT)
    {
        status = Attributes::TargetPositionTiltPercent100ths::Set(Endpoint(), aPosition);
    }

    if (status != Status::Success)
    {
        ChipLogError(NotSpecified, "Cannot set the target position. Error: %d", static_cast<uint8_t>(status));
    }
}

void WindowControl::PositionLEDUpdate(MoveType aMoveType)
{
    Status status{};
    NPercent100ths currentPosition{};

    if (aMoveType == MoveType::LIFT)
    {
        status = Attributes::CurrentPositionLiftPercent100ths::Get(Endpoint(), currentPosition);
        if (Status::Success == status && !currentPosition.IsNull())
        {
            Instance().SetBrightness(MoveType::LIFT, currentPosition.Value());
        }
    }
    else if (aMoveType == MoveType::TILT)
    {
        status = Attributes::CurrentPositionTiltPercent100ths::Get(Endpoint(), currentPosition);
        if (Status::Success == status && !currentPosition.IsNull())
        {
            Instance().SetBrightness(MoveType::TILT, currentPosition.Value());
        }
    }
}

void WindowControl::SetBrightness(MoveType aMoveType, uint16_t aPosition)
{
    uint8_t brightness = PositionToBrightness(aPosition, aMoveType);
    WindowMgr().InitiateAction(static_cast<WindowManager::MoveType_t>(aMoveType), WindowManager::LEVEL_ACTION, 0, &brightness);
}

uint8_t WindowControl::PositionToBrightness(uint16_t aPosition, MoveType aMoveType)
{
    AbsoluteLimits pwmLimits{};

    if (aMoveType == MoveType::LIFT)
    {
        pwmLimits.open   = mLiftIndicator.GetMinLevel();
        pwmLimits.closed = mLiftIndicator.GetMaxLevel();
    }
    else if (aMoveType == MoveType::TILT)
    {
        pwmLimits.open   = mTiltIndicator.GetMinLevel();
        pwmLimits.closed = mTiltIndicator.GetMaxLevel();
    }

    return Percent100thsToValue(pwmLimits, aPosition);
}

void WindowControl::SchedulePostAttributeChange(chip::EndpointId aEndpoint, chip::AttributeId aAttributeId)
{
    AttributeUpdateData * data = chip::Platform::New<AttributeUpdateData>();
    VerifyOrReturn(data != nullptr);

    data->mEndpoint    = aEndpoint;
    data->mAttributeId = aAttributeId;

    chip::DeviceLayer::PlatformMgr().ScheduleWork(DoPostAttributeChange, reinterpret_cast<intptr_t>(data));
}

void WindowControl::DoPostAttributeChange(intptr_t aArg)
{
    AttributeUpdateData * data = reinterpret_cast<AttributeUpdateData *>(aArg);
    VerifyOrReturn(data != nullptr);

    PostAttributeChange(data->mEndpoint, data->mAttributeId);
}
