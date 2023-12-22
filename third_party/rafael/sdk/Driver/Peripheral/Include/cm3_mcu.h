
/*
 * @defgroup peripheral_group Peripheral
 * @{
 * @brief Define all peripheral definitions, structures, and functions.
 * @}
 */

#ifndef CM3MCU_CM3_H
#define CM3MCU_CM3_H

#include "chip_define.h"


#ifdef __cplusplus
extern "C" {
#endif

/* @addtogroup CM3MCU_Definitions CM3MCU_CM3 Definitions
  This file defines all structures and symbols for CM3MCU:
    - peripheral base address
    - peripheral interrupt ID
    - Peripheral definitions
  @{
*/


/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/* @addtogroup CM3MCU_CMSIS Device CMSIS Definitions
  Configuration of the Cortex-M3 Processor and Core Peripherals
  @{
*/

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn_Type
{
    /******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
    NonMaskableInt_IRQn           = -14,    /*!<  2 Cortex-M3 Non Maskable Interrupt                 */
    HardFault_IRQn                = -13,    /*!<  3 Cortex-M3 Hard Fault Interrupt                   */
    MemoryManagement_IRQn         = -12,    /*!<  4 Cortex-M3 Memory Management Interrupt            */
    BusFault_IRQn                 = -11,    /*!<  5 Cortex-M3 Bus Fault Interrupt                    */
    UsageFault_IRQn               = -10,    /*!<  6 Cortex-M3 Usage Fault Interrupt                  */
    SVCall_IRQn                   = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                      */
    DebugMonitor_IRQn             = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                */
    PendSV_IRQn                   = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                      */
    SysTick_IRQn                  = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                  */

    /******  CM3MCU Specific Interrupt Numbers *******************************************************/
    Gpio_IRQn                     = 0,       /*!< GPIO combined Interrupt                          */
    Timer0_IRQn                   = 1,       /*!< TIMER 0 Interrupt                                */
    Timer1_IRQn                   = 2,       /*!< TIMER 1 Interrupt                                */
    Wdt_IRQn                      = 3,       /*!< WatchDog Interrupt                               */
    Uart0_IRQn                    = 4,       /*!< UART 0 Interrupt                                 */
    I2cm_IRQn                     = 5,       /*!< I2C Master Interrupt                             */
    FlashCtl_IRQn                 = 6,       /*!< FlashCtl Interrupt                               */
    Uart1_IRQn                    = 7,       /*!< UART 1 Interrupt                                 */
    Rtc_IRQn                      = 8,       /*!< RTC Interrupt                                    */
    Dma0_C0_IRQn                  = 9,       /*!< DMA0 Channel 0 Interrupt                         */
    Dma0_C1_IRQn                  = 10,      /*!< DMA0 Channel 1 Interrupt                         */
    Dma0_C2_IRQn                  = 11,      /*!< DMA0 Channel 2 Interrupt                         */
    Dma0_C3_IRQn                  = 12,      /*!< DMA0 Channel 3 Interrupt                         */
    Qspi0_IRQn                    = 13,      /*!< QSPI0 Interrupt                                  */
    I2s0_IRQn                     = 14,      /*!< I2S0  Interrupt                                  */
    Timer3_IRQn                   = 15,      /*!< TIMER 3 Interrupt                                */
    Timer4_IRQn                   = 16,      /*!< TIMER 4 Interrupt                                */
    Dma1_C2_IRQn                  = 17,      /*!< DMA1 Channel 2 Interrupt                         */
    Bod_IRQn                      = 18,      /*!< BOD Interrupt                                    */
    Uart2_IRQn                    = 19,      /*!< UART2 Interrupt                                  */
    CommSubsystem_IRQn            = 20,      /*!< COMM_SUBSYSTEM_COMM Interrupt                    */
    Pwm0_IRQn                     = 21,      /*!< PWM0 Interrupt                                   */
    Pwm1_IRQn                     = 22,      /*!< PWM1 Interrupt                                   */
    Pwm2_IRQn                     = 23,      /*!< PWM2 Interrupt                                   */
    Pwm3_IRQn                     = 24,      /*!< PWM3 Interrupt                                   */
    Pwm4_IRQn                     = 25,      /*!< PWM4 Interrupt                                   */
    Sadc_IRQn                     = 26,      /*!< SADC Interrupt                                   */
    Crypto_IRQn                   = 27,      /*!< Crypto  Interrupt                                */
    Timer2_IRQn                   = 28,      /*!< TIMER 2 Interrupt                                */
    Qspi1_IRQn                    = 29,      /*!< QSPI1   Interrupt                                */
    Soft_IRQn                     = 30,      /*!< SOFTWARE  Interrupt                              */
    Trng_IRQn                     = 31,      /*!< TRNG    Interrupt                                */
    Comp_IRQn                     = 32,      /*!< COMPARATOR Interrupt                             */
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __CM3_REV                 0x0201    /*!< Core Revision r2p1                             */
#define __NVIC_PRIO_BITS          3         /*!< Number of Bits used for Priority Levels        */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used   */
#define __MPU_PRESENT             1         /*!< MPU present or not                             */

/*@}*/ /* end of group CM3MCU_CMSIS */


#include "core_cm3.h"                     /* Cortex-M3 processor and core peripherals           */
#include "system_cm3_mcu.h"               /* CM3 MCU example System include file                */

/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/


#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#include "status.h"

#include "assert_help.h"
#include "sysfun.h"

#include <stdbool.h>

#include "gpio_reg.h"
#include "uart_reg.h"
#include "i2cm_reg.h"
#include "timer_reg.h"
#include "sysctrl_reg.h"
#include "wdt_reg.h"
#include "flashctl_reg.h"
#include "flashds.h"
#include "comm_subsystem_ahb_reg.h"
#include "qspi_reg.h"
#include "dma_reg.h"
#include "rtc_reg.h"
#include "i2s_reg.h"
#include "pwm_reg.h"

#include "sadc_reg.h"
#include "crypto_reg.h"
#include "pmu_reg.h"
#include "remap_reg.h"
#include "cachectl_reg.h"

#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif


#if defined ( __CC_ARM   )
#define __STATIC_INLINE  static __inline
#endif

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* @addtogroup MemoryMap  Memory Mapping
  @{
*/

/* Peripheral and SRAM base address */

//#define FLASH_BASE           (0x00000000UL)  /*!< (FLASH     ) Base Address */

#define SRAM_BASE            (0x20000000UL)  /*!< (SRAM      ) Base Address */
#define PERIPH_BASE          (0x40000000UL)  /*!< (Peripheral) Base Address */

#define PERIPH_BASE2         (0xA0000000UL)  /*!< (Peripheral2) Base Address */

#define GPIO_BASE            (PERIPH_BASE)
#define PMU_BASE             (PERIPH_BASE+0x200000UL)
#define REMAP_BASE           (PERIPH_BASE+0x500000UL)
#define RTC_BASE             (PERIPH_BASE+0x600000UL)
#define SYSCTRL_BASE         (PERIPH_BASE+0x800000UL)

#define UART0_BASE           (PERIPH_BASE2)
#define COMM_SUBSYSTEM_AHB_BASE       (PERIPH_BASE2+0x400000UL)
#define UART1_BASE           (PERIPH_BASE2+0x500000UL)
#define UART2_BASE           (PERIPH_BASE2+0x600000UL)

#define I2CM_BASE            (PERIPH_BASE2+0x100000UL)
#define CACHECTRL_BASE       (PERIPH_BASE2+0x200000UL)
#define FLASHCTRL_BASE       (PERIPH_BASE2+0x300000UL)

#define TIMER0_BASE          (PERIPH_BASE2+0x700000UL)
#define TIMER1_BASE          (PERIPH_BASE2+0x800000UL)
#define TIMER2_BASE          (PERIPH_BASE2+0xF00000UL)
#define TIMER3_BASE          (PERIPH_BASE2+0xF40000UL)
#define TIMER4_BASE          (PERIPH_BASE2+0xF80000UL)

#define WDT_BASE             (PERIPH_BASE2+0x900000UL)
#define SADC_BASE            (PERIPH_BASE2+0xD00000UL)

#define CRYPTO_BASE          (PERIPH_BASE2+0xE00000UL)


#define QSPI0_BASE           (0xB0000000UL)
#define QSPI1_BASE           (0x80000000UL)

#define DMA0_BASE            (0xC0000000UL)

#define DMA0_CH0_BASE        (DMA0_BASE)
#define DMA0_CH1_BASE        (DMA0_BASE+0x100UL)
#define DMA0_CH2_BASE        (DMA0_BASE+0x200UL)
#define DMA0_CH3_BASE        (DMA0_BASE+0x300UL)


#define I2S0_BASE            (PERIPH_BASE2+0xA00000UL)
#define XDMA_BASE            (PERIPH_BASE2+0xB00000UL)

#define PWM0_BASE            (PERIPH_BASE2+0xC00000UL)
#define PWM1_BASE            (PERIPH_BASE2+0xC00100UL)
#define PWM2_BASE            (PERIPH_BASE2+0xC00200UL)
#define PWM3_BASE            (PERIPH_BASE2+0xC00300UL)
#define PWM4_BASE            (PERIPH_BASE2+0xC00400UL)

/*@}*/ /* end of group MemoryMap */


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
/* @addtogroup PeripheralDecl CM3MCU_CM3 Peripheral Declaration
  @{
*/

#define GPIO                  ((GPIO_T *) GPIO_BASE)
#define RTC                   ((RTC_T *) RTC_BASE)
#define SYSCTRL               ((SYSCTRL_T *) SYSCTRL_BASE)


#define UART0                 ((UART_T *) UART0_BASE)
#define UART1                 ((UART_T *) UART1_BASE)
#define UART2                 ((UART_T *) UART2_BASE)

#define I2CM                  ((I2CM_T *) I2CM_BASE)

#define CACHE                 ((CACHECTL_T *) CACHECTRL_BASE)
#define FLASH                 ((FLASHCTL_T *) FLASHCTRL_BASE)

#define COMM_SUBSYSTEM_AHB    ((COMM_SUBSYSTEM_AHB_T *) COMM_SUBSYSTEM_AHB_BASE)
#define TIMER0                ((timern_t *) TIMER0_BASE)
#define TIMER1                ((timern_t *) TIMER1_BASE)
#define TIMER2                ((timern_t *) TIMER2_BASE)
#define TIMER3                ((timern_t *) TIMER3_BASE)
#define TIMER4                ((timern_t *) TIMER4_BASE)

#define WDT                   ((wdt_t *) WDT_BASE)


#define QSPI0                 ((QSPI_T *) QSPI0_BASE)
#define QSPI1                 ((QSPI_T *) QSPI1_BASE)

#define SPI0                  QSPI0
#define SPI1                  QSPI1

#define I2S0                  ((i2s_t *) I2S0_BASE)

#define PWM0                  ((pwm_t *) PWM0_BASE)
#define PWM1                  ((pwm_t *) PWM1_BASE)
#define PWM2                  ((pwm_t *) PWM2_BASE)
#define PWM3                  ((pwm_t *) PWM3_BASE)
#define PWM4                  ((pwm_t *) PWM4_BASE)

#define SADC                  ((sadc_t *) SADC_BASE)

#define CRYPTO                ((CRYPTO_T *) CRYPTO_BASE)

#define PMU                   ((pmu_t *) PMU_BASE)

#define REMAP                 ((remap_t *) REMAP_BASE)

/*@}*/ /* end of group PeripheralDecl */



/* @addtogroup IO_ROUTINE I/O Routines
  The Declaration of I/O Routines
  @{
 */

/**
  * @brief Set a 32-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 32-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  * @note The output port must be 32-bit aligned
  */
#define outp32(port,value)    *((volatile uint32_t *)(port)) = (value)

/**
  * @brief Get a 32-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 32-bit data from
  * @return  32-bit unsigned value stored in specified I/O port
  * @note The input port must be 32-bit aligned
  */
#define inp32(port)           (*((volatile uint32_t *)(port)))

/**
  * @brief Set a 16-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 16-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  * @note The output port must be 16-bit aligned
  */
#define outp16(port,value)    *((volatile uint16_t *)(port)) = (value)

/**
  * @brief Get a 16-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 16-bit data from
  * @return  16-bit unsigned value stored in specified I/O port
  * @note The input port must be 16-bit aligned
  */
#define inp16(port)           (*((volatile uint16_t *)(port)))

/**
  * @brief Set a 8-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 8-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  */
#define outp8(port,value)     *((volatile uint8_t *)(port)) = (value)

/**
  * @brief Get a 8-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 8-bit data from
  * @return  8-bit unsigned value stored in specified I/O port
  */
#define inp8(port)            (*((volatile uint8_t *)(port)))


/*@}*/ /* end of group IO_ROUTINE */

/******************************************************************************/
/*                Legacy Constants                                            */
/******************************************************************************/
/* @addtogroup Legacy_Constants Legacy Constants
  Legacy Constants
  @{
*/

#ifndef NULL
#define NULL           (0)      ///< NULL pointer
#endif

#define TRUE           (1UL)      ///< Boolean true, define to use in API parameters or return value
#define FALSE          (0UL)      ///< Boolean false, define to use in API parameters or return value

#define ENABLE         (1UL)      ///< Enable, define to use in API parameters
#define DISABLE        (0UL)      ///< Disable, define to use in API parameters

/* Define one bit mask */
#define BIT0     (0x00000001UL)       ///< Bit 0 mask of an 32 bit integer
#define BIT1     (0x00000002UL)       ///< Bit 1 mask of an 32 bit integer
#define BIT2     (0x00000004UL)       ///< Bit 2 mask of an 32 bit integer
#define BIT3     (0x00000008UL)       ///< Bit 3 mask of an 32 bit integer
#define BIT4     (0x00000010UL)       ///< Bit 4 mask of an 32 bit integer
#define BIT5     (0x00000020UL)       ///< Bit 5 mask of an 32 bit integer
#define BIT6     (0x00000040UL)       ///< Bit 6 mask of an 32 bit integer
#define BIT7     (0x00000080UL)       ///< Bit 7 mask of an 32 bit integer
#define BIT8     (0x00000100UL)       ///< Bit 8 mask of an 32 bit integer
#define BIT9     (0x00000200UL)       ///< Bit 9 mask of an 32 bit integer
#define BIT10    (0x00000400UL)       ///< Bit 10 mask of an 32 bit integer
#define BIT11    (0x00000800UL)       ///< Bit 11 mask of an 32 bit integer
#define BIT12    (0x00001000UL)       ///< Bit 12 mask of an 32 bit integer
#define BIT13    (0x00002000UL)       ///< Bit 13 mask of an 32 bit integer
#define BIT14    (0x00004000UL)       ///< Bit 14 mask of an 32 bit integer
#define BIT15    (0x00008000UL)       ///< Bit 15 mask of an 32 bit integer
#define BIT16    (0x00010000UL)       ///< Bit 16 mask of an 32 bit integer
#define BIT17    (0x00020000UL)       ///< Bit 17 mask of an 32 bit integer
#define BIT18    (0x00040000UL)       ///< Bit 18 mask of an 32 bit integer
#define BIT19    (0x00080000UL)       ///< Bit 19 mask of an 32 bit integer
#define BIT20    (0x00100000UL)       ///< Bit 20 mask of an 32 bit integer
#define BIT21    (0x00200000UL)       ///< Bit 21 mask of an 32 bit integer
#define BIT22    (0x00400000UL)       ///< Bit 22 mask of an 32 bit integer
#define BIT23    (0x00800000UL)       ///< Bit 23 mask of an 32 bit integer
#define BIT24    (0x01000000UL)       ///< Bit 24 mask of an 32 bit integer
#define BIT25    (0x02000000UL)       ///< Bit 25 mask of an 32 bit integer
#define BIT26    (0x04000000UL)       ///< Bit 26 mask of an 32 bit integer
#define BIT27    (0x08000000UL)       ///< Bit 27 mask of an 32 bit integer
#define BIT28    (0x10000000UL)       ///< Bit 28 mask of an 32 bit integer
#define BIT29    (0x20000000UL)       ///< Bit 29 mask of an 32 bit integer
#define BIT30    (0x40000000UL)       ///< Bit 30 mask of an 32 bit integer
#define BIT31    (0x80000000UL)       ///< Bit 31 mask of an 32 bit integer

/* Byte Mask Definitions */
#define BYTE0_Msk              (0x000000FFUL)         ///< Mask to get bit0~bit7 from a 32 bit integer
#define BYTE1_Msk              (0x0000FF00UL)         ///< Mask to get bit8~bit15 from a 32 bit integer
#define BYTE2_Msk              (0x00FF0000UL)         ///< Mask to get bit16~bit23 from a 32 bit integer
#define BYTE3_Msk              (0xFF000000UL)         ///< Mask to get bit24~bit31 from a 32 bit integer

#define GET_BYTE0(u32Param)    (((u32Param) & BYTE0_Msk)      )  /*!< Extract Byte 0 (Bit  0~ 7) from parameter u32Param */
#define GET_BYTE1(u32Param)    (((u32Param) & BYTE1_Msk) >>  8)  /*!< Extract Byte 1 (Bit  8~15) from parameter u32Param */
#define GET_BYTE2(u32Param)    (((u32Param) & BYTE2_Msk) >> 16)  /*!< Extract Byte 2 (Bit 16~23) from parameter u32Param */
#define GET_BYTE3(u32Param)    (((u32Param) & BYTE3_Msk) >> 24)  /*!< Extract Byte 3 (Bit 24~31) from parameter u32Param */

/*@}*/ /* end of group Legacy_Constants */

/************************************************************************/
/*              Peripheral Driver API Header                            */
/************************************************************************/



#include "gpio.h"

#include "i2c_master.h"
#include "flashctl.h"
#include "timer.h"
#include "sysctrl.h"

#include "wdt.h"
#include "qspi.h"

#include "dma.h"
#include "rtc.h"

#include "i2s.h"
#include "pwm.h"
#include "sadc.h"
#include "comparator.h"
#include "swi.h"
#include "lpm.h"


#include "flash_protect.h"

/*set debug port as uart0*/
#define  DEBUGPORT            0

/*This is a software breakpoint, which will stop the firmware when using ICE debugging*/
#define BREAK()     do{ __asm volatile ("BKPT    #0"); while(1);} while(0)

#define ASSERT()    do{ while(1);} while(0)


/*@}*/ /* end of group CM3MCU_Definitions */

#ifdef __cplusplus
}
#endif

#endif  /* CM3MCU_CM3_H */
