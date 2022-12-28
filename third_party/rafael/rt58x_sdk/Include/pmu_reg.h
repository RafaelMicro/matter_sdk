/**************************************************************************//**
 * @file     pmu_reg.h
 *****************************************************************************/
#include "chip_define.h"


#ifndef __PMU_REG_H__
#define __PMU_REG_H__

#if defined (__CC_ARM)
#pragma anon_unions
#endif



enum
{
    PMU_CONFIG_VSEL_0V95      = 0,   /**< Vsel = 0.95V. */
    PMU_CONFIG_VSEL_1V00      = 1,   /**< Vsel = 1.00V. */
    PMU_CONFIG_VSEL_1V05      = 2,   /**< Vsel = 1.05V. */
    PMU_CONFIG_VSEL_1V10      = 3,   /**< Vsel = 1.10V. */
    PMU_CONFIG_VSEL_1V15      = 4,   /**< Vsel = 1.15V. */
    PMU_CONFIG_VSEL_1V20      = 5,   /**< Vsel = 1.20V. */
    PMU_CONFIG_VSEL_1V25      = 6,   /**< Vsel = 1.25V. */
    PMU_CONFIG_VSEL_1V30      = 7,   /**< Vsel = 1.30V. */
};

typedef union pmu_comp0_s
{
    struct pmu_comp0_b
    {
        uint32_t AUX_COMP_SELREF        : 1;
        uint32_t AUX_COMP_INT_EN        : 1;
        uint32_t AUX_COMP_INT_POL       : 2;
        uint32_t AUX_COMP_EN_NM         : 1;
        uint32_t AUX_COMP_EN_SP         : 1;
        uint32_t AUX_COMP_EN_DS         : 1;
        uint32_t AUX_COMP_SELINPUT      : 1;
        uint32_t AUX_COMP_VSEL          : 4;
        uint32_t AUX_COMP_REFSEL        : 4;
        uint32_t AUX_COMP_CHSEL         : 4;
        uint32_t AUX_COMP_PW            : 2;
        uint32_t AUX_COMP_SELHYS        : 2;
        uint32_t AUX_COMP_SWDIV         : 1;
        uint32_t AUX_COMP_PSRR          : 1;
        uint32_t AUX_COMP_TC            : 1;
        uint32_t RESERVED               : 1;
        uint32_t AUX_COMP_DS_WAKEUP_EN  : 1;
        uint32_t AUX_COMP_DS_INV        : 1;
        uint32_t AUX_EN_START           : 2;
    } bit;
    uint32_t reg;
} pmu_comp0_t;

typedef union pmu_comp1_s
{
    struct pmu_comp1_b
    {
        uint32_t AUX_COMP_INT_CLR : 1;
        uint32_t RESERVED1        : 7;
        uint32_t BOD_INT_CLR      : 1;
        uint32_t RESERVED2        : 23;
    } bit;
    uint32_t reg;
} pmu_comp1_t;

typedef union pmu_comp2_s
{
    struct pmu_comp2_b
    {
        uint32_t AUX_COMP_INT_STA : 1;
        uint32_t AUX_COMP_OUT     : 1;
        uint32_t RESERVED1        : 6;
        uint32_t BOD_INT_STA      : 1;
        uint32_t BOD_OUT          : 1;
        uint32_t RESERVED2        : 6;
        uint32_t VDD_DET          : 3;
        uint32_t RESERVED3        : 13;
    } bit;
    uint32_t reg;
} pmu_comp2_t;

typedef union pmu_ts_s
{
    struct pmu_ts_b
    {
        uint32_t RESERVED0        : 16;
        uint32_t TS_VX            : 3;
        uint32_t RESERVED19       : 1;
        uint32_t TS_S             : 3;
        uint32_t RESERVED23       : 1;
        uint32_t TS_EN            : 1;
        uint32_t TS_RST           : 1;
        uint32_t TS_CLK_EN        : 1;
        uint32_t RESERVED27       : 1;
        uint32_t TS_CLK_SEL       : 2;
        uint32_t TEST_AIO_SEL     : 2;
    } bit;
    uint32_t reg;
} pmu_ts_t;

