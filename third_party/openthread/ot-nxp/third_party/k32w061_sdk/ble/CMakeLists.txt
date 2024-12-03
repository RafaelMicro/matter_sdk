#
#  Copyright (c) 2022, The OpenThread Authors.
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

list(APPEND SDK_BLE_LIBS
    $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/ble_controller/lib/lib_ble_controller.a
    # $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/bluetooth/host/lib/lib_ble_5-0_fsci_bbox_cm4_noFP.a
)

add_library(nxp-k32w061-ble
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/bluetooth/application/common/ble_init.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/bluetooth/application/common/ble_host_tasks.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/bluetooth/application/common/ble_controller_task.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/ble_controller/config/controller_config.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/bluetooth/hci_transport/source/hcit_serial_interface.c
)

target_compile_definitions(nxp-k32w061-ble
    PUBLIC
        gSupportBle=1
        SUPPORT_FOR_BLE=1
)

target_include_directories(nxp-k32w061-ble
    PUBLIC
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/bluetooth/application/common
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/bluetooth/host/interface
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/bluetooth/hci_transport/interface
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/ble_controller/interface
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/ble_controller/config
)

target_link_libraries(nxp-k32w061-ble PUBLIC ${NXP_DRIVER_LIB} ${SDK_BLE_LIBS})
