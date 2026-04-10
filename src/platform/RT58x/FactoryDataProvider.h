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
#include "flashctl.h"
#include "log.h"

#if defined(CONFIG_RT584HA4)
#define FLASH_SECURE_OFFSET 0x10200000
#elif defined(CONFIG_RT584H) || defined(CONFIG_RT584L)
#define FLASH_SECURE_OFFSET 0x10000000
#else
#define FLASH_SECURE_OFFSET 0
#endif

#define MODE_CD_COUNT_DATA_ADDR  0x001F4000 + FLASH_SECURE_OFFSET
#define PAI_CERT_LEN_ADDR        0x001F4010 + FLASH_SECURE_OFFSET
#define PAI_CERT_DATA_ADDR       0x001F4020 + FLASH_SECURE_OFFSET
#define DAC_CERT_LEN_ADDR        0x001F4290 + FLASH_SECURE_OFFSET
#define DAC_CERT_DATA_ADDR       0x001F42A0 + FLASH_SECURE_OFFSET
#define DAC_PUBKEY_LEN_ADDR      0x001F4510 + FLASH_SECURE_OFFSET
#define DAC_PUBKEY_DATA_ADDR     0x001F4520 + FLASH_SECURE_OFFSET
#define DAC_PRIVKEY_LEN_ADDR     0x001F4590 + FLASH_SECURE_OFFSET
#define DAC_PRIVKEY_DATA_ADDR    0x001F45A0 + FLASH_SECURE_OFFSET
#define CD0_LEN_ADDR             0x001F4610 + FLASH_SECURE_OFFSET
#define CD0_DATA_ADDR            0x001F4620 + FLASH_SECURE_OFFSET
#define CD1_LEN_ADDR             0x001F4890 + FLASH_SECURE_OFFSET
#define CD1_DATA_ADDR            0x001F48A0 + FLASH_SECURE_OFFSET
#define CD2_LEN_ADDR             0x001F4B10 + FLASH_SECURE_OFFSET
#define CD2_DATA_ADDR            0x001F4B20 + FLASH_SECURE_OFFSET
#define CD3_LEN_ADDR             0x001F4610 + FLASH_SECURE_OFFSET
#define CD3_DATA_ADDR            0x001F4D90 + FLASH_SECURE_OFFSET
#define CD4_LEN_ADDR             0x001F4DA0 + FLASH_SECURE_OFFSET
#define CD4_DATA_ADDR            0x001F4620 + FLASH_SECURE_OFFSET
#define CD5_LEN_ADDR             0x001F5010 + FLASH_SECURE_OFFSET
#define CD5_DATA_ADDR            0x001F5020 + FLASH_SECURE_OFFSET
#define PASSCODE_DATA_ADDR       0x001F5510 + FLASH_SECURE_OFFSET
#define DISCRIMINATOR_DATA_ADDR  0x001F5520 + FLASH_SECURE_OFFSET
#define MANUALCODE_LEN_ADDR      0x001F5530 + FLASH_SECURE_OFFSET
#define MANUALCODE_DATA_ADDR     0x001F5540 + FLASH_SECURE_OFFSET
#define QRCODE_LEN_ADDR          0x001F5560 + FLASH_SECURE_OFFSET
#define QRCODE_DATA_ADDR         0x001F5570 + FLASH_SECURE_OFFSET
#define SALT_LEN_ADDR            0x001F5630 + FLASH_SECURE_OFFSET
#define SALT_DATA_ADDR           0x001F5640 + FLASH_SECURE_OFFSET
#define VERIFIER_LEN_ADDR        0x001F5670 + FLASH_SECURE_OFFSET
#define VERIFIER_DATA_ADDR       0x001F5680 + FLASH_SECURE_OFFSET

