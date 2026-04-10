/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
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

#include "AppTask.h"
#include "BoltLockManager.h"

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>
#include <app/clusters/door-lock-server/door-lock-server.h>
#include <app/data-model/Nullable.h>
#include <assert.h>
#include <lib/support/logging/CHIPLogging.h>

using namespace ::chip;
using namespace chip::app;
using namespace chip::app::DataModel;
using namespace ::chip::app::Clusters;
using namespace ::chip::app::Clusters::DoorLock;
using chip::Protocols::InteractionModel::Status;

void MatterPostAttributeChangeCallback(const chip::app::ConcreteAttributePath & path, uint8_t type, uint16_t size, uint8_t * value)
{
    EndpointId  endpoint    = path.mEndpointId;
    ClusterId   clusterId   = path.mClusterId;
    AttributeId attributeId = path.mAttributeId;
    ChipLogProgress(NotSpecified, "Cluster 0x%x Ep %x Attr %x", clusterId, endpoint, attributeId);

    if (clusterId == DoorLock::Id)
    {
        switch (*value)
        {
        case to_underlying(DlLockState::kLocked):
            BoltLockMgr().InitiateAction(0, BoltLockManager::LOCK_ACTION);
            break;
        case to_underlying(DlLockState::kUnlocked):
            BoltLockMgr().InitiateAction(0, BoltLockManager::UNLOCK_ACTION);
            break;
        default:
            break;
        }
    }
}

bool emberAfPluginDoorLockGetUser(EndpointId endpointId, uint16_t userIndex, EmberAfPluginDoorLockUserInfo & user)
{
    return BoltLockMgr().GetUser(userIndex, user);
}

bool emberAfPluginDoorLockSetUser(EndpointId endpointId, uint16_t userIndex, FabricIndex creator, FabricIndex modifier,
                                  const CharSpan & userName, uint32_t uniqueId, UserStatusEnum userStatus, UserTypeEnum userType,
                                  CredentialRuleEnum credentialRule, const CredentialStruct * credentials, size_t totalCredentials)
{
    return BoltLockMgr().SetUser(userIndex, creator, modifier, userName, uniqueId, userStatus, userType, credentialRule,
                                 credentials, totalCredentials);
}

bool emberAfPluginDoorLockGetCredential(EndpointId endpointId, uint16_t credentialIndex, CredentialTypeEnum credentialType,
                                        EmberAfPluginDoorLockCredentialInfo & credential)
{
    return BoltLockMgr().GetCredential(credentialIndex, credentialType, credential);
}

bool emberAfPluginDoorLockSetCredential(EndpointId endpointId, uint16_t credentialIndex, FabricIndex creator, FabricIndex modifier,
                                        DlCredentialStatus credentialStatus, CredentialTypeEnum credentialType,
                                        const ByteSpan & secret)
{
    return BoltLockMgr().SetCredential(credentialIndex, creator, modifier, credentialStatus, credentialType, secret);
}

