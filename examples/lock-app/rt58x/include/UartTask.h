/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    All rights reserved.
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

#pragma once

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
#define UART_BUF_SIZE  256
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

    void SendCommand(uint16_t cmd, uint8_t payload_len, uint8_t * pbuf);

private:
    friend UartTask & GetUartTask(void);
    void UartHandler();
    void UartBufferPreCheck(uint8_t * cache, int len);
    Staus_t UartParser();
    void ProcessCommand(uint16_t cmd, uint8_t len, uint8_t * pbuf);
    uint8_t CRC_Calc(uint8_t * p, uint8_t len);

    int mTotalLen   = 0;
    int mOffset     = 0;
    uint8_t mDataBuf[UART_BUF_SIZE];
    uint8_t mUartInit = 0;

    static UartTask sUartTask;
};

inline UartTask & GetUartTask(void)
{
    return UartTask::sUartTask;
}
