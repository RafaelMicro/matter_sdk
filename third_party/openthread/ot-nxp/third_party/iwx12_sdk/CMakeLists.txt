#
#  Copyright (c) 2021, The OpenThread Authors.
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

set(TOP_DIR $ENV{NXP_IWX12_SDK_ROOT}/src)
set(SDK_DIR ${TOP_DIR}/ffu/prebuilt/ot_sdk/mcu-sdk-2.0)
set(APP_ROOT ${TOP_DIR}/ffu/openthread)

add_library(${NXP_DRIVER_LIB} INTERFACE)

target_include_directories(${NXP_DRIVER_LIB}
    INTERFACE
        ${APP_ROOT}/devices/IW512
        ${TOP_DIR}/common/incl
        ${TOP_DIR}/common/init/incl
        ${TOP_DIR}/public/incl
        ${TOP_DIR}/wlan/incl
        ${TOP_DIR}/common/rom/incl
        ${TOP_DIR}/public/rom/incl
        ${TOP_DIR}/btu2_5x/incl
        ${TOP_DIR}/btu2_5x/rom/modules/btble/incl
        ${TOP_DIR}/ffu/smac/src/conn_test/CMSIS
        ${TOP_DIR}/btu2_5x/rom/modules/btble/os/threadx
        ${TOP_DIR}/common/modules/hal/spi/incl/
        ${SDK_DIR}/platform/drivers/common
        ${SDK_DIR}/platform/drivers/gpio
        ${SDK_DIR}/platform/drivers/ltc
        ${SDK_DIR}/platform/drivers/port
        ${SDK_DIR}/platform/drivers/trng
        ${SDK_DIR}/components/mem_manager
        ${SDK_DIR}/components/messaging
        ${SDK_DIR}/components/serial_manager
        ${SDK_DIR}/components/uart
        ${SDK_DIR}/components/template
        ${SDK_DIR}/components/lists
        ${SDK_DIR}/components/common_task
        ${SDK_DIR}/components/osa
        ${SDK_DIR}/components/panic
        ${TOP_DIR}/ffu/smac/src/conn_test/framework/common
        ${SDK_DIR}/middleware/wireless/framework/FunctionLib
        ${SDK_DIR}/middleware/wireless/framework/RNG
        ${SDK_DIR}/middleware/wireless/framework/SecLib
        ${TOP_DIR}/ffu/smac/src/conn_test/framework/XCVR/drv
        ${TOP_DIR}/ffu/smac/src/conn_test/framework/XCVR/drv/nb2p4ghz
        ${TOP_DIR}/ffu/smac/src/conn_test/framework/XCVR/drv/nb2p4ghz/configs/gen45
        ${TOP_DIR}/ffu/prebuilt/ot_sdk/ieee-802.15.4/ieee_802_15_4/phy/interface
        ${TOP_DIR}/ffu/prebuilt/ot_sdk/ieee-802.15.4/ieee_802_15_4/phy/source/IW512
)


target_compile_definitions(${NXP_DRIVER_LIB}
    INTERFACE
        #--c99
        -DCPU2 # Not sure to check
        -DBUILT_THREADX_5_5 # Not sure to check
        -DW9177
        -D__ARM_ARCH_7M__=0
        -DCPU_MKW41Z512VHT4=1
        -DFFU_IW512_CM3=1
        -DRADIO_IS_GEN_4P5=1
        -D__ARMCC_VERSION=503102
        -DSERIAL_MANAGER_NON_BLOCKING_MODE=1
        -DSERIAL_USE_CONFIGURE_STRUCTURE=1
        -DSDK_COMPONENT_INTEGRATION=1
        -DSERIAL_PORT_TYPE_UART=1
        -DgSerialManagerMaxInterfaces_c=1
        -DFSL_FEATURE_SOC_APUART_COUNT=1
        -DOSA_USED=1
        -DFSL_RTOS_THREADX=1
        -DFSL_OSA_TASK_ENABLE=1
        -DOT_EXTERNAL_BUILD=1
        -DgFsciIncluded_c=0
        -DRF_OSC_26MHZ=0
        -DHDI_MODE=1
        -DgMainThreadPriority_c=13
        -DSERIAL_MANAGER_TASK_PRIORITY=15
        -DFFU_CNS_TX_PWR_TABLE_CALIBRATION=1
        -DFFU_OT_APP
)
