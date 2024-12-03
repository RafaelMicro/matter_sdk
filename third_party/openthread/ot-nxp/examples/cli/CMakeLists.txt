#
# Copyright (c) 2022-2023, The OpenThread Authors.
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

if(OT_NXP_BUILD_APP_AS_LIB)
    add_library(ot-cli-${OT_NXP_PLATFORM}
        app_ot_freertos.c
        libc_hook.cpp
        ${PROJECT_SOURCE_DIR}/openthread/examples/apps/cli/cli_uart.cpp
    )
else()
    add_executable(ot-cli-${OT_NXP_PLATFORM}
        main.c
        app_ot_freertos.c
        app_freertos_hooks.c
        libc_hook.cpp
        ${PROJECT_SOURCE_DIR}/openthread/examples/apps/cli/cli_uart.cpp
    )
endif()

target_include_directories(ot-cli-${OT_NXP_PLATFORM} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/openthread/examples/platforms
    ${PROJECT_SOURCE_DIR}/examples/common/includes
)

target_link_libraries(ot-cli-${OT_NXP_PLATFORM} PRIVATE
    openthread-cli-ftd
    openthread-ftd
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
    target_link_libraries(ot-cli-${OT_NXP_PLATFORM} PRIVATE
        ${NXP_BOARD_LIB}
    )
endif()
