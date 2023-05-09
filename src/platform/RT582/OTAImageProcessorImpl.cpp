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

#include "OTAImageProcessorImpl.h"
#include <app/clusters/ota-requestor/OTADownloader.h>
#include <app/clusters/ota-requestor/OTARequestorInterface.h>

// extern "C" {
// #include "platform/bootloader/api/btl_interface.h"
// #include "platform/emlib/inc/em_bus.h" // For CORE_CRITICAL_SECTION
// }

// #include "EFR32Config.h"
#include <platform/RT582/RT582Config.h>
#include "cm3_mcu.h"
// #include "util_log.h"
#include "task.h"
#include "fota_define.h"

/// No error, operation OK
#define SL_BOOTLOADER_OK 0L

using namespace ::chip::DeviceLayer::Internal;

uint32_t GetCRC32(uint32_t flash_addr, uint32_t data_len)
{
    uint8_t RemainLen = (data_len & (0x3));
    uint32_t i;
    uint16_t j, k;
    uint32_t ChkSum = ~0;
    uint32_t Len = (data_len >> 2), Read;
    uint32_t *FlashPtr = (uint32_t *)flash_addr;

    for (i = 0; i < Len; i ++)
    {
        //get 32 bits at one time
        Read = FlashPtr[i];
        //get the CRC of 32 bits
        for (j = 0; j < 32; j += 8)
        {
            //get the CRC of 8 bits
            ChkSum ^= ((Read >> j) & 0xFF);
            for (k = 0; k < 8; k ++)
            {
                ChkSum = (ChkSum & 1) ? (ChkSum >> 1) ^ 0xedb88320 : ChkSum >> 1;
            }
        }
    }

    /*if data_len not align 4 bytes*/
    if (RemainLen > 0)
    {
        Read = FlashPtr[i];

        //get the CRC of 32 bits
        for (j = 0; j < (RemainLen << 3); j += 8)
        {
            //get the CRC of 8 bits
            ChkSum ^= ((Read >> j) & 0xFF);
            for (k = 0; k < 8; k ++)
            {
                ChkSum = (ChkSum & 1) ? (ChkSum >> 1) ^ 0xedb88320 : ChkSum >> 1;
            }
        }
    }
    ChkSum = ~ChkSum;
    return ChkSum;
}

