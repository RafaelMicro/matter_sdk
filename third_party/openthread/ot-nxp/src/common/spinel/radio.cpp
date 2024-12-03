/*
 *  Copyright (c) 2021, The OpenThread Authors.
 *  Copyright (c) 2022, NXP.
 *
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

/**
 * @file
 * This file implements OpenThread platform driver API in openthread/platform/radio.h.
 *
 */

#include "ot_platform_common.h"

#include <openthread/platform/radio.h>

#include <lib/spinel/radio_spinel.hpp> // nogncheck

#if defined(OT_PLAT_SPINEL_OVER_SPI)
#include "spi_interface.hpp"
static ot::Spinel::RadioSpinel<ot::NXP::SpiInterface> sRadioSpinel;
#elif defined(OT_PLAT_SPINEL_OVER_HDLC)
#include "spinel_hdlc.hpp"
static ot::Spinel::RadioSpinel<ot::NXP::HdlcInterface> sRadioSpinel;
#elif defined(OT_PLAT_SPINEL_HCI_OVER_HDLC)
#include "spinel_hci_hdlc.hpp"
static ot::Spinel::RadioSpinel<ot::NXP::HdlcSpinelHciInterface> sRadioSpinel;
#endif

void otPlatRadioGetIeeeEui64(otInstance *aInstance, uint8_t *aIeeeEui64)
{
    OT_UNUSED_VARIABLE(aInstance);
    SuccessOrDie(sRadioSpinel.Get(SPINEL_PROP_HWADDR, SPINEL_DATATYPE_EUI64_S, aIeeeEui64));
}

void otPlatRadioSetPanId(otInstance *aInstance, uint16_t panid)
{
    OT_UNUSED_VARIABLE(aInstance);
    SuccessOrDie(sRadioSpinel.SetPanId(panid));
}

void otPlatRadioSetExtendedAddress(otInstance *aInstance, const otExtAddress *aAddress)
{
    OT_UNUSED_VARIABLE(aInstance);
    otExtAddress addr;

    for (size_t i = 0; i < sizeof(addr); i++)
    {
        addr.m8[i] = aAddress->m8[sizeof(addr) - 1 - i];
    }

    SuccessOrDie(sRadioSpinel.SetExtendedAddress(addr));
}

void otPlatRadioSetShortAddress(otInstance *aInstance, uint16_t aAddress)
{
    OT_UNUSED_VARIABLE(aInstance);
    SuccessOrDie(sRadioSpinel.SetShortAddress(aAddress));
}

void otPlatRadioSetPromiscuous(otInstance *aInstance, bool aEnable)
{
    OT_UNUSED_VARIABLE(aInstance);
    SuccessOrDie(sRadioSpinel.SetPromiscuous(aEnable));
}

bool otPlatRadioIsEnabled(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.IsEnabled();
}

otError otPlatRadioEnable(otInstance *aInstance)
{
    return sRadioSpinel.Enable(aInstance);
}

otError otPlatRadioDisable(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.Disable();
}

otError otPlatRadioSleep(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.Sleep();
}

otError otPlatRadioReceive(otInstance *aInstance, uint8_t aChannel)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.Receive(aChannel);
}

otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aFrame)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.Transmit(*aFrame);
}

otRadioFrame *otPlatRadioGetTransmitBuffer(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return &sRadioSpinel.GetTransmitFrame();
}

int8_t otPlatRadioGetRssi(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.GetRssi();
}

otRadioCaps otPlatRadioGetCaps(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.GetRadioCaps();
}

const char *otPlatRadioGetVersionString(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.GetVersion();
}

bool otPlatRadioGetPromiscuous(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.IsPromiscuous();
}

void otPlatRadioEnableSrcMatch(otInstance *aInstance, bool aEnable)
{
    OT_UNUSED_VARIABLE(aInstance);
    SuccessOrDie(sRadioSpinel.EnableSrcMatch(aEnable));
}

otError otPlatRadioAddSrcMatchShortEntry(otInstance *aInstance, uint16_t aShortAddress)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.AddSrcMatchShortEntry(aShortAddress);
}

otError otPlatRadioAddSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress)
{
    OT_UNUSED_VARIABLE(aInstance);
    otExtAddress addr;

    for (size_t i = 0; i < sizeof(addr); i++)
    {
        addr.m8[i] = aExtAddress->m8[sizeof(addr) - 1 - i];
    }

    return sRadioSpinel.AddSrcMatchExtEntry(addr);
}

otError otPlatRadioClearSrcMatchShortEntry(otInstance *aInstance, uint16_t aShortAddress)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.ClearSrcMatchShortEntry(aShortAddress);
}

