/** @file compensate.h
 *
 * @brief Compensate driver file.
 *
 */
/**
* @defgroup sadc_group SADC Driver
* @ingroup peripheral_group
* @{
* @brief Define sadc definitions, structures, and functions.
*/
#ifndef __COMPENSATE_H__
#define __COMPENSATE_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
typedef enum
{
    TX_PWR_COMP_IDLE          = 0,
    TX_PWR_COMP_TEMPERATURE   = 1,
    TX_PWR_COMP_VBAT          = 2,
    TX_PWR_COMP_UPDATE        = 3,
    TX_PWR_0V_ADC             = 4,
    TX_PWR_VBAT               = 5,
} tx_pwr_comp_state_t;

typedef struct __attribute__((packed))
{
    int8_t  offset;
    uint8_t poly_gain;
    uint8_t pa_pw_pre;
}
tx_pwr_comp_element_t;


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/
/**
 * @brief Tx Power Compensation Initialization to enable SW timer for periodic compensation
 * @note Before initializing Tx Power Compensation, make sure that SADC has been configured and enabled
 * @param[in] xPeriodicTimeInSec Periodic time in second for Tx power compensation
 * @return None
 *
 */
void Tx_Power_Compensation_Init(uint32_t xPeriodicTimeInSec);

/**
 * @brief Tx Power Compensation De-initialization to disable SW timer for periodic compensation
 * @param None
 * @return None
 *
 */
void Tx_Power_Compensation_Deinit(void);

/**
 * @brief Tx Power Sadc Int Callback_Handler
 * @param None
 * @return None
 *
 */
void Tx_Power_Sadc_Int_Callback_Handler(sadc_cb_t *p_cb);
#endif /* end of __COMPENSATE_H__ */

/** @} */ /* End of Peripheral Group */