typedef union pmu_pwr_ctrl_s
{
    struct pmu_pwr_ctrl_b
    {
        uint32_t BOD_INT_EN         : 1;
        uint32_t RESERVED1          : 1;
        uint32_t BOD_INT_POL        : 2;
        uint32_t RESERVED2          : 21;
        uint32_t CFG_EN_VREF1_XTAL  : 1;
        uint32_t CFG_SEL_XBUFLOAD   : 1;
        uint32_t CFG_SEL_XBUFIN     : 1;
        uint32_t CFG_DS_32K_OFF     : 1;
        uint32_t CFG_BYP_XBUF_LDO   : 1;
        uint32_t EN_XBUF            : 1;
        uint32_t EN_D_XTALIN        : 1;
    } bit;
    uint32_t reg;
} pmu_pwr_ctrl_t;

typedef union pmu_xtal_s
{
    struct pmu_xtal_b
    {
        uint32_t CFG_XTAL_SETTLE_TIME   : 8;
        uint32_t CFG_PW_XTAL            : 3;
        uint32_t CFG_BYPASS_XTAL_SETTLE : 1;
        uint32_t CFG_XTAL_FAST          : 1;
        uint32_t CFG_XTAL_FAST_AUTO     : 1;
        uint32_t CFG_EN_XTAL            : 1;
        uint32_t CFG_EN_XTAL_AUTO       : 1;
        uint32_t CFG_XTAL_CAP_SEL       : 10;
        uint32_t RESERVED               : 6;
    } bit;
    uint32_t reg;
} pmu_xtal_t;

typedef union pmu_mem_ctrl_s
{
    struct pmu_mem_ctrl_b
    {
        uint32_t RESERVED1        : 1;
        uint32_t CFG_SRAM_TEST1   : 1;
        uint32_t CFG_SRAM_RME     : 1;
        uint32_t RESERVED2        : 1;
        uint32_t CFG_SRAM_RM      : 4;
        uint32_t RESERVED3        : 10;
        uint32_t DLY              : 2;
        uint32_t PWMF             : 3;
        uint32_t CM               : 1;
        uint32_t HG               : 2;
        uint32_t EN_BOD           : 1;
        uint32_t EN_DECT          : 1;
        uint32_t RESERVED4        : 1;
        uint32_t MG               : 1;
        uint32_t SIN              : 2;
    } bit;
    uint32_t reg;
} pmu_mem_ctrl_t;

typedef union pmu_rco32k_s
{
    struct pmu_rco32k_b
    {
        uint32_t RN_32K           : 11;
        uint32_t RESERVED11       : 5;
        uint32_t PW32KXOSC        : 3;
        uint32_t EN_32KXOSC       : 1;
        uint32_t EN_LDO32K        : 1;
        uint32_t RESERVED21       : 3;
        uint32_t C_32K            : 3;
        uint32_t TC_32K           : 1;
        uint32_t PW_32K           : 2;
        uint32_t PSRR_32K         : 1;
        uint32_t SEL_32K          : 1;
    } bit;
    uint32_t reg;
} pmu_rco32k_t;

typedef union pmu_clk_ctrl_s
{
    struct pmu_clk_ctrl_b
    {
        uint32_t RESERVED1        : 1;
        uint32_t EN_CLOCKMODEM    : 1;
        uint32_t RESERVED2        : 10;
        uint32_t CFG_CHIP_EN      : 1;
        uint32_t CFG_CHIP_EN_AUTO : 1;
        uint32_t RESERVED3        : 18;
    } bit;
    uint32_t reg;
} pmu_clk_ctrl_t;

typedef union pmu_pm_sel_s
{
    struct pmu_pm_sel_b
    {
        uint32_t RESERVED0            : 16;
        uint32_t XOCAP_UPDATE_MODE    : 1;
        uint32_t RESERVED17           : 11;
        uint32_t CFG_PWRX_SETTLE_TIME : 3;
        uint32_t RESERVED31           : 1;
    } bit;
    uint32_t reg;
} pmu_pm_sel_t;

