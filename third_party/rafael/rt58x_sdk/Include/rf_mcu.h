/**************************************************************************//**
 * @file     rf_mcu.h
 * @version
 * @brief    header file for rf mcu
 *
 ******************************************************************************/

#ifndef __RF_MCU_H__
#define __RF_MCU_H__

#include "cm3_mcu.h"
#include "rf_mcu_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* External APIs*/
extern RF_MCU_INIT_STATUS RfMcu_SysInit(
    bool load_image,
    const uint8_t *p_sys_image,
    uint32_t image_size,
    COMM_SUBSYSTEM_ISR_CONFIG rf_mcu_isr_cfg,
    RF_MCU_INIT_STATUS rf_mcu_init_state
);

extern void RfMcu_InterruptDisableAll(void);
extern void RfMcu_InterruptEnableAll(void);
extern void RfMcu_InterruptClear(uint32_t value);
extern RF_MCU_STATE RfMcu_McuStateRead(void);
extern void RfMcu_HostCmdSet(uint8_t cmd);

extern bool RfMcu_RxQueueCheck(void);
extern uint16_t RfMcu_RxQueueRead(uint8_t *rx_data, RF_MCU_RXQ_ERROR *rx_queue_error);
extern bool RfMcu_EvtQueueCheck(void);
extern uint16_t RfMcu_EvtQueueRead(uint8_t *evt, RF_MCU_RX_CMDQ_ERROR *rx_evt_error);
extern bool RfMcu_CmdQueueFullCheck(void);
extern RF_MCU_TX_CMDQ_ERROR RfMcu_CmdQueueSend(const uint8_t *cmd, uint32_t cmd_length);
extern bool RfMcu_TxQueueFullCheck(void);
extern RF_MCU_TXQ_ERROR RfMcu_TxQueueSend(uint8_t *tx_data, uint32_t data_length);


/* Internal APIs */
extern RF_MCU_INIT_STATUS RfMcu_SysInitWithPatch(
    bool load_image,
    const uint8_t *p_sys_image,
    uint32_t sys_image_size,
    const uint8_t *p_patch_image,
    uint32_t patch_image_size,
    COMM_SUBSYSTEM_ISR_CONFIG rf_mcu_isr_cfg,
    RF_MCU_INIT_STATUS rf_mcu_init_state
);

extern RF_MCU_INIT_STATUS RfMcu_SysInitWithConst(
    bool load_image,
    const uint8_t *p_sys_image,
    uint32_t sys_image_size,
    const uint8_t *p_const,
    uint32_t const_size,
    COMM_SUBSYSTEM_ISR_CONFIG rf_mcu_isr_cfg,
    RF_MCU_INIT_STATUS rf_mcu_init_state
);

void RfMcu_MemorySet(uint16_t sys_addr, const uint8_t *p_data, uint16_t data_length);
void RfMcu_MemoryGet(uint16_t sys_addr, uint8_t *p_data, uint16_t data_length);
void RfMcu_IoSet(uint8_t queue_id, const uint8_t *p_data, uint16_t data_length);
void RfMcu_IoGet(uint16_t queue_id, uint8_t *p_data, uint16_t data_length);
void RfMcu_HostWakeUpMcu(void);
void RfMcu_HostCtrl(uint32_t ctrl);
void RfMcu_HostModeEnable(void);
void RfMcu_HostModeDisable(void);
void RfMcu_HostResetMcu(void);
uint16_t RfMcu_InterruptEnGet(void);
void RfMcu_InterruptEnSet(uint16_t int_enable);
RF_MCU_TXQ_ERROR RfMcu_TxQueueSendById(uint8_t queue_id, const uint8_t *tx_data, uint32_t data_length);
void RfMcu_SysRdySignalWait(void);
uint8_t RfMcu_PowerStateCheck(void);
void RfMcu_SysInitNotify(void);
void RfMcu_DmaInit(void);
void RfMcu_SpiSfrSet(uint8_t sfrOffset, uint8_t value);
uint8_t RfMcu_SpiSfrGet(uint8_t sfrOffset);
void RfMcu_RegSet(uint16_t reg_address, uint32_t value);
uint32_t RfMcu_RegGet(uint16_t reg_address);
void RfMcu_PmPageSelect(RF_MCU_PM_PAGE_SEL page);
void RfMcu_PmToDmControl(bool Enable);
void RfMcu_ImageLoad(const uint8_t *fw_image, uint32_t image_size);
void RfMcu_IsrInit(COMM_SUBSYSTEM_ISR_t isr, uint32_t content);
void RfMcu_IsrHandler(void);
void RfMcu_IceModeCtrl(bool enable);


#ifdef __cplusplus
}
#endif

#endif /* __RF_MCU_H__ */





