/*
 *  Copyright (c) 2021, The OpenThread Authors.
 *  Copyright (c) 2022-2023, NXP.
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

#ifndef OT_NXP_SPINEL_HDLC_HPP_
#define OT_NXP_SPINEL_HDLC_HPP_

#include "fsl_os_abstraction.h"
#include "fwk_platform_hdlc.h"
#include "ot_platform_common.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "lib/hdlc/hdlc.hpp"
#include "lib/spinel/spinel.h"
#include "lib/spinel/spinel_interface.hpp"

namespace ot {

namespace NXP {

typedef uint8_t HdlcSpinelContext;

/**
 * This class defines an HDLC spinel interface to the Radio Co-processor (RCP).
 *
 */
class HdlcInterface
{
public:
    /**
     * This constructor initializes the object.
     *
     * @param[in] aCallback         Callback on frame received
     * @param[in] aCallbackContext  Callback context
     * @param[in] aFrameBuffer      A reference to a `RxFrameBuffer` object.
     *
     */
    HdlcInterface(ot::Spinel::SpinelInterface::ReceiveFrameCallback aCallback,
                  void                                             *aCallbackContext,
                  ot::Spinel::SpinelInterface::RxFrameBuffer       &aFrameBuffer);

    /**
     * This destructor deinitializes the object.
     *
     */
    virtual ~HdlcInterface(void);

    /**
     * This method initializes the HDLC interface.
     *
     */
    void Init(void);

    /**
     * This method deinitializes the HDLC interface.
     *
     */
    void Deinit(void);

    /**
     * This method performs radio driver processing.
     *
     * @param[in]  aInstance  The ot instance
     *
     */
    void Process(const void *aInstance);

    /**
     * This method encodes and sends a spinel frame to Radio Co-processor (RCP) over the socket.
     *
     * This is blocking call, i.e., if the socket is not writable, this method waits for it to become writable for
     * up to `kMaxWaitTime` interval.
     *
     * @param[in] aFrame     A pointer to buffer containing the spinel frame to send.
     * @param[in] aLength    The length (number of bytes) in the frame.
     *
     * @retval OT_ERROR_NONE     Successfully encoded and sent the spinel frame.
     * @retval OT_ERROR_NO_BUFS  Insufficient buffer space available to encode the frame.
     * @retval OT_ERROR_FAILED   Failed to send due to socket not becoming writable within `kMaxWaitTime`.
     *
     */
    otError SendFrame(const uint8_t *aFrame, uint16_t aLength);

    /**
     * This method waits for receiving part or all of spinel frame within specified timeout.
     *
     * @param[in]  aTimeoutUs  The timeout value in microseconds.
     *
     * @retval OT_ERROR_NONE             Part or all of spinel frame is received.
     * @retval OT_ERROR_RESPONSE_TIMEOUT No spinel frame is received within @p aTimeoutUs.
     *
     */
    otError WaitForFrame(uint64_t aTimeoutUs);

    /**
     * This method is called by the HDLC RX Callback when a HDLC message has been received
     *
     * It will decode and store the Spinel frames in a temporary Spinel frame buffer (mRxSpinelFrameBuffer)
     * This buffer will be then copied to the OpenThread Spinel frame buffer, from the OpenThread task context
     *
     * @param[in] data A pointer to buffer containing the HDLC message to decode.
     * @param[in] len  The length (number of bytes) in the message.
     */
    void ProcessRxData(uint8_t *data, uint16_t len);

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

private:
    enum
    {
        /* HDLC encoder buffer must be larger than the max spinel frame size to be able to handle the HDLC overhead
         * As a host, a TX frame will always contain only 1 spinel frame + HDLC overhead
         * Sizing the buffer for 2 spinel frames should be large enough to handle this */
        kEncoderBufferSize         = SPINEL_FRAME_MAX_SIZE * 2,
        kMaxMultiFrameSize         = 2048,
        kSpinelHdlcFrameReadyEvent = 1 << 0,
    };

    ot::Spinel::FrameBuffer<kEncoderBufferSize>       mEncoderBuffer;
    ot::Hdlc::Encoder                                 mHdlcEncoder;
    platform_hdlc_rx_callback_t                       mHdlcRxCallbackField;
    ot::Spinel::SpinelInterface::RxFrameBuffer       &mReceiveFrameBuffer;
    ot::Spinel::SpinelInterface::ReceiveFrameCallback mReceiveFrameCallback;
    void                                             *mReceiveFrameContext;
    ot::Spinel::MultiFrameBuffer<kMaxMultiFrameSize>  mRxSpinelFrameBuffer;
    ot::Hdlc::Decoder                                 mHdlcSpinelDecoder;
    bool                                              mIsInitialized;
    uint8_t                                          *mSavedFrame;
    uint16_t                                          mSavedFrameLen;
    bool                                              mIsSpinelBufferFull;
    SemaphoreHandle_t                                 mWriteMutexHandle;
    SemaphoreHandle_t                                 mReadMutexHandle;
    QueueHandle_t                                     mMsqQueue;
    EventGroupHandle_t                                mSpinelHdlcEventGroup;

    otError     Write(const uint8_t *aFrame, uint16_t aLength);
    uint32_t    TryReadAndDecode(bool fullRead);
    void        HandleHdlcFrame(otError aError);
    static void HandleHdlcFrame(void *aContext, otError aError);
    static void HdlcRxCallback(uint8_t *data, uint16_t len, void *param);

protected:
    virtual void HandleUnknownHdlcContent(uint8_t *buffer, uint16_t len);
};

} // namespace NXP

} // namespace ot

#endif // OT_NXP_SPINEL_HDLC_HPP_
