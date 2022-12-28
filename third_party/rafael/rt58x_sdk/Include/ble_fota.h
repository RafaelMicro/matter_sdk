/**************************************************************************//**
* @file       fota.h
* @brief      This file contains the functions of FOTA.
*
*****************************************************************************/
#ifndef _FOTA_H_
#define _FOTA_H_

#include <stdint.h>
#include "fota_define.h"
/******************************************************************************
 * Definitions
 ******************************************************************************/
typedef uint8_t fota_state_t;
#define OTA_STATE_IDLE                          0x00                /**< FOTA procedure idle */
#define OTA_STATE_START                         0x01                /**< FOTA procedure start */
#define OTA_STATE_COMPLETE                      0x02                /**< FOTA procedure complete */
#define OTA_STATE_ERASING                       0x03                /**< Legacy FW erase */

typedef uint8_t fota_cmd_t;
#define OTA_CMD_QUERY                           0x00                /**< Query current system information */
#define OTA_CMD_START                           0x01                /**< Start FW OTA update */
#define OTA_CMD_ERASE                           0x02                /**< Erase legacy FW */
#define OTA_CMD_APPLY                           0x03                /**< FW transmission completed, apply new FW */

/*Indication error codes for FOTA command*/
typedef uint8_t fota_errcode_t;
#define OTA_ERR_CODE_NO_ERR                     0x00                /**< Command success */
#define OTA_ERR_CODE_CMD_ERR                    0x01                /**< Unsupported command ID  */
#define OTA_ERR_CODE_ALREADY_START              0x02                /**< FOTA procedure already start */
#define OTA_ERR_CODE_UPDATE_NOT_START           0x03                /**< FOTA procedure was not start */
#define OTA_ERR_CODE_FLASH_ERASE_ERR            0x04                /**< Flash erase fail */
#define OTA_ERR_CODE_FW_CRC_ERR                 0x05                /**< Receiving FW's CRC incorrect */
#define OTA_ERR_CODE_FW_LEN_ERR                 0x06                /**< Receiving FW's length incorrect */
#define OTA_ERR_CODE_OUT_OF_BANK_SIZE           0x07                /**< Updating FW's length larger than bank size */

/*Notification reasons for FOTA data*/
typedef uint8_t fota_notify_t;
#define OTA_DATA_NOTIFY_PERIODIC                0x00                /**< Periodic notification, the notify interval set from FOTA start command */
#define OTA_DATA_NOTIFY_TIMEOUT                 0x01                /**< FOTA data was not received in a specific interval */
#define OTA_DATA_NOTIFY_ADDRESS_UNEXPECTED      0x02                /**< Received FOTA data's address was not continuously */
#define OTA_DATA_NOTIFY_LEN_ERROR               0x03                /**< Received FOTA data length incorrect */
#define OTA_DATA_NOTIFY_TOTAL_LEN_ERR           0x04                /**< Total received FOTA data length is larger than bank size */
#define OTA_DATA_NOTIFY_ADDRESS_ERR             0x05                /**< Received FOTA data's address is larger than bank size */
#define OTA_DATA_NOTIFY_NOT_START               0x06                /**< FOTA data received but FOTA procedure was not start */
#define OTA_DATA_NOTIFY_NONE                    0xFF                /**< No notification needs to send */

typedef uint8_t fota_timer_t;
#define OTA_TIMER_OTA_DATA                      0x00                /**< Timer for send notification if FOTA data was not received in a specific interval */
#define OTA_TIMER_OTA_COMPLETE                  0x01                /**< Timer for system reboot */
#define OTA_TIMER_OTA_DISCONNECT                0x02                /**< Timer for disconnection & complete FOTA procedure */
#define OTA_TIMER_OTA_ERASING                   0x03                /**< Timer for disconnection & erasing legacy FW */

/*OTA step is use for continue the transmission if FOTA procedure restart*/
typedef uint8_t fota_step_t;
#define OTA_STEP_INIT                           0x00                /**< Step size & current step initialization */
#define OTA_STEP_UPDATE                         0x01                /**< Current step update */
#define OTA_STEP_RESET                          0x02                /**< Step size and current step reset */

#define OTA_DATA_STEP_TOTAL_NUM                 (SIZE_OF_FLASH_SECTOR_ERASE - sizeof(fota_information_t))
/** Step number means upgrading FW been splitted to how much steps to stamp.
Cause each step stamp were saving to the end of structure "fota_information_t",
the size of structure "fota_information_t" for now is defined as 4096 bytes,
so that maximum total step number * FLASH_PROGRAM_SIZE for current definitions shall smaller than 448*/

#define OTA_DATA_STEP_STAMPED                   0x88    /**< Specific number use for stamped each steps */

#define PREFIX_LEN                              7
#define FW_INFO_LEN                             16

typedef enum
{
    RUNNING,
    EXPIRED,
} fota_timerstate_t;

typedef struct
{
    uint8_t prefix[PREFIX_LEN];
    uint8_t sysinfo[FW_INFO_LEN];

} sys_information_t;

/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/
/** Ble FOTA Timer tick
 *
 *
 * @note       This function should be called once every second, \n
 *             so that the fota related timers can be normally operation. \n
 *
 * @retval RUNNING : FOTA timer is running.
 * @retval EXPIRED : FOTA timer is expired.
 */
fota_timerstate_t ble_fota_timertick(void);


/** Ble FOTA handle FOTA timer expired event
 *
 * @note       This function should be called when FOTA timer is expired.
 *
 * @param[in] host_id : thid links's host id.
 *
 * @return none
 */
void ble_fota_timerexpiry_handler(uint8_t host_id);

/** Ble FOTA fw buffer flash check
 *
 *
 * @return none
 */
void ble_fota_fw_buffer_flash_check(void);

/** Ble FOTA parameters initialization
 *
 *
 * @return none
 */
void ble_fota_init(void);


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
 *             OTA_CMD_ERASE : Terminated the connection and erasing legacy FW and informations. \n
 *             OTA_CMD_APPLY : Apply the new FW if receiving FW length and CRC matched with OTA_CMD_START. \n
 *
 * @return none
 */
void ble_fota_cmd(uint8_t host_id, uint8_t length, uint8_t *p_data);


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
void ble_fota_data(uint8_t host_id, uint8_t length, uint8_t *p_data);


/** The actions related to FOTA after complete the disconnection.
 *
 * @note       perform the action by FwUpgradeState. \n
 *             OTA_STATE_COMPLETE : System reboot for bootloader to check new FW. \n
 *             OTA_STATE_ERASING : Erase bank FW and bank information and waiting for reconnection. \n
 *
 * @return none
 */
void ble_fota_disconnect(void);


#endif // _FOTA_H_

