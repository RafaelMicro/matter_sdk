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

#include "FactoryDataProvider.h"
#include "CHIPDevicePlatformConfig.h"
#include <crypto/CHIPCryptoPAL.h>

#if CONFIG_CHIP_CERTIFICATION_DECLARATION_STORAGE
#include <credentials/CertificationDeclaration.h>
#include <platform/RT582/RT582Config.h>
#endif

#include <lib/support/SafeInt.h>
#include <lib/support/Base64.h>

// #include <logging/log.h>
// #include "util_log.h"
#include "FactoryDataParser.h"
#include "cm3_mcu.h"

#if RAFAEL_CERTS_ENABLED
#define CHIP_DEVICE_CONFIG_DEVICE_VENDOR_ID                 0x1346
#define CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_ID                0x400E
// #define CHIP_DEVICE_CONFIG_DEVICE_HARDWARE_VERSION_STRING   "1.0"
// #define CHIP_DEVICE_CONFIG_DEVICE_HARDWARE_VERSION          0x0001
#define CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING   "1.0"
#define CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION          0x0001
#define CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME               "RafaelMicro"
#define CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME              "DimmableLight"
// #define CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER               "RAFAEL0001"
#endif

namespace chip {
namespace {

CHIP_ERROR LoadKeypairFromRaw(ByteSpan privateKey, ByteSpan publicKey, Crypto::P256Keypair & keypair)
{
    Crypto::P256SerializedKeypair serializedKeypair;
    ReturnErrorOnFailure(serializedKeypair.SetLength(privateKey.size() + publicKey.size()));
    memcpy(serializedKeypair.Bytes(), publicKey.data(), publicKey.size());
    memcpy(serializedKeypair.Bytes() + publicKey.size(), privateKey.data(), privateKey.size());
    return keypair.Deserialize(serializedKeypair);
}
} // namespace

namespace DeviceLayer {

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    err = mFlashFactoryData.GetFactoryData(&mFactoryData);

    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Failed to read factory data partition");
        return err;
    }

    // info("===> Get commissionable data from factory data\r\n");
    // info("===> Passcode: %d\r\n",      mFactoryData.passcode);
    // info("===> Discriminator: %d\r\n", mFactoryData.discriminator);
    // info("===> Salt: %s\r\n",          mFactoryData.spake2_salt.data);
    // info("===> Verifier: %s\r\n",      mFactoryData.spake2_verifier.data);
    // info("===> QRCode: %s\r\n",        mFactoryData.qrcode.data);
    // info("===> ManualCode: %s\r\n",    mFactoryData.manualcode.data);

    // info("===> cd len: %d\r\n", mFactoryData.cd.len);
    // for (size_t i = 0; i < mFactoryData.cd.len; i++)
    // {
    //     info("%02x ", mFactoryData.cd.data[i]);
    //     if (i != 0 && (i + 1) % 16 == 0) info("\r\n");        
    // }
    // info("\r\n");

    // CHIP_ERROR error = mFlashFactoryData.ProtectFactoryDataPartitionAgainstWrite();

    // Protection against write for external storage is not supported.
    // if (error == CHIP_ERROR_NOT_IMPLEMENTED)
    // {
    //     ChipLogProgress(DeviceLayer, "The device does not support hardware protection against write.");
    //     error = CHIP_NO_ERROR;
    // }
    // else if (error != CHIP_NO_ERROR)
    // {
    //     ChipLogError(DeviceLayer, "Failed to protect the factory data partition.");
    //     return error;
    // }

    // err = mFlashFactoryData.GetFactoryDataPartition(factoryData, factoryDataSize);

    // if (err != CHIP_NO_ERROR)
    // {
    //     ChipLogError(DeviceLayer, "Failed to read factory data partition");
    //     return err;
    // }

    // if (!ParseFactoryData(factoryData, factoryDataSize, &mFactoryData))
    // {
    //     ChipLogError(DeviceLayer, "Failed to parse factory data");
    //     return CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    // }
    // mFactoryData.passcode = 0x5585;

    // // Check if factory data version is correct
    // if (mFactoryData.version != CONFIG_CHIP_FACTORY_DATA_VERSION)
    // {
    //     ChipLogError(DeviceLayer, "Factory data version mismatch. Flash version: %d vs code version: %d", mFactoryData.version,
    //                  CONFIG_CHIP_FACTORY_DATA_VERSION);
    //     return CHIP_ERROR_VERSION_MISMATCH;
    // }

