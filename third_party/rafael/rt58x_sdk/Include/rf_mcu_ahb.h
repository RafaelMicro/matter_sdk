/**************************************************************************//**
 * @file     rf_mcu_ahb.h
 * @version
 * @brief    header file for rf mcu with ahb driver
 *
 ******************************************************************************/

#ifndef __RF_MCU_AHB_H__
#define __RF_MCU_AHB_H__

#include "rf_mcu_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define AHB_ALIGN_4(x)                          (((uintptr_t)(x) + 0x3) & ~0x3)

/* 0x00 */
#define RF_MCU_AHB_DMA_ADDR_REG \
        (COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_AHB_ADDR)


/* 0x04 */
typedef struct reg_rf_mcu_ahb_dma1_s
{
    uint32_t volatile DMA_MCU_ADDR      : 14;
    uint32_t volatile _RSVD_            : 2;
    uint32_t volatile DMA_LENGTH        : 16;
} reg_rf_mcu_ahb_dma1_t, *reg_rf_mcu_ahb_dma1_ptr_t;

#define RF_MCU_AHB_DMA1_PTR \
        ((reg_rf_mcu_ahb_dma1_ptr_t)&COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_DMA1)

#define RF_MCU_AHB_DMA1_REG \
        (COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_DMA1)


/* 0x08 */
typedef struct reg_rf_mcu_ahb_dma2_s
{
    uint32_t volatile DMA_TYPE          : 2;
    uint32_t volatile _RSVD_            : 30;
} reg_rf_mcu_ahb_dma2_t, *reg_rf_mcu_ahb_dma2_ptr_t;

#define RF_MCU_AHB_DMA2_PTR \
        ((reg_rf_mcu_ahb_dma2_ptr_t)&COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_DMA_TYPE)

#define RF_MCU_AHB_DMA2_REG \
        (COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_DMA_TYPE)


/* 0x0C */
typedef struct reg_rf_mcu_ahb_host_s
{
    uint32_t volatile HOST_CMD          : 8;
    uint32_t volatile WAKE_UP           : 1;
    uint32_t volatile SLEEP_EN          : 1;
    uint32_t volatile DEEPSLEEP_EN      : 1;
    uint32_t volatile SYS_RESET         : 1;
    uint32_t volatile _RSVD_            : 4;
    uint32_t volatile DMA_EN            : 1;
    uint32_t volatile _RSVD_2_          : 7;
    uint32_t volatile HOST_MODE_EN      : 1;
    uint32_t volatile HOST_MODE_DIS     : 1;
    uint32_t volatile _RSVD_3_          : 6;
} reg_rf_mcu_ahb_host_t, *reg_rf_mcu_ahb_host_ptr_t;

#define RF_MCU_AHB_HOST_CTRL_PTR \
        ((reg_rf_mcu_ahb_host_ptr_t)&COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST)

#define RF_MCU_AHB_HOST_CTRL_REG \
        (COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_HOST)


/* 0x14 */
typedef struct reg_rf_mcu_ahb_intr_enable_s
{
    uint32_t volatile MCU_SINT_0_EN     : 1;
    uint32_t volatile MCU_SINT_1_EN     : 1;
    uint32_t volatile MCU_SINT_2_EN     : 1;
    uint32_t volatile MCU_SINT_3_EN     : 1;
    uint32_t volatile MCU_SINT_4_EN     : 1;
    uint32_t volatile BMU_RX_EN         : 1;
    uint32_t volatile BMU_DATA_ERR_EN   : 1;
    uint32_t volatile RTC_WAKE_EN       : 1;
    uint32_t volatile DMA_DONE_EN       : 1;
    uint32_t volatile _RSVD_            : 7;
    uint32_t volatile AHB_RESET_PMU_EN  : 1;
    uint32_t volatile _RSVD_2_          : 15;
} reg_rf_mcu_ahb_intr_en_t, *reg_rf_mcu_ahb_intr_en_ptr_t;