typedef union pmu_cal32k_cfg1_s
{
    struct pmu_cal32k_cfg1_b
    {
        uint32_t CFG_CAL32K_LOCK_ERR    : 8;
        uint32_t CFG_CAL32K_AVG_COARSE  : 2;
        uint32_t CFG_CAL32K_AVG_FINE    : 2;
        uint32_t CFG_CAL32K_AVG_LOCK    : 2;
        uint32_t CFG_CAL32K_DLY         : 2;
        uint32_t CFG_CAL32K_FINE_GAIN   : 3;
        uint32_t CFG_CAL32K_SKIP_COARSE : 1;
        uint32_t CFG_CAL32K_LOCK_GAIN   : 3;
        uint32_t CFG_CAL32K_BOUND_MODE  : 1;
        uint32_t CFG_CAL32K_TRACK_EN    : 1;
        uint32_t EN_CK_CAL32K           : 1;
        uint32_t CFG_32K_EST_EN         : 1;
        uint32_t CFG_32K_RC_SEL         : 1;
        uint32_t CFG_32K_EST_MODE       : 2;
        uint32_t CFG_32K_EST_TIME       : 2;
    } bit;
    uint32_t reg;
} pmu_cal32k_cfg1_t;

typedef union pmu_cal32k_r0_s
{
    struct pmu_cal32k_r0_b
    {
        uint32_t EST_32K_RESULT         : 18;
        uint32_t RESERVED18             : 6;
        uint32_t EST_32K_RESULT_VALID   : 1;
        uint32_t RESERVED25             : 3;
        uint32_t CAL32K_BUSY            : 1;
        uint32_t CAL32K_LOCK            : 1;
        uint32_t CAL32K_TIMEOUT         : 1;
        uint32_t RESERVED31             : 1;
    } bit;
    uint32_t reg;
} pmu_cal32k_r0_t;

typedef union pmu_cal32k_cfg0_s
{
    struct pmu_cal32k_cfg0_b
    {
        uint32_t CFG_CAL32K_TARGET : 18;
        uint32_t RESERVED18        : 6;
        uint32_t CFG_CAL32K_EN     : 1;
        uint32_t RESERVED25        : 7;
    } bit;
    uint32_t reg;
} pmu_cal32k_cfg0_t;

typedef union pmu_vout_sel0_s
{
    struct pmu_vout_sel0_b
    {
        uint32_t DCDC_VOSEL_NM    : 4;
        uint32_t DCDC_VOSEL_SP    : 4;
        uint32_t DCDC_VOSEL_DS    : 4;
        uint32_t RESERVED12       : 4;
        uint32_t LLDO_VOSEL_NM    : 4;
        uint32_t LLDO_VOSEL_SP    : 4;
        uint32_t LLDO_VOSEL_DS    : 4;
        uint32_t RESERVED28       : 4;
    } bit;
    uint32_t reg;
} pmu_vout_sel0_t;

typedef union pmu_vout_sel1_s
{
    struct pmu_vout_sel1_b
    {
        uint32_t SLDO_VOSEL_NM    : 4;
        uint32_t SLDO_VOSEL_SP    : 4;
        uint32_t SLDO_VOSEL_DS    : 4;
        uint32_t RESERVED12       : 20;
    } bit;
    uint32_t reg;
} pmu_vout_sel1_t;

typedef union pmu_vout_sel2_s
{
    struct pmu_vout_sel2_b
    {
        uint32_t IOLDO_VOSEL_NM     : 4;
        uint32_t IOLDO_VOSEL_SP     : 4;
        uint32_t IOLDO_VOSEL_DS     : 4;
        uint32_t RESERVED12         : 4;
        uint32_t IOLDO_RET_VOSEL_NM : 3;
        uint32_t RESERVED19         : 1;
        uint32_t IOLDO_RET_VOSEL_SP : 3;
        uint32_t RESERVED23         : 1;
        uint32_t IOLDO_RET_VOSEL_DS : 3;
        uint32_t RESERVED27         : 5;
    } bit;
    uint32_t reg;
} pmu_vout_sel2_t;

typedef union pmu_bod_sel_s
{
    struct pmu_bod_sel_b
    {
        uint32_t BOD_R            : 2;
        uint32_t BOD_F            : 2;
        uint32_t VDECT            : 1;
        uint32_t RESERVED5        : 3;
        uint32_t APMU_TEST        : 2;
        uint32_t POR_VTH          : 2;
        uint32_t RESERVED12       : 4;
        uint32_t PMU_RESERVED_1   : 16;
    } bit;
    uint32_t reg;
} pmu_bod_sel_t;

