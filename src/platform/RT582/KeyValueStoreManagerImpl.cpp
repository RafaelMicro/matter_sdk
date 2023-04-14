/*
 *
 *    Copyright (c) 2021-2022 Project CHIP Authors
 *    All rights reserved.
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

/**
 *    @file
 *          Platform-specific key value storage implementation for RT582
 */


#if 0

#include <lib/support/CHIPMemString.h>
#include <lib/support/CodeUtils.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/RT582/RT582Config.h>
#include <platform/KeyValueStoreManager.h>
#include <stdio.h>
#include <string.h>
// #include "util_log.h"
#include "cm3_mcu.h"
#include <platform/internal/CHIPDeviceLayerInternal.h>
#include <lib/support/CHIPMem.h>

using namespace ::chip;
using namespace ::chip::DeviceLayer::Internal;

namespace chip {
namespace DeviceLayer {
namespace PersistedStorage {

typedef struct __attribute__((packed)) {
    uint8_t valid;
    uint8_t keyId;
    char keyString[32];
} KeyValueStore_t;

// typedef struct __attribute__((packed)) {
//     uint8_t valid;
// } KeyPageMapping_t;

// constexpr size_t kMaxNumberOfKeys  = 0x3f;
// constexpr size_t kMaxKeyValueBytes = 255;

constexpr size_t kMaxNumberOfKeys  = 0x3f;
constexpr size_t kvsMappingTableSize = sizeof(KeyValueStore_t);
static KeyValueStore_t kvs[kMaxNumberOfKeys];

KeyValueStoreManagerImpl KeyValueStoreManagerImpl::sInstance;

KeyValueStoreManagerImpl::KeyValueStoreManagerImpl() {
    uint32_t pageIndex = 0;

    // vPortEnterCritical();
    for (uint16_t keyValueStoreIndex = 0; keyValueStoreIndex < kMaxNumberOfKeys; keyValueStoreIndex++) {
        uint32_t byteIndex = 0;

        kvs[keyValueStoreIndex].valid = flash_read_byte(0xFB000 + pageIndex + byteIndex++);
        while (flash_check_busy()) {}
        kvs[keyValueStoreIndex].keyId = flash_read_byte(0xFB000 + pageIndex + byteIndex++);
        while (flash_check_busy()) {}
        for (uint8_t keyLength = 0; keyLength < 32; keyLength++) {
            kvs[keyValueStoreIndex].keyString[keyLength] = flash_read_byte(0xFB000 + pageIndex + byteIndex++);
            while (flash_check_busy()) {}
        }
        pageIndex += kvsMappingTableSize;
    }
    // vPortExitCritical();
}

CHIP_ERROR KeyValueStoreManagerImpl::_Get(const char * key, void * value, size_t value_size, size_t * read_bytes_size,
                                          size_t offset_bytes)
{
    CHIP_ERROR err         = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    uint8_t pdmIdKvsValue  = chip::DeviceLayer::Internal::RT582Config::kMatterKvs_KeyValue;
    size_t read_bytes      = 0;
    uint8_t keyId          = 0;
    uint16_t pdmInternalId = 0;

    // VerifyOrExit((key != NULL) && (value != NULL), err = CHIP_ERROR_INVALID_ARGUMENT);

    uint32_t pageIndex = 0;

    // vPortEnterCritical();
    // for (uint16_t keyValueStoreIndex = 0; keyValueStoreIndex < kMaxNumberOfKeys; keyValueStoreIndex++) {
    //     uint32_t byteIndex = 0;

    //     kvs[keyValueStoreIndex].valid = flash_read_byte(0xFB000 + pageIndex + byteIndex++);
    //     while (flash_check_busy()) {}
    //     kvs[keyValueStoreIndex].keyId = flash_read_byte(0xFB000 + pageIndex + byteIndex++);
    //     while (flash_check_busy()) {}
    //     for (uint8_t keyLength = 0; keyLength < 32; keyLength++) {
    //         kvs[keyValueStoreIndex].keyString[keyLength] = flash_read_byte(0xFB000 + pageIndex + byteIndex++);
    //         while (flash_check_busy()) {}
    //     }
    //     pageIndex += kvsMappingTableSize;
    // }
    // vPortExitCritical();

    for (keyId = 0; keyId < kMaxNumberOfKeys; keyId++) {
        // info("===> check key[%d]: %s\r\n", keyId, kvs[keyId].keyString);
        if (!strcmp(key, kvs[keyId].keyString)) {
            // info("===> key found[%d]: %s\r\n", keyId, kvs[keyId].keyString);   
            break;
        }
    }

    if (keyId < kMaxNumberOfKeys)
    {
        pdmInternalId = chip::DeviceLayer::Internal::RT582ConfigKey(pdmIdKvsValue, kvs[keyId].keyId);
        err = chip::DeviceLayer::Internal::RT582Config::ReadConfigValueBin(pdmInternalId, (uint8_t *)value, value_size, read_bytes);
        *read_bytes_size = read_bytes;
    //     info("===> Take Matter key [%s] with PDM id: 0x%02x\r\n", key, pdmInternalId);
    //     info("===> Matter key[%d]: %s\r\n", strlen(key) + 1, key);
    //     info("===> Matter value[%d]: ", read_bytes);
    //     for (int i = 0; i < read_bytes; i++) {
    //         info("%02x", *(char *)value + i);
    //     }
    //     info("\r\n");
    // }

exit:
    ConvertError(err);
    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Put(const char * key, const void * value, size_t value_size)
{
    CHIP_ERROR err         = CHIP_ERROR_INVALID_ARGUMENT;
    bool putKey            = false;
    uint8_t pdmIdKvsKey    = chip::DeviceLayer::Internal::RT582Config::kMatterKvs_KeyBase;
    uint8_t pdmIdKvsValue  = chip::DeviceLayer::Internal::RT582Config::kMatterKvs_KeyValue;
    uint16_t pdmInternalId = 0;
    uint16_t freeKeyId;
    uint8_t keyId = 0;
    uint8_t keyLength = 0;

    // VerifyOrExit((key != NULL) && (value != NULL), err = CHIP_ERROR_INVALID_ARGUMENT);
    
    uint32_t pageIndex = 0;

    // vPortEnterCritical();
    // for (uint16_t keyValueStoreIndex = 0; keyValueStoreIndex < kMaxNumberOfKeys; keyValueStoreIndex++) {
    //     uint32_t byteIndex = 0;

    //     kvs[keyValueStoreIndex].valid = flash_read_byte(0xFB000 + pageIndex + byteIndex++);
    //     while (flash_check_busy()) {}
    //     if (kvs[keyValueStoreIndex].valid != 0x00) continue;
    //     kvs[keyValueStoreIndex].keyId = flash_read_byte(0xFB000 + pageIndex + byteIndex++);
    //     while (flash_check_busy()) {}
    //     for (uint8_t keyLength = 0; keyLength < 32; keyLength++) {
    //         kvs[keyValueStoreIndex].keyString[keyLength] = flash_read_byte(0xFB000 + pageIndex + byteIndex++);
    //         while (flash_check_busy()) {}
    //     }
    //     pageIndex += kvsMappingTableSize;
    // }
    // vPortExitCritical();
    
    for (keyId = 0; keyId < kMaxNumberOfKeys; keyId++) {
        // info("===> check key[%d]: %s\r\n", keyId, kvs[keyId].keyString);
        if (!strcmp(key, kvs[keyId].keyString)) {
            // info("===> key found[%d]: %s\r\n", keyId, kvs[keyId].keyString);   
            break;
        }
        if (kvs[keyId].valid == 0xff) {
            kvs[keyId].valid = 0x00;
            kvs[keyId].keyId = keyId+1;
            memcpy(kvs[keyId].keyString, key, strlen(key) + 1);
            break;
        }
    } 

    if (keyId < kMaxNumberOfKeys) {
        pdmInternalId = chip::DeviceLayer::Internal::RT582ConfigKey(pdmIdKvsValue, kvs[keyId].keyId);
        // info("===> Set Matter key [%s] with PDM id: 0x%02x\r\n", key, pdmInternalId);
        // info("===> Matter key[%d]: %s\r\n", strlen(key) + 1, key);
        // info("===> Matter value[%d]: ", value_size);
        // for (int i = 0; i < value_size; i++) {
        //     info("%02x", *(char *)value + i);
        // }
        // info("\r\n");
        err = chip::DeviceLayer::Internal::RT582Config::WriteConfigValueBin(pdmInternalId, (uint8_t *)value, value_size);
    }

    pageIndex = 0;

    // vPortEnterCritical();
    // flash_erase(FLASH_ERASE_SECTOR, 0xFB000);
    // while (flash_check_busy()) {}
    // for (uint16_t keyValueStoreIndex = 0; keyValueStoreIndex < kMaxNumberOfKeys; keyValueStoreIndex++) {
    //     uint8_t byteIndex = 0;

    //     // if (kvs[keyValueStoreIndex].valid != 0x00) continue;
    //     flash_write_byte(0xFB000 + pageIndex + byteIndex, kvs[keyValueStoreIndex].valid);
    //     while (flash_check_busy()) {}
    //     byteIndex++;
    //     flash_write_byte(0xFB000 + pageIndex + byteIndex, kvs[keyValueStoreIndex].keyId);
    //     while (flash_check_busy()) {}
    //     byteIndex++;
    //     for (uint8_t keyLength = 0; keyLength < 32; keyLength++) {
    //         flash_write_byte(0xFB000 + pageIndex + byteIndex, kvs[keyValueStoreIndex].keyString[keyLength]);
    //         while (flash_check_busy()) {}
    //         byteIndex++;
    //     }
    //     pageIndex += kvsMappingTableSize;
    // }
    // vPortExitCritical();

exit:
    ConvertError(err);
    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Delete(const char * key)
{
    CHIP_ERROR err         = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    // CHIP_ERROR err         = CHIP_NO_ERROR;
    uint8_t pdmIdKvsKey    = chip::DeviceLayer::Internal::RT582Config::kMatterKvs_KeyBase;
    uint8_t pdmIdKvsValue  = chip::DeviceLayer::Internal::RT582Config::kMatterKvs_KeyValue;
    uint8_t keyId          = 0;
    uint16_t pdmInternalId = 0;

    // VerifyOrExit((key != NULL), err = CHIP_ERROR_INVALID_ARGUMENT);

    uint32_t pageIndex = 0;

    // vPortEnterCritical();
    // for (uint16_t keyValueStoreIndex = 0; keyValueStoreIndex < kMaxNumberOfKeys; keyValueStoreIndex++) {
    //     uint32_t byteIndex = 0;

    //     kvs[keyValueStoreIndex].valid = flash_read_byte(0xFB000 + pageIndex + byteIndex++);
    //     while (flash_check_busy()) {}
    //     kvs[keyValueStoreIndex].keyId = flash_read_byte(0xFB000 + pageIndex + byteIndex++);
    //     while (flash_check_busy()) {}
    //     for (uint8_t keyLength = 0; keyLength < 32; keyLength++) {
    //         kvs[keyValueStoreIndex].keyString[keyLength] = flash_read_byte(0xFB000 + pageIndex + byteIndex++);
    //         while (flash_check_busy()) {}
    //     }
    //     pageIndex += kvsMappingTableSize;
    // }
    // vPortExitCritical();

    for (keyId = 0; keyId < kMaxNumberOfKeys; keyId++) {
        if (!strcmp(kvs[keyId].keyString, key)) {
            kvs[keyId].valid = 0xff;
            // kvs[keyId].keyId = 0x00;
            memset(kvs[keyId].keyString, 0, 32);
            break;
        }
    }

    if (keyId < kMaxNumberOfKeys)
    {
        pdmInternalId = chip::DeviceLayer::Internal::RT582ConfigKey(pdmIdKvsValue, kvs[keyId].keyId);
        // info("===> Delete the Matter key [%s] with PDM id: 0x%02x\r\n", key, pdmInternalId);
        err = chip::DeviceLayer::Internal::RT582Config::ClearConfigValue(pdmInternalId);
        kvs[keyId].keyId = 0x00;
    }

    pageIndex = 0;
    // vPortEnterCritical();
    // flash_erase(FLASH_ERASE_SECTOR, 0xFB000);
    // while (flash_check_busy()) {}
    // for (uint16_t keyValueStoreIndex = 0; keyValueStoreIndex < kMaxNumberOfKeys; keyValueStoreIndex++) {
    //     uint8_t byteIndex = 0;

    //     flash_write_byte(0xFB000 + pageIndex + byteIndex, kvs[keyValueStoreIndex].valid);
    //     while (flash_check_busy()) {}
    //     byteIndex++;
    //     flash_write_byte(0xFB000 + pageIndex + byteIndex, kvs[keyValueStoreIndex].keyId);
    //     while (flash_check_busy()) {}
    //     byteIndex++;
    //     for (uint8_t keyLength = 0; keyLength < 32; keyLength++) {
    //         flash_write_byte(0xFB000 + pageIndex + byteIndex, kvs[keyValueStoreIndex].keyString[keyLength]);
    //         while (flash_check_busy()) {}
    //         byteIndex++;
    //     }
    //     pageIndex += kvsMappingTableSize;
    // }
    // vPortExitCritical();
exit:
    ConvertError(err);
    return err;
}

void KeyValueStoreManagerImpl::ConvertError(CHIP_ERROR & err)
{
    if (err == CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND)
    {
        err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }
}

} // namespace PersistedStorage
} // namespace DeviceLayer
} // namespace chip


#else


#include <lib/support/CHIPMemString.h>
#include <lib/support/CodeUtils.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/RT582/RT582Config.h>
#include <platform/KeyValueStoreManager.h>
#include <stdio.h>
#include <string.h>
#include "util_log.h"

#include <platform/internal/CHIPDeviceLayerInternal.h>
#include <lib/support/CHIPMem.h>


using namespace ::chip;
using namespace ::chip::DeviceLayer::Internal;

namespace chip {
namespace DeviceLayer {
namespace PersistedStorage {

constexpr size_t kMaxNumberOfKeys  = 0x3b;
constexpr size_t kMaxKeyValueBytes = 255;

KeyValueStoreManagerImpl KeyValueStoreManagerImpl::sInstance;

uint16_t GetStringKeyId(const char * key, uint16_t * freeId)
{
    CHIP_ERROR err                    = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    uint8_t keyId                     = 0;
    uint8_t pdmIdKvsKey               = chip::DeviceLayer::Internal::RT582Config::kMatterKvs_KeyBase;
    bool bFreeIdxFound                = false;
    char keyString[kMaxKeyValueBytes] = { 0 };
    size_t keyStringSize              = 0;
    uint16_t pdmInternalId;

    for (keyId = 0; keyId < kMaxNumberOfKeys; keyId++)
    {
        pdmInternalId = chip::DeviceLayer::Internal::RT582ConfigKey(pdmIdKvsKey, keyId);
        err = chip::DeviceLayer::Internal::RT582Config::ReadConfigValueStr(pdmInternalId, keyString, kMaxKeyValueBytes, keyStringSize);

        if (err == CHIP_NO_ERROR)
        {
            if (strcmp(key, keyString) == 0)
            {
                // found the entry we are looking for
                break;
            }
        }
        else if ((NULL != freeId) && (false == bFreeIdxFound))
        {
            bFreeIdxFound = true;
            *freeId       = keyId;
        }
    }
    return keyId;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Get(const char * key, void * value, size_t value_size, size_t * read_bytes_size,
                                          size_t offset_bytes)
{
    CHIP_ERROR err         = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    uint8_t pdmIdKvsValue  = chip::DeviceLayer::Internal::RT582Config::kMatterKvs_KeyValue;
    size_t read_bytes      = 0;
    uint8_t keyId          = 0;
    uint16_t pdmInternalId = 0;

    VerifyOrExit((key != NULL) && (value != NULL), err = CHIP_ERROR_INVALID_ARGUMENT);

    keyId = GetStringKeyId(key, NULL);

    //ChipLogDetail(DeviceLayer, "KVS, get keyId: 0x%02x", keyId);

    if (keyId < kMaxNumberOfKeys)
    {
        // This is the ID of the actual data
        pdmInternalId = chip::DeviceLayer::Internal::RT582ConfigKey(pdmIdKvsValue, keyId);
        //ChipLogDetail(DeviceLayer, "KVS, get the value of Matter key [%s] with PDM id: 0x%02x", key, pdmInternalId);
        // info("===> Take Matter key [%s] with PDM id: 0x%02x\r\n", key, pdmInternalId);
        // info("===> Matter value[%d]: ", value_size);
        // for (int i = 0; i < value_size; i++) {
        //     info("%02x", *(char *)value + i);
        // }
        // info("\r\n");
        err = chip::DeviceLayer::Internal::RT582Config::ReadConfigValueBin(pdmInternalId, (uint8_t *) value, value_size, read_bytes);
        *read_bytes_size = read_bytes;
    }
    else
    {
       //ChipLogDetail(DeviceLayer, "KVS, Matter key [%s] not found in persistent storage.", key);
    }

exit:
    ConvertError(err);
    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Put(const char * key, const void * value, size_t value_size)
{
    CHIP_ERROR err         = CHIP_ERROR_INVALID_ARGUMENT;
    bool putKey            = false;
    uint8_t pdmIdKvsKey    = chip::DeviceLayer::Internal::RT582Config::kMatterKvs_KeyBase;
    uint8_t pdmIdKvsValue  = chip::DeviceLayer::Internal::RT582Config::kMatterKvs_KeyValue;
    uint16_t pdmInternalId = 0;
    uint16_t freeKeyId;
    uint8_t keyId;

    VerifyOrExit((key != NULL) && (value != NULL), err = CHIP_ERROR_INVALID_ARGUMENT);

    keyId = GetStringKeyId(key, &freeKeyId);

    // Key does not exist. Write both key and value in persistent storage.
    if (kMaxNumberOfKeys == keyId)
    {
        putKey = true;
        keyId  = freeKeyId;
    }

    pdmInternalId = chip::DeviceLayer::Internal::RT582ConfigKey(pdmIdKvsValue, keyId);
    // ChipLogDetail(DeviceLayer, "KVS, save in flash the value of the Matter key [%s] with PDM id: 0x%02x", key, pdmInternalId);
    // info("===> Set Matter key [%s] with PDM id: 0x%02x\r\n", key, pdmInternalId);
    // info("===> Matter value[%d]: ", value_size);
    // for (int i = 0; i < value_size; i++) {
    //     info("%02x", *(char *)value + i);
    // }
    // info("\r\n");

    err = chip::DeviceLayer::Internal::RT582Config::WriteConfigValueBin(pdmInternalId, (uint8_t *) value, value_size);

    /* save the 'key' in flash such that it can be retrieved later on */
    if (err == CHIP_NO_ERROR)
    {
        if (true == putKey)
        {
            pdmInternalId = chip::DeviceLayer::Internal::RT582ConfigKey(pdmIdKvsKey, keyId);
            //ChipLogDetail(DeviceLayer, "KVS, save in flash the Matter key [%s] with PDM id: 0x%02x", key, pdmInternalId);
            err = chip::DeviceLayer::Internal::RT582Config::WriteConfigValueStr(pdmInternalId, key, strlen(key) + 1);

            if (err != CHIP_NO_ERROR)
            {
                //ChipLogDetail(DeviceLayer, "KVS, Error while saving in flash the Matter key [%s] with PDM id: 0x%02x", key, pdmInternalId);
            }
        }
    }
    else
    {
        //ChipLogDetail(DeviceLayer, "KVS, Error while saving in flash the value of the Matter key [%s] with PDM id: 0x%02x, CHIPERROR: %" CHIP_ERROR_FORMAT, key, pdmInternalId, err.Format());
    }

exit:
    ConvertError(err);
    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Delete(const char * key)
{
    CHIP_ERROR err         = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    uint8_t pdmIdKvsKey    = chip::DeviceLayer::Internal::RT582Config::kMatterKvs_KeyBase;
    uint8_t pdmIdKvsValue  = chip::DeviceLayer::Internal::RT582Config::kMatterKvs_KeyValue;
    uint8_t keyId          = 0;
    uint16_t pdmInternalId = 0;

    VerifyOrExit((key != NULL), err = CHIP_ERROR_INVALID_ARGUMENT);

    keyId = GetStringKeyId(key, NULL);

    if (keyId < kMaxNumberOfKeys)
    {
        // entry exists so we can remove it
        pdmInternalId = chip::DeviceLayer::Internal::RT582ConfigKey(pdmIdKvsKey, keyId);

        //ChipLogDetail(DeviceLayer, "KVS, delete from flash the Matter key [%s] with PDM id: 0x%02x", key, pdmInternalId);
        err = chip::DeviceLayer::Internal::RT582Config::ClearConfigValue(pdmInternalId);

        /* also delete the 'key string' from flash */
        if (err == CHIP_NO_ERROR)
        {
            pdmInternalId = chip::DeviceLayer::Internal::RT582ConfigKey(pdmIdKvsValue, keyId);
            ////ChipLogDetail(DeviceLayer, "KVS, delete from flash the value of the Matter key [%s] with PDM id: 0x%02x", key, pdmInternalId);
            // info("===> Delete the Matter key [%s] with PDM id: 0x%02x\r\n", key, pdmInternalId);

            err = chip::DeviceLayer::Internal::RT582Config::ClearConfigValue(pdmInternalId);

            if (err != CHIP_NO_ERROR)
            {
                //ChipLogDetail(DeviceLayer, "KVS, Error while deleting from flash the value of the Matter key [%s] with PDM id: 0x%02x", key, pdmInternalId);
            }
        }
        else
        {
            //ChipLogDetail(DeviceLayer, "KVS, Error while deleting from flash the Matter key [%s] with PDM id: 0x%02x", key, pdmInternalId);
        }
    }
exit:
    ConvertError(err);
    return err;
}

void KeyValueStoreManagerImpl::ConvertError(CHIP_ERROR & err)
{
    if (err == CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND)
    {
        err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }
}

} // namespace PersistedStorage
} // namespace DeviceLayer
} // namespace chip


#endif