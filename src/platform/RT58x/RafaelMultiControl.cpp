/**********************************************************
 * Includes
 *********************************************************/
#include <lib/support/CodeUtils.h>
#include <lib/support/logging/CHIPLogging.h>

#include "RafaelMultiControl.h"
#include "util_log.h"

using namespace chip;
using namespace chip::app;
using namespace chip::DeviceLayer;
using namespace chip::DeviceLayer::Internal;
using namespace chip::Protocols::InteractionModel;
RafMultiControlManager RafMultiControlManager::sRafMultiManager;

void RafMultiControlManager::HandleBleMessage(const ChipDeviceEvent * aEvent)
{
    uint8_t cmd_type = aEvent->TRSPData.data[0];
    uint8_t buf_len = 0;
    uint8_t resp_buf[32] = {0};

    // info_color(LOG_RED, "Receive ble command:\n");
    // util_log_mem(UTIL_LOG_INFO, "  ", aEvent->TRSPData.data, aEvent->TRSPData.len, 0);

    switch (cmd_type){
        case ReadFirmwareInformation:
            buf_len = HandleReadFirmwareInformation(aEvent, resp_buf);
        break;
        case ReadDeviceNumber:
            buf_len = HandleReadDeviceNumber(aEvent, resp_buf);
        break;
        case ReadDeviceType:
            buf_len = HandleReadDeviceType(aEvent, resp_buf);
        break;
        case ReadClusterNumber:
            buf_len = HandleReadClusterNumber(aEvent, resp_buf);
        break;
        case ReadClusterId:
            buf_len = HandleReadClusterId(aEvent, resp_buf);
        break;
        case ReadDeviceMode:
            buf_len = HandleReadDeviceMode(aEvent, resp_buf);
        break;
        case CustomCommand:
            buf_len = HandleCustomCommand(aEvent, resp_buf);
        break;
        case ReadAttribute:
            buf_len = HandleReadAttribute(aEvent, resp_buf);
        break;
        case WriteAttribute:
            buf_len = HandleWriteAttribute(aEvent, resp_buf);
        break;
        default:
        break;
    }
    if(aEvent->TRSPData.data)
    {
        chip::Platform::MemoryFree(aEvent->TRSPData.data);
    }
    if(buf_len > 0)
    {
        uint8_t* buf = static_cast<uint8_t *>(chip::Platform::MemoryAlloc(buf_len));
        memcpy(buf,resp_buf,buf_len);

        PlatformMgr().ScheduleWork(SendCommandToBle, reinterpret_cast<intptr_t>(buf));
    }
}
void RafMultiControlManager::SendCommandToBle(intptr_t arg)
{
    uint8_t* buf = reinterpret_cast<uint8_t*>(arg);

    ChipDeviceEvent AppToBleEvent;
    AppToBleEvent.Type = DeviceEventType::kAppToBle;
    AppToBleEvent.TRSPData.len = buf[1]+2;
    AppToBleEvent.TRSPData.data = buf;
    BLEMgr().OnPlatformEvent(&AppToBleEvent);
    chip::Platform::MemoryFree(buf);
}
void RafMultiControlManager::ReportOnoff(uint16_t ep, bool onoff)
{
    ChipDeviceEvent AppToBleEvent;
    attr_resp_t report_attr_resp;
    uint32_t cluster_id = 0x0006;
    uint32_t attr_id = 0x0000;
    uint8_t buf_len = 13;
    uint8_t* buf = static_cast<uint8_t *>(chip::Platform::MemoryAlloc(buf_len));
    if(buf)
    {
        report_attr_resp.cmd = ReportAttribute;
        report_attr_resp.len = 11;
        report_attr_resp.ep = ep;
        report_attr_resp.cluster_id = cluster_id;
        report_attr_resp.attr_id = attr_id;
        memcpy(buf, &report_attr_resp, buf_len-1);
        buf[buf_len-1] = onoff;
        AppToBleEvent.Type = DeviceEventType::kAppToBle;
        AppToBleEvent.TRSPData.len = buf_len;
        AppToBleEvent.TRSPData.data = buf;
        BLEMgr().OnPlatformEvent(&AppToBleEvent);
        chip::Platform::MemoryFree(buf);
    }
}
void RafMultiControlManager::ReportLevel(uint16_t ep, uint8_t level)
{
    ChipDeviceEvent AppToBleEvent;
    attr_resp_t report_attr_resp;
    uint32_t cluster_id = 0x0008;
    uint32_t attr_id = 0x0000;
    uint8_t buf_len = 13;
    uint8_t* buf = static_cast<uint8_t *>(chip::Platform::MemoryAlloc(buf_len));
    if(buf)
    {
        report_attr_resp.cmd = ReportAttribute;
        report_attr_resp.len = 11;
        report_attr_resp.ep = ep;
        report_attr_resp.cluster_id = cluster_id;
        report_attr_resp.attr_id = attr_id;
        memcpy(buf, &report_attr_resp, buf_len-1);
        buf[buf_len-1] = level;
        AppToBleEvent.Type = DeviceEventType::kAppToBle;
        AppToBleEvent.TRSPData.len = buf_len;
        AppToBleEvent.TRSPData.data = buf;
        BLEMgr().OnPlatformEvent(&AppToBleEvent);
        chip::Platform::MemoryFree(buf);
    }
}
uint8_t RafMultiControlManager::HandleReadFirmwareInformation(const ChipDeviceEvent * aEvent, uint8_t* buf)
{
    uint8_t cmd_type = aEvent->TRSPData.data[0];
    uint8_t len = aEvent->TRSPData.data[1];
    uint8_t* value = &aEvent->TRSPData.data[2];
    uint8_t buf_len = 0;
    if(len == 1 && value[0] == 0)
    {
        //buf_len = 7;
        buf[buf_len++] = ReadFirmwareInformationResp;
        buf[buf_len++] = 1+strlen(CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING);
        buf[buf_len++] = RAF_STATUS_SUCCESS;
        buf_len += strlen(CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING);
        memcpy(buf+3, CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING, strlen(CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING));
    }
    else
    {
        buf[buf_len++] = ReadFirmwareInformationResp;
        buf[buf_len++] = 1;
        buf[buf_len++] = RAF_STATUS_FAIL;
    }
    return buf_len;
}
uint8_t RafMultiControlManager::HandleReadDeviceNumber(const ChipDeviceEvent * aEvent, uint8_t* buf)
{
    uint8_t cmd_type = aEvent->TRSPData.data[0];
    uint8_t len = aEvent->TRSPData.data[1];
    uint8_t* value = &aEvent->TRSPData.data[2];
    uint8_t buf_len = 0;
    uint8_t device_index = value[0];
    if(len == 1 && device_index == 0)
    {
        buf[buf_len++] = ReadDeviceNumberResp;
        buf[buf_len++] = 1;
        buf[buf_len++] = RAF_DEVICE_NUM;
    }
    return buf_len;
}
uint8_t RafMultiControlManager::HandleReadDeviceType(const ChipDeviceEvent * aEvent, uint8_t* buf)
{
    device_type_resp_t device_type_resp;
    uint8_t cmd_type = aEvent->TRSPData.data[0];
    uint8_t len = aEvent->TRSPData.data[1];
    uint8_t* value = &aEvent->TRSPData.data[2];
    uint8_t buf_len = 0;
    uint8_t device_index = value[0];
    if(len == 1 && device_index < RAF_DEVICE_NUM)
    {
        device_type_resp.len = 8;
        device_type_resp.status = RAF_STATUS_SUCCESS;
        device_type_resp.device_id = device_index;
        device_type_resp.device_type = device_type_list[device_index];
        device_type_resp.ep = endpoint_id_list[device_index];
    }
    else
    {
        device_type_resp.len = 1;
        device_type_resp.status = RAF_STATUS_FAIL;
    }
    buf_len = 2+device_type_resp.len;
    memcpy(buf, &device_type_resp, buf_len);
    return buf_len;
}

