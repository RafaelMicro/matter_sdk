/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2019 Nest Labs, Inc.
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
 *          Utilities for accessing persisted device configuration on
 *          platforms based on the Silicon Labs SDK.
 */



/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>
#include <platform/RT582/RT582Config.h>
#include <lib/core/CHIPEncoding.h>

// #include "util_log.h"
#include "cm3_mcu.h"
#include "chip_define.h"
#include "EnhancedFlashDataset.h"


namespace chip {
namespace DeviceLayer {
namespace Internal {



CHIP_ERROR RT582Config::Init()
{
    return CHIP_NO_ERROR;
}

template <typename T>
CHIP_ERROR RT582Config::ReadConfigValue(Key key, T & val)
{
    size_t read_count;
    ReturnErrorOnFailure(ReadConfigValueBin(key, &val, sizeof(val), read_count));
    VerifyOrReturnError(sizeof(val) == read_count, CHIP_ERROR_PERSISTED_STORAGE_FAILED);

    return CHIP_NO_ERROR;
}

CHIP_ERROR RT582Config::ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen)
{
    return ReadConfigValueBin(key, buf, bufSize, outLen);
}

CHIP_ERROR RT582Config::ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen)
{
    return ReadConfigValueBin(key, static_cast<void *>(buf), bufSize, outLen);
}

CHIP_ERROR RT582Config::ReadConfigValueBin(Key key, void * buf, size_t bufSize, size_t & outLen)
{
    //outLen = storage_read(key, bufSize, (uint8_t *)buf);

    //if (outLen == 0)
    //    return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;

    //ChipLogProgress(DeviceLayer, "Read Key %s, len %d", key, bufSize);

    env_node_obj node;

    efd_port_env_lock();

    if (true == efd_get_env_obj(key, &node))
    {
        outLen = 0;
        if (buf && bufSize)
        {
            if (bufSize > node.value_len)
            {
                outLen = node.value_len;
            }
            else
            {
                outLen = bufSize;
            }
        }

        efd_port_read(node.addr.value, (uint32_t *) buf, outLen);

        efd_port_env_unlock();

        return CHIP_NO_ERROR;
    }

    efd_port_env_unlock();

    return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    
    return CHIP_NO_ERROR;
}
template CHIP_ERROR RT582Config::ReadConfigValue(Key key, bool & val);
template CHIP_ERROR RT582Config::ReadConfigValue(Key key, uint32_t & val);
template CHIP_ERROR RT582Config::ReadConfigValue(Key key, uint64_t & val);

template <typename T>
CHIP_ERROR RT582Config::WriteConfigValue(Key key, T val)
{
    return WriteConfigValueBin(key, &val, sizeof(val));
}


template CHIP_ERROR RT582Config::WriteConfigValue(Key key, bool val);
template CHIP_ERROR RT582Config::WriteConfigValue(Key key, uint32_t val);
template CHIP_ERROR RT582Config::WriteConfigValue(Key key, uint64_t val);

CHIP_ERROR RT582Config::WriteConfigValueStr(Key key, const char * str)
{
    return WriteConfigValueStr(key, str, (str != NULL) ? strlen(str) : 0);
}

CHIP_ERROR RT582Config::WriteConfigValueStr(Key key, const char * str, size_t strLen)
{
    return WriteConfigValueBin(key, str, strLen);
}

CHIP_ERROR RT582Config::WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen)
{
    return WriteConfigValueBin(key, static_cast<const void *>(data), dataLen);
}

CHIP_ERROR RT582Config::WriteConfigValueBin(Key key, const void * data, size_t dataLen)
{
    //storage_write(key, dataLen, (uint8_t *)data);

    //ChipLogProgress(DeviceLayer, "Wrire Key %s, len %d", key, dataLen);
    EfErrCode ret = EFD_NO_ERR;

    efd_port_env_lock();

    if (dataLen && data)
    {
        ret = efd_set_env_blob(key, data, dataLen);
    }

    efd_port_env_unlock();

    if (ret == EFD_NO_ERR)
    {
        return CHIP_NO_ERROR;
    }
    else
    {
        return CHIP_ERROR_PERSISTED_STORAGE_FAILED;
    }    
    return CHIP_NO_ERROR;
}

CHIP_ERROR RT582Config::ClearConfigValue(Key key)
{
    ////ChipLogDetail(DeviceLayer, "RT582Config %s, %u", __func__, key);

    return EFD_NO_ERR == efd_del_env(key) ? CHIP_NO_ERROR : CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;

    return CHIP_NO_ERROR;
}

bool RT582Config::ConfigValueExists(Key key)
{
    env_node_obj node;

    return efd_get_env_obj(key, &node);
}

template <typename T>
bool RT582Config::ConfigValueExists(Key key, T & val)
{
    return ChipError::IsSuccess(ReadConfigValue(key, val));
}

CHIP_ERROR RT582Config::FactoryResetConfig(void)
{

#if 0
    vTaskSuspendAll();

    for (int address = RT582CONFIG_BASE_ADDR; address < RT582CONFIG_END_ADDR; address += RT582CONFIG_SECTOR_SIZE ) {
        flash_erase(FLASH_ERASE_SECTOR, address);
        while (flash_check_busy()) {}
    }

    ds_reset_to_default();

    xTaskResumeAll();
#endif

    efd_env_set_default();

    return CHIP_NO_ERROR;
}

CHIP_ERROR RT582Config::ReadKVS(const char * key, void * value, size_t value_size, size_t * read_bytes_size, size_t offset_bytes)
{
    size_t read_len = 0, datalen;
    env_node_obj node;

    efd_port_env_lock();

    if (true == efd_get_env_obj(key, &node))
    {
        if (offset_bytes > node.value_len)
        {
            efd_port_env_unlock();
            return CHIP_ERROR_INVALID_ARGUMENT;
        }

        datalen = node.value_len - offset_bytes;
        if (value_size && value)
        {

            if (value_size > datalen)
            {
                read_len = datalen;
            }
            else
            {
                read_len = value_size;
            }

            efd_port_read(node.addr.value + offset_bytes, (uint32_t *) value, read_len);

            if (read_bytes_size)
            {
                *read_bytes_size = read_len;
            }
        }
        else
        {
            if (read_bytes_size)
            {
                *read_bytes_size = 0;
            }
        }

        efd_port_env_unlock();

        return CHIP_NO_ERROR;
    }

    efd_port_env_unlock();

    return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
}


CHIP_ERROR RT582Config::WriteKVS(const char * key, const void * value, size_t value_size)
{
    //ChipLogProgress(DeviceLayer, "Wrire KVS Key %s, len %d", key, value_size);
    EfErrCode ret = EFD_NO_ERR;

    efd_port_env_lock();

    if (value && value_size)
    {
        ret = efd_set_env_blob(key, value, value_size);
    }

    efd_port_env_unlock();

    if (ret == EFD_NO_ERR)
    {
        return CHIP_NO_ERROR;
    }
    else
    {
        return CHIP_ERROR_PERSISTED_STORAGE_FAILED;
    }
}

CHIP_ERROR RT582Config::ClearKVS(const char * key)
{
    efd_port_env_lock();

    efd_del_env(key);

    efd_port_env_unlock();

    return CHIP_NO_ERROR;
}


void RT582Config::RunConfigUnitTest()
{
    // Run common unit test.
    //::chip::DeviceLayer::Internal::RunConfigUnitTest<RT582Config>();
}
} // namespace Internal
} // namespace DeviceLayer
} // namespace chip