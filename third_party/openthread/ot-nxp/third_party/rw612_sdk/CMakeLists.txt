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

cmake_policy(SET CMP0079 NEW)

if(${SDK_TYPE} STREQUAL "GITHUB")
    message("SDK_GITHUB driver path chosen")
    set(MIDDLEWARE_PATH ${NXP_SDK_ROOT}/middleware)
    set(NXP_SDK_ROOT ${NXP_SDK_ROOT}/core)
    set(DRIVER_CACHE_FILES_PATH ${NXP_SDK_ROOT}/drivers/cache/cache64)
    set(DRIVER_FLEXCOMM_FILES_PATH ${NXP_SDK_ROOT}/drivers/flexcomm)
    set(DRIVER_USART_FILES_PATH ${DRIVER_FLEXCOMM_FILES_PATH}/usart)
    set(DRIVER_FLEXSPI_FILES_PATH ${NXP_SDK_ROOT}/drivers/flexspi)
    set(DRIVER_LPC_GPIO_FILES_PATH ${NXP_SDK_ROOT}/drivers/lpc_gpio)
    set(DRIVER_IMU_FILES_PATH ${NXP_SDK_ROOT}/drivers/imu)
    set(DRIVER_GDMA_FILES_PATH ${NXP_SDK_ROOT}/drivers/gdma)
    set(DRIVER_TRNG_FILES_PATH ${NXP_SDK_ROOT}/drivers/trng)
    set(DRIVER_COMMON_FILES_PATH ${NXP_SDK_ROOT}/drivers/common)
    set(DRIVER_CTIMER_FILES_PATH ${NXP_SDK_ROOT}/drivers/ostimer)
    set(DRIVER_RTC_FILES_PATH ${NXP_SDK_ROOT}/drivers/lpc_rtc)
    set(DRIVER_DEVICE_FILES_PATH ${NXP_SDK_ROOT}/devices/RW612/drivers)
    set(DRIVER_ENET_FILES_PATH ${NXP_SDK_ROOT}/drivers/enet)
    set(DEBUG_CONSOLE_FILES_PATH ${NXP_SDK_ROOT}/utilities/debug_console)
    set(STR_FILES_PATH ${NXP_SDK_ROOT}/utilities/str)
    set(FORMAT_FILES_PATH ${NXP_SDK_ROOT}/utilities/misc_utilities)
elseif(${SDK_TYPE} STREQUAL "PACKAGE")
    set(MIDDLEWARE_PATH ${NXP_SDK_ROOT}/middleware)
    set(DRIVER_FILES_PATH ${NXP_SDK_ROOT}/devices/RW612/drivers)
    set(DRIVER_CACHE_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_FLEXCOMM_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_USART_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_FLEXSPI_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_LPC_GPIO_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_IMU_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_GDMA_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_TRNG_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_COMMON_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_DEVICE_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_MRT_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_CTIMER_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_RTC_FILES_PATH ${DRIVER_FILES_PATH})
    set(DRIVER_ENET_FILES_PATH ${DRIVER_FILES_PATH})
    set(DEBUG_CONSOLE_FILES_PATH ${NXP_SDK_ROOT}/devices/RW612/utilities/debug_console)
    set(STR_FILES_PATH ${NXP_SDK_ROOT}/devices/RW612/utilities/str)
    set(FORMAT_FILES_PATH ${NXP_SDK_ROOT}/devices/RW612/utilities/format)
elseif(${SDK_TYPE} STREQUAL "INTERNAL")
    set(MIDDLEWARE_PATH ${NXP_SDK_ROOT}/middleware)
    set(DRIVER_CACHE_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/cache/cache64)
    set(DRIVER_FLEXCOMM_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/flexcomm)
    set(DRIVER_USART_FILES_PATH ${DRIVER_FLEXCOMM_FILES_PATH}/usart)
    set(DRIVER_FLEXSPI_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/flexspi)
    set(DRIVER_LPC_GPIO_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/lpc_gpio)
    set(DRIVER_IMU_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/imu)
    set(DRIVER_GDMA_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/gdma)
    set(DRIVER_TRNG_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/trng)
    set(DRIVER_COMMON_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/common)
    set(DRIVER_CTIMER_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/ostimer)
    set(DRIVER_RTC_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/lpc_rtc)
    set(DRIVER_DEVICE_FILES_PATH ${NXP_SDK_ROOT}/devices/RW612/drivers)
    set(DRIVER_ENET_FILES_PATH ${NXP_SDK_ROOT}/platform/drivers/enet)
    set(DEBUG_CONSOLE_FILES_PATH ${NXP_SDK_ROOT}/platform/utilities/debug_console)
    set(STR_FILES_PATH ${NXP_SDK_ROOT}/platform/utilities/str)
    set(FORMAT_FILES_PATH ${NXP_SDK_ROOT}/platform/utilities/misc_utilities)