namespace chip {
namespace DeviceLayer {

// size_t GetDecryptedData(uint8_t *Encrypt, uint8_t *Decrypt, uint8_t *key, uint32_t length);

struct InternalFlashFactoryData
{
    template<typename T>
    CHIP_ERROR GetDecryptedData(T *enc, T *dec, uint8_t *key)
    {
        mbedtls_aes_context aes_ctx;

        unsigned int blockNumber = 0;
        unsigned int blockIndex  = 0;
        unsigned int blockSize   = 16;

        mbedtls_aes_init(&aes_ctx);
        mbedtls_aes_setkey_dec(&aes_ctx, key, blockSize * 8);
        blockNumber = enc->len / blockSize;
        for (blockIndex = 0; blockIndex < blockNumber; blockIndex++) 
        {
            mbedtls_aes_crypt_ecb(&aes_ctx, 
                                  MBEDTLS_AES_DECRYPT, 
                                  &enc->data[blockIndex * blockSize], 
                                  &dec->data[blockIndex * blockSize]);
        }
        mbedtls_aes_free(&aes_ctx);
        dec->len = enc->len - dec->data[enc->len - 1];
        return CHIP_NO_ERROR;
    }

    CHIP_ERROR GetFactoryData(struct FactoryData * mFactoryData)
    {
        // data     = reinterpret_cast<uint8_t *>(PM_FACTORY_DATA_ADDRESS);
        // dataSize = PM_FACTORY_DATA_SIZE;
        
        uint8_t sha256[4] = {0xef, 0x37, 0x92, 0xf8};
        uint8_t key[16] = {0};

        size_t Mode = 0;
        size_t CD_Count = 0;

        struct FactoryData mFlashData;

        mbedtls_sha256_ret(sha256, 4, key, 0);

        Mode =  flash_read_byte(MODE_CD_COUNT_DATA_ADDR);
        Mode |= flash_read_byte(MODE_CD_COUNT_DATA_ADDR + 1);
        Mode |= flash_read_byte(MODE_CD_COUNT_DATA_ADDR + 2);
        Mode |= flash_read_byte(MODE_CD_COUNT_DATA_ADDR + 3);

        CD_Count =  flash_read_byte(MODE_CD_COUNT_DATA_ADDR + 4);
        CD_Count |= flash_read_byte(MODE_CD_COUNT_DATA_ADDR + 5);
        CD_Count |= flash_read_byte(MODE_CD_COUNT_DATA_ADDR + 6);
        CD_Count |= flash_read_byte(MODE_CD_COUNT_DATA_ADDR + 7);
        
        mFlashData.pai_cert.len =  flash_read_byte(PAI_CERT_LEN_ADDR);
        mFlashData.pai_cert.len |= flash_read_byte(PAI_CERT_LEN_ADDR + 1) << 8;
        mFlashData.pai_cert.len |= flash_read_byte(PAI_CERT_LEN_ADDR + 2) << 16;
        mFlashData.pai_cert.len |= flash_read_byte(PAI_CERT_LEN_ADDR + 3) << 24;

        if (mFlashData.pai_cert.len > 640)
        {
            mFlashData.pai_cert.len = 640;
        }

        for (size_t i = 0; i < mFlashData.pai_cert.len; i++) 
        {
            mFlashData.pai_cert.data[i] = flash_read_byte(PAI_CERT_DATA_ADDR + i);
        }

        GetDecryptedData((struct FactoryDataCert *)&mFlashData.pai_cert, 
                         (struct FactoryDataCert *)&mFactoryData->pai_cert, 
                         key);

        mFlashData.dac_cert.len =  flash_read_byte(DAC_CERT_LEN_ADDR);
        mFlashData.dac_cert.len |= flash_read_byte(DAC_CERT_LEN_ADDR + 1) << 8;
        mFlashData.dac_cert.len |= flash_read_byte(DAC_CERT_LEN_ADDR + 2) << 16;
        mFlashData.dac_cert.len |= flash_read_byte(DAC_CERT_LEN_ADDR + 3) << 24;

        if (mFlashData.dac_cert.len > 640)
        {
            mFlashData.dac_cert.len = 640;
        }

        for (size_t i = 0; i < mFlashData.dac_cert.len; i++) 
        {
            mFlashData.dac_cert.data[i] = flash_read_byte(DAC_CERT_DATA_ADDR + i);
        }

        GetDecryptedData((struct FactoryDataCert *)&mFlashData.dac_cert, 
                         (struct FactoryDataCert *)&mFactoryData->dac_cert, 
                         key);

        mFlashData.dac_privkey.len =  flash_read_byte(DAC_PRIVKEY_LEN_ADDR);
        mFlashData.dac_privkey.len |= flash_read_byte(DAC_PRIVKEY_LEN_ADDR + 1) << 8;
        mFlashData.dac_privkey.len |= flash_read_byte(DAC_PRIVKEY_LEN_ADDR + 2) << 16;
        mFlashData.dac_privkey.len |= flash_read_byte(DAC_PRIVKEY_LEN_ADDR + 3) << 24;

        if (mFlashData.dac_privkey.len > 640)
        {
            mFlashData.dac_privkey.len = 640;
        }

        for (size_t i = 0; i < mFlashData.dac_privkey.len; i++) 
        {
            mFlashData.dac_privkey.data[i] = flash_read_byte(DAC_PRIVKEY_DATA_ADDR + i);
        }

        GetDecryptedData((struct FactoryDataCert *)&mFlashData.dac_privkey, 
                         (struct FactoryDataCert *)&mFactoryData->dac_privkey, 
                         key);

        mFlashData.cd.len =  flash_read_byte(CD0_LEN_ADDR);
        mFlashData.cd.len |= flash_read_byte(CD0_LEN_ADDR + 1) << 8;
        mFlashData.cd.len |= flash_read_byte(CD0_LEN_ADDR + 2) << 16;
        mFlashData.cd.len |= flash_read_byte(CD0_LEN_ADDR + 3) << 24;

        if (mFlashData.cd.len > 640)
        {
            mFlashData.cd.len = 640;
        }
        mFactoryData->cd.len = mFlashData.cd.len;

        for (size_t i = 0; i < mFlashData.cd.len; i++) 
        {
            mFlashData.cd.data[i] = flash_read_byte(CD0_DATA_ADDR + i);
        }

        GetDecryptedData((struct FactoryDataCert *)&mFlashData.cd, 
                         (struct FactoryDataCert *)&mFactoryData->cd, 
                         key);

        mFactoryData->passcode =  (flash_read_byte(PASSCODE_DATA_ADDR + 0) - 0x30) * 10000000;
        mFactoryData->passcode += (flash_read_byte(PASSCODE_DATA_ADDR + 1) - 0x30) * 1000000;
        mFactoryData->passcode += (flash_read_byte(PASSCODE_DATA_ADDR + 2) - 0x30) * 100000;
        mFactoryData->passcode += (flash_read_byte(PASSCODE_DATA_ADDR + 3) - 0x30) * 10000;
        mFactoryData->passcode += (flash_read_byte(PASSCODE_DATA_ADDR + 4) - 0x30) * 1000;
        mFactoryData->passcode += (flash_read_byte(PASSCODE_DATA_ADDR + 5) - 0x30) * 100;
        mFactoryData->passcode += (flash_read_byte(PASSCODE_DATA_ADDR + 6) - 0x30) * 10;
        mFactoryData->passcode += (flash_read_byte(PASSCODE_DATA_ADDR + 7) - 0x30);

        mFactoryData->discriminator =  (flash_read_byte(DISCRIMINATOR_DATA_ADDR + 0) - 0x30) * 1000;
        mFactoryData->discriminator += (flash_read_byte(DISCRIMINATOR_DATA_ADDR + 1) - 0x30) * 100;
        mFactoryData->discriminator += (flash_read_byte(DISCRIMINATOR_DATA_ADDR + 2) - 0x30) * 10;
        mFactoryData->discriminator += (flash_read_byte(DISCRIMINATOR_DATA_ADDR + 3) - 0x30);

        mFactoryData->manualcode.len =  flash_read_byte(MANUALCODE_LEN_ADDR);
        mFactoryData->manualcode.len |= flash_read_byte(MANUALCODE_LEN_ADDR + 1) << 8;
        mFactoryData->manualcode.len |= flash_read_byte(MANUALCODE_LEN_ADDR + 2) << 16;
        mFactoryData->manualcode.len |= flash_read_byte(MANUALCODE_LEN_ADDR + 3) << 24;

        if (mFactoryData->manualcode.len > 32)
        {
            mFactoryData->manualcode.len = 32;
        }

        for (size_t i = 0; i < mFactoryData->manualcode.len; i++)
        {
            mFactoryData->manualcode.data[i] = flash_read_byte(MANUALCODE_DATA_ADDR + i);
        }
        mFactoryData->manualcode.data[mFactoryData->manualcode.len] = '\0';
        mFactoryData->manualcode.len += 1;

        mFactoryData->qrcode.len =  flash_read_byte(QRCODE_LEN_ADDR);
        mFactoryData->qrcode.len |= flash_read_byte(QRCODE_LEN_ADDR + 1) << 8;
        mFactoryData->qrcode.len |= flash_read_byte(QRCODE_LEN_ADDR + 2) << 16;
        mFactoryData->qrcode.len |= flash_read_byte(QRCODE_LEN_ADDR + 3) << 24;

        if (mFactoryData->qrcode.len > 191)
        {
            mFactoryData->qrcode.len = 191;
        }

        for (size_t i = 0; i < mFactoryData->qrcode.len; i++)
        {
            mFactoryData->qrcode.data[i] = flash_read_byte(QRCODE_DATA_ADDR + i);
        }
        mFactoryData->qrcode.data[mFactoryData->qrcode.len] = '\0';
        mFactoryData->qrcode.len += 1;

        mFlashData.spake2_salt.len =  flash_read_byte(SALT_LEN_ADDR);
        mFlashData.spake2_salt.len |= flash_read_byte(SALT_LEN_ADDR + 1) << 8;
        mFlashData.spake2_salt.len |= flash_read_byte(SALT_LEN_ADDR + 2) << 16;
        mFlashData.spake2_salt.len |= flash_read_byte(SALT_LEN_ADDR + 3) << 24; 

        if (mFlashData.spake2_salt.len > 47)
        {
            mFlashData.spake2_salt.len = 47;
        }

        for (size_t i = 0; i < mFlashData.spake2_salt.len; i++)
        {
            mFlashData.spake2_salt.data[i] = flash_read_byte(SALT_DATA_ADDR + i);
        }

        GetDecryptedData((struct FactoryDataSalt *)&mFlashData.spake2_salt, 
                         (struct FactoryDataSalt *)&mFactoryData->spake2_salt, 
                         key);

        mFactoryData->spake2_salt.data[mFactoryData->spake2_salt.len] = '\0';
        mFactoryData->spake2_salt.len += 1;

        mFlashData.spake2_verifier.len =  flash_read_byte(VERIFIER_LEN_ADDR);
        mFlashData.spake2_verifier.len |= flash_read_byte(VERIFIER_LEN_ADDR + 1) << 8;
        mFlashData.spake2_verifier.len |= flash_read_byte(VERIFIER_LEN_ADDR + 2) << 16;
        mFlashData.spake2_verifier.len |= flash_read_byte(VERIFIER_LEN_ADDR + 3) << 24;

        if (mFlashData.spake2_verifier.len > 255)
        {
            mFlashData.spake2_verifier.len = 255;
        }

        for (size_t i = 0; i < mFlashData.spake2_verifier.len; i++)
        {
            mFlashData.spake2_verifier.data[i] = flash_read_byte(VERIFIER_DATA_ADDR + i);
        }

        GetDecryptedData((struct FactoryDataVerifier *)&mFlashData.spake2_verifier, 
                         (struct FactoryDataVerifier *)&mFactoryData->spake2_verifier, 
                         key);

        mFactoryData->spake2_verifier.data[mFactoryData->spake2_verifier.len] = '\0';
        mFactoryData->spake2_verifier.len += 1;

        mFactoryData->spake2_it = 1000;
        mFactoryData->discriminatorPresent = true;

        return CHIP_NO_ERROR;
    }

