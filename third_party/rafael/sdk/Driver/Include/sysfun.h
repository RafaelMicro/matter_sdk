/**************************************************************************//**
 * @file     sysfun.h
 * @version
 * @brief    SYSTEM HELP FUNCTION API DEFINITION
 *
 * @copyright
 ******************************************************************************/

#ifndef ___SYSFUN_H__
#define ___SYSFUN_H__

/** @defgroup SYSFUN_Driver SYSTEM FUNTION Driver
 *  @ingroup  peripheral_group
 *  @{
 *  @brief  System function header information
 */
#define  OTP_IC_VERSION_OFFSET      0x00
#define  OTP_FT_VERSION_OFFSET      0x04
/**
* @brief chip model version definitions.
*/
typedef enum
{
    CHIP_TYPE_581 = 0x01,            /*!<ic type 581 */
    CHIP_TYPE_582 = 0x02,            /*!<ic type 582 */
    CHIP_TYPE_583 = 0x03,            /*!<ic type 583 */
    CHIP_TYPE_584 = 0x04,            /*!<ic type 584 */
    CHIP_TYPE_UNKNOW = 0xFF,
} chip_type_t;


typedef enum
{
    CHIP_VERSION_SHUTTLE = 0x00,                     /*!<ic type SHUTTLE */
    CHIP_VERSION_MPA = 0x01,                         /*!<ic type MPA */
    CHIP_VERSION_MPB = 0x02,                   /*!<ic type MPB */
    CHIP_VERSION_UNKNOW = 0xFF,
} chip_version_t;

typedef struct __attribute__((packed))
{
    chip_type_t     type;
    chip_version_t  version;
}
chip_model_t;

typedef struct __attribute__((packed))
{
    uint8_t  buf[8];

}
otp_version_t;
/**
 * @brief   Get chip mode
 * @details
 *          Get otp version chip_define.h
 * @retval  return chip_mode_t.
 *
 */
chip_model_t GetOtpVersion(void);
/**
* @brief   Sys_Software_Reset
* @details The function waits until the flash operation is complete and then resets the system.
*
*/
void Sys_Software_Reset(void);
/**
* @brief   critical_section_init
* @details This function clear critical_section_counter;
*
*/
void critical_section_init(void);
/**
* @brief   enter critical sections
* @details This function is nest function, that is, system call this function several times.
*           This function will mask all interrupt , except non-mask interrupt.
*           So as short as possible for using this function.
*
*/
void enter_critical_section(void);

/**
 * @brief   leave critical sections
 * @details Because enter critical sections is nest allowed.
 *           So it will only unmask all inerrupt when the number "enter_critical_section"
 *           equals "leave_critical_section" times.
 *           Please be careful design your code when calling enter_critical_section/leave_critical_section.
 *           One enter_critical_section must have one corresponding leave_critical_section!
 *
 */
void leave_critical_section(void);

/**
 * @brief   check hardware chip version and software defined version compared value.
 * @details
 *           version_check is help function to check
 *           software project setting is the same as hardware IC version.
 *           If software project define "CHIP_VERSION" is
 *           not matched with hardware IC version, this functio will return 0, otherwise 1.
 * @return
 * @retval    0 --- hardware and system defined matched.
 * @retval    1 --- hardware and system defined mis-matched.
 */

uint32_t version_check(void);
/**
 * @brief   check system pmu mode
 * @details
 *            return the system config pmu  mode
 * @return
 * @retval    0 --- LDO MODE
 * @retval    1 --- DCDC Mode
 */
pmu_mode_cfg_t GetPmuMode(void);

typedef enum
{
    IRQ_PRIORITY_HIGHEST = 0,
    IRQ_PRIORITY_HIGH = 1,
    IRQ_PRIORITY_NORMAL = 3,
    IRQ_PRIORITY_LOW = 5,
    IRQ_PRIORITY_LOWEST = 7,
} irq_priority_t;



#endif
