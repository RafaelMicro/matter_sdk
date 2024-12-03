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

if(${SDK_TYPE} STREQUAL "GITHUB")
    set(COMPONENTS_PATH ${NXP_SDK_ROOT}/core/components)
else()
    set(COMPONENTS_PATH ${NXP_SDK_ROOT}/components)
endif()

set(MIDDLEWARE_PATH ${NXP_SDK_ROOT}/middleware)

if(NOT WIFI_PATH)
    set(WIFI_PATH ${MIDDLEWARE_PATH}/wifi_nxp)
endif()

if(NOT EDGEFAST_WIFI_PATH)
    set(EDGEFAST_WIFI_PATH ${COMPONENTS_PATH}/edgefast_wifi)
endif()

if(NOT WIFI_BT_MODULE_PATH)
    set(WIFI_BT_MODULE_PATH ${COMPONENTS_PATH}/wifi_bt_module)
endif()

set(WIFI_COMMON_SOURCES
    ${WIFI_PATH}/port/net/net.c
    ${WIFI_PATH}/port/net/wifi_netif.c
    ${WIFI_PATH}/port/os/os.c
    ${WIFI_PATH}/wifidriver/mlan_11ac.c
    ${WIFI_PATH}/wifidriver/mlan_11ax.c
    ${WIFI_PATH}/wifidriver/mlan_11k.c
    ${WIFI_PATH}/wifidriver/mlan_11v.c
    ${WIFI_PATH}/wifidriver/mlan_mbo.c
    ${WIFI_PATH}/wifidriver/mlan_11d.c
    ${WIFI_PATH}/wifidriver/mlan_11h.c
    ${WIFI_PATH}/wifidriver/mlan_11n.c
    ${WIFI_PATH}/wifidriver/mlan_11n_aggr.c
    ${WIFI_PATH}/wifidriver/mlan_11n_rxreorder.c
    ${WIFI_PATH}/wifidriver/mlan_api.c
    ${WIFI_PATH}/wifidriver/mlan_cfp.c
    ${WIFI_PATH}/wifidriver/mlan_cmdevt.c
    ${WIFI_PATH}/wifidriver/mlan_glue.c
    ${WIFI_PATH}/wifidriver/mlan_init.c
    ${WIFI_PATH}/wifidriver/mlan_join.c
    ${WIFI_PATH}/wifidriver/mlan_misc.c
    ${WIFI_PATH}/wifidriver/mlan_scan.c
    ${WIFI_PATH}/wifidriver/mlan_shim.c
    ${WIFI_PATH}/wifidriver/mlan_sta_cmd.c
    ${WIFI_PATH}/wifidriver/mlan_sta_cmdresp.c
    ${WIFI_PATH}/wifidriver/mlan_sta_event.c
    ${WIFI_PATH}/wifidriver/mlan_sta_ioctl.c
    ${WIFI_PATH}/wifidriver/mlan_sta_rx.c
    ${WIFI_PATH}/wifidriver/mlan_txrx.c
    ${WIFI_PATH}/wifidriver/mlan_uap_cmdevent.c
    ${WIFI_PATH}/wifidriver/mlan_uap_ioctl.c
    ${WIFI_PATH}/wifidriver/mlan_wmm.c
    ${WIFI_PATH}/wifidriver/mlan_action.c
    ${WIFI_PATH}/wifidriver/wifi-debug.c
    ${WIFI_PATH}/wifidriver/wifi-mem.c
    ${WIFI_PATH}/wifidriver/wifi-uap.c
    ${WIFI_PATH}/wifidriver/wifi.c
    ${WIFI_PATH}/wifidriver/wifi_pwrmgr.c
    ${WIFI_PATH}/wlcmgr/wlan.c
    ${WIFI_PATH}/wlcmgr/wlan_basic_cli.c
    ${WIFI_PATH}/wlcmgr/wlan_enhanced_tests.c
    ${WIFI_PATH}/wlcmgr/wlan_tests.c
    ${WIFI_PATH}/wlcmgr/wlan_test_mode_tests.c
    ${WIFI_PATH}/wlcmgr/wlan_txpwrlimit_cfg.c
    ${EDGEFAST_WIFI_PATH}/source/wpl_nxp.c
)

add_subdirectory(wifi_${OT_NXP_PLATFORM})

add_library(wifi
    ${WIFI_COMMON_SOURCES}
)

target_compile_definitions(wifi
    PRIVATE
    vApplicationIdleHook=vApplicationIdleHook_wifi # hack to prevent definition of multiple idle tasks (OT has one)
    tcpip_init=tcpip_init_once
)

target_include_directories(wifi
    PUBLIC
    ${WIFI_PATH}/port/os
    ${WIFI_PATH}/port/net
    ${WIFI_PATH}/port/arch
    ${WIFI_PATH}/incl
    ${WIFI_PATH}/incl/port/os
    ${WIFI_PATH}/incl/port/net
    ${WIFI_PATH}/incl/wifidriver
    ${WIFI_PATH}/incl/wlcmgr
    ${WIFI_PATH}/wifidriver/incl
    ${WIFI_PATH}/wifidriver
    ${WIFI_PATH}/wifi_bt_firmware
    ${PROJECT_SOURCE_DIR}/third_party/wifi
    ${EDGEFAST_WIFI_PATH}/include
    ${WIFI_BT_MODULE_PATH}/template
    PRIVATE
    ${WIFI_BT_MODULE_PATH}/AzureWave/tx_pwr_limits
    ${WIFI_BT_MODULE_PATH}/Murata/tx_pwr_limits
)
target_link_libraries(wifi
    PUBLIC
    nxp-lwip
    PRIVATE
    ot-config
    ${NXP_DRIVER_LIB}
    ${NXP_BOARD_LIB}
)

target_compile_options(wifi
    PUBLIC
    -include ${PROJECT_SOURCE_DIR}/third_party/wifi/wifi_${OT_NXP_PLATFORM}/app_wifi_config.h
)
