/*
 * Copyright (c) 2020, Qorvo Inc
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 * CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 * LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * QORVO INC. SHALL NOT, IN ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 * FOR ANY REASON WHATSOEVER.
 *
 */

/** @file "qorvo_internals.h"
 *
 */

#ifndef _QORVO_INTERNALS_H_
#define _QORVO_INTERNALS_H_

/*
 * Enabled components
 */

#define GP_COMP_GPHAL_BLE
#define GP_COMP_GPHAL_PBM
#define GP_COMP_GPHAL_RADIO
#define GP_COMP_GPHAL_SEC
#define GP_COMP_QVCHIP

/*
 * Components numeric ids
 */

#define GP_COMPONENT_ID_APP                                                             1
#define GP_COMPONENT_ID_ASSERT                                                          29
#define GP_COMPONENT_ID_BASECOMPS                                                       35
#define GP_COMPONENT_ID_BLE                                                             154
#define GP_COMPONENT_ID_BLEACTIVITYMANAGER                                              228
#define GP_COMPONENT_ID_BLEADDRESSRESOLVER                                              214
#define GP_COMPONENT_ID_BLEADVERTISER                                                   215
#define GP_COMPONENT_ID_BLECOMPS                                                        216
#define GP_COMPONENT_ID_BLECONFIG                                                       217
#define GP_COMPONENT_ID_BLECONNECTIONMANAGER                                            75
#define GP_COMPONENT_ID_BLECRYPTO                                                       229
#define GP_COMPONENT_ID_BLEDATACHANNELRXQUEUE                                           218
#define GP_COMPONENT_ID_BLEDATACHANNELTXQUEUE                                           219
#define GP_COMPONENT_ID_BLEDATACOMMON                                                   220
#define GP_COMPONENT_ID_BLEDATARX                                                       221
#define GP_COMPONENT_ID_BLEDATATX                                                       222
#define GP_COMPONENT_ID_BLEINITIATOR                                                    223
#define GP_COMPONENT_ID_BLELLCP                                                         224
#define GP_COMPONENT_ID_BLELLCPFRAMEWORK                                                225
#define GP_COMPONENT_ID_BLELLCPPROCEDURES                                               226
#define GP_COMPONENT_ID_BLEPRESCHED                                                     234
#define GP_COMPONENT_ID_BLESECURITYCOPROCESSOR                                          GP_COMPONENT_ID_BLECRYPTO
#define GP_COMPONENT_ID_BSP                                                             8
#define GP_COMPONENT_ID_COM                                                             10
#define GP_COMPONENT_ID_ECC                                                             192
#define GP_COMPONENT_ID_ENCRYPTION                                                      124
#define GP_COMPONENT_ID_EXTSTORAGE                                                      120
#define GP_COMPONENT_ID_FREERTOS                                                        24
#define GP_COMPONENT_ID_GPHAL                                                           7
#define GP_COMPONENT_ID_HALCORTEXM4                                                     6
#define GP_COMPONENT_ID_HCI                                                             156
#define GP_COMPONENT_ID_JUMPTABLESMATTER_K8E                                            60
#define GP_COMPONENT_ID_JUMPTABLES_K8E                                                  60
#define GP_COMPONENT_ID_LOG                                                             11
#define GP_COMPONENT_ID_NVM                                                             32
#define GP_COMPONENT_ID_PAD                                                             126
#define GP_COMPONENT_ID_PD                                                              104
#define GP_COMPONENT_ID_POOLMEM                                                         106
#define GP_COMPONENT_ID_QORVOBLEHOST                                                    185
#define GP_COMPONENT_ID_QVCHIP                                                          31
#define GP_COMPONENT_ID_QVIO                                                            18
#define GP_COMPONENT_ID_RADIO                                                           204
#define GP_COMPONENT_ID_RANDOM                                                          108
#define GP_COMPONENT_ID_RESET                                                           33
#define GP_COMPONENT_ID_RT_NRT_COMMON                                                   -1
#define GP_COMPONENT_ID_SCHED                                                           9
#define GP_COMPONENT_ID_SILEXCRYPTOSOC                                                  54
#define GP_COMPONENT_ID_STAT                                                            22
#define GP_COMPONENT_ID_UPGRADE                                                         115
#define GP_COMPONENT_ID_UTILS                                                           4
#define GP_COMPONENT_ID_VERSION                                                         129
#define GP_COMPONENT_ID_WMRK                                                            51

/*
 * Component: gpBaseComps
 */

#define GP_BASECOMPS_DIVERSITY_NO_GPCOM_INIT
#define GP_BASECOMPS_DIVERSITY_NO_GPLOG_INIT
#define GP_BASECOMPS_DIVERSITY_NO_GPSCHED_INIT

/*
 * Component: gpBleAddressResolver
 */

