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

#include "spinel_hdlc.hpp"
#include "board.h"
#include "fwk_platform_hdlc.h"

#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"
#include "semphr.h"

#include <openthread/tasklet.h>
#include <openthread/platform/alarm-milli.h>
#include "common/logging.hpp"

namespace ot {

namespace NXP {

HdlcInterface::HdlcInterface(ot::Spinel::SpinelInterface::ReceiveFrameCallback aCallback,
                             void                                             *aCallbackContext,
                             ot::Spinel::SpinelInterface::RxFrameBuffer       &aFrameBuffer)
    : mEncoderBuffer()
    , mHdlcEncoder(mEncoderBuffer)
    , mHdlcRxCallbackField(nullptr)
    , mReceiveFrameBuffer(aFrameBuffer)
    , mReceiveFrameCallback(aCallback)
    , mReceiveFrameContext(aCallbackContext)
    , mHdlcSpinelDecoder(mRxSpinelFrameBuffer, HandleHdlcFrame, this)
    , mIsInitialized(false)
    , mSavedFrame(nullptr)
    , mSavedFrameLen(0)
    , mIsSpinelBufferFull(false)
{
    mHdlcRxCallbackField = HdlcRxCallback;
}

HdlcInterface::~HdlcInterface(void)
{
}

void HdlcInterface::Init(void)
{
    int status;

    if (!mIsInitialized)
    {
        mWriteMutexHandle     = xSemaphoreCreateMutex();
        mReadMutexHandle      = xSemaphoreCreateMutex();
        mMsqQueue             = xQueueCreate(1, sizeof(uint8_t));
        mSpinelHdlcEventGroup = xEventGroupCreate();

        assert((mWriteMutexHandle != NULL) && (mReadMutexHandle != NULL) && (mMsqQueue != NULL) &&
               (mSpinelHdlcEventGroup != NULL));

        /* Initialize the HDLC interface */
        status = PLATFORM_InitHdlcInterface(mHdlcRxCallbackField, this);
        if (status == 0)
        {
            mIsInitialized = true;
        }
        else
        {
            otLogDebgPlat("Failed to initialize HDLC interface %d", status);
        }
    }
}

void HdlcInterface::Deinit(void)
{
    int status;

    status = PLATFORM_TerminateHdlcInterface();
    if (status == 0)
    {
        mIsInitialized = false;
    }
    else
    {
        otLogDebgPlat("Failed to terminate HDLC interface %d", status);
    }
}

void HdlcInterface::Process(const void *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    TryReadAndDecode(false);
}

otError HdlcInterface::SendFrame(const uint8_t *aFrame, uint16_t aLength)
{
    otError error = OT_ERROR_NONE;

    /* Protect concurrent Send operation to avoid any buffer corruption */
    if (xSemaphoreTake(mWriteMutexHandle, portMAX_DELAY) != pdTRUE)
    {
        error = OT_ERROR_FAILED;
        goto exit;
    }

    assert(mEncoderBuffer.IsEmpty());

    SuccessOrExit(error = mHdlcEncoder.BeginFrame());
    SuccessOrExit(error = mHdlcEncoder.Encode(aFrame, aLength));
    SuccessOrExit(error = mHdlcEncoder.EndFrame());
    otLogDebgPlat("frame len to send = %d/%d", mEncoderBuffer.GetLength(), aLength);
    SuccessOrExit(error = Write(mEncoderBuffer.GetFrame(), mEncoderBuffer.GetLength()));

exit:
    if (xSemaphoreGive(mWriteMutexHandle) != pdTRUE)
    {
        error = OT_ERROR_FAILED;
    }
    if (error != OT_ERROR_NONE)
    {
        OT_PLAT_ERR("error = 0x%x", error);
    }

    return error;
}

otError HdlcInterface::WaitForFrame(uint64_t aTimeoutUs)
{
    otError     error        = OT_ERROR_RESPONSE_TIMEOUT;
    TickType_t  timeoutTicks = ((uint32_t)(aTimeoutUs / 1000U)) / portTICK_PERIOD_MS;
    EventBits_t eventBits;

    do
    {
        /* Wait for kSpinelHdlcFrameReadyEvent indicating a frame has been received */
        eventBits = xEventGroupWaitBits(mSpinelHdlcEventGroup, HdlcInterface::kSpinelHdlcFrameReadyEvent, pdTRUE,
                                        pdFALSE, timeoutTicks);

        if ((eventBits & HdlcInterface::kSpinelHdlcFrameReadyEvent) != 0)
        {
            /* Event is set, it means a frame has been received and can be decoded
             * Note: The event is set whenever a frame is received, even when ot task is not blocked in WaitForFrame
             * it means the event could have been set for a previous frame
             * If TryReadAndDecode returns 0, it means the event was set for a previous frame, so we loop and wait again
             * If TryReadAndDecode returns anything else, it means there's real data to process, so we can exit from
             * WaitForFrame */
            if (TryReadAndDecode(true) != 0)
            {
                error = OT_ERROR_NONE;
                break;
            }
        }
        else
        {
            /* The event wasn't set within the timeout range, we exit with a timeout error */
            otLogDebgPlat("WaitForFrame timeout");
            break;
        }
    } while (true);

    return error;
}

void HdlcInterface::ProcessRxData(uint8_t *data, uint16_t len)
{
    uint8_t  event;
    uint32_t remainingRxBufferSize = 0;

    if (xSemaphoreTake(mReadMutexHandle, portMAX_DELAY) != pdTRUE)
    {
        assert(0);
    }

    do
    {
        /* Check if we have enough space to store the frame in the buffer */
        remainingRxBufferSize = mRxSpinelFrameBuffer.GetFrameMaxLength() - mRxSpinelFrameBuffer.GetLength();
        otLogDebgPlat("remainingRxBufferSize = %lu", remainingRxBufferSize);

        if (remainingRxBufferSize >= len)
        {
            // otDumpDebgPlat("Serial", data, len);
            mHdlcSpinelDecoder.Decode(data, len);
            break;
        }
        else
        {
            mIsSpinelBufferFull = true;
            otLogDebgPlat("Spinel buffer full remainingRxLen = %lu", remainingRxBufferSize);
            /* Send a signal to the openthread task to indicate to empty the spinel buffer */
            otTaskletsSignalPending(NULL);
            /* Give the mutex */
            if (xSemaphoreGive(mReadMutexHandle) != pdTRUE)
            {
                assert(0);
            }
            /* Lock the task here until the spinel buffer becomes empty */
            xQueueReceive(mMsqQueue, &event, portMAX_DELAY);
            /* take the mutex again */
            if (xSemaphoreTake(mReadMutexHandle, portMAX_DELAY) != pdTRUE)
            {
                assert(0);
            }
        }
    } while (true);

    if (xSemaphoreGive(mReadMutexHandle) != pdTRUE)
    {
        assert(0);
    }
}

otError HdlcInterface::Write(const uint8_t *aFrame, uint16_t aLength)
{
    otError otResult = OT_ERROR_NONE;
    int     ret;

    otLogDebgPlat("Send tx frame len = %d", aLength);

    ret = PLATFORM_SendHdlcMessage((uint8_t *)aFrame, aLength);
    if (ret != 0)
    {
        otResult = OT_ERROR_FAILED;
        otLogCritPlat("Error send %d", ret);
    }

    /* Always clear the encoder */
    mEncoderBuffer.Clear();
    return otResult;
}

uint32_t HdlcInterface::TryReadAndDecode(bool fullRead)
{
    uint32_t totalBytesRead  = 0;
    uint32_t i               = 0;
    uint8_t  event           = 1;
    uint8_t *oldFrame        = mSavedFrame;
    uint16_t oldLen          = mSavedFrameLen;
    otError  savedFrameFound = OT_ERROR_NONE;

    (void)fullRead;

    if (xSemaphoreTake(mReadMutexHandle, portMAX_DELAY) != pdTRUE)
    {
        assert(0);
    }

    savedFrameFound = mRxSpinelFrameBuffer.GetNextSavedFrame(mSavedFrame, mSavedFrameLen);

    while (savedFrameFound == OT_ERROR_NONE)
    {
        /* Copy the data to the ot frame buffer */
        for (i = 0; i < mSavedFrameLen; i++)
        {
            if (mReceiveFrameBuffer.WriteByte(mSavedFrame[i]) != OT_ERROR_NONE)
            {
                mReceiveFrameBuffer.UndoLastWrites(i);
                /* No more space restore the mSavedFrame to the previous frame */
                mSavedFrame    = oldFrame;
                mSavedFrameLen = oldLen;
                /* Signal the ot task to re-try later */
                otTaskletsSignalPending(NULL);
                otLogDebgPlat("No more space");
                if (xSemaphoreGive(mReadMutexHandle) != pdTRUE)
                {
                    assert(0);
                }
                return totalBytesRead;
            }
            totalBytesRead++;
        }
        otLogDebgPlat("Frame len %d consumed", mSavedFrameLen);
        mReceiveFrameCallback(mReceiveFrameContext);
        oldFrame        = mSavedFrame;
        oldLen          = mSavedFrameLen;
        savedFrameFound = mRxSpinelFrameBuffer.GetNextSavedFrame(mSavedFrame, mSavedFrameLen);
    }

    if (savedFrameFound != OT_ERROR_NONE)
    {
        /* No more frame saved clear the buffer */
        mRxSpinelFrameBuffer.ClearSavedFrames();
        /* If the spinel queue was locked */
        if (mIsSpinelBufferFull)
        {
            mIsSpinelBufferFull = false;
            /* Send an event to unlock the task */
            if (xQueueOverwrite(mMsqQueue, (void *)&event) != pdTRUE)
            {
                assert(0);
            }
        }
    }

    if (xSemaphoreGive(mReadMutexHandle) != pdTRUE)
    {
        assert(0);
    }
    return totalBytesRead;
}

void HdlcInterface::HandleHdlcFrame(void *aContext, otError aError)
{
    static_cast<HdlcInterface *>(aContext)->HandleHdlcFrame(aError);
}

void HdlcInterface::HandleHdlcFrame(otError aError)
{
    uint8_t *buf       = mRxSpinelFrameBuffer.GetFrame();
    uint16_t bufLength = mRxSpinelFrameBuffer.GetLength();

    otDumpDebgPlat("RX FRAME", buf, bufLength);

    if (aError == OT_ERROR_NONE && bufLength > 0)
    {
        if ((buf[0] & SPINEL_HEADER_FLAG) == SPINEL_HEADER_FLAG)
        {
            otLogDebgPlat("Frame correctly received %d", bufLength);
            /* Save the frame */
            mRxSpinelFrameBuffer.SaveFrame();
            /* Send a signal to the openthread task to indicate that a spinel data is pending */
            otTaskletsSignalPending(NULL);
            /* Notify WaitForFrame that a frame is ready */
            (void)xEventGroupSetBits(mSpinelHdlcEventGroup, HdlcInterface::kSpinelHdlcFrameReadyEvent);
        }
        else
        {
            /* Give a chance to a child class to process this HDLC content
             * The current class treats it as an error case because it's supposed to receive only Spinel frames
             * If there's a need to share a same transport interface with another protocol, a child class must
             * override this method */
            HandleUnknownHdlcContent(buf, bufLength);
            /* Not a Spinel frame, discard */
            mRxSpinelFrameBuffer.DiscardFrame();
        }
    }
    else
    {
        otLogCritPlat("Frame will be discarded error = 0x%x", aError);
        mRxSpinelFrameBuffer.DiscardFrame();
    }
}

void HdlcInterface::HdlcRxCallback(uint8_t *data, uint16_t len, void *param)
{
    static_cast<HdlcInterface *>(param)->ProcessRxData(data, len);
}

void HdlcInterface::HandleUnknownHdlcContent(uint8_t *buffer, uint16_t len)
{
    (void)buffer;
    (void)len;
    otLogCritPlat("Unsupported HDLC content received (not Spinel)");
    assert(0);
}

void HdlcInterface::OnRcpReset(void)
{
    mHdlcSpinelDecoder.Reset();
}

} // namespace NXP

} // namespace ot
