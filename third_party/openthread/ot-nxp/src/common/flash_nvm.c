/*
 *  Copyright (c) 2021-2022, The OpenThread Authors.
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

#include "ot_platform_common.h"
#include <openthread/instance.h>
#include <openthread/platform/flash.h>
#include <openthread/platform/settings.h>

#include <assert.h>

#include "FunctionLib.h"
#include "fsl_os_abstraction.h"

#ifndef SAVE_IN_FLASH_DISABLED
#include "NVM_Interface.h"
#else
#define NVM_RegisterDataSet(...)
#define NvModuleInit(...)
#define NvRestoreDataSet(...)
#define NvSaveOnIdle(...)
#define NvSyncSave(...)
#endif

#ifndef OT_PLAT_SAVE_NVM_DATA_ON_IDLE
#define OT_PLAT_SAVE_NVM_DATA_ON_IDLE 1
#endif

#ifdef DEBUG_NVM
#include "fsl_debug_console.h"
#define DBG_PRINTF PRINTF
#else
#define DBG_PRINTF(...)
#endif

#define TLV_TAG_SIZE sizeof(((structTLV_t *)0)->tag)
#define TLV_LEN_SIZE sizeof(((structTLV_t *)0)->len)
#define TLV_HEADER_SIZE (TLV_TAG_SIZE + TLV_LEN_SIZE)

#define OT_SETTINGS_BUFFER_SIZE 1024

#ifndef NVM_ID_OT_DATA
#define NVM_ID_OT_DATA 0xf102
#endif

typedef struct
{
    uint16_t tag;
    uint16_t len;
    uint8_t *pData;
} structTLV_t;

typedef struct
{
    uint16_t recordLen;
    uint16_t recordFreeLen;
    /* Format: <Tag1, Len1, Value1>, ... <TagN, LenN, ValueN>  */
    uint8_t buffer[OT_SETTINGS_BUFFER_SIZE];
} otSettingsBuffer_t;

static otSettingsBuffer_t otSettingsBuffer;
static bool               isInitialized = false;
static OSA_MUTEX_HANDLE_DEFINE(mFlashRamBufferMutexId);

NVM_RegisterDataSet((void *)&otSettingsBuffer, 1, sizeof(otSettingsBuffer), NVM_ID_OT_DATA, gNVM_MirroredInRam_c);

static void moveData(uint8_t *pSrc, uint8_t *pDst)
{
    uint8_t *pIteratorWritter = NULL;
    uint8_t *pIteratorReader  = NULL;
    assert(pSrc >= otSettingsBuffer.buffer && pSrc < otSettingsBuffer.buffer + OT_SETTINGS_BUFFER_SIZE);
    assert(pDst >= otSettingsBuffer.buffer && pDst < otSettingsBuffer.buffer + OT_SETTINGS_BUFFER_SIZE);

    if (pDst > pSrc)
    {
        /* Add bytes */
        pIteratorReader  = otSettingsBuffer.buffer + otSettingsBuffer.recordLen - 1;
        pIteratorWritter = pIteratorReader + (pDst - pSrc);
        while (pIteratorReader >= pSrc)
        {
            *pIteratorWritter = *pIteratorReader;
            pIteratorReader--;
            pIteratorWritter--;
        }
        otSettingsBuffer.recordLen += (pDst - pSrc);
    }
    else if (pDst < pSrc)
    {
        /* Remove bytes */
        pIteratorReader  = pSrc;
        pIteratorWritter = pDst;
        while (pIteratorReader < (otSettingsBuffer.buffer + otSettingsBuffer.recordLen))
        {
            *pIteratorWritter = *pIteratorReader;
            pIteratorReader++;
            pIteratorWritter++;
        }
        otSettingsBuffer.recordLen -= (pSrc - pDst);
        /* Clean remaining bytes */
        FLib_MemSet((void *)(otSettingsBuffer.buffer + otSettingsBuffer.recordLen), 0, (pSrc - pDst));
    }
    otSettingsBuffer.recordFreeLen = OT_SETTINGS_BUFFER_SIZE - otSettingsBuffer.recordLen;
}

