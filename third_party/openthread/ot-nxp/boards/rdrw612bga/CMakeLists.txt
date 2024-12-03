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

add_library(${NXP_BOARD_LIB}
    app_services_init.c
    board.c
    board_lp.c
    pin_mux.c
    clock_config.c
    hardware_init.c
    peripherals.c
    flash_config.c
)

target_include_directories(${NXP_BOARD_LIB}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${OT_NXP_FREERTOS_CONFIG_PATH}
)

target_link_libraries(${NXP_BOARD_LIB}
    PRIVATE
        ot-config
        connfwk-LowPower
        ${NXP_DRIVER_LIB}
        ${OT_PLATFORM_LIB}
)

if(NOT DEFINED BOARD_APP_UART_INSTANCE)
    set(BOARD_APP_UART_INSTANCE 3)
endif()

target_compile_definitions(${NXP_BOARD_LIB}
    PUBLIC
        -DBOARD_APP_UART_INSTANCE=${BOARD_APP_UART_INSTANCE}
        -DBOOT_HEADER_ENABLE=1
)

target_compile_options(${NXP_BOARD_LIB}
    PRIVATE
        ${OT_CFLAGS}
        -Wno-unknown-pragmas
        -Wno-sign-compare
        -Wno-unused-function
        -Wno-unused-parameter
        -Wno-empty-body
)