uint8_t RafMultiControlManager::HandleReadClusterNumber(const ChipDeviceEvent * aEvent, uint8_t* buf)
{
    uint8_t cmd_type = aEvent->TRSPData.data[0];
    uint8_t len = aEvent->TRSPData.data[1];
    uint8_t* value = &aEvent->TRSPData.data[2];
    uint8_t buf_len = 0;
    uint8_t device_index = value[0];
    if(len == 1 && device_index < RAF_DEVICE_NUM)
    {
        uint8_t cluster_number = cluster_number_list[device_index];
        buf[buf_len++] = ReadClusterNumberResp;
        buf[buf_len++] = 3;
        buf[buf_len++] = RAF_STATUS_SUCCESS;
        buf[buf_len++] = device_index;
        buf[buf_len++] = cluster_number;
    }
    else
    {
        buf[buf_len++] = ReadClusterNumberResp;
        buf[buf_len++] = 1;
        buf[buf_len++] = RAF_STATUS_FAIL;
    }
    return buf_len;
}

uint8_t RafMultiControlManager::HandleReadClusterId(const ChipDeviceEvent * aEvent, uint8_t* buf)
{
    cluster_id_resp_t cluster_id_resp;
    uint8_t buf_len = 0;
    uint8_t cmd_type = aEvent->TRSPData.data[0];
    uint8_t len = aEvent->TRSPData.data[1];
    uint8_t* value = &aEvent->TRSPData.data[2];
    uint8_t device_index = value[0];
    if(len == 2 && device_index < RAF_DEVICE_NUM)
    {
        uint8_t cluster_id_index = value[1];                
        uint32_t cluster_id;
        if(device_index == 0)
        {
            cluster_id = cluster_list_ep1[cluster_id_index];
        }
        cluster_id_resp.len = 7;
        cluster_id_resp.status = RAF_STATUS_SUCCESS;
        cluster_id_resp.device_id = device_index;
        cluster_id_resp.cluster_id_index = cluster_id_index;
        cluster_id_resp.cluster_id = cluster_id;
    }
    else
    {
        cluster_id_resp.len = 1;
        cluster_id_resp.status = RAF_STATUS_FAIL;
    }
    buf_len = 2+cluster_id_resp.len;
    memcpy(buf, &cluster_id_resp, buf_len);
    return buf_len;
}