void otPlatSettingsInit(otInstance *aInstance, const uint16_t *aSensitiveKeys, uint16_t aSensitiveKeysLength)
{
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aSensitiveKeys);
    OT_UNUSED_VARIABLE(aSensitiveKeysLength);

    if (!isInitialized)
    {
        isInitialized = true;
        /* Mutex create */
        (void)OSA_MutexCreate(mFlashRamBufferMutexId);
        assert(NULL != mFlashRamBufferMutexId);
        NvModuleInit();
        (void)OSA_MutexLock((osa_mutex_handle_t)mFlashRamBufferMutexId, osaWaitForever_c);
        FLib_MemSet((void *)&otSettingsBuffer, 0, sizeof(otSettingsBuffer));
        otSettingsBuffer.recordFreeLen = OT_SETTINGS_BUFFER_SIZE - otSettingsBuffer.recordLen;
        /* Try to load the ot dataset in RAM */
        NvRestoreDataSet((void *)&otSettingsBuffer, 0);
        (void)OSA_MutexUnlock((osa_mutex_handle_t)mFlashRamBufferMutexId);
    }
}

void otPlatSettingsDeinit(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
}

otError otPlatSettingsGet(otInstance *aInstance, uint16_t aKey, int aIndex, uint8_t *aValue, uint16_t *aValueLength)
{
    OT_UNUSED_VARIABLE(aInstance);
    otError      error           = OT_ERROR_NOT_FOUND;
    uint8_t     *pBufferIterator = otSettingsBuffer.buffer;
    structTLV_t *pTlvIterator    = NULL;
    int          nbKeyFound      = 0;

    (void)OSA_MutexLock((osa_mutex_handle_t)mFlashRamBufferMutexId, osaWaitForever_c);

    /* Loop on the otSeetingsBuffer and try to find a tag corresponding to aKey */
    while (pBufferIterator < otSettingsBuffer.buffer + otSettingsBuffer.recordLen)
    {
        pTlvIterator = (structTLV_t *)pBufferIterator;
        if (pTlvIterator->tag == aKey && nbKeyFound == aIndex)
        {
            if (aValueLength != NULL)
            {
                *aValueLength = pTlvIterator->len;
                if (aValue != NULL && *aValueLength >= pTlvIterator->len)
                {
                    FLib_MemCpy((void *)aValue, pBufferIterator + TLV_HEADER_SIZE, pTlvIterator->len);
                }
            }
            error = OT_ERROR_NONE;
            break;
        }
        else if (pTlvIterator->tag == aKey)
        {
            nbKeyFound++;
        }
        pBufferIterator += TLV_HEADER_SIZE;
        pBufferIterator += pTlvIterator->len;
    }

    (void)OSA_MutexUnlock((osa_mutex_handle_t)mFlashRamBufferMutexId);

    return error;
}

otError otPlatSettingsAdd(otInstance *aInstance, uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength)
{
    OT_UNUSED_VARIABLE(aInstance);
    otError      error           = OT_ERROR_NO_BUFS;
    uint8_t     *pBufferIterator = otSettingsBuffer.buffer;
    structTLV_t *pTlvIterator    = NULL;
    bool         tagFound        = false;
    bool         moveRequired    = false;

    (void)OSA_MutexLock((osa_mutex_handle_t)mFlashRamBufferMutexId, osaWaitForever_c);

    /* Check that we have enough space to store the value */
    if (otSettingsBuffer.recordFreeLen >= TLV_HEADER_SIZE + aValueLength)
    {
        while (pBufferIterator < otSettingsBuffer.buffer + otSettingsBuffer.recordLen)
        {
            pTlvIterator = (structTLV_t *)pBufferIterator;
            if (pTlvIterator->tag == aKey)
            {
                /* An entry with the key already exist, try to find the next diff tag/key and insert before */
                tagFound = true;
            }
            else if (tagFound)
            {
                moveRequired = true;
                break;
            }

            pBufferIterator += TLV_HEADER_SIZE;
            pBufferIterator += pTlvIterator->len;
        }
        if (moveRequired)
        {
            moveData(pBufferIterator, pBufferIterator + TLV_HEADER_SIZE + aValueLength);
        }
        else
        {
            /* Add at the end */
            otSettingsBuffer.recordLen += (TLV_HEADER_SIZE + aValueLength);
            otSettingsBuffer.recordFreeLen = OT_SETTINGS_BUFFER_SIZE - otSettingsBuffer.recordLen;
        }

        FLib_MemCpy(pBufferIterator, &aKey, sizeof(aKey));
        pBufferIterator += sizeof(aKey);
        FLib_MemCpy(pBufferIterator, &aValueLength, sizeof(aValueLength));
        pBufferIterator += sizeof(aValueLength);
        FLib_MemCpy(pBufferIterator, aValue, aValueLength);

#if OT_PLAT_SAVE_NVM_DATA_ON_IDLE
        NvSaveOnIdle(&otSettingsBuffer, false);
#else
        NvSyncSave(&otSettingsBuffer, false);
#endif

        error = OT_ERROR_NONE;
    }

    (void)OSA_MutexUnlock((osa_mutex_handle_t)mFlashRamBufferMutexId);

    return error;
}

