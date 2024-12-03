/*
 *  Copyright (c) 2022, The OpenThread Authors.
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

#ifndef OT_NXP_SPINEL_HCI_HDLC_HPP_
#define OT_NXP_SPINEL_HCI_HDLC_HPP_

#include "spinel_hdlc.hpp"
#include "lib/hdlc/hdlc.hpp"

namespace ot {

namespace NXP {

/**
 * This class defines an HDLC interface to the Radio Co-processor (RCP).
 * Supporting spinel and hci
 */
class HdlcSpinelHciInterface : public HdlcInterface
{
public:
    HdlcSpinelHciInterface(ot::Spinel::SpinelInterface::ReceiveFrameCallback aCallback,
                           void                                             *aCallbackContext,
                           ot::Spinel::SpinelInterface::RxFrameBuffer       &aFrameBuffer)
        : HdlcInterface(aCallback, aCallbackContext, aFrameBuffer)
    {
    }

private:
    void HandleUnknownHdlcContent(uint8_t *buffer, uint16_t len);
};

} // namespace NXP

} // namespace ot

#endif // OT_NXP_SPINEL_HCI_HDLC_HPP_
