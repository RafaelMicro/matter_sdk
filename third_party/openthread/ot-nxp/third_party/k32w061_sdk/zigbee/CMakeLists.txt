#
#  Copyright (c) 2022, The OpenThread Authors.
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


set(TOOL_BASE_DIR "$ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/tools")


set(ENV{PDUMCONFIG} "${TOOL_BASE_DIR}/PDUMConfig/Source/PDUMConfig")
set(ENV{ZPSCONFIG} "${TOOL_BASE_DIR}/ZPSConfig/Source/ZPSConfig")


set(ENV{ZPS_NWK_LIB} "$ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/libs/libZPSNWK_ZED.a")
set(ENV{ZPS_APL_LIB} "$ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/libs/libZPSAPL.a")

list(APPEND SDK_ZB_LIBS
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/framework/PDUM/Library/libPDUM.a
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/framework/PWRM/Library/libPWRM.a

        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/libs/libZPSGP_ZED.a
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/libs/libZPSIPAN_ZED.a
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/libs/libZPSAPL.a
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/libs/libZPSNWK_ZED.a
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/libs/libZPSTSV.a
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/libs/libZPSMAC_Mini_SOC.a
)

add_library(nxp-k32w061-zigbee
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/platform/leds.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/platform/buttons.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/platform/timer.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/platform/wdog.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform/K32W0/platform/crypto.c

        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZigbeeCommon/Source/ZTimer.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZigbeeCommon/Source/ZQueue.c

        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_common.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_timer.c

        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Devices/ZHA/Generic/Source/base_device.c

        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_event.c

        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_attribute.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_buffer.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/dlist.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_clusterCommand.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_command.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_common.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_common.h
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_configureReportingCommandHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_configureReportingCommandSend.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_configureReportingResponseHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_CustomCommandReceive.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_CustomCommandSend.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_defaultResponse.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_discoverAttributesExtendedRequestHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_discoverAttributesExtendedRequestSend.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_discoverAttributesExtendedResponseHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_discoverAttributesRequestHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_discoverAttributesRequestSend.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_discoverAttributesResponseHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_discoverCommandsRequestHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_discoverCommandsRequestSend.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_discoverCommandsResponseHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_event.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_heap.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_internal.h
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_library_options.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_PDUbufferReadWrite.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_PDUbufferReadWriteString.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_readAttributesRequestHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_readAttributesRequestSend.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_readAttributesResponseHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_readReportingConfigurationCommandHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_readReportingConfigurationCommandSend.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_readReportingConfigurationResponseHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_reportManager.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_reportMaths.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_reportScheduler.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_reportStringHandling.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_reportStructure.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_search.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_timer.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_transmit.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_WriteAttributesRequestHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_WriteAttributesRequestSend.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl_WriteAttributesResponseHandle.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source/zcl.c

        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZigbeeCommon/Source/app_zps_link_keys.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZigbeeCommon/Source/appZdpExtraction.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZigbeeCommon/Source/appZpsBeaconHandler.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZigbeeCommon/Source/appZpsExtendedDebug.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZigbeeCommon/Source/ZQueue.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZigbeeCommon/Source/ZTimer.c

        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/OTA/Source/OTA_client.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/OTA/Source/OTA_ClientUpgradeManager.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/OTA/Source/OTA_common.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/OTA/Source/OTA_CustomCommandHandler.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/OTA/Source/OTA_CustomReceiveCommands.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/OTA/Source/OTA.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/OTA/Source/OTA_server.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/OTA/Source/OTA_ServerUpgradeManager.c

        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/framework/OtaSupport/Source/OtaSupport.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/framework/OtaSupport/Source/OtaUtils.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/framework/Flash/External/Source/Eeprom_InternalFlash.c

        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/Basic.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/BasicClientCommands.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/BasicCommandHandler.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/Groups.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/GroupsClientCommands.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/GroupsCommandHandler.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/GroupsServerCommands.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/GroupsTableManager.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/Identify.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/IdentifyClientCommands.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/IdentifyCommandHandler.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/IdentifyServerCommands.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/OnOff.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/OnOffCommandHandler.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source/OnOffCommands.c

        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/framework/Messaging/Source/Messaging.c

        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/Common/bdb_fr.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/Common/bdb_start.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/Common/bdb_state_machine.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/FindAndBind/bdb_fb_common.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/FindAndBind/bdb_fb_initiator.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/FindAndBind/bdb_fb_target.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/FindAndBind/bdb_fb_common.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/FindAndBind/bdb_fb_initiator.c
        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/FindAndBind/bdb_fb_target.c

        $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/NwkSteering/bdb_ns.c

)

target_compile_definitions(nxp-k32w061-zigbee
    PUBLIC
        SDK_BOARD=k32w061dk6
        SDK_DEVICE_FAMILY=K32W061
        LITTLE_ENDIAN_PROCESSOR
        ZIGBEE_USE_FRAMEWORK
        JENNIC_CHIP_FAMILY_JN518x
        JENNIC_CHIP_FAMILY=JN518x
        JN518x
        FSL_OSA_BM_TIMER_CONFIG=FSL_OSA_BM_TIMER_SYSTICK
        gFlashEraseDuringWrite=0
        gInvalidateHeaderLength=336
        gEepromType_d=gEepromDevice_InternalFlash_c
        gOtaVerifyWrite_d=0
        gEePromParams_BufferSize_c=512
        gEepromParams_bufferedWrite_c=1
        gEepromParams_CurrentOffset=0
    )

target_include_directories(nxp-k32w061-zigbee
    PUBLIC
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/platform

            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZigbeeCommon/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/OTA/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/framework/PDUM/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZPSAPL/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZPSMAC/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/ieee-802.15.4/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/ieee-802.15.4/mMac/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZPSNWK/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZPSTSV/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Include/
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Devices/ZHA/Generic/Include/
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Include/
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/framework/PWRM/Include

            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/OutOfBand

            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/framework/OtaSupport/Interface
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/framework/Flash/External/Interface

            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZPSAPL/Include

            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZPSNWK/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZPSTSV/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbbee/ZPSMAC/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZigbeeCommon/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/Common
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/FindAndBind
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/NwkFormation
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/NwkSteering
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/OutOfBand
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/BDB/Source/TouchLink
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCIF/Source
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Devices/ZHA/Generic/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Source
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/OTA/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/General/Include
            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/zigbee/ZCL/Clusters/Commissioning/Include

            $ENV{NXP_K32W0_SDK_ROOT}/middleware/wireless/framework/PWRM/Include
)

target_link_libraries(nxp-k32w061-zigbee PUBLIC ${NXP_DRIVER_LIB} ${SDK_ZB_LIBS})
