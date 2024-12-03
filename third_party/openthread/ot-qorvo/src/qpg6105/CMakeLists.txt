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

target_compile_definitions(ot-config INTERFACE
    "MBEDTLS_CONFIG_FILE=\"${SDK_DIR}/${QORVO_PLATFORM}/inc/${QORVO_PLATFORM}-mbedtls-config.h\""
)

list(APPEND OT_PUBLIC_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/crypto"
)

set(OT_PLATFORM_DEFINES ${OT_PLATFORM_DEFINES} PARENT_SCOPE)

set(OT_PUBLIC_INCLUDES ${OT_PUBLIC_INCLUDES} PARENT_SCOPE)

set(PLATFORM_SOURCES
    alarm.c
    diag.c
    entropy.c
    logging.c
    misc.c
    platform.c
    radio.c
    settings.c
    syscalls_stubs.c
    uart.c
)

add_library(${PLATFORM_TARGET_FTD}
    STATIC
    ${PLATFORM_SOURCES}
)

set_target_properties(${PLATFORM_TARGET_FTD}
    PROPERTIES
        C_STANDARD 99
        CXX_STANDARD 20
)

target_link_libraries(${PLATFORM_TARGET_FTD}
    PRIVATE
        -lc # Required to rearrange linking order to pick up syscalls stubs for libc
        -Wl,--start-group
        ${PLATFORM_DRIVER_FTD}
        ot-config
        -Wl,--end-group
    PUBLIC
        -T${SDK_DIR}/${QORVO_PLATFORM}/ld/${QORVO_PLATFORM}.ld
        -nostdlib
        -Wl,--gc-sections
        -Wl,-Map=$<TARGET_PROPERTY:NAME>.map,--cref
)

target_compile_definitions(${PLATFORM_TARGET_FTD}
    PUBLIC
        ${OT_PLATFORM_DEFINES}
)

target_compile_options(${PLATFORM_TARGET_FTD}
    PRIVATE
        ${OT_CFLAGS}
)

target_include_directories(${PLATFORM_TARGET_FTD}
    PRIVATE
        ${OT_PUBLIC_INCLUDES}
        ${OPENTHREAD_DIR}/examples/platforms
)

add_library(${PLATFORM_TARGET_MTD}
    STATIC
    ${PLATFORM_SOURCES}
)

set_target_properties(${PLATFORM_TARGET_MTD}
    PROPERTIES
        C_STANDARD 99
        CXX_STANDARD 20
)

target_link_libraries(${PLATFORM_TARGET_MTD}
    PRIVATE
        -lc # Required to rearrange linking order to pick up syscalls stubs for libc
        -Wl,--start-group
        ${PLATFORM_DRIVER_MTD}
        ot-config
        -Wl,--end-group
    PUBLIC
        -T${SDK_DIR}/${QORVO_PLATFORM}/ld/${QORVO_PLATFORM}.ld
        -nostdlib
        -Wl,--gc-sections
        -Wl,-Map=$<TARGET_PROPERTY:NAME>.map,--cref
)

target_compile_definitions(${PLATFORM_TARGET_MTD}
    PUBLIC
        ${OT_PLATFORM_DEFINES}
)

target_compile_options(${PLATFORM_TARGET_MTD}
    PRIVATE
        ${OT_CFLAGS}
)

target_include_directories(${PLATFORM_TARGET_MTD}
    PRIVATE
        ${OT_PUBLIC_INCLUDES}
        ${OPENTHREAD_DIR}/examples/platforms
)

target_include_directories(ot-config INTERFACE ${OT_PUBLIC_INCLUDES})
target_compile_definitions(ot-config INTERFACE ${OT_PLATFORM_DEFINES})
