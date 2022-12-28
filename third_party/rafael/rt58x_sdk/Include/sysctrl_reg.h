/**************************************************************************//**
 * @file     sysctrl_reg.h
 * @version
 * @brief    SYS CONTROL register header file
 *
 * @copyright
 ******************************************************************************/
#ifndef ___SYSCTRL_REG_H__
#define ___SYSCTRL_REG_H__

#if defined ( __CC_ARM )
#pragma anon_unions
#endif


typedef struct
{
    __IO uint32_t POWER_STATE_CTRL;
    __IO uint32_t SYS_CLK_CTRL;        /*0x04*/
    __IO uint32_t SYS_LOWPOWER_CTRL;   /*0x08*/
    __I  uint32_t RESV;
    __IO uint32_t GPIO_MAP0;           /*0x10*/
    __IO uint32_t GPIO_MAP1;           /*0x14*/
    __IO uint32_t GPIO_MAP2;           /*0x18*/
    __IO uint32_t GPIO_MAP3;           /*0x1C*/

    __IO uint32_t GPIO_PULL_CTRL[4];    /*0x20 ~ 0x2C*/
    __IO uint32_t GPIO_DRV_CTRL[2];     /*0x30 ~ 0x34*/
    __IO uint32_t GPIO_OD_CTRL;         /*0x38*/
    __IO uint32_t GPIO_AIO_CTRL;        /*0x3C*/
    __IO uint32_t TRNG0;                /*0x40*/
    __IO uint32_t TRNG1;                /*0x44*/
    __IO uint32_t TRNG2;                /*0x48*/
    __IO uint32_t TRNG3;                /*0x4C*/
    __IO uint32_t SRAM_SD_NM;           /*0x50*/
    __IO uint32_t SRAM_SD_SL;           /*0x54*/
    __I  uint32_t RESV2[2];             /*0x58 0x5C*/
    __IO uint32_t SYS_SCRATCH[8];       /*0x60-0x7C*/
    __IO uint32_t DEEPSLEEP_WAKEUP;     /*0x80*/
    __IO uint32_t DEEPSLEEP_INV;        /*0x84*/
    __I  uint32_t RESV3[2];
    __IO uint32_t CHIP_INFO;            /*0x90*/
} SYSCTRL_T;



#define     HCLK_SEL_MASK            (3<<0)
#define     HCLK_SEL_32M             (0)
#define     HCLK_SEL_BASEBAND_PLL    (3)


#define     HCLK_SLEEP_SELECT_SHIFT   4
#define     SLEEP_CLOCK_SEL_MASK      (3<<HCLK_SLEEP_SELECT_SHIFT)

#define     HCLK_SLEEP_DISABLE        0
#define     HCLK_SLEEP_RUN_IN_32K     (1<<HCLK_SLEEP_SELECT_SHIFT)

#define     PERCLK_SLEEP_DISABLE      0
#define     PERCLK_SLEEP_RUN_IN_32K   (1<<5)

#define     BASEBAND_PLL_EN_SHIFT     8
#define     BASEBAND_PLL_MASK         (1<<BASEBAND_PLL_EN_SHIFT)

#define     BASEBAND_PLL_ENABLE       (1<<BASEBAND_PLL_EN_SHIFT)
#define     BASEBAND_PLL_DISABLE      (0<<BASEBAND_PLL_EN_SHIFT)

#define     PLL_FREQ_MASK                   (3<<BASEBAND_PLL_EN_SHIFT)
#define     PLL_RUN_IN_64M                   (3<<BASEBAND_PLL_EN_SHIFT)
#define     PLL_RUN_IN_48M                   (1<<BASEBAND_PLL_EN_SHIFT)


#define     BASEBAND_PLL_FREQ_SHIFT   9
#define     BASEBAND_PLL_FREQ_MASK    (1<<BASEBAND_PLL_FREQ_SHIFT)

#define     BASEBAND_PLL_64M          (1<<BASEBAND_PLL_FREQ_SHIFT)
#define     BASEBAND_PLL_48M          (0<<BASEBAND_PLL_FREQ_SHIFT)
#define     BASEBAND_PLL_32M          (0)