otError otPlatRadioClearSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress)
{
    OT_UNUSED_VARIABLE(aInstance);
    otExtAddress addr;

    for (size_t i = 0; i < sizeof(addr); i++)
    {
        addr.m8[i] = aExtAddress->m8[sizeof(addr) - 1 - i];
    }

    return sRadioSpinel.ClearSrcMatchExtEntry(addr);
}

void otPlatRadioClearSrcMatchShortEntries(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    SuccessOrDie(sRadioSpinel.ClearSrcMatchShortEntries());
}

void otPlatRadioClearSrcMatchExtEntries(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    SuccessOrDie(sRadioSpinel.ClearSrcMatchExtEntries());
}

otError otPlatRadioEnergyScan(otInstance *aInstance, uint8_t aScanChannel, uint16_t aScanDuration)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.EnergyScan(aScanChannel, aScanDuration);
}

otError otPlatRadioGetTransmitPower(otInstance *aInstance, int8_t *aPower)
{
    otError error;

    OT_UNUSED_VARIABLE(aInstance);
    VerifyOrExit(aPower != NULL, error = OT_ERROR_INVALID_ARGS);
    error = sRadioSpinel.GetTransmitPower(*aPower);

exit:
    return error;
}

otError otPlatRadioSetTransmitPower(otInstance *aInstance, int8_t aPower)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.SetTransmitPower(aPower);
}

otError otPlatRadioGetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t *aThreshold)
{
    otError error;

    OT_UNUSED_VARIABLE(aInstance);
    VerifyOrExit(aThreshold != NULL, error = OT_ERROR_INVALID_ARGS);
    error = sRadioSpinel.GetCcaEnergyDetectThreshold(*aThreshold);

exit:
    return error;
}

otError otPlatRadioSetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t aThreshold)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.SetCcaEnergyDetectThreshold(aThreshold);
}

int8_t otPlatRadioGetReceiveSensitivity(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.GetReceiveSensitivity();
}

#if OPENTHREAD_CONFIG_PLATFORM_RADIO_COEX_ENABLE
otError otPlatRadioSetCoexEnabled(otInstance *aInstance, bool aEnabled)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.SetCoexEnabled(aEnabled);
}

bool otPlatRadioIsCoexEnabled(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.IsCoexEnabled();
}

otError otPlatRadioGetCoexMetrics(otInstance *aInstance, otRadioCoexMetrics *aCoexMetrics)
{
    OT_UNUSED_VARIABLE(aInstance);

    otError error = OT_ERROR_NONE;

    VerifyOrExit(aCoexMetrics != NULL, error = OT_ERROR_INVALID_ARGS);

    error = sRadioSpinel.GetCoexMetrics(*aCoexMetrics);

exit:
    return error;
}
#endif

#if OPENTHREAD_CONFIG_DIAG_ENABLE
otError otPlatDiagProcess(otInstance *aInstance, int argc, char *argv[], char *aOutput, size_t aOutputMaxLen)
{
    // deliver the platform specific diags commands to radio only ncp.
    OT_UNUSED_VARIABLE(aInstance);
    char  cmd[OPENTHREAD_CONFIG_DIAG_CMD_LINE_BUFFER_SIZE] = {'\0'};
    char *cur                                              = cmd;
    char *end                                              = cmd + sizeof(cmd);

    for (int index = 0; index < argc; index++)
    {
        cur += snprintf(cur, static_cast<size_t>(end - cur), "%s ", argv[index]);
    }

    return sRadioSpinel.PlatDiagProcess(cmd, aOutput, aOutputMaxLen);
}

void otPlatDiagModeSet(bool aMode)
{
    SuccessOrExit(sRadioSpinel.PlatDiagProcess(aMode ? "start" : "stop", NULL, 0));
    sRadioSpinel.SetDiagEnabled(aMode);

exit:
    return;
}

bool otPlatDiagModeGet(void)
{
    return sRadioSpinel.IsDiagEnabled();
}

void otPlatDiagTxPowerSet(int8_t aTxPower)
{
    char cmd[OPENTHREAD_CONFIG_DIAG_CMD_LINE_BUFFER_SIZE];

    snprintf(cmd, sizeof(cmd), "power %d", aTxPower);
    SuccessOrExit(sRadioSpinel.PlatDiagProcess(cmd, NULL, 0));

exit:
    return;
}

void otPlatDiagChannelSet(uint8_t aChannel)
{
    char cmd[OPENTHREAD_CONFIG_DIAG_CMD_LINE_BUFFER_SIZE];

    snprintf(cmd, sizeof(cmd), "channel %d", aChannel);
    SuccessOrExit(sRadioSpinel.PlatDiagProcess(cmd, NULL, 0));

exit:
    return;
}

