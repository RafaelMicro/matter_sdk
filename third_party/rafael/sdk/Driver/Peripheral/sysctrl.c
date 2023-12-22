/**************************************************************************//**
 * @file     sysctrl.c
 * @version
 * @brief
 *
 * @copyright
*****************************************************************************/

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "cm3_mcu.h"


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
static void Delay_us_32MHz(uint32_t volatile number_of_us);
static void Delay_us_48MHz(uint32_t volatile number_of_us);
static void Delay_us_64MHz(uint32_t volatile number_of_us);
static void Delay_ms_32MHz(uint32_t volatile number_of_ms);
static void Delay_ms_48MHz(uint32_t volatile number_of_ms);
static void Delay_ms_64MHz(uint32_t volatile number_of_ms);


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/


/**************************************************************************************************
 *    GLOBAL VARIABLES
 *************************************************************************************************/
delay_us_t Delay_us = Delay_us_64MHz;
delay_ms_t Delay_ms = Delay_ms_64MHz;


/**************************************************************************************************
 *    LOCAL FUNCTIONS
 *************************************************************************************************/
static void Delay_us_32MHz(uint32_t volatile number_of_us)
{
    uint32_t R0 __attribute__((unused));

    __asm volatile (
        " loop_32MHz:                      \n"
        "    SUBS    R0, R0, #1            \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    BNE    loop_32MHz_overhead    \n"
        "    B      loop_32MHz_end         \n"
        " loop_32MHz_overhead:             \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    B     loop_32MHz              \n"
        " loop_32MHz_end:                  \n"
    );
}


static void Delay_us_48MHz(uint32_t volatile number_of_us)
{
    uint32_t R0 __attribute__((unused));

    __asm volatile (
        " loop_48MHz:                      \n"
        "    SUBS    R0, R0, #1            \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    BNE    loop_48MHz_overhead    \n"
        "    B      loop_48MHz_end         \n"
        " loop_48MHz_overhead:             \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    B     loop_48MHz              \n"
        " loop_48MHz_end: \n"
    );
}


static void Delay_us_64MHz(uint32_t volatile number_of_us)
{
    uint32_t R0 __attribute__((unused));

    __asm volatile (
        " loop_64MHz:                      \n"
        "    SUBS    R0, R0, #1            \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    BNE    loop_64MHz_overhead    \n"
        "    B      loop_64MHz_end         \n"
        " loop_64MHz_overhead:             \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    NOP                           \n"
        "    B     loop_64MHz              \n"
        " loop_64MHz_end:                  \n"
    );
}


static void Delay_ms_32MHz(uint32_t volatile number_of_ms)
{
    while (number_of_ms != 0)
    {
        number_of_ms--;
        Delay_us(999);

        if (number_of_ms != 0)          /*delay for overhead*/
        {
            __asm volatile (
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
            );
        }
    }
}


static void Delay_ms_48MHz(uint32_t volatile number_of_ms)
{
    while (number_of_ms != 0)
    {
        number_of_ms--;
        Delay_us(999);

        __asm volatile (                         /*fine tune 1us delay time*/
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
        );

        if (number_of_ms != 0)          /*delay for overhead*/
        {
            __asm volatile (
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
            );
        }
    }
}


static void Delay_ms_64MHz(uint32_t volatile number_of_ms)
{
    while (number_of_ms != 0)
    {
        number_of_ms--;
        Delay_us(999);

        __asm volatile (                         /*fine tune 1us delay time*/
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
            " NOP \n"
        );

        if (number_of_ms != 0)          /*delay for overhead*/
        {
            __asm volatile (
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
                " NOP \n"
            );
        }
    }
}


/**************************************************************************************************
 *    GLOBAL FUNCTIONS
 *************************************************************************************************/
void Delay_Init(void)
{
    switch (Get_Ahb_System_Clk())
    {
    case SYS_CLK_32MHZ:
        Delay_us = Delay_us_32MHz;
        Delay_ms = Delay_ms_32MHz;
        break;
    case SYS_CLK_48MHZ:
        Delay_us = Delay_us_48MHz;
        Delay_ms = Delay_ms_48MHz;
        break;
    case SYS_CLK_64MHZ:
        Delay_us = Delay_us_64MHz;
        Delay_ms = Delay_ms_64MHz;
        break;
    default:
        break;
    }
}


void pin_set_mode(uint32_t pin_number, uint32_t mode)
{
    uint32_t reg, base, mask_offset, mask;

    if ((pin_number >= 32) || (mode > 7))
    {
        return;     /*Invalid setting mode.*/
    }

    base = MAP_BASE + (pin_number >> 3) * 4;
    mask_offset = (pin_number & 0x7) << 2;
    mask = 0xF << mask_offset;

    /*pin mux setting is share resource.*/
    reg = *((volatile unsigned int *) base);
    reg = reg & ~mask;
    reg = reg | (mode << mask_offset);

    *((volatile unsigned int *)base) =  reg;

    return;
}

