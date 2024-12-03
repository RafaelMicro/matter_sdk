/*
 *    Copyright (c) 2022, The OpenThread Authors.
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *    3. Neither the name of the copyright holder nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 *    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file shows how to implement the NCP vendor hook.
 */

#if OPENTHREAD_ENABLE_NCP_VENDOR_HOOK

#include "ncp_base.hpp"

// Specific SPINEL NXP Commands / Properties definitions
//                     ---
// Independent Reset Properties range [0x100 - 0X110]
// IR CONFIG <uint8 mode> : Configure IR Mode (O:Disable 1:OutOfBand 2:InBand)
#define SPINEL_PROP_VENDOR_NXP_IR_CONFIG (SPINEL_PROP_VENDOR__BEGIN + 0x100)
// IR CMD : Execute Independent Reset without noticed (no arg)
#define SPINEL_PROP_VENDOR_NXP_IR_CMD (SPINEL_PROP_VENDOR__BEGIN + 0x101)

// SET IEEE.802.15.4 MAC Address <uint64> : Set MAC address
#define SPINEL_PROP_VENDOR_NXP_SET_EUI64_CMD (SPINEL_PROP_VENDOR__BEGIN + 0x10A)
// SET / GET TX Power Limit of 15.4 Transmissions
#define SPINEL_PROP_VENDOR_NXP_GET_SET_TXPOWERLIMIT_CMD (SPINEL_PROP_VENDOR__BEGIN + 0x10B)
// SET / GET CCA Configuration used for 15.4 Transmissions
#define SPINEL_PROP_VENDOR_NXP_GET_SET_CCA_CONFIGURE_CMD (SPINEL_PROP_VENDOR__BEGIN + 0x10C)
// GET firmware version used for 15.4
#define SPINEL_PROP_VENDOR_NXP_GET_FW_VERSION_CMD (SPINEL_PROP_VENDOR__BEGIN + 0x10D)
// GET/SET RCP Frequency
#define SPINEL_PROP_VENDOR_NXP_GET_SET_RCP_FREQUENCY_CMD (SPINEL_PROP_VENDOR__BEGIN + 0x10E)

// Manufacturing Properties range [0x3F0 - 0x3FF]
#define SPINEL_CMD_VENDOR_NXP_MFG (SPINEL_CMD_VENDOR__BEGIN + 0x3FF)