namespace chip {

// Define static memebers
uint8_t OTAImageProcessorImpl::mSlotId                                                  = 0;
uint32_t OTAImageProcessorImpl::mPageNo                                                 = 0;
uint32_t OTAImageProcessorImpl::mWriteOffset                                            = 0;
uint16_t OTAImageProcessorImpl::writeBufOffset                                          = 0;
uint8_t OTAImageProcessorImpl::writeBuffer[kAlignmentBytes] __attribute__((aligned(4))) = { 0 };
uint32_t OTAImageProcessorImpl::otaNewVersion                                           = 0;

CHIP_ERROR OTAImageProcessorImpl::PrepareDownload()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandlePrepareDownload, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::Finalize()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandleFinalize, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}
CHIP_ERROR OTAImageProcessorImpl::Apply()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandleApply, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::Abort()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandleAbort, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::ProcessBlock(ByteSpan & block)
{
    if ((block.data() == nullptr) || block.empty())
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }

    // Store block data for HandleProcessBlock to access
    CHIP_ERROR err = SetBlock(block);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(SoftwareUpdate, "Cannot set block data: %" CHIP_ERROR_FORMAT, err.Format());
    }

    DeviceLayer::PlatformMgr().ScheduleWork(HandleProcessBlock, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

bool OTAImageProcessorImpl::IsFirstImageRun()
{
    OTARequestorInterface * requestor = chip::GetRequestorInstance();
    if (requestor == nullptr)
    {
        return false;
    }

    return requestor->GetCurrentUpdateState() == OTARequestorInterface::OTAUpdateStateEnum::kApplying;
}

CHIP_ERROR OTAImageProcessorImpl::ConfirmCurrentImage()
{
    OTARequestorInterface * requestor = chip::GetRequestorInstance();
    if (requestor == nullptr)
    {
        return CHIP_ERROR_INTERNAL;
    }

    uint32_t currentVersion;
    uint32_t targetVersion = requestor->GetTargetVersion();

    ReturnErrorOnFailure(DeviceLayer::ConfigurationMgr().GetSoftwareVersion(currentVersion));
    if (currentVersion != targetVersion)
    {
        ChipLogError(SoftwareUpdate, "Current software version = %" PRIu32 ", expected software version = %" PRIu32, currentVersion,
                     targetVersion);
        return CHIP_ERROR_INCORRECT_STATE;
    }

    return CHIP_NO_ERROR;
}

void OTAImageProcessorImpl::HandlePrepareDownload(intptr_t context)
{
    int32_t err           = SL_BOOTLOADER_OK;
    uint32_t otaBankStart = FOTA_UPDATE_BUFFER_FW_ADDRESS_2MB;
    uint32_t otaBankSize  = SIZE_OF_FOTA_BANK_2MB;
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);

    if (imageProcessor == nullptr)
    {
        ChipLogError(SoftwareUpdate, "ImageProcessor context is null");
        return;
    }
    else if (imageProcessor->mDownloader == nullptr)
    {
        ChipLogError(SoftwareUpdate, "mDownloader is null");
        return;
    }

    // ChipLogProgress(SoftwareUpdate, "HandlePrepareDownload");

    for (uint32_t sector = FOTA_UPDATE_BUFFER_FW_ADDRESS_2MB; sector < otaBankStart + otaBankSize; sector += SIZE_OF_FLASH_SECTOR_ERASE) 
    {
        while (flash_check_busy()) {}
        taskENTER_CRITICAL();
        flash_erase(FLASH_ERASE_SECTOR, sector);
        taskEXIT_CRITICAL();
    }

    // CORE_CRITICAL_SECTION(bootloader_init();)
    mSlotId                                 = 0; // Single slot until we support multiple images
    mPageNo                                 = 0;
    writeBufOffset                          = 0;
    mWriteOffset                            = 0;
    imageProcessor->mParams.downloadedBytes = 0;

    imageProcessor->mHeaderParser.Init();

    // Not calling bootloader_eraseStorageSlot(mSlotId) here because we erase during each write

    imageProcessor->mDownloader->OnPreparedForDownload(err == SL_BOOTLOADER_OK ? CHIP_NO_ERROR : CHIP_ERROR_INTERNAL);
}

void OTAImageProcessorImpl::HandleFinalize(intptr_t context)
{
    uint32_t err          = SL_BOOTLOADER_OK;
    uint8_t *ptr          = NULL;
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);
    if (imageProcessor == nullptr)
    {
        return;
    }

    // Pad the remainder of the write buffer with zeros and write it to bootloader storage
    if (writeBufOffset != 0)
    {
        // Account for last bytes of the image not yet written to storage
        imageProcessor->mParams.downloadedBytes += writeBufOffset;

        while (writeBufOffset != kAlignmentBytes)
        {
            writeBuffer[writeBufOffset] = 0xFF;
            writeBufOffset++;
        }

        while (flash_check_busy());
        taskENTER_CRITICAL();
        flash_write_page((uint32_t)writeBuffer, FOTA_UPDATE_BUFFER_FW_ADDRESS_2MB + mPageNo * kAlignmentBytes);
        // info("===> write last page: %d, address: %08x\r\n", mPageNo, FOTA_UPDATE_BUFFER_FW_ADDRESS_2MB + mPageNo * kAlignmentBytes);
        taskEXIT_CRITICAL();

        // CORE_CRITICAL_SECTION(err = bootloader_eraseWriteStorage(mPageNo, mWriteOffset, writeBuffer, kAlignmentBytes);)
        // if (err)
        // {
        //     ChipLogError(SoftwareUpdate, "ERROR: In HandleFinalize bootloader_eraseWriteStorage() error %ld", err);
        //     imageProcessor->mDownloader->EndDownload(CHIP_ERROR_WRITE_FAILED);
        //     return;
        // }
    }

    imageProcessor->ReleaseBlock();

    ChipLogProgress(SoftwareUpdate, "OTA image downloaded successfully");
}

