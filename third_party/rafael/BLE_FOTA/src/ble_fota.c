/*
 * Copyright (c) 2022-2025 Rafael Microelectronics Inc. All rights reserved.
 * 
 * SPDX-License-Identifier: LicenseRef-RafaelMicro-Proprietary-1.0
 *
 */

/** @file fota.c
 *
 * @brief
 *
 */

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "status.h"
#include "fota_define.h"
#include "ble_fota.h"
#include "ble_profile.h"
#include "hosal_flash.h"
#include "hosal_status.h"

/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/
#define MULTIPLE_OF_32K(Add) ((((Add) & (0x8000-1)) == 0)?1:0)
#define MULTIPLE_OF_64K(Add) ((((Add) & (0x10000-1)) == 0)?1:0)

#define BLE_FOTA_IMAGE_COMPRESS_SUPPORT  (0x01)

#ifndef BLE_VERSION
#define BLE_VERSION "sys ver 0001"
#endif
/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
const sys_information_t systeminfo =
{
    .prefix = "VerGet",          /* Specific string for FOTA tool to search system information, in the code base should not appear second string like this */
    .sysinfo = BLE_VERSION,      /* An example of current system information, able to modify by user */
    .feature_list = BLE_FOTA_IMAGE_COMPRESS_SUPPORT,
};

typedef struct
{
    uint32_t buffered_data_len;
    uint32_t programed_addr;
    uint32_t expectaddr;
    uint32_t currlen;
    uint32_t notify_interval;
    uint32_t last_notify_addr;
} fota_data_params_t;

typedef struct
{
    fota_timer_t timer_type;
    uint32_t expiry_time;
    uint32_t curr_time;
} fota_timer_params_t;

/******************************************************************************
 * Local Variables
 ******************************************************************************/
fota_information_t  *p_fota_info              = (fota_information_t *)(FOTA_UPDATE_BANK_INFO_ADDRESS);

fota_state_t        fota_upgrade_state        = OTA_STATE_IDLE;

fota_data_params_t  fota_data_param           = {.buffered_data_len = 0,
                                                 .programed_addr = 0,
                                                 .expectaddr = 0,
                                                 .currlen = 0,
                                                 .notify_interval = 0xFFFFFFFF,
                                                 .last_notify_addr = 0
                                                };

fota_timer_params_t fota_tmr_param            = {.timer_type = 0,
                                                 .expiry_time = 0,
                                                 .curr_time = 0
                                                };

/******************************************************************************
 * Private Functions
 ******************************************************************************/
/** This function is used to flash program for .
 *
 * @param[in] addr : Address of the flash location to be programmed.
 * @param[in] data : The data to be programmed.
 *
 */
static void fota_flash_program(uint32_t flash_addr, uint8_t *p_data)
{
    hosal_flash_write(HOSAL_FLASH_WRITE_BYTE, flash_addr, p_data);

    while (hosal_flash_ioctrl(HOSAL_FLASH_BUSY, NULL));
}

/** This function is used to flash erase for fota.
 *
 * @param[in] addr : Address of the flash page to be erased.
 *
 */
static uint8_t fota_flash_erase(uint32_t flash_addr)
{
    uint8_t status;

    status = hosal_flash_erase(HOSAL_FLASH_ERASE_SECTOR, flash_addr);
    if (status == HOSAL_STATUS_SUCCESS)
    {
        while (hosal_flash_ioctrl(HOSAL_FLASH_BUSY, NULL));
    }

    return status;
}

static void fota_program(uint32_t flash_addr, uint32_t data, uint32_t data_len)
{
    uint8_t program_idx, data_idx = 0;
    uint8_t *p_data;

    p_data = (uint8_t *)&data;
    for (program_idx = 0; program_idx < data_len; program_idx += FLASH_PROGRAM_SIZE)
    {
        fota_flash_program((flash_addr + data_idx), p_data + data_idx);
        data_idx = data_idx + FLASH_PROGRAM_SIZE;
    }
}

static void ble_fota_system_reboot(void)
{
    sys_software_reset();
}

