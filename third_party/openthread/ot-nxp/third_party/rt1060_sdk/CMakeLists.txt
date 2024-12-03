#
# Copyright (c) 2021-2023, The OpenThread Authors.
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

set(EVK_RT1060_BOARD "evkbmimxrt1060" CACHE STRING "Default evk board used")

message(STATUS " absolut path  ${NXP_SDK_ROOT}")

if(${SDK_TYPE} STREQUAL "GITHUB")
    message("SDK_GITHUB driver path chosen")
    set(MIDDLEWARE_PATH ${NXP_SDK_ROOT}/middleware)
    set(NXP_SDK_ROOT ${NXP_SDK_ROOT}/core)
    set(DRIVER_FILES_PATH ${NXP_SDK_ROOT}/drivers)
    set(DRIVER_LPUART_FILES_PATH ${DRIVER_FILES_PATH}/lpuart)
    set(DRIVER_LPSPI_FILES_PATH ${DRIVER_FILES_PATH}/lpspi)
    set(DRIVER_LPI2C_FILES_PATH ${DRIVER_FILES_PATH}/lpi2c)
    set(DRIVER_EDMA_FILES_PATH ${DRIVER_FILES_PATH}/edma)
    set(DRIVER_DMAMUX_FILES_PATH ${DRIVER_FILES_PATH}/dmamux)
    set(DRIVER_PIT_FILES_PATH ${DRIVER_FILES_PATH}/pit)
    set(DRIVER_TRNG_FILES_PATH ${DRIVER_FILES_PATH}/trng)
    set(DRIVER_FLEXSPI_FILES_PATH ${DRIVER_FILES_PATH}/flexspi)
    set(DRIVER_IGPIO_FILES_PATH ${DRIVER_FILES_PATH}/igpio)
    set(DRIVER_DCP_FILES_PATH ${DRIVER_FILES_PATH}/dcp)
    set(DRIVER_FSL_CACHE_FILES_PATH ${DRIVER_FILES_PATH}/cache/armv7-m7)
    set(DRIVER_COMMON_FILES_PATH ${DRIVER_FILES_PATH}/common)
    set(DRIVER_ENET_FILES_PATH ${DRIVER_FILES_PATH}/enet)
    set(DRIVER_USDHC_FILES_PATH ${DRIVER_FILES_PATH}/usdhc)
    set(DRIVER_DEVICE_FILES_PATH ${NXP_SDK_ROOT}/devices/MIMXRT1062/drivers)
    set(UTILITIES_FILES_PATH ${NXP_SDK_ROOT}/utilities)
    set(UTILITIES_DEVICE_FILES_PATH ${NXP_SDK_ROOT}/devices/MIMXRT1062/utilities)
    set(ASSERT_FILES_PATH ${UTILITIES_FILES_PATH}/assert)
    set(DEBUG_CONSOLE_FILES_PATH ${UTILITIES_FILES_PATH}/debug_console_lite)
    set(STR_FILES_PATH ${UTILITIES_FILES_PATH}/debug_console/str)
    set(FSL_FLEXSPI_NOR_BOOT_FILES_PATH ${NXP_SDK_ROOT}/devices/MIMXRT1062/xip)
elseif(${SDK_TYPE} STREQUAL "PACKAGE")
    message("SDK_PACKAGE driver path chosen")
    set(DRIVER_FILES_PATH ${NXP_SDK_ROOT}/devices/MIMXRT1062/drivers)
    set(DRIVER_LPUART_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_PIT_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_TRNG_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_FLEXSPI_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_IGPIO_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_DCP_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_FSL_CACHE_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_COMMON_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_DEVICE_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_EDMA_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_DMAMUX_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_ENET_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_USDHC_FILES_PATH ${DRIVER_FILES_PATH})
    set(DEBUG_CONSOLE_FILES_PATH ${NXP_SDK_ROOT}/devices/MIMXRT1062/utilities/debug_console)
    set(ASSERT_FILES_PATH ${NXP_SDK_ROOT}/devices/MIMXRT1062/utilities/assert)
    set(STR_FILES_PATH ${NXP_SDK_ROOT}/devices/MIMXRT1062/utilities/str)
    set(FSL_FLEXSPI_NOR_BOOT_FILES_PATH ${NXP_SDK_ROOT}/devices/MIMXRT1062/xip)