#define GP_ROM_PATCHED_Ble_ClearFilterAcceptList
#define GP_ROM_PATCHED_Ble_ManipulateFilterAcceptListAllowedChecker
#define GP_ROM_PATCHED_Ble_ManipulateFilterAcceptListChecker
#define GP_ROM_PATCHED_gpBleAddressResolver_EnableConnectedDevicesInFilterAcceptList
#define GP_ROM_PATCHED_gpBleAddressResolver_UpdateFilterAcceptListEntryState
#define GP_ROM_PATCHED_gpBleAddressResolver_UpdateFilterAcceptListEntryStateBulk
#define GP_ROM_PATCHED_gpBle_LeAddDeviceToFilterAcceptList
#define GP_ROM_PATCHED_gpBle_LeRemoveDeviceFromFilterAcceptList

/*
 * Component: gpBleComps
 */

#define GP_DIVERSITY_BLE_ACL_CONNECTIONS_SUPPORTED
#define GP_DIVERSITY_BLE_CONNECTIONS_SUPPORTED
#define GP_DIVERSITY_BLE_LEGACY_ADVERTISING_FEATURE_PRESENT

/*
 * Component: gpBsp
 */

#define GP_DIVERSITY_QPG6105DK_B01

/*
 * Component: gpCom
 */

#define GP_COM_DIVERSITY_ACTIVATE_TX_CALLBACK
#define GP_COM_DIVERSITY_SERIAL_NO_SYN_NO_CRC
#define GP_COM_DIVERSITY_SERIAL_NO_SYN_SENDTO_ID                                        18
#define GP_COM_DIVERSITY_SERIAL_SYN_DISABLED
#define GP_COM_ZERO_COPY_BLOCK_TRANSFERS

/*
 * Component: gpEncryption
 */

#define GP_ENCRYPTION_DIVERSITY_USE_AES_MMO_HW

/*
 * Component: gpFreeRTOS
 */

#define GP_DIVERSITY_FREERTOS
#define GP_FREERTOS_DIVERSITY_HEAP
#define GP_FREERTOS_DIVERSITY_SLEEP
#define GP_FREERTOS_DIVERSITY_STATIC_ALLOC
#define GP_FREERTOS_DIVERSITY_USE_NON_DEFAULT_CONFIG
#define GP_FREERTOS_DIVERSITY_USE_OWN_CONFIG_HOOKS

/*
 * Component: gphal
 */

#define GP_COMP_GPHAL_ES
#define GP_COMP_GPHAL_ES_ABS_EVENT
#define GP_COMP_GPHAL_ES_EXT_EVENT
#define GP_COMP_GPHAL_ES_EXT_EVENT_WKUP
#define GP_COMP_GPHAL_ES_REL_EVENT
#define GP_DIVERSITY_GPHAL_INTERN
#define GP_DIVERSITY_GPHAL_K8E
#define GP_DIVERSITY_GPHAL_OSCILLATOR_BENCHMARK
#define GP_DIVERSITY_GPHAL_RADIO_MGMT_SUPPORTED
#define GP_DIVERSITY_RT_SYSTEM_ADDR_FROM_LINKERSCRIPT
#define GP_DIVERSITY_RT_SYSTEM_GENERATE
#define GP_DIVERSITY_RT_SYSTEM_MACFILTER_IN_FLASH
#define GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
#define GP_DIVERSITY_RT_SYSTEM_PARTS_IN_ROM
#define GP_HAL_DIVERSITY_INCLUDE_IPC
#define GP_HAL_DIVERSITY_SEC_CRYPTOSOC

/*
 * Component: gpHci
 */

#define GP_HCI_DIVERSITY_HOST_SERVER

/*
 * Component: gpJumpTables_k8e
 */

#define GP_DIVERSITY_JUMPTABLES
#define GP_DIVERSITY_JUMP_TABLE_ASSEMBLY

/*
 * Component: gpLog
 */

#define GP_LOG_DIVERSITY_NO_TIME_NO_COMPONENT_ID
#define GP_LOG_DIVERSITY_VSNPRINTF

/*
 * Component: gpNvm
 */

#define GP_DATA_SECTION_NAME_NVM                                                        gpNvm
#define GP_DATA_SECTION_START_NVM                                                       -0x6000
#define GP_DIVERSITY_NVM
#define GP_NVM_DIVERSITY_ELEMENT_IF
#define GP_NVM_DIVERSITY_ELEMIF_KEYMAP
#define GP_NVM_DIVERSITY_SUBPAGEDFLASH_V2
#define GP_NVM_DIVERSITY_TAG_IF
#define GP_NVM_DIVERSITY_USE_POOLMEM
#define GP_NVM_DIVERSITY_VARIABLE_SETTINGS
#define GP_NVM_DIVERSITY_VARIABLE_SIZE
#define GP_NVM_USE_ASSERT_SAFETY_NET

/*
 * Component: gpPd
 */

#define GP_DIVERSITY_PD_USE_PBM_VARIANT

/*
 * Component: gpSched
 */

