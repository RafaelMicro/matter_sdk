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

#include <common/code_utils.hpp>
#include <openthread/cli.h>
#include <openthread/instance.h>

#ifdef OT_APP_CLI_IPERF_ADDON
#include "iperf_cli.h"
#include "ot_lwip.h"
#endif

#ifdef OT_APP_CLI_DEBUG_ADDON
#include "debug_cli.h"
#endif

#ifdef OT_APP_CLI_LWIP_ADDON
#include "lwip_cli.h"
#endif

#ifdef OT_APP_CLI_WIFI_ADDON
#include "wifi_cli.h"
#endif

#ifdef OT_APP_CLI_PLATFORM_ADDON
#include "radio_cli.h"
#endif

#ifdef OT_APP_CLI_LOWPOWER_ADDON
#include "lowpower_cli.h"
#endif

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static const otCliCommand addonsCommands[] = {
#ifdef OT_APP_CLI_IPERF_ADDON
    {"iperf", ProcessIperf},
#endif
#ifdef OT_APP_CLI_PLATFORM_ADDON
    {"radio_nxp", ProcessRadio},
#endif
#ifdef OT_APP_CLI_DEBUG_ADDON
    {"debug", ProcessDebug},
#endif
#ifdef OT_APP_CLI_LOWPOWER_ADDON
    {"lp", ProcessLowPower},
#endif
#ifdef OT_APP_CLI_LWIP_ADDON
    {"lwip", ProcessLwip},
#endif
#ifdef OT_APP_CLI_WIFI_ADDON
    {"wifi", ProcessWifi},
#endif
};

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static void otAppCliStateChangeCallback(otChangedFlags flags, void *context);

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void otAppCliAddonsInit(otInstance *aInstance)
{
#ifdef OT_APP_CLI_IPERF_ADDON
    otAppCliIperfCliInit(aInstance);
#endif

#ifdef OT_APP_CLI_LOWPOWER_ADDON
    otAppLowPowerCliInit(aInstance);
#endif

    otSetStateChangedCallback(aInstance, otAppCliStateChangeCallback, NULL);

    otCliSetUserCommands(addonsCommands, OT_ARRAY_LENGTH(addonsCommands), NULL);
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static void otAppCliStateChangeCallback(otChangedFlags flags, void *context)
{
#ifdef OT_APP_CLI_IPERF_ADDON
    otPlatLwipUpdateState(flags, context);
#endif
}