typedef union pmu_dcdc_ctrl0_s
{
    struct pmu_dcdc_ctrl0_b
    {
        uint32_t DCDC_PPOWER_HEAVY  : 3;
        uint32_t DCDC_EN_COMP_HEAVY : 1;
        uint32_t DCDC_NPOWER_HEAVY  : 3;
        uint32_t DCDC_EN_ZCD_HEAVY  : 1;
        uint32_t DCDC_PDRIVE_HEAVY  : 3;
        uint32_t DCDC_MG_HEAVY      : 1;
        uint32_t DCDC_NDRIVE_HEAVY  : 3;
        uint32_t DCDC_CM_HEAVY      : 1;
        uint32_t DCDC_PW_HEAVY      : 2;
        uint32_t DCDC_C_SC_HEAVY    : 1;
        uint32_t DCDC_C_HG_HEAVY    : 1;
        uint32_t DCDC_PWMF_HEAVY    : 4;
        uint32_t RESERVED           : 1;
        uint32_t DCDC_OS_PN_HEAVY   : 1;
        uint32_t DCDC_OS_HEAVY      : 2;
        uint32_t DCDC_HG_HEAVY      : 2;
        uint32_t DCDC_DLY_HEAVY     : 2;
    } bit;
    uint32_t reg;
} pmu_dcdc_ctrl0_t;

typedef union pmu_dcdc_ctrl1_s
{
    struct pmu_dcdc_ctrl1_b
    {
        uint32_t DCDC_PPOWER_LIGHT  : 3;
        uint32_t DCDC_EN_COMP_LIGHT : 1;
        uint32_t DCDC_NPOWER_LIGHT  : 3;
        uint32_t DCDC_EN_ZCD_LIGHT  : 1;
        uint32_t DCDC_PDRIVE_LIGHT  : 3;
        uint32_t DCDC_MG_LIGHT      : 1;
        uint32_t DCDC_NDRIVE_LIGHT  : 3;
        uint32_t DCDC_CM_LIGHT      : 1;
        uint32_t DCDC_PW_LIGHT      : 2;
        uint32_t DCDC_C_SC_LIGHT    : 1;
        uint32_t DCDC_C_HG_LIGHT    : 1;
        uint32_t DCDC_PWMF_LIGHT    : 4;
        uint32_t RESERVED           : 1;
        uint32_t DCDC_OS_PN_LIGHT   : 1;
        uint32_t DCDC_OS_LIGHT      : 2;
        uint32_t DCDC_HG_LIGHT      : 2;
        uint32_t DCDC_DLY_LIGHT     : 2;
    } bit;
    uint32_t reg;
} pmu_dcdc_ctrl1_t;

typedef union pmu_en_ctrl_s
{
    struct pmu_en_ctrl_b
    {
        uint32_t EN_DCDC_NM       : 1;
        uint32_t EN_LLDO_NM       : 1;
        uint32_t EN_IOLDO_NM      : 1;
        uint32_t RESERVED3        : 1;
        uint32_t EN_BG1_NM        : 1;
        uint32_t EN_BG2_NM        : 1;
        uint32_t EN_BOD_NM        : 1;
        uint32_t EN_DECT_NM       : 1;
        uint32_t EN_DCDC_SP       : 1;
        uint32_t EN_LLDO_SP       : 1;
        uint32_t EN_IOLDO_SP      : 1;
        uint32_t RESERVED11       : 1;
        uint32_t EN_BG1_SP        : 1;
        uint32_t EN_BG2_SP        : 1;
        uint32_t EN_BOD_SP        : 1;
        uint32_t EN_DECT_SP       : 1;
        uint32_t EN_DCDC_DS       : 1;
        uint32_t EN_LLDO_DS       : 1;
        uint32_t EN_IOLDO_DS      : 1;
        uint32_t RESERVED19       : 1;
        uint32_t EN_BG1_DS        : 1;
        uint32_t EN_BG2_DS        : 1;
        uint32_t EN_BOD_DS        : 1;
        uint32_t EN_DECT_DS       : 1;
        uint32_t RESERVED24       : 1;
        uint32_t RESERVED25       : 1;
        uint32_t DIS_IOLOD_RET    : 1;
        uint32_t UVH_DISABLE      : 1;
        uint32_t DIS_POR_NM       : 1;
        uint32_t DIS_POR_SP       : 1;
        uint32_t DIS_POR_DS       : 1;
        uint32_t RESERVED31       : 1;
    } bit;
    uint32_t reg;
} pmu_en_ctrl_t;

