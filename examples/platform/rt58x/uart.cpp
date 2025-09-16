/*
*
*    Copyright (c) 2021 Project CHIP Authors
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

#include "AppConfig.h"
#include "matter_shell.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "uart.h"
#include <stddef.h>
#include <stdio.h>

#include "hosal_uart.h"
#include "uart_stdio.h"

#include "log.h"

using namespace chip;
//=============================================================================
//                Private Definitions of const value
//=============================================================================

HOSAL_UART_DEV_DECL(uart0_dev,
    0,   /* Port */
    17,  /* Tx Pin*/
    16,  /* Rx Pin*/
    UART_BAUDRATE_115200
)
//=============================================================================
//                Functions
//=============================================================================
void uartConsoleInit(void)
{
    /*Init UART In the first place*/
    hosal_uart_init(&uart0_dev);
}

int16_t uartConsoleWrite(const char *Buf, uint16_t BufLength)
{
    hosal_uart_send(&uart0_dev, Buf, BufLength);
    return (int16_t)BufLength;
}

int16_t uartConsoleRead(char *Buf, uint16_t BufLength)
{
    int16_t cnt = hosal_uart_receive(&uart0_dev, Buf, BufLength);
    return cnt;
}
#ifdef __cplusplus
}
#endif