#define     BASEBAND_PLL_FREQ_48M     (0<<BASEBAND_PLL_FREQ_SHIFT)
#define     BASEBAND_PLL_FREQ_64M     (1<<BASEBAND_PLL_FREQ_SHIFT)

#define     CK32_TIMER3_CLOCK_SHIFT    26
#define     CK32_TIMER3_CLOCK_MASK     (1<<CK32_TIMER3_CLOCK_SHIFT)
#define     CK32_TIMER4_CLOCK_SHIFT    27
#define     CK32_TIMER4_CLOCK_MASK     (1<<CK32_TIMER4_CLOCK_SHIFT)

#define     UART0_CLK                 16
#define     UART1_CLK                 17
#define     UART2_CLK                 18
#define     I2CM_CLK                  19
#define     QSPI0_CLK                 20
#define     QSPI1_CLK                 21
#define     TIMER0_CLK_ENABLE         22
#define     TIMER1_CLK                23
#define     TIMER2_CLK                24
#define     I2S_CLK                   25


#define     UART0_SLEEP_WAKE_EN_SHIFT   16
#define     UART0_SLEEP_WAKE_EN_MASK    (1<<UART0_SLEEP_WAKE_EN_SHIFT)
#define     UART0_SLEEP_WAKE_ENABLE     (1<<UART0_SLEEP_WAKE_EN_SHIFT)
#define     UART0_SLEEP_WAKE_DISABLE    (0)

#define     UART1_SLEEP_WAKE_EN_SHIFT   17
#define     UART1_SLEEP_WAKE_EN_MASK    (1<<UART1_SLEEP_WAKE_EN_SHIFT)
#define     UART1_SLEEP_WAKE_ENABLE     (1<<UART1_SLEEP_WAKE_EN_SHIFT)
#define     UART1_SLEEP_WAKE_DISABLE    (0)

#define     UART2_SLEEP_WAKE_EN_SHIFT   18
#define     UART2_SLEEP_WAKE_EN_MASK    (1<<UART2_SLEEP_WAKE_EN_SHIFT)
#define     UART2_SLEEP_WAKE_ENABLE     (1<<UART2_SLEEP_WAKE_EN_SHIFT)
#define     UART2_SLEEP_WAKE_DISABLE    (0)


#define     EN_HCLK_FROZEN_SHIFT        28
#define     HCLK_FROZEN_MASK            (1<<EN_HCLK_FROZEN_SHIFT)
#define     HCLK_FROZEN_ENABLE          (1<<EN_HCLK_FROZEN_SHIFT)
#define     HCLK_FROZEN_DISABLE         (0)

#define     EN_RTC_PCLK_DS_SHIFT        29
#define     RTC_PCLK_DS_MASK            (1<<EN_RTC_PCLK_DS_SHIFT)
#define     RTC_PCLK_DS_ENABLE          (1<<EN_RTC_PCLK_DS_SHIFT)
#define     RTC_PCLK_DS_DISABLE         (0)


#define     ENTER_SLEEP                 (1)
#define     ENTER_DEEP_SLEEP            (2)


#define     SLOW_CLOCK_SEL_SHIFT        6
#define     SLOW_CLOCK_SEL_MASK         (0x3<<SLOW_CLOCK_SEL_SHIFT)

#define     SLOW_CLOCK_INTERNAL         0
#define     SLOW_CLOCK_FROM_GPIO        1
#define     SLOW_CLOCK_32M_DIV_1000     2
#define     SLOW_CLOCK_XO_32K           3


#define     SYS_SCRATCH_OFFSET       (0x60)


#define     LOWPOWER_SRAM_DS_AUTO_SHIFT   7
#define     LOWPOWER_SRAM_DS_AUTO_MASK    (1<<LOWPOWER_SRAM_DS_AUTO_SHIFT)
#define     LOWPOWER_SRAM_DS_AUTO_ENABLE  (1<<LOWPOWER_SRAM_DS_AUTO_SHIFT)
#define     LOWPOWER_SRAM_DS_AUTO_DISABLE (0)


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define MAP_BASE         (SYSCTRL_BASE + 0x10)
#define PULLOPT_BASE     (SYSCTRL_BASE + 0x20)
#define DRV_BASE         (SYSCTRL_BASE + 0x30)
#define OD_BASE          (SYSCTRL_BASE + 0x38)



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

#endif
