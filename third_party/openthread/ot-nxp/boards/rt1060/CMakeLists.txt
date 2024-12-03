#
# Copyright (c) 2023, The OpenThread Authors.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 1. Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
# 3. Neither the name of the copyright holder nor the
# names of its contributors may be used to endorse or promote products
# derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

list(APPEND BOARD_SOURCES
    app_services_init.c
    board.c
    hardware_init.c
    peripherals.c
)
list(APPEND BOARD_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

if("${EVK_RT1060_BOARD}" STREQUAL "evkcmimxrt1060")
    list(APPEND BOARD_SOURCES ${EVK_RT1060_BOARD}/pin_mux.c)
    list(APPEND BOARD_SOURCES ${EVK_RT1060_BOARD}/clock_config.c)
    list(APPEND BOARD_INCLUDE_DIRECTORIES ${EVK_RT1060_BOARD})
else()
    list(APPEND BOARD_SOURCES evkbimxrt1060/pin_mux.c)
    list(APPEND BOARD_SOURCES evkbimxrt1060/clock_config.c)
    list(APPEND BOARD_INCLUDE_DIRECTORIES evkbimxrt1060)
endif()

list(APPEND BOARD_COMPILE_DEFINITIONS_PUBLIC)
list(APPEND BOARD_COMPILE_DEFINITIONS_PRIVATE)

if("${OT_NXP_TRANSCEIVER}" STREQUAL "iwx12")
    list(APPEND BOARD_SOURCES
        sdmmc_config.c
    )
    list(APPEND BOARD_COMPILE_DEFINITIONS_PUBLIC
        -DSDIO_ENABLED
    )
elseif("${OT_NXP_TRANSCEIVER}" STREQUAL "k32w0")
    list(APPEND BOARD_COMPILE_DEFINITIONS_PRIVATE
        -DBOARD_OTW_K32W0_PIN_INIT
    )
endif()

if("${SPINEL_INTERFACE_TYPE}" STREQUAL "UART")
    list(APPEND BOARD_COMPILE_DEFINITIONS_PRIVATE
        -DBOARD_SPINEL_OVER_UART
    )
elseif("${SPINEL_INTERFACE_TYPE}" STREQUAL "SPI")
    list(APPEND BOARD_COMPILE_DEFINITIONS_PRIVATE
        -DBOARD_SPINEL_OVER_SPI
    )
endif()

if(NOT DEFINED BOARD_APP_UART_INSTANCE)
    set(BOARD_APP_UART_INSTANCE 1)
endif()

add_library(${NXP_BOARD_LIB}
    ${BOARD_SOURCES}
)

target_include_directories(${NXP_BOARD_LIB}
    PUBLIC
    ${BOARD_INCLUDE_DIRECTORIES}
)

target_compile_definitions(${NXP_BOARD_LIB}
    PUBLIC
    -DBOARD_APP_UART_INSTANCE=${BOARD_APP_UART_INSTANCE}
    ${BOARD_COMPILE_DEFINITIONS_PUBLIC}
)

target_compile_definitions(${NXP_BOARD_LIB}
    PRIVATE
    ${BOARD_COMPILE_DEFINITIONS_PRIVATE}
)

target_link_libraries(${NXP_BOARD_LIB}
    PRIVATE
    ${NXP_DRIVER_LIB}
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