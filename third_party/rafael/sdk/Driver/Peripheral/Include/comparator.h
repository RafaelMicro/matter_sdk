/** @file comparator.h
 *
 * @brief Comparator driver header file.
 *
 */

/**
 * @defgroup comp_group Comparator
 * @ingroup peripheral_group
 * @{
 * @brief Define Comparator definitions, structures, and functions.
 */

#ifndef __COMPARATOR_H__
#define __COMPARATOR_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

typedef enum
{
    COMP_CONFIG_SELREF_INTERNAL     = 0,   /**< Input N source internal bg_0.6v. */
    COMP_CONFIG_SELREF_EXTERNAL     = 1,   /**< Input N source external. */
} comp_config_selref_t;

typedef enum
{
    COMP_CONFIG_SELINPUT_INTERNAL   = 0,   /**< Input P source internal vdd div. */
    COMP_CONFIG_SELINPUT_EXTERNAL   = 1,   /**< Input P source external. */
} comp_config_selinput_t;

typedef enum
{
    COMP_CONFIG_REFSEL_CHANNEL_0   = 0,   /**< Select Comparator N input channel 0. */
    COMP_CONFIG_REFSEL_CHANNEL_1   = 1,   /**< Select Comparator N input channel 1. */
    COMP_CONFIG_REFSEL_CHANNEL_2   = 2,   /**< Select Comparator N input channel 2. */
    COMP_CONFIG_REFSEL_CHANNEL_3   = 3,   /**< Select Comparator N input channel 3. */
    COMP_CONFIG_REFSEL_CHANNEL_4   = 4,   /**< Select Comparator N input channel 4. */
    COMP_CONFIG_REFSEL_CHANNEL_5   = 5,   /**< Select Comparator N input channel 5. */
    COMP_CONFIG_REFSEL_CHANNEL_6   = 6,   /**< Select Comparator N input channel 6. */
    COMP_CONFIG_REFSEL_CHANNEL_7   = 7,   /**< Select Comparator N input channel 7. */
    COMP_CONFIG_REFSEL_IOLDO       = 8,   /**< Select Comparator N input IOLDO. */
    COMP_CONFIG_REFSEL_AVSS        = 9,   /**< Select Comparator N input AVSS. */
} comp_config_ref_sel_t;

typedef enum
{
    COMP_CONFIG_PW_SMALLEST   = 0,   /**< Comparator current select smallest. */
    COMP_CONFIG_PW_LARGEST    = 3,   /**< Comparator current select largest. */
} comp_config_pw_t;

typedef enum
{
    COMP_CONFIG_HYS_40   = 0,        /**< Comparator hys window select 40mV. */
    COMP_CONFIG_HYS_80   = 1,        /**< Comparator hys window select 80mV. */
    COMP_CONFIG_HYS_100  = 2,        /**< Comparator hys window select 100mV. */
    COMP_CONFIG_HYS_130  = 3,        /**< Comparator hys window select 130mV. */
} comp_config_hys_sel_t;

typedef enum
{
    COMP_CONFIG_SWDIV_MOS      = 0,   /**< Comparator switch vdd MOS div. */
    COMP_CONFIG_SWDIV_RES      = 1,   /**< Comparator switch vdd RES div. */
} comp_config_swdiv_t;

typedef enum
{
    COMP_CONFIG_DS_WAKEUP_DISABLE      = 0,   /**< Disable the Comparator wakeup in DeepSleep. */
    COMP_CONFIG_DS_WAKEUP_ENABLE       = 1,   /**< Enable the Comparator wakeup in DeepSleep. */
} comp_config_ds_wakeup_t;

typedef enum
{
    COMP_CONFIG_DS_INVERT_DISABLE      = 0,   /**< Disable the Comparator output invert for waking up from DeepSleep. */
    COMP_CONFIG_DS_INVERT_ENABLE       = 1,   /**< Enable the Comparator output invert for waking up from DeepSleep. */
} comp_config_ds_inv_t;

typedef enum
{
    COMP_CONFIG_PSRR_REGULAR      = 0,   /**< Comparator PSRR regular. */
    COMP_CONFIG_PSRR_HIGH_V       = 1,   /**< Comparator PSRR high V. */
} comp_config_psrr_t;