static uint32_t ble_fota_crc32checksum(uint32_t flash_addr, uint32_t data_len)
{
    uint16_t k;
    uint8_t buff;
    uint32_t i, chk_sum = ~0, len = data_len;

    for (i = 0; i < len; i ++ )
    {
        hosal_flash_read(HOSAL_FLASH_READ_BYTE, flash_addr + i, &buff);
        chk_sum ^= buff;
        for (k = 0; k < 8; k ++)
        {
            chk_sum = chk_sum & 1 ? (chk_sum >> 1) ^ 0xedb88320 : chk_sum >> 1;
        }
    }
    return ~chk_sum;
}

/** FOTA timer start to countdown.
 *
 * @note       The purpose for each fota timer were listed as below: \n
 *             OTA_TIMER_OTA_DATA : Send notify when the FOTA data was not send to device in time. \n
 *             OTA_TIMER_OTA_COMPLETE : System reboot directly if disconnection event is missing. \n
 *             OTA_TIMER_OTA_ERASING : Terminated the connection to erase legacy bank FW and information. \n
 *             OTA_TIMER_OTA_DISCONNECT : After valid FW received, terminated the connection and trigger system reboot when disconnection event received. \n
 *
 * @param[in] Timeout : timer timeout value (unit:sec).
 * @param[in] Type : timer type.
 *
 * @return none
 */
static void ble_fota_timerstart(uint8_t timout, fota_timer_t type)
{
    fota_tmr_param.expiry_time = timout + fota_tmr_param.curr_time;
    fota_tmr_param.timer_type = type;
}

/** The actions after FOTA timer expiry.
 *
 * @param[in] host_id : thid links's host id.
 * @param[in] Type : expired timer type.
 *
 * @return none
 */
static void ble_fota_timerexpiry(uint8_t host_id, fota_timer_t type)
{
    switch (type)
    {
    case OTA_TIMER_OTA_COMPLETE:
    {
        ble_fota_system_reboot();
    }
    break;

    case OTA_TIMER_OTA_ERASING:
    {
        ble_cmd_conn_terminate(host_id);
    }
    break;

    case OTA_TIMER_OTA_DISCONNECT:
    {
        ble_fota_timerstart(3, OTA_TIMER_OTA_COMPLETE);
        ble_cmd_conn_terminate(host_id);
    }
    break;

    }
}

/** update Ble FOTA step by input parameter "Action"
 *
 * @note       FOTA step is used to record the bank saving how much upgrading FW roughly,
 *             so that we can continue the transmission if FOTA upgrade process restart unexpectedly. \n
 *             OTA_STEP_INIT : Get current FOTA step \n
 *             OTA_STEP_UPDATE : Stamp next FOTA step \n
 *             OTA_STEP_RESET : Reset current FOTA step to zero \n
 *
 * @param[in] action : the actions for update FOTA step.
 * @param[in] p_expect_add : the next expect address to program to bank.
 *
 * @return none
 */
static void ble_fota_step(fota_step_t action, uint32_t *p_expect_add)
{
    static uint32_t step_size = 0;
    static uint32_t curr_step = 0;
    uint32_t fota_bank_size;


    fota_bank_size = SIZE_OF_FOTA_BANK;
    if (action == OTA_STEP_INIT)
    {
        uint8_t *p_step = 0;

        /*get suitable step size*/
        while (step_size * OTA_DATA_STEP_TOTAL_NUM < fota_bank_size)
        {
            step_size += FLASH_PROGRAM_SIZE_PAGE;
        }

        /*calculate current step*/
        p_step = (uint8_t *)&p_fota_info->expectaddr_initstep;
        while (*(p_step + curr_step) == OTA_DATA_STEP_STAMPED)
        {
            curr_step++;
        }

        /*get expect address*/
        *p_expect_add = curr_step * step_size;

    }
    else if (action == OTA_STEP_UPDATE)
    {
        /*stamp new step*/
        if (*p_expect_add >= (curr_step + 1)*step_size)
        {
            uint8_t *p_addr = (uint8_t *)&p_fota_info->expectaddr_initstep;
            fota_program((uint32_t)(p_addr + curr_step), OTA_DATA_STEP_STAMPED, 1);
            curr_step++;
        }
    }
    else if (action == OTA_STEP_RESET)
    {
        curr_step = 0;
        step_size = 0;
        *p_expect_add = 0;
    }
}