void otPlatDiagRadioReceived(otInstance *aInstance, otRadioFrame *aFrame, otError aError)
{
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aFrame);
    OT_UNUSED_VARIABLE(aError);
}

void otPlatDiagAlarmCallback(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
}
#endif // OPENTHREAD_CONFIG_DIAG_ENABLE

uint32_t otPlatRadioGetSupportedChannelMask(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.GetRadioChannelMask(false);
}

uint32_t otPlatRadioGetPreferredChannelMask(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.GetRadioChannelMask(true);
}

otRadioState otPlatRadioGetState(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.GetState();
}

void otPlatRadioSetMacKey(otInstance             *aInstance,
                          uint8_t                 aKeyIdMode,
                          uint8_t                 aKeyId,
                          const otMacKeyMaterial *aPrevKey,
                          const otMacKeyMaterial *aCurrKey,
                          const otMacKeyMaterial *aNextKey,
                          otRadioKeyType          aKeyType)
{
    SuccessOrDie(sRadioSpinel.SetMacKey(aKeyIdMode, aKeyId, aPrevKey, aCurrKey, aNextKey));
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aKeyType);
}

void otPlatRadioSetMacFrameCounter(otInstance *aInstance, uint32_t aMacFrameCounter)
{
    SuccessOrDie(sRadioSpinel.SetMacFrameCounter(aMacFrameCounter, false));
    OT_UNUSED_VARIABLE(aInstance);
}

void otPlatRadioSetMacFrameCounterIfLarger(otInstance *aInstance, uint32_t aMacFrameCounter)
{
    SuccessOrDie(sRadioSpinel.SetMacFrameCounter(aMacFrameCounter, true));
    OT_UNUSED_VARIABLE(aInstance);
}

uint64_t otPlatRadioGetNow(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.GetNow();
}

uint8_t otPlatRadioGetCslAccuracy(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    return sRadioSpinel.GetCslAccuracy();
}

uint8_t otPlatRadioGetCslUncertainty(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    return sRadioSpinel.GetCslUncertainty();
}

otError otPlatRadioSetChannelMaxTransmitPower(otInstance *aInstance, uint8_t aChannel, int8_t aMaxPower)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.SetChannelMaxTransmitPower(aChannel, aMaxPower);
}

otError otPlatRadioSetRegion(otInstance *aInstance, uint16_t aRegionCode)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.SetRadioRegion(aRegionCode);
}

otError otPlatRadioGetRegion(otInstance *aInstance, uint16_t *aRegionCode)
{
    OT_UNUSED_VARIABLE(aInstance);
    return sRadioSpinel.GetRadioRegion(aRegionCode);
}

otError otPlatRadioReceiveAt(otInstance *aInstance, uint8_t aChannel, uint32_t aStart, uint32_t aDuration)
{
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aChannel);
    OT_UNUSED_VARIABLE(aStart);
    OT_UNUSED_VARIABLE(aDuration);
    return OT_ERROR_NOT_IMPLEMENTED;
}

#define MAX_ARGS_BUFFER_SIZE 256

otError otPlatRadioMfgCommand(otInstance   *aInstance,
                              uint32_t      aKey,
                              uint8_t      *payload,
                              const uint8_t payloadLenIn,
                              uint8_t      *payloadLenOut)
{
    uint8_t        buffer[MAX_ARGS_BUFFER_SIZE]; // temporary buffer used to be passed as arg of GetWithParam
    spinel_ssize_t packed;

    OT_UNUSED_VARIABLE(aInstance);

    packed = spinel_datatype_pack(buffer, sizeof(buffer), SPINEL_DATATYPE_DATA_S, payload, payloadLenIn);
    return sRadioSpinel.GetWithParam((spinel_prop_key_t)aKey, buffer, static_cast<spinel_size_t>(packed),
                                     SPINEL_DATATYPE_DATA_S, payload, &payloadLenOut);
}

otError otPlatRadioCcaConfigValue(uint32_t aKey, otCCAModeConfig *aCcaConfig, uint8_t aSetValue)
{
    otError error;
    uint8_t aCcaMode, aCca1Threshold, aCca2CorrThreshold, aCca2MinNumOfCorrTh;

    if (aSetValue)
    {
        error = sRadioSpinel.Set(
            (spinel_prop_key_t)aKey,
            SPINEL_DATATYPE_STRUCT_S(
                SPINEL_DATATYPE_UINT8_S SPINEL_DATATYPE_UINT8_S SPINEL_DATATYPE_UINT8_S SPINEL_DATATYPE_UINT8_S),
            aCcaConfig->mCcaMode, aCcaConfig->mCca1Threshold, aCcaConfig->mCca2CorrThreshold,
            aCcaConfig->mCca2MinNumOfCorrTh);
    }
    else
    {
        error = sRadioSpinel.Get(
            (spinel_prop_key_t)aKey,
            SPINEL_DATATYPE_STRUCT_S(
                SPINEL_DATATYPE_UINT8_S SPINEL_DATATYPE_UINT8_S SPINEL_DATATYPE_UINT8_S SPINEL_DATATYPE_UINT8_S),
            &aCcaMode, &aCca1Threshold, &aCca2CorrThreshold, &aCca2MinNumOfCorrTh);
        aCcaConfig->mCcaMode            = aCcaMode;
        aCcaConfig->mCca1Threshold      = aCca1Threshold;
        aCcaConfig->mCca2CorrThreshold  = aCca2CorrThreshold;
        aCcaConfig->mCca2MinNumOfCorrTh = aCca2MinNumOfCorrTh;
    }

    return error;
}