static bool setLockStateWithCredentials(chip::EndpointId endpointId, const Nullable<chip::FabricIndex> & fabricIdx,
                                        const Nullable<chip::NodeId> & nodeId, const Optional<ByteSpan> & pinCode,
                                        OperationErrorEnum & err, DlLockState newState)
{
    // No PIN — lock/unlock without user/credential association
    if (!pinCode.HasValue())
    {
        return DoorLockServer::Instance().SetLockState(endpointId, newState, OperationSourceEnum::kRemote,
                                                       NullNullable, NullNullable, fabricIdx, nodeId);
    }

    // Find the matching PIN credential (1-based index)
    uint16_t credentialIndex = 0;
    EmberAfPluginDoorLockCredentialInfo credential;
    for (uint16_t i = 1; i <= CONFIG_LOCK_NUM_CREDENTIALS; i++)
    {
        if (!BoltLockMgr().GetCredential(i, CredentialTypeEnum::kPin, credential))
        {
            continue;
        }
        if (credential.status != DlCredentialStatus::kAvailable &&
            credential.credentialType == CredentialTypeEnum::kPin &&
            credential.credentialData.data_equal(pinCode.Value()))
        {
            credentialIndex = i;
            break;
        }
    }

    if (credentialIndex == 0)
    {
        err = OperationErrorEnum::kInvalidCredential;
        return false;
    }

    // Find the user associated with this credential (1-based index)
    Nullable<uint16_t> userIndex = NullNullable;
    EmberAfPluginDoorLockUserInfo user;
    for (uint16_t i = 1; i <= CONFIG_LOCK_NUM_USERS; i++)
    {
        if (!BoltLockMgr().GetUser(i, user))
        {
            continue;
        }
        for (const auto & cred : user.credentials)
        {
            if (cred.credentialType == CredentialTypeEnum::kPin && cred.credentialIndex == credentialIndex)
            {
                userIndex = MakeNullable(i);
                break;
            }
        }
        if (!userIndex.IsNull())
        {
            break;
        }
    }

    LockOpCredentials userCredential[] = { { CredentialTypeEnum::kPin, credentialIndex } };
    auto userCredentials               = MakeNullable<List<const LockOpCredentials>>(userCredential);

    return DoorLockServer::Instance().SetLockState(endpointId, newState, OperationSourceEnum::kRemote,
                                                   userIndex, userCredentials, fabricIdx, nodeId);
}

bool emberAfPluginDoorLockOnDoorLockCommand(chip::EndpointId endpointId, const Nullable<chip::FabricIndex> & fabricIdx,
                                            const Nullable<chip::NodeId> & nodeId, const Optional<ByteSpan> & pinCode,
                                            OperationErrorEnum & err)
{
    return setLockStateWithCredentials(endpointId, fabricIdx, nodeId, pinCode, err, DlLockState::kLocked);
}

bool emberAfPluginDoorLockOnDoorUnlockCommand(chip::EndpointId endpointId, const Nullable<chip::FabricIndex> & fabricIdx,
                                              const Nullable<chip::NodeId> & nodeId, const Optional<ByteSpan> & pinCode,
                                              OperationErrorEnum & err)
{
    return setLockStateWithCredentials(endpointId, fabricIdx, nodeId, pinCode, err, DlLockState::kUnlocked);
}

void emberAfDoorLockClusterInitCallback(EndpointId endpoint)
{
    DataModel::Nullable<chip::app::Clusters::DoorLock::DlLockState> lockstate;
    DoorLock::Attributes::LockState::Get(1, lockstate);

    DoorLockServer::Instance().InitServer(endpoint);
    if (!lockstate.IsNull())
    {
        DoorLock::Attributes::LockState::Set(1, lockstate);
    }
    const auto logOnFailure = [](Status status, const char * attributeName) {
        if (status != Status::Success)
        {
            ChipLogError(Zcl, "Failed to set DoorLock %s: %x", attributeName, status);
        }
    };

    logOnFailure(DoorLock::Attributes::LockType::Set(endpoint, DlLockType::kDeadBolt), "type");
    logOnFailure(DoorLock::Attributes::NumberOfTotalUsersSupported::Set(endpoint, CONFIG_LOCK_NUM_USERS), "number of users");
    logOnFailure(DoorLock::Attributes::NumberOfPINUsersSupported::Set(endpoint, CONFIG_LOCK_NUM_USERS), "number of PIN users");
    logOnFailure(DoorLock::Attributes::NumberOfRFIDUsersSupported::Set(endpoint, 0), "number of RFID users");
    logOnFailure(DoorLock::Attributes::NumberOfCredentialsSupportedPerUser::Set(endpoint, CONFIG_LOCK_NUM_CREDENTIALS_PER_USER),
                 "number of credentials per user");

    // Set FeatureMap to (kUser|kPinCredential), default is:
    // (kUser|kAccessSchedules|kRfidCredential|kPinCredential) 0x113
    logOnFailure(DoorLock::Attributes::FeatureMap::Set(endpoint, 0x101), "feature map");
}