static void set_flash_erase(uint32_t flash_addr, uint32_t image_size)
{
    uint32_t ErasedSize = 0;;

    while (image_size > ErasedSize)
    {
        if (((image_size - ErasedSize) > 0x10000) &&
                ( MULTIPLE_OF_64K(flash_addr + ErasedSize) ))
        {
            hosal_flash_erase(HOSAL_FLASH_ERASE_64K_SECTOR, flash_addr + ErasedSize);
            ErasedSize += 0x10000;
        }
        else if (((image_size - ErasedSize) > 0x8000) &&
                 ( MULTIPLE_OF_32K(flash_addr + ErasedSize) ))
        {
            hosal_flash_erase(HOSAL_FLASH_ERASE_32K_SECTOR, flash_addr + ErasedSize);
            ErasedSize += 0x8000;
        }
        else
        {
            hosal_flash_erase(HOSAL_FLASH_ERASE_SECTOR, flash_addr + ErasedSize);
            ErasedSize += SIZE_OF_FLASH_SECTOR_ERASE;
        }
        while (hosal_flash_ioctrl(HOSAL_FLASH_BUSY, NULL));
    }
}

static void ble_fota_data_program(uint8_t length, uint8_t *data)
{
    static uint32_t temp_data[FLASH_PROGRAM_SIZE_PAGE >> 2];
    uint32_t fota_update_fw_addr;

    fota_update_fw_addr = FOTA_UPDATE_BUFFER_FW_ADDRESS;
    /*  ------  here buffering OTA data for flash page programming  ------ */
    if (fota_data_param.buffered_data_len == 0)/* no buffering OTA data */
    {
        memcpy(((uint8_t *)&temp_data[0]), data, length);
        fota_data_param.buffered_data_len = length;
    }
    else if ((fota_data_param.buffered_data_len + length) >= FLASH_PROGRAM_SIZE_PAGE) /* buffering OTA data + current OTA data length over than flash page programming size */
    {
        memcpy(((uint8_t *)&temp_data[0]) + fota_data_param.buffered_data_len, data, (FLASH_PROGRAM_SIZE_PAGE - fota_data_param.buffered_data_len));

        hosal_flash_write(HOSAL_FLASH_WRITE_PAGE, (uint32_t)(fota_data_param.programed_addr + fota_update_fw_addr), (uint8_t *)&temp_data[0]);
        while (hosal_flash_ioctrl(HOSAL_FLASH_BUSY, NULL));

        fota_data_param.programed_addr += FLASH_PROGRAM_SIZE_PAGE;
        ble_fota_step(OTA_STEP_UPDATE, &fota_data_param.programed_addr);

        memcpy(((uint8_t *)&temp_data[0]), data + (FLASH_PROGRAM_SIZE_PAGE - fota_data_param.buffered_data_len), length - (FLASH_PROGRAM_SIZE_PAGE - fota_data_param.buffered_data_len));
        fota_data_param.buffered_data_len = length - (FLASH_PROGRAM_SIZE_PAGE - fota_data_param.buffered_data_len);
    }
    else /* buffering OTA data + current OTA data length lower than flash page programming size */
    {
        memcpy(((uint8_t *)&temp_data[0]) + fota_data_param.buffered_data_len, data, length);
        fota_data_param.buffered_data_len += length;
    }

    /* check buffer OTA data + current OTA data length over than new FW size */
    if ((fota_data_param.programed_addr + fota_data_param.buffered_data_len) >= p_fota_info->fotabank_datalen)
    {
        if (FLASH_PROGRAM_SIZE_PAGE - fota_data_param.buffered_data_len)
        {
            memset((((uint8_t *)&temp_data[0]) + fota_data_param.buffered_data_len), 0xFF, (FLASH_PROGRAM_SIZE_PAGE - fota_data_param.buffered_data_len));
        }
        hosal_flash_write(HOSAL_FLASH_WRITE_PAGE, (uint32_t)(fota_data_param.programed_addr + fota_update_fw_addr), (uint8_t *)&temp_data[0]);
        while (hosal_flash_ioctrl(HOSAL_FLASH_BUSY, NULL));

        fota_data_param.programed_addr += fota_data_param.buffered_data_len;
        ble_fota_step(OTA_STEP_UPDATE, &fota_data_param.programed_addr);
    }
}

/******************************************************************************
 * Public Functions
 ******************************************************************************/
/** Ble FOTA Timer tick
 *
 *
 * @note       This function should be called once every second, \n
 *             so that the fota related timers can be normally operation. \n
 *
 * @retval RUNNING : FOTA timer is running.
 * @retval EXPIRED : FOTA timer is expired.
 */
