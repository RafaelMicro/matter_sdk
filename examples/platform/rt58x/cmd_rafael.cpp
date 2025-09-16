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
#include "log.h"

#include "EnhancedFlashDataset.h"

#include "FreeRTOS.h"
#include "task.h"
#include "matter_shell.h"






using namespace chip;
using namespace chip::Shell;
using namespace chip::Logging;

CHIP_ERROR cmd_reset(int argc, char ** argv)
{
   sys_software_reset();

    return CHIP_NO_ERROR;
}

CHIP_ERROR cmd_env(int argc, char ** argv)
{
    info_env();

    return CHIP_NO_ERROR;
}

CHIP_ERROR cmd_heap(int argc, char ** argv)
{
    ChipLogProgress(NotSpecified, "Free heap size 0x%x", xPortGetFreeHeapSize());
    ChipLogProgress(NotSpecified, "Minimum heapsize remaining 0x%x", xPortGetMinimumEverFreeHeapSize());
    return CHIP_NO_ERROR;
}

CHIP_ERROR cmd_rm(int argc, char ** argv)
{
    do
    {
        uint32_t    addr = 0, cnt = 0;
        uint32_t    *pCur = 0;

        if ((argc != 1) && (argc != 2))
        {
            log_error("wrong parameters\n");
            break;
        }

        addr = (*(argv[0] + 1) == 'x')
               ? utility_strtox(argv[0] + 2, 0, 8)
               : utility_strtol(argv[0], 0);

        if (argc == 1)
        {
            cnt = 64;
        }
        else
        {
            cnt  = (*(argv[1] + 1) == 'x')
                   ? utility_strtox(argv[1] + 2, 0, 2)
                   : utility_strtol(argv[1], 0);
        }

        pCur = (uint32_t *)addr;
        //cnt  = (cnt + 0x3) >> 2;

        log_hexdump_out("", 16, (uint8_t *)pCur, cnt);
    } while (0);

    return CHIP_NO_ERROR;
}

static shell_command_t cmds_rafael[] = {
    { &cmd_rm,     "rm",    "Read memory" },
    { &cmd_heap,    "heap",   "Show heap remaining" },
    { &cmd_env,    "env",   "Show env datas" },
    { &cmd_reset,  "reset", "Reset system"},
};
void cmd_rafael_init()
{
    Engine::Root().RegisterCommands(cmds_rafael, MATTER_ARRAY_SIZE(cmds_rafael));
}