uint8_t RafMultiControlManager::HandleReadDeviceMode(const ChipDeviceEvent * aEvent, uint8_t* buf)
{
    uint8_t cmd_type = aEvent->TRSPData.data[0];
    uint8_t len = aEvent->TRSPData.data[1];
    uint8_t* value = &aEvent->TRSPData.data[2];
    uint8_t buf_len = 0;
    if(len == 1 && value[0] == 0)
    {
        buf[buf_len++] = ReadDeviceModeResp;
        buf[buf_len++] = 1;
        buf[buf_len++] = (chip::Server::GetInstance().GetFabricTable().FabricCount() > 0) ;
    }
    return buf_len;
}

uint8_t RafMultiControlManager::HandleCustomCommand(const ChipDeviceEvent * aEvent, uint8_t* buf)
{
    uint8_t cmd_type = aEvent->TRSPData.data[0];
    uint8_t len = aEvent->TRSPData.data[1];
    uint8_t* value = &aEvent->TRSPData.data[2];
    uint8_t buf_len = 0;
    if(len > 0)
    {
        memcpy(buf, value, len+1);
        buf[len] = '\0';
        info_color(LOG_RED, "receive custom command %s\n", buf);
    }
    return buf_len;
}

uint8_t RafMultiControlManager::HandleReadAttribute(const ChipDeviceEvent * aEvent, uint8_t* buf)
{
    attr_resp_t read_attr_resp;
    uint8_t cmd_type = aEvent->TRSPData.data[0];
    uint8_t len = aEvent->TRSPData.data[1];
    uint8_t* value = &aEvent->TRSPData.data[2];
    util_log_mem(UTIL_LOG_INFO, "  ", value, len, 0);
    uint8_t buf_len = 0;
    Status status = Status::Failure;
    if(len >= 10)
    {
        memcpy(&read_attr_resp.ep, value, 10);
        if(read_attr_resp.cluster_id == 0x0006 && read_attr_resp.attr_id == 0x0000)
        {
            bool onoff;
            status = OnOffServer::Instance().getOnOffValue(read_attr_resp.ep, &onoff);
            if (status == Status::Success)
            {
                read_attr_resp.cmd = ReadAttributeResp;
                read_attr_resp.len = 12;
                read_attr_resp.status = RAF_STATUS_SUCCESS;
                buf_len = 2+read_attr_resp.len;
                memcpy(buf, &read_attr_resp, buf_len-1);
                memcpy(buf+buf_len-1, &onoff, sizeof(onoff));
            }
        }
    }
    if (status != Status::Success)
    {
        read_attr_resp.cmd = ReadAttributeResp;
        read_attr_resp.len = 1;
        read_attr_resp.status = RAF_STATUS_FAIL;
        buf_len = 2+read_attr_resp.len;
        memcpy(buf, &read_attr_resp, buf_len);
    }
    return buf_len;
}

uint8_t RafMultiControlManager::HandleWriteAttribute(const ChipDeviceEvent * aEvent, uint8_t* buf)
{
    attr_resp_t write_attr_resp;
    uint8_t cmd_type = aEvent->TRSPData.data[0];
    uint8_t len = aEvent->TRSPData.data[1];
    uint8_t* value = &aEvent->TRSPData.data[2];
    uint8_t buf_len = 0;
    Status status = Status::Failure;
    if(len >= 11)
    {
        memcpy(&write_attr_resp.ep, value, 10);
        if(write_attr_resp.cluster_id == 0x0006 && write_attr_resp.attr_id == 0x0000)
        {
            bool onoff = (value[10] > 0);
            status = OnOffServer::Instance().setOnOffValue(write_attr_resp.ep, onoff,true);
            if (status == Status::Success)
            {
                write_attr_resp.cmd = WriteAttributeResp;
                write_attr_resp.len = 12;
                write_attr_resp.status = RAF_STATUS_SUCCESS;
                buf_len = 2+write_attr_resp.len;
                memcpy(buf, &write_attr_resp, buf_len-1);
                memcpy(buf+buf_len-1, &onoff, sizeof(onoff));
            }
        }
    }    
    if (status != Status::Success)
    {
        write_attr_resp.cmd = WriteAttributeResp;
        write_attr_resp.len = 1;
        write_attr_resp.status = RAF_STATUS_FAIL;
        buf_len = 2+write_attr_resp.len;
        memcpy(buf, &write_attr_resp, buf_len);
    }
    return buf_len;
}