fota_timerstate_t ble_fota_timertick(void)
{
    fota_tmr_param.curr_time++;

    if ((fota_tmr_param.expiry_time != 0) && (fota_tmr_param.curr_time > fota_tmr_param.expiry_time))
    {
        fota_tmr_param.expiry_time = 0;
        return EXPIRED;
    }

    return RUNNING;
}

/** Ble FOTA handle FOTA timer expired event
 *
 * @note       This function should be called when FOTA timer is expired.
 *
 * @param[in] host_id : thid links's host id.
 *
 * @return none
 */
void ble_fota_timerexpiry_handler(uint8_t host_id)
{
    ble_fota_timerexpiry(host_id, fota_tmr_param.timer_type);
}

void ble_fota_fw_buffer_flash_check(void)
{
    uint32_t page_idx = 0, fota_bank_size, fota_update_fw_addr;
    uint8_t page_program_cnt_0 = 0, page_program_cnt_1 = 0;
    uint32_t read_buf[FLASH_PROGRAM_SIZE_PAGE >> 2];
    uint8_t *p_verify_buf;


    if ((p_fota_info->fotabank_ready != FOTA_IMAGE_READY) &&
            (p_fota_info->status != FOTABANK_STATUS_FLASH_PROGRAMMING) &&
            (p_fota_info->status != FOTABANK_STATUS_FLASH_ERASE_FAIL))
    {
        p_verify_buf = pvPortMalloc(FLASH_PROGRAM_SIZE_PAGE);

        if (p_verify_buf)
        {
            fota_bank_size = SIZE_OF_FOTA_BANK;
            fota_update_fw_addr = FOTA_UPDATE_BUFFER_FW_ADDRESS;

            for (page_idx = 0 ; page_idx < fota_bank_size ; page_idx += FLASH_PROGRAM_SIZE_PAGE)
            {
                if (hosal_flash_read(HOSAL_FLASH_READ_PAGE, (fota_update_fw_addr + page_idx), (uint8_t *)&read_buf[0]) != HOSAL_STATUS_SUCCESS)
                {
                    printf("Read flash failed!\n");
                    break;
                }
                while (hosal_flash_ioctrl(HOSAL_FLASH_BUSY, NULL));

                memset(p_verify_buf, 0x00, sizeof(uint8_t) * FLASH_PROGRAM_SIZE_PAGE);
                if (memcmp(read_buf, p_verify_buf, FLASH_PROGRAM_SIZE_PAGE) != 0)
                {
                    page_program_cnt_0++;
                }

                memset(p_verify_buf, 0xFF, sizeof(uint8_t) * FLASH_PROGRAM_SIZE_PAGE);
                if (memcmp(read_buf, p_verify_buf, FLASH_PROGRAM_SIZE_PAGE) != 0)
                {
                    page_program_cnt_1++;
                }

                if ((page_program_cnt_1 != 0) && (page_program_cnt_0 != 0))
                {
                    break;
                }
            }

            if ((page_program_cnt_0 != 0) && (page_program_cnt_1 != 0))
            {
                printf("fw buffer not empty!\n");
                set_flash_erase((uint32_t)fota_update_fw_addr, fota_bank_size);
            }

            vPortFree(p_verify_buf);
        }
    }
}

/** Ble FOTA parameters initialization
 *
 *
 * @return none
 */
void ble_fota_init(void)
{
    uint32_t i = 0;

    if (p_fota_info->fotabank_ready == FOTA_IMAGE_READY)
    {
        printf("FOTA Result = %d\n", p_fota_info->fota_result);

        set_flash_erase((uint32_t)p_fota_info->fotabank_startaddr, p_fota_info->fotabank_datalen);
        hosal_flash_erase(HOSAL_FLASH_ERASE_SECTOR, FOTA_UPDATE_BANK_INFO_ADDRESS);
        while (hosal_flash_ioctrl(HOSAL_FLASH_BUSY, NULL));
    }

    ble_fota_step(OTA_STEP_INIT, &fota_data_param.expectaddr);

    fota_data_param.programed_addr = fota_data_param.currlen = fota_data_param.last_notify_addr = fota_data_param.expectaddr;
    fota_data_param.notify_interval = 0xFFFFFFFF;

    fota_data_param.buffered_data_len = 0;
    printf("sysinfo: ");
    for (i = 0; i < FW_INFO_LEN; i++)
    {
        printf("%c", systeminfo.sysinfo[i]);
    }
    printf("\n");

}

