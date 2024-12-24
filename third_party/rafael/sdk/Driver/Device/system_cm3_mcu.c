
#include <stdint.h>
#include "cm3_mcu.h"

#include "project_config.h"
#include "mp_sector.h"

/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/
/**
 * @brief Register bit write structure.
 */
typedef struct
{
    uint32_t reg_addr;        /**< Register address to be written*/
    uint8_t start_bit;        /**< Register start bit to be written*/
    uint8_t bit_num;          /**< Register bit number to be written*/
    uint32_t reg_value;       /**< The value of register to be written*/
} reg_bit_write_t;


/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#ifndef SET_SYS_CLK
#define SET_SYS_CLK    SYS_CLK_48MHZ
#endif


#if (SET_SYS_CLK == SYS_CLK_32MHZ)
#define XTAL    (32000000UL)            /* Oscillator frequency               */
#elif (SET_SYS_CLK == SYS_CLK_48MHZ)
#define XTAL    (48000000UL)            /* Oscillator frequency               */
#elif (SET_SYS_CLK == SYS_CLK_64MHZ)
#define XTAL    (64000000UL)            /* Oscillator frequency               */
#endif





/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemFrequency = XTAL;    /*!< System Clock Frequency (Core Clock)  */
uint32_t SystemCoreClock = XTAL;    /*!< Processor Clock Frequency            */

/*----------------------------------------------------------------------------
  PMU Initial Table definitions
 *----------------------------------------------------------------------------*/
#if (SET_PMU_MODE == PMU_LDO_MODE)
const reg_bit_write_t pmu_ap_init_table_ldo[] =
{
    { 0x402000C4, 24, 1, 0 },
    { 0x402000C4, 25, 1, 0 },
    //    { 0x402000B0, 26, 1, 0 },
    { 0x402000B0, 26, 1, 1 },
};

const reg_bit_write_t pmu_mp_init_table_ldo[] =
{
    { 0x40200090, 16, 4, 10 },
    { 0x40200090, 20, 4, 10 },
    { 0x40200090, 24, 4, 10 },
    { 0x40200094, 0, 4, 0 },
    { 0x40200094, 4, 4, 0 },
    { 0x40200094, 8, 4, 0 },
    { 0x40200098, 0, 4, 7 },
    { 0x40200098, 4, 4, 7 },
    { 0x40200098, 8, 4, 7 },
    { 0x40200098, 16, 3, 0 },
    { 0x40200098, 20, 3, 0 },
    { 0x40200098, 24, 3, 0 },
};

#elif (SET_PMU_MODE == PMU_DCDC_MODE)
const reg_bit_write_t pmu_ap_init_table_dcdc[] =
{
    { 0x402000C4, 24, 1, 0 },
    { 0x402000C4, 25, 1, 0 },
    //    { 0x402000B0, 26, 1, 0 },
    { 0x402000B0, 26, 1, 1 },
};

const reg_bit_write_t pmu_mp_init_table_dcdc[] =
{
    { 0x40200090, 0, 4, 10 },
    { 0x40200090, 4, 4, 10 },
    { 0x40200090, 8, 4, 10 },
    { 0x40200090, 16, 4, 10 },
    { 0x40200090, 20, 4, 10 },
    { 0x40200090, 24, 4, 10 },
    { 0x40200094, 0, 4, 0 },
    { 0x40200094, 4, 4, 0 },
    { 0x40200094, 8, 4, 0 },
    { 0x40200098, 0, 4, 7 },
    { 0x40200098, 4, 4, 7 },
    { 0x40200098, 8, 4, 7 },
    { 0x40200098, 16, 3, 0 },
    { 0x40200098, 20, 3, 0 },
    { 0x40200098, 24, 3, 0 },
};

#endif


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate(void)             /* Get Core Clock Frequency      */
{
    SystemCoreClock = XTAL;
}

void SystemFrequencyUpdate(void)             /* Update System clock Frequency */
{
    sys_clk_sel_t sys_clk_mode;

    sys_clk_mode = Get_Ahb_System_Clk();

    if (sys_clk_mode == SYS_CLK_32MHZ)
    {
        SystemFrequency = 32000000;
    }
    else if (sys_clk_mode == SYS_CLK_48MHZ)
    {
        SystemFrequency = 48000000;
    }
    else if (sys_clk_mode == SYS_CLK_64MHZ)
    {
        SystemFrequency = 64000000;
    }
    else
    {
        SystemFrequency = 32000000;
    }
}
/*----------------------------------------------------------------------------
  PMU settings in LDO mode
 *----------------------------------------------------------------------------*/
