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
//0x1F0000~0x1F2000: openthread
//0x1F2000~0x1F3000: check factory reset
//0x1F3000~0x1F4000: reserved
#define CHECK_RESET_START_ADDRESS 0x1F2000 
#define CHECK_RESET_BLOCK_SIZE 0x1000

void init_rt58xPlatform(void);
void kvs_init(void);
bool rt58x_factory_reset_check(void);
void write_reboot_count(void);

#ifdef __cplusplus
}
#endif
