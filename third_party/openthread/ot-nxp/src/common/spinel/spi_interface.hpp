/*
 *  Copyright (c) 2021, The OpenThread Authors.
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
 *   This file includes definitions for the SPI interface to radio (RCP).
 */

#ifndef OT_NXP_SPI_INTERFACE_HPP_
#define OT_NXP_SPI_INTERFACE_HPP_

#include "fsl_adapter_gpio.h"
#include "fsl_adapter_spi.h"
#include "fsl_common.h"
#include "ot_platform_common.h"
#include "lib/spinel/spinel_interface.hpp"

#include "ncp/ncp_spi.hpp"

namespace ot {
namespace NXP {

/**
 * This class defines an SPI interface to the Radio Co-processor (RCP).
 *
 */
class SpiInterface
{
public:
    /**
     * This constructor initializes the object.
     *
     * @param[in] aCallback         A reference to a `Callback` object.
     * @param[in] aCallbackContext  The context pointer passed to the callback.
     * @param[in] aFrameBuffer      A reference to a `RxFrameBuffer` object.
     *
     */
    SpiInterface(Spinel::SpinelInterface::ReceiveFrameCallback aCallback,
                 void                                         *aCallbackContext,
                 Spinel::SpinelInterface::RxFrameBuffer       &aFrameBuffer);

    /**
     * This destructor deinitializes the object.
     *
     */
    ~SpiInterface(void);

    /**
     * This method initializes the interface to the Radio Co-processor (RCP).
     *
     */
    void Init(void);

    /**
     * This method deinitializes the interface to the RCP.
     *
     */
    void Deinit(void);

    /**
     * This method encodes and sends a spinel frame to Radio Co-processor (RCP) over the socket.
     *
     * @param[in] aFrame     A pointer to buffer containing the spinel frame to send.
     * @param[in] aLength    The length (number of bytes) in the frame.
     *
     * @retval OT_ERROR_NONE     Successfully encoded and sent the spinel frame.
     * @retval OT_ERROR_BUSY     Failed due to another operation is on going.
     * @retval OT_ERROR_NO_BUFS  Insufficient buffer space available to encode the frame.
     * @retval OT_ERROR_FAILED   Failed to call the SPI driver to send the frame.
     *
     */
    otError SendFrame(const uint8_t *aFrame, uint16_t aLength);

    /**
     * This method waits for receiving part or all of spinel frame within specified interval.
     *
     * @param[in]  aTimeout  The timeout value in microseconds.
     *
     * @retval OT_ERROR_NONE             Part or all of spinel frame is received.
     * @retval OT_ERROR_RESPONSE_TIMEOUT No spinel frame is received within @p aTimeout.
     *
     */
    otError WaitForFrame(uint64_t aTimeoutUs);

    /**
     * This method performs radio driver processing.
     *
     * @param[in]  aInstance  The ot instance
     *
     */
    void Process(const void *aInstance);

    /**
     * This method is called when RCP failure detected and resets internal states of the interface.
     *
     */
    void OnRcpReset(void);

    /**
     * This method is called when RCP is reset to recreate the connection with it.
     * Intentionally empty.
     *
     */
    otError ResetConnection(void) { return OT_ERROR_NONE; }

    /**
     * This method hardware resets the RCP.
     *
     * @retval OT_ERROR_NONE             Successfully reset the RCP.
     * @retval OT_ERROR_NOT_IMPLEMENTED  The hardware reset is not implemented.
     *
     */
    otError HardwareReset(void) { return OT_ERROR_NOT_IMPLEMENTED; }

    /**
     * This method increases the value of pendingSpiRxDataValue
     *
     */
    void IncreasePendingSpiRxDataCounter(void);

    /**
     * This method displays SPI diagnostic statistics on OT CLI.
     *
     */
    void DiagLogStats(void);

    hal_gpio_status_t status;

private:
    enum
    {
        kSpiAlignAllowanceMax = 16,
        kSpiFrameHeaderSize   = 5,
    };

    enum
    {
        kMaxFrameSize = Spinel::SpinelInterface::kMaxFrameSize,
    };

    Spinel::SpinelInterface::ReceiveFrameCallback mReceiveFrameCallback;
    void                                         *mReceiveFrameContext;
    Spinel::SpinelInterface::RxFrameBuffer       &mRxFrameBuffer;

    uint8_t  mSpiAlignAllowance;
    uint32_t mResetDelay;
    uint16_t mSpiSmallPacketSize;

    uint64_t mSlaveResetCount;
    uint64_t mSpiFrameCount;
    uint64_t mSpiValidFrameCount;
    uint64_t mSpiGarbageFrameCount;
    uint64_t mSpiDuplexFrameCount;
    uint64_t mSpiUnresponsiveFrameCount;
    uint64_t mSpiRxFrameCount;
    uint64_t mSpiRxFrameByteCount;
    uint64_t mSpiTxFrameCount;
    uint64_t mSpiTxFrameByteCount;
    uint64_t mSpiRxDiscard;
    uint64_t mSpiRxFrameLargerCount;

    bool     mSpiTxIsReady;
    uint16_t mSpiTxRefusedCount;
    uint16_t mSpiTxPayloadSize;
    uint8_t  mSpiTxFrameBuffer[kMaxFrameSize + kSpiAlignAllowanceMax];

    uint16_t mSpiSlaveDataLen;
    uint16_t mSpiSlaveAcceptLen;

    uint8_t resetSpinelCmdResponseReceived;
    bool    isInitialized;

    HAL_SPI_MASTER_HANDLE_DEFINE(otSpiMasterHandle);
    GPIO_HANDLE_DEFINE(otGpioSpiIntHandle);
    volatile uint16_t pendingSpiRxDataCounter;

    void InitSpi(void);
    void DeinitSpi(void);

    void LogStats(void);

    uint8_t *GetRealRxFrameStart(uint8_t *aSpiRxFrameBuffer, uint8_t aAlignAllowance, uint16_t &aSkipLength);
    otError  PushPullSpi(void);
    otError  DoSpiTransfer(uint8_t *aSpiRxFrameBuffer, uint32_t aTransferLength);

    // Non-copyable, intentionally not implemented.
    SpiInterface(const SpiInterface &);
    SpiInterface &operator=(const SpiInterface &);
};

} // namespace NXP
} // namespace ot

#endif // OT_NXP_SPI_INTERFACE_HPP_
