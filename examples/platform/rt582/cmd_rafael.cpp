/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
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

#include <lib/shell/Engine.h>

#include <crypto/RandUtils.h>
#include <lib/core/CHIPCore.h>
#include <lib/support/Base64.h>
#include <lib/support/CHIPArgParser.hpp>
#include <lib/support/CodeUtils.h>

#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ChipShellCollection.h>

#include "util_string.h"
#include "util_log.h"

#include "sys_arch.h"
#include "mib_counters.h"
#include "EnhancedFlashDataset.h"

#include "FreeRTOS.h"
#include "task.h"


#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

using namespace chip;
using namespace chip::Shell;
using namespace chip::Logging;

CHIP_ERROR cmd_reset(int argc, char ** argv)
{
    const TickType_t xDelay250ms = pdMS_TO_TICKS(250UL);

    vTaskDelay(xDelay250ms);

    NVIC_DisableIRQ((IRQn_Type)Wdt_IRQn);

    return CHIP_NO_ERROR;
}

CHIP_ERROR cmd_mem(int argc, char ** argv)
{
    sys_malloc_info_printf();

    return CHIP_NO_ERROR;
}

CHIP_ERROR cmd_mib(int argc, char ** argv)
{
    mib_counter_printf();

    return CHIP_NO_ERROR;
}

CHIP_ERROR cmd_env(int argc, char ** argv)
{
    info_env();

    return CHIP_NO_ERROR;
}

CHIP_ERROR cmd_fbench(int argc, char ** argv)
{
    /* full chip benchmark test */
    int result,i,j,w_cnt = 0,r_cnt = 0,d_cnt =0;
    uint32_t start_time, time_cast, total_bytes = 0,avg_time =0 ;;
    size_t write_size, cur_op_size;
    uint8_t *write_data, *read_data;
    size_t read_len;


    char key[] = "Test";

    sys_set_random_seed(xTaskGetTickCount());

    for(j = 0;j<5000;j++)
    {
        info(".");
    do
    {
        write_size = sys_random() % 1024;
    } while (write_size==0);
    
    write_data = (uint8_t *)pvPortMalloc(write_size);
    read_data = (uint8_t *)pvPortMalloc(write_size);

    if (write_data && read_data)
    {
        total_bytes += write_size;
        for (i = 0; i < write_size; i ++) {
            write_data[i] = i & 0xFF;
        }

        /* benchmark testing */
        /* write test */
        start_time = xTaskGetTickCount();

        efd_port_env_lock();

        if (write_data && write_size)
        {
            result = efd_set_env_blob(key, write_data, write_size);
        }

        efd_port_env_unlock();        

        if (result == EFD_NO_ERR)
        {
            time_cast = xTaskGetTickCount() - start_time;

            if(j==0)
                avg_time = time_cast;
            avg_time = (time_cast + avg_time)/2;
            w_cnt++;
        }
        else
        {
            err("Write benchmark has an error. Error code: %d.\n", result);
        }
        /* read test */
        start_time = xTaskGetTickCount();

        efd_port_env_lock();

        efd_get_env_blob(key, read_data, write_size, &read_len);

        efd_port_env_unlock();  

        if(!memcmp(read_data, write_data, write_size))
        {
            time_cast = xTaskGetTickCount() - start_time;
            r_cnt++;
        }
        else
        {
            err("Read benchmark has an error. Compare Error\n");
        }

        start_time = xTaskGetTickCount();

        efd_port_env_lock();
        efd_del_env(key);
        efd_port_env_unlock();  
        if (result >= 0)
        {
            time_cast = xTaskGetTickCount() - start_time;
            d_cnt++;
        }
        else
        {
            err("Delete benchmark has an error. Error code: %d.\n", result);
        }
    }
    else
    {
        info("Low memory!\n");
    }
    vPortFree(write_data);
    vPortFree(read_data);
    }

    info("\nTotal Run: %d, W: %d, R: %d, D: %d, Bytes: %d\n", j, w_cnt, r_cnt, d_cnt, total_bytes);
    info("Avg Write cost time %dmS\n", avg_time);
    return CHIP_NO_ERROR;   
}

CHIP_ERROR cmd_rd(int argc, char ** argv)
{
    do
    {
        uint32_t    addr = 0, cnt = 0;
        uint32_t    *pCur = 0;

        if ((argc != 1) && (argc != 2))
        {
            err("wrong parameters\n");
            break;
        }

        addr = (*(argv[0] + 1) == 'x')
               ? utility_strtox(argv[0] + 2, 0)
               : utility_strtox(argv[0], 0);

        if (argc == 1)
        {
            cnt = 64;
        }
        else
        {
            cnt  = (*(argv[1] + 1) == 'x')
                   ? utility_strtox(argv[1] + 2, 0)
                   : utility_strtox(argv[1], 0);
        }

        pCur = (uint32_t *)addr;
        //cnt  = (cnt + 0x3) >> 2;

        util_log_mem(UTIL_LOG_INFO, "  ", (uint8_t *)pCur, cnt, 0);

        info("\n\n");
    } while (0);

    return CHIP_NO_ERROR;
}

static shell_command_t cmds_rafael[] = {
    { &cmd_rd,     "rd",    "Read memory" },
    { &cmd_mem,    "mem",   "Show memory usage" },
    { &cmd_mib,    "mib",   "Show mib counters" },
    { &cmd_env,    "env",   "Show env datas" },
    { &cmd_fbench, "bench", "Flash bench"},
    { &cmd_reset,  "reset", "Reset system"},
};
void cmd_rafael_init()
{
    Engine::Root().RegisterCommands(cmds_rafael, ArraySize(cmds_rafael));
}
