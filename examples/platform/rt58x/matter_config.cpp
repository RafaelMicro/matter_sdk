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

#include "AppConfig.h"
#include "OTAConfig.h"
#include <matter_config.h>

#include <FreeRTOS.h>
#include "task.h"

#include <mbedtls/platform.h>

#ifdef ENABLE_CHIP_SHELL
#include "matter_shell.h"
#endif

#include "mcu.h"
#include "log.h"
#include <EnhancedFlashDataset.h>
#include "fota_define.h"
#include "flashctl.h"

// ================================================================================
// FreeRTOS Callbacks
// ================================================================================
extern "C" void vApplicationIdleHook(void)
{
    otTaskletsSignalPending(0);
    // FreeRTOS Idle callback
    // Check CHIP Config nvm3 and repack flash if necessary.
    //Internal::RT58xConfig::RepackNvm3Flash();
}
void MatterFotaInit(void)
{
    fota_information_t  *p_fota_info = (fota_information_t *)(FOTA_UPDATE_BANK_INFO_ADDRESS);

    if (p_fota_info->fotabank_ready == FOTA_IMAGE_READY)
    { 
        if (p_fota_info->fota_result == FOTA_RESULT_SUCCESS)
        {
            log_error("fota success\r\n");
        } 
        else
        {
            log_error("fota failed: %d\r\n", p_fota_info->fota_result);
        }  
        while (flash_check_busy());
        taskENTER_CRITICAL();
        flash_erase(FLASH_ERASE_SECTOR, FOTA_UPDATE_BANK_INFO_ADDRESS);
        taskEXIT_CRITICAL();
    }
}
