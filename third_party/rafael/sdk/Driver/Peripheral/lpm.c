/**************************************************************************//**
 * @file     lpm.c
 * @version
 * @brief    Low Power Mode driver
 *
 * @copyright
*****************************************************************************/

#include "cm3_mcu.h"

uint32_t low_power_mask_status = LOW_POWER_NO_MASK;
uint32_t comm_subsystem_wakeup_mask_status = COMM_SUBSYS_WAKEUP_NO_MASK;
low_power_level_cfg_t low_power_level = LOW_POWER_LEVEL_NORMAL;
low_power_wakeup_cfg_t low_power_wakeup = LOW_POWER_WAKEUP_NULL;
uint32_t low_power_wakeup_update = 0;
uint32_t low_power_wakeup_uart = 0;
uint32_t low_power_wakeup_gpio = 0;


void Lpm_Low_Power_Mask(uint32_t mask)
{
    low_power_mask_status |= mask;
}

void Lpm_Low_Power_Unmask(uint32_t unmask)
{
    low_power_mask_status &= (~unmask);
}

uint32_t Lpm_Get_Low_Power_Mask_Status(void)
{
    return low_power_mask_status;
}

void Lpm_Comm_Subsystem_Wakeup_Mask(uint32_t mask)
{
    comm_subsystem_wakeup_mask_status |= mask;
}

void Lpm_Comm_Subsystem_Wakeup_Unmask(uint32_t unmask)
{
    comm_subsystem_wakeup_mask_status &= (~unmask);
}

void Lpm_Comm_Subsystem_Check_System_Ready(void)
{
    uint32_t status;
    do
    {
        status = COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_INFO;
    } while (((status & 0x01) != 1));

}


void Lpm_Comm_Subsystem_Check_System_DeepSleep(void)
{
    uint32_t status;
    do
    {
        status = ((COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_INFO & 0x6) >> 1);
    } while ((status != 1UL));
}


uint32_t Lpm_Get_Comm_Subsystem_Wakeup_Mask_Status(void)
{
    return comm_subsystem_wakeup_mask_status;
}


void Lpm_Set_Low_Power_Level(low_power_level_cfg_t low_power_level_cfg)
{
    low_power_level = low_power_level_cfg;
    low_power_wakeup_update = 1;
}


void Lpm_Enable_Low_Power_Wakeup(low_power_wakeup_cfg_t low_power_wakeup_cfg)
{
    uint32_t wakeup_source = 0;
    uint32_t wakeup_source_index = 0;

    wakeup_source = (low_power_wakeup_cfg & 0xFFFF);

    if (wakeup_source == LOW_POWER_WAKEUP_GPIO)
    {
        wakeup_source_index = (low_power_wakeup_cfg >> 16);
        low_power_wakeup_gpio |= (1 << wakeup_source_index);
    }
    else if (wakeup_source == LOW_POWER_WAKEUP_UART_RX)
    {
        wakeup_source_index = (low_power_wakeup_cfg >> 16);
        low_power_wakeup_uart |= (1 << wakeup_source_index);
    }
    else
    {
        low_power_wakeup |= wakeup_source;
    }

    low_power_wakeup_update = 1;
}

void Lpm_Disable_Low_Power_Wakeup(low_power_wakeup_cfg_t low_power_wakeup_cfg)
{
    uint32_t wakeup_source = 0;
    uint32_t wakeup_source_index = 0;

    wakeup_source = (low_power_wakeup_cfg & 0xFFFF);

    if (wakeup_source == LOW_POWER_WAKEUP_GPIO)
    {
        wakeup_source_index = (low_power_wakeup_cfg >> 16);
        low_power_wakeup_gpio &= (~(1 << wakeup_source_index));
    }
    else if (wakeup_source == LOW_POWER_WAKEUP_UART_RX)
    {
        wakeup_source_index = (low_power_wakeup_cfg >> 16);
        low_power_wakeup_uart &= (~(1 << wakeup_source_index));
    }
    else
    {
        low_power_wakeup &= ~wakeup_source;
    }

    low_power_wakeup_update = 1;
}

