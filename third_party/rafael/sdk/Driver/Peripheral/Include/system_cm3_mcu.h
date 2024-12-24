
/** @defgroup    System_cm3_mcu   System_CM3_MCU_Device
 *  @ingroup     peripheral_group
 *  @breif System_cm3_mcu header file
 *  @{
 *  @details   System Initialization header file for Cortex-M3 device based on CMSIS-CORE
*/
#ifndef SYSTEM_CM3MCU_H
#define SYSTEM_CM3MCU_H

#ifdef __cplusplus
extern "C" {
#endif



#include <stdint.h>

/*----------------------------------------------------------------------------
  Define PMU Mode
 *----------------------------------------------------------------------------*/
/*Define PMU mode type*/
#define PMU_LDO_MODE     0
#define PMU_DCDC_MODE    1

#ifndef SET_PMU_MODE
#define SET_PMU_MODE    PMU_DCDC_MODE
#endif

typedef enum
{
    PMU_MODE_LDO = 0,               /*!< System PMU LDO Mode */
    PMU_MODE_DCDC,                  /*!< System PMU DCDC Mode */
} pmu_mode_cfg_t;

/**
 * @brief Set the system PMU mode
 * @param[in] pmu_mode Specifies the system PMU mode
 *   This parameter can be the following values:
 *     @arg PMU_MODE_LDO: Specifies the system PMU LDO mode
 *     @arg PMU_MODE_DCDC: Specifies the system PMU DCDC mode
 * @return None
 */
void SystemPmuSetMode(pmu_mode_cfg_t pmu_mode);
extern uint32_t SystemFrequency;    /*!< System Clock Frequency (Core Clock)  */
extern uint32_t SystemCoreClock;    /*!< Processor Clock Frequency            */


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
void SystemInit(void);
/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
void SystemCoreClockUpdate(void);
/**
 * Update SystemFrequency variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemFrequency with current core Clock
 *         retrieved from clock mode.
 */

void SystemFrequencyUpdate(void);
/*@}*/ /* end of peripheral_group System_cm3_mcu Driver */

#ifdef __cplusplus
}
#endif

#endif /* end of SYSTEM_CM3MCU_H */