#if ((CHIP_VERSION == RT58X_MPA) || (CHIP_VERSION == RT58X_MPB))
void Reg_Bit_Write(reg_bit_write_t reg_write_item)
{
    uint32_t bit_max_value;
    uint32_t reg_value_buf;

    bit_max_value = ((1 << reg_write_item.bit_num) - 1);

    reg_value_buf = *((uint32_t *)reg_write_item.reg_addr);
    reg_value_buf   &= (~(bit_max_value << reg_write_item.start_bit));
    reg_value_buf |= ((reg_write_item.reg_value & bit_max_value) << reg_write_item.start_bit);
    *((uint32_t *)(reg_write_item.reg_addr)) = reg_value_buf;
}


void SystemPmuUpdateDig(void)              /* Update PMU settings for digital configuration */
{
    PMU->PMU_XTAL.bit.CFG_XTAL_SETTLE_TIME = 0x3F;
    PMU->PMU_XTAL.bit.CFG_XTAL_CAP_SEL = 0x191;
    SYSCTRL->SYS_LOWPOWER_CTRL = (SYSCTRL->SYS_LOWPOWER_CTRL & ~LOWPOWER_SRAM_DS_AUTO_MASK) | LOWPOWER_SRAM_DS_AUTO_ENABLE;
    PMU->PMU_XTAL.bit.CFG_BYPASS_XTAL_SETTLE = 1;
    PMU->PMU_CLK_CTRL.bit.CFG_CHIP_EN_AUTO = 1;
    PMU->PMU_PWR_CTRL.bit.CFG_BYP_XBUF_LDO = 1;

    SYSCTRL->GPIO_AIO_CTRL = ((SYSCTRL->GPIO_AIO_CTRL & ~FLASH_DRV_SEL_MASK) | FLASH_DRV_SEL_SET);

}


void SystemPmuSetMode(pmu_mode_cfg_t pmu_mode)
{
    if (pmu_mode == PMU_MODE_DCDC)
    {
        PMU->PMU_EN_CTRL.bit.EN_LLDO_NM = 1;
        PMU->PMU_EN_CTRL.bit.EN_DCDC_NM = 1;
        PMU->PMU_EN_CTRL.bit.EN_LLDO_NM = 0;
    }
    else if (pmu_mode == PMU_MODE_LDO)
    {
        PMU->PMU_EN_CTRL.bit.EN_LLDO_NM = 1;
        PMU->PMU_EN_CTRL.bit.EN_DCDC_NM = 1;
        PMU->PMU_EN_CTRL.bit.EN_DCDC_NM = 0;
    }
    else //default dcdc mode
    {
        PMU->PMU_EN_CTRL.bit.EN_LLDO_NM = 1;
        PMU->PMU_EN_CTRL.bit.EN_DCDC_NM = 1;
        PMU->PMU_EN_CTRL.bit.EN_LLDO_NM = 0;
    }
}

#if (SET_PMU_MODE == PMU_LDO_MODE)
void SystemPmuUpdateLdo(void)              /* Update PMU settings in LDO mode */
{
    uint32_t reg_num = 0;
    reg_bit_write_t reg_write_item;
    uint32_t i = 0;

    pmu_other_ctrl_t pmu_other_ctrl_buf;
    pmu_en_ctrl_t pmu_en_ctrl_buf;

    pmu_other_ctrl_buf = PMU->PMU_OTHER_CTRL;
    pmu_other_ctrl_buf.bit.LLDO_SIN = 2;
    pmu_other_ctrl_buf.bit.LLDO_LOUT = 0;
    pmu_other_ctrl_buf.bit.LLDO_IOC = 4;
    pmu_other_ctrl_buf.bit.IOLDO_SIN = 2;
    pmu_other_ctrl_buf.bit.IOLDO_LOUT = 1;
    pmu_other_ctrl_buf.bit.IOLDO_IOC = 6;
    pmu_other_ctrl_buf.bit.IOLDO_RET_SIN = 3;
    pmu_other_ctrl_buf.bit.IOLDO_RET_LOUT = 0;
    PMU->PMU_OTHER_CTRL = pmu_other_ctrl_buf;

    pmu_en_ctrl_buf = PMU->PMU_EN_CTRL;
    pmu_en_ctrl_buf.bit.UVH_DISABLE = 1;
    pmu_en_ctrl_buf.bit.EN_BG1_NM = 0;
    PMU->PMU_EN_CTRL = pmu_en_ctrl_buf;

    PMU->PMU_BOD_SEL.bit.POR_VTH = 3;
    PMU->PMU_BBPLL.bit.UVL_VTH = 3;

    PMU->PMU_RCO32K.bit.RN_32K = 2047;
    PMU->PMU_RCO32K.bit.PW_32K = 3;

    SystemPmuSetMode(PMU_MODE_LDO);

    reg_num = sizeof(pmu_ap_init_table_ldo) / sizeof(reg_bit_write_t);

    for (i = 0; i < reg_num; i++)
    {
        reg_write_item = pmu_ap_init_table_ldo[i];
        Reg_Bit_Write(reg_write_item);
    }

    reg_num = sizeof(pmu_mp_init_table_ldo) / sizeof(reg_bit_write_t);

    for (i = 0; i < reg_num; i++)
    {
        reg_write_item = pmu_mp_init_table_ldo[i];
        Reg_Bit_Write(reg_write_item);
    }
}