uint32_t pin_get_mode(uint32_t pin_number)
{
    uint32_t reg, base, mask_offset;

    if (pin_number >= 32)
    {
        return 0xF;     /*Invalid setting mode.*/
    }

    base = MAP_BASE + (pin_number >> 3) * 4;
    mask_offset = (pin_number & 0x7) << 2;

    reg = *((volatile unsigned int *) base);
    reg = reg >> mask_offset;
    reg = reg & 0x7;

    return reg;
}

/*
 * For multithread OS, and dynamic enable/disable peripheral environment,
 * Set this register has race condition issue.
 * so we need critical section protect.
 *
 */
void enable_perclk(uint32_t clock)
{
    if ((clock < UART0_CLK) || (clock > I2S_CLK))
    {
        return;     /*Invalid setting mode.*/
    }

    SYSCTRL->SYS_CLK_CTRL |= (1 << clock) ;
}

/*
 * For multithread OS, and dynamic enable/disable peripheral environment,
 * Set this register has race condition issue,
 *  so we need critical section protect.
 *
 */
void disable_perclk(uint32_t clock)
{
    if ((clock < UART0_CLK) || (clock > I2S_CLK))
    {
        return;     /*Invalid setting mode.*/
    }

    SYSCTRL->SYS_CLK_CTRL &= ~(1 << clock);
}


/*
 *
 */
void pin_set_pullopt(uint32_t pin_number, uint32_t mode)
{
    uint32_t reg, base, mask_offset, mask;

    if ((pin_number >= 32) || (mode > 7))
    {
        return;     /*Invalid setting mode.*/
    }

    base = PULLOPT_BASE + (pin_number >> 3) * 4;
    mask_offset = (pin_number & 0x7) << 2;
    mask = 0xF << mask_offset;


    /*pin mux setting is share resource.*/
    reg = *((volatile unsigned int *) base);
    reg = reg & ~mask;
    reg = reg | (mode << mask_offset);

    *((volatile unsigned int *)base) =  reg;

    return;
}

/*
 * For multithread OS, and dynamic enable/disable peripheral environment,
 * Set this register has race condition issue, (in fact, almost impossible)
 *  so we need critical section protect.
 *
 */
void pin_set_drvopt(uint32_t pin_number, uint32_t mode)
{
    uint32_t reg, base, mask_offset, mask;

    if ((pin_number >= 32) || (mode > 3))
    {
        return;     /*Invalid setting mode.*/
    }

    base = DRV_BASE + (pin_number >> 4) * 4;
    mask_offset = (pin_number & 0xF) << 1;
    mask = 0x3 << mask_offset;


    /*pin mux setting is share resource.*/
    reg = *((volatile unsigned int *) base);
    reg = reg & ~mask;
    reg = reg | (mode << mask_offset);

    *((volatile unsigned int *)base) =  reg;

    return;
}

/*
 * For multithread OS, and dynamic enable/disable peripheral environment,
 * Set this register has race condition issue, so we need critical section protect.
 * In fact, it is almost impossible to dynamic change open drain.
 *
 */
void enable_pin_opendrain(uint32_t pin_number)
{
    uint32_t base, mask, reg;

    if (pin_number >= 32)
    {
        return;     /*Invalid setting mode.*/
    }

    base = OD_BASE ;
    mask = 1 << pin_number ;

    /*pin mux setting is share resource.*/
    reg = *((volatile unsigned int *) base);
    reg = reg | mask;
    *((volatile unsigned int *)base) =  reg;

    return;
}

/*
 * For multithread OS, and dynamic enable/disable peripheral environment,
 * Set this register has race condition issue, so we need critical section protect.
 * In fact, it is almost impossible to dynamic change open drain.
 *
 */
void disable_pin_opendrain(uint32_t pin_number)
{
    uint32_t base, mask, reg;

    if (pin_number >= 32)
    {
        return;     /*Invalid setting mode.*/
    }

    base = OD_BASE ;
    mask = ~(1 << pin_number);

    /*pin mux setting is share resource.*/
    reg = *((volatile unsigned int *) base);
    reg = reg & mask;
    *((volatile unsigned int *)base) =  reg;

    return;
}

