/*
 *
 *    Copyright (c) 2020-2022 Project CHIP Authors
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

#if 0


#pragma once

#include <functional>

#include <platform/internal/CHIPDeviceLayerInternal.h>

namespace chip {
namespace DeviceLayer {
namespace Internal {

constexpr inline uint16_t RT582ConfigKey(uint8_t keyBaseOffset, uint8_t id)
{
    return static_cast<uint16_t>(keyBaseOffset) << 6 | (id & 0x3F);
}
class RT582Config
{
public:
    using Key = uint16_t;

    static constexpr uint8_t kMatterFactory_KeyBase = 0x3;
    static constexpr uint8_t kMatterConfig_KeyBase  = 0x2;
    static constexpr uint8_t kMatterKvs_KeyBase     = 0x1;
    static constexpr uint8_t kMatterKvs_KeyValue    = 0x0;

    // Key definitions for well-known configuration values.
    // Factory config keys
    static constexpr Key kConfigKey_SerialNum             = RT582ConfigKey(kMatterFactory_KeyBase, 0x00);
    static constexpr Key kConfigKey_MfrDeviceId           = RT582ConfigKey(kMatterFactory_KeyBase, 0x01);
    static constexpr Key kConfigKey_MfrDeviceCert         = RT582ConfigKey(kMatterFactory_KeyBase, 0x02);
    static constexpr Key kConfigKey_MfrDevicePrivateKey   = RT582ConfigKey(kMatterFactory_KeyBase, 0x03);
    static constexpr Key kConfigKey_ManufacturingDate     = RT582ConfigKey(kMatterFactory_KeyBase, 0x04);
    static constexpr Key kConfigKey_SetupPinCode          = RT582ConfigKey(kMatterFactory_KeyBase, 0x05);
    static constexpr Key kConfigKey_MfrDeviceICACerts     = RT582ConfigKey(kMatterFactory_KeyBase, 0x06);
    static constexpr Key kConfigKey_SetupDiscriminator    = RT582ConfigKey(kMatterFactory_KeyBase, 0x07);
    static constexpr Key kConfigKey_Spake2pIterationCount = RT582ConfigKey(kMatterFactory_KeyBase, 0x08);
    static constexpr Key kConfigKey_Spake2pSalt           = RT582ConfigKey(kMatterFactory_KeyBase, 0x09);
    static constexpr Key kConfigKey_Spake2pVerifier       = RT582ConfigKey(kMatterFactory_KeyBase, 0x0A);
    static constexpr Key kConfigKey_SoftwareVersion       = RT582ConfigKey(kMatterFactory_KeyBase, 0x0B);

    // Matter Config Keys
    // static constexpr Key kConfigKey_ServiceConfig      = RT582ConfigKey(kMatterConfig_KeyBase, 0x00);
    // static constexpr Key kConfigKey_PairedAccountId    = RT582ConfigKey(kMatterConfig_KeyBase, 0x01);
    // static constexpr Key kConfigKey_ServiceId          = RT582ConfigKey(kMatterConfig_KeyBase, 0x02);
    static constexpr Key kConfigKey_LastUsedEpochKeyId = RT582ConfigKey(kMatterConfig_KeyBase, 0x00);
    static constexpr Key kConfigKey_FailSafeArmed      = RT582ConfigKey(kMatterConfig_KeyBase, 0x01);
    static constexpr Key kConfigKey_GroupKey           = RT582ConfigKey(kMatterConfig_KeyBase, 0x02);
    static constexpr Key kConfigKey_HardwareVersion    = RT582ConfigKey(kMatterConfig_KeyBase, 0x03);
    static constexpr Key kConfigKey_RegulatoryLocation = RT582ConfigKey(kMatterConfig_KeyBase, 0x04);
    static constexpr Key kConfigKey_CountryCode        = RT582ConfigKey(kMatterConfig_KeyBase, 0x05);
    static constexpr Key kConfigKey_RebootCount        = RT582ConfigKey(kMatterConfig_KeyBase, 0x06);
    static constexpr Key kConfigKey_UniqueId           = RT582ConfigKey(kMatterConfig_KeyBase, 0x07);
    // static constexpr Key kConfigKey_LockUser           = RT582ConfigKey(kMatterConfig_KeyBase, 0x0B);
    // static constexpr Key kConfigKey_Credential         = RT582ConfigKey(kMatterConfig_KeyBase, 0x08);
    // static constexpr Key kConfigKey_LockUserName       = RT582ConfigKey(kMatterConfig_KeyBase, 0x0D);
    // static constexpr Key kConfigKey_CredentialData     = RT582ConfigKey(kMatterConfig_KeyBase, 0x09);
    // static constexpr Key kConfigKey_UserCredentials    = RT582ConfigKey(kMatterConfig_KeyBase, 0x0A);
    // static constexpr Key kConfigKey_WeekDaySchedules   = RT582ConfigKey(kMatterConfig_KeyBase, 0x10);
    // static constexpr Key kConfigKey_YearDaySchedules   = RT582ConfigKey(kMatterConfig_KeyBase, 0x11);
    // static constexpr Key kConfigKey_HolidaySchedules   = RT582ConfigKey(kMatterConfig_KeyBase, 0x12);
    // static constexpr Key kConfigKey_BootReason         = RT582ConfigKey(kMatterConfig_KeyBase, 0x13);

    // static constexpr Key kConfigKey_GroupKeyBase       = RT582ConfigKey(kMatterConfig_KeyBase, 0x1F);
    // // Allows 16 Group Keys to be created.    
    // static constexpr Key kConfigKey_GroupKeyMax        = RT582ConfigKey(kMatterKvs_KeyValue, 0x2F); 

    static constexpr Key kMinConfigKey_MatterFactory = RT582ConfigKey(kMatterFactory_KeyBase, 0x00);
    static constexpr Key kMaxConfigKey_MatterFactory = RT582ConfigKey(kMatterFactory_KeyBase, 0x0B);
    static constexpr Key kMinConfigKey_MatterConfig  = RT582ConfigKey(kMatterConfig_KeyBase, 0x00);
    static constexpr Key kMaxConfigKey_MatterConfig  = RT582ConfigKey(kMatterConfig_KeyBase, 0x07);
    // static constexpr Key kMaxConfigKey_MatterConfig  = RT582ConfigKey(kMatterConfig_KeyBase, 0x0F);

    static constexpr Key kMinConfigKey_KVSKey      = RT582ConfigKey(kMatterKvs_KeyBase, 0x00);
    static constexpr Key kMaxConfigKey_KVSKey      = RT582ConfigKey(kMatterKvs_KeyBase, 0x3f);
    static constexpr Key kMinConfigKey_KVSValue    = RT582ConfigKey(kMatterKvs_KeyValue, 0x00);
    static constexpr Key kMaxConfigKey_KVSValue    = RT582ConfigKey(kMatterKvs_KeyValue, 0x3f);    


    static CHIP_ERROR Init(void);
    // Configuration methods used by the GenericConfigurationManagerImpl<> template.
    template <typename T>
    static CHIP_ERROR ReadConfigValue(Key key, T & val);
    
    static CHIP_ERROR ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen);
    static CHIP_ERROR ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen);
    static CHIP_ERROR ReadConfigValueBin(Key key, void * buf, size_t bufSize, size_t & outLen);

    template <typename T>
    static CHIP_ERROR WriteConfigValue(Key key, T val);
    static CHIP_ERROR WriteConfigValueStr(Key key, const char * str);
    static CHIP_ERROR WriteConfigValueStr(Key key, const char * str, size_t strLen);
    static CHIP_ERROR WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen);
    static CHIP_ERROR WriteConfigValueBin(Key key, const void * data, size_t dataLen);
    static CHIP_ERROR ClearConfigValue(Key key);
    static bool ConfigValueExists(Key key);
    static CHIP_ERROR FactoryResetConfig(void);
    static void RunConfigUnitTest(void);

    static uint32_t RT582KeyaddrPasser(uint32_t id)
    {
        uint32_t index = id;

        do
        {
            if (id >= kMinConfigKey_MatterFactory) {
                index = (kMaxConfigKey_MatterFactory - kMinConfigKey_MatterFactory) +
                        (kMaxConfigKey_MatterConfig - kMinConfigKey_MatterConfig) +
                        (kMaxConfigKey_KVSKey - kMinConfigKey_KVSKey) +
                        (id - kMinConfigKey_MatterFactory);
            }
            else if (id >= kMinConfigKey_MatterConfig) {
                index = (kMaxConfigKey_MatterConfig - kMinConfigKey_MatterConfig) +
                        (kMaxConfigKey_KVSKey - kMinConfigKey_KVSKey) +
                        (id - kMinConfigKey_MatterConfig);
            }
            else if (id >= kMinConfigKey_KVSKey) {
                index = (kMaxConfigKey_KVSKey - kMinConfigKey_KVSKey) +
                        (id - kMinConfigKey_KVSKey);
            }
            else if (id >= kMinConfigKey_KVSValue) {
                index = id - kMinConfigKey_KVSValue;
            }
            
            // if (id >= kMinConfigKey_KVSValue)
            // {
            //     index = ((kMaxConfigKey_KVSKey - kMinConfigKey_KVSKey) + 
            //             (kMaxConfigKey_MatterConfig - kMinConfigKey_MatterConfig) + 
            //             (kMaxConfigKey_MatterFactory - kMinConfigKey_MatterFactory) +
            //             (id - kMinConfigKey_KVSValue));
            //     break;
            // }

            // if (id >= kMinConfigKey_KVSKey)
            // {
            //     index = ((kMaxConfigKey_MatterConfig - kMinConfigKey_MatterConfig) + 
            //             (kMaxConfigKey_MatterFactory - kMinConfigKey_MatterFactory) +
            //             (id - kMinConfigKey_KVSKey));
            //     break;
            // }

            // if (id >= kMinConfigKey_MatterConfig)
            // {
            //     index = ((kMaxConfigKey_MatterFactory - kMinConfigKey_MatterFactory) +
            //             (id - kMinConfigKey_MatterConfig));
            //     break;
            // }

            // if (id >= kMinConfigKey_MatterFactory)
            // {
            //     index = (id - kMinConfigKey_MatterFactory);
            //     break;
            // }              

        } while (0);
        
        return index;
    }

protected:

private:    
};
} // namespace Internal
} // namespace DeviceLayer
} // namespace chip


#else 


#pragma once

#include <functional>

#include <platform/internal/CHIPDeviceLayerInternal.h>

namespace chip {
namespace DeviceLayer {
namespace Internal {

constexpr inline uint16_t RT582ConfigKey(uint8_t keyBaseOffset, uint8_t id)
{
    // return static_cast<uint16_t>(keyBaseOffset) << 6 | (id & 0x3F);
    return static_cast<uint16_t>(keyBaseOffset) + id;
}

class RT582Config
{
public:
    using Key = uint16_t;

    // static constexpr uint8_t kMatterFactory_KeyBase = 0x0;
    // static constexpr uint8_t kMatterConfig_KeyBase  = 0x1;
    static constexpr uint8_t kMatterKvs_KeyBase     = 0x2C;
    static constexpr uint8_t kMatterKvs_KeyValue    = 0x96;

    // Key definitions for well-known configuration values.
    // Factory config keys
    // static constexpr Key kConfigKey_SerialNum             = RT582ConfigKey(kMatterFactory_KeyBase, 0x00);
    // static constexpr Key kConfigKey_MfrDeviceId           = RT582ConfigKey(kMatterFactory_KeyBase, 0x01);
    // static constexpr Key kConfigKey_MfrDeviceCert         = RT582ConfigKey(kMatterFactory_KeyBase, 0x02);
    // static constexpr Key kConfigKey_MfrDevicePrivateKey   = RT582ConfigKey(kMatterFactory_KeyBase, 0x03);
    // static constexpr Key kConfigKey_ManufacturingDate     = RT582ConfigKey(kMatterFactory_KeyBase, 0x04);
    // static constexpr Key kConfigKey_SetupPinCode          = RT582ConfigKey(kMatterFactory_KeyBase, 0x05);
    // static constexpr Key kConfigKey_MfrDeviceICACerts     = RT582ConfigKey(kMatterFactory_KeyBase, 0x06);
    // static constexpr Key kConfigKey_SetupDiscriminator    = RT582ConfigKey(kMatterFactory_KeyBase, 0x07);
    // static constexpr Key kConfigKey_Spake2pIterationCount = RT582ConfigKey(kMatterFactory_KeyBase, 0x08);
    // static constexpr Key kConfigKey_Spake2pSalt           = RT582ConfigKey(kMatterFactory_KeyBase, 0x09);
    // static constexpr Key kConfigKey_Spake2pVerifier       = RT582ConfigKey(kMatterFactory_KeyBase, 0x0A);
    // static constexpr Key kConfigKey_SoftwareVersion       = RT582ConfigKey(kMatterFactory_KeyBase, 0x0B);

    static constexpr Key kConfigKey_SerialNum             = 0x00;
    static constexpr Key kConfigKey_MfrDeviceId           = 0x01;
    static constexpr Key kConfigKey_MfrDeviceCert         = 0x02;
    static constexpr Key kConfigKey_MfrDevicePrivateKey   = 0x03;
    static constexpr Key kConfigKey_ManufacturingDate     = 0x04;
    static constexpr Key kConfigKey_SetupPinCode          = 0x05;
    static constexpr Key kConfigKey_MfrDeviceICACerts     = 0x06;
    static constexpr Key kConfigKey_SetupDiscriminator    = 0x07;
    static constexpr Key kConfigKey_Spake2pIterationCount = 0x08;
    static constexpr Key kConfigKey_Spake2pSalt           = 0x09;
    static constexpr Key kConfigKey_Spake2pVerifier       = 0x0A;
    static constexpr Key kConfigKey_SoftwareVersion       = 0x0B;

    // Matter Config Keys
    // static constexpr Key kConfigKey_ServiceConfig      = RT582ConfigKey(kMatterConfig_KeyBase, 0x00);
    // static constexpr Key kConfigKey_PairedAccountId    = RT582ConfigKey(kMatterConfig_KeyBase, 0x01);
    // static constexpr Key kConfigKey_ServiceId          = RT582ConfigKey(kMatterConfig_KeyBase, 0x02);

    // static constexpr Key kConfigKey_LastUsedEpochKeyId = RT582ConfigKey(kMatterConfig_KeyBase, 0x00);
    // static constexpr Key kConfigKey_FailSafeArmed      = RT582ConfigKey(kMatterConfig_KeyBase, 0x01);
    // static constexpr Key kConfigKey_GroupKey           = RT582ConfigKey(kMatterConfig_KeyBase, 0x02);
    // static constexpr Key kConfigKey_HardwareVersion    = RT582ConfigKey(kMatterConfig_KeyBase, 0x03);
    // static constexpr Key kConfigKey_RegulatoryLocation = RT582ConfigKey(kMatterConfig_KeyBase, 0x04);
    // static constexpr Key kConfigKey_CountryCode        = RT582ConfigKey(kMatterConfig_KeyBase, 0x05);
    // static constexpr Key kConfigKey_RebootCount        = RT582ConfigKey(kMatterConfig_KeyBase, 0x06);
    // static constexpr Key kConfigKey_UniqueId           = RT582ConfigKey(kMatterConfig_KeyBase, 0x07);

    static constexpr Key kConfigKey_LastUsedEpochKeyId = 0x0C;
    static constexpr Key kConfigKey_FailSafeArmed      = 0x0D;
    static constexpr Key kConfigKey_GroupKey           = 0x0E;
    static constexpr Key kConfigKey_HardwareVersion    = 0x0F;
    static constexpr Key kConfigKey_RegulatoryLocation = 0x10;
    static constexpr Key kConfigKey_CountryCode        = 0x11;
    static constexpr Key kConfigKey_RebootCount        = 0x12;
    static constexpr Key kConfigKey_UniqueId           = 0x13;

    // static constexpr Key kConfigKey_LockUser           = RT582ConfigKey(kMatterConfig_KeyBase, 0x0B);
    // static constexpr Key kConfigKey_Credential         = RT582ConfigKey(kMatterConfig_KeyBase, 0x08);
    // static constexpr Key kConfigKey_LockUserName       = RT582ConfigKey(kMatterConfig_KeyBase, 0x0D);
    // static constexpr Key kConfigKey_CredentialData     = RT582ConfigKey(kMatterConfig_KeyBase, 0x09);
    // static constexpr Key kConfigKey_UserCredentials    = RT582ConfigKey(kMatterConfig_KeyBase, 0x0A);
    // static constexpr Key kConfigKey_WeekDaySchedules   = RT582ConfigKey(kMatterConfig_KeyBase, 0x10);
    // static constexpr Key kConfigKey_YearDaySchedules   = RT582ConfigKey(kMatterConfig_KeyBase, 0x11);
    // static constexpr Key kConfigKey_HolidaySchedules   = RT582ConfigKey(kMatterConfig_KeyBase, 0x12);
    // static constexpr Key kConfigKey_BootReason         = RT582ConfigKey(kMatterConfig_KeyBase, 0x13);

    // static constexpr Key kConfigKey_GroupKeyBase       = RT582ConfigKey(kMatterConfig_KeyBase, 0x1F);
    // // Allows 16 Group Keys to be created.    
    // static constexpr Key kConfigKey_GroupKeyMax        = RT582ConfigKey(kMatterKvs_KeyValue, 0x2F); 

    // static constexpr Key kMinConfigKey_MatterFactory = RT582ConfigKey(kMatterFactory_KeyBase, 0x00);
    // static constexpr Key kMaxConfigKey_MatterFactory = RT582ConfigKey(kMatterFactory_KeyBase, 0x0B);
    // static constexpr Key kMinConfigKey_MatterConfig  = RT582ConfigKey(kMatterConfig_KeyBase, 0x00);
    // static constexpr Key kMaxConfigKey_MatterConfig  = RT582ConfigKey(kMatterConfig_KeyBase, 0x07);

    static constexpr Key kMinConfigKey_MatterFactory = 0x14;
    static constexpr Key kMaxConfigKey_MatterFactory = 0x1f;
    static constexpr Key kMinConfigKey_MatterConfig  = 0x20;
    static constexpr Key kMaxConfigKey_MatterConfig  = 0x2B;

    // static constexpr Key kMaxConfigKey_MatterConfig  = RT582ConfigKey(kMatterConfig_KeyBase, 0x0F);

    // static constexpr Key kMinConfigKey_KVSKey      = RT582ConfigKey(kMatterKvs_KeyBase, 0x00);
    // static constexpr Key kMaxConfigKey_KVSKey      = RT582ConfigKey(kMatterKvs_KeyBase, 0x3e);
    // static constexpr Key kMinConfigKey_KVSValue    = RT582ConfigKey(kMatterKvs_KeyValue, 0x00);
    // static constexpr Key kMaxConfigKey_KVSValue    = RT582ConfigKey(kMatterKvs_KeyValue, 0x3e);  

    static constexpr Key kMinConfigKey_KVSKey      = 0x2C;
    static constexpr Key kMaxConfigKey_KVSKey      = 0x95;
    static constexpr Key kMinConfigKey_KVSValue    = 0x96;
    static constexpr Key kMaxConfigKey_KVSValue    = 0xFE;   


    static CHIP_ERROR Init(void);
    // Configuration methods used by the GenericConfigurationManagerImpl<> template.
    template <typename T>
    static CHIP_ERROR ReadConfigValue(Key key, T & val);
    
    static CHIP_ERROR ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen);
    static CHIP_ERROR ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen);
    static CHIP_ERROR ReadConfigValueBin(Key key, void * buf, size_t bufSize, size_t & outLen);

    template <typename T>
    static CHIP_ERROR WriteConfigValue(Key key, T val);
    static CHIP_ERROR WriteConfigValueStr(Key key, const char * str);
    static CHIP_ERROR WriteConfigValueStr(Key key, const char * str, size_t strLen);
    static CHIP_ERROR WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen);
    static CHIP_ERROR WriteConfigValueBin(Key key, const void * data, size_t dataLen);
    static CHIP_ERROR ClearConfigValue(Key key);
    static bool ConfigValueExists(Key key);
    template <typename T>
    static bool ConfigValueExists(Key key, T & val);
    static CHIP_ERROR FactoryResetConfig(void);
    static void RunConfigUnitTest(void);

    static uint32_t RT582KeyaddrPasser(uint32_t id)
    {
        // uint32_t index = id;

        // do
        // {
            // if (id >= kMinConfigKey_MatterFactory) {
            //     index = (kMaxConfigKey_MatterFactory - kMinConfigKey_MatterFactory) +
            //             (kMaxConfigKey_MatterConfig - kMinConfigKey_MatterConfig) +
            //             (kMaxConfigKey_KVSKey - kMinConfigKey_KVSKey) +
            //             (id - kMinConfigKey_MatterFactory);
            // }
            // else if (id >= kMinConfigKey_MatterConfig) {
            //     index = (kMaxConfigKey_MatterConfig - kMinConfigKey_MatterConfig) +
            //             (kMaxConfigKey_KVSKey - kMinConfigKey_KVSKey) +
            //             (id - kMinConfigKey_MatterConfig);
            // }
            // else if (id >= kMinConfigKey_KVSKey) {
            //     index = (kMaxConfigKey_KVSKey - kMinConfigKey_KVSKey) +
            //             (id - kMinConfigKey_KVSKey);
            // }
            // else if (id >= kMinConfigKey_KVSValue) {
            //     index = id - kMinConfigKey_KVSValue;
            // }
            
            // if (id >= kMinConfigKey_KVSValue)
            // {
            //     index = ((kMaxConfigKey_KVSKey - kMinConfigKey_KVSKey) + 
            //             (kMaxConfigKey_MatterConfig - kMinConfigKey_MatterConfig) + 
            //             (kMaxConfigKey_MatterFactory - kMinConfigKey_MatterFactory) +
            //             (id - kMinConfigKey_KVSValue));
            //     break;
            // }

            // if (id >= kMinConfigKey_KVSKey)
            // {
            //     index = ((kMaxConfigKey_MatterConfig - kMinConfigKey_MatterConfig) + 
            //             (kMaxConfigKey_MatterFactory - kMinConfigKey_MatterFactory) +
            //             (id - kMinConfigKey_KVSKey));
            //     break;
            // }

            // if (id >= kMinConfigKey_MatterConfig)
            // {
            //     index = ((kMaxConfigKey_MatterFactory - kMinConfigKey_MatterFactory) +
            //             (id - kMinConfigKey_MatterConfig));
            //     break;
            // }

            // if (id >= kMinConfigKey_MatterFactory)
            // {
            //     index = (id - kMinConfigKey_MatterFactory);
            //     break;
            // }              

        // } while (0);
        
        return (id + 1);
    }

protected:

private:    
};
} // namespace Internal
} // namespace DeviceLayer
} // namespace chip

#endif