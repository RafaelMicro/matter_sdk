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

add_executable(ot-rcp-ble-hci-bb-${OT_NXP_PLATFORM}
    main.c
    ${PROJECT_SOURCE_DIR}/openthread/examples/apps/ncp/ncp.c
)

target_include_directories(ot-rcp-ble-hci-bb-${OT_NXP_PLATFORM} PRIVATE
    ${PROJECT_SOURCE_DIR}/openthread/examples/platforms
)

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
)

if (OT_APP_RCP_BLE_HCI_BB_SINGLE_UART)
    #In case of single UART support, following defines are added, in this case all hci and spinel frame would go through the ot plat UART interface
    target_compile_definitions(${NXP_DRIVER_LIB}
        PUBLIC
            gUartDebugConsole_d=1
            SDK_DEBUGCONSOLE=1
            gHcitSerialManagerSupport_d=0 # Disable serial manager on hci
            OT_PLAT_APP_UART_INSTANCE=0 # UART 0 with flow control would be used to transfert HCI and spinel packet
    )
    target_link_libraries(ot-rcp-ble-hci-bb-${OT_NXP_PLATFORM} PUBLIC
        -Wl,--wrap=otNcpHdlcReceive
        -Wl,--wrap=Hcit_SendPacket
        -Wl,--wrap=otLinkRawTransmit
    )
else ()
    #In other cases the OT UART instance is moved on UART1 and HCI on UART0
    target_compile_definitions(${NXP_DRIVER_LIB}
        PUBLIC
            gHcitSerialManagerSupport_d=1
            gHcitInterfaceNumber_d=0
            gHcitInterfaceSpeed_d=gUARTBaudRate115200_c
            OT_PLAT_APP_UART_INSTANCE=1
    )
endif ()

#create empty libraries to be able to link with connectivity.ld
add_library(cr_c app_preinclude.h)
add_library(cr_eabihelpers app_preinclude.h)
add_library(cr_newlib_nohost app_preinclude.h)
set_property(TARGET cr_c cr_eabihelpers cr_newlib_nohost PROPERTY LINKER_LANGUAGE CXX)

set(APP_LINKER_FILE ${PROJECT_SOURCE_DIR}/src/k32w0/k32w061/k32w061_dm.ld)

target_link_libraries(ot-rcp-ble-hci-bb-${OT_NXP_PLATFORM} PUBLIC
    ${OT_PLATFORM_LIB}
    openthread-rcp
    ot-config
    -Llib
    -T${APP_LINKER_FILE}
    -Xlinker --defsym=gUseInternalStorageLink_d=0
    -Xlinker --defsym=gUseNVMLink_d=1
    -Xlinker --defsym=__ram_vector_table__=1
    -Xlinker --defsym=__app_load_address__=0
    -Xlinker --defsym=__app_stated_size__=573440
    -Xlinker --defsym=__app_id__=0
)
