/**************************************************************************//**
 * @file     sysfun.c
 * @version
 * @brief    System function implement
 *
 * @copyright
 ******************************************************************************/
#include "assert_help.h"
#include "cm3_mcu.h"

#include "project_config.h"

#if (MODULE_ENABLE(SUPPORT_MULTITASKING))
#include "FreeRTOS.h"
#include "task.h"
#endif

static int critical_counter = 0;


void enter_critical_section(void)
{
    if (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk)
    {
        return;
    }
#if (MODULE_ENABLE(SUPPORT_MULTITASKING))
    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED)
    {
        __disable_irq();
    }
    else
    {
        vPortEnterCritical();
    }
#else
    __disable_irq();
#endif

    critical_counter++;
}

void leave_critical_section(void)
{
    if (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk)
    {
        return;
    }
    critical_counter--;
    assert_param(critical_counter >= 0);
#if (MODULE_ENABLE(SUPPORT_MULTITASKING))
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        vPortExitCritical();
    }
#endif
    if (critical_counter == 0)
    {
#if (MODULE_ENABLE(SUPPORT_MULTITASKING))
        if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED)
        {
            __enable_irq();
        }
#else
        __enable_irq();
#endif
    }
}

/*
 *      version_check is help function to check
 *   software project setting is the same as hardware IC version.
 *   If software project define "CHIP_VERSION" is
 *   not matched with hardware IC version, this functio will return 0, otherwise 1.
 *
 */
uint32_t version_check(void)
{

    uint32_t   version_info, ret = 1, chip_id, chip_rev;

    version_info =  SYSCTRL->CHIP_INFO ;

    chip_id =  version_info & IC_CHIP_ID_MASK;
    chip_rev = version_info & IC_CHIP_REVISION_MASK;

#if (CHIP_VERSION == RT58X_MPA)

    if ((chip_id != IC_RT58X) || (chip_rev != IC_CHIP_REVISION_MPA))
    {
        return 0;       /*hardware is different from software setting*/
    }

#elif (CHIP_VERSION == RT58X_MPB)

    if ((chip_id != IC_RT58X) || (chip_rev != IC_CHIP_REVISION_MPB))
    {
        return 0;       /*hardware is different from software setting*/
    }

#elif (CHIP_VERSION == RT58X_SHUTTLE)

#error "SHUTTLE IC has been *** OBSOLETED!! ***"

#endif

    return ret;

}

chip_model_t GetOtpVersion()
{
    uint32_t   otp_rd_buf_addr[64];
    uint32_t     i;
    uint8_t    buf_Tmp[4];

    otp_version_t otp_version;
    chip_model_t  chip_model;

    chip_model.type = CHIP_TYPE_UNKNOW;
    chip_model.version = CHIP_VERSION_UNKNOW;

    if (flash_read_otp_sec_page((uint32_t)otp_rd_buf_addr) != STATUS_SUCCESS)
    {
        return  chip_model;
    }

    for (i = 0; i < 8; i++)
    {
        *(uint32_t *)buf_Tmp = otp_rd_buf_addr[(i / 4)];
        otp_version.buf[i] = buf_Tmp[(i % 4)];
    }

    if (otp_version.buf[0] == 0xFF) //otp version flag
    {
        return chip_model;
    }

    /*ASCII Value*/
    otp_version.buf[5] -= 0x30; /* ascii 0~9 0x30~0x39 */
    otp_version.buf[6] -= 0x40; /* ascii A~Z 0x41~0x5A */

    /*reference chip_define.h
     CHIP_ID(TYPE,VER)                   ((TYPE << 8) | VER)
     CHIP_MODEL                           CHIP_ID(CHIP_TYPE,CHIP_VERSION)
    */
    chip_model.type = (chip_type_t)otp_version.buf[5];

    chip_model.version = (chip_version_t)otp_version.buf[6];

    return chip_model;
}
/*
 *  Reset the system software by using the watchdog timer to reset the chip.
 *
 */
void Sys_Software_Reset(void)
{
    wdt_ctrl_t controller;
    controller.reg = 0;

    /*wait flash operation finish*/
    while (flash_check_busy()) {;}

    /*get watch dog control register*/
    controller.reg = WDT->CONTROL.reg;

    /*Check the lock bit.
     If the lock bit is set to 1, it indicates that the WDT control register cannot be modified.
     Only waiting for the WDT to occur is possible.
    */
    if (controller.bit.LOCKOUT == 1)
    {
        while (1) {;}     //wait watch dog reset
    }

    /*confit wdt register*/
    controller.bit.WDT_EN = 1;
    controller.bit.RESET_EN = 1;    /*Lock*/
    controller.bit.LOCKOUT = 1;
    controller.bit.PRESCALE = WDT_PRESCALE_32;
    WDT->WINDOW_MIN = 0;
    WDT->LOAD = 1;
    WDT->CONTROL.reg = controller.reg;
    while (1) {;}
}


/*
 *  system PMU Mode
 *
 */
pmu_mode_cfg_t GetPmuMode(void)
{
    pmu_mode_cfg_t Mode;
    Mode = PMU_MODE_DCDC;
    if ( (PMU->PMU_EN_CTRL.bit.EN_LLDO_NM == 0) && (PMU->PMU_EN_CTRL.bit.EN_DCDC_NM == 1))
    {
        Mode = PMU_MODE_DCDC;
    }
    else if ( (PMU->PMU_EN_CTRL.bit.EN_LLDO_NM == 1) && (PMU->PMU_EN_CTRL.bit.EN_DCDC_NM == 0))
    {
        Mode = PMU_MODE_LDO;
    }

    return Mode;
}