typedef enum
{
    COMP_CONFIG_TC_REGULAR      = 0,   /**< Comparator temperature coefficient regular. */
    COMP_CONFIG_TC_COMPENSATE   = 1,   /**< Comparator temperature coefficient compensate. */
} comp_config_tc_t;

typedef enum
{
    COMP_CONFIG_EN_START_0      = 0,
    COMP_CONFIG_EN_START_1      = 1,
    COMP_CONFIG_EN_START_2      = 2,
    COMP_CONFIG_EN_START_3      = 3,
} comp_config_en_start_t;

typedef enum
{
    COMP_CONFIG_INT_POL_RISING      = 0,   /**< Rising edge. */
    COMP_CONFIG_INT_POL_FALLING     = 1,   /**< Falling edge. */
    COMP_CONFIG_INT_POL_BOTH        = 2,   /**< Both edge. */
} comp_config_int_pol_t;

typedef enum
{
    COMP_CONFIG_CHSEL_CHANNEL_0      = 0,   /**< Select Comparator P input channel 0. */
    COMP_CONFIG_CHSEL_CHANNEL_1      = 1,   /**< Select Comparator P input channel 1. */
    COMP_CONFIG_CHSEL_CHANNEL_2      = 2,   /**< Select Comparator P input channel 2. */
    COMP_CONFIG_CHSEL_CHANNEL_3      = 3,   /**< Select Comparator P input channel 3. */
    COMP_CONFIG_CHSEL_CHANNEL_4      = 4,   /**< Select Comparator P input channel 4. */
    COMP_CONFIG_CHSEL_CHANNEL_5      = 5,   /**< Select Comparator P input channel 5. */
    COMP_CONFIG_CHSEL_CHANNEL_6      = 6,   /**< Select Comparator P input channel 6. */
    COMP_CONFIG_CHSEL_CHANNEL_7      = 7,   /**< Select Comparator P input channel 7. */
    COMP_CONFIG_CHSEL_AVDD_1V        = 8,   /**< Select Comparator P input AVDD 1V. */
    COMP_CONFIG_CHSEL_AVSS           = 9,   /**< Select Comparator P input AVSS. */
    COMP_CONFIG_CHSEL_CHANNEL_MAX    = 10,   /**< Max Channel 10. */
} comp_config_ch_sel_t;

typedef enum
{
    COMP_CONFIG_VOLTAGE_0      = 0,   /**< Select the Comparator internal vdd div voltage: Power Fail 1.78V. */
    COMP_CONFIG_VOLTAGE_1      = 1,   /**< Select the Comparator internal vdd div voltage: Power Fail 1.82V. */
    COMP_CONFIG_VOLTAGE_2      = 2,   /**< Select the Comparator internal vdd div voltage: Power Fail 1.85V. */
    COMP_CONFIG_VOLTAGE_3      = 3,   /**< Select the Comparator internal vdd div voltage: Power Fail 1.89V. */
    COMP_CONFIG_VOLTAGE_4      = 4,   /**< Select the Comparator internal vdd div voltage: Power Fail 1.93V. */
    COMP_CONFIG_VOLTAGE_5      = 5,   /**< Select the Comparator internal vdd div voltage: Power Fail 1.97V. */
    COMP_CONFIG_VOLTAGE_6      = 6,   /**< Select the Comparator internal vdd div voltage: Power Fail 2.01V. */
    COMP_CONFIG_VOLTAGE_7      = 7,   /**< Select the Comparator internal vdd div voltage: Power Fail 2.06V. */
    COMP_CONFIG_VOLTAGE_8      = 8,   /**< Select the Comparator internal vdd div voltage: Power Fail 2.10V. */
    COMP_CONFIG_VOLTAGE_9      = 9,   /**< Select the Comparator internal vdd div voltage: Power Fail 2.15V. */
    COMP_CONFIG_VOLTAGE_10     = 10,  /**< Select the Comparator internal vdd div voltage: Power Fail 2.20V. */
    COMP_CONFIG_VOLTAGE_11     = 11,  /**< Select the Comparator internal vdd div voltage: Power Fail 2.25V. */
    COMP_CONFIG_VOLTAGE_12     = 12,  /**< Select the Comparator internal vdd div voltage: Power Fail 2.31V. */
    COMP_CONFIG_VOLTAGE_13     = 13,  /**< Select the Comparator internal vdd div voltage: Power Fail 2.37V. */
    COMP_CONFIG_VOLTAGE_14     = 14,  /**< Select the Comparator internal vdd div voltage: Power Fail 2.43V. */
    COMP_CONFIG_VOLTAGE_15     = 15,  /**< Select the Comparator internal vdd div voltage: Power Fail 2.50V. */
    COMP_CONFIG_VOLTAGE_MAX    = 16,  /**< Max Voltage Level. */
} comp_config_v_sel_t;


