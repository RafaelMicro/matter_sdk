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

#ifdef __cplusplus
extern "C" {
#endif
#include <assert.h>
#include <string.h>

#include "init_lock_app_rt582Platform.h"
#include "cm3_mcu.h"

typedef struct {
    uint32_t onTimeMs;
    uint32_t offTimeMs;
    uint32_t nowTimeMs;
    uint32_t state  : 1;
    uint32_t used   : 1;
    uint32_t        : 30;
} flash_info_t;

static flash_info_t flash_table[32];

static void init_lock_app_pin_mux(void)
{
    gpio_cfg_output(20);
    gpio_cfg_output(21);
    gpio_pin_set(20);
    gpio_pin_set(21);
    return;
}

static void _timer_isr_handler(uint32_t timer_id)
{
    int i;
    for(i=0;i<32;i++)
    {
        if(!flash_table[i].used)
            continue;

        if(flash_table[i].state == 0)
        {
            flash_table[i].nowTimeMs++;
            if(flash_table[i].nowTimeMs > flash_table[i].offTimeMs)
            {
                flash_table[i].state = 1;
                gpio_pin_clear(i);
                flash_table[i].nowTimeMs = 0;
            }          
        }
        else
        {
            flash_table[i].nowTimeMs++;
            if(flash_table[i].nowTimeMs > flash_table[i].onTimeMs)
            {
                flash_table[i].state = 0;
                gpio_pin_set(i);
                flash_table[i].nowTimeMs = 0;
            }            
        }
    }
    //printf("2");
}

void init_rt582_led_flash(uint32_t pin, uint32_t onTimeMs, uint32_t offTimeMs)
{
    if(onTimeMs == 0 && offTimeMs == 0)
    {
        flash_table[pin].state = 0;
        flash_table[pin].used = 0;
    }
    else
    {
        flash_table[pin].onTimeMs = onTimeMs;
        flash_table[pin].offTimeMs = offTimeMs;
        flash_table[pin].state = 0;
        flash_table[pin].used = 1;
    }
    gpio_pin_set(pin);
}

void init_lock_app_rt582Platform(void)
{
    timer_config_mode_t cfg;

    init_lock_app_pin_mux();

    cfg.int_en = ENABLE;
    cfg.mode = TIMER_PERIODIC_MODE;
    cfg.prescale = TIMER_PRESCALE_32;

    Timer_Open(2, cfg, _timer_isr_handler);
    Timer_Int_Priority(2, 3);

    Timer_Start(2, 999);

    memset(flash_table, 0, sizeof(flash_table));    
}

#ifdef __cplusplus
}
#endif
