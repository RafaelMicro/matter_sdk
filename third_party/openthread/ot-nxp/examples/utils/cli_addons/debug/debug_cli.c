/*
 *  Copyright (c) 2023, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */
#include "fsl_common.h"
#include "ot_platform_common.h"
#include <openthread/cli.h>
#include "common/logging.hpp"

/* -------------------------------------------------------------------------- */
/*                             Private definitions                            */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static otError ProcessSpiCmd(void *aContext, uint8_t aArgsLength, char *aArgs[]);

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static const otCliCommand debugCommands[] = {
    {"spi", ProcessSpiCmd}, //
};

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

otError ProcessDebug(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    otError error  = OT_ERROR_NONE;
    int     n_cmds = (sizeof(debugCommands) / sizeof(debugCommands[0]));
    int     i      = 0;
    do
    {
        if (aArgsLength == 0)
        {
            error = OT_ERROR_INVALID_ARGS;
            break;
        }

        for (i = 0; i < n_cmds; i++)
        {
            if (!strcmp(debugCommands[i].mName, aArgs[0]))
            {
                error = debugCommands[i].mCommand(aContext, aArgsLength - 1, &aArgs[1]);
                break;
            }
        }
        if (i == n_cmds)
        {
            error = OT_ERROR_INVALID_ARGS;
        }
        break;
    } while (false);

    return error;
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static otError ProcessSpiCmd(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    OT_UNUSED_VARIABLE(aContext);
    OT_UNUSED_VARIABLE(aArgsLength);
    OT_UNUSED_VARIABLE(aArgs);
    otError error = OT_ERROR_NONE;

#if defined(OT_PLAT_SPINEL_OVER_SPI)
    otLogInfoPlat("ProcessSpiCmd");
    otPlatRadioSpiDiag();
#else
    error = OT_ERROR_INVALID_COMMAND;
#endif

    return error;
}
