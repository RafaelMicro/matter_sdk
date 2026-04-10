/**********************************************************
 * Includes
 *********************************************************/
#include "AppTask.h"
#include "AppConfig.h"
#include "AppEvent.h"
#include "UartTask.h"
#include <platform/CHIPDeviceLayer.h>

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::DeviceLayer;

#define UART_ACTIVATE_PIN 7

/**********************************************************
 * Global Variables
 *********************************************************/
HOSAL_UART_DEV_DECL(uart1_dev, 1, 28, 29, UART_BAUDRATE_115200)
UartTask UartTask::sUartTask;
static TaskHandle_t  sUartTaskHandle;
static StackType_t   uartStack[1024 / sizeof(StackType_t)];
static StaticTask_t  uartTaskStruct;

static uint8_t uart_buf[UART_BUF_SIZE];

/**********************************************************
 * Function
 *********************************************************/
#if CONFIG_HOSAL_SOC_IDLE_SLEEP
static void gpio_isr_event(uint32_t pin, void * isr_param)
{
    if (pin == UART_ACTIVATE_PIN)
    {
        GetUartTask().UartSignalPendingFromISR();
    }
}
#endif

CHIP_ERROR UartTask::Init()
{
    hosal_uart_init(&uart1_dev);
#if CONFIG_HOSAL_SOC_IDLE_SLEEP
    hosal_gpio_input_config_t pin_cfg;
    /* gpio0 pin setting */
    pin_cfg.param        = NULL;
    pin_cfg.pin_int_mode = GPIO_PIN_INT_EDGE_FALLING;
    pin_cfg.usr_cb       = (void *) gpio_isr_event;

    hosal_gpio_set_debounce_time(DEBOUNCE_SLOWCLOCKS_1024);
    hosal_pin_set_pullopt(UART_ACTIVATE_PIN, HOSAL_PULL_UP_100K);
    hosal_gpio_cfg_input(UART_ACTIVATE_PIN, pin_cfg);
    hosal_gpio_debounce_enable(UART_ACTIVATE_PIN);
    hosal_gpio_int_enable(UART_ACTIVATE_PIN);
#endif

    // Start Uart task.
    sUartTaskHandle = xTaskCreateStatic(UartTaskMain, "UART",
                                        MATTER_ARRAY_SIZE(uartStack), nullptr, 1, uartStack, &uartTaskStruct);

    if (sUartTaskHandle == nullptr)
    {
        return CHIP_ERROR_NO_MEMORY;
    }
    return CHIP_NO_ERROR;
}

void UartTask::UartTaskMain(void * pvParameter)
{
    while (true)
    {
        sUartTask.UartHandler();
#if CONFIG_HOSAL_SOC_IDLE_SLEEP
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
#else
        vTaskDelay(20);
#endif
    }
}

#if CONFIG_HOSAL_SOC_IDLE_SLEEP
void UartTask::UartSignalPending()
{
    if (sUartTaskHandle)
    {
        xTaskNotifyGive(sUartTaskHandle);
    }
}

void UartTask::UartSignalPendingFromISR()
{
    BaseType_t yieldRequired = pdFALSE;

    if (sUartTaskHandle)
    {
        vTaskNotifyGiveFromISR(sUartTaskHandle, &yieldRequired);
    }
    portYIELD_FROM_ISR(yieldRequired);
}
#endif

void UartTask::UartHandler()
{
    int len;
    uint32_t pin_status;
    uint8_t uart_cache[UART_CACHE_SIZE] = { 0 };
    Staus_t staus                       = kStaus_DataInvalid;
#if CONFIG_HOSAL_SOC_IDLE_SLEEP
    if (!mUartInit)
    {
        hosal_uart_init(&uart1_dev);
        mUartInit = 1;
    }
#endif
    do
    {
        len = hosal_uart_receive(&uart1_dev, uart_cache, UART_CACHE_SIZE);
        if (len)
        {
            sUartTask.UartBufferPreCheck(uart_cache, len);
            staus = sUartTask.UartParser();
        }
    } while (0);

#if CONFIG_HOSAL_SOC_IDLE_SLEEP
    hosal_gpio_pin_get(UART_ACTIVATE_PIN, &pin_status);
    if (pin_status == 0)
    {
        sUartTask.UartSignalPending();
    }
    else
    {
        mUartInit = 0;
    }
#endif
}

