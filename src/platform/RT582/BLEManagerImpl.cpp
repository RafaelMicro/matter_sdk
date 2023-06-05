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

/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>
#if CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE


#include <platform/internal/BLEManager.h>

#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"

#include <ble/CHIPBleServiceData.h>
#include <lib/support/CodeUtils.h>
#include <lib/support/logging/CHIPLogging.h>
#include <platform/CommissionableDataProvider.h>
#include <platform/DeviceInstanceInfoProvider.h>
#include <crypto/RandUtils.h>

#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
#include <setup_payload/AdditionalDataPayloadGenerator.h>
#endif

#include "task_dual.h"
#include "ble_event.h"
#include "ble_profile.h"
#include "ble_att_gatt.h"
#include "util_log.h"

// #include "rfb_comm_common.h"
extern "C" void rafael_rfb_init(void);
bool ble_active = false;

using namespace ::chip;
using namespace ::chip::Ble;

namespace chip {
namespace DeviceLayer {
namespace Internal {

namespace {

#define CHIP_ADV_DATA_TYPE_FLAGS 0x01
#define CHIP_ADV_DATA_TYPE_UUID 0x03
#define CHIP_ADV_DATA_TYPE_NAME 0x09
#define CHIP_ADV_DATA_TYPE_SERVICE_DATA 0x16

#define CHIP_ADV_DATA_FLAGS 0x06

#define CHIP_ADV_DATA 0
#define CHIP_ADV_SCAN_RESPONSE_DATA 1
#define CHIP_ADV_SHORT_UUID_LEN 2

// Default Connection  parameters
#define BLE_CONFIG_MIN_INTERVAL (48) // Time = Value x 0.625 ms = 30ms
#define BLE_CONFIG_MAX_INTERVAL (160) // Time = Value x 0.625 ms = 100ms
#define BLE_CONFIG_LATENCY (0)
#define BLE_CONFIG_TIMEOUT (100)          // Time = Value x 10 ms = 1s
#define BLE_CONFIG_MIN_CE_LENGTH (0)      // Leave to min value
#define BLE_CONFIG_MAX_CE_LENGTH (0xFFFF) // Leave to max value

#define BLE_APP_CB_QUEUE_SIZE           16
#define APP_ISR_QUEUE_SIZE              2
#define APP_REQ_QUEUE_SIZE              6
#define APP_QUEUE_SIZE                  (BLE_APP_CB_QUEUE_SIZE + APP_ISR_QUEUE_SIZE + APP_REQ_QUEUE_SIZE)

#define APP_TRSP_P_HOST_ID              0

// Advertising device name
#define DEVICE_NAME                     'R', 'A', 'F', '*', 'L', 'I', 'G', 'H', 'T'

/* FreeRTOS sw timer */
TimerHandle_t sbleAdvTimeoutTimer;

/* advertising configuration */
#define BLERT_ADV_MAX_NO (2)
#define BLERT_SCAN_RSP_MAX_NO (2)
#define BLERT_MAX_ADV_DATA_LEN (31)
#define CHIP_ADV_SHORT_UUID_LEN (2)

// GAP device name
static const uint8_t         DEVICE_NAME_STR[] = {DEVICE_NAME};

// Device BLE Address
static ble_gap_addr_t  DEVICE_ADDR = {.addr_type = RANDOM_STATIC_ADDR,
                                        .addr = {0x21, 0x21, 0x23, 0x24, 0x25, 0xC6 }
                                        };


static TaskHandle_t BluetoothEventTaskHandle;

static xQueueHandle g_app_msg_q;
static SemaphoreHandle_t semaphore_cb;
static SemaphoreHandle_t semaphore_isr;
static SemaphoreHandle_t semaphore_app;
static uint8_t g_rx_buffer[BLE_GATT_DATA_LENGTH_MAX];
static uint8_t g_rx_buffer_length;
static ble_cfg_t gt_app_cfg;
static uint8_t g_advertising_host_id = BLE_HOSTID_RESERVED;
static uint8_t g_mtu_size = BLE_GATT_ATT_MTU_MAX;
const uint8_t UUID_CHIPoBLEService[]    = { 0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80,
                                         0x00, 0x10, 0x00, 0x00, 0xF6, 0xFF, 0x00, 0x00 };
const uint8_t ShortUUID_CHIPoBLEService[]  = { 0xF6, 0xFF };
const ChipBleUUID ChipUUID_CHIPoBLEChar_RX = { { 0x18, 0xEE, 0x2E, 0xF5, 0x26, 0x3D, 0x45, 0x59, 0x95, 0x9F, 0x4F, 0x9C, 0x42, 0x9F,
                                                 0x9D, 0x11 } };
const ChipBleUUID ChipUUID_CHIPoBLEChar_TX = { { 0x18, 0xEE, 0x2E, 0xF5, 0x26, 0x3D, 0x45, 0x59, 0x95, 0x9F, 0x4F, 0x9C, 0x42, 0x9F,
                                                 0x9D, 0x12 } };

static uint8_t g_use_slow_adv_interval = false;

#define BLE_ERR_STATE_TRANSLATE(state)  (state == BLE_ERR_OK)?CHIP_NO_ERROR:\
                                        (state == BLE_ERR_UNKNOW_TYPE)?CHIP_ERROR_WELL_EMPTY:\
                                        (state == BLE_ERR_NOT_INIT)?CHIP_ERROR_WELL_UNINITIALIZED:\
                                        (state == BLE_ERR_DUPLICATE_INIT)?CHIP_NO_ERROR:\
                                        (state == BLE_ERR_DATA_MALLOC_FAIL)?CHIP_ERROR_NO_MEMORY:\
                                        (state == BLE_ERR_QUEUE_MALLOC_FAIL)?CHIP_ERROR_NO_MEMORY:\
                                        (state == BLE_ERR_THREAD_MALLOC_FAIL)?CHIP_ERROR_NO_MEMORY:\
                                        (state == BLE_ERR_SEMAPHORE_MALLOC_FAIL)?CHIP_ERROR_NO_MEMORY:\
                                        (state == BLE_ERR_WRONG_CONFIG)?CHIP_ERROR_MESSAGE_INCOMPLETE:\
                                        (state == BLE_BUSY)?CHIP_ERROR_SENDING_BLOCKED:\            
                                        (state == BLE_ERR_SENDTO_POINTER_NULL)?CHIP_ERROR_SENDING_BLOCKED:\ 
                                        (state == BLE_ERR_SENDTO_FAIL)?CHIP_ERROR_SENDING_BLOCKED:\ 
                                        (state == BLE_ERR_RECVFROM_POINTER_NULL)?CHIP_ERROR_SENDING_BLOCKED:\ 
                                        (state == BLE_ERR_RECVFROM_NO_DATA)?CHIP_ERROR_SENDING_BLOCKED:\ 
                                        (state == BLE_ERR_RECVFROM_FAIL)?CHIP_ERROR_SENDING_BLOCKED:\ 
                                        (state == BLE_ERR_RECVFROM_LEN_NOT_ENOUGH)?CHIP_ERROR_MESSAGE_INCOMPLETE:\ 
                                        (state == BLE_ERR_ALLOC_MEMORY_FAIL)?CHIP_ERROR_NO_MEMORY:\ 
                                        (state == BLE_ERR_TIMER_OP)?CHIP_ERROR_TIMEOUT:\ 
                                        (state == BLE_ERR_INVALID_STATE)?CHIP_ERROR_INCORRECT_STATE:\ 
                                        (state == BLE_ERR_INVALID_PARAMETER)?CHIP_ERROR_MESSAGE_INCOMPLETE:\ 
                                        (state == BLE_ERR_CMD_NOT_SUPPORTED)?CHIP_ERROR_NO_MESSAGE_HANDLER:\ 
                                        (state == BLE_ERR_INVALID_HOST_ID)?CHIP_ERROR_NO_MESSAGE_HANDLER: \ 
                                        (state == BLE_ERR_INVALID_HANDLE)?CHIP_ERROR_NO_MESSAGE_HANDLER:CHIP_ERROR_MESSAGE_INCOMPLETE                                                                       
typedef enum
{
    QUEUE_TYPE_APP_REQ,   /**< Application queue type: application request.*/
    QUEUE_TYPE_OTHERS,    /**< Application queue type: others including BLE events and BLE service events.*/
} app_queue_param_type_t;

typedef enum
{
    APP_REQUEST_IDLE,             /**< Application request event: idle.*/
    APP_REQUEST_ADV_START,        /**< Application request event: advertising start.*/
    APP_REQUEST_TRSPS_DATA_SEND,  /**< Application request event: TRSP server data send.*/
} app_request_t;

typedef enum
{
    STATE_STANDBY,        /**< Application state: standby.*/
    STATE_ADVERTISING,    /**< Application state: advertising.*/
    STATE_CONNECTED,      /**< Application state: connected.*/
} ble_state_t;


typedef struct
{
    uint8_t   host_id;    /**< Application request parameter: host id.*/
    uint16_t  app_req;    /**< Application request parameter: @ref app_request_t "application request event".*/
} app_req_param_t;

typedef struct
{
    uint8_t   event;        /**< Application queue parameter: event.*/
    uint8_t   from_isr;     /**< Application queue parameter: Dose the Request come from interruption? */
    uint16_t  param_type;   /**< Application queue parameter: @ref app_queue_param_type_t "application queue type".*/
    union
    {
        app_req_param_t  app_req;   /**< Application queue parameter: application request event. */
        ble_tlv_t        *pt_tlv;   /**< Application queue parameter: parameters (type: @ref ble_event_t, length, and value). */
    } param;
} app_queue_t;

} // namespace

BLEManagerImpl BLEManagerImpl::sInstance;

void BLEManagerImpl::ble_evt_task(void * arg)
{
    ChipLogDetail(DeviceLayer, "BLE task running");
    int status;
    app_queue_t p_app_q;

    status = BLE_ERR_OK;

    // BLE default setting and profile init
    status = ble_init();
    if (status != BLE_ERR_OK)
    {
        ChipLogError(DeviceLayer, "ble_init fail: %d", status);
        while (1);
    }

    // start adv
    //if (app_request_set(APP_TRSP_P_HOST_ID, APP_REQUEST_ADV_START, false) == false)
    //{
        // No Application queue buffer. Error.
    //}

    while (true)
    {
        if (xQueueReceive(g_app_msg_q, &p_app_q, portMAX_DELAY) == pdTRUE)
        {
            switch (p_app_q.param_type)
            {
            case QUEUE_TYPE_APP_REQ:
            {
                if (p_app_q.from_isr == true)
                {
                    xSemaphoreGive(semaphore_isr);
                }
                else
                {
                    xSemaphoreGive(semaphore_app);
                }
                app_evt_handler(&p_app_q.param.app_req);
            }
            break;

            case QUEUE_TYPE_OTHERS:
            {
                if (p_app_q.param.pt_tlv != NULL)
                {
                    switch (p_app_q.param.pt_tlv->type)
                    {
                    case BLE_APP_GENERAL_EVENT:
                        ble_evt_handler((ble_evt_param_t *)p_app_q.param.pt_tlv->value);
                        break;

                    case BLE_APP_SERVICE_EVENT:
                    {
                        ble_evt_att_param_t *p_svcs_param = (ble_evt_att_param_t *)p_app_q.param.pt_tlv->value;

                        switch (p_svcs_param->gatt_role)
                        {
                        case BLE_GATT_ROLE_CLIENT:
                            att_db_link[p_svcs_param->host_id].p_client_db[p_svcs_param->cb_index]->att_handler(p_svcs_param);
                            break;

                        case BLE_GATT_ROLE_SERVER:
                            att_db_link[p_svcs_param->host_id].p_server_db[p_svcs_param->cb_index]->att_handler(p_svcs_param);
                            break;

                        default:
                            break;
                        }
                    }
                    break;

                    default:
                        break;
                    }

                    // free
                    vPortFree(p_app_q.param.pt_tlv);
                    xSemaphoreGive(semaphore_cb);
                }
            }
            break;

            default:
                break;
            }
        }
    }
}

void BLEManagerImpl::ble_evt_indication_cb(uint32_t data_len)
{
    int i32_err;
    uint8_t *p_buf;
    app_queue_t p_app_q;

    if (xSemaphoreTake(semaphore_cb, 0) == pdPASS)
    {
        do
        {
            p_buf = (uint8_t *)pvPortMalloc(data_len);
            if (!p_buf)
            {
                xSemaphoreGive(semaphore_cb);
                break;
            }

            p_app_q.event = 0;
            i32_err = ble_event_msg_recvfrom(p_buf, &data_len);
            p_app_q.param_type = QUEUE_TYPE_OTHERS;
            p_app_q.param.pt_tlv = (ble_tlv_t *)p_buf;

            if (i32_err == 0)
            {
                while (xQueueSendToBack(g_app_msg_q, &p_app_q, 1) != pdTRUE);
            }
            else
            {
                info_color(LOG_RED, "[%s] err = %d !\n", __func__, (ble_err_t)i32_err);
                vPortFree(p_buf);
                xSemaphoreGive(semaphore_cb);
            }
        } while (0);
    }
}

void BLEManagerImpl::ble_evt_handler(void *p_param)
{
    ble_evt_param_t *p_ble_evt_param =(ble_evt_param_t *)p_param;
    switch (p_ble_evt_param->event)
    {
    case BLE_ADV_EVT_SET_ENABLE:
    {
        ble_evt_adv_set_adv_enable_t *p_adv_enable = (ble_evt_adv_set_adv_enable_t *)&p_ble_evt_param->event_param.ble_evt_adv.param.evt_set_adv_enable;

        if (p_adv_enable->status == BLE_HCI_ERR_CODE_SUCCESS)
        {
            if (p_adv_enable->adv_enabled == true)
            {
                if (g_advertising_host_id != BLE_HOSTID_RESERVED)
                {
                    ble_app_link_info[g_advertising_host_id].state = STATE_ADVERTISING;
                }
                ChipLogDetail(DeviceLayer, "Advertising...");
            }
            else
            {
                if (g_advertising_host_id != BLE_HOSTID_RESERVED)
                {
                    ble_app_link_info[g_advertising_host_id].state = STATE_STANDBY;
                }
                ChipLogDetail(DeviceLayer, "Idle.");
            }
        }
        else
        {
            ChipLogError(DeviceLayer, "Advertising enable failed.");
        }
    }
    break;

    case BLE_GAP_EVT_CONN_COMPLETE:
    {
        ble_evt_gap_conn_complete_t *p_conn_param = (ble_evt_gap_conn_complete_t *)&p_ble_evt_param->event_param.ble_evt_gap.param.evt_conn_complete;

        if (p_conn_param->status != BLE_HCI_ERR_CODE_SUCCESS)
        {
            ChipLogError(DeviceLayer, "Connect failed, error code = 0x%02x", p_conn_param->status);
        }
        else
        {
            ble_active = true;
            ble_app_link_info[p_conn_param->host_id].state = STATE_CONNECTED;
            ChipLogProgress(DeviceLayer, "Connected, ID=%d, Connected to %02x:%02x:%02x:%02x:%02x:%02x",
                       p_conn_param->host_id,
                       p_conn_param->peer_addr.addr[5],
                       p_conn_param->peer_addr.addr[4],
                       p_conn_param->peer_addr.addr[3],
                       p_conn_param->peer_addr.addr[2],
                       p_conn_param->peer_addr.addr[1],
                       p_conn_param->peer_addr.addr[0]);
        }
    }
    break;

    case BLE_GAP_EVT_CONN_PARAM_UPDATE:
    {
        ble_evt_gap_conn_param_update_t *p_conn_param = (ble_evt_gap_conn_param_update_t *)&p_ble_evt_param->event_param.ble_evt_gap.param.evt_conn_param_update;

        if (p_conn_param->status != BLE_HCI_ERR_CODE_SUCCESS)
        {
            ChipLogError(DeviceLayer, "Connection update failed, error code = 0x%02x", p_conn_param->status);
        }
        else
        {
            ChipLogDetail(DeviceLayer, "Connection updated");
            ChipLogDetail(DeviceLayer, "ID: %d, ", p_conn_param->host_id);
            ChipLogDetail(DeviceLayer, "Interval: %d, ", p_conn_param->conn_interval);
            ChipLogDetail(DeviceLayer, "Latency: %d, ", p_conn_param->periph_latency);
            ChipLogDetail(DeviceLayer, "Supervision Timeout: %d", p_conn_param->supv_timeout);
        }
    }
    break;

    case BLE_GAP_EVT_PHY_READ:
    case BLE_GAP_EVT_PHY_UPDATE:
    {
        ble_evt_gap_phy_t *p_phy_param = (ble_evt_gap_phy_t *)&p_ble_evt_param->event_param.ble_evt_gap.param.evt_phy;
        if (p_phy_param->status != BLE_HCI_ERR_CODE_SUCCESS)
        {
            ChipLogError(DeviceLayer, "PHY update/read failed, error code = 0x%02x", p_phy_param->status);
        }
        else
        {
            ChipLogDetail(DeviceLayer, "PHY updated/read, ID: %d, TX PHY: %d, RX PHY: %d", p_phy_param->host_id, p_phy_param->tx_phy, p_phy_param->rx_phy);
        }
    }
    break;

    case BLE_ATT_GATT_EVT_MTU_EXCHANGE:
    {
        ble_evt_mtu_t *p_mtu_param = (ble_evt_mtu_t *)&p_ble_evt_param->event_param.ble_evt_att_gatt.param.ble_evt_mtu;

        g_mtu_size = p_mtu_param->mtu; // update to real mtu size

        ChipLogDetail(DeviceLayer, "MTU Exchanged, ID:%d, size: %d", p_mtu_param->host_id, p_mtu_param->mtu);
    }
    break;

    case BLE_ATT_GATT_EVT_WRITE_SUGGESTED_DEFAULT_DATA_LENGTH:
    {
        ble_evt_suggest_data_length_set_t *p_data_len_param = (ble_evt_suggest_data_length_set_t *)&p_ble_evt_param->event_param.ble_evt_att_gatt.param.ble_evt_suggest_data_length_set;

        if (p_data_len_param->status == BLE_HCI_ERR_CODE_SUCCESS)
        {
            ChipLogDetail(DeviceLayer, "Write default data length, status: %d", p_data_len_param->status);
        }
        else
        {
            ChipLogError(DeviceLayer, "Write default data length, status: %d", p_data_len_param->status);
        }
    }
    break;

    case BLE_ATT_GATT_EVT_DATA_LENGTH_CHANGE:
    {
        ble_evt_data_length_change_t *p_data_len_param = (ble_evt_data_length_change_t *)&p_ble_evt_param->event_param.ble_evt_att_gatt.param.ble_evt_data_length_change;
        ChipLogDetail(DeviceLayer, "Data length changed, ID: %n", p_data_len_param->host_id);
        ChipLogDetail(DeviceLayer, "MaxTxOctets: %d  MaxTxTime:%d", p_data_len_param->max_tx_octets, p_data_len_param->max_tx_time);
        ChipLogDetail(DeviceLayer, "MaxRxOctets: %d  MaxRxTime:%d", p_data_len_param->max_rx_octets, p_data_len_param->max_rx_time);
    }
    break;

    case BLE_GAP_EVT_DISCONN_COMPLETE:
    {
        ChipDeviceEvent disconnectEvent;
        ble_evt_gap_disconn_complete_t *p_disconn_param = (ble_evt_gap_disconn_complete_t *)&p_ble_evt_param->event_param.ble_evt_gap.param.evt_disconn_complete;
        if (p_disconn_param->status != BLE_HCI_ERR_CODE_SUCCESS)
        {
            ChipLogError(DeviceLayer, "Disconnect failed, error code = 0x%02x", p_disconn_param->status);
        }
        else
        {
            ChipLogProgress(DeviceLayer, "Disconnect, ID:%d, Reason:0x%02x", p_disconn_param->host_id, p_disconn_param->reason);
        }        
        /* Send Connection close event */
        disconnectEvent.Type = DeviceEventType::kCHIPoBLEConnectionClosed;
        PlatformMgr().PostEvent(&disconnectEvent);

    }
    break;

    default:
        break;
    }
}

bool BLEManagerImpl::app_request_set(uint8_t host_id, uint32_t request, bool from_isr)
{
    app_queue_t p_app_q;

    p_app_q.event = 0; // from BLE
    p_app_q.param_type = QUEUE_TYPE_APP_REQ;
    p_app_q.param.app_req.host_id = host_id;
    p_app_q.param.app_req.app_req = request;

    if (from_isr == false)
    {
        if (xSemaphoreTake(semaphore_app, 0) == pdTRUE)
        {
            p_app_q.from_isr = false;
            if (xQueueSendToBack(g_app_msg_q, &p_app_q, 0) != pdTRUE)
            {
                // send error
                xSemaphoreGive(semaphore_app);
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        BaseType_t context_switch = pdFALSE;

        if (xSemaphoreTakeFromISR(semaphore_isr, &context_switch) == pdTRUE)
        {
            p_app_q.from_isr = true;
            context_switch = pdFALSE;
            if (xQueueSendToBackFromISR(g_app_msg_q, &p_app_q, &context_switch) != pdTRUE)
            {
                context_switch = pdFALSE;
                xSemaphoreGiveFromISR(semaphore_isr, &context_switch);
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

void BLEManagerImpl::ble_svcs_matter_evt_handler(void *p_matter_evt_param)
{
    ble_evt_att_param_t *p_param = (ble_evt_att_param_t *)p_matter_evt_param;
    PacketBufferHandle data;
    CHIP_ERROR err = CHIP_NO_ERROR;

    if (p_param->gatt_role == BLE_GATT_ROLE_SERVER)
    {
        /* ----------------- Handle event from client ----------------- */
        switch (p_param->event)
        {
        case BLESERVICE_MATTER_CLIENT_TX_BUFFER_WRITE_EVENT:
        {
            System::PacketBufferHandle buf;
            buf = System::PacketBufferHandle::New(p_param->length);
            memcpy(buf->Start(), p_param->data, p_param->length);
            buf->SetDataLength(p_param->length);
            //ChipLogDetail(DeviceLayer,"Write request/command received for CHIPoBLE RX characteristic (len %u)",buf->DataLength());

            // Post an event to the CHIP queue to deliver the data into the CHIP stack.
            {
                ChipDeviceEvent event;
                event.Type                        = DeviceEventType::kCHIPoBLEWriteReceived;
                event.CHIPoBLEWriteReceived.ConId = p_param->host_id;
                event.CHIPoBLEWriteReceived.Data  = std::move(buf).UnsafeRelease();
                err                               = PlatformMgr().PostEvent(&event);
            }
        }
        break;

        case BLESERVICE_MATTER_ADDITIONAL_COMMISSIONING_RELATED_DATA_READ_EVENT:
        {
            ble_err_t status;
            const uint8_t readDataRsp[] = "matter data";
            ble_gatt_data_param_t gatt_data_param;

            ChipLogDetail(DeviceLayer, "read matter data\n");
            gatt_data_param.host_id = p_param->host_id;
            gatt_data_param.handle_num = p_param->handle_num;
            gatt_data_param.length = SIZE_STRING(readDataRsp);
            gatt_data_param.p_data = (uint8_t *)readDataRsp;

            status = ble_svcs_data_send(TYPE_BLE_GATT_READ_RSP, &gatt_data_param);
            if (status != BLE_ERR_OK)
            {
                info_color(LOG_RED, "ble_gatt_read_rsp status: %d\n", status);
            }
            
        }
        break;

        case BLESERVICE_MATTER_CLIENT_RX_BUFFER_INDICATE_CONFIRM_EVENT:
        {
            ChipDeviceEvent event;       
            event.Type                          = DeviceEventType::kCHIPoBLEIndicateConfirm;
            event.CHIPoBLEIndicateConfirm.ConId = p_param->host_id;
            PlatformMgr().PostEvent(&event);                
        }
        break;

        case BLESERVICE_MATTER_CLIENT_RX_BUFFER_CCCD_WRITE_EVENT:
        {
            //ChipLogDetail(DeviceLayer, "get CCCD write event"); 
            bool indicationsEnabled = (*p_param->data);
            if (indicationsEnabled)
            {
                
                {
                    ChipDeviceEvent event;
                    event.Type                    = DeviceEventType::kCHIPoBLESubscribe;
                    event.CHIPoBLESubscribe.ConId = p_param->host_id;
                    err                           = PlatformMgr().PostEvent(&event);
                }
                ChipLogDetail(DeviceLayer, "SubscribePostEvent: %u", err); 
            }
            else
            {
                ChipDeviceEvent event;
                event.Type                    = DeviceEventType::kCHIPoBLEUnsubscribe;
                event.CHIPoBLESubscribe.ConId = p_param->host_id;
                err                           = PlatformMgr().PostEvent(&event);
                ChipLogDetail(DeviceLayer, "Unsubscribe PostEvent: %u", err); 
            }
        }
        break;

        default:
            break;
        }
    }
}
int BLEManagerImpl::server_profile_init(uint8_t host_id)
{
    ble_err_t status;
    ble_info_link0_t *p_profile_info = (ble_info_link0_t*)ble_app_link_info[host_id].profile_info;

    ble_app_link_info[host_id].state = STATE_STANDBY;

    do
    {
        // GAP Related
        // -------------------------------------
        status = ble_svcs_gaps_init(host_id, BLE_GATT_ROLE_SERVER, &(p_profile_info->svcs_info_gaps), NULL);
        if (status != BLE_ERR_OK)
        {
            break;
        }

        // set GAP device name
        status = ble_svcs_gaps_device_name_set((uint8_t *)DEVICE_NAME_STR, sizeof(DEVICE_NAME_STR));
        if (status != BLE_ERR_OK)
        {
            break;
        }

        // GATT Related
        // -------------------------------------
        status = ble_svcs_gatts_init(host_id, BLE_GATT_ROLE_SERVER, &(p_profile_info->svcs_info_gatts), NULL);
        if (status != BLE_ERR_OK)
        {
            break;
        }

        // DIS Related
        // -------------------------------------
        status = ble_svcs_dis_init(host_id, BLE_GATT_ROLE_SERVER, &(p_profile_info->svcs_info_dis), NULL);
        if (status != BLE_ERR_OK)
        {
            break;
        }
        status = ble_svcs_matter_init(host_id, BLE_GATT_ROLE_SERVER, &(p_profile_info->svcs_info_matter), (ble_svcs_evt_matter_handler_t)ble_svcs_matter_evt_handler);

    }while(0);

    return status;
}

int BLEManagerImpl::ble_init(void)
{
    int status;
    ble_unique_code_format_t unique_code_param;
    ble_gap_addr_t device_addr_param;

    status = BLE_ERR_OK;
    do
    {
        status = ble_cmd_phy_controller_init();
        if (status != BLE_ERR_OK)
        {
            break;
        }

        status = ble_cmd_read_unique_code(&unique_code_param);
        if (status == BLE_ERR_OK)
        {
            device_addr_param.addr_type = unique_code_param.addr_type;
            memcpy(&device_addr_param.addr, &unique_code_param.ble_addr, 6);
            status = ble_cmd_device_addr_set((ble_gap_addr_t *)&device_addr_param);
            if (status != BLE_ERR_OK)
            {
                break;
            }
            status = ble_cmd_write_identity_resolving_key((ble_sm_irk_param_t *)&unique_code_param.ble_irk[0]);
            if (status != BLE_ERR_OK)
            {
                break;
            }
        }
        else
        {
            for(int i=0;i<5;i++) DEVICE_ADDR.addr[i] = static_cast<int>(chip::Crypto::GetRandU8());
            status = ble_cmd_device_addr_set((ble_gap_addr_t *)&DEVICE_ADDR);
            if (status != BLE_ERR_OK)
            {
                break;
            }
        }

        status = ble_cmd_resolvable_address_init();
        if (status != BLE_ERR_OK)
        {
            break;
        }

        status = ble_cmd_suggest_data_len_set(BLE_GATT_DATA_LENGTH_MAX);
        if (status != BLE_ERR_OK)
        {
            break;
        }

        status = server_profile_init(0);
        if (status != BLE_ERR_OK)
        {
            break;
        }
    } while (0);

    return status;
}

void BLEManagerImpl::app_evt_handler(void *p_param)
{
    app_req_param_t *p_app_param = (app_req_param_t *)p_param;
    ble_err_t status;
    uint8_t host_id;
    ble_info_link0_t *p_profile_info;

    host_id = p_app_param->host_id;
    p_profile_info = (ble_info_link0_t *)ble_app_link_info[host_id].profile_info;

    switch (p_app_param->app_req)
    {
    case APP_REQUEST_ADV_START:
        do
        {
            // set preferred MTU size and data length
            #if 0
            status = ble_cmd_default_mtu_size_set(host_id, BLE_GATT_ATT_MTU_MAX);
            if (status != BLE_ERR_OK)
            {
                info_color(LOG_RED, "ble_cmd_default_mtu_size_set() status = %d\n", status);
                break;
            }
            #endif
            // enable advertising
            status = (ble_err_t)adv_init();
            if (status != BLE_ERR_OK)
            {
                info_color(LOG_RED, "adv_init() status = %d\n", status);
                break;
            }

            status = (ble_err_t)adv_enable(host_id);
            if (status != BLE_ERR_OK)
            {
                info_color(LOG_RED, "adv_enable() status = %d\n", status);
                break;
            }
        } while (0);

        //g_mtu_size = BLE_GATT_ATT_MTU_MIN;
        break;

    default:
        break;
    }
}

int BLEManagerImpl::adv_init(void)
{
    ble_err_t status;
    ble_adv_param_t adv_param;
    ble_adv_data_param_t adv_data_param;
    ble_adv_data_param_t adv_scan_data_param;
    ble_gap_addr_t addr_param;
    const uint8_t SCANRSP_ADLENGTH = (1) + sizeof(DEVICE_NAME_STR); //  1 byte data type

    // adv data
    uint8_t adv_data[] =
        {
            0x02,
            GAP_AD_TYPE_FLAGS,
            BLE_GAP_FLAGS_LIMITED_DISCOVERABLE_MODE,
        };

    // scan response data
    uint8_t adv_scan_rsp_data[] =
        {
            SCANRSP_ADLENGTH,                // AD length
            GAP_AD_TYPE_LOCAL_NAME_COMPLETE, // AD data type
            DEVICE_NAME,                     // the name is shown on scan list
        };

    ble_cmd_device_addr_get(&addr_param);
    do
    {
        adv_param.adv_type = ADV_TYPE_ADV_IND;
        adv_param.own_addr_type = addr_param.addr_type;
        if (g_use_slow_adv_interval == true)
        {
            adv_param.adv_interval_min = CHIP_DEVICE_CONFIG_BLE_SLOW_ADVERTISING_INTERVAL_MIN;
            adv_param.adv_interval_max = CHIP_DEVICE_CONFIG_BLE_SLOW_ADVERTISING_INTERVAL_MAX;       
        }
        else
        {
            adv_param.adv_interval_min = CHIP_DEVICE_CONFIG_BLE_FAST_ADVERTISING_INTERVAL_MIN;
            adv_param.adv_interval_max = CHIP_DEVICE_CONFIG_BLE_FAST_ADVERTISING_INTERVAL_MAX;              
        }
        adv_param.adv_channel_map = ADV_CHANNEL_ALL;
        adv_param.adv_filter_policy = ADV_FILTER_POLICY_ACCEPT_ALL;

        // set adv parameter
        status = ble_cmd_adv_param_set(&adv_param);
        if (status != BLE_ERR_OK)
        {
            info_color(LOG_RED, "ble_cmd_adv_param_set() status = %d\n", status);
            break;
        }

        // set adv data
        adv_data_param.length = sizeof(adv_data);
        memcpy(&adv_data_param.data, &adv_data, sizeof(adv_data));
        status = ble_cmd_adv_data_set(&adv_data_param);
        if (status != BLE_ERR_OK)
        {
            info_color(LOG_RED, "ble_cmd_adv_data_set() status = %d\n", status);
            break;
        }

        // set scan rsp data
        adv_scan_data_param.length = sizeof(adv_scan_rsp_data);
        memcpy(&adv_scan_data_param.data, &adv_scan_rsp_data, sizeof(adv_scan_rsp_data));
        status = ble_cmd_adv_scan_rsp_set(&adv_scan_data_param);
        if (status != BLE_ERR_OK)
        {
            info_color(LOG_RED, "ble_cmd_adv_scan_rsp_set() status = %d\n", status);
            break;
        }
    } while (0);

    return status;
}

int BLEManagerImpl::adv_enable(uint8_t host_id)
{
    ble_err_t status;

    status = ble_cmd_adv_enable(host_id);
    if (status != BLE_ERR_OK)
    {
        info_color(LOG_RED, "adv_enable() status = %d\n", status);
    }

    return status;
}


CHIP_ERROR BLEManagerImpl::_Init()
{
    CHIP_ERROR err= CHIP_NO_ERROR;

    mServiceMode = ConnectivityManager::kCHIPoBLEServiceMode_Enabled;

    // Initialize the CHIP BleLayer.
    err = BleLayer::Init(this, this, &DeviceLayer::SystemLayer());
    // SuccessOrExit(err);
#if 1
    gt_app_cfg.pf_evt_indication = ble_evt_indication_cb;

    task_dual_init();

    ble_host_stack_init(&gt_app_cfg);

    g_app_msg_q = xQueueCreate(APP_QUEUE_SIZE, sizeof(app_queue_t));
    semaphore_cb = xSemaphoreCreateCounting(BLE_APP_CB_QUEUE_SIZE, BLE_APP_CB_QUEUE_SIZE);
    semaphore_isr = xSemaphoreCreateCounting(APP_ISR_QUEUE_SIZE, APP_ISR_QUEUE_SIZE);
    semaphore_app = xSemaphoreCreateCounting(APP_REQ_QUEUE_SIZE, APP_REQ_QUEUE_SIZE);    

    if (BluetoothEventTaskHandle != NULL)
    {
        return CHIP_ERROR_INCORRECT_STATE;
    }

    xTaskCreate(ble_evt_task, 
        CHIP_DEVICE_CONFIG_THREAD_TASK_NAME,
        CHIP_DEVICE_CONFIG_BLE_APP_TASK_STACK_SIZE / sizeof(StackType_t), 
        this, 
        TASK_PRIORITY_APP,
        &BluetoothEventTaskHandle);

    if (BluetoothEventTaskHandle == NULL)
    {
        return CHIP_ERROR_NO_MEMORY;
    }    
#endif

    // Create FreeRTOS sw timer for BLE timeouts and interval change.
    sbleAdvTimeoutTimer = xTimerCreate("BleAdvTimer",       // Just a text name, not used by the RTOS kernel
                                       1,                   // == default timer period (mS)
                                       false,               // no timer reload (==one-shot)
                                       (void *) this,       // init timer id = ble obj context
                                       BleAdvTimeoutHandler // timer callback handler
    );
    mFlags.Set(Flags::kRTBLEStackInitialized);
    mFlags.Set(Flags::kFastAdvertisingEnabled);
    PlatformMgr().ScheduleWork(DriveBLEState, 0);
exit:
    return err;
}

void BLEManagerImpl::BleAdvTimeoutHandler(TimerHandle_t xTimer)
{
    if (BLEMgrImpl().mFlags.Has(Flags::kFastAdvertisingEnabled))
    {
        ChipLogDetail(DeviceLayer, "bleAdv Timeout : Start slow advertisment");
        
        ChipLogProgress(DeviceLayer, "bleAdv Timeout : Start slow advertisment");
        BLEMgrImpl().StopAdvertising();
        BLEMgrImpl()._SetAdvertisingMode(BLEAdvertisingMode::kSlowAdvertising);
    }
}

uint16_t BLEManagerImpl::_NumConnections(void)
{
    return (ble_app_link_info[g_advertising_host_id].state == STATE_CONNECTED)?1:0;
}

bool BLEManagerImpl::_IsAdvertisingEnabled(void)
{
    return mFlags.Has(Flags::kAdvertisingEnabled);
}

CHIP_ERROR BLEManagerImpl::_SetAdvertisingEnabled(bool val)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    //if (mFlags.Has(Flags::kAdvertisingEnabled) != val)
    {
        mFlags.Set(Flags::kAdvertisingEnabled, val);
        PlatformMgr().ScheduleWork(DriveBLEState, 0);
    }

exit:
    return err;
}

CHIP_ERROR BLEManagerImpl::_SetAdvertisingMode(BLEAdvertisingMode mode)
{
    switch (mode)
    {
    case BLEAdvertisingMode::kFastAdvertising:
        mFlags.Set(Flags::kFastAdvertisingEnabled, true);
        g_use_slow_adv_interval = false;
        break;
    case BLEAdvertisingMode::kSlowAdvertising:
        mFlags.Set(Flags::kFastAdvertisingEnabled, false);
        g_use_slow_adv_interval = true;    
        break;
    default:
        return CHIP_ERROR_INVALID_ARGUMENT;
    }
    mFlags.Set(Flags::kRestartAdvertising);
    PlatformMgr().ScheduleWork(DriveBLEState, 0);
    return CHIP_NO_ERROR;
}

CHIP_ERROR BLEManagerImpl::_GetDeviceName(char * buf, size_t bufSize)
{
    ble_info_link0_t *p_profile_info = (ble_info_link0_t*)ble_app_link_info[0].profile_info;

    if (p_profile_info->svcs_info_gaps.server_info.data.device_name_len >= bufSize)
    {
        return CHIP_ERROR_BUFFER_TOO_SMALL;
    }
    memcpy(buf, p_profile_info->svcs_info_gaps.server_info.data.device_name,
            p_profile_info->svcs_info_gaps.server_info.data.device_name_len);
    return CHIP_NO_ERROR;
}

CHIP_ERROR BLEManagerImpl::_SetDeviceName(const char * deviceName)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    ble_err_t status;

    status = ble_svcs_gaps_device_name_set((uint8_t *)deviceName, sizeof(deviceName));
    if (status != BLE_ERR_OK)
    {
        mFlags.Clear(Flags::kDeviceNameSet);
        info_color(LOG_RED, "device name set() status = %d\n", status);
        err = BLE_ERR_STATE_TRANSLATE(status);
    }
    else
    {
        mFlags.Set(Flags::kDeviceNameSet);
        ChipLogProgress(DeviceLayer, "Setting device name to : \"%s\"", deviceName);
    }    
    return err;
}

CHIP_ERROR BLEManagerImpl::ConfigureAdvertisingData(void)
{
    CHIP_ERROR err;
    CHIP_ERROR chipErr;
    uint16_t discriminator;
    uint16_t advInterval = 0;
    uint32_t mDeviceNameLength = 0;
    uint8_t index = 0;

    uint8_t advPayload[BLE_ADV_DATA_SIZE_MAX] = { 0 };
    uint8_t chipOverBleService[2];
    ChipBLEDeviceIdentificationInfo mDeviceIdInfo = { 0 };
    uint8_t mDeviceIdInfoLength = 0;
    uint8_t chipAdvDataFlags = CHIP_ADV_DATA_TYPE_SERVICE_DATA;


    ble_adv_param_t adv_param;
    ble_gap_addr_t addr_param;
    ble_adv_data_param_t adv_data;
    ble_adv_data_param_t scan_rsp;
    ble_err_t status;
    chipErr = GetCommissionableDataProvider()->GetSetupDiscriminator(discriminator);
    if (chipErr != CHIP_NO_ERROR)
    {
        return chipErr;
    }

    if (!mFlags.Has(Flags::kDeviceNameSet))
    {
        memset(mDeviceName, 0, kMaxDeviceNameLength);
        snprintf(mDeviceName, kMaxDeviceNameLength, "%s%04u", CHIP_DEVICE_CONFIG_BLE_DEVICE_NAME_PREFIX, discriminator);
    }

    mDeviceNameLength = strlen(mDeviceName); // Device Name length + length field

    /**************** Prepare advertising data *******************************************/
    chipErr = ConfigurationMgr().GetBLEDeviceIdentificationInfo(mDeviceIdInfo);
    SuccessOrExit(chipErr);
    mDeviceIdInfoLength = sizeof(mDeviceIdInfo);

    if ((mDeviceIdInfoLength + CHIP_ADV_SHORT_UUID_LEN + 1) > BLE_ADV_DATA_SIZE_MAX)
    {
        return CHIP_ERROR_INCORRECT_STATE;
    }

    adv_data.data[index++] = 0x02;                                                                    // length
    adv_data.data[index++] = CHIP_ADV_DATA_TYPE_FLAGS;                                                // AD type : flags
    adv_data.data[index++] = CHIP_ADV_DATA_FLAGS;                                                     // AD value
    adv_data.data[index++] = static_cast<uint8_t>(mDeviceIdInfoLength + CHIP_ADV_SHORT_UUID_LEN + 1); // AD length
    adv_data.data[index++] = CHIP_ADV_DATA_TYPE_SERVICE_DATA;                                         // AD type : Service Data
    adv_data.data[index++] = ShortUUID_CHIPoBLEService[0];                                            // AD value
    adv_data.data[index++] = ShortUUID_CHIPoBLEService[1];
    memcpy(&adv_data.data[index], (void *) &mDeviceIdInfo, mDeviceIdInfoLength); // AD value
    index += mDeviceIdInfoLength;
    adv_data.length = index;

    ble_cmd_adv_data_set(&adv_data);
    if (status != BLE_ERR_OK)
    {
        ChipLogError(DeviceLayer,"adv_data() status = %d\n", status);
        err = BLE_ERR_STATE_TRANSLATE(status);
    }
    /**************** Prepare scan response data *******************************************/
    index = 0;
    scan_rsp.data[index++] = CHIP_ADV_SHORT_UUID_LEN + 1;  // AD length
    scan_rsp.data[index++] = CHIP_ADV_DATA_TYPE_UUID;      // AD type : uuid
    scan_rsp.data[index++] = ShortUUID_CHIPoBLEService[0]; // AD value
    scan_rsp.data[index++] = ShortUUID_CHIPoBLEService[1];

    scan_rsp.data[index++] = static_cast<uint8_t>(mDeviceNameLength + 1); // length
    scan_rsp.data[index++] = CHIP_ADV_DATA_TYPE_NAME;                     // AD type : name
    memcpy(&scan_rsp.data[index], mDeviceName, mDeviceNameLength);        // AD value
    index += mDeviceNameLength;
    scan_rsp.length = index;

    ble_cmd_adv_scan_rsp_set(&scan_rsp);
    if (status != BLE_ERR_OK)
    {
        ChipLogError(DeviceLayer,"adv_data() status = %d\n", status);
        err = BLE_ERR_STATE_TRANSLATE(status);
    }

    /**************** Prepare advertising parameters *************************************/
 
    ble_cmd_device_addr_get(&addr_param);
    if (mFlags.Has(Flags::kFastAdvertisingEnabled))
    {
        adv_param.adv_interval_min = CHIP_DEVICE_CONFIG_BLE_FAST_ADVERTISING_INTERVAL_MIN;
        adv_param.adv_interval_max = CHIP_DEVICE_CONFIG_BLE_FAST_ADVERTISING_INTERVAL_MAX;    
    }
    else
    {
        adv_param.adv_interval_min = CHIP_DEVICE_CONFIG_BLE_SLOW_ADVERTISING_INTERVAL_MIN;
        adv_param.adv_interval_max = CHIP_DEVICE_CONFIG_BLE_SLOW_ADVERTISING_INTERVAL_MAX;  
    }

    adv_param.adv_type = ADV_TYPE_ADV_IND;
    adv_param.own_addr_type = addr_param.addr_type;

    adv_param.adv_channel_map = ADV_CHANNEL_ALL;
    adv_param.adv_filter_policy = ADV_FILTER_POLICY_ACCEPT_ALL;

    status = ble_cmd_adv_param_set(&adv_param);
    if (status != BLE_ERR_OK)
    {
        ChipLogError(DeviceLayer,"adv_param() status = %d\n", status);
        err = BLE_ERR_STATE_TRANSLATE(status);
    }
#if 1
    if (status == BLE_ERR_OK)
    {
        status = ble_cmd_default_mtu_size_set(0, BLE_GATT_ATT_MTU_MAX);
        if (status != BLE_ERR_OK)
        {
            info_color(LOG_RED, "ble_cmd_default_mtu_size_set() status = %d\n", status);
            err = BLE_ERR_STATE_TRANSLATE(status);
        }
    }
#endif
    if (status == BLE_ERR_OK)
    {
        status = ble_cmd_adv_enable(0);
        if (status != BLE_ERR_OK)
        {
            ChipLogError(DeviceLayer,"adv_param() status = %d\n", status);
            err = BLE_ERR_STATE_TRANSLATE(status);
        }
    }
    return CHIP_NO_ERROR;
exit:
    return chipErr;
}

CHIP_ERROR BLEManagerImpl::StartAdvertising(void)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    mFlags.Set(Flags::kAdvertising);
    mFlags.Clear(Flags::kRestartAdvertising);

    if (mFlags.Has(Flags::kFastAdvertisingEnabled))
    {
        ChipLogProgress(DeviceLayer, "Start Slow Advertisment Timer");
        StartBleAdvTimeoutTimer(CHIP_DEVICE_CONFIG_BLE_ADVERTISING_INTERVAL_CHANGE_TIME);
    }

    err = ConfigureAdvertisingData();
    if (err == CHIP_NO_ERROR)
    /* schedule NFC emulation stop */
    {
        ChipDeviceEvent advChange;
        advChange.Type                             = DeviceEventType::kCHIPoBLEAdvertisingChange;
        advChange.CHIPoBLEAdvertisingChange.Result = kActivity_Started;
        err                                        = PlatformMgr().PostEvent(&advChange);
    }

    return err;
}

CHIP_ERROR BLEManagerImpl::StopAdvertising(void)
{
    ble_err_t err;
    CHIP_ERROR error = CHIP_NO_ERROR;

    if (mFlags.Has(Flags::kAdvertising))
    {
        mFlags.Clear(Flags::kAdvertising);
        mFlags.Clear(Flags::kFastAdvertisingEnabled);
        mFlags.Clear(Flags::kRestartAdvertising);

        err = ble_cmd_adv_disable();
        if (err != BLE_ERR_OK)
        {
            return CHIP_ERROR_INCORRECT_STATE;
        }
        else
        {
            {
                ChipDeviceEvent advChange;
                advChange.Type                             = DeviceEventType::kCHIPoBLEAdvertisingChange;
                advChange.CHIPoBLEAdvertisingChange.Result = kActivity_Stopped;
                error                                      = PlatformMgr().PostEvent(&advChange);
            }
        }
    }
    CancelBleAdvTimeoutTimer();

    return error;
}

void BLEManagerImpl::DriveBLEState(void)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    // Check if BLE stack is initialized
    // VerifyOrExit(mFlags.Has(Flags::kRTBLEStackInitialized), /* */);
    // Start advertising if needed...
    if (mFlags.Has(Flags::kAdvertisingEnabled))
    {
        // Start/re-start advertising if not already started, or if there is a pending change
        // to the advertising configuration.
        if (!mFlags.Has(Flags::kAdvertising) || mFlags.Has(Flags::kRestartAdvertising))
        {
            ChipLogProgress(DeviceLayer, "Start Advertising");
            err = StartAdvertising();
            SuccessOrExit(err);
        }
    }
    // Otherwise, stop advertising if it is enabled.
    else if (mFlags.Has(Flags::kAdvertising))
    {
        err = StopAdvertising();
        SuccessOrExit(err);
        ChipLogProgress(DeviceLayer, "Stopped Advertising");
    }

exit:
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Disabling CHIPoBLE service due to error: %s", ErrorStr(err));
        mServiceMode = ConnectivityManager::kCHIPoBLEServiceMode_Disabled;
    }
}

void BLEManagerImpl::DriveBLEState(intptr_t arg)
{
    sInstance.DriveBLEState();
}

void BLEManagerImpl::_OnPlatformEvent(const ChipDeviceEvent * event)
{
    switch (event->Type)
    {
    case DeviceEventType::kCHIPoBLESubscribe: {
        ChipDeviceEvent connEstEvent;

        //ChipLogProgress(DeviceLayer, "_OnPlatformEvent kCHIPoBLESubscribe");
        HandleSubscribeReceived(event->CHIPoBLESubscribe.ConId, &CHIP_BLE_SVC_ID, &ChipUUID_CHIPoBLEChar_TX);
        connEstEvent.Type = DeviceEventType::kCHIPoBLEConnectionEstablished;
        PlatformMgr().PostEventOrDie(&connEstEvent);
    }
    break;

    case DeviceEventType::kCHIPoBLEUnsubscribe: {
        //ChipLogProgress(DeviceLayer, "_OnPlatformEvent kCHIPoBLEUnsubscribe");
        HandleUnsubscribeReceived(event->CHIPoBLEUnsubscribe.ConId, &CHIP_BLE_SVC_ID, &ChipUUID_CHIPoBLEChar_TX);
    }
    break;

    case DeviceEventType::kCHIPoBLEWriteReceived: {
        //ChipLogProgress(DeviceLayer, "_OnPlatformEvent kCHIPoBLEWriteReceived");
        HandleWriteReceived(event->CHIPoBLEWriteReceived.ConId, &CHIP_BLE_SVC_ID, &ChipUUID_CHIPoBLEChar_RX,
                            PacketBufferHandle::Adopt(event->CHIPoBLEWriteReceived.Data));
    }
    break;

    case DeviceEventType::kCHIPoBLEConnectionError: {
        //ChipLogProgress(DeviceLayer, "_OnPlatformEvent kCHIPoBLEConnectionError");
        HandleConnectionError(event->CHIPoBLEConnectionError.ConId, event->CHIPoBLEConnectionError.Reason);
    }
    break;

    case DeviceEventType::kCHIPoBLEIndicateConfirm: {
        //ChipLogProgress(DeviceLayer, "_OnPlatformEvent kCHIPoBLEIndicateConfirm");
        HandleIndicationConfirmation(event->CHIPoBLEIndicateConfirm.ConId, &CHIP_BLE_SVC_ID, &ChipUUID_CHIPoBLEChar_TX);
    }
    break;

    case DeviceEventType::kCHIPoBLEConnectionClosed:
        mFlags.Set(Flags::kRestartAdvertising);
        //_SetAdvertisingEnabled(true);
        break;

    case DeviceEventType::kThreadStateChange:
        //HandleThreadStateChange(event);
        break;    

    case DeviceEventType::kOperationalNetworkEnabled:
        //HandleOperationalNetworkEnabled(event);
        break;

    default:
        //ChipLogProgress(DeviceLayer, "_OnPlatformEvent default:  event->Type = %d", event->Type);
        break;
    }
}


CHIP_ERROR BLEManagerImpl::HandleOperationalNetworkEnabled(const ChipDeviceEvent * event)
{
    ChipDeviceEvent disconnectEvent;

    ChipLogDetail(DeviceLayer, "HandleOperationalNetworkEnabled");
    ble_cmd_conn_terminate(0);

    return CHIP_NO_ERROR;
}

CHIP_ERROR BLEManagerImpl::HandleThreadStateChange(const ChipDeviceEvent * event)
{
    CHIP_ERROR error = CHIP_NO_ERROR;

    ChipLogDetail(DeviceLayer, "HandleThreadStateChange");

exit:
    return error;
}

bool BLEManagerImpl::SubscribeCharacteristic(BLE_CONNECTION_OBJECT conId, const ChipBleUUID * svcId, const ChipBleUUID * charId)
{
    ChipLogProgress(DeviceLayer, "BLEManagerImpl::SubscribeCharacteristic() not supported");
    return false;
}

bool BLEManagerImpl::UnsubscribeCharacteristic(BLE_CONNECTION_OBJECT conId, const ChipBleUUID * svcId, const ChipBleUUID * charId)
{
    ChipLogProgress(DeviceLayer, "BLEManagerImpl::UnsubscribeCharacteristic() not supported");
    return false;
}

bool BLEManagerImpl::CloseConnection(BLE_CONNECTION_OBJECT conId)
{
    ble_err_t status = BLE_ERR_OK;

    status = ble_cmd_conn_terminate(0);
    ChipLogProgress(DeviceLayer, "Closing BLE GATT connection (con %u)", conId);

    if (status != BLE_ERR_OK)
    {
        ChipLogError(DeviceLayer, "sl_bt_connection_close() failed: %d", status);
    }

    return (status == BLE_ERR_OK);
}

uint16_t BLEManagerImpl::GetMTU(BLE_CONNECTION_OBJECT conId) const
{   
    return g_mtu_size;
}

bool BLEManagerImpl::SendIndication(BLE_CONNECTION_OBJECT conId, const ChipBleUUID * svcId, const ChipBleUUID * charId,
                                    PacketBufferHandle data)
{
    ble_err_t status;
    CHIP_ERROR err;
    ble_gatt_data_param_t param;
    ble_info_link0_t *p_profile_info = (ble_info_link0_t*)ble_app_link_info[0].profile_info;

    param.host_id = 0;
    param.handle_num = p_profile_info->svcs_info_matter.server_info.handles.hdl_client_rx_buffer;


    param.p_data = data->Start();
    param.length  = data->DataLength();
    status = ble_svcs_data_send(TYPE_BLE_GATT_INDICATION, &param);
    

    if (status != BLE_ERR_OK)
    {
        ChipLogError(DeviceLayer, "BLEManagerImpl::SendIndication() failed: %d", status);
        return false;
    }
    return true;
}

bool BLEManagerImpl::SendWriteRequest(BLE_CONNECTION_OBJECT conId, const ChipBleUUID * svcId, const ChipBleUUID * charId,
                                      PacketBufferHandle pBuf)
{
    ChipLogProgress(DeviceLayer, "BLEManagerImpl::SendWriteRequest() not supported");
    return false;
}

bool BLEManagerImpl::SendReadRequest(BLE_CONNECTION_OBJECT conId, const ChipBleUUID * svcId, const ChipBleUUID * charId,
                                     PacketBufferHandle pBuf)
{
    ChipLogProgress(DeviceLayer, "BLEManagerImpl::SendReadRequest() not supported");
    return false;
}

bool BLEManagerImpl::SendReadResponse(BLE_CONNECTION_OBJECT conId, BLE_READ_REQUEST_CONTEXT requestContext,
                                      const ChipBleUUID * svcId, const ChipBleUUID * charId)
{
    ChipLogProgress(DeviceLayer, "BLEManagerImpl::SendReadResponse() not supported");
    return false;
}

void BLEManagerImpl::NotifyChipConnectionClosed(BLE_CONNECTION_OBJECT conId)
{
}

#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
CHIP_ERROR BLEManagerImpl::EncodeAdditionalDataTlv()
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    BitFlags<AdditionalDataFields> additionalDataFields;
    AdditionalDataPayloadGeneratorParams additionalDataPayloadParams;

#if CHIP_ENABLE_ROTATING_DEVICE_ID && defined(CHIP_DEVICE_CONFIG_ROTATING_DEVICE_ID_UNIQUE_ID)
    uint8_t rotatingDeviceIdUniqueId[ConfigurationManager::kRotatingDeviceIDUniqueIDLength] = {};
    MutableByteSpan rotatingDeviceIdUniqueIdSpan(rotatingDeviceIdUniqueId);

    err = DeviceLayer::GetDeviceInstanceInfoProvider()->GetRotatingDeviceIdUniqueId(rotatingDeviceIdUniqueIdSpan);
    SuccessOrExit(err);
    err = ConfigurationMgr().GetLifetimeCounter(additionalDataPayloadParams.rotatingDeviceIdLifetimeCounter);
    SuccessOrExit(err);
    additionalDataPayloadParams.rotatingDeviceIdUniqueId = rotatingDeviceIdUniqueIdSpan;
    additionalDataFields.Set(AdditionalDataFields::RotatingDeviceId);
#endif /* CHIP_ENABLE_ROTATING_DEVICE_ID && defined(CHIP_DEVICE_CONFIG_ROTATING_DEVICE_ID_UNIQUE_ID) */

    err = AdditionalDataPayloadGenerator().generateAdditionalDataPayload(additionalDataPayloadParams, c3AdditionalDataBufferHandle,
                                                                         additionalDataFields);

exit:
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Failed to generate TLV encoded Additional Data (%s)", __func__);
    }

    return err;
}


#endif // CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING

/*******************************************************************************
 * FreeRTOS Task Management Functions
 *******************************************************************************/

void BLEManagerImpl::CancelBleAdvTimeoutTimer(void)
{
    if (xTimerStop(sbleAdvTimeoutTimer, 0) == pdFAIL)
    {
        ChipLogError(DeviceLayer, "Failed to stop BledAdv timeout timer");
    }
}

void BLEManagerImpl::StartBleAdvTimeoutTimer(uint32_t aTimeoutInMs)
{
    if (xTimerIsTimerActive(sbleAdvTimeoutTimer))
    {
       CancelBleAdvTimeoutTimer();
    }

    // timer is not active, change its period to required value (== restart).
    // FreeRTOS- Block for a maximum of 100 ticks if the change period command
    // cannot immediately be sent to the timer command queue.
    if (xTimerChangePeriod(sbleAdvTimeoutTimer, aTimeoutInMs / portTICK_PERIOD_MS, 100) != pdPASS)
    {
       ChipLogError(DeviceLayer, "Failed to start BledAdv timeout timer");
    }
}


} // namespace Internal
} // namespace DeviceLayer
} // namespace chip
#endif // CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
