/*
 *  Copyright (c) 2017, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file implements the OpenThread platform abstraction for UART communication.
 *
 */

#include "fsl_device_registers.h"
#include <stddef.h>
#include <stdint.h>

#include "platform-rw610n.h"
#include <utils/code_utils.h>
#include <utils/uart.h>

#include "board.h"
#include "fsl_apuart.h"
#include "platform_ot.h"

static SERIAL_MANAGER_HANDLE_DEFINE(otCliSerialHandle);
static SERIAL_MANAGER_WRITE_HANDLE_DEFINE(otCliSerialWriteHandle);
static SERIAL_MANAGER_READ_HANDLE_DEFINE(otCliSerialReadHandle);
static void Uart_RxCallBack(void *pData, serial_manager_callback_message_t *message, serial_manager_status_t status);
static void Uart_TxCallBack(void *pBuffer, serial_manager_callback_message_t *message, serial_manager_status_t status);

bool_t sendDone = FALSE;

uint8_t rxBuffer[kReceiveBufferSize];

otError otPlatUartEnable(void)
{
    PLATFORM_InitOtConsole(otCliSerialHandle);
    SerialManager_OpenWriteHandle((serial_handle_t)otCliSerialHandle, (serial_write_handle_t)otCliSerialWriteHandle);
    SerialManager_OpenReadHandle((serial_handle_t)otCliSerialHandle, (serial_read_handle_t)otCliSerialReadHandle);
    SerialManager_InstallRxCallback((serial_read_handle_t)otCliSerialReadHandle, Uart_RxCallBack, NULL);
    SerialManager_InstallTxCallback((serial_write_handle_t)otCliSerialWriteHandle, Uart_TxCallBack, NULL);

    return OT_ERROR_NONE;
}

otError otPlatUartDisable(void)
{
    return OT_ERROR_NONE;
}

otError otPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength)
{
    SerialManager_WriteBlocking((serial_write_handle_t)otCliSerialWriteHandle, (uint8_t *)aBuf, aBufLength);
    sendDone = TRUE;

    return OT_ERROR_NONE;
}

void otPlatUartProcess(otInstance *aInstance)
{
    if (sendDone)
    {
        otPlatUartSendDone();
        sendDone = FALSE;
    }
}

otError otPlatUartFlush(void)
{
    return OT_ERROR_NOT_IMPLEMENTED;
}

static void Uart_RxCallBack(void *pData, serial_manager_callback_message_t *message, serial_manager_status_t status)
{
    uint32_t bytesRead = 0U;
    if (SerialManager_TryRead((serial_read_handle_t)otCliSerialReadHandle, rxBuffer, kReceiveBufferSize, &bytesRead) ==
        kStatus_SerialManager_Success)
    {
        uint32_t data = 0U;
        while ((data = APUART_ReadSingleByte()) != apUART_FIFO_EMPTY)
        {
            rxBuffer[bytesRead++] = (uint8_t)data;
        }
        otPlatUartReceived(rxBuffer, bytesRead);
    }
}

static void Uart_TxCallBack(void *pBuffer, serial_manager_callback_message_t *message, serial_manager_status_t status)
{
    otPlatUartSendDone();
}
