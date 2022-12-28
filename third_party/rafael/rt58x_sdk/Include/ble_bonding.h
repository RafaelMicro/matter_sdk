/** @file ble_bonding.h
 *
 * @addtogroup ble_sec_api
 * @{
 * @defgroup ble_bonding_storage BLE Bonding Storage APIs
 * @{
 * @brief This file defined BLE Bonding Related.
 * @}
 * @}
 */
#ifndef __BLE_BONDING_H__
#define __BLE_BONDING_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "ble_host_ref.h"
#include "fota_define.h"


/**************************************************************************************************
 *    DEFINITIONS
 *************************************************************************************************/


/** @brief Flash default value definition.
 * @ingroup ble_bonding_storage
*/
#define FLASH_DEFAULT_VALUE                           0xFF


/**
 * @defgroup ble_bonding_flash_map MCU Flash Map (BLE Bonding) Porting Definition
 * @{
 * @ingroup ble_bonding_storage
 * @brief MCU flash map for BLE bonding porting related definition.
 * @attention Do not modify the setting of @ref SIZE_OF_BONDING_INFORMATION and @ref SIZE_OF_BONDING_DATA.
 */
#define SIZE_OF_BONDING_INFORMATION                   (SIZE_OF_FLASH_SECTOR_ERASE << 1)     /**< Total size of BLE bonding information array.*/
/** @} */


/**
 * @defgroup ble_bonding_program_exp The Exponent of Flash Programming Size Definition
 * @{
 * @ingroup ble_bonding_storage
 * @brief Defined supported the exponent of flash programming size.
 */
#define FLASH_PROGRAM_SIZE_FOR_EXPONENT_0             0     /**< @ref FLASH_PROGRAM_SIZE = 2^exponent = 2^0 = 1 */
/** @} */


/** @brief Selected the exponent of flash programming size from @ref flashProgramSizeExponentDef "The exponent of flash programming size definition list".
 * @ingroup ble_bonding_storage
 * @attention The value of 2^ @ref FLASH_PROGRAM_SIZE_FOR_POWER_OF_2 shall be equal to @ref FLASH_PROGRAM_SIZE.
*/
#define FLASH_PROGRAM_SIZE_FOR_POWER_OF_2             FLASH_PROGRAM_SIZE_FOR_EXPONENT_0


/** @brief Selected the exponent of flash page size from @ref flashPageSizeDef "The exponent of flash page size definition list".
 * @ingroup ble_bonding_storage
 * @attention The value of 2^ @ref FLASH_PAGE_SIZE_FOR_POWER_OF_2 shall be equal to @ref FLASH_PAGE_SIZE.
*/
#define FLASH_SECTOR_SIZE_FOR_POWER_OF_2              12     /**< FLASH_PAGE 4096 = 2^12 */



/* Total size of BLE bonding information array.*/
/**
 * @ingroup ble_bonding_storage
 * @{
 */
#define SIZE_OF_KEY_BLK                                128                                              /**< Size of bond key information.*/
#define SIZE_OF_DATA_BLK                               128                                              /**< Size of data information.*/
#define SIZE_OF_INFO_BLK                               (SIZE_OF_KEY_BLK+SIZE_OF_DATA_BLK)               /**< Size of BLE bonding information block.*/
#define SIZE_OF_INFO_BLK_FOR_POWER_OF_2                8                                                /**< SIZE_OF_INFO_BLK 256 = 2^8. */

#define NUM_OF_TOTAL_INFO_BLK                          (SIZE_OF_BONDING_INFORMATION/SIZE_OF_INFO_BLK)   /**< Total number of inforamtion size of bound information block.*/
#define NUM_OF_FLASH_PAGE_FOR_BONDING_INFO_BLK         (SIZE_OF_BONDING_INFORMATION/SIZE_OF_FLASH_SECTOR_ERASE)    /**< Total number of flash size of bound information block.*/
#define NUM_OF_INFO_BLK_ONE_PAGE                       (SIZE_OF_FLASH_SECTOR_ERASE/SIZE_OF_INFO_BLK)               /**< Amount of information bound using flash page.*/

#define TAB_FLASH_BONDING_INFO_BLK                     SIZE_OF_INFO_BLK                                 /**< Tab Size of BLE bonding information block.*/

#define ERASE_THRESHOLD                                (NUM_OF_TOTAL_INFO_BLK-1)                        /**< Can be set 1 ~ (NUM_OF_TOTAL_INFO_BLK-1) */
/** @} */


/**
 * @defgroup ble_bonding_op_cmd BLE Bonding Operation Command Definition
 * @ingroup ble_bonding_storage
 * @{
 */