/** Ble FOTA current state
 *
 *
 * @return @ref fota_state_t the state of BLE FOTA
 */
fota_state_t ble_fota_state_get(void)
{
    return fota_upgrade_state;
}

/** The actions related to FOTA after complete the disconnection.
 *
 * @note       perform the action by fota_upgrade_state. \n
 *             OTA_STATE_COMPLETE : System reboot for bootloader to check new FW. \n
 *             OTA_STATE_ERASING : Erase bank FW and bank information and waiting for reconnection. \n
 *
 * @return none
 */
void ble_fota_disconnect(void)
{
    if (fota_upgrade_state == OTA_STATE_COMPLETE)
    {
        ble_fota_system_reboot();
    }
    else if (fota_upgrade_state == OTA_STATE_ERASING)
    {
        uint32_t page_idx = 0, fota_bank_size, fota_update_fw_addr;
        uint8_t status;

        fota_bank_size = SIZE_OF_FOTA_BANK;
        fota_update_fw_addr = FOTA_UPDATE_BUFFER_FW_ADDRESS;

        fota_flash_erase(FOTA_UPDATE_BANK_INFO_ADDRESS);
        for (page_idx = 0 ; page_idx < fota_bank_size ; page_idx += SIZE_OF_FLASH_SECTOR_ERASE)
        {
            status = fota_flash_erase(fota_update_fw_addr + page_idx);
            if (status != STATUS_SUCCESS)
            {
                printf("flash erase fail %d\n", status);
                fota_program((uint32_t)(&p_fota_info->status), FOTABANK_STATUS_FLASH_ERASE_FAIL, 4);
                break;
            }
        }
        ble_fota_step(OTA_STEP_RESET, &fota_data_param.expectaddr);
        ble_fota_init();
        fota_upgrade_state = OTA_STATE_IDLE;
    }
    else
    {
        fota_upgrade_state = OTA_STATE_IDLE;
    }
}

/** Ble FOTA command processing
 *
 *
 * @param[in] host_id : thid links's host id.
 * @param[in] length : command length.
 * @param[in] p_data : command payload.
 *
 * @note       First byte of command payload contains command ID and each command ID may contain different information behind.
 *             OTA_CMD_QUERY : Get device current system information. \n
 *             OTA_CMD_START : Start FW upgrade, this command contains new FW length and CRC. \n
 *             OTA_CMD_ERASE : Terminated the connection and erasing legacy FW and information. \n
 *             OTA_CMD_APPLY : Apply the new FW if receiving FW length and CRC matched with OTA_CMD_START. \n
 *
 * @return none
 */