else()
    message(FATAL_ERROR "Unknown SDK_TYPE")
endif()

set(RW612_SDK_SOURCES

    # startup files
    ${NXP_SDK_ROOT}/devices/RW612/gcc/startup_RW612.S
    ${NXP_SDK_ROOT}/devices/RW612/system_RW612.c

    # other SDK files
    ${DRIVER_COMMON_FILES_PATH}/fsl_common.c
    ${DRIVER_COMMON_FILES_PATH}/fsl_common_arm.c
    ${DRIVER_DEVICE_FILES_PATH}/fsl_clock.c
    ${DRIVER_DEVICE_FILES_PATH}/fsl_power.c
    ${DRIVER_DEVICE_FILES_PATH}/fsl_reset.c
    ${DRIVER_DEVICE_FILES_PATH}/fsl_ocotp.c
    ${DRIVER_CACHE_FILES_PATH}/fsl_cache.c
    ${DRIVER_FLEXCOMM_FILES_PATH}/fsl_flexcomm.c
    ${DRIVER_USART_FILES_PATH}/fsl_usart.c
    ${DRIVER_FLEXSPI_FILES_PATH}/fsl_flexspi.c
    ${DRIVER_LPC_GPIO_FILES_PATH}/fsl_gpio.c
    ${DRIVER_IMU_FILES_PATH}/fsl_imu.c
    ${DRIVER_GDMA_FILES_PATH}/fsl_gdma.c
    ${DRIVER_TRNG_FILES_PATH}/fsl_trng.c
    ${DRIVER_RTC_FILES_PATH}/fsl_rtc.c
    ${DEBUG_CONSOLE_FILES_PATH}/fsl_debug_console.c
    ${STR_FILES_PATH}/fsl_str.c
    ${FORMAT_FILES_PATH}/fsl_format.c

    # SDK components
    ${NXP_SDK_ROOT}/components/serial_manager/fsl_component_serial_manager.c
    ${NXP_SDK_ROOT}/components/serial_manager/fsl_component_serial_port_uart.c
    ${NXP_SDK_ROOT}/components/lists/fsl_component_generic_list.c
    ${NXP_SDK_ROOT}/components/osa/fsl_os_abstraction_free_rtos.c
    ${NXP_SDK_ROOT}/components/uart/fsl_adapter_usart.c
    ${NXP_SDK_ROOT}/components/rpmsg/fsl_adapter_rfimu.c

    # ${NXP_SDK_ROOT}/components/rng/fsl_adapter_software_rng.c
    ${NXP_SDK_ROOT}/components/flash/mflash/rdrw612bga/mflash_drv.c
    ${NXP_SDK_ROOT}/components/rng/fsl_adapter_trng.c
    ${NXP_SDK_ROOT}/components/time_stamp/fsl_adapter_ostimer_time_stamp.c
    ${NXP_SDK_ROOT}/components/power_manager/devices/RW612/fsl_pm_device.c
    ${NXP_SDK_ROOT}/components/power_manager/core/fsl_pm_core.c
    ${DRIVER_CTIMER_FILES_PATH}/fsl_ostimer.c

    # LittleFS
    ${MIDDLEWARE_PATH}/littlefs/lfs.c
    ${MIDDLEWARE_PATH}/littlefs/lfs_util.c

    # Loader files
    ${NXP_SDK_ROOT}/components/conn_fwloader/fsl_loader.c
    ${NXP_SDK_ROOT}/components/conn_fwloader/fsl_loader_utils.c
    ${NXP_SDK_ROOT}/components/conn_fwloader/life_cycle.c
    ${NXP_SDK_ROOT}/components/conn_fwloader/nboot_hal.c
)

