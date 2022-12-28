/**************************************************************************//**
 * @file     rt569mp_init.h
 * @version
 * @brief    host layer phy related configure

 ******************************************************************************/

#ifndef __RT569MP_INIT_H__
#define __RT569MP_INIT_H__

#ifdef __cplusplus
extern "C"
{
#endif


/**************************************************************************************************
 *    Global Prototypes
 *************************************************************************************************/
#if (RF_MCU_CHIP_MODEL == RF_MCU_CHIP_569MP)
void RfMcu_PhyExtMemInit(void);
#endif /* CHECK_FW_RAM_BASE */

#ifdef __cplusplus
}
#endif
#endif /* __RT569MP_INIT_H__ */