#define CMD_FB_GET_NEXT_PID                               0x01     /**< Bonding operation command : Get next PID number. */
#define CMD_FB_INIT_INFO_FLASHBOND                        0x02     /**< Bonding operation command : Initial Information Bonding space. */
#define CMD_FB_INIT_DATA_FLASHBOND                        0x03     /**< Bonding operation command : Initial Data Bonding space. */
#define CMD_FB_CHK_IF_FLASH_INITED                        0x04     /**< Bonding operation command : Check if bonding space has been initialized. */
#define CMD_FB_GET_EXIST_PID_BY_HOST_ID                   0x05     /**< Bonding operation command : Get exist PID number. */
#define CMD_FB_GET_KEY_FLASHBOND_PARA_BOND                0x06     /**< Bonding operation command : Get Bonding key information data. */
#define CMD_FB_GET_DATA_FLASHBOND_EXIST_HOSTID_DBLK_START 0x07     /**< Bonding operation command : Get starting position of the exist data bonding space. */
#define CMD_FB_GET_DATA_FLASHBOND_NXT_PID_DBLK_START      0x08     /**< Bonding operation command : Get starting position of next data bonding space. */
#define CMD_FB_PSH_DATA_FLASHBOND_EXIST_HOSTID_DBLK       0x09     /**< Bonding operation command : Push (Program) bonding data to exist bonding space. */
#define CMD_FB_CHK_IF_FLASHBOND_NEED_TO_ERASE_PAGE        0x0A     /**< Bonding operation command : Check if bonding space need to be erase or not. */
#define CMD_FB_PSH_BACKUP_KEY_FLASH_PARA_BOND             0x0B     /**< Bonding operation command : Push (Program) the backup key information to new bonding space. */
#define CMD_FB_PSH_BACKUP_DATA_FLASH_PARA_BOND            0x0C     /**< Bonding operation command : Push (Program) the backup data to new bonding space. */
#define CMD_FB_GET_DATA_VALUE_BY_HANDLE                   0x0D     /**< Bonding operation command : Get the backup data by valid handle & host id. */
/** @} */

/**
 * @defgroup ble_bonding_op_error BLE Bonding Operation Error Code Definition
 * @ingroup ble_bonding_storage
 * @{
 */
#define FLH_BND_ERR_CODE_NO_ERR                           0x00     /**< Bonding operation error code : No Error.  */
#define FLH_BND_ERR_CODE_FLASH_NOT_INI                    0x01     /**< Bonding operation error code : Flash space is not initialized. */
#define FLH_BND_ERR_CODE_IN_PROCESS                       0x02     /**< Bonding operation error code : Processing. */
#define FLH_BND_ERR_CODE_NO_FREE_PID                      0x03     /**< Bonding operation error code : No free adea in bonding space. */
#define FLH_BND_ERR_CODE_NO_EXIST_HOST_ID                 0x04     /**< Bonding operation error code : NOT found the exist host id number. */
#define FLH_BND_ERR_CODE_HOST_ID_DNT_MATCH                0x05     /**< Bonding operation error code : Host id not match. */
#define ERR_CODE_FLH_BND_NO_ENOUGH_REST_SPACE             0x06     /**< Bonding operation error code : Not enough program space. */
#define ERR_BND_ERR_CODE_NOT_EXIST_HANDLE                 0x07     /**< Bonding operation error code : NOT found the exist handle number. */
#define ERR_CODE_FLH_BND_SYS_ERROR                        0xFF     /**< Bonding operation error code : System Error. */
/** @} */


/**
 * @defgroup ble_bonding_tag_info BLE Bonding Tag Number Related Definition
 * @ingroup ble_bonding_storage
 * @{
 */
#define TAB_INFO_FLASHBOND_PID                            0                                             /**< The tag number of the PID in the information bonding space. */
#define TAB_INFO_FLASHBOND_HOSTID                         2                                             /**< The tag number of the host ID in the information bonding space. */
#define TAB_INFO_FLASHBOND_INI_ADDR                       4                                             /**< The tag number of BLE INI_ADDR in the information bonding space. */
#define TAB_INFO_FLASHBOND_SMP_PARA_BOND                  (TAB_INFO_FLASHBOND_INI_ADDR+SIZE_BLE_ADDR)   /**< The tag number of BLE SMP data in the information bonding space. */