otError otPlatSettingsSet(otInstance *aInstance, uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength)
{
    OT_UNUSED_VARIABLE(aInstance);
    otError      error           = OT_ERROR_NO_BUFS;
    uint8_t     *pBufferIterator = otSettingsBuffer.buffer;
    structTLV_t *pTlvIterator    = NULL;
    uint8_t     *ptagFoundOffset = NULL;
    bool         moveRequired    = false;
    uint32_t     bytesToRemove   = 0;

    (void)OSA_MutexLock((osa_mutex_handle_t)mFlashRamBufferMutexId, osaWaitForever_c);

    /* Try to find an entry with a tag = akey */
    while (pBufferIterator < otSettingsBuffer.buffer + otSettingsBuffer.recordLen)
    {
        pTlvIterator = (structTLV_t *)pBufferIterator;
        if (pTlvIterator->tag == aKey)
        {
            /* An entry with the key already exist, try to find the next diff tag/key */
            ptagFoundOffset = pBufferIterator;
        }
        else if (ptagFoundOffset != NULL)
        {
            moveRequired = true;
            break;
        }

        pBufferIterator += TLV_HEADER_SIZE;
        pBufferIterator += pTlvIterator->len;
    }
    if (ptagFoundOffset != NULL)
    {
        bytesToRemove = pBufferIterator - ptagFoundOffset;
    }
    /* Make sure that we have enough space to add the data */
    if ((otSettingsBuffer.recordFreeLen + bytesToRemove) >= TLV_HEADER_SIZE + aValueLength)
    {
        if (moveRequired)
        {
            moveData(pBufferIterator, ptagFoundOffset + TLV_HEADER_SIZE + aValueLength);
        }
        else
        {
            /* Add at the end */
            otSettingsBuffer.recordLen -= bytesToRemove;
            otSettingsBuffer.recordLen += (TLV_HEADER_SIZE + aValueLength);
            otSettingsBuffer.recordFreeLen = OT_SETTINGS_BUFFER_SIZE - otSettingsBuffer.recordLen;
        }
        if (ptagFoundOffset == NULL)
        {
            /* Add at the end */
            ptagFoundOffset = pBufferIterator;
        }
        FLib_MemCpy(ptagFoundOffset, &aKey, sizeof(aKey));
        ptagFoundOffset += sizeof(aKey);
        FLib_MemCpy(ptagFoundOffset, &aValueLength, sizeof(aValueLength));
        ptagFoundOffset += sizeof(aValueLength);
        FLib_MemCpy(ptagFoundOffset, aValue, aValueLength);

#if OT_PLAT_SAVE_NVM_DATA_ON_IDLE
        NvSaveOnIdle(&otSettingsBuffer, false);
#else
        NvSyncSave(&otSettingsBuffer, false);
#endif

        error = OT_ERROR_NONE;
    }

    (void)OSA_MutexUnlock((osa_mutex_handle_t)mFlashRamBufferMutexId);
    return error;
}