elseif(${SDK_TYPE} STREQUAL "INTERNAL")
    message("SDK_DEV driver path chosen")
    set(DRIVER_LPUART_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/lpuart)
    set(DRIVER_PIT_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/pit)
    set(DRIVER_TRNG_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/trng)
    set(DRIVER_FLEXSPI_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/flexspi)
    set(DRIVER_IGPIO_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/igpio)
    set(DRIVER_DCP_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/dcp)
    set(DRIVER_FSL_CACHE_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/cache/armv7-m7)
    set(DRIVER_COMMON_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/common)
    set(DRIVER_DEVICE_FILES_PATH ${NXP_SDK_ROOT}/devices/MIMXRT1062/drivers)
    set(DRIVER_EDMA_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/edma)
    set(DRIVER_DMAMUX_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/dmamux)
    set(DRIVER_ENET_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/enet)
    set(DRIVER_USDHC_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/usdhc)
    set(DEBUG_CONSOLE_FILES_PATH ${NXP_SDK_ROOT}/platform/utilities/debug_console)
    set(ASSERT_FILES_PATH ${NXP_SDK_ROOT}/platform/utilities/assert)
    set(STR_FILES_PATH ${NXP_SDK_ROOT}/platform/utilities/str)
    set(FSL_FLEXSPI_NOR_BOOT_FILES_PATH ${NXP_SDK_ROOT}/devices/MIMXRT1052/xip)
else()
    message(FATAL_ERROR "Unknown SDK_TYPE")
endif()

