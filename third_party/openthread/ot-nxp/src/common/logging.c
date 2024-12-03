/*
 *  Copyright (c) 2021, The OpenThread Authors.
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
 * @file logging.c
 * Platform abstraction for the logging
 *
 */

#include <openthread-core-config.h>
#include <openthread/config.h>
#include <openthread/platform/logging.h>
#include <openthread/platform/toolchain.h>

#include "fsl_debug_console.h"
#include "stdio.h"
#include "string.h"

#ifdef MULTICORE_LOGGING_ENABLED
#include "multicore.h"
#endif

#ifdef LOG_32K_TICK_TIMESTAMP_ENABLED
#include "board.h"
#include "fsl_gpt.h"
#endif

#define LOG_BUFFER_SIZE 256
#ifndef LOG_RING_BUFFER_SIZE
#define LOG_RING_BUFFER_SIZE (1024 * 50)
#endif
#define EOL_CHARS "\r\n\0" /* End of Line Characters */
#define EOL_CHARS_LEN 3    /* Length of EOL */
#define LOG_DUMP_TASK_SIZE ((configSTACK_DEPTH_TYPE)2048 / sizeof(portSTACK_TYPE))

static char sTxBuffer[LOG_BUFFER_SIZE];

#ifdef LOG_BUFFER_ENABLED
typedef struct
{
    uint8_t  buffer[LOG_RING_BUFFER_SIZE]; /* format: <2_bytes_log_size1><log1> ... <2_bytes_log_sizeN><logN> */
    uint32_t in;
    uint32_t out;
    uint32_t space_left;
} logRingBuffer_t;

static logRingBuffer_t   logRingBuffer;
static TaskHandle_t      dump_task_handle = NULL;
static SemaphoreHandle_t mutexHandle      = NULL;

static void RingInit(logRingBuffer_t *ring)
{
    ring->in         = 0;
    ring->out        = 0;
    ring->space_left = LOG_RING_BUFFER_SIZE;
    mutexHandle      = xSemaphoreCreateMutex();
}

/*
 * Consume a log in the ring buffer
 * Return the length of the log buffer consumed in the ring buffer.
 */
static uint16_t RingLogConsume(logRingBuffer_t *ring, uint8_t *pBufferOut, uint16_t bufferOutSize)
{
    uint32_t nbByteToRead = 0;

    xSemaphoreTake(mutexHandle, portMAX_DELAY);
    do
    {
        nbByteToRead = LOG_RING_BUFFER_SIZE - ring->space_left;
        if (nbByteToRead == 0)
            break;
        /* Extract/read the size of the log message */
        nbByteToRead = ring->buffer[ring->out] << 8;
        nbByteToRead += ring->buffer[(ring->out + 1) % LOG_RING_BUFFER_SIZE];
        /* Check that we have enough space to store the whole log, otherwise abort */
        if (nbByteToRead > bufferOutSize)
        {
            nbByteToRead = 0;
            break;
        }
        /* Jump to the log content and skip the log length */
        ring->out = (ring->out + sizeof(uint16_t)) % LOG_RING_BUFFER_SIZE;
        for (int i = 0; i < nbByteToRead; i++)
        {
            pBufferOut[i] = ring->buffer[ring->out];
            ring->out++;
            if (ring->out == LOG_RING_BUFFER_SIZE)
                ring->out = 0;
            ring->space_left++;
        }
    } while (0);
    xSemaphoreGive(mutexHandle);
    return nbByteToRead;
}

/*
 * Add a log buffer in the ring buffer.
 * Return true if the whole log message has been added otherwise false
 */
static bool RingLogAdd(logRingBuffer_t *ring, const uint8_t *pData, uint16_t size)
{
    bool logAdded = false;

    xSemaphoreTake(mutexHandle, portMAX_DELAY);
    /* Check that we have enough space to store the log and its size */
    if (ring->space_left >= size + sizeof(uint16_t))
    {
        ring->buffer[ring->in]                              = size >> 8;
        ring->buffer[(ring->in + 1) % LOG_RING_BUFFER_SIZE] = (uint8_t)size;
        ring->in                                            = (ring->in + sizeof(uint16_t)) % LOG_RING_BUFFER_SIZE;
        for (int i = 0; i < size; i++)
        {
            ring->buffer[ring->in++] = pData[i];
            if (ring->in == LOG_RING_BUFFER_SIZE)
                ring->in = 0;
            ring->space_left--;
        }
        logAdded = true;
    }
    xSemaphoreGive(mutexHandle);
    return logAdded;
}

static void otPlatLogDumpTask(void *param)
{
    uint8_t  logBuffer[LOG_BUFFER_SIZE];
    uint16_t logBufferSize = 0;

    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        do
        {
            /* try to read the size to consume */
            logBufferSize = RingLogConsume(&logRingBuffer, logBuffer, sizeof(logBuffer));
            if (logBufferSize == 0)
                break;
#ifdef MULTICORE_LOGGING_ENABLED
            multicore_send_data(logBuffer, logBufferSize);
#else
            PRINTF("%s", logBuffer);
#endif
        } while (1);
    }
}
#endif

static void otPlatLogImpl(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, va_list ap)
{
    OT_UNUSED_VARIABLE(aLogLevel);
    OT_UNUSED_VARIABLE(aLogRegion);
    int len     = 0;
    int maxSize = LOG_BUFFER_SIZE - EOL_CHARS_LEN;

#ifdef LOG_32K_TICK_TIMESTAMP_ENABLED
    uint32_t curTimeStampS = GPT_GetCurrentTimerCount(GPT2);
    len                    = snprintf(sTxBuffer, maxSize, "[%lu]", curTimeStampS);
    maxSize -= len;
#endif

    len += vsnprintf(sTxBuffer + len, maxSize, aFormat, ap);
    if (len >= 0)
    {
        memcpy(sTxBuffer + len, EOL_CHARS, EOL_CHARS_LEN);
        len += EOL_CHARS_LEN;
#ifdef LOG_BUFFER_ENABLED
        if (RingLogAdd(&logRingBuffer, (uint8_t *)sTxBuffer, len))
        {
            xTaskNotifyGive(dump_task_handle);
        }
#else
#ifdef MULTICORE_LOGGING_ENABLED
        multicore_send_data((uint8_t *)sTxBuffer, len);
#else
        PRINTF("%s", sTxBuffer);
#endif
#endif
    }
}

#if (OPENTHREAD_CONFIG_LOG_OUTPUT == OPENTHREAD_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED)
OT_TOOL_WEAK void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, ...)
{
    va_list ap;
    va_start(ap, aFormat);
    otPlatLogImpl(aLogLevel, aLogRegion, aFormat, ap);
    va_end(ap);
}
#endif

void otPlatLogInit(void)
{
#if (OPENTHREAD_CONFIG_LOG_OUTPUT == OPENTHREAD_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED)
#ifdef LOG_32K_TICK_TIMESTAMP_ENABLED
    gpt_config_t gptConfig;
    GPT_GetDefaultConfig(&gptConfig);
    gptConfig.clockSource = kGPT_ClockSource_LowFreq;
    GPT_Init(GPT2, &gptConfig);
    GPT_StartTimer(GPT2);
#endif
#ifdef LOG_BUFFER_ENABLED
    RingInit(&logRingBuffer);
    xTaskCreate(otPlatLogDumpTask, "DUMP_TASK", LOG_DUMP_TASK_SIZE, NULL, tskIDLE_PRIORITY + 1U, &dump_task_handle);
#endif
#endif
}
