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
 *          for the Rafael RT58x platforms.
 */

#pragma once
#if CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE

#include <ble/CHIPBleServiceData.h>

#include "ble_profile.h"

#include "FreeRTOS.h"
#include "timers.h"


namespace chip {
namespace DeviceLayer {
namespace Internal {

using namespace chip::Ble;
/**
 * Concrete implementation of the BLEManager singleton object for the RT58x platforms.
 */
class BLEManagerImpl final : public BLEManager, private BleLayer, private BlePlatformDelegate, private BleApplicationDelegate
{
    // Allow the BLEManager interface class to delegate method calls to
    // the implementation methods provided by this class.
    friend BLEManager;

#if RAF_ENABLE_MULTI_CONTROL && defined(RAF_ENABLE_ZIGBEE)
public:
    void BLESendMessage(uint8_t * data, uint8_t len);
#endif

private:
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

    CHIP_ERROR SubscribeCharacteristic(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId,
                                 const Ble::ChipBleUUID * charId) override;
    CHIP_ERROR UnsubscribeCharacteristic(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId,
                                   const Ble::ChipBleUUID * charId) override;
    CHIP_ERROR CloseConnection(BLE_CONNECTION_OBJECT conId) override;
    uint16_t GetMTU(BLE_CONNECTION_OBJECT conId) const override;
    CHIP_ERROR SendIndication(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId, const Ble::ChipBleUUID * charId,
                        System::PacketBufferHandle pBuf) override;
    CHIP_ERROR SendWriteRequest(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId, const Ble::ChipBleUUID * charId,
                          System::PacketBufferHandle pBuf) override;
    CHIP_ERROR SendReadRequest(BLE_CONNECTION_OBJECT conId, const ChipBleUUID * svcId, const ChipBleUUID * charId, PacketBufferHandle pBuf);
    CHIP_ERROR SendReadResponse(BLE_CONNECTION_OBJECT conId, BLE_READ_REQUEST_CONTEXT requestContext, const ChipBleUUID * svcId, const ChipBleUUID * charId);
    // ===== Members that implement virtual methods on BleApplicationDelegate.

    void NotifyChipConnectionClosed(BLE_CONNECTION_OBJECT conId) override;


    // ===== Members for internal use by the following friends.

    friend BLEManager & BLEMgr(void);
    friend BLEManagerImpl & BLEMgrImpl(void);

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

#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
    PacketBufferHandle c3AdditionalDataBufferHandle;
#endif

    CHIPoBLEServiceMode mServiceMode;
    char mDeviceName[kMaxDeviceNameLength + 1];
    void DriveBLEState(void);
    CHIP_ERROR StartAdvertising(void);
    CHIP_ERROR StopAdvertising(void);

    static ble_err_t ble_app_event_cb(void *p_param);
    static ble_err_t ble_service_data_cb(void *p_param);

    static void ble_evt_task(void * arg);
    static void ble_evt_handler(void *p_param);
    static void ble_svcs_matter_evt_handler(void *p_matter_evt_param);

    static int server_profile_init(uint8_t host_id);
    static int ble_init(void);
    static bool app_request_set(uint8_t host_id, uint32_t request, bool from_isr);
    static void app_evt_handler(void *p_param);

    static void BleAdvTimeoutHandler(TimerHandle_t xTimer);
    static void BleConnTimeoutHandler(TimerHandle_t xTimer);
    static void DriveBLEState(intptr_t arg);
    
    void CancelBleConnTimeoutTimer(void);
    void CancelBleAdvTimeoutTimer(void);
    void StartBleAdvTimeoutTimer(uint32_t aTimeoutInMs);
#if RAF_ENABLE_MULTI_CONTROL
    CHIP_ERROR ConfigureAdvertisingData(uint8_t matter_adv_enabled);
    static void ble_svcs_trsps_evt_handler(void *p_matter_evt_param);
    void HandleAppToBleEvent(const ChipDeviceEvent * event);
    static void fota_timer_handler(TimerHandle_t timer);
    static bool fota_sw_timer_start(void);
    static void ble_svcs_fota_evt_handler(ble_evt_att_param_t *p_param);
#else
    CHIP_ERROR ConfigureAdvertisingData(void);
#endif

#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
    CHIP_ERROR EncodeAdditionalDataTlv();
#endif

    static BLEManagerImpl sInstance;
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
 * that are specific to the RT58x platforms.
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
