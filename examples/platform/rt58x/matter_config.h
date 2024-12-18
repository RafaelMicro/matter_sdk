/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2022 Silabs.
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

#include <lib/support/CHIPMem.h>
#include <lib/support/CHIPPlatformMemory.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/KeyValueStoreManager.h>

#ifdef CHIP_CONFIG_USE_SUBSCRIPTION_CALLBACKS
#include "SubscriptionCallback.h"
#endif // CHIP_CONFIG_USE_SUBSCRIPTION_CALLBACKS

class RT58xMatterConfig
{
public:
    static CHIP_ERROR InitMatter(const char * appName);

#ifdef CHIP_CONFIG_USE_SUBSCRIPTION_CALLBACKS
    static SubscriptionCallback mSubscriptionHandler;
#endif // CHIP_CONFIG_USE_SUBSCRIPTION_CALLBACKS

private:
    static CHIP_ERROR InitOpenThread(void);
    static void InitWiFi(void);
    static void ConnectivityEventCallback(const chip::DeviceLayer::ChipDeviceEvent * event, intptr_t arg);
    static void InitOTARequestorHandler(chip::System::Layer * systemLayer, void * appState);
    
};