    return err;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetCertificationDeclaration(MutableByteSpan & out_cd_buffer)
{
    // const uint8_t kCdForAllExamples[235] = {
    //     0x30, 0x81, 0xe8, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01,
    //     0x07, 0x02, 0xa0, 0x81, 0xda, 0x30, 0x81, 0xd7, 0x02, 0x01, 0x03, 0x31,
    //     0x0d, 0x30, 0x0b, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04,
    //     0x02, 0x01, 0x30, 0x44, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
    //     0x01, 0x07, 0x01, 0xa0, 0x37, 0x04, 0x35, 0x15, 0x24, 0x00, 0x01, 0x25,
    //     0x01, 0x46, 0x13, 0x36, 0x02, 0x05, 0x0e, 0x40, 0x18, 0x25, 0x03, 0x01,
    //     0x01, 0x2c, 0x04, 0x13, 0x5a, 0x49, 0x47, 0x30, 0x30, 0x30, 0x30, 0x30,
    //     0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x24,
    //     0x05, 0x00, 0x24, 0x06, 0x00, 0x24, 0x07, 0x01, 0x24, 0x08, 0x01, 0x18,
    //     0x31, 0x7d, 0x30, 0x7b, 0x02, 0x01, 0x03, 0x80, 0x14, 0x62, 0xfa, 0x82,
    //     0x33, 0x59, 0xac, 0xfa, 0xa9, 0x96, 0x3e, 0x1c, 0xfa, 0x14, 0x0a, 0xdd,
    //     0xf5, 0x04, 0xf3, 0x71, 0x60, 0x30, 0x0b, 0x06, 0x09, 0x60, 0x86, 0x48,
    //     0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86,
    //     0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x04, 0x47, 0x30, 0x45, 0x02, 0x21,
    //     0x00, 0xe5, 0x7d, 0x0f, 0x2e, 0xd8, 0x95, 0x78, 0x19, 0x16, 0x64, 0xf8,
    //     0x9a, 0x81, 0x86, 0x68, 0xd2, 0xad, 0xac, 0x36, 0x20, 0x54, 0x95, 0x77,
    //     0xbc, 0xdd, 0xbc, 0xcf, 0x46, 0xab, 0xa3, 0x18, 0x66, 0x02, 0x20, 0x26,
    //     0x55, 0x59, 0x3a, 0x52, 0x93, 0x38, 0x54, 0xe8, 0x2c, 0xd0, 0x4d, 0xf6,
    //     0x56, 0x66, 0x96, 0xfc, 0x33, 0xe0, 0xc1, 0x05, 0x01, 0x8c, 0x63, 0xe8,
    //     0x6a, 0xe2, 0xfb, 0x7f, 0x8c, 0x2e, 0xb5 
    
    // };
    // return CopySpanToMutableSpan(ByteSpan{ kCdForAllExamples }, out_cd_buffer);

    ReturnErrorCodeIf(out_cd_buffer.size() < mFactoryData.cd.len, CHIP_ERROR_BUFFER_TOO_SMALL);
    ReturnErrorCodeIf(!mFactoryData.cd.data, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);

    memcpy(out_cd_buffer.data(), mFactoryData.cd.data, mFactoryData.cd.len);
    out_cd_buffer.reduce_size(mFactoryData.cd.len);
    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetFirmwareInformation(MutableByteSpan & out_firmware_info_buffer)
{
    out_firmware_info_buffer.reduce_size(0);

    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetDeviceAttestationCert(MutableByteSpan & out_dac_buffer)
{
    ReturnErrorCodeIf(out_dac_buffer.size() < mFactoryData.dac_cert.len, CHIP_ERROR_BUFFER_TOO_SMALL);
    ReturnErrorCodeIf(!mFactoryData.dac_cert.data, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);

    memcpy(out_dac_buffer.data(), mFactoryData.dac_cert.data, mFactoryData.dac_cert.len);
    out_dac_buffer.reduce_size(mFactoryData.dac_cert.len);
    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetProductAttestationIntermediateCert(MutableByteSpan & out_pai_buffer)
{
    ReturnErrorCodeIf(out_pai_buffer.size() < mFactoryData.pai_cert.len, CHIP_ERROR_BUFFER_TOO_SMALL);
    ReturnErrorCodeIf(!mFactoryData.pai_cert.data, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);

    memcpy(out_pai_buffer.data(), mFactoryData.pai_cert.data, mFactoryData.pai_cert.len);
    out_pai_buffer.reduce_size(mFactoryData.pai_cert.len);
    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::SignWithDeviceAttestationKey(const ByteSpan & message_to_sign,
                                                                               MutableByteSpan & out_signature_buffer)
{
    Crypto::P256ECDSASignature signature;
    Crypto::P256Keypair keypair;

    VerifyOrReturnError(out_signature_buffer.size() >= signature.Capacity(), CHIP_ERROR_BUFFER_TOO_SMALL);
    ReturnErrorCodeIf(!mFactoryData.dac_cert.data, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);
    ReturnErrorCodeIf(!mFactoryData.dac_privkey.data, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);

    // Extract public key from DAC cert.
    ByteSpan dacCertSpan{ reinterpret_cast<uint8_t *>(mFactoryData.dac_cert.data), mFactoryData.dac_cert.len };
    chip::Crypto::P256PublicKey dacPublicKey;

    ReturnErrorOnFailure(chip::Crypto::ExtractPubkeyFromX509Cert(dacCertSpan, dacPublicKey));
    ReturnErrorOnFailure(
        LoadKeypairFromRaw(ByteSpan(reinterpret_cast<uint8_t *>(mFactoryData.dac_privkey.data), mFactoryData.dac_privkey.len),
                           ByteSpan(dacPublicKey.Bytes(), dacPublicKey.Length()), keypair));
    ReturnErrorOnFailure(keypair.ECDSA_sign_msg(message_to_sign.data(), message_to_sign.size(), signature));

    return CopySpanToMutableSpan(ByteSpan{ signature.ConstBytes(), signature.Length() }, out_signature_buffer);

    // Crypto::P256ECDSASignature signature;
    // Crypto::P256Keypair keypair;
    // Crypto::P256SerializedKeypair serializedKeypair;

    // VerifyOrReturnError(IsSpanUsable(out_signature_buffer), CHIP_ERROR_INVALID_ARGUMENT);
    // VerifyOrReturnError(IsSpanUsable(message_to_sign), CHIP_ERROR_INVALID_ARGUMENT);
    // VerifyOrReturnError(out_signature_buffer.size() >= signature.Capacity(), CHIP_ERROR_BUFFER_TOO_SMALL);

    // // In a non-exemplary implementation, the public key is not needed here. It is used here merely because
    // // Crypto::P256Keypair is only (currently) constructable from raw keys if both private/public keys are present.
    // // ReturnErrorOnFailure(LoadKeypairFromRaw(DevelopmentCerts::kDacPrivateKey, DevelopmentCerts::kDacPublicKey, keypair));

    // ReturnErrorOnFailure(serializedKeypair.SetLength(DAC_PrivKey_Len + DAC_PubKey_Len));
    // memcpy(serializedKeypair.Bytes(), DAC_PubKey_Array, DAC_PubKey_Len);
    // memcpy(serializedKeypair.Bytes() + DAC_PubKey_Len, DAC_PrivKey_Array, DAC_PrivKey_Len);
    // keypair.Deserialize(serializedKeypair);

    // ReturnErrorOnFailure(keypair.ECDSA_sign_msg(message_to_sign.data(), message_to_sign.size(), signature));

    // return CopySpanToMutableSpan(ByteSpan{ signature.ConstBytes(), signature.Length() }, out_signature_buffer);
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetSetupDiscriminator(uint16_t & setupDiscriminator)
{
    VerifyOrReturnError(mFactoryData.discriminatorPresent, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);
    setupDiscriminator = mFactoryData.discriminator;
    
    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::SetSetupDiscriminator(uint16_t setupDiscriminator)
{
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetSpake2pIterationCount(uint32_t & iterationCount)
{
    ReturnErrorCodeIf(mFactoryData.spake2_it == 0, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);
    iterationCount = mFactoryData.spake2_it;

    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetSpake2pSalt(MutableByteSpan & saltBuf)
{
    static constexpr size_t kSpake2pSalt_MaxBase64Len = BASE64_ENCODED_LEN(chip::Crypto::kSpake2p_Max_PBKDF_Salt_Length) + 1;
    char saltB64[kSpake2pSalt_MaxBase64Len] = { 0 };

    ReturnErrorCodeIf(mFactoryData.spake2_salt.len > sizeof(saltB64), CHIP_ERROR_BUFFER_TOO_SMALL);
    memcpy(saltB64, mFactoryData.spake2_salt.data, mFactoryData.spake2_salt.len);

    VerifyOrReturnError(chip::CanCastTo<uint32_t>(mFactoryData.spake2_salt.len), CHIP_ERROR_INTERNAL);

    size_t saltLen = chip::Base64Decode32(saltB64, static_cast<uint32_t>(mFactoryData.spake2_salt.len), reinterpret_cast<uint8_t *>(saltB64));

    ReturnErrorCodeIf(mFactoryData.spake2_salt.len > saltBuf.size(), CHIP_ERROR_BUFFER_TOO_SMALL);
    memcpy(saltBuf.data(), saltB64, mFactoryData.spake2_salt.len);

    saltBuf.reduce_size(saltLen);

    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetSpake2pVerifier(MutableByteSpan & verifierBuf, size_t & verifierLen)
{
    static constexpr size_t kSpake2pSerializedVerifier_MaxBase64Len =
    BASE64_ENCODED_LEN(chip::Crypto::kSpake2p_VerifierSerialized_Length) + 1;

    char verifierB64[kSpake2pSerializedVerifier_MaxBase64Len] = { 0 };

    ReturnErrorCodeIf(mFactoryData.spake2_verifier.len > sizeof(verifierB64), CHIP_ERROR_BUFFER_TOO_SMALL);
    memcpy(verifierB64, mFactoryData.spake2_verifier.data, mFactoryData.spake2_verifier.len);

    VerifyOrReturnError(chip::CanCastTo<uint32_t>(mFactoryData.spake2_verifier.len), CHIP_ERROR_INTERNAL);
    verifierLen =
    chip::Base64Decode32(verifierB64, static_cast<uint32_t>(mFactoryData.spake2_verifier.len), reinterpret_cast<uint8_t *>(verifierB64));

    ReturnErrorCodeIf(verifierLen > verifierBuf.size(), CHIP_ERROR_BUFFER_TOO_SMALL);
    memcpy(verifierBuf.data(), verifierB64, verifierLen);
    verifierBuf.reduce_size(verifierLen);

    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetSetupPasscode(uint32_t & setupPasscode)
{
    ReturnErrorCodeIf(mFactoryData.passcode == 0, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);
    setupPasscode = mFactoryData.passcode;

    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::SetSetupPasscode(uint32_t setupPasscode)
{
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetVendorName(char * buf, size_t bufSize)
{
    // err("enter %s\r\n", __FUNCTION__);
    // ReturnErrorCodeIf(bufSize < mFactoryData.vendor_name.len + 1, CHIP_ERROR_BUFFER_TOO_SMALL);
    // ReturnErrorCodeIf(!mFactoryData.vendor_name.data, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);

    // memcpy(buf, mFactoryData.vendor_name.data, mFactoryData.vendor_name.len);
    // buf[mFactoryData.vendor_name.len] = 0;

    memcpy(buf, CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME, strlen(CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME) + 1);
    buf[strlen(CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME) + 1] = 0;
    // err("vendor name: %s\r\n", buf);
    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetVendorId(uint16_t & vendorId)
{
    // err("enter %s\r\n", __FUNCTION__);
    // VerifyOrReturnError(mFactoryData.vendorIdPresent, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);
    // vendorId = mFactoryData.vendor_id;
    vendorId = CHIP_DEVICE_CONFIG_DEVICE_VENDOR_ID;
    // err("vid: %x\r\n", vendorId);
    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetProductName(char * buf, size_t bufSize)
{
    // err("enter %s\r\n", __FUNCTION__);
    // ReturnErrorCodeIf(bufSize < mFactoryData.product_name.len + 1, CHIP_ERROR_BUFFER_TOO_SMALL);
    // ReturnErrorCodeIf(!mFactoryData.product_name.data, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);

    // memcpy(buf, mFactoryData.product_name.data, mFactoryData.product_name.len);
    // buf[mFactoryData.product_name.len] = 0;

    memcpy(buf, CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME, strlen(CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME) + 1);
    buf[strlen(CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME) + 1] = 0;
    // err("product name: %s\r\n", CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_NAME);
    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetProductId(uint16_t & productId)
{
    // err("enter %s\r\n", __FUNCTION__);
    // VerifyOrReturnError(mFactoryData.productIdPresent, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);
    // productId = mFactoryData.product_id;
    productId = CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_ID;
    // err("pid: %x\r\n", productId);
    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetPartNumber(char * buf, size_t bufSize)
{
    // err("enter %s\r\n", __FUNCTION__);
    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetProductURL(char * buf, size_t bufSize)
{
    // err("enter %s\r\n", __FUNCTION__);
    memcpy(buf, "https://www.rafaelmicro.com/tw/", strlen("https://www.rafaelmicro.com/tw/") + 1);
    // err("URL: %s\r\n", buf);

    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetProductLabel(char * buf, size_t bufSize)
{
    // err("enter %s\r\n", __FUNCTION__);
    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetSerialNumber(char * buf, size_t bufSize)
{
    // err("enter %s\r\n", __FUNCTION__);
    // ReturnErrorCodeIf(bufSize < mFactoryData.sn.len + 1, CHIP_ERROR_BUFFER_TOO_SMALL);
    // ReturnErrorCodeIf(!mFactoryData.sn.data, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);

    // memcpy(buf, mFactoryData.sn.data, mFactoryData.sn.len);
    // buf[mFactoryData.sn.len] = 0;

    // memcpy(buf, CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER, strlen(CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER) + 1);
    // buf[strlen(CHIP_DEVICE_CONFIG_TEST_SERIAL_NUMBER) + 1] = 0;

    uint8_t temp[256];

    flash_read_sec_register((uint32_t)temp, 0x1100);
    sprintf(buf, "%02x%02x%02x%02x%02x%02x%02x%02x\n", 
        temp[8], temp[9], temp[10], temp[11], temp[12], temp[13], temp[14], temp[15]);

    // err("serial number: %s\r\n", buf);
    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetManufacturingDate(uint16_t & year, uint8_t & month, uint8_t & day)
{
    // err("enter %s\r\n", __FUNCTION__);
    // VerifyOrReturnError(mFactoryData.date_year != 0, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);
    // year  = mFactoryData.date_year;
    // month = mFactoryData.date_month;
    // day   = mFactoryData.date_day;
    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetHardwareVersion(uint16_t & hardwareVersion)
{
    // err("enter %s\r\n", __FUNCTION__);
    // VerifyOrReturnError(mFactoryData.hwVerPresent, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);
    // hardwareVersion = mFactoryData.hw_ver;
    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetHardwareVersionString(char * buf, size_t bufSize)
{
    // err("enter %s\r\n", __FUNCTION__);
    // ReturnErrorCodeIf(bufSize < mFactoryData.hw_ver_str.len + 1, CHIP_ERROR_BUFFER_TOO_SMALL);
    // ReturnErrorCodeIf(!mFactoryData.hw_ver_str.data, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);

    // memcpy(buf, mFactoryData.hw_ver_str.data, mFactoryData.hw_ver_str.len);
    // buf[mFactoryData.hw_ver_str.len] = 0;

    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetRotatingDeviceIdUniqueId(MutableByteSpan & uniqueIdSpan)
{
    // err("enter %s\r\n", __FUNCTION__);
    // ReturnErrorCodeIf(uniqueIdSpan.size() < mFactoryData.rd_uid.len, CHIP_ERROR_BUFFER_TOO_SMALL);
    // ReturnErrorCodeIf(!mFactoryData.rd_uid.data, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);

    // memcpy(uniqueIdSpan.data(), mFactoryData.rd_uid.data, mFactoryData.rd_uid.len);

    return CHIP_NO_ERROR;
}

template <class FlashFactoryData>
CHIP_ERROR FactoryDataProvider<FlashFactoryData>::GetEnableKey(MutableByteSpan & enableKey)
{
    // err("enter %s\r\n", __FUNCTION__);
    // ReturnErrorCodeIf(!mFactoryData.enable_key.data, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);
    // ReturnErrorCodeIf(enableKey.size() < mFactoryData.enable_key.len, CHIP_ERROR_BUFFER_TOO_SMALL);

    // memcpy(enableKey.data(), mFactoryData.enable_key.data, mFactoryData.enable_key.len);

    // enableKey.reduce_size(mFactoryData.enable_key.len);

    return CHIP_NO_ERROR;
}

// Fully instantiate the template class in whatever compilation unit includes this file.
template class FactoryDataProvider<InternalFlashFactoryData>;
template class FactoryDataProvider<ExternalFlashFactoryData>;

} // namespace DeviceLayer
} // namespace chip
