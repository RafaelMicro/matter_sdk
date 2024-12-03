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

cmake_policy(SET CMP0076 NEW) # target_sources() command now converts relative source file paths to absolute
cmake_policy(SET CMP0079 NEW)

add_library(ot-cli-addons
    ${CMAKE_CURRENT_SOURCE_DIR}/addons_cli.c
)

target_include_directories(ot-cli-addons PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/openthread/examples/platforms
    ${PROJECT_SOURCE_DIR}/examples/common/includes
)

target_link_libraries(ot-cli-addons PRIVATE
    ${OT_PLATFORM_LIB}
    ot-config
)

if(OT_APP_CLI_FREERTOS_LOWPOWER)
    target_link_libraries(ot-cli-addons PRIVATE
        connfwk-LowPower
    )
endif()

if(OT_APP_CLI_FREERTOS_IPERF)
    add_subdirectory(iperf)
endif()

if(OT_APP_CLI_FREERTOS_PLATFORM)
    add_subdirectory(platform)
endif()

if(OT_APP_CLI_FREERTOS_DEBUG)
    add_subdirectory(debug)
endif()

if(OT_APP_CLI_FREERTOS_LOWPOWER)
    add_subdirectory(lowpower)
endif()

if(OT_APP_CLI_FREERTOS_WIFI)
    add_subdirectory(wifi)
endif()

if(OT_APP_CLI_FREERTOS_LWIP)
    add_subdirectory(lwip)
endif()
