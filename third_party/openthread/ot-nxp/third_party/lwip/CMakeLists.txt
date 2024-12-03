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

if(NOT LWIP_PATH)
    set(LWIP_PATH ${NXP_SDK_ROOT}/middleware/lwip)
endif()

set(LWIP_SDK_SOURCES
    ${LWIP_PATH}/port/sys_arch.c
    ${LWIP_PATH}/port/arch/perf.h

    ${LWIP_PATH}/src/api/api_lib.c
    ${LWIP_PATH}/src/api/api_msg.c
    ${LWIP_PATH}/src/api/err.c
    ${LWIP_PATH}/src/api/if_api.c
    ${LWIP_PATH}/src/api/netbuf.c
    ${LWIP_PATH}/src/api/netdb.c
    ${LWIP_PATH}/src/api/netifapi.c
    ${LWIP_PATH}/src/api/sockets.c
    ${LWIP_PATH}/src/api/tcpip.c

    ${LWIP_PATH}/src/core/altcp.c
    ${LWIP_PATH}/src/core/altcp_alloc.c
    ${LWIP_PATH}/src/core/altcp_tcp.c
    ${LWIP_PATH}/src/core/def.c
    ${LWIP_PATH}/src/core/dns.c
    ${LWIP_PATH}/src/core/inet_chksum.c
    ${LWIP_PATH}/src/core/init.c
    ${LWIP_PATH}/src/core/ip.c
    ${LWIP_PATH}/src/core/mem.c
    ${LWIP_PATH}/src/core/memp.c
    ${LWIP_PATH}/src/core/netif.c
    ${LWIP_PATH}/src/core/pbuf.c
    ${LWIP_PATH}/src/core/raw.c
    ${LWIP_PATH}/src/core/stats.c
    ${LWIP_PATH}/src/core/sys.c
    ${LWIP_PATH}/src/core/tcp.c
    ${LWIP_PATH}/src/core/tcp_in.c
    ${LWIP_PATH}/src/core/tcp_out.c
    ${LWIP_PATH}/src/core/timeouts.c
    ${LWIP_PATH}/src/core/udp.c

    ${LWIP_PATH}/src/core/ipv4/acd.c
    ${LWIP_PATH}/src/core/ipv4/autoip.c
    ${LWIP_PATH}/src/core/ipv4/dhcp.c
    ${LWIP_PATH}/src/core/ipv4/icmp.c
    ${LWIP_PATH}/src/core/ipv4/igmp.c
    ${LWIP_PATH}/src/core/ipv4/ip4.c
    ${LWIP_PATH}/src/core/ipv4/ip4_addr.c
    ${LWIP_PATH}/src/core/ipv4/ip4_frag.c
    ${LWIP_PATH}/src/core/ipv4/etharp.c

    ${LWIP_PATH}/src/core/ipv6/dhcp6.c
    ${LWIP_PATH}/src/core/ipv6/ethip6.c
    ${LWIP_PATH}/src/core/ipv6/icmp6.c
    ${LWIP_PATH}/src/core/ipv6/inet6.c
    ${LWIP_PATH}/src/core/ipv6/ip6.c
    ${LWIP_PATH}/src/core/ipv6/ip6_addr.c
    ${LWIP_PATH}/src/core/ipv6/ip6_frag.c
    ${LWIP_PATH}/src/core/ipv6/mld6.c
    ${LWIP_PATH}/src/core/ipv6/nd6.c

    ${LWIP_PATH}/src/netif/ethernet.c
)

if(OT_NXP_LWIP_ETH)
    list(APPEND LWIP_SDK_SOURCES
        ${LWIP_PATH}/port/enet_ethernetif.c
        ${LWIP_PATH}/port/ethernetif.c
        ${LWIP_PATH}/port/ethernetif_mmac.c
        ${LWIP_PATH}/port/enet_ethernetif_kinetis.c
    )
endif()

if(OT_NXP_LWIP_IPERF)
    list(APPEND LWIP_SDK_SOURCES
        ${LWIP_PATH}/src/apps/lwiperf/lwiperf.c
    )
endif()

add_library(nxp-lwip
    ${LWIP_SDK_SOURCES}
    lwip_tcpip_init_once.c
)

target_compile_definitions(nxp-lwip
    PUBLIC
)

target_include_directories(nxp-lwip
    PUBLIC
    ${LWIP_PATH}
    ${PROJECT_SOURCE_DIR}/src/common/br
    ${LWIP_PATH}/port
    ${LWIP_PATH}/port/arch
    ${LWIP_PATH}/src/include
    ${LWIP_PATH}/src/include/lwip/apps
    ${PROJECT_SOURCE_DIR}/src/common/lwip
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(nxp-lwip
    PRIVATE
    ot-config
    ${NXP_DRIVER_LIB}
)