if(OT_NXP_MBEDTLS_PORT)
    set(ELS_PKC_CL_PATH ${NXP_SDK_ROOT}/components/els_pkc)
    list(APPEND RW612_SDK_SOURCES

        # els_pkc component for mbedtls els_pkc port
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_AesCcm.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_AesCcmEngine.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_AesGcm.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_AesGcmEngine.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_Modes.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_Multipart.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_Els_Oneshot.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAead/src/mcuxClAead.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAes/src/mcuxClAes_KeyTypes.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClBuffer/src/mcuxClBuffer.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClCipherModes/src/mcuxClCipherModes_Common_Helper.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClCipherModes/src/mcuxClCipherModes_Crypt_Els_Modes.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClCipherModes/src/mcuxClCipherModes_Els_Aes.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClCipherModes/src/mcuxClCipherModes_Els_AesEngine.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClCipherModes/src/mcuxClCipherModes_Els_Aes_Internal.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClCipher/src/mcuxClCipher.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Constants.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_EdDSA_GenerateKeyPair.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_EdDSA_GenerateKeyPair_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_EdDSA_GenerateSignature.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_EdDSA_GenerateSignature_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_EdDSA_GenerateSignatureMode.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_EdDSA_InitPrivKeyInputMode.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_EdDSA_Internal_CalcHashModN.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_EdDSA_Internal_CalcHashModN_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_EdDSA_Internal_DecodePoint_Ed25519.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_EdDSA_Internal_DecodePoint_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_EdDSA_Internal_SetupEnvironment.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_EdDSA_Internal_SignatureMechanisms.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_EdDSA_VerifySignature.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Internal_BlindedScalarMult.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Internal_Convert_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Internal_GenerateMultiplicativeBlinding.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Internal_Interleave_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Internal_InterleaveScalar.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Internal_InterleaveTwoScalars.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Internal_PointComparison_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Internal_RecodeAndReorderScalar.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Internal_SetupEnvironment.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Internal_SetupEnvironment_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Internal_Types.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_KeyTypes.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_MontDH_GenerateKeyPair.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_MontDH_KeyAgreement.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Mont_Internal_DhSetupEnvironment.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Mont_Internal_MontDhX.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Mont_Internal_MontDhX_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Mont_Internal_SecureScalarMult_XZMontLadder.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Mont_Internal_SecureScalarMult_XZMontLadder_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_TwEd_Internal_FixScalarMult.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_TwEd_Internal_PlainFixScalarMult25519.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_TwEd_Internal_PlainPtrSelectComb.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_TwEd_Internal_PlainPtrSelectML.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_TwEd_Internal_PlainVarScalarMult.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_TwEd_Internal_PointArithmeticEd25519.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_TwEd_Internal_PointArithmeticEd25519_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_TwEd_Internal_PointSubtraction_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_TwEd_Internal_PointValidation_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_TwEd_Internal_PrecPointImportAndValidate.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_TwEd_Internal_VarScalarMult.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_TwEd_Internal_VarScalarMult_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_WeierECC_Internal_BlindedSecretKeyGen.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_WeierECC_Internal_BlindedSecretKeyGen_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_WeierECC_Internal_GenerateKeyPair.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_WeierECC_Internal_KeyAgreement_ECDH.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_WeierECC_Internal_SetupEnvironment.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Internal_ConvertPoint_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Internal_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Internal_PointArithmetic.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Internal_PointArithmetic_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Internal_PointCheck.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Internal_PointCheck_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Internal_PointMult.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Internal_SecurePointMult_CoZMontLadder.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Internal_SecurePointMult_CoZMontLadder_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Internal_SetupEnvironment.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_KeyGen.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_KeyGen_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_PointMult.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_PointMult_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Sign.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Sign_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Verify.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/src/mcuxClEcc_Weier_Verify_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEls/src/mcuxClEls_Aead.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEls/src/mcuxClEls_Cipher.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEls/src/mcuxClEls_Cmac.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEls/src/mcuxClEls_Common.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEls/src/mcuxClEls_Ecc.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEls/src/mcuxClEls_GlitchDetector.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEls/src/mcuxClEls_Hash.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEls/src/mcuxClEls_Hmac.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEls/src/mcuxClEls_Kdf.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEls/src/mcuxClEls_KeyManagement.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEls/src/mcuxClEls_Rng.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHashModes/src/mcuxClHashModes_Core_c_sha1.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHashModes/src/mcuxClHashModes_Core_els_sha2.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHashModes/src/mcuxClHashModes_Internal_c_sha1.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHashModes/src/mcuxClHashModes_Internal_els_sha2.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHash/src/mcuxClHash_api_multipart_common.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHash/src/mcuxClHash_api_multipart_compute.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHash/src/mcuxClHash_api_oneshot_compute.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHash/src/mcuxClHash_Internal.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHmac/src/mcuxClHmac_Els.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHmac/src/mcuxClHmac_Functions.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHmac/src/mcuxClHmac_Helper.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHmac/src/mcuxClHmac_KeyTypes.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHmac/src/mcuxClHmac_Modes.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHmac/src/mcuxClHmac_Sw.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClKey/src/mcuxClKey.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClKey/src/mcuxClKey_Protection.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMacModes/src/mcuxClMacModes_Common.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMacModes/src/mcuxClMacModes_Common_Modes.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMacModes/src/mcuxClMacModes_Els_Cbcmac.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMacModes/src/mcuxClMacModes_Els_Cmac.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMacModes/src/mcuxClMacModes_Els_Functions.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMac/src/mcuxClMac.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_ExactDivide.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_ExactDivideOdd.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_ExactDivideOdd_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_ModExp_SqrMultL2R.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_ModInv.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_ModInv_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_NDash.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_NDash_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_QDash.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_QDash_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_ReduceModEven.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_SecModExp.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_SecModExp_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/src/mcuxClMath_Utils.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMemory/src/mcuxClMemory.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClPadding/src/mcuxClPadding.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClPkc/src/mcuxClPkc_Calculate.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClPkc/src/mcuxClPkc_ImportExport.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClPkc/src/mcuxClPkc_Initialize.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClPkc/src/mcuxClPkc_UPTRT.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClPrng/src/mcuxClPrng_ELS.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandomModes/src/mcuxClRandomModes_CtrDrbg.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandomModes/src/mcuxClRandomModes_CtrDrbg_Els.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandomModes/src/mcuxClRandomModes_CtrDrbg_PrDisabled.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandomModes/src/mcuxClRandomModes_ElsMode.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandomModes/src/mcuxClRandomModes_NormalMode.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandomModes/src/mcuxClRandomModes_PatchMode.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandomModes/src/mcuxClRandomModes_PrDisabled.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandomModes/src/mcuxClRandomModes_TestMode.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandom/src/mcuxClRandom_DRBG.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandom/src/mcuxClRandom_PRNG.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_ComputeD.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_ComputeD_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_GenerateProbablePrime.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_getMillerRabinTestIterations.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_KeyGeneration_Crt.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_KeyGeneration_Crt_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_KeyGeneration_Plain.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_Mgf1.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_MillerRabinTest.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_MillerRabinTest_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_ModInv.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_Pkcs1v15Encode_sign.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_Pkcs1v15Verify.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_PrivateCrt.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_PrivateCrt_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_PrivatePlain.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_PssEncode.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_PssVerify.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_Public.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_PublicExp.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_RemoveBlinding.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_RemoveBlinding_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_Sign.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_Sign_NoEMSA.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_TestPQDistance.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_TestPQDistance_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_TestPrimeCandidate.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_TestPrimeCandidate_FUP.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_Verify.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_VerifyE.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/src/mcuxClRsa_Verify_NoEMSA.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClSession/src/mcuxClSession.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClTrng/src/mcuxClTrng_SA_TRNG.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxCsslMemory/src/mcuxCsslMemory_Clear.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxCsslMemory/src/mcuxCsslMemory_Compare.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxCsslMemory/src/mcuxCsslMemory_Copy.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxCsslMemory/src/mcuxCsslMemory_Internal_SecureCompare_Stub.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxCsslMemory/src/mcuxCsslMemory_Set.c
        ${ELS_PKC_CL_PATH}/src/comps/mcuxCsslParamIntegrity/src/mcuxCsslParamIntegrity.c
        ${ELS_PKC_CL_PATH}/src/platforms/rw61x/mcux_els.c
        ${ELS_PKC_CL_PATH}/src/platforms/rw61x/mcux_pkc.c
    )