list(APPEND SDK_SOURCES

    # startup files
    ${NXP_SDK_ROOT}/devices/MIMXRT1062/gcc/startup_MIMXRT1062.S

    # ${NXP_SDK_ROOT}/devices/MIMXRT1062/mcuxpresso/startup_mimxrt1062.c
    ${NXP_SDK_ROOT}/devices/MIMXRT1062/system_MIMXRT1062.c

    # other SDK files
    ${DRIVER_LPUART_FILES_PATH}/fsl_lpuart.c
    ${DRIVER_LPUART_FILES_PATH}/fsl_lpuart_edma.c
    ${DRIVER_LPSPI_FILES_PATH}/fsl_lpspi.c
    ${DRIVER_LPI2C_FILES_PATH}/fsl_lpi2c.c
    ${DRIVER_IGPIO_FILES_PATH}/fsl_gpio.c
    ${DRIVER_PIT_FILES_PATH}/fsl_pit.c
    ${DRIVER_TRNG_FILES_PATH}/fsl_trng.c
    ${DRIVER_FLEXSPI_FILES_PATH}/fsl_flexspi.c
    ${DRIVER_COMMON_FILES_PATH}/fsl_common_arm.c
    ${DRIVER_DEVICE_FILES_PATH}/fsl_clock.c
    ${DRIVER_DCP_FILES_PATH}/fsl_dcp.c
    ${DRIVER_FSL_CACHE_FILES_PATH}/fsl_cache.c
    ${DRIVER_EDMA_FILES_PATH}/fsl_edma.c
    ${DRIVER_DMAMUX_FILES_PATH}/fsl_dmamux.c
    ${DEBUG_CONSOLE_FILES_PATH}/fsl_debug_console.c
    ${STR_FILES_PATH}/fsl_str.c
    ${ASSERT_FILES_PATH}/fsl_assert.c
    ${DRIVER_ENET_FILES_PATH}/fsl_enet.c
    ${DRIVER_USDHC_FILES_PATH}/fsl_usdhc.c

    # XIP
    ${FSL_FLEXSPI_NOR_BOOT_FILES_PATH}/fsl_flexspi_nor_boot.c
    ${NXP_SDK_ROOT}/boards/${EVK_RT1060_BOARD}/xip/${EVK_RT1060_BOARD}_flexspi_nor_config.c

    # SDK components
    ${NXP_SDK_ROOT}/components/serial_manager/fsl_component_serial_manager.c
    ${NXP_SDK_ROOT}/components/serial_manager/fsl_component_serial_port_uart.c
    ${NXP_SDK_ROOT}/components/uart/fsl_adapter_lpuart.c
    ${NXP_SDK_ROOT}/components/spi/fsl_adapter_lpspi.c
    ${NXP_SDK_ROOT}/components/i2c/fsl_adapter_lpi2c.c
    ${NXP_SDK_ROOT}/components/lists/fsl_component_generic_list.c
    ${NXP_SDK_ROOT}/components/osa/fsl_os_abstraction_free_rtos.c
    ${NXP_SDK_ROOT}/components/log/fsl_component_log.c
    ${NXP_SDK_ROOT}/components/log/fsl_component_log_backend_debugconsole.c
    ${NXP_SDK_ROOT}/components/timer_manager/fsl_component_timer_manager.c
    ${NXP_SDK_ROOT}/components/timer/fsl_adapter_pit.c
    ${NXP_SDK_ROOT}/components/time_stamp/fsl_adapter_pit_time_stamp.c
    ${NXP_SDK_ROOT}/components/rng/fsl_adapter_trng.c
    ${NXP_SDK_ROOT}/components/internal_flash/fsl_adapter_flexspi_nor_flash.c
    ${NXP_SDK_ROOT}/components/gpio/fsl_adapter_igpio.c
    ${NXP_SDK_ROOT}/components/crc/fsl_adapter_software_crc.c
    ${NXP_SDK_ROOT}/components/phy/device/phyksz8081/fsl_phyksz8081.c
    ${NXP_SDK_ROOT}/components/silicon_id/fsl_silicon_id.c
    ${NXP_SDK_ROOT}/components/silicon_id/socs/rt10xx/fsl_silicon_id_soc.c

    # File system
    ${MIDDLEWARE_PATH}/littlefs/lfs.c
    ${MIDDLEWARE_PATH}/littlefs/lfs_util.c
    ${NXP_SDK_ROOT}/components/flash/mflash/mimxrt1062/mflash_drv.c

    # sdmmc
    ${MIDDLEWARE_PATH}/sdmmc/sdio/fsl_sdio.c
    ${MIDDLEWARE_PATH}/sdmmc/common/fsl_sdmmc_common.c
    ${MIDDLEWARE_PATH}/sdmmc/osa/fsl_sdmmc_osa.c
    ${MIDDLEWARE_PATH}/sdmmc/host/usdhc/non_blocking/fsl_sdmmc_host.c

    # sdk hook
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk_hook/mbedtls/ksdk_mbedtls_hook.c
)