/**
 * @brief Comparator configuration.
 */
typedef struct
{
    comp_config_selref_t      comp_selref;        /**< Comparator selref*/
    comp_config_selinput_t    comp_selinput;      /**< Comparator input P source select*/
    comp_config_ref_sel_t     comp_ref_sel;       /**< Comparator input N source select*/
    comp_config_pw_t          comp_pw;            /**< Comparator current select*/
    comp_config_hys_sel_t     comp_hys_sel;       /**< Comparator hys window select*/
    comp_config_swdiv_t       comp_swdiv;         /**< Comparator vdd div type switch*/
    comp_config_ds_wakeup_t   comp_ds_wakeup;     /**< Comparator wakeup in DeepSleep*/
    comp_config_ds_inv_t      comp_ds_inv;        /**< Comparator output invert for waking up from DeepSleep*/
    comp_config_psrr_t        comp_psrr;          /**< Comparator PSRR select*/
    comp_config_tc_t          comp_tc;            /**< Comparator temperature coefficient select*/
    comp_config_en_start_t    comp_en_start;      /**< Comparator en start select*/
    comp_config_int_pol_t     comp_int_pol;       /**< Comparator interrupt polarity*/
    comp_config_ch_sel_t      comp_ch_sel;        /**< Comparator AIO channel*/
    comp_config_v_sel_t       comp_v_sel;         /**< Comparator internal vdd div voltage*/
} comp_config_t;

/**
 * @brief User cb handler prototype.
 *
 * This function is called when the requested number of samples has been processed.
 *
 */
typedef void (*comp_isr_handler_t)(void);

/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/
#define COMP_ENABLE()                     (PMU->PMU_COMP0.bit.AUX_COMP_EN_NM = ENABLE)           /**< Enable the Comparator module*/
#define COMP_ENABLE_SP()                  (PMU->PMU_COMP0.bit.AUX_COMP_EN_SP = ENABLE)           /**< Enable the Comparator module at Sleep*/
#define COMP_ENABLE_DS()                  (PMU->PMU_COMP0.bit.AUX_COMP_EN_DS = ENABLE)           /**< Enable the Comparator module at DeepSleep*/
#define COMP_INT_ENABLE()                 (PMU->PMU_COMP0.bit.AUX_COMP_INT_EN = ENABLE)          /**< Enable the Comparator interrupt*/
#define COMP_INT_DISABLE()                (PMU->PMU_COMP0.bit.AUX_COMP_INT_EN = DISABLE)         /**< Disable the Comparator interrupt*/
#define COMP_INT_CLEAR()                  (PMU->PMU_COMP1.bit.AUX_COMP_INT_CLR = ENABLE)         /**< Clear the Comparator interrupt status*/
#define COMP_INT_STATUS_GET()             (PMU->PMU_COMP2.bit.AUX_COMP_INT_STA)                  /**< Return the Comparator interrupt status*/
#define COMP_OUT_GET()                    (PMU->PMU_COMP2.bit.AUX_COMP_OUT)                      /**< Return the Comparator output result*/
#define COMP_INT_POL(para_set)            (PMU->PMU_COMP0.bit.AUX_COMP_INT_POL = para_set)       /**< Set the Comparator interrupt polarity*/
#define COMP_CH_SEL(para_set)             (PMU->PMU_COMP0.bit.AUX_COMP_CHSEL = para_set)         /**< Select the Comparator P input for comparision*/
#define COMP_V_SEL(para_set)              (PMU->PMU_COMP0.bit.AUX_COMP_VSEL = para_set)          /**< Select the Comparator internal vdd div voltage to compare with selected AIO channel*/
#define COMP_V_SEL_GET()                  (PMU->PMU_COMP0.bit.AUX_COMP_VSEL)                     /**< Return the Comparator internal vdd div voltage to compare with selected AIO channel*/