endif()

if(OT_NXP_LWIP_ETH)
    list(APPEND RW612_SDK_SOURCES
        ${NXP_SDK_ROOT}/components/phy/device/phyksz8081/fsl_phyksz8081.c
        ${NXP_SDK_ROOT}/components/silicon_id/fsl_silicon_id.c
        ${NXP_SDK_ROOT}/components/silicon_id/socs/rw610/fsl_silicon_id_soc.c
        ${DRIVER_ENET_FILES_PATH}/fsl_enet.c
    )
endif()

add_library(${NXP_DRIVER_LIB}
    ${RW612_SDK_SOURCES}
)

set(RW612_SDK_INCLUDES

    # CMSIS includes
    ${NXP_SDK_ROOT}/CMSIS/Core/Include

    # other SDK files
    ${NXP_SDK_ROOT}/devices/RW612/drivers
    ${NXP_SDK_ROOT}/devices/RW612
    ${NXP_SDK_ROOT}/platform/drivers/ostimer
    ${DRIVER_COMMON_FILES_PATH}
    ${DRIVER_DEVICE_FILES_PATH}
    ${DRIVER_FLEXSPI_FILES_PATH}
    ${DEBUG_CONSOLE_FILES_PATH}
    ${DRIVER_CACHE_FILES_PATH}
    ${DRIVER_FLEXCOMM_FILES_PATH}
    ${DRIVER_USART_FILES_PATH}
    ${DRIVER_LPC_GPIO_FILES_PATH}
    ${DRIVER_IMU_FILES_PATH}
    ${DRIVER_GDMA_FILES_PATH}
    ${DRIVER_TRNG_FILES_PATH}
    ${DRIVER_RTC_FILES_PATH}
    ${STR_FILES_PATH}
    ${FORMAT_FILES_PATH}
    ${DRIVER_ENET_FILES_PATH}
    ${DRIVER_CTIMER_FILES_PATH}

    # SDK components
    ${NXP_SDK_ROOT}/components/uart
    ${NXP_SDK_ROOT}/components/serial_manager
    ${NXP_SDK_ROOT}/components/lists
    ${NXP_SDK_ROOT}/components/osa
    ${NXP_SDK_ROOT}/components/rpmsg
    ${NXP_SDK_ROOT}/components/rng
    ${NXP_SDK_ROOT}/components/flash/mflash
    ${NXP_SDK_ROOT}/components/flash/mflash/rdrw612bga
    ${NXP_SDK_ROOT}/components/timer
    ${NXP_SDK_ROOT}/components/time_stamp
    ${NXP_SDK_ROOT}/components/power_manager/devices/RW612
    ${NXP_SDK_ROOT}/components/power_manager/core/
    ${NXP_SDK_ROOT}/components/gpio/

    # LittleFS
    ${MIDDLEWARE_PATH}/littlefs

    # Loader files
    ${NXP_SDK_ROOT}/components/conn_fwloader/include
)

