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

#include "init_rt58xPlatform.h"
#include "uart.h"
#include "cm3_mcu.h"
#include "crypto_util.h"
#include "EnhancedFlashDataset.h"

#define RANDOM_TIME_MAX        (10*100)
#define RANDOM_TIME_MIN        (1*100)

static void init_default_pin_mux(void)
{
    int i, j;

    /*set all pin to gpio, except GPIO16, GPIO17 */
    for (i = 0; i < 32; i++)
    {
        if ((i != 16) && (i != 17))
        {
            pin_set_mode(i, MODE_GPIO);
        }
    }
    /*uart0 pinmux*/
    pin_set_mode(16, MODE_UART); /*GPIO16 as UART0 RX*/
    pin_set_mode(17, MODE_UART); /*GPIO17 as UART0 TX*/
    return;
}

static void wdt_isr(void)
{
    Wdt_Kick();
}

static void init_wdt_init(void)
{
    wdt_config_mode_t wdt_mode;
    wdt_config_tick_t wdt_cfg_ticks;

    wdt_mode.int_enable = 1;
    wdt_mode.reset_enable = 1;
    wdt_mode.lock_enable = 0;
    wdt_mode.prescale = WDT_PRESCALE_32;

    wdt_cfg_ticks.wdt_ticks = 1200 * 1000;
    wdt_cfg_ticks.int_ticks = 200 * 1000;
    wdt_cfg_ticks.wdt_min_ticks = 0;

    Wdt_Start(wdt_mode, wdt_cfg_ticks, wdt_isr);
}

void init_rt58xPlatform(void)
{
    uint32_t random = 0;
    NVIC_SetPriority(CommSubsystem_IRQn, 0x04);
    NVIC_SetPriority(Wdt_IRQn, 0x01);
    
    init_default_pin_mux();

    Delay_Init();

    //random = ((RANDOM_TIME_MAX - RANDOM_TIME_MIN) * (get_random_number() % 10000)) / ((RANDOM_TIME_MAX + 100) + RANDOM_TIME_MIN);

    //Delay_ms(random);

#if(CHIP_CONFIG_ENABLE_ICD_SERVER != 1)    
    init_wdt_init();
#endif
    
    dma_init();
    uartConsoleInit();
    crypto_lib_init();

   #if (LPWR_FLASH_PROTECT_ENABLE==1 && CHIP_CONFIG_ENABLE_ICD_SERVER != 1)
   flash_vbat_protect_init();
   flash_cmp_protect_init();
   #endif

    enhanced_flash_dataset_init();
    
#if(CHIP_CONFIG_ENABLE_ICD_SERVER == 1)
    timern_t *TIMER;
    TIMER = TIMER4;
    NVIC_DisableIRQ((IRQn_Type)(Timer4_IRQn));
    NVIC_SetPriority((IRQn_Type)(Timer4_IRQn), 1);

    TIMER->LOAD = 0;
    TIMER->CLEAR = 0;
    TIMER->CONTROL.reg = 0;

    TIMER->CONTROL.bit.PRESCALE = 0;
    TIMER->CONTROL.bit.MODE = 0;
    TIMER->CONTROL.bit.EN = 0;

    Lpm_Set_Low_Power_Level(LOW_POWER_LEVEL_SLEEP0);
    Lpm_Enable_Low_Power_Wakeup(LOW_POWER_WAKEUP_GPIO);
    Lpm_Enable_Low_Power_Wakeup(LOW_POWER_WAKEUP_32K_TIMER);
#endif  
}

// void kvs_init(void)
// {
//     for (uint16_t keyValueMappingTalbePageIndex = 0xFB000; keyValueMappingTalbePageIndex < 0xFB800; keyValueMappingTalbePageIndex++) {
//         flash_write_byte(keyValueMappingTalbePageIndex, 0);
//         while (flash_check_busy()) {}
//     }
// }

#ifdef __cplusplus
}
#endif