void UartTask::UartBufferPreCheck(uint8_t * cache, int len)
{
    if (mTotalLen + len > UART_BUF_SIZE)
    {
        if (len > mOffset)
        {
            memset(mDataBuf, 0, UART_BUF_SIZE);
            mTotalLen = 0;
            mOffset   = 0;
        }
        else
        {
            uint8_t temp_buf[UART_BUF_SIZE];
            memcpy(temp_buf, mDataBuf + mOffset, mTotalLen - mOffset);
            memset(mDataBuf, 0, UART_BUF_SIZE);
            memcpy(mDataBuf, temp_buf, mTotalLen - mOffset);
            mTotalLen -= mOffset;
            mOffset = 0;
        }
    }
    memcpy(mDataBuf + mTotalLen, cache, len);
    mTotalLen += len;
}

UartTask::Staus_t UartTask::UartParser()
{
//|------------------------------------------------------|
//|    Header  |   Len   |  Command | Payload | Checksum |
//| FF FC FC FF| 1 byte  |  2 bytes | n bytes |  1 byte  |
//|------------------------------------------------------|
//Len = len(Command)+len(Payload)
    int data_len          = mTotalLen - mOffset;
    int idx               = -1;
    uint8_t header[4]     = { 0xFF, 0xFC, 0xFC, 0xFF };
    if (data_len < 8)
    {
        return kStaus_DataInvalid;
    }
    for (int i = mOffset; i < mTotalLen - 3; i++)
    {
        if (memcmp(&mDataBuf[i], header, 4) == 0)
        {
            idx = i;
            break;
        }
    }
    if (idx < 0)
    {
        return kStaus_DataInvalid;
    }
    if ((mTotalLen - idx) < 8)
    {
        return kStaus_DataInvalid;
    }
    uint8_t * data_buf = &mDataBuf[idx];
    uint8_t len        = data_buf[4];
    uint16_t cmd       = data_buf[5] | (data_buf[6] << 8);
    if ((5 + len + 1) > data_len)
    {
        return kStaus_DataInvalid;
    }
    if (sUartTask.CRC_Calc(data_buf + 4, len + 1) != data_buf[5 + len])
    {
        log_error("crc 0x%x error, should be 0x%x\n", data_buf[5 + len], sUartTask.CRC_Calc(data_buf + 4, len + 1));
        mOffset = idx + len + 6;
        return kStaus_CRCError;
    }
    mOffset = idx + len + 6;
    sUartTask.ProcessCommand(cmd, len - 2, &data_buf[7]);

    return kStaus_DataValid;
}

void UartTask::ProcessCommand(uint16_t cmd, uint8_t len, uint8_t * pbuf)
{
    ChipLogProgress(NotSpecified, "Process cmd %d\n", cmd);
    switch (cmd)
    {
        case 0x0001:
            PlatformMgr().LockChipStack();
            DoorLockServer::Instance().SetLockState(1, DoorLock::DlLockState::kLocked);
            PlatformMgr().UnlockChipStack();
            break;
        case 0x0002:
            PlatformMgr().LockChipStack();
            DoorLockServer::Instance().SetLockState(1, DoorLock::DlLockState::kUnlocked);
            PlatformMgr().UnlockChipStack();
            break;
        default:
            break;
    }
}

void UartTask::SendCommand(uint16_t cmd, uint8_t payload_len, uint8_t * pbuf)
{
//|------------------------------------------------------|
//|    Header  |   Len   |  Command | Payload | Checksum |
//| FF FC FC FF| 1 byte  |  2 bytes | n bytes |  1 byte  |
//|------------------------------------------------------|
//Len = len(Command)+len(Payload)
#if CONFIG_HOSAL_SOC_IDLE_SLEEP
    hosal_uart_init(&uart1_dev);
#endif
    uint8_t len          = payload_len + 2;
    uint16_t total_len   = 5 + len + 1;
    uint8_t temp_buf[UART_BUF_SIZE];
    if (total_len > UART_BUF_SIZE)
    {
        // Message too large for buffer, do not send
        return;
    }
    temp_buf[0] = 0xFF;
    temp_buf[1] = 0xFC;
    temp_buf[2] = 0xFC;
    temp_buf[3] = 0xFF;
    temp_buf[4] = len;
    temp_buf[5] = cmd & 0xFF;
    temp_buf[6] = (cmd >> 8) & 0xFF;
    if (payload_len > 0 && pbuf != NULL)
    {
        memcpy(&temp_buf[7], pbuf, payload_len);
    }
    temp_buf[5 + len] = sUartTask.CRC_Calc(&temp_buf[4], len + 1);
    hosal_uart_send(&uart1_dev, temp_buf, total_len);
    log_hexdump_out("", 16, temp_buf, total_len);
}

uint8_t UartTask::CRC_Calc(uint8_t * buf, uint8_t len)
{
    uint8_t cs = 0;

    for (int i = 0; i < len; i++)
        cs += buf[i];

    return (~cs);
}
