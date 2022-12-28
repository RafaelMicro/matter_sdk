/**************************************************************************//**
 * @file     gpio_reg.h
 * @version  V1.00
 * @brief    GPIO register definition header file
 *
 * @copyright
 *****************************************************************************/
#ifndef ___GPIO_REG_H__
#define ___GPIO_REG_H__

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif



typedef struct
{
    __IO uint32_t STATE;
    __IO uint32_t INT_STATUS;     /*0x04*/
    __IO uint32_t OUTPUT_EN;      /*0x08*/
    __IO uint32_t INPUT_EN;       /*0x0C*/
    __IO uint32_t ENABLE_INT;     /*0x10*/
    __IO uint32_t DISABLE_INT;    /*0x14*/
    __IO uint32_t EDGE_TRIG;      /*0x18*/
    __IO uint32_t LEVEL_TRIG;     /*0x1C*/
    __IO uint32_t POSTITIVE;      /*0x20*/
    __IO uint32_t NEGATIVE;       /*0x24*/
    __IO uint32_t BOTH_EDGE_EN;   /*0x28*/
    __IO uint32_t BOTH_EDGE_CLR;  /*0x2C*/
    __IO uint32_t EDGE_INT_CLR;   /*0x30*/

    __IO uint32_t Reserve[3];

    __IO uint32_t DEBOUCE_EN;     /*0x40 Enable Debounce*/
    __IO uint32_t DEBOUCE_DIS;    /*0x44 Disable Debounce*/
    __IO uint32_t DEBOUNCE_TIME;

} GPIO_T;

/*register alias name*/
#define OUTPUT_HIGH STATE
#define OUTPUT_LOW  INT_STATUS



#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

#endif
