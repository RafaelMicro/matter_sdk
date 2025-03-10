/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
 *
 * hal_BleFreeRTOS.h
 *   Hardware Abstraction Layer BLE freeRTOS for ARM devices.
 *
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 * CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 * LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * QORVO INC. SHALL NOT, IN ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 * FOR ANY REASON WHATSOEVER.
 *
 *
 */

#ifndef _HAL_BLEFREERTOS_H_
#define _HAL_BLEFREERTOS_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "global.h"

/*****************************************************************************
 *                    Definitions
 *****************************************************************************/
#define GP_BLE_TASK_EVENT_RCI_CONN_REQ  0x01
#define GP_BLE_TASK_EVENT_RCI_CONN_RSP  0x02
#define GP_BLE_TASK_EVENT_IPCGPM        0x03

/*****************************************************************************
 *                    Function Prototype Definitions
 *****************************************************************************/
void hal_BleIsrRciDefer(UInt8 event, UInt8 pbmEntry);
void hal_BleIsrIPCGPMDefer(void);
void hal_BleTaskCreate(void);
void hal_BleTaskDestroy(void);
Bool hal_IsBleTaskCreated(void);

#endif //_HAL_BLEFREERTOS_H_