typedef union pmu_other_ctrl_s
{
    struct pmu_other_ctrl_b
    {
        uint32_t LLDO_SIN         : 2;
        uint32_t LLDO_LOUT        : 1;
        uint32_t RESERVED3        : 1;
        uint32_t LLDO_IOC         : 3;
        uint32_t LLDO_EN_OCP      : 1;
        uint32_t IOLDO_SIN        : 2;
        uint32_t IOLDO_LOUT       : 1;
        uint32_t IOLDO_CM         : 1;
        uint32_t IOLDO_RET_SIN    : 2;
        uint32_t IOLDO_RET_LOUT   : 2;
        uint32_t DCDC_IOC         : 3;
        uint32_t DCDC_EN_OCP      : 1;
        uint32_t DCDC_SW_HEAVY    : 1;
        uint32_t DCDC_AUTO_HEAVY  : 1;
        uint32_t RESERVED22       : 2;
        uint32_t IOLDO_IOC        : 3;
        uint32_t IOLDO_EN_OCP     : 1;
        uint32_t SEL_BG0P6        : 1;
        uint32_t SEL_BG0P95       : 1;
        uint32_t SEL_BG1P2        : 1;
        uint32_t SEL_IREF         : 1;
    } bit;
    uint32_t reg;
} pmu_other_ctrl_t;

typedef union pmu_bbpllr_s
{
    struct pmu_bbpllr_b
    {
        uint32_t RESERVED0        : 24;
        uint32_t BBPLL_BANK_VCO   : 3;
        uint32_t RESERVED27       : 1;
        uint32_t BBPLL_VTBIT      : 2;
        uint32_t RESERVED30       : 2;
    } bit;
    uint32_t reg;
} pmu_bbpllr_t;

typedef union pmu_bbpll_s
{
    struct pmu_bbpll_b
    {
        uint32_t RESERVED0        : 2;
        uint32_t BBPLL_HI         : 2;
        uint32_t BBPLL_INI_BANK   : 2;
        uint32_t BBPLL_PW         : 2;
        uint32_t BBPLL_BYP_LDO    : 1;
        uint32_t BBPLL_MANUBANK   : 1;
        uint32_t BBPLL_TRIGGER_BG : 1;
        uint32_t BBPLL_TP_VT      : 1;
        uint32_t BBPLL_SEL_DLY    : 1;
        uint32_t BBPLL_SEL_ICP    : 1;
        uint32_t BBPLL_SEL_TC     : 1;
        uint32_t BBPLL_SEL_VTH    : 1;
        uint32_t BBPLL_RESETN_MAN : 1;
        uint32_t BBPLL_RESETN_AUTO: 1;
        uint32_t BBPLL_EN_START   : 2;
        uint32_t BBPLL_BYP        : 1;
        uint32_t RESERVED21       : 1;
        uint32_t BBPLL_BANK1_MAN  : 1;
        uint32_t BBPLL_BANK1_AUTO : 1;
        uint32_t EN_UVL           : 1;
        uint32_t UVL_OUT_VALID    : 1;
        uint32_t RESERVED26       : 2;
        uint32_t UVL_VTH          : 2;
        uint32_t RESERVED30       : 2;
    } bit;
    uint32_t reg;
} pmu_bbpll_t;


