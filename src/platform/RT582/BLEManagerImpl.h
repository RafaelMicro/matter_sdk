/*
 *
 *    Copyright (c) 2020-2021 Project CHIP Authors
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
 *          Provides an implementation of the BLEManager singleton object
 *          for the Silicon Labs EFR32 platforms.
 */

#pragma once
#if CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE

#include "FreeRTOS.h"
#include "timers.h"

namespace chip {
namespace DeviceLayer {
namespace Internal {

using namespace chip::Ble;
/**
 * Concrete implementation of the BLEManager singleton object for the EFR32 platforms.
 */
class BLEManagerImpl final : public BLEManager, private BleLayer, private BlePlatformDelegate, private BleApplicationDelegate
{
    // Allow the BLEManager interface class to delegate method calls to
    // the implementation methods provided by this class.
    friend BLEManager;

    // ===== Members that implement the BLEManager internal interface.
    CHIP_ERROR _Init(void);
    void _Shutdown() {}
    bool _IsAdvertisingEnabled(void);
    CHIP_ERROR _SetAdvertisingEnabled(bool val);
    bool _IsAdvertising(void);
    CHIP_ERROR _SetAdvertisingMode(BLEAdvertisingMode mode);
    CHIP_ERROR _GetDeviceName(char * buf, size_t bufSize);
    CHIP_ERROR _SetDeviceName(const char * deviceName);
    uint16_t _NumConnections(void);
    void _OnPlatformEvent(const ChipDeviceEvent * event);
    BleLayer * _GetBleLayer(void);

    // ===== Members that implement virtual methods on BlePlatformDelegate.

    bool SubscribeCharacteristic(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId,
                                 const Ble::ChipBleUUID * charId) override;
    bool UnsubscribeCharacteristic(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId,
                                   const Ble::ChipBleUUID * charId) override;
    bool CloseConnection(BLE_CONNECTION_OBJECT conId) override;
    uint16_t GetMTU(BLE_CONNECTION_OBJECT conId) const override;
    bool SendIndication(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId, const Ble::ChipBleUUID * charId,
                        System::PacketBufferHandle pBuf) override;
    bool SendWriteRequest(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId, const Ble::ChipBleUUID * charId,
                          System::PacketBufferHandle pBuf) override;
    bool SendReadRequest(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId, const Ble::ChipBleUUID * charId,
                         System::PacketBufferHandle pBuf) override;
    bool SendReadResponse(BLE_CONNECTION_OBJECT conId, BLE_READ_REQUEST_CONTEXT requestContext, const Ble::ChipBleUUID * svcId,
                          const Ble::ChipBleUUID * charId) override;

    // ===== Members that implement virtual methods on BleApplicationDelegate.

    void NotifyChipConnectionClosed(BLE_CONNECTION_OBJECT conId) override;


    CHIP_ERROR HandleThreadStateChange(const ChipDeviceEvent * event);
    CHIP_ERROR HandleOperationalNetworkEnabled(const ChipDeviceEvent * event);

    // ===== Members for internal use by the following friends.

    friend BLEManager & BLEMgr(void);
    friend BLEManagerImpl & BLEMgrImpl(void);

    static BLEManagerImpl sInstance;

    // ===== Private members reserved for use by this class only.
private:
    enum class Flags : uint16_t
    {
        kAdvertisingEnabled     = 0x0001,
        kFastAdvertisingEnabled = 0x0002,
        kAdvertising            = 0x0004,
        kRestartAdvertising     = 0x0008,
        kRTBLEStackInitialized  = 0x0010,
        kDeviceNameSet          = 0x0020,
    };
    BitFlags<BLEManagerImpl::Flags> mFlags;

    enum
    {
        kMaxConnections      = BLE_LAYER_NUM_BLE_ENDPOINTS,
        kMaxDeviceNameLength = 16,
        kUnusedIndex         = 0xFF,
    };

    // The advertising set handle allocated from Bluetooth stack.
    uint8_t advertising_set_handle = 0xff;
#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
    PacketBufferHandle c3AdditionalDataBufferHandle;
#endif

    CHIPoBLEServiceMode mServiceMode;
    char mDeviceName[kMaxDeviceNameLength + 1];

    void DriveBLEState(void);
    CHIP_ERROR ConfigureAdvertising(void);
    CHIP_ERROR StartAdvertising(void);
    CHIP_ERROR StopAdvertising(void);
    static void ble_evt_task(void * arg);
    static void ble_evt_indication_cb(uint32_t data_len);
    static void ble_evt_handler(void *p_param);

    static void ble_svcs_matter_evt_handler(void *p_matter_evt_param);
    static int server_profile_init(uint8_t host_id);
    static int ble_init(void);
    static bool app_request_set(uint8_t host_id, uint32_t request, bool from_isr);
    static void app_evt_handler(void *p_param);
    static int adv_init(void);
    static int adv_enable(uint8_t host_id);

    static void DriveBLEState(intptr_t arg);
    CHIP_ERROR ConfigureAdvertisingData(void);
    static void CancelBleAdvTimeoutTimer(void);
    static void StartBleAdvTimeoutTimer(uint32_t aTimeoutInMs);

#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
    CHIP_ERROR EncodeAdditionalDataTlv();
#endif
};

/**
 * Returns a reference to the public interface of the BLEManager singleton object.
 *
 * Internal components should use this to access features of the BLEManager object
 * that are common to all platforms.
 */
inline BLEManager & BLEMgr(void)
{
    return BLEManagerImpl::sInstance;
}

/**
 * Returns the platform-specific implementation of the BLEManager singleton object.
 *
 * Internal components can use this to gain access to features of the BLEManager
 * that are specific to the EFR32 platforms.
 */
inline BLEManagerImpl & BLEMgrImpl(void)
{
    return BLEManagerImpl::sInstance;
}

inline BleLayer * BLEManagerImpl::_GetBleLayer()
{
    return this;
}

} // namespace Internal
} // namespace DeviceLayer
} // namespace chip

#endif // CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
