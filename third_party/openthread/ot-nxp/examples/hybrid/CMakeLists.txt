#
#  Copyright (c) 2022-2023, The OpenThread Authors.
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  3. Neither the name of the copyright holder nor the
#     names of its contributors may be used to endorse or promote products
#     derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#

if(OT_APP_RCP_BLE_HCI_BB)
    add_subdirectory(ot_rcp_ble_hci_bb)
endif()

if(OT_APP_FTD_ZB_ED_RX_OFF)
    add_subdirectory(ot_cli_ftd_zb_ed_rx_off)
endif()

if(OT_APP_RCP_UART_FC_ZB_ED_RX_OFF)
    add_subdirectory(ot_rcp_uart_fc_zb_ed_rx_off)
endif()

if(OT_APP_RCP_UART_FC_BLE_HCI_BB_ZB_ED_RX_OFF)
    add_subdirectory(ot_rcp_uart_fc_ble_hci_bb_zb_ed_rx_off)
endif()

if(OT_APP_FTD_ZC)
    add_subdirectory(ot_cli_ftd_zc)
endif()

if(OT_APP_RCP_UART_DMA_ZB_COPROCESSOR)
    add_subdirectory(ot_rcp_uart_dma_zb_coprocessor)
endif()
