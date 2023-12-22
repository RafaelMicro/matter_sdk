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
/*******************************************************************************/
#pragma once

#include "util_log.h"

#ifdef __cplusplus
extern "C" {
#endif

void init_lighting_app_rt58xPlatform(void);
void init_rt58x_led_flash(uint32_t pin, uint32_t onTimeMs, uint32_t offTimeMs);
void rt58x_led_level_ctl(uint32_t id, uint8_t current_lv);

#ifdef __cplusplus
}
#endif
