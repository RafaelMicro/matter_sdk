/**********************************************************
 * Includes
 *********************************************************/

#include <stdbool.h>
#include <stdint.h>

#include "AppEvent.h"
#include "AppTask.h"
#include "FreeRTOS.h"

#include "BoltLockManager.h"
#include <lib/core/CHIPError.h>
#include <platform/CHIPDeviceLayer.h>

#include "hosal_uart.h"
#include "hosal_gpio.h"
#include "log.h"
#include "mcu.h"

/**********************************************************
 * Defines
 *********************************************************/
#define UART_BUF_SIZE 256
#define UART_CACHE_SIZE 16

/**********************************************************
 * UartManager Declaration
 *********************************************************/

class UartTask
{
public:
    CHIP_ERROR Init();

    enum Staus_t
    {
        kStaus_DataValid = 0,
        kStaus_DataInvalid,
        kStaus_CRCError,
    } Staus;

    static void UartTaskMain(void * pvParameter);
    void UartSignalPending();
    void UartSignalPendingFromISR();

    void SendCommand(uint16_t cmd,uint8_t payload_len,uint8_t *pbuf);

private:
    friend UartTask & GetUartTask(void);
    void UartHandler();
    void UartBufferPreCheck(uint8_t *cache,int len);
    Staus_t UartParser();
    void ProcessCommand(uint16_t cmd,uint8_t len,uint8_t *pbuf);
    uint8_t CRC_Calc(uint8_t *p, uint8_t len);

    int mTotalLen = 0;
    int mOffset = 0;
    uint8_t mDataBuf[UART_BUF_SIZE];

    static UartTask sUartTask;
};
inline UartTask & GetUartTask(void)
{
    return UartTask::sUartTask;
}