list(APPEND SDK_INCLUDES

    # CMSIS includes
    ${NXP_SDK_ROOT}/CMSIS/Core/Include

    # other SDK files
    ${NXP_SDK_ROOT}/devices/MIMXRT1062/drivers
    ${NXP_SDK_ROOT}/devices/MIMXRT1062
    ${DRIVER_COMMON_FILES_PATH}
    ${DRIVER_LPUART_FILES_PATH}
    ${DRIVER_LPSPI_FILES_PATH}
    ${DRIVER_LPI2C_FILES_PATH}
    ${DRIVER_IGPIO_FILES_PATH}
    ${DRIVER_DCP_FILES_PATH}
    ${DRIVER_FSL_CACHE_FILES_PATH}
    ${DRIVER_PIT_FILES_PATH}
    ${DRIVER_TRNG_FILES_PATH}
    ${DRIVER_FLEXSPI_FILES_PATH}
    ${DRIVER_EDMA_FILES_PATH}
    ${DRIVER_DMAMUX_FILES_PATH}
    ${DEBUG_CONSOLE_FILES_PATH}
    ${UTILITIES_FILES_PATH}/debug_console/debug_console
    ${STR_FILES_PATH}
    ${DRIVER_ENET_FILES_PATH}
    ${DRIVER_USDHC_FILES_PATH}

    # XIP
    ${NXP_SDK_ROOT}/devices/MIMXRT1052/xip
    ${NXP_SDK_ROOT}/boards/${EVK_RT1060_BOARD}/xip

    # SDK components
    ${NXP_SDK_ROOT}/components/uart
    ${NXP_SDK_ROOT}/components/serial_manager
    ${NXP_SDK_ROOT}/components/lists
    ${NXP_SDK_ROOT}/components/osa
    ${NXP_SDK_ROOT}/components/common_task
    ${NXP_SDK_ROOT}/components/log
    ${NXP_SDK_ROOT}/components/timer_manager
    ${NXP_SDK_ROOT}/components/timer
    ${NXP_SDK_ROOT}/components/time_stamp/
    ${NXP_SDK_ROOT}/components/rng
    ${NXP_SDK_ROOT}/components/spi
    ${NXP_SDK_ROOT}/components/i2c
    ${NXP_SDK_ROOT}/components/internal_flash
    ${NXP_SDK_ROOT}/components/gpio
    ${NXP_SDK_ROOT}/components/crc
    ${NXP_SDK_ROOT}/components/phy/
    ${NXP_SDK_ROOT}/components/phy/device/phyksz8081
    ${NXP_SDK_ROOT}/components/silicon_id
    ${NXP_SDK_ROOT}/components/silicon_id/socs

    # File system
    ${MIDDLEWARE_PATH}/littlefs
    ${NXP_SDK_ROOT}/components/flash/mflash
    ${NXP_SDK_ROOT}/components/flash/mflash/mimxrt1062

    # sdmmc
    ${MIDDLEWARE_PATH}/sdmmc/sdio/
    ${MIDDLEWARE_PATH}/sdmmc/common/
    ${MIDDLEWARE_PATH}/sdmmc/host/usdhc
    ${MIDDLEWARE_PATH}/sdmmc/osa
)

list(APPEND SDK_COMPILE_DEFINITIONS
    -D__STARTUP_CLEAR_BSS
    -D__STARTUP_INITIALIZE_NONCACHEDATA
    -D__STARTUP_INITIALIZE_RAMFUNCTION

    # -DSERIAL_MANAGER_NON_BLOCKING_MODE=1
    -DSERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY=1
    -DSDK_OS_FREE_RTOS
    -DUSE_RTOS
    -DOSA_USED
    -DFSL_OSA_MAIN_FUNC_ENABLE=0
    -DFSL_RTOS_FREE_RTOS
    -DXIP_EXTERNAL_FLASH=1
    -DXIP_BOOT_HEADER_ENABLE=1
    -DCPU_MIMXRT1062DVL6A
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1
    -DSDK_COMPONENT_INTEGRATION=1
    -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING
    -DHAL_UART_ADAPTER_FIFO=1
    -DSDK_DEBUGCONSOLE_UART=1
    -DSERIAL_PORT_TYPE_UART=1
    -DCONFIG_ARM=1
    -DSERIAL_MANAGER_RING_BUFFER_FLOWCONTROL=1
    -DSERIAL_MANAGER_TASK_STACK_SIZE=4048

    # LittleFS configs
    LFS_CONFIG=fwk_lfs_config.h
    LFS_THREADSAFE=1
    -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE
)

