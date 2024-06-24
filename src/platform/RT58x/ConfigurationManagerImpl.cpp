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

/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <platform/internal/GenericConfigurationManagerImpl.ipp>

#include <platform/ConfigurationManager.h>
#include <platform/RT58x/RT58xConfig.h>

#include "cm3_mcu.h"

namespace chip {
namespace DeviceLayer {

using namespace ::chip::DeviceLayer::Internal;

ConfigurationManagerImpl & ConfigurationManagerImpl::GetDefaultInstance()
{
    static ConfigurationManagerImpl sInstance;
    return sInstance;
}

CHIP_ERROR ConfigurationManagerImpl::Init()
{
    CHIP_ERROR err;
    uint32_t rebootCount;

    if (RT58xConfig::ConfigValueExists(RT58xConfig::kConfigKey_RebootCount))
    {
        err = GetRebootCount(rebootCount);
        SuccessOrExit(err);

        err = StoreRebootCount(rebootCount + 1);
        SuccessOrExit(err);
    }
    else
    {
        // The first boot after factory reset of the Node.
        err = StoreRebootCount(1);
        SuccessOrExit(err);
    }
    // Initialize the generic implementation base class.
    err = Internal::GenericConfigurationManagerImpl<RT58xConfig>::Init();
    SuccessOrExit(err);

    err = CHIP_NO_ERROR;

exit:
    return err;
}

CHIP_ERROR ConfigurationManagerImpl::GetSoftwareVersion(uint32_t & softwareVer)
{
    CHIP_ERROR err;

    err = RT58xConfig::ReadConfigValue(RT58xConfig::kConfigKey_SoftwareVersion, softwareVer);
    if (err != CHIP_NO_ERROR)
    {
        softwareVer = CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION;
        StoreSoftwareVersion(softwareVer);
    }
    ChipLogProgress(SoftwareUpdate, "Software Version = %d", softwareVer);

    return CHIP_NO_ERROR;
}

CHIP_ERROR ConfigurationManagerImpl::StoreSoftwareVersion(uint32_t softwareVer)
{
    RT58xConfig::WriteConfigValue(RT58xConfig::kConfigKey_SoftwareVersion, softwareVer);
    // ChipLogProgress(SoftwareUpdate, "Software Version = %d", softwareVer);
    return CHIP_NO_ERROR;
}

bool ConfigurationManagerImpl::CanFactoryReset()
{
    // TODO: query the application to determine if factory reset is allowed.
    return true;
}

void ConfigurationManagerImpl::InitiateFactoryReset()
{
    PlatformMgr().ScheduleWork(DoFactoryReset);
}

CHIP_ERROR ConfigurationManagerImpl::ReadPersistedStorageValue(::chip::Platform::PersistedStorage::Key persistedStorageKey,
                                                               uint32_t & value)
{
    char keyname[sizeof(KCONFIG_SECT_PSV) + 10];

    sprintf(keyname, "%s_%x", KCONFIG_SECT_PSV, persistedStorageKey);

    CHIP_ERROR err = ReadConfigValue(keyname, value);
    if (err == CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND)
    {
        err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }
    return err;
}

CHIP_ERROR ConfigurationManagerImpl::WritePersistedStorageValue(::chip::Platform::PersistedStorage::Key persistedStorageKey,
                                                                uint32_t value)
{
    char keyname[sizeof(KCONFIG_SECT_PSV) + 10];

    sprintf(keyname, "%s_%d", KCONFIG_SECT_PSV, persistedStorageKey);

    return WriteConfigValue(keyname, value);
}

CHIP_ERROR ConfigurationManagerImpl::GetRebootCount(uint32_t & rebootCount)
{
    return ReadConfigValue(RT58xConfig::kConfigKey_RebootCount, rebootCount);
}

CHIP_ERROR ConfigurationManagerImpl::StoreRebootCount(uint32_t rebootCount)
{
    return WriteConfigValue(RT58xConfig::kConfigKey_RebootCount, rebootCount);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, bool & val)
{
    return RT58xConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, uint32_t & val)
{
    return RT58xConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, uint64_t & val)
{
    return RT58xConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen)
{
    return RT58xConfig::ReadConfigValueStr(key, buf, bufSize, outLen);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen)
{
    return RT58xConfig::ReadConfigValueBin(key, buf, bufSize, outLen);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, bool val)
{
    return RT58xConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, uint32_t val)
{
    return RT58xConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, uint64_t val)
{
    return RT58xConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueStr(Key key, const char * str)
{
    return RT58xConfig::WriteConfigValueStr(key, str);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueStr(Key key, const char * str, size_t strLen)
{
    return RT58xConfig::WriteConfigValueStr(key, str, strLen);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen)
{
    return RT58xConfig::WriteConfigValueBin(key, data, dataLen);
}

void ConfigurationManagerImpl::RunConfigUnitTest(void)
{
    RT58xConfig::RunConfigUnitTest();
}

void ConfigurationManagerImpl::DoFactoryReset(intptr_t arg)
{
    CHIP_ERROR err;
    uint32_t reboot_cnt = 0;

    ChipLogProgress(DeviceLayer, "Performing factory reset");

    err = RT58xConfig::FactoryResetConfig();
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "FactoryResetConfig() failed: %s", chip::ErrorStr(err));
    }

    ConfigurationMgr().StoreRebootCount(reboot_cnt);

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD

    ChipLogProgress(DeviceLayer, "Clearing Thread provision");
    ThreadStackMgr().ErasePersistentInfo();

#endif // CHIP_DEVICE_CONFIG_ENABLE_THREAD

    // Restart the system.
    ChipLogProgress(DeviceLayer, "System restarting");
    vTaskDelay(300);
    Sys_Software_Reset();
}

ConfigurationManager & ConfigurationMgrImpl()
{
    return ConfigurationManagerImpl::GetDefaultInstance();
}

} // namespace DeviceLayer
} // namespace chip