void OTAImageProcessorImpl::HandleApply(intptr_t context)
{
    uint32_t err = SL_BOOTLOADER_OK;
    fota_information_t t_bootloader_ota_info = {0};
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);

    if (imageProcessor == nullptr)
    {
        return;
    }

    // Force KVS to store pending keys such as data from StoreCurrentUpdateInfo()
    // chip::DeviceLayer::PersistedStorage::KeyValueStoreMgrImpl().ForceKeyMapSave();
    chip::DeviceLayer::ConfigurationMgr().StoreSoftwareVersion(otaNewVersion);

    // CORE_CRITICAL_SECTION(err = bootloader_verifyImage(mSlotId, NULL);)
    if (err != SL_BOOTLOADER_OK)
    {
        ChipLogError(SoftwareUpdate, "ERROR: bootloader_verifyImage() error %ld", err);

        return;
    }

    // CORE_CRITICAL_SECTION(err = bootloader_setImageToBootload(mSlotId);)
    if (err != SL_BOOTLOADER_OK)
    {
        ChipLogError(SoftwareUpdate, "ERROR: bootloader_setImageToBootload() error %ld", err);

        return;
    }

    memcpy(&t_bootloader_ota_info, (uint8_t *)FOTA_UPDATE_BANK_INFO_ADDRESS, sizeof(t_bootloader_ota_info));

    t_bootloader_ota_info.fotabank_ready = FOTA_IMAGE_READY;
    t_bootloader_ota_info.fotabank_startaddr = FOTA_UPDATE_BUFFER_FW_ADDRESS_2MB;
    t_bootloader_ota_info.fota_image_info = FOTA_IMAGE_INFO_COMPRESSED;
    t_bootloader_ota_info.signature_len = 0;
    t_bootloader_ota_info.target_startaddr = APP_START_ADDRESS;
    t_bootloader_ota_info.fotabank_datalen = imageProcessor->mParams.downloadedBytes;
    t_bootloader_ota_info.reserved[0] = 0x1234;
    t_bootloader_ota_info.fota_result = 0xFF;

    t_bootloader_ota_info.fotabank_crc = GetCRC32(t_bootloader_ota_info.fotabank_startaddr, t_bootloader_ota_info.fotabank_datalen);

    while (flash_check_busy());
    taskENTER_CRITICAL();
    flash_erase(FLASH_ERASE_SECTOR, FOTA_UPDATE_BANK_INFO_ADDRESS);
    taskEXIT_CRITICAL();

    while (flash_check_busy());
    taskENTER_CRITICAL();
    flash_write_page((uint32_t)&t_bootloader_ota_info, FOTA_UPDATE_BANK_INFO_ADDRESS);
    taskEXIT_CRITICAL();

    // This reboots the device
    ChipLogProgress(SoftwareUpdate, "system restarting...");
    chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Milliseconds32(300), HandleRestart, nullptr);
}

void OTAImageProcessorImpl::HandleRestart(chip::System::Layer * systemLayer, void * appState)
{
    Sys_Software_Reset();
}

void OTAImageProcessorImpl::HandleAbort(intptr_t context)
{
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);
    if (imageProcessor == nullptr)
    {
        return;
    }

    // Not clearing the image storage area as it is done during each write
    imageProcessor->ReleaseBlock();
}

