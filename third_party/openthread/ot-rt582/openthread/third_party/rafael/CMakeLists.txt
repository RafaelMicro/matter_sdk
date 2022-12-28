#
#  Copyright (c) 2020, The OpenThread Authors.
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

if(OT_PLATFORM STREQUAL "rt58x")
    set_property(SOURCE ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Device/GCC/gcc_startup_cm3_mcu.S PROPERTY LANGUAGE C)

    add_library(rafael-rt58x-driver
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Device/system_cm3_mcu.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Device/GCC/gcc_startup_cm3_mcu.S
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Peripheral/dma.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Peripheral/flashctl.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Peripheral/uart_retarget.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Peripheral/mp_sector.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Peripheral/sysctrl.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Peripheral/sysfun.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Peripheral/timer.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Peripheral/wdt.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Peripheral/gpio.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/PHY/rf_mcu.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/PHY/rf_mcu_ahb.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/PHY/rf_mcu_spi.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/PHY/rf_common_init.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/PHY/comm_subsystem_drv.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/PHY/rt569mp/rt569mp_fw.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/PHY/rt569mp/rt569mp_init.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/RFB/rfb.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/RFB/rfb_comm_15p4Mac.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/RFB/rfb_comm_common.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/RFB/rfb_comm_wisun.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/RFB/rfb_comm_isr.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/RFB/rfb_port.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Crypto/aes.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Crypto/crypto.c
        ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Crypto/crypto_util.c
    )

    target_compile_definitions(rafael-rt58x-driver
        PUBLIC
            ${OT_PLATFORM_DEFINES}
    )

    target_compile_options(rafael-rt58x-driver
        PRIVATE
            ${OT_CFLAGS}
            -Wno-unknown-pragmas
            -Wno-sign-compare
            -Wno-unused-function
            -Wno-unused-parameter
            -Wno-empty-body
	    -w
    )

    target_include_directories(rafael-rt58x-driver
        PUBLIC
            ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src
            ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/RFB/include
            ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/RUCI/include
            ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Portable/bsp/Include
            ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Portable/Utility/include
            ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/CMSIS/Include
            ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Include
            ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Peripheral/Include
            ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/Crypto/include
            ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/PHY/include
            ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/PHY/rf_fw
            ${PROJECT_SOURCE_DIR}/examples/platforms/rt58x/src/Driver/PHY/rt569mp/include
    )

    target_link_libraries(rafael-rt58x-driver PRIVATE ot-config)
endif()