if(OT_NXP_LWIP_ETH)
    list(APPEND RW612_SDK_INCLUDES
        ${NXP_SDK_ROOT}/components/phy/
        ${NXP_SDK_ROOT}/components/phy/device/phyksz8081
        ${NXP_SDK_ROOT}/components/silicon_id
        ${NXP_SDK_ROOT}/components/silicon_id/socs
        ${DRIVER_ENET_FILES_PATH}
    )
endif()

if(OT_NXP_MBEDTLS_PORT)
    list(APPEND RW612_SDK_INCLUDES

        # els_pkc component for mbedtls els_pkc port
        ${ELS_PKC_CL_PATH}
        ${ELS_PKC_CL_PATH}/src/compiler
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAead/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAead/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAeadModes/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAeadModes/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAes/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClAes/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClBuffer/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClBuffer/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClCipher/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClCipher/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClCipherModes/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClCipherModes/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClCore/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEcc/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEls/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClEls/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHash/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHash/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHashModes/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHashModes/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHmac/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClHmac/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClKey/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClKey/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMac/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMac/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMacModes/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMacModes/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMath/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMemory/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClMemory/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClPadding/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClPadding/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClPkc/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClPkc/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClPrng/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClPrng/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandom/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandom/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandomModes/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRandomModes/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClRsa/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClSession/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClSession/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClTrng/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxClTrng/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxCsslCPreProcessor/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxCsslDataIntegrity/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxCsslFlowProtection/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxCsslMemory/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxCsslMemory/inc/internal
        ${ELS_PKC_CL_PATH}/src/comps/mcuxCsslParamIntegrity/inc
        ${ELS_PKC_CL_PATH}/src/comps/mcuxCsslSecureCounter/inc
        ${ELS_PKC_CL_PATH}/src/inc
        ${ELS_PKC_CL_PATH}/src/platforms/rw61x
        ${ELS_PKC_CL_PATH}/src/platforms/rw61x/inc
    )
