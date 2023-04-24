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


#if 0

/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>
#include <platform/RT582/RT582Config.h>
#include <lib/core/CHIPEncoding.h>

#include "util_log.h"
#include "cm3_mcu.h"

#define RT582CONFIG_BASE_ADDR       0xF3000
#define RT582CONFIG_END_ADDR        0xFB000
#define RT582CONFIG_ID_PER_SIZE     0x100
#define RT582CONFIG_FLASH_PAGE_SIZE 0x100
#define RT582CONFIG_SECTOR_SIZE     0x1000

#define RT582CONFIG_USE_FLASH_DS    1

namespace chip {
namespace DeviceLayer {
namespace Internal {

// static uint8_t storage_backup[RT582CONFIG_SECTOR_SIZE];
static size_t storage_read(uint32_t id, size_t bufSize, uint8_t *buf)
{
#if RT582CONFIG_USE_FLASH_DS
    ds_rw_t t_ds_r;
    uint32_t ds_ret = 0;
    uint32_t flash_keyid = RT582Config::RT582KeyaddrPasser(id);
    // ChipLogDetail(DeviceLayer, "storage_read key id: 0x%02x, flash id: 0x%02x", id, flash_keyid);
    t_ds_r.type = flash_keyid;
    t_ds_r.len = 0;
    t_ds_r.address = 0;

    ds_ret = ds_read(&t_ds_r);

    if (ds_ret == STATUS_SUCCESS) {
        for (int i = 0; i < t_ds_r.len; i++) {
            buf[i] = flash_read_byte(t_ds_r.address + i);
        }
    }
    return t_ds_r.len;
#else
    uint32_t sector_addr, id_addr, i, offset, chk, data_size;
    uint32_t flash_keyid = RT582Config::RT582KeyaddrPasser(id);
    ChipLogDetail(DeviceLayer, "storage_read key id: 0x%02x, flash id: 0x%02x", id, flash_keyid);

    id_addr = RT582CONFIG_BASE_ADDR + (RT582CONFIG_ID_PER_SIZE * flash_keyid);
    sector_addr = id_addr - (id_addr % RT582CONFIG_SECTOR_SIZE);

    for(i=0;i<16;i++)
    {
        flash_read_page_syncmode((uint32_t)&storage_backup[i*RT582CONFIG_FLASH_PAGE_SIZE], (sector_addr+(i*RT582CONFIG_FLASH_PAGE_SIZE)));
    }
    offset = (RT582CONFIG_ID_PER_SIZE*flash_keyid) % 0x1000;

    if((bufSize < storage_backup[offset]) && (storage_backup[offset] != 0xFF))
    {
        //ChipLogDetail(DeviceLayer, "Cfg  %s !!!!!!!!!!", __func__); 
        return 0xFFFFFFFF;
    }

    if(storage_backup[offset] == 0xFF)
    {
        //ChipLogDetail(DeviceLayer, "Cfg  %s id 0x%02x not found", __func__, id); 
        return 0xFFFFFFFE;
    }

    memcpy(buf, &storage_backup[offset+1], storage_backup[offset]);

    //if(chk == (bufSize-1))
    //    return 0;
    
    return storage_backup[offset];
#endif
}

size_t storage_write(uint32_t id, size_t dataLen, uint8_t *data)
{
#if RT582CONFIG_USE_FLASH_DS
    uint32_t ds_ret = 0;
    ds_rw_t t_ds_w, t_ds_r;;
    uint32_t flash_keyid = RT582Config::RT582KeyaddrPasser(id);
    // ChipLogDetail(DeviceLayer, "storage_write key id: 0x%02x, flash id: 0x%02x size %d", id, flash_keyid, dataLen);
    t_ds_w.type = flash_keyid;
    t_ds_w.len = dataLen;
    t_ds_w.address = (uint32_t)data;

    // t_ds_r.type = flash_keyid;
    // t_ds_r.len = 0;
    // t_ds_r.address = 0;

    // ds_ret = ds_read(&t_ds_r);

    // if (ds_ret == STATUS_SUCCESS) {
    //     info("===> same key id[%02x] address: %08x, len: %04x\r\n", flash_keyid, t_ds_r.address, t_ds_r.len);
    //     t_ds_w.location = t_ds_r.address - (t_ds_r.address % 0x100);
    // }
    // else {
        // t_ds_w.location = 0;
    // }
    if (dataLen > 0) {

        ds_ret = ds_write(&t_ds_w);

        if (ds_ret != STATUS_SUCCESS) {
            ChipLogError(DeviceLayer, "ds_write failed %d", ds_ret);
            while(1);
        }
        else {
            vTaskSuspendAll();
            flush_cache();
            xTaskResumeAll();
        }
    }
#else
    uint32_t sector_addr, id_addr, i, offset;
    uint32_t flash_keyid = RT582Config::RT582KeyaddrPasser(id);
    //ChipLogDetail(DeviceLayer, "storage_write key id: 0x%02x, flash id: 0x%02x", id, flash_keyid);

    id_addr = RT582CONFIG_BASE_ADDR+(RT582CONFIG_ID_PER_SIZE*flash_keyid);
    sector_addr = id_addr - (id_addr % RT582CONFIG_SECTOR_SIZE);

    for(i=0;i<16;i++)
    {
        flash_read_page_syncmode((uint32_t)&storage_backup[i*RT582CONFIG_FLASH_PAGE_SIZE], (sector_addr+(i*RT582CONFIG_FLASH_PAGE_SIZE)));
    }
    while (flash_check_busy());
    vPortEnterCritical();
    flash_erase(FLASH_ERASE_SECTOR, sector_addr);
    vPortExitCritical();
    while (flash_check_busy());

    offset = (RT582CONFIG_ID_PER_SIZE*flash_keyid) % 0x1000;

    storage_backup[offset] = dataLen;
    memcpy(&storage_backup[offset+1], data, dataLen);

    for(i=0;i<16;i++)
    {
        while (flash_check_busy());
        vPortEnterCritical();
        flash_write_page(
            (uint32_t)&storage_backup[i*RT582CONFIG_FLASH_PAGE_SIZE], 
            (sector_addr+(i*RT582CONFIG_FLASH_PAGE_SIZE)));
        vPortExitCritical();
    }
    while (flash_check_busy());

    ////ChipLogDetail(DeviceLayer, "%s sector 0x%08X - 0x%08X", __func__, sector_addr, (sector_addr+(0x10*RT582CONFIG_ID_PER_SIZE)));
    ////ChipLogDetail(DeviceLayer, "%s page 0x%08X", __func__, sector_addr+offset);
#endif
    return dataLen;
}

void storage_erase(uint32_t id)
{
#if RT582CONFIG_USE_FLASH_DS
    //ChipLogDetail(DeviceLayer, "storage_erase key id: 0x%02x", id);
    uint32_t flash_keyid = RT582Config::RT582KeyaddrPasser(id);

    ds_delete_type(flash_keyid);
    vTaskSuspendAll();
    flush_cache();
    xTaskResumeAll();

#else
    uint32_t sector_addr, id_addr, i, offset, ignore_page = 0;
    uint32_t flash_keyid = RT582Config::RT582KeyaddrPasser(id);
    ////ChipLogDetail(DeviceLayer, "storage_erase key id: 0x%02x, flash id: 0x%02x", id, flash_keyid);

    id_addr = RT582CONFIG_BASE_ADDR+(RT582CONFIG_ID_PER_SIZE*flash_keyid);
    sector_addr = id_addr - (id_addr % RT582CONFIG_SECTOR_SIZE);

    for(i=0;i<16;i++)
    {
        flash_read_page_syncmode((uint32_t)&storage_backup[i*RT582CONFIG_FLASH_PAGE_SIZE], (sector_addr+(i*RT582CONFIG_ID_PER_SIZE)));
    }

    while (flash_check_busy());
    vPortEnterCritical();
    flash_erase(FLASH_ERASE_SECTOR, sector_addr);
    vPortExitCritical();
    while (flash_check_busy());

    offset = (RT582CONFIG_ID_PER_SIZE*flash_keyid) % 0x1000;
    memset(&storage_backup[offset], 0xff, RT582CONFIG_ID_PER_SIZE);

    //ignore_page = ((RT582CONFIG_ID_PER_SIZE*flash_keyid) % RT582CONFIG_SECTOR_SIZE) / RT582CONFIG_ID_PER_SIZE;

    for(i=0;i<16;i++)
    {
        //if(i == ignore_page)
        //    continue;        
        while (flash_check_busy());
        vPortEnterCritical();
        flash_write_page(
            (uint32_t)&storage_backup[i*RT582CONFIG_FLASH_PAGE_SIZE], 
            (sector_addr+(i*RT582CONFIG_FLASH_PAGE_SIZE)));
        vPortExitCritical();
    }
    while (flash_check_busy());
    ////ChipLogDetail(DeviceLayer, "%s sector 0x%08X - 0x%08X", __func__, sector_addr, (sector_addr+(0x10*RT582CONFIG_ID_PER_SIZE)));
    ////ChipLogDetail(DeviceLayer, "%s page 0x%08X", __func__, sector_addr+offset);
#endif
}

CHIP_ERROR RT582Config::Init()
{

#if RT582CONFIG_USE_FLASH_DS
    ds_config_t ds_init_cfg;
    uint32_t ds_ret = 0;

    ds_init_cfg.start_address = RT582CONFIG_BASE_ADDR;
    ds_init_cfg.end_address = RT582CONFIG_END_ADDR;	
    ds_init_cfg.ds_page_mode = DS_ONE_PAGE_MODE;	
    ds_init_cfg.page_size = DATA_ONE_PAGE_SIZE;
    ds_init_cfg.page_size = 0x4000;
    ds_ret = ds_initinal(ds_init_cfg);

    if (ds_ret != STATUS_SUCCESS)
    {
        ChipLogError(DeviceLayer, "ds_initial fail %d", ds_ret);
        while(1);
    }

    CHIP_ERROR err;
    uint32_t testval;
    if (!RT582Config::ConfigValueExists(RT582Config::kConfigKey_RebootCount, testval))
    {
        err = chip::DeviceLayer::ConfigurationMgr().StoreRebootCount(0);
    }

#endif
    
    return ChipError(0);
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
    outLen = storage_read(key, bufSize, (uint8_t *)buf);

    if (outLen == 0)
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
    storage_write(key, dataLen, (uint8_t *)data);
    
    return CHIP_NO_ERROR;
}

CHIP_ERROR RT582Config::ClearConfigValue(Key key)
{
    storage_erase(key);

    return CHIP_NO_ERROR;
}

bool RT582Config::ConfigValueExists(Key key)
{
    uint8_t val;

    return ChipError::IsSuccess(ReadConfigValue(key, val));
}

template <typename T>
bool RT582Config::ConfigValueExists(Key key, T & val)
{
    return ChipError::IsSuccess(ReadConfigValue(key, val));
}

CHIP_ERROR RT582Config::FactoryResetConfig(void)
{
    ////ChipLogDetail(DeviceLayer, "RT582Config %s", __func__);
    // for (Key key = kMinConfigKey_MatterFactory; key < kMaxConfigKey_MatterFactory; key++)
    //     ClearConfigValue(key);

    vTaskSuspendAll();

    while (flash_check_busy());
    flash_erase(FLASH_ERASE_SECTOR, 0xF3000);
    while (flash_check_busy());
    flash_erase(FLASH_ERASE_SECTOR, 0xF4000);
    while (flash_check_busy());
    flash_erase(FLASH_ERASE_SECTOR, 0xF5000);
    while (flash_check_busy());
    flash_erase(FLASH_ERASE_SECTOR, 0xF6000);
    while (flash_check_busy());
    flash_erase(FLASH_ERASE_SECTOR, 0xF7000);
    while (flash_check_busy());
    flash_erase(FLASH_ERASE_SECTOR, 0xF8000);
    while (flash_check_busy());
    flash_erase(FLASH_ERASE_SECTOR, 0xF9000);
    while (flash_check_busy());
    flash_erase(FLASH_ERASE_SECTOR, 0xFA000);
    while (flash_check_busy());
    flash_erase(FLASH_ERASE_SECTOR, 0xFB000);
    while (flash_check_busy());

    ds_reset_to_default();

    xTaskResumeAll();
#if 0
    for (Key key = kMinConfigKey_MatterConfig; key < kMaxConfigKey_MatterConfig; key++)
        ClearConfigValue(key);

    for (Key key = kMinConfigKey_KVSKey; key < kMaxConfigKey_KVSKey; key++)
        ClearConfigValue(key);

    for (Key key = kMinConfigKey_KVSValue; key < kMaxConfigKey_KVSValue; key++)
        ClearConfigValue(key);                    
#endif
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


#else

/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>
#include <platform/RT582/RT582Config.h>
#include <lib/core/CHIPEncoding.h>

// #include "util_log.h"
#include "cm3_mcu.h"

#if (PLATFORM_CONFIG == RAFAEL_RT582_1M)
#define RT582CONFIG_BASE_ADDR       0xF3000
#define RT582CONFIG_END_ADDR        0xFB000
#elif (PLATFORM_CONFIG == RAFAEL_RT583_2M)
#define RT582CONFIG_BASE_ADDR       0x1E3000
#define RT582CONFIG_END_ADDR        0x1EB000
#endif
#define RT582CONFIG_ID_PER_SIZE     0x100
#define RT582CONFIG_FLASH_PAGE_SIZE 0x100
#define RT582CONFIG_SECTOR_SIZE     0x1000

#define RT582CONFIG_USE_FLASH_DS    1

namespace chip {
namespace DeviceLayer {
namespace Internal {

///static uint8_t storage_backup[RT582CONFIG_SECTOR_SIZE];
static size_t storage_read(uint32_t id, size_t bufSize, uint8_t *buf)
{
#if RT582CONFIG_USE_FLASH_DS
    ds_rw_t t_ds_r;
    uint32_t ds_ret = 0;
    uint32_t flash_keyid = RT582Config::RT582KeyaddrPasser(id);
    // ChipLogDetail(DeviceLayer, "storage_read key id: 0x%02x, flash id: 0x%02x", id, flash_keyid);
    t_ds_r.type = flash_keyid;
    t_ds_r.len = 0;
    t_ds_r.address = 0;

    ds_ret = ds_read(&t_ds_r);

    if (ds_ret != STATUS_SUCCESS)
    {
        // ChipLogError(DeviceLayer, "ds_read failed %d", ds_ret);
    }
    else
    {
        for(int i=0;i<t_ds_r.len;i++)
            buf[i] = flash_read_byte(t_ds_r.address + i);
    }
    return t_ds_r.len;
#else
    uint32_t sector_addr, id_addr, i, offset, chk, data_size;
    uint32_t flash_keyid = RT582Config::RT582KeyaddrPasser(id);
    // ChipLogDetail(DeviceLayer, "storage_read key id: 0x%02x, flash id: 0x%02x", id, flash_keyid);

    id_addr = RT582CONFIG_BASE_ADDR + (RT582CONFIG_ID_PER_SIZE * flash_keyid);
    sector_addr = id_addr - (id_addr % RT582CONFIG_SECTOR_SIZE);

    for(i=0;i<16;i++)
    {
        flash_read_page_syncmode((uint32_t)&storage_backup[i*RT582CONFIG_FLASH_PAGE_SIZE], (sector_addr+(i*RT582CONFIG_FLASH_PAGE_SIZE)));
    }
    offset = (RT582CONFIG_ID_PER_SIZE*flash_keyid) % 0x1000;

    if((bufSize < storage_backup[offset]) && (storage_backup[offset] != 0xFF))
    {
        //ChipLogDetail(DeviceLayer, "Cfg  %s !!!!!!!!!!", __func__); 
        return 0xFFFFFFFF;
    }

    if(storage_backup[offset] == 0xFF)
    {
        //ChipLogDetail(DeviceLayer, "Cfg  %s id 0x%02x not found", __func__, id); 
        return 0xFFFFFFFE;
    }

    memcpy(buf, &storage_backup[offset+1], storage_backup[offset]);

    //if(chk == (bufSize-1))
    //    return 0;
    
    return storage_backup[offset];
#endif
}

size_t storage_write(uint32_t id, size_t dataLen, uint8_t *data)
{
#if RT582CONFIG_USE_FLASH_DS
    uint32_t ds_ret = 0;
    ds_rw_t t_ds_w;
    uint32_t flash_keyid = RT582Config::RT582KeyaddrPasser(id);
    // ChipLogDetail(DeviceLayer, "storage_write key id: 0x%02x, flash id: 0x%02x size %d", id, flash_keyid, dataLen);
    t_ds_w.type = flash_keyid;
    t_ds_w.len = dataLen;
    t_ds_w.address = (uint32_t)data;

    if (dataLen > 0)
    {
        ds_ret = ds_write(&t_ds_w);
        if (ds_ret != STATUS_SUCCESS)
        {
            ChipLogError(DeviceLayer, "ds_write failed %d", ds_ret);
            while(1);
        }
        else
        {
            vTaskSuspendAll();
            flush_cache();
            xTaskResumeAll();
        }
    }
#else
    uint32_t sector_addr, id_addr, i, offset;
    uint32_t flash_keyid = RT582Config::RT582KeyaddrPasser(id);
    //ChipLogDetail(DeviceLayer, "storage_write key id: 0x%02x, flash id: 0x%02x", id, flash_keyid);

    id_addr = RT582CONFIG_BASE_ADDR+(RT582CONFIG_ID_PER_SIZE*flash_keyid);
    sector_addr = id_addr - (id_addr % RT582CONFIG_SECTOR_SIZE);


    for(i=0;i<16;i++)
    {
        flash_read_page_syncmode((uint32_t)&storage_backup[i*RT582CONFIG_FLASH_PAGE_SIZE], (sector_addr+(i*RT582CONFIG_FLASH_PAGE_SIZE)));
    }
    while (flash_check_busy());
    vPortEnterCritical();
    flash_erase(FLASH_ERASE_SECTOR, sector_addr);
    vPortExitCritical();
    while (flash_check_busy());

    offset = (RT582CONFIG_ID_PER_SIZE*flash_keyid) % 0x1000;

    storage_backup[offset] = dataLen;
    memcpy(&storage_backup[offset+1], data, dataLen);

    for(i=0;i<16;i++)
    {
        while (flash_check_busy());
        vPortEnterCritical();
        flash_write_page(
            (uint32_t)&storage_backup[i*RT582CONFIG_FLASH_PAGE_SIZE], 
            (sector_addr+(i*RT582CONFIG_FLASH_PAGE_SIZE)));
        vPortExitCritical();
    }
    while (flash_check_busy());

    ////ChipLogDetail(DeviceLayer, "%s sector 0x%08X - 0x%08X", __func__, sector_addr, (sector_addr+(0x10*RT582CONFIG_ID_PER_SIZE)));
    ////ChipLogDetail(DeviceLayer, "%s page 0x%08X", __func__, sector_addr+offset);
#endif
    return dataLen;
}

void storage_erase(uint32_t id)
{

#if RT582CONFIG_USE_FLASH_DS
    // ChipLogDetail(DeviceLayer, "storage_erase key id: 0x%02x", id);
    uint32_t flash_keyid = RT582Config::RT582KeyaddrPasser(id);

    ds_delete_type(flash_keyid);
    vTaskSuspendAll();
    flush_cache();
    xTaskResumeAll();

#else
    uint32_t sector_addr, id_addr, i, offset, ignore_page = 0;
    uint32_t flash_keyid = RT582Config::RT582KeyaddrPasser(id);
    ////ChipLogDetail(DeviceLayer, "storage_erase key id: 0x%02x, flash id: 0x%02x", id, flash_keyid);

    id_addr = RT582CONFIG_BASE_ADDR+(RT582CONFIG_ID_PER_SIZE*flash_keyid);
    sector_addr = id_addr - (id_addr % RT582CONFIG_SECTOR_SIZE);

    for(i=0;i<16;i++)
    {
        flash_read_page_syncmode((uint32_t)&storage_backup[i*RT582CONFIG_FLASH_PAGE_SIZE], (sector_addr+(i*RT582CONFIG_ID_PER_SIZE)));
    }

    while (flash_check_busy());
    vPortEnterCritical();
    flash_erase(FLASH_ERASE_SECTOR, sector_addr);
    vPortExitCritical();
    while (flash_check_busy());

    offset = (RT582CONFIG_ID_PER_SIZE*flash_keyid) % 0x1000;
    memset(&storage_backup[offset], 0xff, RT582CONFIG_ID_PER_SIZE);

    //ignore_page = ((RT582CONFIG_ID_PER_SIZE*flash_keyid) % RT582CONFIG_SECTOR_SIZE) / RT582CONFIG_ID_PER_SIZE;

    for(i=0;i<16;i++)
    {
        //if(i == ignore_page)
        //    continue;        
        while (flash_check_busy());
        vPortEnterCritical();
        flash_write_page(
            (uint32_t)&storage_backup[i*RT582CONFIG_FLASH_PAGE_SIZE], 
            (sector_addr+(i*RT582CONFIG_FLASH_PAGE_SIZE)));
        vPortExitCritical();
    }
    while (flash_check_busy());
    ////ChipLogDetail(DeviceLayer, "%s sector 0x%08X - 0x%08X", __func__, sector_addr, (sector_addr+(0x10*RT582CONFIG_ID_PER_SIZE)));
    ////ChipLogDetail(DeviceLayer, "%s page 0x%08X", __func__, sector_addr+offset);
#endif
}

CHIP_ERROR RT582Config::Init()
{

#if RT582CONFIG_USE_FLASH_DS
    ds_config_t ds_init_cfg;
    uint32_t ds_ret = 0;

    ds_init_cfg.start_address = RT582CONFIG_BASE_ADDR;
    ds_init_cfg.end_address = RT582CONFIG_END_ADDR;	
    ds_init_cfg.ds_page_mode = DS_ONE_PAGE_MODE;	
    ds_init_cfg.page_size = DATA_ONE_PAGE_SIZE;
    // info("===> start: %x, end: %x\r\n", RT582CONFIG_BASE_ADDR, RT582CONFIG_END_ADDR);
    ds_ret = ds_initinal(ds_init_cfg);

    if (ds_ret != STATUS_SUCCESS)
    {
        ChipLogError(DeviceLayer, "ds_initial fail %d", ds_ret);
        while(1);
    }
#endif
    
    return ChipError(0);
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
    outLen = storage_read(key, bufSize, (uint8_t *)buf);

    if (outLen == 0)
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
    storage_write(key, dataLen, (uint8_t *)data);
    
    return CHIP_NO_ERROR;
}

CHIP_ERROR RT582Config::ClearConfigValue(Key key)
{
    ////ChipLogDetail(DeviceLayer, "RT582Config %s, %u", __func__, key);

    storage_erase(key);

    return CHIP_NO_ERROR;
}

bool RT582Config::ConfigValueExists(Key key)
{
    uint8_t val;

    return ChipError::IsSuccess(ReadConfigValue(key, val));
}


CHIP_ERROR RT582Config::FactoryResetConfig(void)
{
    ////ChipLogDetail(DeviceLayer, "RT582Config %s", __func__);
    // for (Key key = kMinConfigKey_MatterFactory; key < kMaxConfigKey_MatterFactory; key++)
    //     ClearConfigValue(key);

    vTaskSuspendAll();

    // while (flash_check_busy());
    // flash_erase(FLASH_ERASE_SECTOR, 0xF3000);
    // while (flash_check_busy());
    // flash_erase(FLASH_ERASE_SECTOR, 0xF4000);
    // while (flash_check_busy());
    // flash_erase(FLASH_ERASE_SECTOR, 0xF5000);
    // while (flash_check_busy());
    // flash_erase(FLASH_ERASE_SECTOR, 0xF6000);
    // while (flash_check_busy());
    // flash_erase(FLASH_ERASE_SECTOR, 0xF7000);
    // while (flash_check_busy());
    // flash_erase(FLASH_ERASE_SECTOR, 0xF8000);
    // while (flash_check_busy());
    // flash_erase(FLASH_ERASE_SECTOR, 0xF9000);
    // while (flash_check_busy());
    // flash_erase(FLASH_ERASE_SECTOR, 0xFA000);
    // while (flash_check_busy());
    // flash_erase(FLASH_ERASE_SECTOR, 0xFB000);
    // while (flash_check_busy());

    for (int address = RT582CONFIG_BASE_ADDR; address <= RT582CONFIG_END_ADDR; address += RT582CONFIG_SECTOR_SIZE ) {
        flash_erase(FLASH_ERASE_SECTOR, address);
        while (flash_check_busy()) {}
    }

    ds_reset_to_default();

    xTaskResumeAll();
#if 0
    for (Key key = kMinConfigKey_MatterConfig; key < kMaxConfigKey_MatterConfig; key++)
        ClearConfigValue(key);

    for (Key key = kMinConfigKey_KVSKey; key < kMaxConfigKey_KVSKey; key++)
        ClearConfigValue(key);

    for (Key key = kMinConfigKey_KVSValue; key < kMaxConfigKey_KVSValue; key++)
        ClearConfigValue(key);                    
#endif
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


#endif