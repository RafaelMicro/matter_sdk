/**********************************************************
 * Includes
 *********************************************************/
#ifndef RAF_MULTI_CONTROL_H
#define RAF_MULTI_CONTROL_H

#include <lib/core/CHIPError.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/RT58x/BLEManagerImpl.h>
#include <app/server/Server.h>
#include <app/clusters/on-off-server/on-off-server.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app/InteractionModelEngine.h>
/**********************************************************
 * Device Config
 *********************************************************/
#define RAF_DEVICE_NUM 1
#define RAF_CLUSTER_NUM_EP1 3

static uint32_t device_type_list[RAF_DEVICE_NUM] = {CHIP_DEVICE_CONFIG_DEVICE_TYPE};
static uint16_t endpoint_id_list[RAF_DEVICE_NUM] = {1};
static uint8_t  cluster_number_list[RAF_DEVICE_NUM] = {RAF_CLUSTER_NUM_EP1};

static uint32_t cluster_list_ep1[RAF_CLUSTER_NUM_EP1] = {0x0003, 0x0006, 0x0008};

/**********************************************************
 * Define
 *********************************************************/
#define RAF_STATUS_SUCCESS 0
#define RAF_STATUS_FAIL    1

class RafMultiControlManager
{
public:
    enum Ble_Command_Type: uint8_t{
        ReadFirmwareInformation     = 0x00,
        ReadDeviceNumber            = 0x01,
        ReadDeviceType              = 0x02,
        ReadClusterNumber           = 0x03,
        ReadClusterId               = 0x04,
        ReadDeviceMode              = 0x05,
        CustomCommand               = 0x07,
        ReadAttribute               = 0x20,
        WriteAttribute              = 0x21,
        ReadFirmwareInformationResp = 0x80,
        ReadDeviceNumberResp        = 0x81,
        ReadDeviceTypeResp          = 0x82,
        ReadClusterNumberResp       = 0x83,
        ReadClusterIdResp           = 0x84,
        ReadDeviceModeResp          = 0x85,
        CustomCommandResp           = 0x87,
        ReadAttributeResp           = 0xA0,
        WriteAttributeResp          = 0xA1,
        ReportAttribute             = 0xA2,
    }ble_cmd;
    static void HandleBleMessage(const chip::DeviceLayer::ChipDeviceEvent * aEvent);
    static void SendCommandToBle(intptr_t arg);
    static void ReportOnoff(uint16_t ep, bool onoff);
    static void ReportLevel(uint16_t ep, uint8_t level);
    
    static uint8_t HandleReadFirmwareInformation(const  chip::DeviceLayer::ChipDeviceEvent * aEvent, uint8_t* buf);
    static uint8_t HandleReadDeviceNumber(const  chip::DeviceLayer::ChipDeviceEvent * aEvent, uint8_t* buf);
    static uint8_t HandleReadDeviceType(const  chip::DeviceLayer::ChipDeviceEvent * aEvent, uint8_t* buf);
    static uint8_t HandleReadClusterNumber(const  chip::DeviceLayer::ChipDeviceEvent * aEvent, uint8_t* buf);
    static uint8_t HandleReadClusterId(const  chip::DeviceLayer::ChipDeviceEvent * aEvent, uint8_t* buf);
    static uint8_t HandleReadDeviceMode(const  chip::DeviceLayer::ChipDeviceEvent * aEvent, uint8_t* buf);
    static uint8_t HandleCustomCommand(const  chip::DeviceLayer::ChipDeviceEvent * aEvent, uint8_t* buf);
    static uint8_t HandleReadAttribute(const  chip::DeviceLayer::ChipDeviceEvent * aEvent, uint8_t* buf);
    static uint8_t HandleWriteAttribute(const  chip::DeviceLayer::ChipDeviceEvent * aEvent, uint8_t* buf);

private:
    friend RafMultiControlManager & RafMCMgr(void);
    static RafMultiControlManager sRafMultiManager;
};

inline RafMultiControlManager & RafMCMgr(void)
{
    return RafMultiControlManager::sRafMultiManager;
}
//=============================================================================
//                Private Struct
//=============================================================================
typedef struct __attribute__((packed))
{
    uint8_t cmd = RafMultiControlManager::Ble_Command_Type::ReadDeviceTypeResp;
    uint8_t len;
    uint8_t status;
    uint8_t device_id;
    uint32_t device_type;
    uint16_t ep;
} device_type_resp_t;

typedef struct __attribute__((packed))
{
    uint8_t cmd = RafMultiControlManager::Ble_Command_Type::ReadClusterIdResp;
    uint8_t len;
    uint8_t status;
    uint8_t device_id;
    uint8_t cluster_id_index;
    uint32_t cluster_id;
} cluster_id_resp_t;

typedef struct __attribute__((packed))
{
    uint8_t cmd;
    uint8_t len;
    uint8_t status;
    uint16_t ep;
    uint32_t cluster_id;
    uint32_t attr_id;
} attr_resp_t;
typedef struct __attribute__((packed))
{
    uint8_t cmd;
    uint8_t len;
    uint16_t ep;
    uint32_t cluster_id;
    uint32_t attr_id;
} report_attr_t;
#endif //RAF_MULTI_CONTROL_H
