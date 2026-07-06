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

#include <lib/core/DataModelTypes.h>
#include <lib/support/CHIPMem.h>
#include <lib/support/CHIPPlatformMemory.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/KeyValueStoreManager.h>
#include "app/clusters/identify-server/IdentifyCluster.h"
#include "app/server-cluster/ServerClusterInterfaceRegistry.h"
#include "app/DefaultTimerDelegate.h"

class RafaelIdentifyDelegate : public chip::app::Clusters::IdentifyDelegate
{
public:
    RafaelIdentifyDelegate(chip::EndpointId endPointId) : mIdentifyTimer(nullptr), mIdentifyFinished(0), mIdentifyOnOff(0), mEndPointId(endPointId) {}
    void OnIdentifyStart(chip::app::Clusters::IdentifyCluster & cluster) override;

    void OnIdentifyStop(chip::app::Clusters::IdentifyCluster & cluster) override;

    void OnTriggerEffect(chip::app::Clusters::IdentifyCluster & cluster) override;

    bool IsTriggerEffectEnabled() const override { return true; }
    static void OnTriggerIdentifyEffectBlink(chip::System::Layer * systemLayer, void * appState);
    static void OnTriggerIdentifyEffectBreathe(chip::System::Layer * systemLayer, void * appState);
    static void OnTriggerIdentifyEffectOk(chip::System::Layer * systemLayer, void * appState);
    static void OnTriggerIdentifyEffectChannel(chip::System::Layer * systemLayer, void * appState);
    static void OnTriggerIdentifyEffectCompleted(chip::System::Layer * systemLayer, void * appState);

    void SetIdentifyCallback(void (*onToggle)(uint8_t),
                             void (*onStop)(void))
    {
        mIdentifyToggleCallback = onToggle;
        mIdentifyStopCallback  = onStop;
    }
    void SetIdentifyTimer(void);
    uint8_t IsIdentifying() { return mIdentifyFinished==0; };
    static void IdentifyTimerEventHandler(TimerHandle_t xTimer);
    chip::EndpointId mEndPointId = 1;
private:
    void (*mIdentifyToggleCallback)(uint8_t) = nullptr;
    void (*mIdentifyStopCallback)(void)  = nullptr;
    TimerHandle_t mIdentifyTimer;
    StaticTimer_t mIdentifyTimerStruct;
    uint8_t mIdentifyFinished;
    uint8_t mIdentifyOnOff;
    chip::app::Clusters::Identify::EffectIdentifierEnum mIdentifyEffect = chip::app::Clusters::Identify::EffectIdentifierEnum::kStopEffect;
};

#ifdef CHIP_CONFIG_USE_SUBSCRIPTION_CALLBACKS
#include "SubscriptionCallback.h"
#endif // CHIP_CONFIG_USE_SUBSCRIPTION_CALLBACKS

CHIP_ERROR IdentifyInit(void (*onToggle)(uint8_t), void (*onStop)(void));
void MatterFotaInit(void);
void MatterNetworkInit(void);
void DoFactoryReset(intptr_t arg);