#define COMP_SELREF_SEL(para_set)         (PMU->PMU_COMP0.bit.AUX_COMP_SELREF = para_set)        /**< Select the Comparator selref*/
#define COMP_SELINPUT_SEL(para_set)       (PMU->PMU_COMP0.bit.AUX_COMP_SELINPUT = para_set)      /**< Select the Comparator input P source*/
#define COMP_REF_SEL(para_set)            (PMU->PMU_COMP0.bit.AUX_COMP_REFSEL = para_set)        /**< Select the Comparator N input for comparision*/
#define COMP_PW_SEL(para_set)             (PMU->PMU_COMP0.bit.AUX_COMP_PW = para_set)            /**< Select the Comparator current*/
#define COMP_HYS_SEL(para_set)            (PMU->PMU_COMP0.bit.AUX_COMP_SELHYS = para_set)        /**< Select the Comparator hys window*/
#define COMP_SWDIV_SEL(para_set)          (PMU->PMU_COMP0.bit.AUX_COMP_SWDIV = para_set)         /**< Switch the Comparator vdd div type*/
#define COMP_DS_WAKEUP(para_set)          (PMU->PMU_COMP0.bit.AUX_COMP_DS_WAKEUP_EN = para_set)  /**< Select the Comparator wakeup in DeepSleep*/
#define COMP_DS_INV(para_set)             (PMU->PMU_COMP0.bit.AUX_COMP_DS_INV = para_set)        /**< Select the invert of the Comparator output for waking up from DeepSleep*/
#define COMP_PSRR_SEL(para_set)           (PMU->PMU_COMP0.bit.AUX_COMP_PSRR = para_set)          /**< Select the Comparator PSRR for constant Gm*/
#define COMP_TC_SEL(para_set)             (PMU->PMU_COMP0.bit.AUX_COMP_TC = para_set)            /**< Select the Comparator temperature coefficient*/
#define COMP_EN_START_SEL(para_set)       (PMU->PMU_COMP0.bit.AUX_EN_START = para_set)           /**< Select the Comparator en start*/

/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/
/**
 * @brief Comparator interrupt service routine callback for user application.
 * @param[in] comp_int_callback Comparator interrupt callback handler
 * @return None
 */
void Comp_Register_Int_Callback(comp_isr_handler_t comp_int_callback);

/**
 * @brief Enable the specified Comparator interrupts
 * @param None
 * @return None
 */
void Comp_Int_Enable(void);

/**
 * @brief Disable all Comparator interrupt(s)
 * @param None
 * @return None
 */
void Comp_Int_Disable(void);

/**
 * @brief Initinal the comparator config
 * @param[in] p_config
 *          \arg comp_selref        Comparator selref
 *          \arg comp_selinput      Comparator input P source select
 *          \arg comp_ref_sel       Comparator input N source select
 *          \arg comp_pw            Comparator current select
 *          \arg comp_hys_sel       Comparator hys window select
 *          \arg comp_swdiv         Comparator vdd div type switch
 *          \arg comp_ds_wakeup     Comparator wakeup in DeepSleep
 *          \arg comp_ds_inv        Comparator output invert for waking up from DeepSleep
 *          \arg comp_psrr          Comparator PSRR select
 *          \arg comp_tc            Comparator temperature coefficient select
 *          \arg comp_en_start      Comparator en start select
 *          \arg comp_int_pol       Comparator interrupt polarity
 *          \arg comp_ch_sel        Comparator AIO channel
 *          \arg comp_v_sel         Comparator internal vdd div voltage
 * @param[in] comp_int_callback    register comparator interrutp callback function
 * @return None
 */
uint32_t Comp_Init(comp_config_t *p_config, comp_isr_handler_t comp_int_callback);

/**
 * @brief Comparator Enable
 * @return None
 */
void Comp_Enable(void);
/**
 * @brief Comparator Enable At Sleep
 * @return None
 */
void Comp_Enable_At_Sleep(void);
/**
 * @brief Comparator Enable At DeepSleep
 * @return None
 */
void Comp_Enable_At_DeepSleep(void);

#endif /* end of _COMPARATOR_H_ */

/** @} */ /* end of peripheral group */
