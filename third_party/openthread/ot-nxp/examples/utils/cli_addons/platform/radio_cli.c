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

#include <openthread-core-config.h>
#include <openthread/cli.h>
#include <openthread/error.h>
#include <openthread/instance.h>
#include "common/logging.hpp"

#include <openthread/crypto.h>

#include "ot_platform_common.h"
#include "radio_cli.h"
#include <openthread/platform/radio.h>
#include "lib/spinel/spinel.h"
/* -------------------------------------------------------------------------- */
/*                             Private definitions                            */
/* -------------------------------------------------------------------------- */
#define MFG_CMD_ACTION_GET 0
#define MFG_CMD_ACTION_SET 1

#define MFG_CMD_GET_SET_CHANNEL 0x0b       // 11
#define MFG_CMD_GET_SET_TXPOWER 0x0f       // 15
#define MFG_CMD_CONTINUOUS_TX 0x11         // 17
#define MFG_CMD_GET_SET_PAYLOAD_SIZE 0x14  // 20
#define MFG_CMD_GET_RX_RESULT 0x1f         // 31
#define MFG_CMD_START_RX_TEST 0x20         // 32
#define MFG_CMD_BURST_TX 0x21              // 33
#define MFG_CMD_DUTY_CYCLE_TX 0x23         // 35
#define MFG_CMD_GET_SET_CCA_THRESHOLD 0x2F // 47
#define MFG_CMD_CONTINOUS_CCA_TEST 0X31    // 49
#define MFG_CMD_GET_CCA_STATUS 0x32        // 50
#define MFG_CMD_CONTINOUS_ED_TEST 0x37     // 55
#define MFG_CMD_GET_ED_VALUE 0x38          // 56
#define MFG_CMD_PHY_TX_TEST_PSDU 0x39      // 57
#define MFG_CMD_PHY_RX_TX_ACK_TEST 0x3A    // 58
#define MFG_CMD_SET_GENERIC_PARAM 0x3B     // 59
#define MFG_CMD_GET_SET_LATENCY 0x3C       // 60
#define MFG_CMD_BRIC_ENCRYPT 0x46          // 70
#define MFG_CMD_BRIC_DECRYPT 0x47          // 71
#define MFG_CMD_GENERIC 0xFF               // 255
#define MAX_VERSION_STRING_SIZE 128        //< Max size of version string.

// NXP Spinel commands
// Independent Reset Properties range [0x100 - 0X110]

// IR CONFIG <uint8 mode> : Configure IR Mode (O:Disable 1:OutOfBand 2:InBand)
#define SPINEL_PROP_VENDOR_NXP_IR_CONFIG (SPINEL_PROP_VENDOR__BEGIN + 0x100)

// IR CMD : Execute Independent Reset without noticed (no arg)
#define SPINEL_PROP_VENDOR_NXP_IR_CMD (SPINEL_PROP_VENDOR__BEGIN + 0x101)

// SET IEEE.802.15.4 MAC Address <uint64> : Set MAC address
#define SPINEL_PROP_VENDOR_NXP_SET_EUI64_CMD (SPINEL_PROP_VENDOR__BEGIN + 0x10A)

// SET / GET TX Power Limit of 15.4 Transmissions
#define SPINEL_PROP_VENDOR_NXP_GET_SET_TXPOWERLIMIT_CMD (SPINEL_PROP_VENDOR__BEGIN + 0x10B)

// SET IEEE.802.15.4 CCA configuration
#define SPINEL_PROP_VENDOR_NXP_GET_SET_CCA_CONFIGURE_CMD (SPINEL_PROP_VENDOR__BEGIN + 0x10C)

// GET transceiver FW VERSION
#define SPINEL_PROP_VENDOR_NXP_GET_FW_VERSION_CMD (SPINEL_PROP_VENDOR__BEGIN + 0x10D)

// Manufacturing Properties range [0x3F0 - 0x3FF]
#define SPINEL_CMD_VENDOR_NXP_MFG (SPINEL_CMD_VENDOR__BEGIN + 0x3FF)

#define MFG_CMD_ACTION_USE_BRIC_CRYPTO MFG_CMD_ACTION_GET
#define MFG_CMD_ACTION_USE_OT_SW_CRYPTO MFG_CMD_ACTION_SET

#define MFG_AES128_CCM_Encrypt MFG_CMD_ACTION_USE_BRIC_CRYPTO
//#define MFG_AES128_CCM_Encrypt MFG_CMD_ACTION_USE_OT_SW_CRYPTO

//#define MFG_AES128_CCM_Decrypt MFG_CMD_ACTION_USE_BRIC_CRYPTO
#define MFG_AES128_CCM_Decrypt MFG_CMD_ACTION_USE_OT_SW_CRYPTO

#define PAYLOAD_LENGTH_STATUS 16
#define PAYLOAD_LENGTH_TAG 16

#define SETUP_AUTHTAGLEN 10

// MACRO: ASCII-Hexadecimal encoded string to Hexadecimal converter
#define M_AHTOH(S, L, P)                                                                                       \
    for (uint8_t i = 0; i < (L / 2); i++, P++)                                                                 \
    {                                                                                                          \
        *P = (S[2 * i] <= '9')                                                                                 \
                 ? ((S[2 * i] - '0') * 16)                                                                     \
                 : ((S[2 * i] <= 'F') ? (((S[2 * i] - 'A') + 10) << 4) : (((S[2 * i] - 'a') + 10) << 4));      \
        *P |= (S[(2 * i) + 1] <= '9')                                                                          \
                  ? (S[(2 * i) + 1] - '0')                                                                     \
                  : ((S[(2 * i) + 1] <= 'F') ? ((S[(2 * i) + 1] - 'A') + 10) : ((S[(2 * i) + 1] - 'a') + 10)); \
    }

// MACRO: ASCII string to Hexadecimal converter
#define M_ATOH(S, L, P)                                                                                  \
    for (uint8_t i = 0; i < L; i++, P++)                                                                 \
    {                                                                                                    \
        *P = (S[i] <= '9') ? (S[i] - '0') : ((S[i] <= 'F') ? ((S[i] - 'A') + 10) : ((S[i] - 'a') + 10)); \
    }

// MACRO: Hexadecimal to ASCII string converter
#define M_HTOA(S, L, P)                                                                           \
    {                                                                                             \
        uint8_t  myi;                                                                             \
        uint8_t *myp;                                                                             \
        otCliOutputFormat(S);                                                                     \
        for (myi = 0, myp = (uint8_t *)P; myi < L; myi++, myp++) otCliOutputFormat("%02x", *myp); \
        otCliOutputFormat("\r\n");                                                                \
    }

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static otError ProcessIRCmd(void *aContext, uint8_t aArgsLength, char *aArgs[]);
static otError ProcessSetEui64(void *aContext, uint8_t aArgsLength, char *aArgs[]);
static otError ProcessTxPowerLimit(void *aContext, uint8_t aArgsLength, char *aArgs[]);
static otError ProcessMfgGetInt8(void *aContext, uint8_t cmdId, uint8_t aArgsLength);
static otError ProcessMfgSetInt8(void   *aContext,
                                 uint8_t cmdId,
                                 uint8_t aArgsLength,
                                 char   *aArgs[],
                                 int8_t  min,
                                 int8_t  max);
static otError ProcessMfgCommands(void *aContext, uint8_t aArgsLength, char *aArgs[]);
static otError ProcessGetSetCcaCfg(void *aContext, uint8_t aArgsLength, char *aArgs[]);
static otError ProcessGetFwVersion(void *aContext, uint8_t aArgsLength, char *aArgs[]);
static otError ProcessAES128CCMEncrypt(void *aContext, uint8_t aArgsLength, char *aArgs[]);
static otError ProcessAES128CCMDecrypt(void *aContext, uint8_t aArgsLength, char *aArgs[]);
static otError mfgGenericCommand(void *aContext, uint8_t aArgsLength, char *aArgs[]);

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */
static uint8_t mfgEnable = 0;