void ble_fota_cmd(uint8_t host_id, uint8_t length, uint8_t *p_data)
{
    fota_cmd_param_t *p_fota_cmd = (fota_cmd_param_t *)p_data;
    uint8_t ind_len = sizeof(fota_errcode_t); /*first byte of indication always contains error code*/
    ble_info_link0_t *p_profile_info;
    ble_gatt_data_param_t param;
    uint32_t fota_bank_size, fota_update_fw_addr;
    fota_idc_param_t fota_idc;

    fota_bank_size = SIZE_OF_FOTA_BANK;
    fota_update_fw_addr = FOTA_UPDATE_BUFFER_FW_ADDRESS;

    switch (p_fota_cmd->cmd_id)
    {
    case OTA_CMD_QUERY:
    {
        /* FOTA query command format.
            _______________________
            octets    |1          |
            _______________________
            parameter | CommandID |
                      | (0x00)    |
            _______________________ */

        fota_idc.err_code = OTA_ERR_CODE_NO_ERR;
        /*Send system information & feature list by indication*/
        memcpy(fota_idc.param.sys_info.info, systeminfo.sysinfo, FW_INFO_LEN);
        fota_idc.param.sys_info.feature_list = systeminfo.feature_list;
        ind_len += sizeof(fota_idc_sys_info_t);

        fota_upgrade_state = OTA_STATE_QUERY;
    }
    break;

    case OTA_CMD_START:
    {

        /* FOTA start command format.
            ________________________________________________________
            octets    |1          |4       |4    |4         |1     |
            ________________________________________________________
            parameter | CommandID | FW     | FW  | Notify   | FW   |
                      | (0x01)    | length | CRC | interval | info |
            _________________________________________________________ */
        fota_data_param.notify_interval = p_fota_cmd->cmd_param.start_cmd.notify_interval;

        if ((p_fota_info->status == FOTABANK_STATUS_FLASH_PROGRAMMING) || (fota_upgrade_state == OTA_STATE_START)) /*Check if there were unfinish FOTA data */
        {
            printf("ExpectAddr: 0x%08x\n", fota_data_param.expectaddr);
            fota_data_param.currlen = fota_data_param.last_notify_addr = fota_data_param.expectaddr;
            /*Send legacy FW informations (FW length, FW CRC & next expected FW address) by indication*/
            fota_idc.err_code = OTA_ERR_CODE_ALREADY_START;
            fota_idc.param.already_start.expect_fw_len = p_fota_info->fotabank_datalen;
            fota_idc.param.already_start.expect_fw_crc = p_fota_info->fotabank_crc;
            fota_idc.param.already_start.stored_fw_len = fota_data_param.expectaddr;

            ind_len += sizeof(fota_idc_already_start_t);
            fota_upgrade_state = OTA_STATE_START;

        }
        else if (p_fota_cmd->cmd_param.start_cmd.fw_len > fota_bank_size) /*Check if updating FW length larger than bank size */
        {
            /*Send bank size by indication*/
            fota_idc.err_code = OTA_ERR_CODE_OUT_OF_BANK_SIZE;
            fota_idc.param.out_of_bank.bank_size = fota_bank_size;

            ind_len += sizeof(fota_idc_out_of_bank_t);
        }
        else if (p_fota_info->status == FOTABANK_STATUS_FLASH_ERASE_FAIL) /*Check if bank flash was fail to erase*/
        {
            /*Send bank flash fail to erase by indication*/
            fota_idc.err_code = OTA_ERR_CODE_FLASH_ERASE_ERR;
        }
        else
        {
            /*Record updating FW information into flash and start FOTA update procedure*/
            fota_upgrade_state = OTA_STATE_START;
            fota_idc.err_code = OTA_ERR_CODE_NO_ERR;
            fota_data_param.currlen = fota_data_param.expectaddr = 0;
            fota_program((uint32_t)(&p_fota_info->status), FOTABANK_STATUS_FLASH_PROGRAMMING, sizeof(p_fota_info->status));
            fota_program((uint32_t)(&p_fota_info->fotabank_crc), p_fota_cmd->cmd_param.start_cmd.fw_crc, sizeof(p_fota_info->fotabank_crc));
            fota_program((uint32_t)(&p_fota_info->fotabank_datalen), p_fota_cmd->cmd_param.start_cmd.fw_len, sizeof(p_fota_info->fotabank_datalen));
            if ((length - sizeof(p_fota_cmd->cmd_id)) == 12)
            {
                printf("mobile app unsupport image compress\n");
                fota_program((uint32_t)(&p_fota_info->fota_image_info), 0, sizeof(p_fota_info->fota_image_info));
            }
            else
            {
                if (p_fota_cmd->cmd_param.start_cmd.fw_info & FOTA_IMAGE_INFO_COMPRESSED)
                {
                    printf("update image compressed\n");
                }

                if (p_fota_cmd->cmd_param.start_cmd.fw_info & FOTA_IMAGE_INFO_SIGNATURE)
                {
                    printf("update image need to check signature\n");
                    fota_program((uint32_t)(&p_fota_info->signature_len), p_fota_cmd->cmd_param.start_cmd.signature_len, sizeof(p_fota_info->signature_len));
                }
                fota_program((uint32_t)(&p_fota_info->fota_image_info), p_fota_cmd->cmd_param.start_cmd.fw_info, sizeof(p_fota_info->fota_image_info));
            }

            hosal_flash_ioctrl(HOSAL_FLASH_CACHE, NULL);
        }
        printf("fota start %d, ExpectAddr: 0x%08x interval %d\n", fota_idc.err_code, fota_data_param.expectaddr, fota_data_param.notify_interval);

    }
    break;

    case OTA_CMD_ERASE:
    {
        /* FOTA erase command format.
            _______________________
            octets    |1          |
            _______________________
            parameter | CommandID |
                      | (0x02)    |
            _______________________ */
        /*Erasing the legacy updating FW and FW information*/
        fota_idc.err_code = OTA_ERR_CODE_NO_ERR;
        fota_upgrade_state = OTA_STATE_ERASING;
        ble_fota_timerstart(1, OTA_TIMER_OTA_ERASING);
        printf("fota erase, disconnect to clean legacy FW\n");
    }
    break;

    case OTA_CMD_APPLY:
    {
        uint32_t chk_sum;

        /* FOTA apply command format.
            _________________________________
            octets    |1          |4        |
            _________________________________
            parameter | CommandID | Bank0   |
                      | (0x03)    | address |
            _________________________________  */

        if (fota_upgrade_state != OTA_STATE_START) /*Check if FOTA procedure has started */
        {
            fota_idc.err_code = OTA_ERR_CODE_UPDATE_NOT_START;
        }
        else
        {
            fota_upgrade_state = OTA_STATE_COMPLETE;

            chk_sum = ble_fota_crc32checksum((uint32_t)fota_update_fw_addr, p_fota_info->fotabank_datalen);
            printf("ChkSum 0x%08x 0x%08x\n", chk_sum,  p_fota_info->fotabank_crc);
            if (fota_data_param.currlen != p_fota_info->fotabank_datalen) /*Check if receiving FW length matched FOTA start command*/
            {
                fota_idc.err_code = OTA_ERR_CODE_FW_LEN_ERR;
            }
            else if (chk_sum != p_fota_info->fotabank_crc) /*Check if receiving FW CRC matched FOTA start command*/
            {
                printf("ChkSum 0x%08x 0x%08x\n", chk_sum,  p_fota_info->fotabank_crc);
                fota_idc.err_code = OTA_ERR_CODE_FW_CRC_ERR;
            }
            else
            {
                fota_idc.err_code = OTA_ERR_CODE_NO_ERR;
                /*Set FOTA informations into flash for bootloader*/
                fota_program((uint32_t)(&p_fota_info->fotabank_ready), FOTA_IMAGE_READY, sizeof(p_fota_info->fotabank_ready));
                fota_program((uint32_t)(&p_fota_info->fotabank_startaddr), (uint32_t)fota_update_fw_addr, sizeof(p_fota_info->fotabank_startaddr));
                fota_program((uint32_t)(&p_fota_info->target_startaddr), p_fota_cmd->cmd_param.apply_cmd.bank0_address, sizeof(p_fota_info->target_startaddr));


            }
            ble_fota_timerstart(1, OTA_TIMER_OTA_DISCONNECT);
        }
        printf("fota apply %d\n", fota_idc.err_code);
    }
    break;

    default:
        printf("not supported %d\n", p_fota_cmd->cmd_id);
        fota_idc.err_code = OTA_ERR_CODE_CMD_ERR;
        break;
    }

    p_profile_info = (ble_info_link0_t *)ble_app_link_info[host_id].profile_info;

    // set parameters
    param.host_id = host_id;
    param.handle_num = p_profile_info->svcs_info_fotas.server_info.handles.hdl_command;
    param.length = ind_len;
    param.p_data = (uint8_t *)&fota_idc;

    ble_svcs_data_send(TYPE_BLE_GATT_INDICATION, &param);
}