sys_clk_sel_t Pll_Unlock_Check(void)
{
    sys_clk_sel_t sys_clk_mode;

    sys_clk_mode = Get_Ahb_System_Clk();                                                                /*get mcu system clock mode.*/

    if ((sys_clk_mode == SYS_CLK_48MHZ) || (sys_clk_mode == SYS_CLK_64MHZ))   /*PLL 48Mhz/64Mhz unlock check*/
    {

        if (  ((PLL_VIBIT_STATUS() == PLL_UNLOCK_VIBIT_A) && (PLL_BANK_VCO_STATUS() == PLL_UNLOCK_BANK_VCO_A)) ||
                ((PLL_VIBIT_STATUS() == PLL_UNLOCK_VIBIT_B) && (PLL_BANK_VCO_STATUS() == PLL_UNLOCK_BANK_VCO_B))
           )
        {
            enter_critical_section();

            Change_Ahb_System_Clk(SYS_32MHZ_CLK);

            leave_critical_section();

            sys_clk_mode = Get_Ahb_System_Clk();
        }
        else  if (((PLL_VIBIT_STATUS() == PLL_UNLOCK_VIBIT_A)) || ((PLL_VIBIT_STATUS() == PLL_UNLOCK_VIBIT_B)))
        {
            enter_critical_section();

            Change_Ahb_System_Clk(sys_clk_mode);

            leave_critical_section();

            sys_clk_mode = Get_Ahb_System_Clk();
        }
        else
        {
            /*PLL Stable*/
        }
    }

    return sys_clk_mode;
}

uint32_t Pll_Status_Check(void)
{

    if (  ((PLL_VIBIT_STATUS() == PLL_UNLOCK_VIBIT_A) && (PLL_BANK_VCO_STATUS() == PLL_UNLOCK_BANK_VCO_A)) ||
            ((PLL_VIBIT_STATUS() == PLL_UNLOCK_VIBIT_B) && (PLL_BANK_VCO_STATUS() == PLL_UNLOCK_BANK_VCO_B))
       )
    {
        return STATUS_INVALID_REQUEST;
    }
    else  if (((PLL_VIBIT_STATUS() == PLL_UNLOCK_VIBIT_A) && (PLL_BANK_VCO_STATUS() != PLL_UNLOCK_BANK_VCO_A)) ||
              ((PLL_VIBIT_STATUS() == PLL_UNLOCK_VIBIT_B) && (PLL_BANK_VCO_STATUS() != PLL_UNLOCK_BANK_VCO_B))
             )
    {
        return STATUS_EBUSY;
    }
    else  if (PLL_LOCK_STATUS() == PLL_LOCK_DOWN)
    {
        return STATUS_SUCCESS;
    }

    return STATUS_INVALID_REQUEST;
}

uint32_t Change_Ahb_System_Clk(sys_clk_sel_t sys_clk_mode)
{
    volatile uint32_t i, j = 0;
    volatile uint32_t pll_status = 0;

    if (sys_clk_mode > SYS_CLK_64MHZ)    /*Invalid parameter*/
    {
        return STATUS_ERROR;
    }

    SYSCTRL->SYS_CLK_CTRL = (SYSCTRL->SYS_CLK_CTRL & ~HCLK_SEL_MASK) | HCLK_SEL_32M;            //set pll to 32Mhz
    SYSCTRL->SYS_CLK_CTRL = (SYSCTRL->SYS_CLK_CTRL & ~BASEBAND_PLL_MASK);                       //baseband pll disable

    if (sys_clk_mode == SYS_CLK_32MHZ)    /*clock mode is 32Mhz*/
    {
    }
    else if (sys_clk_mode == SYS_CLK_48MHZ)        /*Set PLL to 48MHz*/
    {
        SYSCTRL->SYS_CLK_CTRL = (SYSCTRL->SYS_CLK_CTRL & ~BASEBAND_PLL_FREQ_MASK) | BASEBAND_PLL_48M;

#if (CHIP_VERSION == RT58X_MPB)
        PMU->PMU_BBPLL.bit.BBPLL_BANK1_MAN =  0; /*change BBPLL setting 2022/03/25 for MPB */
#endif

        for (j = 0; j < PLL_CHECK_COUNT; j++)
        {

            SYSCTRL->SYS_CLK_CTRL = (SYSCTRL->SYS_CLK_CTRL | BASEBAND_PLL_ENABLE);
            /*
            * need delay 200 us.
            * If code is not in cache, it will take more time for preload data.
            */
            for (i = 0; i < PLL_WAIT_PERIOD; i++)
            {
                __NOP();
            }

            if (PLL_LOCK_STATUS() == PLL_LOCK_DOWN)            //PLL lock down
            {
                SYSCTRL->SYS_CLK_CTRL = (SYSCTRL->SYS_CLK_CTRL & ~HCLK_SEL_MASK) | HCLK_SEL_BASEBAND_PLL;      /*config BASEBAND_PLL_ENABLE*/

                break;
            }
            else
            {
                SYSCTRL->SYS_CLK_CTRL = (SYSCTRL->SYS_CLK_CTRL & ~BASEBAND_PLL_MASK);                          /*baseband pll disable*/

                //delay > 32us
                for (i = 0; i < PLL_DELAY_PERIOD; i++)
                {
                    __NOP();
                }

                pll_status = Pll_Status_Check();

                if (pll_status == STATUS_INVALID_REQUEST)
                {
                    break;
                }
            }
        }
    }
    else if (sys_clk_mode == SYS_CLK_64MHZ)        /*Set PLL to 64MHz*/
    {

        SYSCTRL->SYS_CLK_CTRL = (SYSCTRL->SYS_CLK_CTRL & ~BASEBAND_PLL_FREQ_MASK) | BASEBAND_PLL_64M;

#if (CHIP_VERSION == RT58X_MPB)
        PMU->PMU_BBPLL.bit.BBPLL_BANK1_MAN =  1; /*change BBPLL setting 2022/03/25 for MPB */
#endif

        for (j = 0; j < PLL_CHECK_COUNT; j++)
        {
            SYSCTRL->SYS_CLK_CTRL = (SYSCTRL->SYS_CLK_CTRL | BASEBAND_PLL_ENABLE);      /*baseband pll enable*/

            /*
            * need delay 200 us.
            * If code is not in cache, it will take more time for preload data.
            */
            for (i = 0; i < PLL_WAIT_PERIOD; i++)
            {
                __NOP();
            }

            if (PLL_LOCK_STATUS() == PLL_LOCK_DOWN)            //PLL lock down
            {
                SYSCTRL->SYS_CLK_CTRL = (SYSCTRL->SYS_CLK_CTRL & ~HCLK_SEL_MASK) | HCLK_SEL_BASEBAND_PLL;        /*config BASEBAND_PLL_ENABLE*/

                break;
            }
            else
            {
                SYSCTRL->SYS_CLK_CTRL = (SYSCTRL->SYS_CLK_CTRL & ~BASEBAND_PLL_MASK);                            //baseband pll disable

                //delay > 32us
                for (i = 0; i < PLL_DELAY_PERIOD; i++)
                {
                    __NOP();
                }

                pll_status = Pll_Status_Check();

                if (pll_status == STATUS_INVALID_REQUEST)
                {
                    break;
                }
            }
        }
    }

    flash_timing_init();

    SystemFrequencyUpdate();

    Delay_Init();

    return STATUS_SUCCESS;
}