static const otCliCommand radioCommands[] = {
    {"ircmd", ProcessIRCmd},              //=> InBand Independent Reset command
    {"seteui64", ProcessSetEui64},        //=> Set ieee.802.15.4 MAC Address
    {"txpwrlimit", ProcessTxPowerLimit},  //=> Set/Get TX power limit for 15.4
    {"mfgcmd", ProcessMfgCommands},       //=> Generic VSC for MFG RF commands
    {"ccacfg", ProcessGetSetCcaCfg},      //=> Set/Get CCA configuration for 802.15.4 CCA Before Tx operation
    {"fwversion", ProcessGetFwVersion},   //=> Get firmware version for 15.4
    {"encrypt", ProcessAES128CCMEncrypt}, //=> self-test AES128_CCM Encrypt
    {"decrypt", ProcessAES128CCMDecrypt}, //=> self-test AES128_CCM Decrypt
};

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

otError ProcessRadio(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    otError error  = OT_ERROR_NONE;
    int     n_cmds = (sizeof(radioCommands) / sizeof(radioCommands[0]));
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
            if (!strcmp(radioCommands[i].mName, aArgs[0]))
            {
                error = radioCommands[i].mCommand(aContext, aArgsLength - 1, &aArgs[1]);
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

static otError ProcessIRCmd(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    OT_UNUSED_VARIABLE(aContext);
    otError error = OT_ERROR_INVALID_ARGS;

    OT_UNUSED_VARIABLE(aArgsLength);
    OT_UNUSED_VARIABLE(aArgs);
    otLogInfoPlat("ProcessIRCmd");
    error = otPlatResetOt();

    return error;
}

static otError ProcessSetEui64(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    OT_UNUSED_VARIABLE(aContext);
    otError error = OT_ERROR_INVALID_ARGS;

    otLogInfoPlat("ProcessSetEui64");

    if (aArgsLength == 1)
    {
        otExtAddress addr;
        char        *hex = *aArgs;

        otLogInfoPlat("+ seteui64 %s (len %ld)", *aArgs, (uint32_t)strlen(*aArgs));

        if ((hex[1] == 'x') && (strlen(*aArgs) == 18))
        {
            hex   = hex + 2;
            error = OT_ERROR_NONE;

            for (uint32_t i = 0; (i < 8) && (error == OT_ERROR_NONE); i++)
            {
                addr.m8[i] = 0;
                for (uint32_t k = 0; k < 2; k++)
                {
                    // get current character then increment
                    uint8_t byte = *hex++;
                    // transform hex character to the 4bit equivalent number, using the ascii table indexes
                    if (byte >= '0' && byte <= '9')
                        byte = byte - '0';
                    else if (byte >= 'a' && byte <= 'f')
                        byte = byte - 'a' + 10;
                    else if (byte >= 'A' && byte <= 'F')
                        byte = byte - 'A' + 10;
                    else
                    {
                        error = OT_ERROR_INVALID_ARGS;
                        break;
                    }
                    // shift 4 to make space for new digit, and add the 4 bits of the new digit
                    addr.m8[i] = (addr.m8[i] << 4) | (byte & 0xF);
                }
            }

            if (error == OT_ERROR_NONE)
            {
                uint64_t addr64t = 0;

                for (size_t i = 0; i < sizeof(addr64t); i++)
                {
                    addr64t |= ((uint64_t)addr.m8[sizeof(addr64t) - 1 - i]) << (i * 8);
                }

                error = otPlatRadioSendSetPropVendorUint64Cmd(SPINEL_PROP_VENDOR_NXP_SET_EUI64_CMD, addr64t);
            }
        }
    }

    return error;
}

static otError ProcessTxPowerLimit(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    OT_UNUSED_VARIABLE(aContext);
    otError error        = OT_ERROR_INVALID_ARGS;
    uint8_t txPowerLimit = 0;

    otLogInfoPlat("TxPowerLimit");

    if (aArgsLength == 1) // set tx power limit
    {
        txPowerLimit = (uint8_t)atoi(aArgs[0]);

        if ((txPowerLimit >= 1) && (txPowerLimit <= OT_NXP_PLAT_TX_PWR_LIMIT_MAX))
        {
            otLogInfoPlat("Set TX power limit: %d", txPowerLimit);
            error = otPlatRadioSendSetPropVendorUint8Cmd(SPINEL_PROP_VENDOR_NXP_GET_SET_TXPOWERLIMIT_CMD, txPowerLimit);
        }
        else
        {
            otLogInfoPlat("The TX power limit set is out of range");
        }
    }
    else if (aArgsLength == 0) // get tx power limit
    {
        error = otPlatRadioSendGetPropVendorUint8Cmd(SPINEL_PROP_VENDOR_NXP_GET_SET_TXPOWERLIMIT_CMD, &txPowerLimit);
        otLogInfoPlat("Get TX power limit: %d", txPowerLimit);

        otCliOutputFormat("%d\r\n", txPowerLimit);
    }

    return error;
}

static otError ProcessMfgGetInt8(void *aContext, uint8_t cmdId, uint8_t aArgsLength)
{
    otError error       = OT_ERROR_INVALID_ARGS;
    uint8_t outputLen   = 12;
    uint8_t payload[12] = {11};
    uint8_t payloadLen  = 12;

    if (aArgsLength == 1)
    {
        payload[1] = cmdId;
        payload[2] = MFG_CMD_ACTION_GET;

        otPlatRadioMfgCommand(aContext, SPINEL_CMD_VENDOR_NXP_MFG, (uint8_t *)payload, payloadLen, &outputLen);

        if ((outputLen >= 5) && (payload[3] == 0))
        {
            if ((cmdId == MFG_CMD_GET_SET_TXPOWER) && OT_NXP_PLAT_TX_PWR_HALF_DBM)
            {
                otCliOutputFormat("%d\r\n", ((int8_t)payload[4]) / 2);
            }
            else
            {
                otCliOutputFormat("%d\r\n", (int8_t)payload[4]);
            }
            error = OT_ERROR_NONE;
        }
        else
        {
            error = OT_ERROR_FAILED;
        }
    }

    return error;
}

static otError ProcessMfgSetInt8(void   *aContext,
                                 uint8_t cmdId,
                                 uint8_t aArgsLength,
                                 char   *aArgs[],
                                 int8_t  min,
                                 int8_t  max)
{
    otError error       = OT_ERROR_INVALID_ARGS;
    uint8_t outputLen   = 12;
    uint8_t payload[12] = {11};
    uint8_t payloadLen  = 12;
    int8_t  setValue    = 0;

    if (aArgsLength == 2)
    {
        setValue = (int8_t)atoi(aArgs[1]);
        if ((setValue >= min) && (setValue <= max))
        {
            payload[1] = cmdId;
            payload[2] = MFG_CMD_ACTION_SET;
            payload[4] = (uint8_t)setValue;

            if ((cmdId == MFG_CMD_GET_SET_TXPOWER) && OT_NXP_PLAT_TX_PWR_HALF_DBM)
            {
                payload[4] = ((uint8_t)setValue) << 1; // convert dBm to half dBm
            }
            else
            {
                payload[4] = (uint8_t)setValue;
            }

            otPlatRadioMfgCommand(aContext, SPINEL_CMD_VENDOR_NXP_MFG, (uint8_t *)payload, payloadLen, &outputLen);

            if ((outputLen >= 4) && (payload[3] == 0))
            {
                error = OT_ERROR_NONE;
            }
            else
            {
                error = OT_ERROR_FAILED;
            }
        }
    }

    return error;
}

static otError ProcessMfgCommands(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    uint8_t payload[12] = {11};
    uint8_t payloadLen  = 12;
    uint8_t outputLen   = 12;
    otError error       = OT_ERROR_INVALID_ARGS;
    uint8_t cmdId, idx;

    do
    {
        if (aArgsLength == 1)
        {
            cmdId = (uint8_t)atoi(aArgs[0]);
            if ((cmdId == 0) || (cmdId == 1))
            {
                mfgEnable = cmdId;
                otLogInfoPlat("MFG command SUCCESS");
                error = OT_ERROR_NONE;
                break;
            }
        }

        if (mfgEnable == 0)
        {
            otLogInfoPlat("MFG command not enabled");
            otCliOutputFormat("MFG command not enabled. to enable it : radio_nxp mfgcmd 1\r\n");
            error = OT_ERROR_FAILED;
            break;
        }

        if ((aArgsLength > 0) && (mfgEnable == 1))
        {
            cmdId = (uint8_t)atoi(aArgs[0]);

            switch (cmdId)
            {
            case MFG_CMD_GET_SET_CHANNEL: // get channel
                error = ProcessMfgGetInt8(aContext, MFG_CMD_GET_SET_CHANNEL, aArgsLength);
                break;

            case MFG_CMD_GET_SET_CHANNEL + 1: // set channel
                error = ProcessMfgSetInt8(aContext, MFG_CMD_GET_SET_CHANNEL, aArgsLength, aArgs, 11, 26);
                break;

            case MFG_CMD_GET_SET_TXPOWER: // get txpower
                error = ProcessMfgGetInt8(aContext, MFG_CMD_GET_SET_TXPOWER, aArgsLength);
                break;

            case MFG_CMD_GET_SET_TXPOWER + 1: // set txpower
                error = ProcessMfgSetInt8(aContext, MFG_CMD_GET_SET_TXPOWER, aArgsLength, aArgs, -20,
                                          OT_NXP_PLAT_TX_PWR_LIMIT_MAX / 2);
                break;

            case MFG_CMD_CONTINUOUS_TX:
                error = ProcessMfgSetInt8(aContext, MFG_CMD_CONTINUOUS_TX, aArgsLength, aArgs, 0, 1);
                break;

            case MFG_CMD_GET_SET_PAYLOAD_SIZE: // get
                error = ProcessMfgGetInt8(aContext, MFG_CMD_GET_SET_PAYLOAD_SIZE, aArgsLength);
                break;

            case MFG_CMD_GET_SET_PAYLOAD_SIZE + 1: // set
                error = ProcessMfgSetInt8(aContext, MFG_CMD_GET_SET_PAYLOAD_SIZE, aArgsLength, aArgs, 17, 116);
                break;

            case MFG_CMD_GET_RX_RESULT:
            {
                if (aArgsLength == 1)
                {
                    payload[1] = MFG_CMD_GET_RX_RESULT;
                    payload[2] = MFG_CMD_ACTION_GET;
                    otPlatRadioMfgCommand(aContext, SPINEL_CMD_VENDOR_NXP_MFG, (uint8_t *)payload, payloadLen,
                                          &outputLen);
                    if (outputLen >= 11)
                    {
                        otCliOutputFormat("status : %d\r\n", payload[4]);
                        otCliOutputFormat("rx_pkt_count : %d\r\n", payload[5] | (payload[6] << 8));
                        otCliOutputFormat("total_pkt_count : %d\r\n", payload[7] | (payload[8] << 8));
                        otCliOutputFormat("rssi : %d\r\n", (int8_t)payload[9]);
                        otCliOutputFormat("lqi : %d\r\n", payload[10]);
                        error = OT_ERROR_NONE;
                    }
                    else
                    {
                        error = OT_ERROR_FAILED;
                    }
                }
            }
            break;

            case MFG_CMD_START_RX_TEST:
            {
                if (aArgsLength == 1)
                {
                    payload[1] = MFG_CMD_START_RX_TEST;
                    otPlatRadioMfgCommand(aContext, SPINEL_CMD_VENDOR_NXP_MFG, (uint8_t *)payload, payloadLen,
                                          &outputLen);
                    error = OT_ERROR_NONE;
                }
            }
            break;

            case MFG_CMD_BURST_TX:
            {
                uint8_t mode = 0, gap = 0;
                if (aArgsLength == 3)
                {
                    mode = (uint8_t)atoi(aArgs[1]);
                    gap  = (uint8_t)atoi(aArgs[2]);
                    if ((mode < 8) && (gap > 5))
                    {
                        payload[1] = MFG_CMD_BURST_TX;
                        payload[4] = mode;
                        payload[5] = (uint8_t)atoi(aArgs[2]);
                        otPlatRadioMfgCommand(aContext, SPINEL_CMD_VENDOR_NXP_MFG, (uint8_t *)payload, payloadLen,
                                              &outputLen);
                        error = OT_ERROR_NONE;
                    }
                }
            }
            break;

            case MFG_CMD_DUTY_CYCLE_TX:
                error = ProcessMfgSetInt8(aContext, MFG_CMD_DUTY_CYCLE_TX, aArgsLength, aArgs, 0, 1);
                break;

            case MFG_CMD_GET_SET_CCA_THRESHOLD: // get
                error = ProcessMfgGetInt8(aContext, MFG_CMD_GET_SET_CCA_THRESHOLD, aArgsLength);
                break;

            case MFG_CMD_GET_SET_CCA_THRESHOLD + 1: // set
                error = ProcessMfgSetInt8(aContext, MFG_CMD_GET_SET_CCA_THRESHOLD, aArgsLength, aArgs, -110, 0);
                break;

            case MFG_CMD_CONTINOUS_CCA_TEST:
            {
                if (aArgsLength == 3)
                {
                    payload[1] = MFG_CMD_CONTINOUS_CCA_TEST;
                    payload[2] = MFG_CMD_ACTION_SET;
                    payload[4] = (uint8_t)atoi(aArgs[1]);
                    payload[5] = (uint8_t)atoi(aArgs[2]);
                    if ((payload[4] < 2) && (payload[5] < 4))
                    {
                        otPlatRadioMfgCommand(aContext, SPINEL_CMD_VENDOR_NXP_MFG, (uint8_t *)payload, payloadLen,
                                              &outputLen);
                        if ((outputLen >= 4) && (payload[3] == 0))
                        {
                            error = OT_ERROR_NONE;
                        }
                        else
                        {
                            error = OT_ERROR_FAILED;
                        }
                    }
                }
            }
            break;

            case MFG_CMD_GET_CCA_STATUS: // get
                error = ProcessMfgGetInt8(aContext, MFG_CMD_GET_CCA_STATUS, aArgsLength);
                break;

            case MFG_CMD_CONTINOUS_ED_TEST:
                error = ProcessMfgSetInt8(aContext, MFG_CMD_CONTINOUS_ED_TEST, aArgsLength, aArgs, 0, 1);
                break;

            case MFG_CMD_GET_ED_VALUE:
                error = ProcessMfgGetInt8(aContext, MFG_CMD_GET_ED_VALUE, aArgsLength);
                break;

            case MFG_CMD_PHY_TX_TEST_PSDU:
            {
                uint8_t count_opt, gap, ackEnable;
                if (aArgsLength == 4)
                {
                    payload[1] = MFG_CMD_PHY_TX_TEST_PSDU;
                    payload[2] = MFG_CMD_ACTION_SET;

                    count_opt = (uint8_t)atoi(aArgs[1]);
                    gap       = (uint8_t)atoi(aArgs[2]);
                    ackEnable = (uint8_t)atoi(aArgs[3]);
                    if ((count_opt < 8) && (gap > 5) && (ackEnable < 2))
                    {
                        payload[4] = count_opt;
                        payload[5] = gap;
                        payload[6] = ackEnable;
                        otPlatRadioMfgCommand(aContext, SPINEL_CMD_VENDOR_NXP_MFG, (uint8_t *)payload, payloadLen,
                                              &outputLen);
                        if ((outputLen >= 5) && (payload[3] == 0))
                        {
                            error = OT_ERROR_NONE;
                        }
                    }
                }
            }
            break;

            case MFG_CMD_PHY_RX_TX_ACK_TEST:
                error = ProcessMfgSetInt8(aContext, MFG_CMD_PHY_RX_TX_ACK_TEST, aArgsLength, aArgs, 0, 1);
                break;

            case MFG_CMD_SET_GENERIC_PARAM:
            {
                uint16_t panid, destaddr, srcaddr;
                if (aArgsLength == 5)
                {
                    panid    = (uint16_t)strtol(aArgs[2], NULL, 16);
                    destaddr = (uint16_t)strtol(aArgs[3], NULL, 16);
                    srcaddr  = (uint16_t)strtol(aArgs[4], NULL, 16);

                    payload[1]  = MFG_CMD_SET_GENERIC_PARAM;
                    payload[2]  = MFG_CMD_ACTION_SET;
                    payload[4]  = (uint8_t)atoi(aArgs[1]);           // SEQ_NUM
                    payload[5]  = (uint8_t)(panid & 0xFF);           // PAN ID LSB
                    payload[6]  = (uint8_t)((panid >> 8) & 0xFF);    // PAN ID MSB
                    payload[7]  = (uint8_t)(destaddr & 0xFF);        // DEST ADDR LSB
                    payload[8]  = (uint8_t)((destaddr >> 8) & 0xFF); // DEST ADDR MSB
                    payload[9]  = (uint8_t)(srcaddr & 0xFF);         // SRC ADDR LSB
                    payload[10] = (uint8_t)((srcaddr >> 8) & 0xFF);  // SRC ADDR MSB

                    otPlatRadioMfgCommand(aContext, SPINEL_CMD_VENDOR_NXP_MFG, (uint8_t *)payload, payloadLen,
                                          &outputLen);
                    if ((outputLen >= 5) && (payload[3] == 0))
                    {
                        error = OT_ERROR_NONE;
                    }
                    else
                    {
                        error = OT_ERROR_FAILED;
                    }
                }
            }
            break;

            case MFG_CMD_GET_SET_LATENCY:
            {
                if (aArgsLength == 11)
                {
                    payload[1] = MFG_CMD_GET_SET_LATENCY;
                    payload[2] = (uint8_t)atoi(aArgs[1]); // Get or set
                    payload[4] = (uint8_t)atoi(aArgs[3]); // State
                    payload[5] = (uint8_t)atoi(aArgs[4]); // TX/RX Toggle
                    payload[6] = (uint8_t)atoi(aArgs[5]); // GPIO number
                    payload[7] = (uint8_t)atoi(aArgs[6]); // PANID[15:8]
                    payload[8] = (uint8_t)atoi(aArgs[7]); // PANID[7:0]

                    otPlatRadioMfgCommand(aContext, SPINEL_CMD_VENDOR_NXP_MFG, (uint8_t *)payload, payloadLen,
                                          &outputLen);
                    if ((outputLen >= 9) && (payload[3] == 0))
                    {
                        if (payload[2] == MFG_CMD_ACTION_GET)
                        {
                            otCliOutputFormat("State: %d\r\n", payload[4]);
                            otCliOutputFormat("TX/RX Toggle: %d\r\n", payload[5]);
                            otCliOutputFormat("GPIO number: %d\r\n", payload[6]);
                            otCliOutputFormat("PANID: %d\r\n", (payload[7] << 8) | (payload[8]));
                        }
                        error = OT_ERROR_NONE;
                    }
                    else
                    {
                        error = OT_ERROR_FAILED;
                    }
                }
            }
            case MFG_CMD_GENERIC:
            {
                error = mfgGenericCommand(aContext, aArgsLength, aArgs);
            }
            break;

            default:
                error = OT_ERROR_NOT_IMPLEMENTED;
                break;
            }
        }

        // HANDLE ERRORS
        if (error == OT_ERROR_NONE)
        {
            otLogInfoPlat("MFG command SUCCESS");
        }
        else if (aArgsLength == payloadLen)
        {
            // If user passed all the payload, this means this is a direct message for the RCP.
            // Send it and print the return results.
            for (idx = 0; idx < payloadLen; idx++)
            {
                payload[idx] = (uint8_t)atoi(aArgs[idx]);
            }
            otPlatRadioMfgCommand(aContext, SPINEL_CMD_VENDOR_NXP_MFG, (uint8_t *)payload, payloadLen, &outputLen);
            for (idx = 0; idx < outputLen; idx++)
            {
                otCliOutputFormat("%d ", payload[idx]);
            }
            otCliOutputFormat("\r\n");
            error = OT_ERROR_NONE;
            otLogInfoPlat("MFG command SUCCESS");
        }
        else if (error == OT_ERROR_INVALID_ARGS)
        {
            otLogInfoPlat("MFG command Invalid parameter");
        }
        else if (error == OT_ERROR_NOT_IMPLEMENTED)
        {
            otLogInfoPlat("MFG command not implemented");
        }
        else
        {
            otLogInfoPlat("MFG command FAILED");
        }
    } while (false);

    return error;
}

static otError ProcessGetSetCcaCfg(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    otError         error = OT_ERROR_INVALID_ARGS;
    otCCAModeConfig aCcaCfg;

    if (aArgsLength == 4) // set cca configuration
    {
        aCcaCfg.mCcaMode            = (uint8_t)strtol(aArgs[0], NULL, 16);
        aCcaCfg.mCca1Threshold      = (uint8_t)strtol(aArgs[1], NULL, 16);
        aCcaCfg.mCca2CorrThreshold  = (uint8_t)strtol(aArgs[2], NULL, 16);
        aCcaCfg.mCca2MinNumOfCorrTh = (uint8_t)strtol(aArgs[3], NULL, 16);
        if ((((aCcaCfg.mCcaMode >= 1) && (aCcaCfg.mCcaMode <= 4)) || (aCcaCfg.mCcaMode == 0xFF)) &&
            (aCcaCfg.mCca2MinNumOfCorrTh <= 6))
        {
            error = otPlatRadioCcaConfigValue(SPINEL_PROP_VENDOR_NXP_GET_SET_CCA_CONFIGURE_CMD, &aCcaCfg, 0x1);
        }
    }
    else if (aArgsLength == 0) // get cca configuration
    {
        error = otPlatRadioCcaConfigValue(SPINEL_PROP_VENDOR_NXP_GET_SET_CCA_CONFIGURE_CMD, &aCcaCfg, 0x0);

        otCliOutputFormat("CCA Configuration:\r\n");
        otCliOutputFormat(
            "CCA Mode type [CCA1=1, CCA2=2, CCA3=3[CCA1 AND CCA2], CCA3=4[CCA1 OR CCA2], NoCCA=0xFF], : 0x%x\r\n",
            aCcaCfg.mCcaMode);
        otCliOutputFormat("CCA1 Threshold Value : 0x%x\r\n", aCcaCfg.mCca1Threshold);
        otCliOutputFormat("CCA2 Correlation Threshold Value : 0x%x\r\n", aCcaCfg.mCca2CorrThreshold);
        otCliOutputFormat("CCA2 Minimim Number of Correlation Threshold Value : 0x%x\r\n", aCcaCfg.mCca2MinNumOfCorrTh);
    }
    else
    {
        otCliOutputFormat("CCA configuration FAILED! Invalid input arg\r\n \
                           Format: ccacfg <CcaMode> <Cca1Threshold> \
                           <Cca2CorrThreshold> <Cca2MinNumOfCorrTh>\r\n \
                           CcaMode: CCA Mode type [CCA1=1, CCA2=2, CCA3=3[CCA1 AND CCA2], CCA3=4[CCA1 OR CCA2], NoCCA=0xFF]\r\n \
                           Cca1Threshold[1Byte Hex value]: Energy threshold for CCA Mode1\r\n \
                           Cca2CorrThreshold[1Byte Hex value]: CCA Mode 2 Correlation Threshold\r\n \
                           Cca2MinNumOfCorrTh: [0 to 6]\r\n");
    }

    return error;
}

static otError ProcessGetFwVersion(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    otError error = OT_ERROR_INVALID_ARGS;
    OT_UNUSED_VARIABLE(aContext);
    OT_UNUSED_VARIABLE(aArgs);

    if (aArgsLength == 0)
    {
        const char version[MAX_VERSION_STRING_SIZE] = {0};
        error = otPlatRadioSendGetPropVendorCmd(SPINEL_PROP_VENDOR_NXP_GET_FW_VERSION_CMD, version,
                                                MAX_VERSION_STRING_SIZE);
        if (error == OT_ERROR_NONE)
        {
            otCliOutputFormat("%s\r\n", version);
        }
    }
    return error;
}

static otError ProcessAES128CCMEncrypt(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    /*
    ** AES module within FFU 15.4/Thread IP block:
    ** ------------------------------------------
    **
    ** Redfinch FFU 15.4/Thread IP block embed a Basic Radio Integrated Crypto engine, called BRIC
    ** For our 15.4/Thread operations we use only the encryption function: AES_CCM() for encryption
    **     with a key size of 128 bits​.
    **
    */

    /*
    ** CMD Syntax:          radio_nxp  encrypt  <AES_128_Key> <IV/Nonce> <PT/PlainText>
    ** Return:              [GeneratedTag] [CT (CipherText)]
    */

    // IMU variables
    otError error = OT_ERROR_INVALID_ARGS;

    uint8_t cmdId = MFG_CMD_BRIC_ENCRYPT;

    uint8_t payload[256 - 4] = {11};
    uint8_t payloadLen       = 0;
    uint8_t outputLen        = 0;

    // INPUT variables:    <AES_128_Key> <IV/Nonce> <PlainText>
    char   *key  = NULL;
    uint8_t Klen = 0;

    char   *nonce = NULL;
    uint8_t Nlen  = 0;

    char   *PT    = NULL;
    uint8_t PTlen = 0;

    // OUTPUT variables:    [GeneratedTag] [CT (CipherText)]
    // uint8_t AuthTaglen = SETUP_AUTHTAGLEN;

    uint8_t CTlen = 0;

    // local var.
    uint8_t *ptr;
    uint8_t  i;

    /*
    **
    ** Message format exchanged between CPU3_APP and CPU2_NBU:
    ** ------------------------------------------------------
    **
    **       --1B--   --1B--   --1B--   -----1B----   ---n*1B---
    **       length | cmd_id | action | return_code | body[256-4]
    **
    **
    ** INPUT payload body format:
    ** -------------------------
    **
    **                    -1B-   -n*1B-   -1B-   -n*1B-    -1B-    ----n*1B---
    **       body format: Klen |  key   | Nlen |  nonce | PTlen | PT (PlainText)
    **
    **                    key:       AES 128 bits key (hexa value sent, )
    **                    nonce:     initialization vector (nonce) (hexa value sent)
    **                    PT:        input plain text to encrypt in the CCM mode
    **
    **
    ** OUTPUT payload body format:
    ** --------------------------
    **
    **       return_code (OT_ERROR_NONE | OT_ERROR_FAILED | OT_ERROR_INVALID_ARGS | OT_ERROR_NOT_A_FEATURE)
    **
    **                    -1B-    -1B-    ---1B---     -1B-    ----4B---    -n*1B-    ----n*1B----
    **       body format: Klen |  Nlen | AuthTaglen | CTlen | BRIC retval |  Tag   | CT (CipherText)
    **
    **                    Klen:        size of the AES 128 bits key
    **                    Nlen:        size of the initialization vector (nonce)
    **                    AuthTaglen:  size of the authentication tag generated after encryption
    **                    CTlen:       size of the resulting ciphered text
    **                    BRIC retval: return value of the encryption done by the BRIC HW Crypto module
    **                    if ( BRIC_retval == Success )
    **                        ..........alignment to next 16-Byte boundary..........
    **                        Tag:         the authentication tag generated after encryption (size: 10 Bytes)
    **                        ..........alignment to next 16-Byte boundary..........
    **                        CT:          resulting ciphered text (MAX size: 230 Bytes)
    */

    if ((aArgsLength == 0) || (aArgsLength < 3) || (aArgsLength > 3))
    {
        if (strcmp(aArgs[0], "help") == 0)
        {
            otCliOutputFormat("Usage: radio_nxp encrypt <AES_128_Key> <IV/Nonce> <PlainText>\r\n");
            otCliOutputFormat("\r\n");
            otCliOutputFormat("Description: This function encrypts the input plain text in the CCM mode as specified "
                              "in [NIST SP 800-38C]\r\n");
            otCliOutputFormat("\r\n");
            otCliOutputFormat("Arguments:\r\n");
            otCliOutputFormat("    AES_128_Key      AES 128 bits key (size: 16 Bytes)\r\n");
            otCliOutputFormat(
                "    IV/Nonce         initialization vector (nonce) (sizes: 7,8,9,10,11,12 or 13 Bytes)\r\n");
            otCliOutputFormat(
                "    PlainText        input plain text to encrypt in the CCM mode (MAX size: 230 Bytes)\r\n");
            otCliOutputFormat("\r\n");
            otCliOutputFormat("Results:\r\n");
            otCliOutputFormat(
                "    GTag             the authentication tag generated after encryption (size: 10 Bytes)\r\n");
            otCliOutputFormat("    CipherText       resulting ciphered text (MAX size: 230 Bytes)\r\n");

            error = OT_ERROR_NONE;
        }
        else
        {
            otCliOutputFormat("Description: This function encrypts the input plain text in the CCM mode as specified "
                              "in [NIST SP 800-38C]\r\n");
            otCliOutputFormat("Usage:       radio_nxp encrypt <AES_128_Key> <IV/Nonce> <PlainText>\r\n");
            otCliOutputFormat("Please type 'radio_nxp encrypt help' to see the arguments in detail\r\n");

            error = OT_ERROR_INVALID_ARGS;
        }
    }
    else
    {
        // Prepare Payload message to CPU2_NBU
        ptr = payload;
        ++ptr;                           // payload[0] = {11}
        *ptr++ = cmdId;                  // payload[1] = MFG_CMD_BRIC_ENCRYPT=70
        *ptr++ = MFG_AES128_CCM_Encrypt; // payload[2] = either MFG_CMD_ACTION_GET=0 (Use BRIC HW Crypto)
        //              or     MFG_CMD_ACTION_SET=1 (Use OT SW layer)
        *ptr++ = OT_ERROR_NONE; // payload[3] = return_code (set to OT_ERROR_NONE=0)

        // 2 'key' input formats allowed (either ASCII __OR__ Hexadecimal)
        //     Example: (ASCII) 1234567890123456 __OR__ (Hex) 0x01020304050607080900010203040506
        key  = aArgs[0];
        Klen = strlen(aArgs[0]);

        if (key[0] == '0' && key[1] == 'x')
        {
            key += 2;
            Klen -= 2;
            *ptr++ = Klen / 2;
            M_AHTOH(key, Klen, ptr);
            Klen = Klen / 2;
        }
        else
        {
            *ptr++ = Klen; // Klen
            M_ATOH(key, Klen, ptr);
        }

        // 2 'IV/Nonce' input formats allowed (either ASCII __OR__ Hexadecimal)
        //     Example: (ASCII) 0123456789012 __OR__ (Hex) 0x00010203040506070809000102
        nonce = aArgs[1];
        Nlen  = strlen(aArgs[1]);

        if (nonce[0] == '0' && nonce[1] == 'x')
        {
            nonce += 2;
            Nlen -= 2;
            *ptr++ = Nlen / 2;
            M_AHTOH(nonce, Nlen, ptr);
            Nlen = Nlen / 2;
        }
        else
        {
            *ptr++ = Nlen; // Nlen
            M_ATOH(nonce, Nlen, ptr);
        }

        // 2 'PlainText' input formats allowed (either ASCII __OR__ Hexadecimal)
        //     Example: (ASCII) Hello_World! __OR__ (Hex) 0x48656c6c6f5f576f726c6421
        PT    = aArgs[2];
        PTlen = strlen(aArgs[2]);

        if (PT[0] == '0' && PT[1] == 'x')
        {
            PT += 2;
            PTlen -= 2;
            *ptr++ = PTlen / 2;
            M_AHTOH(PT, PTlen, ptr);
            PTlen = PTlen / 2;
        }
        else
        {
            *ptr++ = PTlen; // PTlen
            for (i = 0; i < PTlen; i++, ptr++) *ptr = PT[i];
        }

        // lengths sent and received to/from CPU2_NBU
        payloadLen = (Klen + 1) + (Nlen + 1) + (PTlen + 1) + 4;
        CTlen      = PTlen;
        outputLen  = PAYLOAD_LENGTH_STATUS + PAYLOAD_LENGTH_TAG + CTlen;

        if (payload[2] == MFG_CMD_ACTION_USE_BRIC_CRYPTO)
        {
            //--------------------------------------------------------------------------------
            // Delegate Cryptograhic operation (using BRIC Crypto HW nodule) to CPU2_NBU
            //--------------------------------------------------------------------------------
            otPlatRadioMfgCommand(aContext, SPINEL_CMD_VENDOR_NXP_MFG, (uint8_t *)payload, payloadLen, &outputLen);

            if (payload[3])
            {
                if (payload[3] == 0xff)
                {
                    // ERROR returned by CPU2_NBU: Invalid Input arguments!
                    otCliOutputFormat("ERROR: Invalid Input arguments!\r\n");
                }
                else
                {
                    // ERROR returned by CPU2_NBU / BRIC HW Crypto module
                    otCliOutputFormat("ERROR: AES128_CCM Encryption error '%d'\r\n", (int8_t)payload[3]);
                }
            }
            else
            {
                // SUCCESS: AES128_CCM Encryption operation did the job :-)

                // dump GTag
                M_HTOA("GeneratedTag: 0x", payload[6], &payload[PAYLOAD_LENGTH_STATUS]);

                // dump CT (CipherText)
                M_HTOA("CipherText:   0x", payload[7], &payload[PAYLOAD_LENGTH_STATUS + PAYLOAD_LENGTH_TAG]);
            }
        }
        else if (payload[2] == MFG_CMD_ACTION_USE_OT_SW_CRYPTO)
        {
            char   *MY_key  = NULL;
            uint8_t MY_Klen = 0;

            char   *MY_nonce = NULL;
            uint8_t MY_Nlen  = 0;

            char    *MY_PT    = NULL;
            uint32_t MY_PTlen = 0;

            // extract Crypto info from payload[]
            MY_Nlen  = (uint8_t)payload[4 + MY_Klen + 1];
            MY_nonce = (char *)&payload[4 + MY_Klen + 1 + 1];

            MY_PTlen = (uint32_t)payload[4 + MY_Klen + 1 + Nlen + 1];
            MY_PT    = (char *)&payload[4 + MY_Klen + 1 + Nlen + 1 + 1];

            char     MY_CT[256 - PAYLOAD_LENGTH_STATUS - PAYLOAD_LENGTH_TAG] = {0};
            uint32_t MY_CTlen                                                = MY_PTlen;
            char     MY_Tag[16]                                              = {0};
            uint8_t  MY_AuthTaglen                                           = SETUP_AUTHTAGLEN;

            otCryptoKey MY_CryptoKey;

            MY_CryptoKey.mKey       = (uint8_t *)MY_key;
            MY_CryptoKey.mKeyLength = (uint16_t)MY_Klen; // MY_Klen
            MY_CryptoKey.mKeyRef    = 0;
            if ((MY_Klen != 16) || ((MY_Nlen < 7) || (MY_Nlen > 13)) ||
                (MY_PTlen > (256 - PAYLOAD_LENGTH_STATUS - PAYLOAD_LENGTH_TAG)))
            {
                // ERROR: Invalid Input arguments!
                otCliOutputFormat("ERROR: Invalid Input arguments!\r\n");
            }
            else
            {
                //--------------------------------------------------------------------------------
                // Delegate Cryptograhic operation (using OpenThread SW layer) to CPU3_APP
                //--------------------------------------------------------------------------------
                otCryptoAesCcm(&MY_CryptoKey, MY_AuthTaglen, MY_nonce, MY_Nlen, NULL, 0,
                               MY_PT, // IN
                               MY_CT, // OUT
                               (uint32_t)MY_PTlen,
                               true,    // AES128_CCM Encrypt
                               MY_Tag); // OUT
                // dump GTag
                M_HTOA("GeneratedTag: 0x", MY_AuthTaglen, MY_Tag);

                // dump CT (CipherText)
                M_HTOA("CipherText:   0x", MY_CTlen, MY_CT);
            }
        }
        else
        {
            otCliOutputFormat("ERROR ... Choose either AES128_CCM Encrypt done by BRIC HW Crypto Engine or done by "
                              "OpenThread SW layer\r\n");
        }

        error = OT_ERROR_NONE;
    }

    return error;
}

static otError ProcessAES128CCMDecrypt(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    /*
    ** AES module within OpenThread SW layer:
    ** -------------------------------------
    **
    ** OpenThread Security applies AES CCM (Counter with CBC-MAC) crypto to encrypt/decrypt the IEEE 802.15.4 or
    **     MLE messages and validates the message integration code.
    ** Hardware acceleration should at least support basic AES ECB (Electronic Codebook Book) mode for AES CCM basic
    *functional call.    **
    ** REF1: [ AES module ]               https://openthread.io/guides/porting/implement-advanced-features
    ** REF2: [ void otCryptoAesCcm(...) ] https://openthread.io/reference/group/api-crypto
    **
    */

    /*
    ** CMD Syntax:          radio_nxp  decrypt  <AES_128_Key> <IV/Nonce> <GeneratedTag> <CT/CipherText>
    ** Return:              [ComputedTag] [PT (PlainText)]
    */

    // IMU variables
    otError error = OT_ERROR_INVALID_ARGS;

    uint8_t cmdId = MFG_CMD_BRIC_DECRYPT;

    uint8_t payload[256 - 4] = {11};
    uint8_t payloadLen       = 0;
    uint8_t outputLen        = 0;

    // INPUT variables:    <AES_128_Key> <IV/Nonce> <GeneratedTag> <CipherText>
    char   *key  = NULL;
    uint8_t Klen = 0;

    char   *nonce = NULL;
    uint8_t Nlen  = 0;

    char   *Tag        = NULL; // IN & OUT
    uint8_t AuthTaglen = 0;

    char   *CT    = NULL;
    uint8_t CTlen = 0;

    // OUTPUT variables:    [ComputedTag] [PT (PlainText)]
    uint8_t PTlen = 0;

    // local var.
    uint8_t *ptr;
    uint8_t  i;

    /*
    **
    ** Message format exchanged between CPU3_APP and CPU2_NBU:
    ** ------------------------------------------------------
    **
    **       --1B--   --1B--   --1B--   -----1B----   ---n*1B---
    **       length | cmd_id | action | return_code | body[256-4]
    **
    **
    ** INPUT payload body format:
    ** -------------------------
    **
    **                    -1B-   -n*1B-   -1B-   -n*1B-    -1B-    ----n*1B---
    **       body format: Klen |  key   | Nlen |  nonce | AuthTaglen |  GTag  | CTlen | CT (CipherText)
    **
    **                    key:       AES 128 bits key (hexa value sent, )
    **                    nonce:     initialization vector (nonce) (hexa value sent)
    **                    GTag:      the authentication tag generated after encryption
    **                    CT:        input ciphered text to encrypt in the CCM mode
    **
    **
    ** OUTPUT payload body format:
    ** --------------------------
    **
    **       return_code (OT_ERROR_NONE | OT_ERROR_FAILED | OT_ERROR_INVALID_ARGS | OT_ERROR_NOT_A_FEATURE)
    **
    **                    -1B-    -1B-    ---1B---     -1B-    ----4B---    -n*1B-    ----n*1B----
    **       body format: Klen |  Nlen | AuthTaglen | CTlen | BRIC retval |  CTag   | PT (PlainText)
    **
    **                    Klen:        size of the AES 128 bits key
    **                    Nlen:        size of the initialization vector (nonce)
    **                    AuthTaglen:  size of the authentication tag generated after encryption
    **                    CTlen:       size of the resulting ciphered text
    **                    BRIC retval: return value of the encryption done by the BRIC HW Crypto module
    **                    if ( BRIC_retval == Success )
    **                        ..........alignment to next 16-Byte boundary..........
    **                        CTag:        the authentication tag computed after decryption (size: 10 Bytes)
    **                        ..........alignment to next 16-Byte boundary..........
    **                        PT:          resulting plain text (MAX size: 230 Bytes)
    */

    if ((aArgsLength == 0) || (aArgsLength < 4) || (aArgsLength > 4))
    {
        if (strcmp(aArgs[0], "help") == 0)
        {
            otCliOutputFormat("Usage: radio_nxp decrypt <AES_128_Key> <IV/Nonce> <GTag> <CipherText>\r\n");
            otCliOutputFormat("\r\n");
            otCliOutputFormat("Description: This function decrypts the input ciphered text in the CCM mode as "
                              "specified in [NIST SP 800-38C]\r\n");
            otCliOutputFormat("\r\n");
            otCliOutputFormat("Arguments:\r\n");
            otCliOutputFormat("    AES_128_Key      AES 128 bits key (size: 16 Bytes)\r\n");
            otCliOutputFormat(
                "    IV/Nonce         initialization vector (nonce) (sizes: 7,8,9,10,11,12 or 13 Bytes)\r\n");
            otCliOutputFormat(
                "    GTag             the authentication tag generated after encryption (size: 10 Bytes)\r\n");
            otCliOutputFormat(
                "    CipherText       input ciphered text to decrypt in the CCM mode (MAX size: 230 Bytes)\r\n");
            otCliOutputFormat("\r\n");
            otCliOutputFormat("Result:\r\n");
            otCliOutputFormat(
                "    CTag             the authentication tag computed after decryption (size: 10 Bytes)\r\n");
            otCliOutputFormat("    PlainText        resulting plain text (MAX size: 230 Bytes)\r\n");
            error = OT_ERROR_NONE;
        }
        else
        {
            otCliOutputFormat("Description: This function decrypts the input ciphered text in the CCM mode as "
                              "specified in [NIST SP 800-38C]\r\n");
            otCliOutputFormat("Usage:       radio_nxp decrypt <AES_128_Key> <IV/Nonce> <GTag> <CipherText>\r\n");
            otCliOutputFormat("Please type 'radio_nxp decrypt help' to see the arguments in detail\r\n");

            error = OT_ERROR_INVALID_ARGS;
        }
    }
    else
    {
        // Prepare Payload message to CPU2_NBU
        ptr = payload;
        ++ptr;                           // payload[0] = {11}
        *ptr++ = cmdId;                  // payload[1] = MFG_CMD_BRIC_DECRYPT=71
        *ptr++ = MFG_AES128_CCM_Decrypt; // payload[2] = either MFG_CMD_ACTION_GET=0 (Use BRIC HW Crypto)
                                         //              or     MFG_CMD_ACTION_SET=1 (Use OT SW layer)
        *ptr++ = OT_ERROR_NONE;          // payload[3] = return_code (set to OT_ERROR_NONE=0)

        // 2 'key' input formats allowed (either ASCII __OR__ Hexadecimal)
        //     Example: (ASCII) 1234567890123456 __OR__ (Hex) 0x01020304050607080900010203040506
        key  = aArgs[0];
        Klen = strlen(aArgs[0]);

        if (key[0] == '0' && key[1] == 'x')
        {
            key += 2;
            Klen -= 2;
            *ptr++ = Klen / 2;
            M_AHTOH(key, Klen, ptr);
            Klen = Klen / 2;
        }
        else
        {
            *ptr++ = Klen; // Klen
            M_ATOH(key, Klen, ptr);
        }

        // 2 'IV/Nonce' input formats allowed (either ASCII __OR__ Hexadecimal)
        //     Example: (ASCII) 0123456789012 __OR__ (Hex) 0x00010203040506070809000102
        nonce = aArgs[1];
        Nlen  = strlen(aArgs[1]);

        if (nonce[0] == '0' && nonce[1] == 'x')
        {
            nonce += 2;
            Nlen -= 2;
            *ptr++ = Nlen / 2;
            M_AHTOH(nonce, Nlen, ptr);
            Nlen = Nlen / 2;
        }
        else
        {
            *ptr++ = Nlen; // Nlen
            M_ATOH(nonce, Nlen, ptr);
        }

        // 2 'Tag' input formats allowed (either ASCII __OR__ Hexadecimal)
        //     Example: (ASCII) 0123AbC789 __OR__ (Hex) 0x000102030a0b0c070809
        Tag        = aArgs[2];
        AuthTaglen = strlen(aArgs[2]);

        if (Tag[0] == '0' && Tag[1] == 'x')
        {
            Tag += 2;
            AuthTaglen -= 2;
            *ptr++ = AuthTaglen / 2;
            M_AHTOH(Tag, AuthTaglen, ptr);
            AuthTaglen = AuthTaglen / 2;
        }
        else
        {
            *ptr++ = AuthTaglen; // AuthTaglen
            M_ATOH(Tag, AuthTaglen, ptr);
        }

        // 2 'CipherText' input formats allowed (either ASCII __OR__ Hexadecimal)
        //     Example: (ASCII) 0123AbC789012 __OR__ (Hex) 0x000102030a0b0c070809000102
        CT    = aArgs[3];
        CTlen = strlen(aArgs[3]);

        if (CT[0] == '0' && CT[1] == 'x')
        {
            CT += 2;
            CTlen -= 2;
            *ptr++ = CTlen / 2;
            M_AHTOH(CT, CTlen, ptr);
            CTlen = CTlen / 2;
        }
        else
        {
            *ptr++ = CTlen; // CTlen
            for (i = 0; i < CTlen; i++, ptr++) *ptr = CT[i];
        }

        // lengths sent and received to/from CPU2_NBU
        payloadLen = (Klen + 1) + (Nlen + 1) + (AuthTaglen + 1) + (CTlen + 1) + 4;
        PTlen      = CTlen;
        outputLen  = PAYLOAD_LENGTH_STATUS + PAYLOAD_LENGTH_TAG + PTlen;
        if (payload[2] == MFG_CMD_ACTION_USE_BRIC_CRYPTO)
        {
            //--------------------------------------------------------------------------------
            // Delegate Cryptograhic operation (using BRIC Crypto HW nodule) to CPU2_NBU
            //--------------------------------------------------------------------------------
            otPlatRadioMfgCommand(aContext, SPINEL_CMD_VENDOR_NXP_MFG, (uint8_t *)payload, payloadLen, &outputLen);

            if (payload[3])
            {
                if (payload[3] == 0xff)
                {
                    // ERROR returned by CPU2_NBU: Invalid Input arguments!
                    otCliOutputFormat("ERROR: Invalid Input arguments!\r\n");
                }
                else
                {
                    // ERROR returned by CPU2_NBU / BRIC HW Crypto module
                    otCliOutputFormat("ERROR: AES128_CCM Decryption error '%d'\r\n", (int8_t)payload[3]);
                }
            }
            else
            {
                // SUCCESS: AES128_CCM Decryption operation did the job :-)

                // dump CTag
                M_HTOA("ComputedTag:  0x", payload[6], &payload[PAYLOAD_LENGTH_STATUS]);

                // dump PT (PlainText)
                M_HTOA("PlainText:    0x", payload[7], &payload[PAYLOAD_LENGTH_STATUS + PAYLOAD_LENGTH_TAG]);
            }
        }
        else if (payload[2] == MFG_CMD_ACTION_USE_OT_SW_CRYPTO)
        {
            char   *MY_key  = NULL;
            uint8_t MY_Klen = 0;

            char   *MY_nonce = NULL;
            uint8_t MY_Nlen  = 0;

            // char  *MY_Tag        = NULL;
            uint8_t MY_AuthTaglen = 0;

            char    *MY_CT    = NULL;
            uint32_t MY_CTlen = 0;

            // extract Crypto info from payload[]
            MY_Klen = (uint8_t)payload[4];
            MY_key  = (char *)&payload[4 + 1];

            MY_Nlen  = (uint8_t)payload[4 + MY_Klen + 1];
            MY_nonce = (char *)&payload[4 + MY_Klen + 1 + 1];

            MY_AuthTaglen = (uint8_t)payload[4 + MY_Klen + 1 + Nlen + 1];
            // MY_Tag        = (char *)   &payload[4+MY_Klen+1+Nlen+1+1];

            MY_CTlen = (uint32_t)payload[4 + MY_Klen + 1 + Nlen + 1 + AuthTaglen + 1];
            MY_CT    = (char *)&payload[4 + MY_Klen + 1 + Nlen + 1 + AuthTaglen + 1 + 1];

            char     MY_PT[256 - PAYLOAD_LENGTH_STATUS - PAYLOAD_LENGTH_TAG] = {0};
            uint32_t MY_PTlen                                                = MY_CTlen;

            char    MY_Comp_Tag[16]    = {0};
            uint8_t MY_Comp_AuthTaglen = MY_AuthTaglen;

            otCryptoKey MY_CryptoKey;

            MY_CryptoKey.mKey       = (uint8_t *)MY_key;
            MY_CryptoKey.mKeyLength = (uint16_t)MY_Klen; // MY_Klen
            MY_CryptoKey.mKeyRef    = 0;

            if ((MY_Klen != 16) || ((MY_Nlen < 7) || (MY_Nlen > 13)) || (MY_AuthTaglen != SETUP_AUTHTAGLEN) ||
                (MY_CTlen > (256 - PAYLOAD_LENGTH_STATUS - PAYLOAD_LENGTH_TAG)))
            {
                // ERROR: Invalid Input arguments!
                otCliOutputFormat("ERROR: Invalid Input arguments!\r\n");
            }
            else
            {
                //--------------------------------------------------------------------------------
                // Delegate Cryptograhic operation (using OpenThread SW layer) to CPU3_APP
                //--------------------------------------------------------------------------------
                otCryptoAesCcm(&MY_CryptoKey, MY_AuthTaglen, MY_nonce, MY_Nlen, NULL, 0,
                               MY_PT, // OUT
                               MY_CT, // IN
                               (uint32_t)MY_CTlen,
                               false,        // AES128_CCM Decrypt
                               MY_Comp_Tag); // OUT

                // dump CTag
                M_HTOA("ComputedTag:  0x", MY_Comp_AuthTaglen, MY_Comp_Tag);

                // dump PT (PlainText)
                M_HTOA("PlainText:    0x", MY_PTlen, MY_PT);
            }
        }
        else
        {
            otCliOutputFormat("ERROR ... Choose either AES128_CCM Decrypt done by BRIC HW Crypto Engine or done by "
                              "OpenThread SW layer\r\n");
        }

        error = OT_ERROR_NONE;
    }

    return error;
}

static otError mfgGenericCommand(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    /*
    ** Annex management FFU 15.4 :
    ** ---------------------------
    */

    /*
    ** CMD Syntax: radio_nxp annex <Annex Type> <data>
    */

    otError error = OT_ERROR_INVALID_ARGS;
    uint8_t cmdId = MFG_CMD_GENERIC;

    uint8_t payload[256 - 4] = {6};
    uint8_t payloadLen       = 0;
    uint8_t outputLen        = 0;

    // INPUT variables:    <cmdType> <cmdData>
    uint8_t cmdType = 0;
    char   *cmdData = NULL;
    uint8_t dataLen = 0;

    // uint8_t CTlen = 0;

    // local var.
    uint8_t *ptr;

    /*
    **
    ** Message format exchanged between CPU3_APP and CPU2_NBU:
    ** ------------------------------------------------------
    **
    **       --1B--   --1B--   --1B--   -----1B----   ---n*1B---
    **       length | cmd_id | action | return_code | body[256-4]
    **
    **
    ** INPUT payload body format:
    ** -------------------------
    **                      --1B--             --1B--           --n*1B--
    **       body format: Command Type | Command Length Data | Command Data
    **
    **                    Command Type        : 1 Byte (55 to 108, see ManufactureDataStructure_x.x.x.doc)
    **                    Command Length Data : 1 Byte
    **                    Command Data        : n Bytes
    */

    if ((aArgsLength == 0) || (aArgsLength < 3) || (aArgsLength > 3))
    {
        if (strcmp(aArgs[0], "help") == 0)
        {
            otCliOutputFormat("Usage: radio_nxp mfgcmd 255 <Type> <Data>\r\n");
            otCliOutputFormat("\r\n");
            otCliOutputFormat("Description: This function sends a generic command with its type and data\r\n");
            otCliOutputFormat("\r\n");
            otCliOutputFormat("Arguments:\r\n");
            otCliOutputFormat("    Type (size: 1 Bytes)\r\n");
            otCliOutputFormat("    Data (size: n*1 Bytes)\r\n");

            error = OT_ERROR_NONE;
        }
        else
        {
            otCliOutputFormat("Description: This function sends a generic command with its type and data \r\n");
            otCliOutputFormat("Usage:       radio_nxp mfgcmd 255 <Command Type> <Command Data>\r\n");
            otCliOutputFormat("Please type 'radio_nxp mfgcmd 255 help' to see the arguments in detail\r\n");
        }
    }
    else
    {
        // Prepare Payload message to CPU2_NBU
        ptr = payload;
        ++ptr;                       // payload[0] = {7}
        *ptr++ = cmdId;              // payload[1] = MFG_CMD_ADVANCE=255
        *ptr++ = MFG_CMD_ACTION_SET; // payload[2] = MFG_CMD_ACTION_SET
        *ptr++ = OT_ERROR_NONE;      // payload[4] = return_code (set to OT_ERROR_NONE=0)

        cmdType = (uint8_t)atoi(aArgs[1]);
        if ((cmdType >= OT_NXP_PLAT_MFG_FIRST_ANNEX_ID) && (cmdType <= OT_NXP_PLAT_MFG_LAST_ANNEX_ID))
        {
            *ptr++ = cmdType; // payload[5] = Annex Type
            otLogInfoPlat("Set Annex: %d", cmdType);

            // cmdData input format allowed Hexadecimal
            cmdData = aArgs[2];
            dataLen = strlen(aArgs[2]);

            error = OT_ERROR_NONE;
        }
        else
        {
            otLogInfoPlat("The Annex Type limit set is out of range");
        }

        if (error == OT_ERROR_NONE)
        {
            if (cmdData[0] == '0' && cmdData[1] == 'x')
            {
                cmdData += 2;
                dataLen -= 2;
                *ptr++ = dataLen / 2;
                M_AHTOH(cmdData, dataLen, ptr);
                dataLen = dataLen / 2;

                error = OT_ERROR_NONE;
            }
            else
            {
                otLogInfoPlat("The Annex Data input format allows only Hexadecimal");
            }
        }

        if (error == OT_ERROR_NONE)
        {
            // lengths sent to CPU2_NBU
            payloadLen = 4 + 1 + 1 + dataLen; // 4 + cmdType + dataLen field + dataLen
            outputLen  = dataLen;

            otPlatRadioMfgCommand(aContext, SPINEL_CMD_VENDOR_NXP_MFG, (uint8_t *)payload, payloadLen, &outputLen);
            if (payload[3] == 0)
            {
                error = OT_ERROR_NONE;
            }
            else if (payload[3] == 0xff)
            {
                // ERROR returned by CPU2_NBU: Invalid Input arguments!
                error = OT_ERROR_INVALID_ARGS;
            }
            else
            {
                error = OT_ERROR_FAILED;
            }
        }
    }

    return error;
}