/** Ble FOTA data processing
 *
 *
 * @param[in] host_id : thid links's host id.
 * @param[in] length : data length.
 * @param[in] p_data : data payload.
 *
 * @note       First 4 bytes of data payload is data header which contains the FOTA data address (3 bytes) and length (1 byte),
 *             if there were invalid data header, send notification to response it. \n
 *
 * @return none
 */
void ble_fota_data(uint8_t host_id, uint8_t length, uint8_t *p_data)
{
    fota_data_param_t *p_fota_data = (fota_data_param_t *)p_data;
    ble_err_t status;
    uint8_t notify_len = sizeof(fota_notify_t);
    uint32_t fota_bank_size;
    fota_notify_param_t fota_notify;


    fota_bank_size = SIZE_OF_FOTA_BANK;

    /* FOTA data format.
    _____________________________________
    octets    |3        |1       | Var  |
    _____________________________________
    parameter | Data    | Data   | Data |
              | address | length |      |
    _____________________________________  */

    fota_notify.notify_code = OTA_DATA_NOTIFY_NONE;
    if (fota_upgrade_state != OTA_STATE_START) /*Check if FOTA procedure has started*/
    {
        fota_notify.notify_code = OTA_DATA_NOTIFY_NOT_START;
    }
    else if (p_fota_data->data_addr > fota_bank_size) /*Check if FOTA data's address is larger than bank size*/
    {
        fota_notify.notify_code = OTA_DATA_NOTIFY_ADDRESS_ERR;
    }
    else if (p_fota_data->data_addr != fota_data_param.expectaddr) /*Check if FOTA data's address is matched with expecting*/
    {
        fota_notify.notify_code = OTA_DATA_NOTIFY_ADDRESS_UNEXPECTED;

        fota_notify.param.addr_unexpect.expect_addr = fota_data_param.expectaddr;
        notify_len += sizeof(fota_notify_addr_unexpect_t);
        fota_data_param.last_notify_addr = fota_data_param.expectaddr;
        printf("Add unexp %d %d\n", p_fota_data->data_addr, fota_data_param.expectaddr);
    }
    else if ((p_fota_data->data_len == 0) || (p_fota_data->data_len > length)) /*Check if FOTA data length vaild*/
    {
        fota_notify.notify_code = OTA_DATA_NOTIFY_LEN_ERROR;

        fota_notify.param.data_len_err.rx_data_len = p_fota_data->data_len;
        notify_len += sizeof(fota_notify_rx_data_len_err_t);
        printf("data len %d length %d\n", p_fota_data->data_len, length);
    }
    else if ((p_fota_data->data_len + fota_data_param.currlen) >  fota_bank_size)/*Check if total received FOTA data length larger than bank size*/
    {
        fota_notify.notify_code = OTA_DATA_NOTIFY_TOTAL_LEN_ERR;

        fota_notify.param.total_len_err.total_data_len = (fota_data_param.currlen + p_fota_data->data_len);
        notify_len += sizeof(fota_notify_total_data_len_err_t);
    }

    if (fota_notify.notify_code != OTA_DATA_NOTIFY_NONE)/*Check if notification needs to send*/
    {
        ble_gatt_data_param_t param;
        ble_info_link0_t *p_profile_info = (ble_info_link0_t *)ble_app_link_info[host_id].profile_info;

        // set parameters
        param.host_id = host_id;
        param.handle_num = p_profile_info->svcs_info_fotas.server_info.handles.hdl_data;
        param.length = notify_len;
        param.p_data = (uint8_t *)&fota_notify;

        status = ble_svcs_data_send(TYPE_BLE_GATT_NOTIFICATION, &param);
        if (status != BLE_ERR_OK)
        {
            printf("Notify send failed, status = %d\n", fota_notify.notify_code);
        }
    }
    else
    {
        /* programming OTA data into flash ota bank*/
        ble_fota_data_program(p_fota_data->data_len, p_fota_data->data);

        /* update how much OTA data is receiving */
        fota_data_param.expectaddr += p_fota_data->data_len;

        /*Check if periodic notification interval reached*/
        if (fota_data_param.expectaddr >= (fota_data_param.notify_interval + fota_data_param.last_notify_addr))
        {
            ble_gatt_data_param_t param;
            ble_info_link0_t *p_profile_info = (ble_info_link0_t *)ble_app_link_info[host_id].profile_info;

            fota_data_param.last_notify_addr = fota_data_param.expectaddr;

            fota_notify.notify_code = OTA_DATA_NOTIFY_PERIODIC;
            fota_notify.param.periodic.expect_addr = fota_data_param.expectaddr;
            notify_len += sizeof(fota_notify_periodic_t);

            // set parameters
            param.host_id = host_id;
            param.handle_num = p_profile_info->svcs_info_fotas.server_info.handles.hdl_data;
            param.length = notify_len;
            param.p_data = (uint8_t *)&fota_notify;

            status = ble_svcs_data_send(TYPE_BLE_GATT_NOTIFICATION, &param);
            if (status != BLE_ERR_OK)
            {
                printf("Notify periodic send fail\n");
            }
            printf("Notify int 0x%04x %d\n", fota_data_param.expectaddr, status);
        }

        fota_data_param.currlen += p_fota_data->data_len;
    }
}