typedef struct
{
    __IO  pmu_comp0_t  PMU_COMP0;             //offset: 0x00
    __IO  pmu_comp1_t  PMU_COMP1;             //offset: 0x04
    __IO  pmu_comp2_t  PMU_COMP2;             //offset: 0x08
    __IO  uint32_t  PMU_RESERVED0C;           //offset: 0x0C
    __IO  pmu_ts_t  PMU_TS;                   //offset: 0x10
    __IO  uint32_t  PMU_RVD0;                 //offset: 0x14
    __IO  uint32_t  PMU_RVD1;                 //offset: 0x18
    __IO  uint32_t  PMU_RESERVED1C;           //offset: 0x1C
    __IO  pmu_pwr_ctrl_t  PMU_PWR_CTRL;       //offset: 0x20
    __IO  pmu_xtal_t  PMU_XTAL;               //offset: 0x24
    __IO  pmu_mem_ctrl_t  PMU_MEM_CTRL;       //offset: 0x28
    __IO  uint32_t  PMU_RESERVED2C;           //offset: 0x2C
    __IO  uint32_t  PMU_BOD;                  //offset: 0x30
    __IO  pmu_rco32k_t  PMU_RCO32K;           //offset: 0x34
    __IO  uint32_t  PMU_DEBUG_CTRL;           //offset: 0x38
    __IO  pmu_clk_ctrl_t  PMU_CLK_CTRL;       //offset: 0x3C
    __IO  uint32_t  PMU_GPIO_DRV;             //offset: 0x40
    __IO  uint32_t  PMU_RESERVED44;           //offset: 0x44
    __IO  pmu_pm_sel_t  PMU_PM_SEL;           //offset: 0x48
    __IO  uint32_t  PMU_RESERVED4C;           //offset: 0x4C
    __IO  pmu_cal32k_cfg0_t  PMU_CAL32K_CFG0; //offset: 0x50
    __IO  pmu_cal32k_cfg1_t  PMU_CAL32K_CFG1; //offset: 0x54
    __IO  pmu_cal32k_r0_t  PMU_CAL32K_RESULT0;//offset: 0x58
    __IO  uint32_t  PMU_RESERVED5C;           //offset: 0x5C
    __IO  uint32_t  PMU_RESERVED60;           //offset: 0x60
    __IO  uint32_t  PMU_RESERVED64;           //offset: 0x64
    __IO  uint32_t  PMU_RESERVED68;           //offset: 0x68
    __IO  uint32_t  PMU_RESERVED6C;           //offset: 0x6C
    __IO  uint32_t  PMU_RESERVED70;           //offset: 0x70
    __IO  uint32_t  PMU_RESERVED74;           //offset: 0x74
    __IO  uint32_t  PMU_RESERVED78;           //offset: 0x78
    __IO  uint32_t  PMU_RESERVED7C;           //offset: 0x7C
    __IO  uint32_t  PMU_RESERVED80;           //offset: 0x80
    __IO  uint32_t  PMU_RESERVED84;           //offset: 0x84
    __IO  uint32_t  PMU_RESERVED88;           //offset: 0x88
    __IO  uint32_t  PMU_RESERVED8C;           //offset: 0x8C
    __IO  pmu_vout_sel0_t  PMU_VOUT_SEL0;     //offset: 0x90
    __IO  pmu_vout_sel1_t  PMU_VOUT_SEL1;     //offset: 0x94
    __IO  pmu_vout_sel2_t  PMU_VOUT_SEL2;     //offset: 0x98
    __IO  pmu_bod_sel_t  PMU_BOD_SEL;         //offset: 0x9C
    __IO  pmu_dcdc_ctrl0_t  PMU_DCDC_CTRL0;   //offset: 0xA0
    __IO  pmu_dcdc_ctrl1_t  PMU_DCDC_CTRL1;   //offset: 0xA4
    __IO  uint32_t  PMU_RESERVEDA8;           //offset: 0xA8
    __IO  pmu_other_ctrl_t  PMU_OTHER_CTRL;   //offset: 0xAC
    __IO  pmu_en_ctrl_t  PMU_EN_CTRL;         //offset: 0xB0
    __IO  uint32_t  PMU_RESERVEDB4;           //offset: 0xB4
    __IO  uint32_t  PMU_RESERVEDB8;           //offset: 0xB8
    __IO  pmu_bbpllr_t  PMU_BBPLLR;           //offset: 0xBC
    __IO  uint32_t  PMU_RESERVEDC0;           //offset: 0xC0
    __IO  pmu_bbpll_t  PMU_BBPLL;             //offset: 0xC4
    __IO  uint32_t  PMU_RESERVEDC8;           //offset: 0xC8
    __IO  uint32_t  PMU_RESERVEDCC;           //offset: 0xCC
} pmu_t;

#define PLL_LOCK_STATUS()                           (PMU->PMU_BBPLLR.bit.BBPLL_VTBIT)
#define PLL_VIBIT_STATUS()                          (PMU->PMU_BBPLLR.bit.BBPLL_VTBIT)
#define PLL_BANK_VCO_STATUS()                 (PMU->PMU_BBPLLR.bit.BBPLL_BANK_VCO)

#if defined (__CC_ARM)
#pragma no_anon_unions
#endif

#endif