#define RF_MCU_AHB_INTR_EN_PTR \
        ((reg_rf_mcu_ahb_intr_en_ptr_t)&COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_INTR_EN)

#define RF_MCU_AHB_INTR_EN_REG \
        (COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_INTR_EN)


/* 0x14 */
typedef struct reg_rf_mcu_ahb_intr_clear_s
{
    uint32_t volatile MCU_SINT_0_CLR    : 1;
    uint32_t volatile MCU_SINT_1_CLR    : 1;
    uint32_t volatile MCU_SINT_2_CLR    : 1;
    uint32_t volatile MCU_SINT_3_CLR    : 1;
    uint32_t volatile MCU_SINT_4_CLR    : 1;
    uint32_t volatile BMU_RX_CLR        : 1;
    uint32_t volatile BMU_DATA_ERR_CLR  : 1;
    uint32_t volatile RTC_WAKE_CLR      : 1;
    uint32_t volatile DMA_DONE_CLR      : 1;
    uint32_t volatile _RSVD_            : 23;
} reg_rf_mcu_ahb_intr_clear_t, *reg_rf_mcu_ahb_intr_clear_ptr_t;

#define RF_MCU_AHB_INTR_CLR_PTR \
        ((reg_rf_mcu_ahb_intr_clear_ptr_t)&COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_INTR_CLR)

#define RF_MCU_AHB_INTR_CLR_REG \
        (COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_INTR_CLR)


/* 0x18 */
typedef struct reg_rf_mcu_ahb_intr_status_s
{
    uint32_t volatile MCU_SINT_0_INTR   : 1;
    uint32_t volatile MCU_SINT_1_INTR   : 1;
    uint32_t volatile MCU_SINT_2_INTR   : 1;
    uint32_t volatile MCU_SINT_3_INTR   : 1;
    uint32_t volatile MCU_SINT_4_INTR   : 1;
    uint32_t volatile BMU_RX_INTR       : 1;
    uint32_t volatile BMU_DATA_ERR_INTR : 1;
    uint32_t volatile RTC_WAKE_INTR     : 1;
    uint32_t volatile DMA_DONE_INTR     : 1;
    uint32_t volatile _RSVD_            : 7;
    uint32_t volatile DMA_BUSY          : 1;
    uint32_t volatile _RSVD_2_          : 15;
} reg_rf_mcu_ahb_intr_status_t, *reg_rf_mcu_ahb_intr_status_ptr_t;

#define RF_MCU_AHB_INTR_STATUS_PTR \
        ((reg_rf_mcu_ahb_intr_status_ptr_t)&COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_INTR_STATUS)

#define RF_MCU_AHB_INTR_STATUS_REG \
        (COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_INTR_STATUS)


/* 0x1C */
typedef struct reg_rf_mcu_ahb_rx_info_s
{
    uint32_t volatile RX_Q_R_RDY        : 2;
    uint32_t volatile _RSVD_            : 6;
    uint32_t volatile MAC_RX_INFO       : 4;
    uint32_t volatile _RSVD_2_          : 4;
    uint32_t volatile RX_PKT_LEN        : 12;
    uint32_t volatile _RSVD_3_          : 4;
} reg_rf_mcu_ahb_rx_info_t, *reg_rf_mcu_ahb_rx_info_ptr_t;

#define RF_MCU_RX_INFO_PTR \
        ((reg_rf_mcu_ahb_rx_info_ptr_t)&COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_RX_INFO)

#define RF_MCU_RX_INFO_REG \
        (COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_RX_INFO)


/* 0x20 */
typedef struct reg_rf_mcu_ahb_tx_info_s
{
    uint32_t volatile TX_Q_W_RDY        : 8;
    uint32_t volatile MCU_STATE         : 8;
    uint32_t volatile CMDR_LEN          : 12;
    uint32_t volatile _RSVD_            : 4;
} reg_rf_mcu_ahb_tx_info_t, *reg_rf_mcu_ahb_tx_info_ptr_t;