if("${OT_NXP_TRANSCEIVER}" STREQUAL "iwx12")
    if(OT_NXP_LWIP_WIFI)
    # Do nothing transceiver file will be added in the wifi lib
    else()
        # If Wi-Fi is disabled provide the path to the app_config file to have all required flags for transceivers
        list(APPEND SDK_TARGET_COMPILE_OPTIONS -include ${PROJECT_SOURCE_DIR}/third_party/wifi/wifi_${OT_NXP_PLATFORM}/app_wifi_config.h)
    endif()

    list(APPEND SDK_INCLUDES
        ${DRIVER_USDHC_FILES_PATH}
        ${DRIVER_SDHC_FILES_PATH}
        ${MIDDLEWARE_PATH}/wifi_nxp/wifidriver
        ${MIDDLEWARE_PATH}/wifi_nxp/wifi_bt_firmware/nw61x
        ${MIDDLEWARE_PATH}/sdmmc/common
        ${MIDDLEWARE_PATH}/sdmmc/sdio
        ${MIDDLEWARE_PATH}/sdmmc/host/usdhc
        ${MIDDLEWARE_PATH}/sdmmc/osa
        ${MIDDLEWARE_PATH}/wifi_nxp/incl/wifidriver
        ${MIDDLEWARE_PATH}/wifi_nxp/incl
        ${MIDDLEWARE_PATH}/wifi_nxp/incl/wlcmgr
        ${MIDDLEWARE_PATH}/wifi_nxp/incl/port/os
        ${MIDDLEWARE_PATH}/wifi_nxp/wifidriver
        ${MIDDLEWARE_PATH}/wifi_nxp/sdio_nxp_abs/incl
        ${MIDDLEWARE_PATH}/wifi_nxp/wifidriver/incl
        ${MIDDLEWARE_PATH}/wifi_nxp/fwdnld_intf_abs
        ${MIDDLEWARE_PATH}/wifi_nxp/sdio_nxp_abs/incl
        ${MIDDLEWARE_PATH}/wifi_nxp/firmware_dnld
        ${NXP_SDK_ROOT}/components/wifi_bt_module/template
        ${NXP_SDK_ROOT}/components/wifi_bt_module/incl
    )
    list(APPEND SDK_SOURCES
        ${MIDDLEWARE_PATH}/wifi_nxp/firmware_dnld/firmware_dnld.c
        ${MIDDLEWARE_PATH}/wifi_nxp/fwdnld_intf_abs/fwdnld_intf_abs.c
        ${MIDDLEWARE_PATH}/wifi_nxp/sdio_nxp_abs/fwdnld_sdio.c
        ${MIDDLEWARE_PATH}/wifi_nxp/sdio_nxp_abs/mlan_sdio.c
        ${MIDDLEWARE_PATH}/wifi_nxp/wifidriver/sdio.c
        ${MIDDLEWARE_PATH}/wifi_nxp/wifidriver/wifi-sdio.c
        ${DRIVER_USDHC_FILES_PATH}/fsl_usdhc.c
        ${NXP_SDK_ROOT}/components/wifi_bt_module/template/wifi_bt_config.c
    )
    list(APPEND SDK_COMPILE_DEFINITIONS
        -DNOT_DEFINE_DEFAULT_WIFI_MODULE
        -D${OT_NXP_TRANSCEIVER_INTERFACE_DETAIL}
    )
elseif("${OT_NXP_TRANSCEIVER}" STREQUAL "k32w0")
    list(APPEND SDK_COMPILE_DEFINITIONS
        -DHAL_UART_DMA_ENABLE=1 # OTW requires the DMA to be enabled
    )
endif()

add_library(${NXP_DRIVER_LIB}
    ${SDK_SOURCES}
)

target_include_directories(${NXP_DRIVER_LIB}
    PUBLIC
    ${SDK_INCLUDES}
)

target_compile_definitions(${NXP_DRIVER_LIB}
    PUBLIC
    ${SDK_COMPILE_DEFINITIONS}
)

target_link_libraries(${NXP_DRIVER_LIB}
    PUBLIC
    freertos_kernel
    ${NXP_BOARD_LIB}

    # Dependency to little_fs config files
    connfwk-config
    connfwk-platform-${OT_NXP_PLATFORM_FAMILY}
    connfwk-platform-${OT_NXP_PLATFORM}
)

target_link_options(${NXP_DRIVER_LIB}
    PUBLIC

    # mbedtls_hardware_poll is wrapped to another definition
    -Wl,--wrap=mbedtls_hardware_poll
)

target_compile_options(${NXP_DRIVER_LIB}
    PUBLIC
    ${SDK_TARGET_COMPILE_OPTIONS}
    PRIVATE
    ${OT_CFLAGS}
    -Wno-unknown-pragmas
    -Wno-sign-compare
    -Wno-unused-function
    -Wno-unused-parameter
    -Wno-empty-body
)

set(OT_NXP_MBEDTLS_CONFIG_FILE "\"ot-nxp-mbedtls-config.h\"" PARENT_SCOPE)
set(OT_NXP_MBEDTLS_PORT ksdk PARENT_SCOPE)
