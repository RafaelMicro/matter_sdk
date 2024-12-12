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

#include "init_device_environment.h"
#include "cm3_mcu.h"


static void init_pin_mux(void)
{
    gpio_cfg_output(20);
    gpio_cfg_output(21);
    gpio_pin_set(20);
    gpio_pin_set(21);
    return;
}

void smart_plug_trigger(uint32_t state)
{
    gpio_pin_write(21, !state);
}

void init_device_environment(void)
{
    init_pin_mux(); 
}

#ifdef __cplusplus
}
#endif
