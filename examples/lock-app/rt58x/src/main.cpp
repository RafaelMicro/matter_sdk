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
#include "init_rt58x_platform.h"
#include "init_device_environment.h"
#include <DeviceInfoProviderImpl.h>

#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include "RT58xConfig.h"

#include <lib/core/CHIPError.h>

#if ENABLE_CHIP_SHELL
#include "matter_shell.h"
#endif

using namespace ::chip;
using namespace ::chip::Inet;
using namespace ::chip::DeviceLayer;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer::Internal;

// ================================================================================
// Main Code
// ================================================================================
int main(void)
{
    CHIP_ERROR err;

    init_rt58x_platform();
    init_device_environment();

    err = chip::Platform::MemoryInit();
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "Platform::MemoryInit() failed");
        return 0;
    }

    ChipLogProgress(NotSpecified, "=============================================================================");
    ChipLogProgress(NotSpecified, "Rafael-Door-Lock-example(Matter 1.5) starting Version %d", CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION);
    ChipLogProgress(NotSpecified, "=============================================================================");

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
#if (ENABLE_CHIP_SHELL && (CONFIG_HOSAL_SOC_IDLE_SLEEP == 0))
    startShellTask();
#endif
    vTaskStartScheduler();

exit:    
    return 0;
}