    // CHIP_ERROR GetFactoryDataPasscode(uint8_t *& data, size_t & dataSize)
    // {
    //     return CHIP_NO_ERROR;
    // }

    // CHIP_ERROR GetFactoryDataPartition(uint8_t *& data, size_t & dataSize)
    // {
    //     data     = reinterpret_cast<uint8_t *>(PM_FACTORY_DATA_ADDRESS);
    //     dataSize = PM_FACTORY_DATA_SIZE;
    //     return CHIP_NO_ERROR;
    // }

    // CHIP_ERROR ProtectFactoryDataPartitionAgainstWrite()
    // {
    //     int ret = fprotect_area(PM_FACTORY_DATA_ADDRESS, PM_FACTORY_DATA_SIZE);
    //     return System::MapErrorZephyr(ret);
    //     return CHIP_NO_ERROR;
    // }
};

struct ExternalFlashFactoryData
{
    size_t GetDecryptedData(uint8_t *Encrypt, uint8_t *Decrypt, uint8_t *key, uint32_t length)
    {
        return 0;
    }

    CHIP_ERROR GetFactoryData(struct FactoryData * mFactoryData)
    {
        return CHIP_NO_ERROR;
    }

    // CHIP_ERROR GetFactoryDataPasscode(uint8_t *& data, size_t & dataSize)
    // {
    //     return CHIP_NO_ERROR;
    // }

