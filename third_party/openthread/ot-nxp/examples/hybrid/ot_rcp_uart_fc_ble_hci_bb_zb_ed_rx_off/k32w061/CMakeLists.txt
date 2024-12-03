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

if (SDK_RELEASE)
    set(EXAMPLE_BASE_DIR $ENV{NXP_K32W0_SDK_ROOT}/boards/k32w061dk6/wireless_examples/zigbee/zigbee_ed_rx_off)
else()
    set(EXAMPLE_BASE_DIR $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/examples/zigbee_ed_rx_off/src)
endif()

set(ZPSCFG "${CMAKE_CURRENT_SOURCE_DIR}/../../zb_common/enddevice_rxoff.zpscfg")

set(ENCRYPTION_KEY 1234567890ABCDEFA1B2C3D4E5F6F1B4)
set(MANUFACTURER_CODE 0x1037)
set(SINGLE_CHANNEL 11)

set_property(SOURCE pdum_apdu.S PROPERTY LANGUAGE C)

add_executable(ot-rcp-uart-fc-ble-hci-bb-zb-ed-rx-off-${OT_NXP_PLATFORM}
    ${PROJECT_SOURCE_DIR}/openthread/examples/apps/ncp/ncp.c
    main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../zb_common/app_helpers.c

    ${EXAMPLE_BASE_DIR}/app_start.c

    ${EXAMPLE_BASE_DIR}/app_main.c

    ${EXAMPLE_BASE_DIR}/app_end_device_node.c
    ${EXAMPLE_BASE_DIR}/app_zcl_task.c
    ${EXAMPLE_BASE_DIR}/app_reporting.c

    $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/examples/zb_common/app_zigbee_config.c
    $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/examples/zb_common/app_ota_client.c
    $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/examples/zb_common/app_leds.c
    $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/examples/zb_common/app_crypto.c
    $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/examples/zb_common/app_buttons.c

    $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZigbeeCommon/Source/port_JN518x.c

    pdum_gen.c
    zps_gen.c
    pdum_apdu.S
)

# Add specific target for the generated files files, no need to create a lib,
# running the command for generating them is enough.
add_custom_target(zigbee-gen
    COMMAND "$ENV{PDUMCONFIG}" -z enddevice_rxoff -e ENDIAN -f ${ZPSCFG} -o ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND "$ENV{ZPSCONFIG}" -n enddevice_rxoff -t JN518x -l $ENV{ZPS_NWK_LIB} -a $ENV{ZPS_APL_LIB} -e LITTLE_ENDIAN -f ${ZPSCFG} -o ${CMAKE_CURRENT_SOURCE_DIR}
    BYPRODUCTS
        ${CMAKE_CURRENT_SOURCE_DIR}/pdum_gen.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pdum_gen.h
        ${CMAKE_CURRENT_SOURCE_DIR}/zps_gen.c
        ${CMAKE_CURRENT_SOURCE_DIR}/zps_gen.h
        ${CMAKE_CURRENT_SOURCE_DIR}/pdum_apdu.S
)

# Mark files as generated, otherwise compilation would fail (source not found).
set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/pdum_gen.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zps_gen.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pdum_apdu.S
    GENERATED
)

# Make the zigbee lib depend on the new target, the app depends on the zigbee lib,
# and it is compiled first.
add_dependencies(nxp-k32w061-zigbee zigbee-gen)

target_include_directories(ot-rcp-uart-fc-ble-hci-bb-zb-ed-rx-off-${OT_NXP_PLATFORM} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/openthread/examples/platforms
    $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/examples/zb_common
    $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/ieee-802.15.4/MacDynamic/Include
    ${PROJECT_SOURCE_DIR}/examples/hybrid/zb_common
    $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform
    ${EXAMPLE_BASE_DIR}
)

if (SDK_RELEASE)
    # for SDK release, app.h is in under bm
    target_include_directories(nxp-k32w061-zigbee PUBLIC
        ${EXAMPLE_BASE_DIR}/bm
    )
else()
    target_include_directories(nxp-k32w061-zigbee PUBLIC
        # for non-SDK release, app.h resides here
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/board/dk6
    )
endif()