#elif (SET_PMU_MODE == PMU_DCDC_MODE)
void SystemPmuUpdateDcdc(void)              /* Update PMU settings in DCDC mode */
{
    uint32_t reg_num = 0;
    reg_bit_write_t reg_write_item;
    uint32_t i = 0;
    uint32_t chip_version = (get_chip_version() >> 4) & 0x0F; //bit4~bit7 chip version

    pmu_other_ctrl_t pmu_other_ctrl_buf;
    pmu_en_ctrl_t pmu_en_ctrl_buf;
    pmu_dcdc_ctrl0_t pmu_dcdc_ctrl0_buf;
    pmu_dcdc_ctrl1_t pmu_dcdc_ctrl1_buf;

    pmu_other_ctrl_buf = PMU->PMU_OTHER_CTRL;
    pmu_other_ctrl_buf.bit.LLDO_SIN = 2;
    pmu_other_ctrl_buf.bit.LLDO_LOUT = 0;
    pmu_other_ctrl_buf.bit.LLDO_IOC = 5;
    pmu_other_ctrl_buf.bit.IOLDO_SIN = 2;
    pmu_other_ctrl_buf.bit.IOLDO_LOUT = 0;
    pmu_other_ctrl_buf.bit.IOLDO_IOC = 6;
    pmu_other_ctrl_buf.bit.IOLDO_RET_SIN = 3;
    pmu_other_ctrl_buf.bit.IOLDO_RET_LOUT = 0;
    //===
    pmu_other_ctrl_buf.bit.DCDC_SW_HEAVY = 0;
    pmu_other_ctrl_buf.bit.DCDC_AUTO_HEAVY = 1;

    PMU->PMU_OTHER_CTRL = pmu_other_ctrl_buf;

    pmu_en_ctrl_buf = PMU->PMU_EN_CTRL;
    pmu_en_ctrl_buf.bit.UVH_DISABLE = 1;
    pmu_en_ctrl_buf.bit.EN_BG1_NM = 0;
    PMU->PMU_EN_CTRL = pmu_en_ctrl_buf;

    PMU->PMU_BOD_SEL.bit.POR_VTH = 0;

    if (chip_version == RT58X_MPB)
    {
        PMU->PMU_BBPLL.bit.BBPLL_HI = 1;
    }
    else
    {
        PMU->PMU_BBPLL.bit.BBPLL_HI = 3;
    }

    if (flash_get_deviceinfo() == RT582_FLASH_TYPE)
    {
        PMU->PMU_BOD_SEL.bit.APMU_TEST = 1;
    }
    else
    {
        PMU->PMU_BOD_SEL.bit.APMU_TEST = 3;
    }


    PMU->PMU_BBPLL.bit.UVL_VTH = 3;

#if (CHIP_VERSION == RT58X_MPA)
    PMU->PMU_RCO32K.bit.RN_32K = 2047;
#elif (CHIP_VERSION == RT58X_MPB)
    PMU->PMU_RCO32K.bit.RN_32K = 0;
#else
    PMU->PMU_RCO32K.bit.RN_32K = 0;     /*default MPB Setting*/
#endif

    PMU->PMU_RCO32K.bit.C_32K = 0;
#if (CHIP_VERSION == RT58X_MPA)
    PMU->PMU_RCO32K.bit.PW_32K = 2;
#elif (CHIP_VERSION == RT58X_MPB)
    PMU->PMU_RCO32K.bit.PW_32K = 0;
#else
    PMU->PMU_RCO32K.bit.PW_32K = 0;     /*default MPB Setting*/
#endif


    pmu_dcdc_ctrl0_buf = PMU->PMU_DCDC_CTRL0;
    pmu_dcdc_ctrl0_buf.bit.DCDC_PPOWER_HEAVY = 0;
    pmu_dcdc_ctrl0_buf.bit.DCDC_EN_COMP_HEAVY = 1;
#if (CHIP_VERSION == RT58X_MPA)
    pmu_dcdc_ctrl0_buf.bit.DCDC_NPOWER_HEAVY = 7;
#elif (CHIP_VERSION == RT58X_MPB)
    pmu_dcdc_ctrl0_buf.bit.DCDC_NPOWER_HEAVY = 0;
#else
    pmu_dcdc_ctrl0_buf.bit.DCDC_NPOWER_HEAVY = 0;   /*default MPB Setting*/
#endif

    pmu_dcdc_ctrl0_buf.bit.DCDC_EN_ZCD_HEAVY = 0;
#if (CHIP_VERSION == RT58X_MPA)
    pmu_dcdc_ctrl0_buf.bit.DCDC_PDRIVE_HEAVY = 5;
#elif (CHIP_VERSION == RT58X_MPB)
    pmu_dcdc_ctrl0_buf.bit.DCDC_PDRIVE_HEAVY = 6;
#else
    pmu_dcdc_ctrl0_buf.bit.DCDC_PDRIVE_HEAVY = 6;   /*default MPB Setting*/
#endif
    pmu_dcdc_ctrl0_buf.bit.DCDC_MG_HEAVY = 1;
    pmu_dcdc_ctrl0_buf.bit.DCDC_NDRIVE_HEAVY = 5;
    pmu_dcdc_ctrl0_buf.bit.DCDC_CM_HEAVY = 1;
    pmu_dcdc_ctrl0_buf.bit.DCDC_PW_HEAVY = 0;
    pmu_dcdc_ctrl0_buf.bit.DCDC_C_SC_HEAVY = 0;
    pmu_dcdc_ctrl0_buf.bit.DCDC_C_HG_HEAVY = 1;

#if (CHIP_VERSION == RT58X_MPA)
    pmu_dcdc_ctrl0_buf.bit.DCDC_PWMF_HEAVY = 8;
#elif (CHIP_VERSION == RT58X_MPB)
    pmu_dcdc_ctrl0_buf.bit.DCDC_PWMF_HEAVY = 7;
#else
    pmu_dcdc_ctrl0_buf.bit.DCDC_PWMF_HEAVY = 7;     /*default MPB Setting*/
#endif

    pmu_dcdc_ctrl0_buf.bit.DCDC_OS_PN_HEAVY = 1;

#if (CHIP_VERSION == RT58X_MPA)
    pmu_dcdc_ctrl0_buf.bit.DCDC_OS_HEAVY = 3;
#elif (CHIP_VERSION == RT58X_MPB)
    pmu_dcdc_ctrl0_buf.bit.DCDC_OS_HEAVY = 1;
#else
    pmu_dcdc_ctrl0_buf.bit.DCDC_OS_HEAVY = 1;       /*default MPB Setting*/
#endif

    pmu_dcdc_ctrl0_buf.bit.DCDC_HG_HEAVY = 3;
#if (CHIP_VERSION == RT58X_MPA)
    pmu_dcdc_ctrl0_buf.bit.DCDC_DLY_HEAVY = 2;
#elif (CHIP_VERSION == RT58X_MPB)
    pmu_dcdc_ctrl0_buf.bit.DCDC_DLY_HEAVY = 0;
#else
    pmu_dcdc_ctrl0_buf.bit.DCDC_DLY_HEAVY = 0;      /*default MPB Setting*/
#endif

    PMU->PMU_DCDC_CTRL0 = pmu_dcdc_ctrl0_buf;

    pmu_dcdc_ctrl1_buf = PMU->PMU_DCDC_CTRL1;
    pmu_dcdc_ctrl1_buf.bit.DCDC_PPOWER_LIGHT = 2;
    pmu_dcdc_ctrl1_buf.bit.DCDC_EN_COMP_LIGHT = 1;
    pmu_dcdc_ctrl1_buf.bit.DCDC_NPOWER_LIGHT = 7;
    pmu_dcdc_ctrl1_buf.bit.DCDC_EN_ZCD_LIGHT = 1;
    pmu_dcdc_ctrl1_buf.bit.DCDC_PDRIVE_LIGHT = 3;
    pmu_dcdc_ctrl1_buf.bit.DCDC_MG_LIGHT = 1;
    pmu_dcdc_ctrl1_buf.bit.DCDC_NDRIVE_LIGHT = 3;
    pmu_dcdc_ctrl1_buf.bit.DCDC_CM_LIGHT = 1;
    pmu_dcdc_ctrl1_buf.bit.DCDC_PW_LIGHT = 0;
    pmu_dcdc_ctrl1_buf.bit.DCDC_C_SC_LIGHT = 0;
    pmu_dcdc_ctrl1_buf.bit.DCDC_C_HG_LIGHT = 1;
    pmu_dcdc_ctrl1_buf.bit.DCDC_PWMF_LIGHT = 12;
    pmu_dcdc_ctrl1_buf.bit.DCDC_OS_PN_LIGHT = 1;
    pmu_dcdc_ctrl1_buf.bit.DCDC_OS_LIGHT = 1;
    pmu_dcdc_ctrl1_buf.bit.DCDC_HG_LIGHT = 3;
    pmu_dcdc_ctrl1_buf.bit.DCDC_DLY_LIGHT = 0;
    PMU->PMU_DCDC_CTRL1 = pmu_dcdc_ctrl1_buf;

    //===
    PMU->PMU_BBPLL.bit.BBPLL_BYP_LDO = 1;
    PMU->PMU_BBPLL.bit.BBPLL_SEL_VTH = 1;
    //PMU->PMU_BBPLL.bit.BBPLL_HI = 1;
    //===

    SystemPmuSetMode(PMU_MODE_DCDC);

    reg_num = sizeof(pmu_ap_init_table_dcdc) / sizeof(reg_bit_write_t);

    for (i = 0; i < reg_num; i++)
    {
        reg_write_item = pmu_ap_init_table_dcdc[i];
        Reg_Bit_Write(reg_write_item);
    }

    reg_num = sizeof(pmu_mp_init_table_dcdc) / sizeof(reg_bit_write_t);

    for (i = 0; i < reg_num; i++)
    {
        reg_write_item = pmu_mp_init_table_dcdc[i];

        Reg_Bit_Write(reg_write_item);
    }

}

