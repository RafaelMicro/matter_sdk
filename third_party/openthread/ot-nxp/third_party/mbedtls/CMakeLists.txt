#
#  Copyright (c) 2022, NXP.
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

cmake_policy(SET CMP0079 NEW)

if(NOT OT_NXP_MBEDTLS_CONFIG_FILE)
    set(OT_NXP_MBEDTLS_CONFIG_FILE "\"${OT_NXP_PLATFORM}-mbedtls-config.h\"")
endif()

if(NOT OT_NXP_MBEDTLS_CONFIG_FILE_PATH)
    set(OT_NXP_MBEDTLS_CONFIG_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/configs)
endif()

if(NOT OT_NXP_MBEDTLS_PATH)
    if(MIDDLEWARE_PATH)
        # Build mbedtls from SDK middleware by default
        set(OT_NXP_MBEDTLS_PATH ${MIDDLEWARE_PATH}/mbedtls)
    else()
        set(OT_NXP_MBEDTLS_PATH ${NXP_SDK_ROOT}/middleware/mbedtls)
    endif()
endif()

set(ENABLE_TESTING OFF CACHE BOOL "Disable mbedtls test" FORCE)
set(ENABLE_PROGRAMS OFF CACHE BOOL "Disable mbetls program" FORCE)

string(REPLACE "-Wconversion" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
string(REPLACE "-Wconversion" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(MBEDTLS_TARGET_PREFIX nxp-${OT_NXP_PLATFORM}-)
set(mbedtls_target    "${MBEDTLS_TARGET_PREFIX}mbedtls")
set(mbedx509_target   "${MBEDTLS_TARGET_PREFIX}mbedx509")
set(mbedcrypto_target "${MBEDTLS_TARGET_PREFIX}mbedcrypto")
list (APPEND thirdparty_inc_public
    #Config file path
    ${OT_NXP_MBEDTLS_CONFIG_FILE_PATH}
    #openthread mbedtls-config.h path file required
    ${PROJECT_SOURCE_DIR}/openthread/third_party/mbedtls
)

if(OT_NXP_MBEDTLS_PORT)
    # Port will add sources and include directories to thirdparty_src and thirdparty_inc_public
    include(port/${OT_NXP_MBEDTLS_PORT}.cmake)
endif()

#Make sure to disable some CMAKE_C_FLAGS that would be added by mbedtls
# Instead use only CMAKE_C_FLAGS set by the application
set(CMAKE_COMPILER_IS_GNUCC OFF)
string(REPLACE "GNU" "G-N-U" CMAKE_C_COMPILER_ID "${CMAKE_C_COMPILER_ID}")
add_subdirectory(${OT_NXP_MBEDTLS_PATH} ${PROJECT_BINARY_DIR}/mbedtls)
string(REPLACE "G-N-U" "GNU" CMAKE_C_COMPILER_ID "${CMAKE_C_COMPILER_ID}")
set(CMAKE_COMPILER_IS_GNUCC ON)

target_compile_definitions(${mbedtls_target}
    PUBLIC
        MBEDTLS_CONFIG_FILE=${OT_NXP_MBEDTLS_CONFIG_FILE}
)

target_compile_options(${mbedcrypto_target}
    PRIVATE
        -Wno-unused-function
        -Wno-unused-variable
        -Wno-unused-const-variable
        -Wno-memset-elt-size
        -Wno-int-conversion
)

target_link_libraries(${mbedtls_target}
    PUBLIC
        ${NXP_DRIVER_LIB}
    PRIVATE
        ot-config
)

target_link_libraries(${mbedx509_target}
    PUBLIC
        ${NXP_DRIVER_LIB}
    PRIVATE
        ot-config
)

target_link_libraries(${mbedcrypto_target}
    PUBLIC
        ${NXP_DRIVER_LIB}
    PRIVATE
        ot-config
)

target_link_libraries(ot-config
    INTERFACE
        ${mbedtls_target}
)