#define GP_ROM_PATCHED_Sched_CanGoToSleep
#define GP_ROM_PATCHED_Sched_DumpEvent
#define GP_ROM_PATCHED_Sched_ExecEvent
#define GP_ROM_PATCHED_Sched_FindEventArg
#define GP_ROM_PATCHED_Sched_GetEvent
#define GP_ROM_PATCHED_Sched_GetEventIdlePeriod
#define GP_ROM_PATCHED_Sched_ReleaseEvent
#define GP_ROM_PATCHED_Sched_ReleaseEventBody
#define GP_ROM_PATCHED_Sched_RescheduleEvent
#define GP_ROM_PATCHED_Sched_RescheduleEventAbs
#define GP_ROM_PATCHED_Sched_ScheduleEvent
#define GP_ROM_PATCHED_Sched_ScheduleEventInSeconds
#define GP_ROM_PATCHED_gpSched_Clear
#define GP_ROM_PATCHED_gpSched_DeInit
#define GP_ROM_PATCHED_gpSched_DumpList
#define GP_ROM_PATCHED_gpSched_EventQueueEmpty
#define GP_ROM_PATCHED_gpSched_ExistsEventArg
#define GP_ROM_PATCHED_gpSched_GetRemainingTimeArgInSecAndUs
#define GP_ROM_PATCHED_gpSched_GoToSleep
#define GP_ROM_PATCHED_gpSched_Init
#define GP_ROM_PATCHED_gpSched_Main_Body
#define GP_ROM_PATCHED_gpSched_ScheduleEventArg
#define GP_ROM_PATCHED_gpSched_SetGotoSleepEnable
#define GP_ROM_PATCHED_gpSched_UnscheduleEventArg
#define GP_SCHED_DIVERSITY_RUN_ON_SCHED
#define GP_SCHED_DIVERSITY_SCHEDULE_INSECONDSAPI
#define GP_SCHED_DIVERSITY_SLEEP
#define GP_SCHED_DIVERSITY_USE_ARGS

/*
 * Component: gpUpgrade
 */

#define GP_APP_DIVERSITY_SECURE_BOOTLOADER
#define GP_DATA_SECTION_SIZE_OTA                                                        0x97000
#define GP_DIVERSITY_APP_LICENSE_BASED_BOOT
#define GP_DIVERSITY_FLASH_APP_START_OFFSET                                             0x8000
#define GP_UPGRADE_DIVERSITY_BASE_BOOTLOADER_PATH                                       /builds/GooxPtdA/3/qorvo/wcon/lps_sw/depot/Components/Qorvo/Bootloader/apps/AppBootloader/UserModeBootloader_QPG6105DK_B01_matter.py

/*
 * Component: gpUtils
 */

#define GP_DIVERSITY_UTILS_MATH
#define GP_UTILS_DIVERSITY_CIRCULAR_BUFFER
#define GP_UTILS_DIVERSITY_LINKED_LIST

/*
 * Component: halCortexM4
 */

#define GP_BSP_CONTROL_WDT_TIMER
#define GP_DIVERSITY_ENABLE_DEFAULT_BOD_HANDLING
#define GP_DIVERSITY_ROMUSAGE_FOR_MATTER
#define GP_KX_FLASH_SIZE                                                                1024
#define GP_KX_HEAP_SIZE                                                                 (4 * 1024)
#define GP_KX_SYSRAM_SIZE                                                               32
#define GP_KX_UCRAM_SIZE                                                                96
#define HAL_DIVERSITY_NO_FPU
#define HAL_DIVERSITY_UART
#define HAL_DIVERSITY_UART_RX_BUFFER_CALLBACK
#define QPG6105

/*
 * Component: MatterQorvoGlue
 */

#define GP_DATA_SECTION_SIZE_JTOTA                                                      0x1000

/*
 * Component: qorvoBleHost
 */

#define CORDIO_BLEHOST_DIVERSITY_HCI_INTERNAL
#define CORDIO_BLE_HOST_ATT_SERVER
#define CORDIO_BLE_HOST_EXCLUDE_CORDIOAPPFW
#define CORDIO_BLE_HOST_EXCLUDE_SMPR
#define CORDIO_BLE_HOST_PROFILES_ORIG_SERVPROF
#define WSF_ASSERT_ENABLED                                                              TRUE

/*
 * Component: qvCHIP
 */

#define QVCHIP_DIVERSITY_KVS_HASH_KEYS

/*
 * Component: silexCryptoSoc
 */

#define GP_ROM_PATCHED_ba414e_set_config
#define GP_ROM_PATCHED_generate_ccm_header

/*
 * Other flags
 */

#define GP_DIVERSITY_BOOTLOADER
#define GP_DIVERSITY_CORTEXM4
#define GP_DIVERSITY_KEEP_NRT_FROM_FPGABITMAP
#define GP_DIVERSITY_LOG
#define GP_DIVERSITY_RT_SYSTEM_IN_FLASH_VERSION                                         1
#define GP_GIT_DESCRIBE                                                                 "1ca9f7e6c"
#define GP_GIT_SHA                                                                      1ca9f7e6c47f535afe1234f2176489f563761bc8
#define GP_GIT_SHA_SHORT                                                                1ca9f7e
#define GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS                                              0
#define HAL_DEFAULT_GOTOSLEEP_THRES                                                     30000
#define HAL_DIVERSITY_SLEEP
#define HAL_DIVERSITY_SPI
#define HAL_DIVERSITY_TWI
#define HAL_TWI_CLK_SPEED                                                               100000

#endif //_QORVO_INTERNALS_H_
