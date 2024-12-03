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

#include "spinel_hci_hdlc.hpp"
#include "fsl_common.h"
#include <openthread/tasklet.h>
#include "common/logging.hpp"
#include "lib/spinel/spinel.h"

/*
 *   This class allows to support hci and spinel on a single UART.
 *   For that the HDLC-Lite framing protocol is used to transfert spinel and hci frames
 *   In addition, HCI and spinel frames can be distinguished by using the Spinel convention which is
 *   line compatible with BTBLE HCI. The first two bit of the spinel header are checked (HCI frames, which always start
 *   with either "0x01"or "0x04").
 *
 */

/*********
 * NXP hook to support HCI/SPINEL over HDLC on a single UART
 *********/

extern void         otPlatRadioSendFrameToSpinelInterface(uint8_t *buf, uint16_t length);
extern "C" uint16_t hci_transport_enqueue(uint8_t *packet, uint16_t packet_len, uint16_t *enqueued_len);

/*
 * Allows to wrap the function hci_uart_write_data.
 * In this case the hci frame will be encapsulated in a hdlc frame (a call to otPlatRadioSendFrameToSpinelInterface
 * allows to do it).
 */
extern "C" void __wrap_hci_uart_write_data(uint8_t *buf, uint16_t length)
{
    otPlatRadioSendFrameToSpinelInterface(buf, length);
}

/*
 * Wrap the function hci_uart_bt_init with an empty content.
 * In this case we assume that spinel is in charge of managing the UART interface
 */
extern "C" void __wrap_hci_uart_bt_init(void)
{
}

/*
 * Wrap the function hci_uart_bt_shutdown with an empty content.
 * In this case we assume that spinel is in charge of managing the UART interface
 */
extern "C" void __wrap_hci_uart_bt_shutdown(void)
{
}

/*
 * Wrap the function hci_uart_init, the goal of the new implemetation is to set the value of hci_uart_state to
 * "initilized", so that then a call to hci_transport_enqueue can be done.
 *
 */
extern "C" uint8_t hci_uart_state;
extern "C" void    __wrap_hci_uart_init(void)
{
    hci_uart_state = 0x1U;
}

/*********
 * End - NXP hook to support HCI/SPINEL over HDLC on a single UART
 *********/

namespace ot {

namespace NXP {

void HdlcSpinelHciInterface::HandleUnknownHdlcContent(uint8_t *buffer, uint16_t len)
{
    uint16_t enqueued_len = 0;

    /* Transfert the frame to the HCI decoder */
    hci_transport_enqueue(buffer, len, &enqueued_len);
    assert(enqueued_len == len);
}

} // namespace NXP

} // namespace ot
