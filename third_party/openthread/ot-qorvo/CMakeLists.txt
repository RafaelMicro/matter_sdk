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

cmake_minimum_required(VERSION 3.10.2)
project(ot-qorvo VERSION 0.1.0)

set(QORVO_PLATFORM_VALUES
    "gp712"
    "qpg6105"
    "qpg7015m"
)
set(QORVO_MBEDTLS_USED
    "qpg6105"
)

set_property(CACHE QORVO_PLATFORM PROPERTY STRINGS ${QORVO_PLATFORM_VALUES})
if(NOT QORVO_PLATFORM IN_LIST QORVO_PLATFORM_VALUES)
    message(FATAL_ERROR "Please select a supported platform: ${QORVO_PLATFORM_VALUES}")
endif()

if (QORVO_PLATFORM IN_LIST QORVO_MBEDTLS_USED)
    set(OT_BUILTIN_MBEDTLS_MANAGEMENT OFF CACHE BOOL "disable builtin mbedtls management" FORCE)
    set(OT_EXTERNAL_MBEDTLS "qorvo-mbedtls" CACHE STRING "use qorvo mbedtls" FORCE)
    set(OT_MBEDTLS ${OT_EXTERNAL_MBEDTLS})
    set(QORVO_MBEDTLS 1)
else()
    set(QORVO_MBEDTLS 0)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

if (DEFINED ENV{QORVO_OT_SDK})
    set(SDK_DIR "$ENV{QORVO_OT_SDK}")
else()
    set(SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/Qorvo/repo")
    if(NOT EXISTS ${SDK_DIR}/${QORVO_PLATFORM})
        message(FATAL_ERROR "Please select a correct SDK directory: ${SDK_DIR}/${QORVO_PLATFORM} does not exist")
    endif()
endif()

set(PLATFORM_TARGET_FTD "openthread-${QORVO_PLATFORM}-ftd")
set(PLATFORM_DRIVER_FTD "${QORVO_PLATFORM}-driver-ftd")
set(OT_PLATFORM_LIB_FTD "${PLATFORM_TARGET_FTD}")

set(PLATFORM_TARGET_MTD "openthread-${QORVO_PLATFORM}-mtd")
set(PLATFORM_DRIVER_MTD "${QORVO_PLATFORM}-driver-mtd")
set(OT_PLATFORM_LIB_MTD "${PLATFORM_TARGET_MTD}")

set(PLATFORM_TARGET_RCP "openthread-${QORVO_PLATFORM}-ftd")
set(PLATFORM_DRIVER_RCP "${QORVO_PLATFORM}-driver-ftd")
set(OT_PLATFORM_LIB_RCP "${PLATFORM_TARGET_RCP}")
if (${QORVO_PLATFORM} EQUAL "qpg7015m")
    unset(OT_NCP_VENDOR_HOOK_SOURCE CACHE)
    set(OT_NCP_VENDOR_HOOK_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/${QORVO_PLATFORM}/" CACHE STRING "Vendor hook directory")
    set(OT_NCP_VENDOR_HOOK_SOURCE "spinel_extension.cpp" CACHE STRING "Vendor hook file")
endif()

include("${PROJECT_SOURCE_DIR}/etc/options.cmake")

set(OPENTHREAD_DIR ${PROJECT_SOURCE_DIR}/openthread)
add_subdirectory(${OPENTHREAD_DIR} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

target_compile_definitions(ot-config INTERFACE
    OPENTHREAD_CONFIG_FILE="openthread-core-${QORVO_PLATFORM}-config.h"
    OPENTHREAD_PROJECT_CORE_CONFIG_FILE="openthread-core-${QORVO_PLATFORM}-config.h"
    OPENTHREAD_CORE_CONFIG_PLATFORM_CHECK_FILE="openthread-core-${QORVO_PLATFORM}-config-check.h"
)

target_include_directories(ot-config INTERFACE
    ${PROJECT_SOURCE_DIR}/src/${QORVO_PLATFORM}
)

add_subdirectory(src/${QORVO_PLATFORM})
add_subdirectory(third_party/Qorvo)