#endif
#endif


#if (!MODULE_ENABLE(RCO40K_CALIBRATION_DISABLE))
void Rco40k_Analog_Calibration(void)
{
    uint32_t rco32k_c;
    uint32_t rco32k_result;

    PMU->PMU_CAL32K_CFG0.bit.CFG_CAL32K_EN = 0;
    PMU->PMU_CAL32K_CFG1.bit.CFG_32K_RC_SEL = 0;

    PMU->PMU_RCO32K.bit.RN_32K = 0;

    for (rco32k_c = 0; rco32k_c < 8; rco32k_c++)
    {
        PMU->PMU_RCO32K.bit.C_32K = rco32k_c;
        PMU->PMU_CAL32K_CFG1.bit.EN_CK_CAL32K = 1;
        PMU->PMU_CAL32K_CFG1.bit.CFG_32K_EST_MODE = 0;
        PMU->PMU_CAL32K_CFG1.bit.CFG_32K_EST_TIME = 2;
        PMU->PMU_CAL32K_CFG1.bit.CFG_32K_EST_EN = 1;
        Delay_ms(1);
        PMU->PMU_CAL32K_CFG1.bit.CFG_32K_EST_EN = 0;
        rco32k_result = PMU->PMU_CAL32K_RESULT0.bit.EST_32K_RESULT;
        if (rco32k_result >= 102915)
        {
            break;
        }
    }

    PMU->PMU_CAL32K_CFG1.bit.CFG_CAL32K_TRACK_EN = 1;
    PMU->PMU_CAL32K_CFG0.bit.CFG_CAL32K_TARGET = 102400;
    PMU->PMU_CAL32K_CFG1.bit.CFG_CAL32K_LOCK_ERR = 5;
    PMU->PMU_CAL32K_CFG1.bit.CFG_CAL32K_AVG_COARSE = 1;
    PMU->PMU_CAL32K_CFG1.bit.CFG_CAL32K_AVG_FINE = 3;
    PMU->PMU_CAL32K_CFG1.bit.CFG_CAL32K_AVG_LOCK = 3;
    PMU->PMU_CAL32K_CFG1.bit.CFG_CAL32K_DLY = 2;
    PMU->PMU_CAL32K_CFG1.bit.CFG_CAL32K_FINE_GAIN = 5;
    PMU->PMU_CAL32K_CFG1.bit.CFG_CAL32K_SKIP_COARSE = 1;
    PMU->PMU_CAL32K_CFG1.bit.CFG_CAL32K_LOCK_GAIN = 5;
    PMU->PMU_CAL32K_CFG1.bit.CFG_32K_RC_SEL = 1;
    PMU->PMU_CAL32K_CFG1.bit.CFG_CAL32K_BOUND_MODE = 0;
    PMU->PMU_CAL32K_CFG0.bit.CFG_CAL32K_EN = 1;
}
#endif


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit(void)
{
    /*  This SystemInit is for boot used only,
     *  Please see system_cm3_mcu.s
     *  please don't call this function in other place.
     *
     *  Remak:  .BSS section not initial yet.
     *   So you can not use any global variable.
     *  Also,  NO  race condition issue here...
     */
    /*set PMU XTAL. 2020/10/26 add  --- set cfg_xtal_settle_time*/
    PMU->PMU_XTAL.reg = (PMU->PMU_XTAL.reg & ~0xFF)  | 0x3F;
    /*config sram shut down to default value*/
    set_sram_shutdown_sleep(0x00);

#if (CHIP_VERSION == RT58X_MPB)
    /*change BBPLL setting 2022/03/25 for MPB */
    PMU->PMU_BBPLL.bit.BBPLL_SEL_VTH = 1;
#endif

    /*enable flash QSPI mode.*/
    flash_enable_qe();

#if ((CHIP_VERSION == RT58X_MPA) || (CHIP_VERSION == RT58X_MPB))
    SystemPmuUpdateDig();

#if (SET_PMU_MODE == PMU_LDO_MODE)
    SystemPmuUpdateLdo();
#elif (SET_PMU_MODE == PMU_DCDC_MODE)
    SystemPmuUpdateDcdc();
#else
    SystemPmuUpdateDcdc();
#endif
#endif

#if (!MODULE_ENABLE(NONUSE_MP_SECTOR))
    MpSectorInit();
#endif

#if (SET_SYS_CLK == SYS_CLK_32MHZ)
    Change_Ahb_System_Clk(SYS_32MHZ_CLK);
#elif (SET_SYS_CLK == SYS_CLK_48MHZ)
    Change_Ahb_System_Clk(SYS_48MHZ_CLK);
#elif (SET_SYS_CLK == SYS_CLK_64MHZ)
    Change_Ahb_System_Clk(SYS_64MHZ_CLK);
#endif

    SystemCoreClockUpdate();

    sys_set_retention_reg(6, 0x07);     //Enable CM3 peripherals/APBGPIO and Remap/communication subsystem/ reset wdt triggered.

#if (CHIP_TYPE < RT584)
    {
        /*Enable dmaClkGate to save DMA power for each channel*/
        DMA_T   *DMA;

        DMA = (DMA_T *) DMA0_CH0_BASE;
        DMA->DMA_ENABLE |= DMA_DMACLK_GATE;
        DMA = (DMA_T *) DMA0_CH1_BASE;
        DMA->DMA_ENABLE |= DMA_DMACLK_GATE;
        DMA = (DMA_T *) DMA0_CH2_BASE;
        DMA->DMA_ENABLE |= DMA_DMACLK_GATE;
        DMA = (DMA_T *) DMA0_CH3_BASE;
        DMA->DMA_ENABLE |= DMA_DMACLK_GATE;
    }
#endif

#if (!MODULE_ENABLE(RCO40K_CALIBRATION_DISABLE))
    Rco40k_Analog_Calibration();
#endif


#if (MODULE_ENABLE(EXT32K_GPIO_ENABLE))
    set_ext32k_pin(EXT32K_GPIO7);                   /* externl slow clock 32.768khz, select input gpio pin, gpio0~gpio7 (0~7) */
    set_slow_clock_source(EXT32K_GPIO_ENABLE);      /* set slow clock(32.768khz) source form gpio */
#endif

    return;
}