void Lpm_Set_Platform_Low_Power_Wakeup(low_power_platform_enter_mode_cfg_t platform_low_power_mode)
{
    do
    {
        if (low_power_wakeup_update == 0)
        {
            break;
        }

        low_power_wakeup_update = 0;

        if (platform_low_power_mode == LOW_POWER_PLATFORM_ENTER_SLEEP)
        {
            /* HCLK sleep selection in Sleep */
            if ((low_power_wakeup & LOW_POWER_WAKEUP_RTC_TIMER) ||
                    (low_power_wakeup & LOW_POWER_WAKEUP_COMPARATOR) ||
                    (low_power_wakeup & LOW_POWER_WAKEUP_32K_TIMER) ||
                    (low_power_wakeup_gpio != 0))
            {
                SYSCTRL->SYS_CLK_CTRL |= HCLK_SLEEP_RUN_IN_32K;
            }
            else
            {
                SYSCTRL->SYS_CLK_CTRL &= (~HCLK_SLEEP_RUN_IN_32K);
            }

            /* AUX comparator enable selection in Sleep */
            if (low_power_wakeup & LOW_POWER_WAKEUP_COMPARATOR)
            {
                PMU->PMU_COMP0.bit.AUX_COMP_EN_SP = 1;
            }
            else
            {
                PMU->PMU_COMP0.bit.AUX_COMP_EN_SP = 0;
            }

            /* UART0 sleep wake enable selection in Sleep */
            if (low_power_wakeup_uart & (1 << 0))
            {
                SYSCTRL->SYS_LOWPOWER_CTRL |= UART0_SLEEP_WAKE_EN_MASK;
            }
            else
            {
                SYSCTRL->SYS_LOWPOWER_CTRL &= (~UART0_SLEEP_WAKE_EN_MASK);
            }

            /* UART1 sleep wake enable selection in Sleep */
            if (low_power_wakeup_uart & (1 << 1))
            {
                SYSCTRL->SYS_LOWPOWER_CTRL |= UART1_SLEEP_WAKE_EN_MASK;
            }
            else
            {
                SYSCTRL->SYS_LOWPOWER_CTRL &= (~UART1_SLEEP_WAKE_EN_MASK);
            }

            /* UART2 sleep wake enable selection in Sleep */
            if (low_power_wakeup_uart & (1 << 2))
            {
                SYSCTRL->SYS_LOWPOWER_CTRL |= UART2_SLEEP_WAKE_EN_MASK;
            }
            else
            {
                SYSCTRL->SYS_LOWPOWER_CTRL &= (~UART2_SLEEP_WAKE_EN_MASK);
            }
        }
        else if (platform_low_power_mode == LOW_POWER_PLATFORM_ENTER_DEEP_SLEEP)
        {
            if (low_power_wakeup & LOW_POWER_WAKEUP_RTC_TIMER)
            {
                SYSCTRL->SYS_CLK_CTRL |= HCLK_SLEEP_RUN_IN_32K;        /* HCLK sleep selection in Sleep, HCLK is forced to 32KHz in Sleep */
                SYSCTRL->SYS_CLK_CTRL |= RTC_PCLK_DS_MASK;             /* Enable RTC PCLK in Sleep, this allows RTC wakeup in DeepSleep */
                PMU->PMU_PWR_CTRL.bit.CFG_DS_32K_OFF = 0;              /* Enable 32KHz in DeepSleep mode, this allows RTC to work in DeepSleep */
            }
            else
            {
                SYSCTRL->SYS_CLK_CTRL &= (~HCLK_SLEEP_RUN_IN_32K);     /* HCLK sleep selection in Sleep, HCLK is disabled in Sleep */
                SYSCTRL->SYS_CLK_CTRL &= (~RTC_PCLK_DS_MASK);          /* Disable RTC PCLK in Sleep */
                PMU->PMU_PWR_CTRL.bit.CFG_DS_32K_OFF = 1;              /* Disable 32KHz in DeepSleep mode */
            }

            set_deepsleep_wakeup_pin(low_power_wakeup_gpio);           /* Set the corresponding bits to enable the wakeup of GPIOx in Deep Sleep */

            if (low_power_wakeup & LOW_POWER_WAKEUP_COMPARATOR)
            {
                PMU->PMU_COMP0.bit.AUX_COMP_EN_DS = 1;                 /* AUX comparator enable selection in Sleep, AUX comparator enable in DeepSleep */
                PMU->PMU_COMP0.bit.AUX_COMP_DS_WAKEUP_EN = 1;          /* AUX comparator wakeup enable selection in Sleep, AUX comparator wakeup enable in DeepSleep */
            }
            else
            {
                PMU->PMU_COMP0.bit.AUX_COMP_EN_DS = 0;                 /* AUX comparator enable selection in Sleep, AUX comparator disable in DeepSleep */
                PMU->PMU_COMP0.bit.AUX_COMP_DS_WAKEUP_EN = 0;          /* AUX comparator wakeup enable selection in Sleep, AUX comparator wakeup disable in DeepSleep */
            }
        }

    } while (0);
}

void Lpm_Set_GPIO_Deepsleep_Wakeup_Invert(uint32_t value)
{
    set_deepsleep_wakeup_invert(value);                                /* Set the corresponding bit0~bit31 to invert the GPIO0~GPIO31 for wakeup in DeepSleep Mode */
}

void Lpm_Set_Comparator_Deepsleep_Wakeup_Invert(uint32_t value)
{
    PMU->PMU_COMP0.bit.AUX_COMP_DS_INV = value;                        /* Invert the Aux Comparator Output for wakeup in DeepSleep Mode */
}

void Lpm_Set_Sram_Normal_Shutdown(uint32_t value)
{
    set_sram_shutdown_normal(value);                                   /* SRAM shut-down control in Normal Mode, set the corresponding bit0~bit4 to shut-down SRAM0~SRAM4 in Normal Mode */
}

void Lpm_Set_Sram_Sleep_Deepsleep_Shutdown(uint32_t value)
{
    set_sram_shutdown_sleep(value);                                   /* SRAM shut-down control in Low Power Mode, set the corresponding bit0~bit4 to shut-down SRAM0~SRAM4 in Sleep/DeepSleep Mode */
}


