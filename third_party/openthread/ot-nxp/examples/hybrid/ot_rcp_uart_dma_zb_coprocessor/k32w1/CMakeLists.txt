#
#  Copyright (c) 2023, The OpenThread Authors.
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

if (SDK_RELEASE)
    set(EXAMPLE_BASE_DIR ${ZB_EXMPL_DIR}/coprocessor/bm)
else()
    if (SDK_GH)
        set(EXAMPLE_BASE_DIR ${ZB_EXMPL_DIR}/coprocessor/bm)
    else()
        set(EXAMPLE_BASE_DIR ${ZB_EXMPL_DIR}/zigbee_coprocessor/src)
    endif()
endif()

set(ZPSCFG "${EXAMPLE_BASE_DIR}/coordinator.zpscfg")

set(SINGLE_CHANNEL 11)

set_property(SOURCE pdum_apdu.S PROPERTY LANGUAGE C)

add_executable(ot-rcp-uart-dma-zb-coprocessor-${OT_NXP_PLATFORM}
    ${PROJECT_SOURCE_DIR}/openthread/examples/apps/ncp/ncp.c
    main.c

    ${EXAMPLE_BASE_DIR}/app_start.c
    ${EXAMPLE_BASE_DIR}/app_main.c

    ${EXAMPLE_BASE_DIR}/app_coordinator.c
    ${EXAMPLE_BASE_DIR}/app_serial_commands.c
    ${EXAMPLE_BASE_DIR}/app_zb_stack_utils.c

    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/examples/zb_common/serial_link_wkr.c
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/examples/zb_common/serial_link_cmds_wkr.c

    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/examples/zb_common/app_zigbee_config.c
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/examples/zb_common/app_uart.c
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/examples/zb_common/app_crypto.c

    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/ZigbeeCommon/Source/port_JN518x.c

    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/examples/zb_common/app_leds.c
)

add_library(nxp-k32w1-zigbee-gen
    ${CMAKE_CURRENT_SOURCE_DIR}/pdum_gen.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zps_gen.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pdum_apdu.S
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h
)

target_include_directories(nxp-k32w1-zigbee-gen
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}

    ${EXAMPLE_BASE_DIR}

    $ENV{NXP_K32W1_SDK_ROOT}/devices/K32W1480/internal

    $ENV{NXP_K32W1_SDK_ROOT}/devices/K32W1480

    $ENV{NXP_K32W1_SDK_ROOT}/CMSIS/Core/Include

    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/framework/SecLib
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/framework/Common
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/framework/PDUM/Include

    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/ieee-802.15.4/Include

    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/platform/K32W1/framework/Common
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/platform/K32W1/framework/SecLib
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/ZigbeeCommon/Include
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/ZPSAPL/Include
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/ZPSNWK/Include
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/ZPSMAC/Include
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/ZPSTSV/Include
)

target_compile_definitions(nxp-k32w1-zigbee-gen
    PRIVATE
    CPU_K32W1480VFTA
)

if (SDK_RELEASE)
    add_custom_command(OUTPUT ${ZPSCFG}
        COMMAND dos2unix -q $ENV{PDUMCONFIG}
        COMMAND dos2unix -q $ENV{ZPSCONFIG}
        COMMAND dos2unix -q $ENV{EXAMPLE_BASE_DIR}/build_version.sh
    )
endif()

add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/version.h"
    COMMAND NXP_SDK_ROOT=$ENV{NXP_K32W1_SDK_ROOT} VERSION_FILE=${CMAKE_CURRENT_SOURCE_DIR}/version.h sh "${EXAMPLE_BASE_DIR}/build_version.sh"
)