#define RF_MCU_TX_INFO_PTR \
        ((reg_rf_mcu_ahb_tx_info_ptr_t)&COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_TX_INFO)

#define RF_MCU_TX_INFO_REG \
        (COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_TX_INFO)

/* 0x24 */
typedef struct reg_rf_mcu_ahb_sys_info_s
{
    uint32_t volatile SYS_READY         : 1;
    uint32_t volatile PWR_STATE         : 2;
    uint32_t volatile _RSVD_            : 1;
    uint32_t volatile CHIP_REV          : 4;
    uint32_t volatile _RSVD_2_          : 8;
    uint32_t volatile IO_CMD_BUSY       : 1;
    uint32_t volatile HOST_CMD_BUSY     : 1;
    uint32_t volatile _RSVD_3_          : 14;
} reg_rf_mcu_ahb_sys_info_t, *reg_rf_mcu_ahb_sys_info_ptr_t;

#define RF_MCU_SYS_INFO_PTR \
        ((reg_rf_mcu_ahb_sys_info_ptr_t)&COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_INFO)

#define RF_MCU_SYS_INFO_REG \
        (COMM_SUBSYSTEM_AHB->COMM_SUBSYSTEM_INFO)

void RfMcu_MemorySetAhb(uint16_t sys_addr, const uint8_t *p_data, uint16_t data_length);
void RfMcu_MemoryGetAhb(uint16_t sys_addr, uint8_t *p_data, uint16_t data_length);
void RfMcu_IoSetAhb(uint8_t queue_id, const uint8_t *p_data, uint16_t data_length);
void RfMcu_IoGetAhb(uint16_t queue_id, uint8_t *p_data, uint16_t data_length);
void RfMcu_HostCmdSetAhb(uint8_t cmd);
void RfMcu_HostCtrlAhb(uint32_t ctrl);
void RfMcu_InterruptEnableAhb(void);
void RfMcu_InterruptDisableAhb(void);
uint16_t RfMcu_InterruptEnGetAhb(void);
void RfMcu_InterruptEnSetAhb(uint16_t int_enable);
void RfMcu_InterruptClearAhb(uint32_t value);
void RfMcu_DmaBusyCheck(void);
bool RfMcu_RxQueueIsReadyAhb(void);
uint16_t RfMcu_RxQueueReadAhb(uint8_t *rx_data, RF_MCU_RXQ_ERROR *rx_queue_error);
bool RfMcu_EvtQueueIsReadyAhb(void);
uint16_t RfMcu_EvtQueueReadAhb(uint8_t *evt, RF_MCU_RX_CMDQ_ERROR *rx_evt_error);
bool RfMcu_TxQueueIsOccupiedAhb(uint8_t queue_id);
bool RfMcu_TxQueueFullCheckAhb(void);
RF_MCU_TXQ_ERROR RfMcu_TxQueueSendAhb(uint8_t queue_id, const uint8_t *tx_data, uint32_t data_length);
bool RfMcu_CmdQueueFullCheckAhb(void);
RF_MCU_TX_CMDQ_ERROR RfMcu_CmdQueueSendAhb(const uint8_t *cmd, uint32_t cmd_length);
RF_MCU_STATE RfMcu_McuStateReadAhb(void);
void RfMcu_SysRdySignalWaitAhb(void);
RF_MCU_PWR_STATE RfMcu_PowerStateGetAhb(void);
uint8_t RfMcu_PowerStateCheckAhb(void);
void RfMcu_HostWakeUpMcuAhb (void);
void RfMcu_SysInitNotifyAhb(void);
void RfMcu_AhbIsrHandler(COMM_SUBSYSTEM_ISR_t isr_cb);

#ifdef __cplusplus
}
#endif

#endif /* __RF_MCU_AHB_H__ */