endif()

if(OT_NXP_BUILD_APP_AS_LIB)
    # ethermind/coex_app_cli include
    list(APPEND RW612_SDK_INCLUDES
        ${MIDDLEWARE_PATH}/wireless/wpa_supplicant-rtos/port/mbedtls
    )
endif()

# freertos
target_include_directories(${NXP_DRIVER_LIB}
    PUBLIC
    ${RW612_SDK_INCLUDES}
)

target_compile_definitions(${NXP_DRIVER_LIB}
    PUBLIC
    -DCPU_RW612ETA1I
    -DCPU_RW612ETA2I
    -DCPU3
    -D__STARTUP_CLEAR_BSS
    -DSDK_OS_FREE_RTOS
    -DOSA_USED
    -DFSL_OSA_MAIN_FUNC_ENABLE=0
    -DFSL_RTOS_FREE_RTOS
    -DSDK_DEBUGCONSOLE_UART=1
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING
    -DSERIAL_PORT_TYPE_UART=1
    -DHAL_UART_ADAPTER_LOWPOWER=1

    # -DSERIAL_MANAGER_NON_BLOCKING_MODE=1
    -DSERIAL_MANAGER_TASK_STACK_SIZE=1024
    -DSERIAL_MANAGER_TASK_PRIORITY=PRIORITY_RTOS_TO_OSA\(2\)
    -DTM_ENABLE_TIME_STAMP=1
    -DIMU_TASK_STACK_SIZE=1024
    -DgNvStorageIncluded_d=1
    -DAPP_FLEXSPI_AMBA_BASE=0x08000000
    -DOT_PLAT_SAVE_NVM_DATA_ON_IDLE=1
    -DgAspCapability_d=1
    -DFFU_CNS_TX_PWR_TABLE_CALIBRATION=1
    -DIMU_TASK_PRIORITY=2 # OSA priority: configMAX_PRIORITIES - (osa_prio) - 2 = FreeRTOS priority
    -DgPlatformDisableBleLowPower_d=0 # Enable BLE/15.4 low power feature
    LFS_CONFIG=fwk_lfs_config.h
    LFS_THREADSAFE=1
    -DNOT_DEFINE_DEFAULT_WIFI_MODULE
    -DWIFI_BOARD_RW610
)

if(OT_NXP_LWIP_ETH)
    target_compile_definitions(${NXP_DRIVER_LIB}
        PUBLIC
        -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE
        -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS
        -DUSE_RTOS
    )
endif()

if(OT_NXP_DEVICE_REVISION MATCHES "A0")
    target_compile_definitions(${NXP_DRIVER_LIB}
        PUBLIC
        -DRW610_A1=0
    )
endif()

if(OT_NXP_ENABLE_WPA_SUPP_MBEDTLS)
    target_compile_definitions(${NXP_DRIVER_LIB}
        PUBLIC
        -DCONFIG_WPA_SUPP_MBEDTLS # Enable wpa_supplicant mbedtls extend config
    )
endif()

target_compile_options(${NXP_DRIVER_LIB}
    PRIVATE
    ${OT_CFLAGS}
    -Wno-unknown-pragmas
    -Wno-sign-compare
    -Wno-unused-function
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wno-empty-body
    -Wno-int-conversion
    -Wno-int-in-bool-context
    -Wno-memset-elt-size
    -Wno-parentheses
)

target_link_libraries(${NXP_DRIVER_LIB}
    PRIVATE
    ${NXP_BOARD_LIB} # board.h needed by firmware loader for flexspi flash
    PUBLIC
    freertos_kernel

    # Dependencies to LFS_CONFIG
    connfwk-config
    connfwk-platform-${CONNFWK_PLATFORM}
    ${OT_EXTERNAL_MBEDTLS}
)
