#
# Copyright (c) 2023, NXP
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

# Each platform must set Connectivity Framework CMake config flags in order to build its various libraries
# CONNFWK_PLATFORM is mandatory to select the correct platform
# To enable the build of some library, its flag must be set, for example:
# set(CONNFWK_FLIB ON) -> will build the FunctionLib module as a lib
# set(CONNFWK_PWR ON) -> will build the LowPower module as a lib
# and so on ...

# Tell the framework which SDK lib to link to, to retrieve dependencies (include paths, flags)
list(APPEND CONNFWK_MCUX_SDK_LIB_EXTERNAL ${NXP_DRIVER_LIB})

if(OT_NXP_LWIP_WIFI)
    list(APPEND CONNFWK_MCUX_SDK_LIB_EXTERNAL wifi)
endif()

# Tell the framework which board lib to link to, to retrieve dependencies (include paths, flags)
set(CONNFWK_BOARD_LIB_EXTERNAL ${NXP_BOARD_LIB})

# Add the Framework as a subdirectory
add_subdirectory(${NXP_SDK_ROOT}/middleware/wireless/framework ${PROJECT_BINARY_DIR}/framework)

target_compile_definitions(connfwk-config
    INTERFACE
    ${CONNFWK_COMPILE_DEFINITIONS}
)
