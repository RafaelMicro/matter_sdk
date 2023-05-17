/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#pragma once

#include <credentials/DeviceAttestationCredsProvider.h>
#include <platform/CommissionableDataProvider.h>
#include <platform/DeviceInstanceInfoProvider.h>

// #include <drivers/flash.h>
// #include <fprotect.h>
// #include <pm_config.h>
#include <system/SystemError.h>

#include "mbedtls/aes.h"
#include "mbedtls/sha256.h"
#include "FactoryDataParser.h"
#include "util_log.h"

#define PAI_CERT_ADDR    0x001F4010
#define DAC_CERT_ADDR    0x001F4290
#define DAC_PRIVKEY_ADDR 0x001F4590
#define DAC_PUBKEY_ADDR  0x001F4510

namespace chip {
namespace DeviceLayer {

// size_t GetDecryptionCertification(uint8_t *Encrypt, uint8_t *Decrypt, uint8_t *key, uint32_t length);

struct InternalFlashFactoryData
{
    CHIP_ERROR GetDecryptionCertification(struct FactoryDataCert *enc, struct FactoryDataCert *dec, uint8_t *key)
    {
        mbedtls_aes_context aes_ctx;

        unsigned int blockNumber = 0;
        unsigned int blockIndex  = 0;
        unsigned int blockSize   = 16;

        mbedtls_aes_init(&aes_ctx);
        mbedtls_aes_setkey_dec(&aes_ctx, key, blockSize * 8);
        blockNumber = enc->len / blockSize;
        for (blockIndex = 0; blockIndex < blockNumber; blockIndex++) {
            mbedtls_aes_crypt_ecb(&aes_ctx, 
                                MBEDTLS_AES_DECRYPT, 
                                &enc->data[blockIndex * blockSize], 
                                &dec->data[blockIndex * blockSize]);
        }
        mbedtls_aes_free(&aes_ctx);

        dec->len = enc->len - dec->data[enc->len - 1];
        // info("===> len: %d\r\n", dec->len);
        // for (int i = 0; i < dec->len; i++) {
        //     info("%02x ", dec->data[i]);
        // }
        // info("\r\n");
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR GetFactoryDataCertificates(struct FactoryData * mFactoryData)
    {
        // data     = reinterpret_cast<uint8_t *>(PM_FACTORY_DATA_ADDRESS);
        // dataSize = PM_FACTORY_DATA_SIZE;
        
        uint8_t sha256[4] = {0xef, 0x37, 0x92, 0xf8};
        uint8_t key[16] = {0};

        struct FactoryData mFlashData;

        mbedtls_sha256(sha256, 4, key, 0);

        mFlashData.pai_cert.len = flash_read_byte(PAI_CERT_ADDR);
        mFlashData.pai_cert.len |= flash_read_byte(PAI_CERT_ADDR + 1) << 8;

        for (uint32_t i = 0; i < mFlashData.pai_cert.len; i++) {
            mFlashData.pai_cert.data[i] = flash_read_byte(PAI_CERT_ADDR + 2 + i);
        }

        GetDecryptionCertification(&mFlashData.pai_cert, &mFactoryData->pai_cert, key);

        mFlashData.dac_cert.len = flash_read_byte(DAC_CERT_ADDR);
        mFlashData.dac_cert.len |= flash_read_byte(DAC_CERT_ADDR + 1) << 8;

        for (uint32_t i = 0; i < mFlashData.dac_cert.len; i++) {
            mFlashData.dac_cert.data[i] = flash_read_byte(DAC_CERT_ADDR + 2 + i);
        }

        GetDecryptionCertification(&mFlashData.dac_cert, &mFactoryData->dac_cert, key);

        // ENC_DAC_PubKey_Len = flash_read_byte(DAC_PUBKEY_ADDR);
        // ENC_DAC_PubKey_Len |= flash_read_byte(DAC_PUBKEY_ADDR+1) << 8;

        mFlashData.dac_privkey.len = flash_read_byte(DAC_PRIVKEY_ADDR);
        mFlashData.dac_privkey.len |= flash_read_byte(DAC_PRIVKEY_ADDR + 1) << 8;

        for (uint32_t i = 0; i < mFlashData.dac_privkey.len; i++) {
            mFlashData.dac_privkey.data[i] = flash_read_byte(DAC_PRIVKEY_ADDR + 2 + i);
        }

        GetDecryptionCertification(&mFlashData.dac_privkey, &mFactoryData->dac_privkey, key);

        return CHIP_NO_ERROR;
    }

    CHIP_ERROR GetFactoryDataPartition(uint8_t *& data, size_t & dataSize)
    {
        // data     = reinterpret_cast<uint8_t *>(PM_FACTORY_DATA_ADDRESS);
        // dataSize = PM_FACTORY_DATA_SIZE;
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR ProtectFactoryDataPartitionAgainstWrite()
    {
        // int ret = fprotect_area(PM_FACTORY_DATA_ADDRESS, PM_FACTORY_DATA_SIZE);
        // return System::MapErrorZephyr(ret);
        return CHIP_NO_ERROR;
    }
};

struct ExternalFlashFactoryData
{
    size_t GetDecryptionCertification(uint8_t *Encrypt, uint8_t *Decrypt, uint8_t *key, uint32_t length)
    {
        return 0;
    }

    CHIP_ERROR GetFactoryDataCertificates(struct FactoryData * mFactoryData)
    {
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR GetFactoryDataPartition(uint8_t *& data, size_t & dataSize)
    {
        // int ret = flash_read(mFlashDevice, PM_FACTORY_DATA_ADDRESS, mFactoryDataBuffer, PM_FACTORY_DATA_SIZE);

        // if (ret != 0)
        // {
        //     return CHIP_ERROR_READ_FAILED;
        // }

        // data     = mFactoryDataBuffer;
        // dataSize = PM_FACTORY_DATA_SIZE;

        return CHIP_NO_ERROR;
    }

    CHIP_ERROR ProtectFactoryDataPartitionAgainstWrite() { return CHIP_ERROR_NOT_IMPLEMENTED; }

    // const struct device * mFlashDevice = DEVICE_DT_GET(DT_CHOSEN(zephyr_flash_controller));
    uint8_t mFactoryDataBuffer[2]; //PM_FACTORY_DATA_SIZE
};

// template <class FlashFactoryData>
// class FactoryDataProvider : public chip::Credentials::DeviceAttestationCredentialsProvider,
//                             public CommissionableDataProvider,
//                             public DeviceInstanceInfoProvider
template <class FlashFactoryData>
class FactoryDataProvider : public chip::Credentials::DeviceAttestationCredentialsProvider,
                            public chip::DeviceLayer::CommissionableDataProvider
{
public:
    CHIP_ERROR Init();

    // ===== Members functions that implement the DeviceAttestationCredentialsProvider
    CHIP_ERROR GetCertificationDeclaration(MutableByteSpan & outBuffer) override;
    CHIP_ERROR GetFirmwareInformation(MutableByteSpan & out_firmware_info_buffer) override;
    CHIP_ERROR GetDeviceAttestationCert(MutableByteSpan & outBuffer) override;
    CHIP_ERROR GetProductAttestationIntermediateCert(MutableByteSpan & outBuffer) override;
    CHIP_ERROR SignWithDeviceAttestationKey(const ByteSpan & messageToSign, MutableByteSpan & outSignBuffer) override;

    // ===== Members functions that implement the CommissionableDataProvider
    CHIP_ERROR GetSetupDiscriminator(uint16_t & setupDiscriminator) override;
    CHIP_ERROR SetSetupDiscriminator(uint16_t setupDiscriminator) override;
    CHIP_ERROR GetSpake2pIterationCount(uint32_t & iterationCount) override;
    CHIP_ERROR GetSpake2pSalt(MutableByteSpan & saltBuf) override;
    CHIP_ERROR GetSpake2pVerifier(MutableByteSpan & verifierBuf, size_t & verifierLen) override;
    CHIP_ERROR GetSetupPasscode(uint32_t & setupPasscode) override;
    CHIP_ERROR SetSetupPasscode(uint32_t setupPasscode) override;

    // ===== Members functions that implement the DeviceInstanceInfoProvider
    // CHIP_ERROR GetVendorName(char * buf, size_t bufSize) override;
    // CHIP_ERROR GetVendorId(uint16_t & vendorId) override;
    // CHIP_ERROR GetProductName(char * buf, size_t bufSize) override;
    // CHIP_ERROR GetProductId(uint16_t & productId) override;
    // CHIP_ERROR GetSerialNumber(char * buf, size_t bufSize) override;
    // CHIP_ERROR GetManufacturingDate(uint16_t & year, uint8_t & month, uint8_t & day) override;
    // CHIP_ERROR GetHardwareVersion(uint16_t & hardwareVersion) override;
    // CHIP_ERROR GetHardwareVersionString(char * buf, size_t bufSize) override;
    // CHIP_ERROR GetRotatingDeviceIdUniqueId(MutableByteSpan & uniqueIdSpan) override;

    // ===== Members functions that are platform-specific
    // CHIP_ERROR GetEnableKey(MutableByteSpan & enableKey);

private:
    static constexpr uint16_t kFactoryDataPartitionSize    = 100; // PM_FACTORY_DATA_SIZE
    static constexpr uint32_t kFactoryDataPartitionAddress = 100; // PM_FACTORY_DATA_ADDRESS
    static constexpr uint8_t kDACPrivateKeyLength          = 32;
    static constexpr uint8_t kDACPublicKeyLength           = 65;

    struct FactoryData mFactoryData;
    FlashFactoryData mFlashFactoryData;
};

} // namespace DeviceLayer
} // namespace chip
