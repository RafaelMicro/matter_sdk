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
#include <FreeRTOS.h>

#include "semphr.h"
#include "wlan.h"
#include "wpl.h"
#include <openthread/cli.h>

/* -------------------------------------------------------------------------- */
/*                             Private definitions                            */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static otError ProcessConnectCmd(void *aContext, uint8_t aArgsLength, char *aArgs[]);
static otError ProcessDisconnectCmd(void *aContext, uint8_t aArgsLength, char *aArgs[]);
static otError ProcessScanCmd(void *aContext, uint8_t aArgsLength, char *aArgs[]);
static otError ProcessStateCmd(void *aContext, uint8_t aArgsLength, char *aArgs[]);
static otError ProcessHelpCmd(void *aContext, uint8_t aArgsLength, char *aArgs[]);

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static const otCliCommand commands[] = {
    {"connect", ProcessConnectCmd}, {"disconnect", ProcessDisconnectCmd},
    {"scan", ProcessScanCmd},       {"state", ProcessStateCmd},
    {"help", ProcessHelpCmd},
};

static char *wifi_label = "my-net";

static SemaphoreHandle_t scanLock = NULL;

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

otError ProcessWifi(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    otError error  = OT_ERROR_NONE;
    int     n_cmds = (sizeof(commands) / sizeof(commands[0]));
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
            if (!strcmp(commands[i].mName, aArgs[0]))
            {
                error = commands[i].mCommand(aContext, aArgsLength - 1, &aArgs[1]);
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

static otError ProcessConnectCmd(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    OT_UNUSED_VARIABLE(aContext);
    char *pass = (aArgsLength >= 2) ? aArgs[1] : "";

    if (aArgsLength >= 1)
    {
        wpl_ret_t ret;
        char     *ssid = aArgs[0];

        /* Make sure we are not connected: */
        ProcessDisconnectCmd(aContext, aArgsLength, aArgs);

        ret = WPL_AddNetwork(ssid, pass, wifi_label);
        switch (ret)
        {
        case WPLRET_SUCCESS:
            break;
        case WPLRET_NOT_READY:
            return OT_ERROR_BUSY;
        case WPLRET_BAD_PARAM:
            otCliOutputFormat(" Wrong length of SSID or password\r\n");
            return OT_ERROR_INVALID_ARGS;
        default:
            return OT_ERROR_FAILED;
        }

        ret = WPL_Join(wifi_label);
        switch (ret)
        {
        case WPLRET_SUCCESS:
            return OT_ERROR_NONE;
        case WPLRET_NOT_READY:
            return OT_ERROR_BUSY;
        case WPLRET_FAIL:
            return OT_ERROR_FAILED;
        case WPLRET_TIMEOUT:
            return OT_ERROR_RESPONSE_TIMEOUT;
        case WPLRET_AUTH_FAILED:
            otCliOutputFormat("Auth failed.\r\n");
            return OT_ERROR_SECURITY;
        case WPLRET_NOT_FOUND:
            otCliOutputFormat("Network not found.\r\n");
            return OT_ERROR_NOT_FOUND;
        case WPLRET_ADDR_FAILED:
            otCliOutputFormat(" Failed to get IPv4 from DHCP.\r\n");
            return OT_ERROR_FAILED;
        default:
            return OT_ERROR_FAILED;
        }
    }

    return OT_ERROR_INVALID_ARGS;
}

static otError ProcessDisconnectCmd(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    OT_UNUSED_VARIABLE(aContext);
    OT_UNUSED_VARIABLE(aArgsLength);
    OT_UNUSED_VARIABLE(aArgs);

    (void)WPL_Leave();
    (void)WPL_RemoveNetwork(wifi_label);

    return OT_ERROR_NONE;
}

static otError ProcessHelpCmd(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    OT_UNUSED_VARIABLE(aContext);
    OT_UNUSED_VARIABLE(aArgsLength);
    OT_UNUSED_VARIABLE(aArgs);

    otCliOutputFormat("wifi connect <ssid> [password]\r\n");
    otCliOutputFormat("wifi scan\r\n");
    otCliOutputFormat("wifi state\r\n");
    otCliOutputFormat("wifi disconnect\r\n");

    return OT_ERROR_NONE;
}

void PrintScan()
{
    unsigned int            i;
    struct wlan_scan_result res;
    for (i = 0; wlan_get_scan_result(i, &res) == WM_SUCCESS; i++)
    {
        otCliOutputFormat("%x:%x:%x:%x:%x:%x -%u dBm %s\r\n", res.bssid[0], res.bssid[1], res.bssid[2], res.bssid[3],
                          res.bssid[4], res.bssid[5], res.rssi, res.ssid);
    }
}

static int ScanCB(unsigned int count)
{
    OT_UNUSED_VARIABLE(count);

    PrintScan();
    xSemaphoreGive(scanLock);
    return WM_SUCCESS;
}

static otError ProcessScanCmd(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    OT_UNUSED_VARIABLE(aContext);
    OT_UNUSED_VARIABLE(aArgsLength);
    OT_UNUSED_VARIABLE(aArgs);

    int ret;

    scanLock = xSemaphoreCreateBinary();
    if (scanLock == NULL)
    {
        return OT_ERROR_NOT_CAPABLE;
    }

    ret = wlan_scan(&ScanCB);
    if (ret == WM_SUCCESS)
    {
        xSemaphoreTake(scanLock, portMAX_DELAY);
    }

    vSemaphoreDelete(scanLock);

    return OT_ERROR_NONE;
}

static otError ProcessStateCmd(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    OT_UNUSED_VARIABLE(aContext);
    OT_UNUSED_VARIABLE(aArgsLength);
    OT_UNUSED_VARIABLE(aArgs);

    enum wlan_connection_state state;
    const int                  ret = wlan_get_connection_state(&state);

    if (ret != WM_SUCCESS)
    {
        return OT_ERROR_INVALID_STATE;
    }

    switch (state)
    {
    case WLAN_DISCONNECTED:
        otCliOutputFormat("disconnected\r\n");
        break;
    case WLAN_CONNECTING:
        otCliOutputFormat("connecting\r\n");
        break;
    case WLAN_ASSOCIATED:
        otCliOutputFormat("associated\r\n");
        break;
    case WLAN_CONNECTED:
        otCliOutputFormat("connected\r\n");
        break;
    case WLAN_UAP_STARTED:
        otCliOutputFormat("uAP started\r\n");
        break;
    case WLAN_UAP_STOPPED:
        otCliOutputFormat("uAP stopped\r\n");
        break;
    case WLAN_SCANNING:
        otCliOutputFormat("scanning\r\n");
        break;
    case WLAN_ASSOCIATING:
        otCliOutputFormat("associating\r\n");
        break;
    default:
        otCliOutputFormat("unknown state %u\r\n", state);
    }

    return OT_ERROR_NONE;
}