void OTAImageProcessorImpl::HandleProcessBlock(intptr_t context)
{
    uint32_t err          = SL_BOOTLOADER_OK;
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);

    if (imageProcessor == nullptr)
    {
        ChipLogError(SoftwareUpdate, "ImageProcessor context is null");
        return;
    }
    else if (imageProcessor->mDownloader == nullptr)
    {
        ChipLogError(SoftwareUpdate, "mDownloader is null");
        return;
    }

    ByteSpan block        = imageProcessor->mBlock;
    CHIP_ERROR chip_error = imageProcessor->ProcessHeader(block);

    if (chip_error != CHIP_NO_ERROR)
    {
        ChipLogError(SoftwareUpdate, "Matter image header parser error %s", chip::ErrorStr(chip_error));
        imageProcessor->mDownloader->EndDownload(CHIP_ERROR_INVALID_FILE_IDENTIFIER);
        return;
    }

    // Copy data into the word-aligned writeBuffer, once it fills write its contents to the bootloader storage
    // Final data block is handled in HandleFinalize().
    uint32_t blockReadOffset = 0;
    
    // info("===> fota block len: %d\r\n", block.size());
    while (blockReadOffset < block.size())
    {
        writeBuffer[writeBufOffset] = *((block.data()) + blockReadOffset);
        writeBufOffset++;
        blockReadOffset++;
        if (writeBufOffset == kAlignmentBytes)
        {
            writeBufOffset = 0;
            while (flash_check_busy()) {}
            taskENTER_CRITICAL();
            flash_write_page((uint32_t)writeBuffer, FOTA_UPDATE_BUFFER_FW_ADDRESS_2MB + mPageNo * kAlignmentBytes);
            // info("===> write page: %d, address: %08x\r\n", mPageNo, FOTA_UPDATE_BUFFER_FW_ADDRESS_2MB + mPageNo * kAlignmentBytes);
            taskEXIT_CRITICAL();
            mPageNo++;

            // CORE_CRITICAL_SECTION(err = bootloader_eraseWriteStorage(mSlotId, mWriteOffset, writeBuffer, kAlignmentBytes);)
            // if (err)
            // {
            //     ChipLogError(SoftwareUpdate, "ERROR: In HandleProcessBlock bootloader_eraseWriteStorage() error %ld", err);
            //     imageProcessor->mDownloader->EndDownload(CHIP_ERROR_WRITE_FAILED);
            //     return;
            // }
            mWriteOffset += kAlignmentBytes;
            imageProcessor->mParams.downloadedBytes += kAlignmentBytes;
        }
    }

    imageProcessor->mDownloader->FetchNextData();
}

CHIP_ERROR OTAImageProcessorImpl::ProcessHeader(ByteSpan & block)
{
    if (mHeaderParser.IsInitialized())
    {
        OTAImageHeader header;
        CHIP_ERROR error = mHeaderParser.AccumulateAndDecode(block, header);

        // Needs more data to decode the header
        ReturnErrorCodeIf(error == CHIP_ERROR_BUFFER_TOO_SMALL, CHIP_NO_ERROR);
        ReturnErrorOnFailure(error);

        // SL TODO -- store version somewhere
        // ChipLogProgress(SoftwareUpdate, "Image Header software version: %ld payload size: %lu", header.mSoftwareVersion,
        //                 (long unsigned int) header.mPayloadSize);
        chip::OTAImageProcessorImpl::otaNewVersion = header.mSoftwareVersion;
        // ChipLogProgress(SoftwareUpdate, "software version: %ld", chip::OTAImageProcessorImpl::otaNewVersion);
        mParams.totalFileBytes = header.mPayloadSize;
        mHeaderParser.Clear();
    }
    return CHIP_NO_ERROR;
}

// Store block data for HandleProcessBlock to access
CHIP_ERROR OTAImageProcessorImpl::SetBlock(ByteSpan & block)
{
    if ((block.data() == nullptr) || block.empty())
    {
        return CHIP_NO_ERROR;
    }

    // Allocate memory for block data if we don't have enough already
    if (mBlock.size() < block.size())
    {
        ReleaseBlock();

        mBlock = MutableByteSpan(static_cast<uint8_t *>(chip::Platform::MemoryAlloc(block.size())), block.size());
        if (mBlock.data() == nullptr)
        {
            return CHIP_ERROR_NO_MEMORY;
        }
    }

    // Store the actual block data
    CHIP_ERROR err = CopySpanToMutableSpan(block, mBlock);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(SoftwareUpdate, "Cannot copy block data: %" CHIP_ERROR_FORMAT, err.Format());
        return err;
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::ReleaseBlock()
{
    if (mBlock.data() != nullptr)
    {
        chip::Platform::MemoryFree(mBlock.data());
    }
    mPageNo = 0;
    mBlock = MutableByteSpan();
    return CHIP_NO_ERROR;
}

} // namespace chip