    // CHIP_ERROR GetFactoryDataPartition(uint8_t *& data, size_t & dataSize)
    // {
    //     int ret = flash_read(mFlashDevice, PM_FACTORY_DATA_ADDRESS, mFactoryDataBuffer, PM_FACTORY_DATA_SIZE);

    //     if (ret != 0)
    //     {
    //         return CHIP_ERROR_READ_FAILED;
    //     }

    //     data     = mFactoryDataBuffer;
    //     dataSize = PM_FACTORY_DATA_SIZE;

    //     return CHIP_NO_ERROR;
    // }

    // CHIP_ERROR ProtectFactoryDataPartitionAgainstWrite() { return CHIP_ERROR_NOT_IMPLEMENTED; }

    // const struct device * mFlashDevice = DEVICE_DT_GET(DT_CHOSEN(zephyr_flash_controller));
    // uint8_t mFactoryDataBuffer[2]; //PM_FACTORY_DATA_SIZE
};

template <class FlashFactoryData>
class FactoryDataProvider : public chip::Credentials::DeviceAttestationCredentialsProvider,
                            public chip::DeviceLayer::CommissionableDataProvider,
                            public chip::DeviceLayer::DeviceInstanceInfoProvider
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
    CHIP_ERROR GetVendorName(char * buf, size_t bufSize) override;
    CHIP_ERROR GetVendorId(uint16_t & vendorId) override;
    CHIP_ERROR GetProductName(char * buf, size_t bufSize) override;
    CHIP_ERROR GetProductId(uint16_t & productId) override;
    CHIP_ERROR GetPartNumber(char * buf, size_t bufSize) override;
    CHIP_ERROR GetProductURL(char * buf, size_t bufSize) override;
    CHIP_ERROR GetProductLabel(char * buf, size_t bufSize) override;
    CHIP_ERROR GetSerialNumber(char * buf, size_t bufSize) override;
    CHIP_ERROR GetManufacturingDate(uint16_t & year, uint8_t & month, uint8_t & day) override;
    CHIP_ERROR GetHardwareVersion(uint16_t & hardwareVersion) override;
    CHIP_ERROR GetHardwareVersionString(char * buf, size_t bufSize) override;
    CHIP_ERROR GetRotatingDeviceIdUniqueId(MutableByteSpan & uniqueIdSpan) override;

    // ===== Members functions that are platform-specific
    CHIP_ERROR GetEnableKey(MutableByteSpan & enableKey);

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
