#
#  Copyright (c) 2021, The OpenThread Authors.
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

if (DEFINED ENV{QORVO_OT_MBEDTLS_ALT_DIR})
    set(QORVO_MBEDTLS_ALT_DIR "$ENV{QORVO_OT_MBEDTLS_ALT_DIR}")
else()
    set(QORVO_MBEDTLS_ALT_DIR "${QORVO_MBEDTLS_SDK_DIR}/../../mbedtls_alt_3.3.0/")
endif()

message(STATUS "Adding static lib `qorvo-mbedtls` from ${MBEDCRYPTO}")
add_library(qorvo-mbedtls STATIC IMPORTED GLOBAL)

set(INCLUDE_DIRS "${QORVO_MBEDTLS_SDK_DIR}/include"
                 "${QORVO_MBEDTLS_SDK_DIR}/include/mbedtls"
                 "${QORVO_MBEDTLS_SDK_DIR}/include/psa"
                 "${QORVO_MBEDTLS_ALT_DIR}"
                 "${QORVO_MBEDTLS_ALT_CONFIG}"
)
set_target_properties(qorvo-mbedtls
    PROPERTIES
        C_STANDARD 99
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        IMPORTED_LOCATION "${MBEDCRYPTO}"
        INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIRS}"
)

# target_include_directories(qorvo-mbedtls
#     INTERFACE
#         ${QORVO_MBEDTLS_SDK_DIR}/include
#         ${QORVO_MBEDTLS_SDK_DIR}/include/mbedtls
#         ${QORVO_MBEDTLS_SDK_DIR}/include/psa
#         ${QORVO_MBEDTLS_ALT_DIR}
#         ${QORVO_MBEDTLS_ALT_CONFIG}
# )