sys_clk_sel_t Get_Ahb_System_Clk(void)
{
    sys_clk_sel_t clk_mode = SYS_32MHZ_CLK;

    if ((SYSCTRL->SYS_CLK_CTRL & HCLK_SEL_MASK) == HCLK_SEL_32M)
    {
        clk_mode = SYS_32MHZ_CLK;
    }
    else if ((SYSCTRL->SYS_CLK_CTRL & HCLK_SEL_MASK) == HCLK_SEL_BASEBAND_PLL)
    {
        if ((SYSCTRL->SYS_CLK_CTRL & BASEBAND_PLL_MASK) != BASEBAND_PLL_ENABLE)
        {
            ASSERT();
        }
        if ((SYSCTRL->SYS_CLK_CTRL & BASEBAND_PLL_FREQ_MASK) == BASEBAND_PLL_48M)
        {
            clk_mode = SYS_48MHZ_CLK;
        }
        else if ((SYSCTRL->SYS_CLK_CTRL & BASEBAND_PLL_FREQ_MASK) == BASEBAND_PLL_64M)
        {
            clk_mode = SYS_64MHZ_CLK;
        }
    }
    else
    {
        ASSERT();
    }

    return clk_mode;
}

uint32_t get_random_number(void)
{
    /*
     * Please note this function is block mode. it will block about 4 ms ~ 6 ms
     * If you don't want the code block here, use interrupt mode.
     */
    uint32_t  temp;

    SYSCTRL->TRNG1 = 0x0;    /*select von Neumann */

    /*TRNG*/
    SYSCTRL->TRNG0 = 1;

    while (SYSCTRL->TRNG2 & 0x1)
        ;

    /*Clear interrut pending*/
    SYSCTRL->TRNG0 = 0x2;
    /*Get random number */
    temp = SYSCTRL->TRNG3;

    return temp;
}


void set_slow_clock_source(uint32_t mode)
{
    /*Slow clock selection.*/
    uint32_t  temp;

    if (mode > 3)
    {
        return;    /*Invalid mode*/
    }

    temp  = SYSCTRL->SYS_CLK_CTRL & ~(SLOW_CLOCK_SEL_MASK) ;
    temp |= (mode << SLOW_CLOCK_SEL_SHIFT);
    SYSCTRL->SYS_CLK_CTRL = temp;

}
void set_ext32k_pin(uint32_t pin_number)
{

    if (pin_number > 7) /*GPIO0~GPIO7 can input EXT_32K*/
    {
        return;     /*Invalid setting mode.*/
    }

    pin_set_mode(pin_number, MODE_EXT32K);

    return;
}