#Disabling some warnings due to the SDK
target_compile_options(ot-config INTERFACE
    -Wno-unused-parameter
    -Wno-sign-compare
    -Wno-pedantic
    -Wno-unused-function
)

#Add a dependeny to SDK files with app_pre_include
target_compile_definitions(${NXP_DRIVER_LIB}
    PUBLIC
        APP_PRE_INCLUDE="${CMAKE_CURRENT_SOURCE_DIR}/app_preinclude.h"
        DUAL_MODE_APP
        SDK_DEBUGCONSOLE=DEBUGCONSOLE_REDIRECT_TO_SDK
)

target_compile_definitions(nxp-k32w061-zigbee
    PUBLIC
        EndDevice
        OTA_INTERNAL_STORAGE
        INTERNAL_ENCRYPTED
        CLD_OTA_KEY="${ENCRYPTION_KEY}"
        CLD_OTA_MANF_ID_VALUE=${MANUFACTURER_CODE}
        SINGLE_CHANNEL=${SINGLE_CHANNEL}
)

target_include_directories(nxp-k32w061-zigbee PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${EXAMPLE_BASE_DIR}
)

#In case of single UART support, following defines are added, in this case all hci and spinel frame would go through the ot plat UART interface
target_compile_definitions(${NXP_DRIVER_LIB}
    PUBLIC
        gUartDebugConsole_d=1
        gHcitSerialManagerSupport_d=0 # Disable serial manager on hci
        OT_PLAT_APP_UART_INSTANCE=0 # UART 0 with flow control would be used to transfert HCI and spinel packet
)
target_link_libraries(ot-rcp-uart-fc-ble-hci-bb-zb-ed-rx-off-${OT_NXP_PLATFORM} PUBLIC
    -Wl,--wrap=otNcpHdlcReceive
    -Wl,--wrap=Hcit_SendPacket
    -Wl,--wrap=otLinkRawTransmit
)

target_compile_definitions(ot-rcp-uart-fc-ble-hci-bb-zb-ed-rx-off-${OT_NXP_PLATFORM}
    PUBLIC
        SLEEP_MIN_RETENTION
        EndDevice
        PRINTF_ADVANCED_ENABLE=1
        ZBPRO_DEVICE_TYPE=ZED
        OTA_ENCRYPTED
        BUILD_OTA
        OTA_INTERNAL_STORAGE
        ZED_SLEEP_PERIOD=7000 # ZED 'sleeps' for 7s
        POLL_TIME_FAST=500    # ZED sends a Data Req every POLL_TIME_FAST ms
        ZED_MAX_POLLS=3       # ZED sends ZED_MAX_POLLS + 1 Data Reqs
        ZB_PROTO_ID=0
)

# We can't use app_preinclude.h since it is -imacro-ed, and not -includ-ed
# as it's done on other platforms
target_compile_options(ot-rcp-uart-fc-ble-hci-bb-zb-ed-rx-off-${OT_NXP_PLATFORM}
    PRIVATE
        -include ${CMAKE_CURRENT_SOURCE_DIR}/../../zb_common/zigbee_gen.h
)

#create empty libraries to be able to link with connectivity.ld
add_library(cr_c app_preinclude.h)
add_library(cr_eabihelpers app_preinclude.h)
add_library(cr_newlib_nohost app_preinclude.h)
set_property(TARGET cr_c cr_eabihelpers cr_newlib_nohost PROPERTY LINKER_LANGUAGE CXX)

set(APP_LINKER_FILE ${PROJECT_SOURCE_DIR}/src/k32w0/k32w061/k32w061_dm.ld)

target_link_libraries(ot-rcp-uart-fc-ble-hci-bb-zb-ed-rx-off-${OT_NXP_PLATFORM} PUBLIC
    ${OT_PLATFORM_LIB}
    openthread-rcp
    ot-config

    -Llib
    -T${APP_LINKER_FILE}
    -Xlinker --defsym=gUseInternalStorageLink_d=0
    -Xlinker --defsym=gUseNVMLink_d=1
    -Xlinker --defsym=__ram_vector_table__=1
    -Xlinker --defsym=__app_load_address__=0
    -Xlinker --defsym=__app_stated_size__=613376
    -Xlinker --defsym=__app_id__=0
)
