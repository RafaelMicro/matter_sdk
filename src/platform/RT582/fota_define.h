/**************************************************************************//**
* @file       fota.h
* @brief      This file contains the functions of FOTA.
*
*****************************************************************************/
#ifndef _FOTA_DEFINE_H_
#define _FOTA_DEFINE_H_

#include <stdint.h>
/******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * @brief Defined supported flash sector erase  size.
 */
#define SIZE_OF_FLASH_SECTOR_ERASE                    4096          /**< Size of Flash erase Type.*/
/** @} */


/**
 * @brief Defined supported flash programming size.
 */
#define FLASH_PROGRAM_SIZE_1BYTE                      1     /**< Programming 1 byte at once  */
#define FLASH_PROGRAM_SIZE_2BYTE                      2     /**< Programming 2 bytes at once */
#define FLASH_PROGRAM_SIZE_4BYTE                      4     /**< Programming 4 bytes at once */
#define FLASH_PROGRAM_SIZE_PAGE                     256     /**< Programming 256 bytes at once */
/** @} */


/**
 * @brief Selected a flash programming size at once from @ref flashProgramSizeDef "Flash programming size definition list".
 *
 * @attention Please update the definition of @ref FLASH_PROGRAM_SIZE_FOR_POWER_OF_2 if change the definition of @ref FLASH_PROGRAM_SIZE.
*/
#define FLASH_PROGRAM_SIZE                            FLASH_PROGRAM_SIZE_1BYTE



#define BOOTLOADER_SIZE                               0x8000              /**< Total Size for bootloader area.*/
#define MP_SECTOR_SIZE                                0x10000             /**< Total Size for MP sector area.*/

#define FOTA_UPDATE_BANK_INFO_ADDRESS                 0x00007000          /**< Address of FOTA update information*/
#define APP_START_ADDRESS                             0x00008000          /**< Start address of application code.*/

#define FOTA_UPDATE_BUFFER_FW_ADDRESS_1MB             0x00099000          /**< Address of FOTA update data.*/
#define SIZE_OF_FOTA_BANK_1MB                         0x00057000          /**< Total Size for saving FOTA update data.*/

#define FOTA_UPDATE_BUFFER_FW_ADDRESS_1MB_UNCOMPRESS  0x00100000          /**< Address of uncompressed FOTA update data.*/
#define SIZE_OF_FOTA_BANK_1MB_UNCOMPRESS              0x00074000          /**< Total Size for saving FOTA update data.*/

#define FOTA_UPDATE_BUFFER_FW_ADDRESS_512K            0x0004F000          /**< Address of FOTA update data.*/
#define SIZE_OF_FOTA_BANK_512K                        0x0002B000          /**< Total Size for saving FOTA update data.*/

#define FOTA_UPDATE_BUFFER_FW_ADDRESS_512K_UNCOMPRESS 0x00041000          /**< Address of FOTA update data.*/
#define SIZE_OF_FOTA_BANK_512K_UNCOMPRESS             0x00039000          /**< Total Size for saving FOTA update data.*/

#define FOTA_IMAGE_READY                              0xA55A6543          /**< Specific string for notify bootloader that bank1 exist image ready to update.*/

typedef uint32_t fotabank_status;
#define FOTABANK_STATUS_FLASH_PROGRAMMING             0x00000000
#define FOTABANK_STATUS_FLASH_ERASE_FAIL              0x00000001

typedef uint8_t fotaimage_info;
#define FOTA_IMAGE_INFO_COMPRESSED                          0x01
#define FOTA_IMAGE_INFO_SIGNATURE                           0x02

#define FOTA_RESULT_SUCCESS                                 0x00
#define FOTA_RESULT_ERR_VERIFY_SIZE_NOT_FOUND               0x01
#define FOTA_RESULT_ERR_IMAGE_SIZE                          0x02
#define FOTA_RESULT_ERR_SECURE_MAGIC_PATTERN_MISMATCH       0x03
#define FOTA_RESULT_ERR_SECURE_VERIFY_SIZE_MISMATCH         0x04
#define FOTA_RESULT_ERR_IMAGE_ECDSA_VERIFY_FAIL             0x05
#define FOTA_RESULT_ERR_CHECK_IMAGE_CRC_FAIL                0x06
#define FOTA_RESULT_ERR_TARGET_ADDR_IS_ILLEGAL              0x07
#define FOTA_RESULT_ERR_COMPRESS_DECODE_FAIL                0x08


typedef struct
{
    fotabank_status     status;                 /* Two possible results for Bank 1: already programming, erase failure */
    uint32_t            fotabank_ready;         /* Notify bootloader that new FW is stored in Bank 1 */
    uint32_t            fotabank_crc;           /* Crc32 checksum of FW in Bank 1 */
    uint32_t            fotabank_datalen;       /* Data length of FW in Bank 1 */
    uint32_t            fotabank_startaddr;     /* The starting flash address of fota bank */
    uint32_t            target_startaddr;       /* The starting flash address of Target */
    uint8_t             fota_result;            /* The FOTA result after Reboot */
    fotaimage_info      fota_image_info;        /* The FOTA image compressed */
    uint32_t            signature_len;          /* The length to generate signature */
    uint8_t             reserved_[2];           /* Reserved for future use */
    uint32_t            reserved[8];            /* Reserved for future use */
    uint32_t            expectaddr_initstep;    /* The initial (first) address for stored step that use for calculate expecting address to retransmission.
                                                For resume FOTA transmission, we use steps to counting so far how much data were store into fota bank,
                                                more step means more data been stored. Therefore, we can use step number to calculate "Expecting start address".*/
} fota_information_t;

#endif  //_FOTA_DEFINE_H_