// Max version string size
#define MAX_VERSION_STRING_SIZE 128
namespace ot {
namespace Ncp {

class NcpBaseVendor : public ot::Ncp::NcpBase
{
public:
    otError VendorCommandResponse(uint8_t aHeader, uint8_t *payload, uint16_t len);
};

extern "C" {
otError otIwx12IRConfig(uint8_t mode);
otError otIwx12IRCommand(void);
otError otIwx12SetEui64(uint8_t *aIeeeEui64);
otError otIwx12SetTxPowerLimit(uint8_t txPowerLimit);
otError otIwx12GetTxPowerLimit(uint8_t *txPowerLimit);
otError otIwx12GetSetCcaConfig(uint8_t *mCcaMode,
                               uint8_t *mCca1Threshold,
                               uint8_t *mCca2CorrThreshold,
                               uint8_t *mCca2MinNumOfCorrTh,
                               uint8_t  aSetValue);
otError otIwx12GetFwVersion(char *fwVersionBuf);
otError otIwx12GetSetSpiFreq(uint32_t *spiMaxFreq, uint8_t aSetValue);

int MfgVendorCommandDispatch(uint8_t aHeader, uint8_t *payload, uint16_t len);

void MfgVendorCommandResponse(uint8_t aHeader, uint8_t *payload, uint16_t len)
{
    NcpBaseVendor *ncpVendor = static_cast<NcpBaseVendor *>(NcpBase::GetNcpInstance());
    ncpVendor->VendorCommandResponse(aHeader, payload, len);
}
}

otError NcpBaseVendor::VendorCommandResponse(uint8_t aHeader, uint8_t *payload, uint16_t len)
{
    otError error = OT_ERROR_NONE;

    SuccessOrExit(error = mEncoder.BeginFrame(aHeader, SPINEL_CMD_VENDOR_NXP_MFG));
    SuccessOrExit(error = mEncoder.WriteData(reinterpret_cast<const uint8_t *>(payload), len));
    SuccessOrExit(error = mEncoder.EndFrame());

exit:
    return error;
}

otError NcpBase::VendorCommandHandler(uint8_t aHeader, unsigned int aCommand)
{
    otError error = OT_ERROR_NONE;

    switch (aCommand)
    {
    case SPINEL_CMD_VENDOR_NXP_MFG:

        const uint8_t *payload;
        uint16_t       payloadLen;

        SuccessOrExit(error = mDecoder.ReadData(payload, payloadLen));

        if (MfgVendorCommandDispatch(aHeader, (uint8_t *)payload, payloadLen) == 0)
        {
            SuccessOrExit(error = mEncoder.BeginFrame(aHeader, SPINEL_CMD_VENDOR_NXP_MFG));
            SuccessOrExit(error = mEncoder.WriteData(reinterpret_cast<const uint8_t *>(payload), payloadLen));
            SuccessOrExit(error = mEncoder.EndFrame());
        }

        break;

    default:
        error = PrepareLastStatusResponse(aHeader, SPINEL_STATUS_INVALID_COMMAND);
    }
exit:
    return error;
}

void NcpBase::VendorHandleFrameRemovedFromNcpBuffer(Spinel::Buffer::FrameTag aFrameTag)
{
    // This method is a callback which mirrors `NcpBase::HandleFrameRemovedFromNcpBuffer()`.
    // It is called when a spinel frame is sent and removed from NCP buffer.
    //
    // (a) This can be used to track and verify that a vendor spinel frame response is
    //     delivered to the host (tracking the frame using its tag).
    //
    // (b) It indicates that NCP buffer space is now available (since a spinel frame is
    //     removed). This can be used to implement reliability mechanisms to re-send
    //     a failed spinel command response (or an async spinel frame) transmission
    //     (failed earlier due to NCP buffer being full).

    OT_UNUSED_VARIABLE(aFrameTag);
}

otError NcpBase::VendorGetPropertyHandler(spinel_prop_key_t aPropKey)
{
    otError error = OT_ERROR_NONE;

    switch (aPropKey)
    {
        // TODO: Implement your property get handlers here.
        //
        // Get handler should retrieve the property value and then encode and write the
        // value into the NCP buffer. If the "get" operation itself fails, handler should
        // write a `LAST_STATUS` with the error status into the NCP buffer. `OT_ERROR_NO_BUFS`
        // should be returned if NCP buffer is full and response cannot be written.

    case SPINEL_PROP_VENDOR_NXP_GET_FW_VERSION_CMD:
    {
        char fwVersion[MAX_VERSION_STRING_SIZE];

        SuccessOrExit(error = otIwx12GetFwVersion(fwVersion));
        SuccessOrExit(error = mEncoder.WriteUtf8(fwVersion));
    }
    break;

    case SPINEL_PROP_VENDOR_NXP_GET_SET_RCP_FREQUENCY_CMD:
    {
        uint32_t spiFreq = 0;

        SuccessOrExit(error = otIwx12GetSetSpiFreq(&spiFreq, 0));
        SuccessOrExit(error = mEncoder.WriteUint32(spiFreq));
    }
    break;

    case SPINEL_CMD_VENDOR_NXP_MFG:

        const uint8_t *payload;
        uint16_t       payloadLen;

        SuccessOrExit(error = mDecoder.ReadData(payload, payloadLen));

        if (MfgVendorCommandDispatch(0, (uint8_t *)payload, payloadLen) == 0)
        {
            SuccessOrExit(error = mEncoder.WriteData(reinterpret_cast<const uint8_t *>(payload), payloadLen));
        }

        break;

    case SPINEL_PROP_VENDOR_NXP_GET_SET_TXPOWERLIMIT_CMD:
    {
        uint8_t txPowerLimit = 0;

        SuccessOrExit(error = otIwx12GetTxPowerLimit(&txPowerLimit));
        SuccessOrExit(error = mEncoder.WriteUint8(txPowerLimit));
    }
    break;

    case SPINEL_PROP_VENDOR_NXP_GET_SET_CCA_CONFIGURE_CMD:
    {
        otCCAModeConfig aCcaConfig;
        SuccessOrExit(error =
                          otIwx12GetSetCcaConfig(&aCcaConfig.mCcaMode, &aCcaConfig.mCca1Threshold,
                                                 &aCcaConfig.mCca2CorrThreshold, &aCcaConfig.mCca2MinNumOfCorrTh, 0));
        SuccessOrExit(error = mEncoder.OpenStruct());
        {
            SuccessOrExit(error = mEncoder.WriteUint8(aCcaConfig.mCcaMode));
            SuccessOrExit(error = mEncoder.WriteUint8(aCcaConfig.mCca1Threshold));
            SuccessOrExit(error = mEncoder.WriteUint8(aCcaConfig.mCca2CorrThreshold));
            SuccessOrExit(error = mEncoder.WriteUint8(aCcaConfig.mCca2MinNumOfCorrTh));
        }
        SuccessOrExit(error = mEncoder.CloseStruct());
    }
    break;

    default:
        error = OT_ERROR_NOT_FOUND;
        break;
    }

exit:
    return error;
}

otError NcpBase::VendorSetPropertyHandler(spinel_prop_key_t aPropKey)
{
    otError error = OT_ERROR_NONE;

    switch (aPropKey)
    {
        // Implement your property set handlers here.
        //
        // Set handler should first decode the value from the input Spinel frame and then
        // perform the corresponding set operation. The handler should not prepare the
        // spinel response and therefore should not write anything to the NCP buffer.
        // The error returned from handler (other than `OT_ERROR_NOT_FOUND`) indicates the
        // error in either parsing of the input or the error of the set operation. In case
        // of a successful "set", `NcpBase` set command handler will invoke the
        // `VendorGetPropertyHandler()` for the same property key to prepare the response.
    case SPINEL_PROP_VENDOR_NXP_IR_CONFIG:
    {
        uint8_t mode = 0;

        SuccessOrExit(error = mDecoder.ReadUint8(mode));
        SuccessOrExit(error = otIwx12IRConfig(mode));
    }
    break;

    case SPINEL_PROP_VENDOR_NXP_IR_CMD:
    {
        SuccessOrExit(error = otIwx12IRCommand());
    }
    break;

    case SPINEL_PROP_VENDOR_NXP_SET_EUI64_CMD:
    {
        otExtAddress addr;
        uint64_t     u64;

        SuccessOrExit(error = mDecoder.ReadUint64(u64));
        ot::Encoding::BigEndian::WriteUint64(u64, addr.m8);
        SuccessOrExit(error = otIwx12SetEui64(addr.m8));
    }
    break;

    case SPINEL_PROP_VENDOR_NXP_GET_SET_RCP_FREQUENCY_CMD:
    {
        uint32_t spiFreq = 0;

        SuccessOrExit(error = mDecoder.ReadUint32(spiFreq));
        SuccessOrExit(error = otIwx12GetSetSpiFreq(&spiFreq, 1));
    }
    break;

    case SPINEL_PROP_VENDOR_NXP_GET_SET_TXPOWERLIMIT_CMD:
    {
        uint8_t txPowerLimit;

        SuccessOrExit(error = mDecoder.ReadUint8(txPowerLimit));
        SuccessOrExit(error = otIwx12SetTxPowerLimit(txPowerLimit));
    }
    break;

    case SPINEL_PROP_VENDOR_NXP_GET_SET_CCA_CONFIGURE_CMD:
    {
        otCCAModeConfig aCcaConfig;
        SuccessOrExit(error = mDecoder.OpenStruct());
        {
            SuccessOrExit(error = mDecoder.ReadUint8(aCcaConfig.mCcaMode));
            SuccessOrExit(error = mDecoder.ReadUint8(aCcaConfig.mCca1Threshold));
            SuccessOrExit(error = mDecoder.ReadUint8(aCcaConfig.mCca2CorrThreshold));
            SuccessOrExit(error = mDecoder.ReadUint8(aCcaConfig.mCca2MinNumOfCorrTh));
        }
        SuccessOrExit(error = mDecoder.CloseStruct());
        SuccessOrExit(error =
                          otIwx12GetSetCcaConfig(&aCcaConfig.mCcaMode, &aCcaConfig.mCca1Threshold,
                                                 &aCcaConfig.mCca2CorrThreshold, &aCcaConfig.mCca2MinNumOfCorrTh, 1));
    }
    break;

    default:
        error = OT_ERROR_NOT_FOUND;
        break;
    }
exit:
    return error;
}

} // namespace Ncp
} // namespace ot

