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

add_executable(ot-br-${OT_NXP_PLATFORM}
    br_ot_freertos.c
    main.c
    ${PROJECT_SOURCE_DIR}/examples/cli/libc_hook.cpp
    ${PROJECT_SOURCE_DIR}/examples/cli/app_freertos_hooks.c
    ${PROJECT_SOURCE_DIR}/openthread/examples/apps/cli/cli_uart.cpp
)

if(OT_NXP_LWIP_WIFI)
    if(OT_NXP_LWIP_ETH)
        message(FATAL_ERROR "Only an external interface needs to be configured for the device to work. Please select either WIFI or Ethernet")
    else()
        target_compile_definitions(ot-br-${OT_NXP_PLATFORM} PRIVATE OT_APP_BR_WIFI_EN)
        target_link_libraries(ot-br-${OT_NXP_PLATFORM} PRIVATE wifi)
    endif()

    # In case of wifi support
    target_compile_definitions(ot-br-${OT_NXP_PLATFORM}
        PRIVATE
        WIFI_IDLE_HOOK_CALL_REQUIRED
    )
else()
    if(OT_NXP_LWIP_ETH)
        target_compile_definitions(ot-br-${OT_NXP_PLATFORM} PRIVATE OT_APP_BR_ETH_EN)
    else()
        message(FATAL_ERROR "An external interface needs to be configured for the device to work. Please select WIFI or Ethernet")
    endif()
endif()

target_include_directories(ot-br-${OT_NXP_PLATFORM} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/openthread/examples/platforms
    ${PROJECT_SOURCE_DIR}/examples/common/includes
)

target_link_libraries(ot-br-${OT_NXP_PLATFORM} PRIVATE
    openthread-cli-ftd
    openthread-ftd
    nxp-lwip
    ${OT_PLATFORM_LIB}
    ${NXP_DRIVER_LIB}
    ot-config
    ot-cli-addons
    -Wl,--start-group ${OT_PLATFORM_LIB} -Wl,--end-group

    # Wrapping libc malloc functions and defining the libc heap size to 0
    -Wl,--defsym=__heap_size__=0
    -Wl,--wrap=_malloc_r
    -Wl,--wrap=_realloc_r
    -Wl,--wrap=_free_r
    -Wl,--wrap=_calloc_r
    -Wl,--wrap=malloc
    -Wl,--wrap=free
    -Wl,--wrap=realloc
    -Wl,--wrap=calloc
)

if(NXP_BOARD_LIB)
    target_link_libraries(ot-br-${OT_NXP_PLATFORM} PRIVATE
        ${NXP_BOARD_LIB}
    )
endif()

if(DEFINED WIFI_SSID)
    target_compile_definitions(ot-br-${OT_NXP_PLATFORM}
        PRIVATE
        WIFI_SSID="${WIFI_SSID}"
    )
endif()

if(DEFINED WIFI_PASSWORD)
    target_compile_definitions(ot-br-${OT_NXP_PLATFORM}
        PRIVATE
        WIFI_PASSWORD="${WIFI_PASSWORD}"
    )
endif()