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

#include "timers.h"
#include "AppConfig.h"
#include "init_rt58x_platform.h"
#include "init_device_environment.h"
#include "EnhancedFlashDataset.h"
#include <DeviceInfoProviderImpl.h>
#include <crypto/CHIPCryptoPAL.h>
#include <lib/support/CHIPPlatformMemory.h>

#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include "RT58xConfig.h"

#include <lib/core/CHIPError.h>
#include "zigbee_platform.h"
#include "zigbee_zcl_msg_handler.h"
#include "zigbee_api.h"
#include "hosal_rf.h"
#include "MultiApp.h"

#include <mbedtls/aes.h>
#include <mbedtls/platform.h>

#if ENABLE_CHIP_SHELL
#include "matter_shell.h"
#endif

using namespace ::chip;
using namespace ::chip::Inet;
using namespace ::chip::DeviceLayer;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer::Internal;

static uint8_t g_joined_network = 0;
static uint16_t g_panid = 0xFFFF;
static uint16_t g_short_addr = 0xFFFF;
static uint8_t g_joined_channel = 0xFF;

// ================================================================================
// Main Code
// ================================================================================

int main(void)
{
    CHIP_ERROR err;

    init_rt58x_platform();
    init_lighting_app_rt58xPlatform();
    hosal_rf_init(HOSAL_RF_MODE_MULTI_PROTOCOL);

    GetMultiApp().Init();

    if(GetMultiApp().GetCurrentProtocol() == MultiApp::CurrentProtocol::kNone ||
        GetMultiApp().GetCurrentProtocol() == MultiApp::CurrentProtocol::kMatter)
    {
        err = chip::Platform::MemoryInit();
        if (err != CHIP_NO_ERROR)
        {
            ChipLogError(NotSpecified, "Platform::MemoryInit() failed");
            return 0;
        }

        ChipLogProgress(NotSpecified, "==================================================");
        ChipLogProgress(NotSpecified, "Rafael-Light-example(Matter 1.5) starting Version %d", CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION);
        ChipLogProgress(NotSpecified, "==================================================");

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
    }
    if(GetMultiApp().GetCurrentProtocol() == MultiApp::CurrentProtocol::kNone ||
        GetMultiApp().GetCurrentProtocol() == MultiApp::CurrentProtocol::kZigbee)
    {
        zigbee_app_init();
        zbStart();
    }
    vTaskStartScheduler();

exit:
    return 0;
}

void app_main_loop(void* parameters_ptr) {
    zb_app_event_t sevent = ZB_APP_EVENT_NONE;

    ZB_THREAD_SAFE(
        ZB_AF_REGISTER_DEVICE_CTX(&simple_desc_light_ctx);
        for (int i = 0; i < simple_desc_light_ctx.ep_count; i++) {
            ZB_AF_SET_ENDPOINT_HANDLER(
                simple_desc_light_ctx.ep_desc_list[i]->ep_id,
                zigbee_zcl_msg_handler);
        }
    )
    ZIGBEE_APP_NOTIFY(ZB_APP_EVENT_INIT);

    for (;;) {
        if (ulTaskNotifyTake(pdFALSE, portMAX_DELAY) != 0) {
            ZIGBEE_APP_GET_NOTIFY(sevent);

            switch (sevent) {
                case ZB_APP_EVENT_INIT: {
                    zigbee_app_nwk_start(ZIGBEE_CHANNEL_ALL_MASK(), 32, 0);
                    log_info("ZigBee APP init");
                } break;

                case ZB_APP_EVENT_NOT_JOINED: {

                    ZB_THREAD_SAFE(bdb_start_top_level_commissioning(
                        ZB_BDB_NETWORK_STEERING));
                    log_info("ZigBee APP not joined");
                } break;

                case ZB_APP_EVENT_JOINED: {
                    if(GetMultiApp().GetCurrentProtocol() == MultiApp::CurrentProtocol::kNone)
                    {
                        GetMultiApp().SuspendMatterTask();
                        GetMultiApp().SetCurrentProtocol(MultiApp::CurrentProtocol::kZigbee);
                    }
                    log_info("ZigBee APP joined");
                    g_joined_network = 1;
                    scene_db_check();
                    startup_db_check();
                    set_startup_status();

                    ZB_THREAD_SAFE(g_panid = zb_get_pan_id();
                                   g_short_addr = zb_get_short_address();
                                   g_joined_channel = zb_get_current_channel();)

                    log_info("PAN ID: %04X, Short Addr: %04X, Channel: %d",
                             g_panid, g_short_addr, g_joined_channel);

                } break;
                case ZB_APP_EVENT_FACTORY_RESET: {
                    zigbee_do_factory_reset();
                } break;
                case ZB_APP_EVENT_HANDLE_BLE_MESSAGE: {
                    ble_msg_t msg;
                    while (xQueueReceive(g_ble_msg_queue, &msg, 0) == pdTRUE)
                    {
                        // TODO: handle BLE message (msg.data, msg.len)
                        log_info("[Zigbee] Receive BLE message:");
                        log_info_hexdump("", msg.data, msg.len);
                        BLEMgrImpl().BLESendMessage(msg.data, msg.len);
                        vPortFree(msg.data);
                    }
                } break;
                default: break;
            }
        }
    }
}
