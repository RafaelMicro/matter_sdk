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

#include "AppConfig.h"
#include <lib/support/CHIPPlatformMemory.h>
#include <platform/CHIPDeviceLayer.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <assert.h>
#include <string.h>

#include <mbedtls/platform.h>

#if CHIP_ENABLE_OPENTHREAD
#include <openthread-core-config.h>
#include <openthread/cli.h>
#include <openthread/config.h>
#include <openthread/dataset.h>
#include <openthread/error.h>
#include <openthread/heap.h>
#include <openthread/icmp6.h>
#include <openthread/instance.h>
#include <openthread/link.h>
#include <openthread/platform/openthread-system.h>
#include <openthread/tasklet.h>
#include <openthread/thread.h>
#include <utils/uart.h>


#if OPENTHREAD_CONFIG_HEAP_EXTERNAL_ENABLE
#include "openthread/heap.h"
#endif // OPENTHREAD_CONFIG_HEAP_EXTERNAL_ENABLE
#endif // CHIP_ENABLE_OPENTHREAD

#include "init_rt58x_platform.h"
#include "lmac15p4.h"
#include "uart.h"
#include "mcu.h"
#include "rt_crypto.h"
#include "EnhancedFlashDataset.h"
#include "hosal_wdt.h"
#include "hosal_rf.h"
#include "hosal_dma.h"
#include "hosal_gpio.h"
#include "hosal_crypto_aes.h"
#include "flashctl.h"
#include "hosal_sysctrl.h"
#include "app_hooks.h"
#include "dump_boot_info.h"
#if (ENABLE_CHIP_SHELL && (CHIP_CONFIG_ENABLE_ICD_SERVER == 0))
#include "cli.h"
#endif
#include "log.h"

static bool do_factory_reset;
extern void __libc_init_array(void);

static void wdt_isr(void)
{
    hosal_wdt_kick();
}
void write_reboot_count(void) {
    uint8_t reboot_count;
    size_t actual_len;
    efd_get_env_blob("reboot", (void *) &reboot_count, sizeof(uint8_t), &actual_len);
    reboot_count +=1;
    efd_set_env_blob("reboot", (void *) &reboot_count, sizeof(uint8_t));
    delay_ms(500);
    if(reboot_count >= 5) {
        do_factory_reset = 1;
    }
    reboot_count = 0;
    efd_set_env_blob("reboot", (void *) &reboot_count, sizeof(uint8_t));
}

bool rt58x_factory_reset_check(void)
{
    return do_factory_reset;
}

static void wdt_init(void)
{
    hosal_wdt_config_mode_t hal_wdt_cfg;
    hosal_wdt_config_tick_t hal_wdt_tick_cfg;

    hal_wdt_cfg.int_enable = 1;
    hal_wdt_cfg.reset_enable = 1;
    hal_wdt_cfg.lock_enable = 0;
    hal_wdt_cfg.prescale = HOSAL_WDT_PRESCALE_32;

    hal_wdt_tick_cfg.wdt_ticks = 10000 * 1000;
    hal_wdt_tick_cfg.int_ticks = 200 * 1000;
    hal_wdt_tick_cfg.wdt_min_ticks = 0;

    hosal_wdt_start(hal_wdt_cfg, hal_wdt_tick_cfg, (void*)wdt_isr);
    NVIC_EnableIRQ(Wdt_IRQn);
    NVIC_SetPriority(Wdt_IRQn, 0x01);
}
void init_rt58x_platform(void)
{
    vHeapRegionsInt();
    __libc_init_array();
    wdt_init();
    uartConsoleInit();
    _dump_boot_info();
    enhanced_flash_dataset_init();
    //efd_env_set_default();
}
#ifdef __cplusplus
}
#endif