//-------------------------------------------------------------------------------------------------------------------
// When OPENTHREAD_ENABLE_NCP_VENDOR_HOOK is enabled, vendor code is
// expected to provide the `otAppNcpInit()` function. The reason behind
// this is to enable vendor code to define its own sub-class of
// `NcpBase` or `NcpHdlc`/`NcpSpi`.
//
// Example below show how to add a vendor sub-class over `NcpHdlc`.
#if OPENTHREAD_CONFIG_NCP_SPI_ENABLE
#include "ncp_spi.hpp"
#include "common/new.hpp"

namespace ot {
namespace Ncp {

static OT_DEFINE_ALIGNED_VAR(sNcpRaw, sizeof(NcpSpi), uint64_t);

extern "C" void otNcpSpiInit(otInstance *aInstance)
{
    NcpSpi   *ncpSpi   = nullptr;
    Instance *instance = static_cast<Instance *>(aInstance);

    ncpSpi = new (&sNcpRaw) NcpSpi(instance);

    if (ncpSpi == nullptr || ncpSpi != NcpBase::GetNcpInstance())
    {
        OT_ASSERT(false);
    }
}

} // namespace Ncp
} // namespace ot
#else

#include "ncp_hdlc.hpp"
#include "common/new.hpp"

namespace ot {
namespace Ncp {

static OT_DEFINE_ALIGNED_VAR(sNcpRawHdlc, sizeof(NcpHdlc), uint64_t);

extern "C" void otNcpHdlcInit(otInstance *aInstance, otNcpHdlcSendCallback aSendCallback)
{
    NcpHdlc  *ncpHdlc  = nullptr;
    Instance *instance = static_cast<Instance *>(aInstance);

    ncpHdlc = new (&sNcpRawHdlc) NcpHdlc(instance, aSendCallback);

    if (ncpHdlc == nullptr || ncpHdlc != NcpBase::GetNcpInstance())
    {
        OT_ASSERT(false);
    }
}

} // namespace Ncp
} // namespace ot
#endif // #if OPENTHREAD_CONFIG_NCP_SPI_ENABLE
#endif // #if OPENTHREAD_ENABLE_NCP_VENDOR_HOOK
