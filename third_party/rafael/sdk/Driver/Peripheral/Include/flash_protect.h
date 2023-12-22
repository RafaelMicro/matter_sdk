/**************************************************************************//**
 * @file     flash protect
 .h
 * @version
 * @brief    Flash protect header file
 *
 * @copyright
 ******************************************************************************/
/** @defgroup Flash_protect Flash
*  @ingroup  peripheral_group
*  @{
*  @brief  FLASH_protect information
*/
#include "chip_define.h"
#include "project_config.h"

#ifndef ___FLASH_PROTECT_H__
#define ___FLASH_PROTECT_H__

#ifdef __cplusplus
extern "C"
{
#endif


extern volatile uint32_t flash_protect;

/**
 * @brief set voltage comparator when the vat voltage low than 2.2v will set flash_protct variable true
 *        let the flash control function can't read/write data
 *
 * @param[in] NONE
 *
 * @retval    NONE
 * @retval    NONE
 *
 */
void flash_cmp_protect_init(void);
/**
 * @brief read sadc vbat voltage to check vbat voltage
 *
 * @param[in] NONE
 *
 * @retval    NONE
 * @retval    NONE
 *
 */
void flash_vbat_protect_init(void);



/*@}*/ /* end of peripheral_group Flash_Drver */

#ifdef __cplusplus
}
#endif

#endif /* end of ___FLASHCTL_H__ */


