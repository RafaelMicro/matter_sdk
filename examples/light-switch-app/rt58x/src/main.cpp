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

#include <AppTask.h>

#include "AppConfig.h"
#include "init_rt58xPlatform.h"
#include "init_light_switch_app_rt58xPlatform.h"
#include <DeviceInfoProviderImpl.h>

#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include "RT58xConfig.h"

#include <lib/core/CHIPError.h>

#if ENABLE_CHIP_SHELL
#include "matter_shell.h"
#endif

#define BLE_DEV_NAME "Rafael-Light"
using namespace ::chip;
using namespace ::chip::Inet;
using namespace ::chip::DeviceLayer;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer::Internal;

// ================================================================================
// Main Code
// ================================================================================
extern void cmd_rafael_init();
void ChipEventHandler(const ChipDeviceEvent * event, intptr_t /* arg */)
{
    switch (event->Type)
    {
    case DeviceEventType::kCHIPoBLEAdvertisingChange:
        break;
    case DeviceEventType::kThreadStateChange:
        break;
    case DeviceEventType::kThreadConnectivityChange:
        break;
    default:
        break;
    }
}

int main(void)
{
    CHIP_ERROR err;

    init_rt58xPlatform();
    init_light_switch_app_rt58xPlatform();
    
    err = chip::Platform::MemoryInit();
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "Platform::MemoryInit() failed");
        return 0;
    }

#if (ENABLE_CHIP_SHELL && (CHIP_CONFIG_ENABLE_ICD_SERVER == 0))
    startShellTask();
    cmd_rafael_init();
#endif
    info( "==================================================\n");
    info( "chip-rt58x-light-switch-example(Matter 1.2) starting Version %d\r\n", CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION);
    info( "==================================================\n\n");

    err = PlatformMgr().InitChipStack();
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "PlatformMgr().InitChipStack() failed");
        goto exit;
    }

    err = GetAppTask().StartAppTask();
    if (err != CHIP_NO_ERROR)
    {
       ChipLogError(NotSpecified, "GetAppTask().StartAppTask() failed %s", ErrorStr(err));
    }

    vTaskStartScheduler();


exit:    
    return 0;
}