add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/pdum_gen.c" "${CMAKE_CURRENT_SOURCE_DIR}/pdum_gen.h" "${CMAKE_CURRENT_SOURCE_DIR}/pdum_apdu.S"
    DEPENDS ${ZPSCFG}
    COMMAND sh "$ENV{PDUMCONFIG}" -z coordinator -e ENDIAN -f ${ZPSCFG} -o ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/zps_gen.c" "${CMAKE_CURRENT_SOURCE_DIR}/zps_gen.h"
    DEPENDS ${ZPSCFG}
    COMMAND sh "$ENV{ZPSCONFIG}" -n coordinator -t JN518x -l $ENV{ZPS_NWK_LIB} -a $ENV{ZPS_APL_LIB} -e LITTLE_ENDIAN -f ${ZPSCFG} -o ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(ot-rcp-uart-dma-zb-coprocessor-${OT_NXP_PLATFORM} PRIVATE
    ${PROJECT_SOURCE_DIR}/openthread/examples/platforms
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/examples/zb_common
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/ieee-802.15.4/Include
    $ENV{NXP_K32W1_SDK_ROOT}/${MIDLWR_DIR}/wireless/zigbee/platform
#    ${PROJECT_SOURCE_DIR}/examples/hybrid/zb_common
)

if (SDK_RELEASE)
    # for SDK release, app.h is in under bm
    target_include_directories(ot-rcp-uart-dma-zb-coprocessor-${OT_NXP_PLATFORM} PUBLIC
#        ${EXAMPLE_BASE_DIR}/bm
    )
else()
    target_include_directories(ot-rcp-uart-dma-zb-coprocessor-${OT_NXP_PLATFORM} PRIVATE
        # for non-SDK release, app.h resides here
#        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/examples/board/dk6
    )
endif()


target_compile_options(ot-config INTERFACE
    -Wno-unused-parameter
    -Wno-sign-compare
    -Wno-pedantic
    -Wno-unused-function
)

#Add a dependeny to SDK files with app_pre_include
target_compile_definitions(${NXP_DRIVER_LIB}
    PUBLIC
        FSL_OSA_TASK_ENABLE=1
        FSL_OSA_MAIN_FUNC_ENABLE=1
        SDK_DEBUGCONSOLE=DEBUGCONSOLE_REDIRECT_TO_SDK
        DebugConsole_c=0
        MAC_TYPE_SOC
        FSL_OSA_BM_TIMER_CONFIG=FSL_OSA_BM_TIMER_SYSTICK
        BOARD_UART_INSTANCE=0
        BOARD_DEBUG_UART_BAUDRATE=1000000
)

target_compile_definitions(nxp-k32w1-zigbee
    PUBLIC
#        Router # for OTA. Collides with OT code
        DISABLE_OTA
        SINGLE_CHANNEL=${SINGLE_CHANNEL}
)

target_compile_options(nxp-k32w1-zigbee-plat
    PRIVATE
        -include ${CMAKE_CURRENT_SOURCE_DIR}/app_preinclude.h
)

target_include_directories(nxp-k32w1-zigbee-plat PUBLIC
    ${EXAMPLE_BASE_DIR}
)

target_include_directories(nxp-k32w1-zigbee PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${EXAMPLE_BASE_DIR}
)

target_compile_definitions(ot-rcp-uart-dma-zb-coprocessor-${OT_NXP_PLATFORM}
    PUBLIC
        DISABLE_OTA
        NCP_COPRO
        ZB_COORD_DEVICE
#        Router		// for OTA. Collides with OT code
        PRINTF_ADVANCED_ENABLE=1
        ZBPRO_DEVICE_TYPE=ZCR
)

target_compile_options(ot-rcp-uart-dma-zb-coprocessor-${OT_NXP_PLATFORM}
    PRIVATE
        -include ${CMAKE_CURRENT_SOURCE_DIR}/app_preinclude.h
)
set(APP_LINKER_FILE ${PROJECT_SOURCE_DIR}/src/k32w1/k32w1/K32W1480_connectivity.ld)

target_compile_options(ot-config INTERFACE
    -Wno-unused-parameter
    -Wno-sign-compare
    -Wno-pedantic
    -Wno-unused-function
)

target_link_libraries(ot-rcp-uart-dma-zb-coprocessor-${OT_NXP_PLATFORM} PUBLIC
    nxp-k32w1-zigbee-gen
    nxp-k32w1-zigbee
    openthread-rcp
    ${OT_PLATFORM_LIB}
    ${OT_MBEDTLS}
    ot-config
    -Llib
)