#define TAB_PARA_DATA_ERROR_CODE                          0                                             /**< The tag number of error code. */
#define TAB_PARA_DATA_PID_H                               1                                             /**< The tag number of PID in data space. (High Byte) */
#define TAB_PARA_DATA_PID_L                               2                                             /**< The tag number of PID in data space. (Low Byte)*/
#define TAB_PARA_DATA_HOSTID                              3                                             /**< The tag number of host id in data space. */
#define TAB_PARA_DATA_INI_ADDR                            4                                             /**< The tag number of INI_ADDR . */
#define TAB_PARA_DATA_BOND_ROLE                          11                                             /**< The tag number of BLE connection role value. */
#define TAB_PARA_DATA_PEER_ID_ADDR_TYPE                  56                                             /**< The tag number of BLE Peer Identity address type. */
#define TAB_PARA_DATA_OWN_RAND                           71                                             /**< The tag number of own RAND value. */
#define TAB_PARA_DATA_OWN_EDIV                           79                                             /**< The tag number of own EDIV value. */
#define TAB_PARA_DATA_OWN_LTK                            81                                             /**< The tag number of own LTK value. */
#define TAB_PARA_DATA_OWN_IRK                            97                                             /**< The tag number of own IRK value. */
#define TAB_PARA_DATA_DAT_START_H                         4                                             /**< The tag number of the starting position. (High Byte)*/
#define TAB_PARA_DATA_DAT_START_L                         5                                             /**< The tag number of the starting position. (Low Byte)*/
#define TAB_PARA_DATA_DAT_HDL_H                           (TAB_PARA_DATA_DAT_START_L+1)                 /**< The tag number of data handle location. (High Byte)*/
#define TAB_PARA_DATA_DAT_HDL_L                           (TAB_PARA_DATA_DAT_START_L+2)                 /**< The tag number of data handle location. (Low Byte)*/
#define TAB_PARA_DATA_GATT_ROLE                           (TAB_PARA_DATA_DAT_START_L+3)                 /**< The tag number of the GATT data role. */
#define TAB_PARA_DATA_DAT_SIZE                            (TAB_PARA_DATA_DAT_START_L+4)                 /**< The tag number of data size location. */
#define TAB_PARA_DATA_DAT                                 (TAB_PARA_DATA_DAT_START_L+5)                 /**< The tag number of data location. */
/** @} */


/**
 * @defgroup ble_bonding_pos BLE Bonding the Starting Position Related Definition
 * @ingroup ble_bonding_storage
 * @{
 */
#define BOND_PEER_ADDR_TYPE                                0                                            /**< The starting position of PEER_ADDR type in the information bonding data. */
#define BOND_PEER_ADDR                                     1                                            /**< The starting position of PEER_ADDR in the information bonding data. */
#define BOND_ROLE                                          7                                            /**< The starting position of BLE connection role in the information bonding data. */
#define BOND_STK_GEN_METHOD                                8                                            /**< The starting position of BLE key gen method in the information bonding data. */
#define BOND_MAX_ENC_KEY_SIZE                              9                                            /**< The starting position of BLE KEY Size in the information bonding data. */
#define BOND_KEY_TYPE                                     10                                            /**< The starting position of BLE key type in the information bonding data. */
#define BOND_PEER_RAND                                    11                                            /**< The starting position of BLE PEER_RAND in the information bonding data. */
#define BOND_PEER_EDIV                                    19                                            /**< The starting position of BLE PEER EDIV in the information bonding data. */
#define BOND_PEER_LTK                                     21                                            /**< The starting position of BLE PEER LTK in the information bonding data. */
#define BOND_PEER_IRK                                     37                                            /**< The starting position of BLE PEER IRK in the information bonding data. */
#define BOND_PEER_ID_ADDR_TYPE                            53                                            /**< The starting position of BLE PEER ID ADDR Type in the information bonding data. */
#define BOND_PEER_ID_ADDR                                 54                                            /**< The starting position of BLE PEER ID ADDR in the information bonding data. */
#define BOND_OWN_ADDR_TYPE                                60                                            /**< The starting position of BLE Own ADDR Type in the information bonding data. */
#define BOND_OWN_ADDR                                     61                                            /**< The starting position of BLE Own ADDR in the information bonding data. */
#define BOND_OWN_RAND                                     67                                            /**< The starting position of BLE Own RAND in the information bonding data. */
#define BOND_OWN_EDIV                                     75                                            /**< The starting position of BLE Own EDIV in the information bonding data. */
#define BOND_OWN_LTK                                      77                                            /**< The starting position of BLE Own LTK in the information bonding data. */
#define BOND_OWN_IRK                                      93                                            /**< The starting position of BLE Own IRK in the information bonding data. */
#define SMP_PARA_BOND_SIZE                                (BOND_OWN_IRK+16)                             /**< The total size of the information bonding data. */
/** @} */

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
typedef struct
{
    uint8_t bonding_space[NUM_OF_TOTAL_INFO_BLK][SIZE_OF_INFO_BLK];
} bonding_space_t;

/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** @brief This function is used to control Bonding operation for BLE.
 *
 * @ingroup ble_bonding_storage
 *
 * @param[in] opcode     : control command of bonding operation.
 * @param[in] para_data  : the pointer of command data.
 *
 * @param[out] para_data : pointer to return data. \n
 */
uint8_t *ble_flashbond_cmd(uint8_t opcode, uint8_t *para_data);



/** @brief This function is used to fill Bonding information in Flash.
 *
 * @ingroup ble_bonding_storage
 *
 * @param[in] none
 *
 * @return none
 */
void ble_flashbond_fillwithPID(uint8_t *para_data);



/** @brief This function is used to restore bonding data to service and profile.
 *
 * @ingroup ble_bonding
 *
 * @param[in] none
 *
 * @return none
 */
uint8_t *ble_flashbond_restore_data(uint8_t *para_data);


#ifdef __cplusplus
};
#endif

#endif  //(ifndef __BLE_BONDING_H__)