void Lpm_Enter_Low_Power_Mode(void)
{
    pmu_dcdc_ctrl1_t pmu_cdcd_ctrl1_buf;

    if (low_power_mask_status == LOW_POWER_NO_MASK)
    {
        pmu_cdcd_ctrl1_buf.reg = PMU->PMU_DCDC_CTRL1.reg;

        PMU->PMU_DCDC_CTRL1.bit.DCDC_EN_COMP_LIGHT = 0;
        PMU->PMU_DCDC_CTRL1.bit.DCDC_MG_LIGHT = 0;

        if (low_power_level == LOW_POWER_LEVEL_SLEEP3)
        {
            COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST |= COMMUMICATION_SUBSYSTEM_HOSTMODE;   /* enable host mode */
            COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST |= COMMUMICATION_SUBSYSTEM_RESET;      /* communication system host control reset */
            Lpm_Comm_Subsystem_Check_System_Ready();
            COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST |= COMMUMICATION_SUBSYSTEM_DEEPSLEEP;  /* communication system enter deep sleep mode */
            Lpm_Comm_Subsystem_Check_System_DeepSleep();

            Lpm_Set_Platform_Low_Power_Wakeup(LOW_POWER_PLATFORM_ENTER_DEEP_SLEEP);        /* set platform system wakeup source when entering deep sleep mode*/
            SYSCTRL->POWER_STATE_CTRL = LOW_POWER_PLATFORM_ENTER_DEEP_SLEEP;               /* platform system enter deep sleep mode */
            __DSB();
            __WFI();
            __ISB();
        }
        else if (low_power_level == LOW_POWER_LEVEL_SLEEP2)
        {
            COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST |= COMMUMICATION_SUBSYSTEM_HOSTMODE;   /* enable host mode */
            COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST |= COMMUMICATION_SUBSYSTEM_RESET;      /* communication system host control reset */
            Lpm_Comm_Subsystem_Check_System_Ready();
            COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST |= COMMUMICATION_SUBSYSTEM_DEEPSLEEP;  /* communication system enter deep sleep mode */
            Lpm_Comm_Subsystem_Check_System_DeepSleep();

            Lpm_Set_Platform_Low_Power_Wakeup(LOW_POWER_PLATFORM_ENTER_SLEEP);             /* set platform system wakeup source when entering sleep mode*/
            SYSCTRL->POWER_STATE_CTRL = LOW_POWER_PLATFORM_ENTER_SLEEP;                    /* platform system enter sleep mode */
            __DSB();
            __WFI();
            __ISB();

            if (comm_subsystem_wakeup_mask_status != COMM_SUBSYS_WAKEUP_NO_MASK)
            {
                COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST |= COMMUMICATION_SUBSYSTEM_HOSTMODE;   /* enable host mode */
                COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST |= COMMUMICATION_SUBSYSTEM_WAKEUP;     /* communication system wakeup from deep sleep mode */

                /* Need to reload the communication subsystem code here */
            }
        }
        else if (low_power_level == LOW_POWER_LEVEL_SLEEP1)
        {
            COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST |= COMMUMICATION_SUBSYSTEM_HOSTMODE;   /* enable host mode */
            COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST |= COMMUMICATION_SUBSYSTEM_SLEEP;      /* communication system enter sleep mode */

            Lpm_Set_Platform_Low_Power_Wakeup(LOW_POWER_PLATFORM_ENTER_SLEEP);             /* set platform system wakeup source when entering sleep mode*/
            SYSCTRL->POWER_STATE_CTRL = LOW_POWER_PLATFORM_ENTER_SLEEP;                    /* platform system enter sleep mode */
            __DSB();
            __WFI();
            __ISB();

            if (comm_subsystem_wakeup_mask_status != COMM_SUBSYS_WAKEUP_NO_MASK)
            {
                COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST |= COMMUMICATION_SUBSYSTEM_HOSTMODE;   /* enable host mode */
                COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST |= COMMUMICATION_SUBSYSTEM_WAKEUP;     /* communication system wakeup from sleep mode */
            }
        }
        else if (low_power_level == LOW_POWER_LEVEL_SLEEP0)
        {
            Lpm_Set_Platform_Low_Power_Wakeup(LOW_POWER_PLATFORM_ENTER_SLEEP);                 /* set platform system wakeup source when entering sleep mode*/
            SYSCTRL->POWER_STATE_CTRL = LOW_POWER_PLATFORM_ENTER_SLEEP;                        /* platform system enter sleep mode */
            __DSB();
            __WFI();
            __ISB();
        }

        PMU->PMU_DCDC_CTRL1.bit.DCDC_EN_COMP_LIGHT = pmu_cdcd_ctrl1_buf.bit.DCDC_EN_COMP_LIGHT;
        PMU->PMU_DCDC_CTRL1.bit.DCDC_MG_LIGHT = pmu_cdcd_ctrl1_buf.bit.DCDC_MG_LIGHT;
    }
}