void otPlatRadioInitSpinelInterface(void)
{
    sRadioSpinel.GetSpinelInterface().Init();
}

void otPlatRadioInit(void)
{
#if OPENTHREAD_RADIO_RESET_ENABLE
    bool resetRadio = true;
#else
    bool resetRadio = false;
#endif

    sRadioSpinel.Init(resetRadio, false);
}

void otPlatRadioDeinit(void)
{
    sRadioSpinel.Deinit();
}

void otPlatRadioProcess(const otInstance *aInstance)
{
    sRadioSpinel.Process(aInstance);
    // sRadioSpinel.GetSpinelInterface().Process(aInstance);
}

void otPlatRadioSendFrameToSpinelInterface(uint8_t *buf, uint16_t length)
{
    sRadioSpinel.GetSpinelInterface().SendFrame(buf, length);
}

otError otPlatRadioSendSetPropVendorUint8Cmd(uint32_t aKey, uint8_t value)
{
    return sRadioSpinel.Set((spinel_prop_key_t)aKey, SPINEL_DATATYPE_UINT8_S, value);
}

otError otPlatRadioSendGetPropVendorUint8Cmd(uint32_t aKey, uint8_t *value)
{
    return sRadioSpinel.Get((spinel_prop_key_t)aKey, SPINEL_DATATYPE_UINT8_S, value);
}

otError otPlatRadioSendSetPropVendorUint16Cmd(uint32_t aKey, uint16_t value)
{
    return sRadioSpinel.Set((spinel_prop_key_t)aKey, SPINEL_DATATYPE_UINT16_S, value);
}

otError otPlatRadioSendGetPropVendorUint16Cmd(uint32_t aKey, uint16_t *value)
{
    return sRadioSpinel.Get((spinel_prop_key_t)aKey, SPINEL_DATATYPE_UINT16_S, value);
}

otError otPlatRadioSendSetPropVendorUint32Cmd(uint32_t aKey, uint32_t value)
{
    return sRadioSpinel.Set((spinel_prop_key_t)aKey, SPINEL_DATATYPE_UINT32_S, value);
}

otError otPlatRadioSendGetPropVendorUint32Cmd(uint32_t aKey, uint32_t *value)
{
    return sRadioSpinel.Get((spinel_prop_key_t)aKey, SPINEL_DATATYPE_UINT32_S, value);
}

otError otPlatRadioSendSetPropVendorUint64Cmd(uint32_t aKey, uint64_t value)
{
    return sRadioSpinel.Set((spinel_prop_key_t)aKey, SPINEL_DATATYPE_UINT64_S, value);
}

otError otPlatRadioSendGetPropVendorUint64Cmd(uint32_t aKey, uint64_t *value)
{
    return sRadioSpinel.Get((spinel_prop_key_t)aKey, SPINEL_DATATYPE_UINT64_S, value);
}

otError otPlatRadioSendGetPropVendorCmd(uint32_t aKey, const char *value, uint8_t length)
{
    return sRadioSpinel.Get((spinel_prop_key_t)aKey, SPINEL_DATATYPE_UTF8_S, value, length);
}

#if OPENTHREAD_CONFIG_MLE_LINK_METRICS_SUBJECT_ENABLE
otError otPlatRadioConfigureEnhAckProbing(otInstance          *aInstance,
                                          otLinkMetrics        aLinkMetrics,
                                          const otShortAddress aShortAddress,
                                          const otExtAddress  *aExtAddress)
{
    OT_UNUSED_VARIABLE(aInstance);

    return sRadioSpinel.ConfigureEnhAckProbing(aLinkMetrics, aShortAddress, *aExtAddress);
}
#endif

#if defined(OT_PLAT_SPINEL_OVER_SPI)
void otPlatRadioSpiDiag(void)
{
    sRadioSpinel.GetSpinelInterface().DiagLogStats();
}
#endif
