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


#ifdef __cplusplus
extern "C" {
#endif

extern int tst_ecdh(void);
extern int tst_sha256(void);
extern int tst_ecdsa(void);

#ifdef __cplusplus
}
#endif



using namespace chip;
using namespace chip::Shell;
using namespace chip::Logging;

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

CHIP_ERROR cmd_test_ecdh(int argc, char ** argv)
{
    tst_ecdh();

    return CHIP_NO_ERROR;
}

CHIP_ERROR cmd_test_sha256(int argc, char ** argv)
{
    tst_sha256();

    return CHIP_NO_ERROR;
}

CHIP_ERROR cmd_test_ecdsa(int argc, char ** argv)
{
    tst_ecdsa();

    return CHIP_NO_ERROR;
}

static shell_command_t cmds_rafael[] = {
    { &cmd_rd, "rd", "Read memory" },
    { &cmd_test_ecdh, "tecdh", "Test ECDH"},
    { &cmd_test_sha256, "tsha256", "Test SHA256"},
    { &cmd_test_ecdsa, "tecdsa", "Test ECDSA"},
};

void cmd_rafael_init()
{
    Engine::Root().RegisterCommands(cmds_rafael, ArraySize(cmds_rafael));
}
