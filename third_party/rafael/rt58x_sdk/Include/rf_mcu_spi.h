/**************************************************************************//**
 * @file     rf_mcu_spi.h
 * @version
 * @brief    header file for rf mcu with spi driver
 *
 ******************************************************************************/

#ifndef __RF_MCU_SPI_H__
#define __RF_MCU_SPI_H__

#include "rf_mcu_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if (CFG_RF_MCU_CTRL_TYPE == RF_MCU_CTRL_BY_SPI)

#define RF_MCU_SPI_SFR_ADDR_CHIP_STATE          0x00
#define RF_MCU_SPI_SFR_ADDR_CHIP_ID             0x01
#define RF_MCU_SPI_SFR_ADDR_ISR_STATE           0x02
#define RF_MCU_SPI_SFR_ADDR_ISR_STATE_CLR       0x02
#define RF_MCU_SPI_SFR_ADDR_RXQ_CMD_STATE       0x03
#define RF_MCU_SPI_SFR_ADDR_RX_LENGTH           0x04
#define RF_MCU_SPI_SFR_ADDR_RX_INFO             0x05
#define RF_MCU_SPI_SFR_ADDR_CMD_LENGTH_1        0x06
#define RF_MCU_SPI_SFR_ADDR_CMD_LENGTH_2        0x07
#define RF_MCU_SPI_SFR_ADDR_TXQ_STATE           0x08
#define RF_MCU_SPI_SFR_ADDR_MCU_STATE           0x09
#define RF_MCU_SPI_SFR_ADDR_ISR_ENABLE_MASK     0x0A
#define RF_MCU_SPI_SFR_ADDR_HOST_CMD_STATE      0x0B
#define RF_MCU_SPI_SFR_ADDR_MODE_SELECT         0x0E
#define RF_MCU_SPI_SFR_ADDR_HOST_CTRL           0x0F


#define RF_MCU_HOST_CMD_BUSY_BIT                0x40
#define RF_MCU_SPI_CMD_BUSY_BIT                 0x80

#define RF_MCU_HOST_CMD_STATE_MUC_INIT_SUCCEED  0x01
#define RF_MCU_HOST_CMD_STATE_EVENT_DONE        0x02
#define RF_MCU_HOST_CMD_STATE_TX_FAIL           0x04
#define RF_MCU_HOST_CMD_STATE_HOST_WAKE_UP      0x08

#define RF_MCU_SPI_HOST_MODE_ENABLE_BIT         0x01
#define RF_MCU_SPI_ICE_MODE_ENABLE_BIT          0x04

#define RF_MCU_SPI_HOST_CTRL_HOST_WAKEUP        0x01
#define RF_MCU_SPI_HOST_CTRL_RESET_BIT          0x08

void RfMcu_SpiSetSFR(uint8_t reg_addr, uint8_t value);
uint8_t RfMcu_SpiGetSFR(uint8_t reg_addr);
void RfMcu_MemorySetSpi(uint16_t sys_addr, const uint8_t *p_data, uint16_t data_length);
void RfMcu_MemoryGetSpi(uint16_t sys_addr, uint8_t *p_data, uint16_t data_length);
void RfMcu_IoSetSpi(uint8_t queue_id, const uint8_t *p_data, uint16_t data_length);
void RfMcu_IoGetSpi(uint16_t queue_id, uint8_t *p_data, uint16_t data_length);
void RfMcu_HostCmdSetSpi(uint8_t cmd);
void RfMcu_HostCtrlSpi(uint32_t ctrl);
void RfMcu_InterruptEnableSpi(void);
void RfMcu_InterruptDisableSpi(void);
uint16_t RfMcu_InterruptEnGetSpi(void);
void RfMcu_InterruptEnSetSpi(uint16_t int_enable);
void RfMcu_InterruptClearSpi(uint32_t value);
bool RfMcu_RxQueueIsReadySpi(void);
uint16_t RfMcu_RxQueueReadSpi(uint8_t *rx_data, RF_MCU_RXQ_ERROR *rx_queue_error);
bool RfMcu_EvtQueueIsReadySpi(void);
uint16_t RfMcu_EvtQueueReadSpi(uint8_t *evt, RF_MCU_RX_CMDQ_ERROR *rx_evt_error);
bool RfMcu_TxQueueIsOccupiedSpi(uint8_t queue_id);
bool RfMcu_TxQueueFullCheckSpi(void);
RF_MCU_TXQ_ERROR RfMcu_TxQueueSendSpi(uint8_t queue_id, const uint8_t *tx_data, uint32_t data_length);
bool RfMcu_CmdQueueFullCheckSpi(void);
RF_MCU_TX_CMDQ_ERROR RfMcu_CmdQueueSendSpi(const uint8_t *cmd, uint32_t cmd_length);
RF_MCU_STATE RfMcu_McuStateReadSpi(void);
void RfMcu_SysRdySignalWaitSpi(void);
RF_MCU_PWR_STATE RfMcu_PowerStateGetSpi(void);
uint8_t RfMcu_PowerStateCheckSpi(void);
void RfMcu_HostWakeUpMcuSpi (void);
void RfMcu_SysInitNotifySpi(void);
void RfMcu_SpiIceModeCtrl(bool enable);

void RfMcu_SpiIsrHandler(COMM_SUBSYSTEM_ISR_t isr_cb);
void RfMcu_SpiInit(void);

#endif /* CFG_RF_MCU_CTRL_TYPE */

#ifdef __cplusplus
}
#endif

#endif /* __RF_MCU_SPI_H__ */