otError otPlatSettingsDelete(otInstance *aInstance, uint16_t aKey, int aIndex)
{
    OT_UNUSED_VARIABLE(aInstance);
    otError      error                = OT_ERROR_NOT_FOUND;
    uint8_t     *pBufferIterator      = otSettingsBuffer.buffer;
    structTLV_t *pTlvIterator         = NULL;
    int          nbKeyFound           = 0;
    uint8_t     *pOffsetStartToRemove = NULL;
    uint8_t     *pOffsetEndToRemove   = NULL;

    (void)OSA_MutexLock((osa_mutex_handle_t)mFlashRamBufferMutexId, osaWaitForever_c);

    /* Loop on the otSettingsBuffer and try to find a tag corresponding to aKey */
    while (pBufferIterator < otSettingsBuffer.buffer + otSettingsBuffer.recordLen)
    {
        pTlvIterator = (structTLV_t *)pBufferIterator;
        if (pTlvIterator->tag == aKey)
        {
            if (pOffsetStartToRemove == NULL)
            {
                pOffsetStartToRemove = pBufferIterator;
            }
            if (aIndex == nbKeyFound)
            {
                pOffsetStartToRemove = pBufferIterator;
                pOffsetEndToRemove   = pBufferIterator + TLV_HEADER_SIZE + pTlvIterator->len;
                break;
            }
            nbKeyFound++;
        }
        else if (pOffsetStartToRemove != NULL)
        {
            /* Case where aIndex == -1, we already found one key/tag and now we are looking for the first != tag/key */
            pOffsetEndToRemove = pBufferIterator;
            break;
        }
        pBufferIterator += TLV_HEADER_SIZE;
        pBufferIterator += pTlvIterator->len;
    }

    if (pOffsetStartToRemove != NULL)
    {
        if (pOffsetEndToRemove == NULL || pOffsetEndToRemove >= otSettingsBuffer.buffer + otSettingsBuffer.recordLen)
        {
            /* In case there is no tag/key != from aKey after the first occurence of aKey, only adjust the record size
             * (no move required) */
            pOffsetEndToRemove = otSettingsBuffer.buffer + otSettingsBuffer.recordLen;
            FLib_MemSet((void *)pOffsetStartToRemove, 0, pOffsetEndToRemove - pOffsetStartToRemove);
            otSettingsBuffer.recordLen -= (pOffsetEndToRemove - pOffsetStartToRemove);
            otSettingsBuffer.recordFreeLen = OT_SETTINGS_BUFFER_SIZE - otSettingsBuffer.recordLen;
        }
        else
        {
            moveData(pOffsetEndToRemove, pOffsetStartToRemove);
        }

#if OT_PLAT_SAVE_NVM_DATA_ON_IDLE
        NvSaveOnIdle(&otSettingsBuffer, false);
#else
        NvSyncSave(&otSettingsBuffer, false);
#endif

        error = OT_ERROR_NONE;
    }

    (void)OSA_MutexUnlock((osa_mutex_handle_t)mFlashRamBufferMutexId);

    return error;
}

void otPlatSettingsWipe(otInstance *aInstance)
{
#if OT_PLAT_SAVE_NVM_DATA_ON_IDLE
    int again;
#endif

    OT_UNUSED_VARIABLE(aInstance);
    (void)OSA_MutexLock((osa_mutex_handle_t)mFlashRamBufferMutexId, osaWaitForever_c);
    FLib_MemSet((void *)&otSettingsBuffer, 0, sizeof(otSettingsBuffer));
    otSettingsBuffer.recordFreeLen = OT_SETTINGS_BUFFER_SIZE - otSettingsBuffer.recordLen;
#if OT_PLAT_SAVE_NVM_DATA_ON_IDLE
    /* When saving the NVM data on idle, simply calling the NvSyncSave() here
     * is not a solution because there is a risk for it to overlap with NVM
     * operations already started by the idle task. This will lead to the
     * NvSyncSave() call to occasionally fail its intended purpose.
     *
     * Normally, in this case, we would choose to post a request to flush the
     * NVM data on idle and expect that all pending NVM operations to be
     * executed during the (subsequent) NvShutdown() phase. However, because
     * the NvShutdown() API currently has a problem, we cannot do that. In its
     * stead we are going to use a temporary workaround here and force
     * execution of the entire NVM task queue before continuing. This
     * workaround will be removed when the NvShutdown() function will be fixed. */
    NvSaveOnIdle(&otSettingsBuffer, false);
    do
    {
        again = NvIdle();
    } while (again);
#else
    /* Save it in flash now */
    NvSyncSave(&otSettingsBuffer, false);
#endif
    (void)OSA_MutexUnlock((osa_mutex_handle_t)mFlashRamBufferMutexId);
}

void otPlatSaveSettingsIdle(void)
{
    if (isInitialized)
    {
#if OT_PLAT_SAVE_NVM_DATA_ON_IDLE
        (void)OSA_MutexLock((osa_mutex_handle_t)mFlashRamBufferMutexId, osaWaitForever_c);
        NvIdle();
        (void)OSA_MutexUnlock((osa_mutex_handle_t)mFlashRamBufferMutexId);
#endif
    }
}

#ifdef DEBUG_NVM
void otPlatDumpOtSettings(void)
{
    (void)OSA_MutexLock((osa_mutex_handle_t)mFlashRamBufferMutexId, osaWaitForever_c);
    DBG_PRINTF("otSettingsBuffer.recordLen = %d\n", otSettingsBuffer.recordLen);
    DBG_PRINTF("otSettingsBuffer.recordFreeLen = %d\n", otSettingsBuffer.recordFreeLen);
    DBG_PRINTF("Content = [ ");
    for (int i = 0; i < OT_SETTINGS_BUFFER_SIZE; i++)
    {
        DBG_PRINTF("0x%x ", otSettingsBuffer.buffer[i]);
    }
    DBG_PRINTF("]\n");
    (void)OSA_MutexUnlock((osa_mutex_handle_t)mFlashRamBufferMutexId);
}
#endif
