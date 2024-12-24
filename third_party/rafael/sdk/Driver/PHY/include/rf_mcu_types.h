/**************************************************************************//**
 * @file     rf_mcu_types.h
 * @version
 * @brief    header file for rf mcu types
 *
 ******************************************************************************/

#ifndef __RF_MCU_TYPES_H__
#define __RF_MCU_TYPES_H__

#include "cm3_mcu.h"
#include "rf_mcu_chip.h"
#include "project_config.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define COMM_SUBSYSTEM_DMA_INT_ENABLE       (BIT8)
#define COMM_SUBSYSTEM_DMA_INT_CLR          (COMM_SUBSYSTEM_DMA_INT_ENABLE)
#define COMM_SUBSYSTEM_SYS_RDY              (BIT0)

#define COMM_SUBSYS_TX_CMD_QUEUE_ID         (7)
#define COMM_SUBSYS_RX_CMD_QUEUE_ID         (1)
#define COMM_SUBSYS_RX_QUEUE_ID             (0)
#define COMM_SUBSYS_MCU_INIT_SUCCEED_2      (0xEA)

#define RF_MCU_SW_0_INTR                    (BIT0)
#define RF_MCU_SW_1_INTR                    (BIT1)
#define RF_MCU_SW_2_INTR                    (BIT2)
#define RF_MCU_SW_3_INTR                    (BIT3)
#define RF_MCU_SW_4_INTR                    (BIT4)
#define RF_MCU_BMU_RX_VALID_INTR            (BIT5)
#define RF_MCU_BMU_DATA_ERROR_INTR          (BIT6)
#define RF_MCU_RTC_WAKE_INTR                (BIT7)
#define RF_MCU_DMA_DONE_INTR                (BIT8)

#define COMM_SUBSYSTEM_INT_ALL_MASK         (0x1FF)

#ifndef COMM_SUBSYSTEM_INT_ENABLE
#define COMM_SUBSYSTEM_INT_ENABLE           ( \
                                            RF_MCU_SW_0_INTR            | \
                                            RF_MCU_SW_1_INTR            | \
                                            RF_MCU_SW_2_INTR            | \
                                            RF_MCU_SW_3_INTR            | \
                                            RF_MCU_SW_4_INTR            | \
                                            RF_MCU_BMU_RX_VALID_INTR    | \
                                            RF_MCU_BMU_DATA_ERROR_INTR  | \
                                            0 \
                                            )
#endif

#define COMM_SUBSYSTEM_INT_STATUS_MASK      (COMM_SUBSYSTEM_INT_ENABLE)
#define COMM_SUBSYSTEM_DMA_IS_BUSY          (BIT16)

#define COMM_SUBSYS_TX_CMD_Q_AVAILABLE      (BIT7)
#define COMM_SUBSYS_RX_CMD_Q_AVAILABLE      (BIT1)
#define COMM_SUBSYS_RX_Q_AVAILABLE          (BIT0)

#define COMM_SUBSYS_MAX_RX_Q_LEN            (0xFFF)
#define COMM_SUBSYS_MAX_RX_CMDQ_LEN         (0xFFF)
#define COMM_SUBSYS_PROGRAM_START_ADDR      (0x8000)
#define COMM_SUBSYS_FW_PAGE_SIZE            (0x8000)
#define RF_MCU_PATCH_START_ADDR             (0xF000)
#define RF_MCU_PATCH_MAX_SIZE               (0x1000)
#define RF_MCU_M0_PROGRAM_START_ADDR        (0x1000)
#define RF_MCU_M0_CONST_START_ADDR          (0x7000)
#define RF_MCU_MP_CONST_START_ADDR          (0x4040)

#define RF_MCU_PM_SEL_REG_ADDR              (0x0448)

/* The register are DMA transfer type */
typedef uint8_t COMM_SUBSYS_DMA_TYPE;
#define COMM_SUBSYSTEM_DMA_TYPE_IO_READ                (COMM_SUBSYS_DMA_TYPE)(0x0) // Move data from RX FIFO to Host memory
#define COMM_SUBSYSTEM_DMA_TYPE_IO_WRITE               (COMM_SUBSYS_DMA_TYPE)(0x1) // Move data from TX FIFO to commsubsystem memory
#define COMM_SUBSYSTEM_DMA_TYPE_MEM_READ               (COMM_SUBSYS_DMA_TYPE)(0x2) // Move data from commsubsystem memory to Host memory
#define COMM_SUBSYSTEM_DMA_TYPE_MEM_WRITE              (COMM_SUBSYS_DMA_TYPE)(0x3) // Move data from Host memory to commsubsystem memory

/* The register are defined to control commsubsystem by Host, each of bit is WRITE only.*/
#define COMM_SUBSYSTEM_HOST_CTRL_WAKE_UP               0x00000100             /**< bit8 */
#define COMM_SUBSYSTEM_HOST_CTRL_SLEEP_EN              0x00000200             /**< bit9 */
#define COMM_SUBSYSTEM_HOST_CTRL_DEEP_SLEEP_EN         0x00000400             /**< bit10 */
#define COMM_SUBSYSTEM_HOST_CTRL_RESET                 0x00000800             /**< bit11 */
#define COMM_SUBSYSTEM_HOST_CTRL_DMA_ENABLE            0x00010000             /**< bit16 */
#define COMM_SUBSYSTEM_HOST_CTRL_ENABLE_HOST_MODE      0x01000000             /**< bit24 */
#define COMM_SUBSYSTEM_HOST_CTRL_DISABLE_HOST_MODE     0x02000000             /**< bit25 */

#if (CFG_RF_MCU_CTRL_TYPE == RF_MCU_CTRL_BY_SPI)
#define RF_MCU_SPI_GPIO_ISR_SELECT            22
#endif

#define HOST_MCU_DATA_MEM_VIEW(x)       ((x) - 0x8000)
#define HOST_MCU_ENDIAN_SWAP_16B(x)     (((x >> 8) & 0x00FF) | ((x << 8) & 0xFF00))
#define HOST_MCU_SHARE_Q_ADDR_GET(staddr, offset, blksize)                                      \
                                        HOST_MCU_DATA_MEM_VIEW((staddr) + (offset) * (blksize))
#ifndef UNUSED
#define UNUSED(x) ((void)x)
#endif

typedef enum rf_mcu_init_status_e
{
    RF_MCU_INIT_NO_ERROR           = 0,
    RF_MCU_FIRMWARE_LOADING_FAIL,
    RF_MCU_CONST_LOADING_FAIL,
    RF_MCU_PATCH_LOADING_FAIL,
    RF_MCU_CPU_AWAKE_FAIL,
} RF_MCU_INIT_STATUS;


typedef enum rf_mcu_tx_queue_err_e
{
    RF_MCU_TXQ_ERR_INIT             = 0,
    RF_MCU_TXQ_SET_SUCCESS,
    RF_MCU_TXQ_FULL,
} RF_MCU_TXQ_ERROR;


typedef enum rf_mcu_tx_cmd_err_e
{
    RF_MCU_TX_CMDQ_ERR_INIT         = 0,
    RF_MCU_TX_CMDQ_SET_SUCCESS,
    RF_MCU_TX_CMDQ_FULL,
} RF_MCU_TX_CMDQ_ERROR;


typedef enum rf_mcu_rx_queue_err_e
{
    RF_MCU_RXQ_ERR_INIT             = 0,
    RF_MCU_RXQ_GET_SUCCESS,
    RF_MCU_RXQ_NOT_AVAILABLE,
} RF_MCU_RXQ_ERROR;


typedef enum rf_mcu_rx_cmd_err_e
{
    RF_MCU_RX_CMDQ_ERR_INIT         = 0,
    RF_MCU_RX_CMDQ_GET_SUCCESS,
    RF_MCU_RX_CMDQ_NOT_AVAILABLE,
} RF_MCU_RX_CMDQ_ERROR;


typedef enum rf_mcu_pwr_state_e
{
    RF_MCU_PWR_STATE_TRANSITION     = 0x0,
    RF_MCU_PWR_STATE_DEEP_SLEEP,
    RF_MCU_PWR_STATE_SLEEP,
    RF_MCU_PWR_STATE_NORMAL,
} RF_MCU_PWR_STATE;


typedef enum rf_mcu_pm_page_sel_e
{
    RF_MCU_PM_PAGE_SEL_0            = 0,
    RF_MCU_PM_PAGE_SEL_1,
    RF_MCU_PM_PAGE_SEL_2,
} RF_MCU_PM_PAGE_SEL;


typedef enum _rf_mcu_state
{
    RF_MCU_STATE_NULL               = 0,
    RF_MCU_STATE_INIT_SUCCEED       = BIT0,
    RF_MCU_STATE_EVENT_DONE         = BIT1,
    RF_MCU_STATE_RSVD_BIT2          = BIT2,
    RF_MCU_STATE_TX_FAIL            = BIT3,
    RF_MCU_STATE_TX_CSMACA_FAIL     = BIT4,
    RF_MCU_STATE_TX_NO_ACK          = BIT5,
    RF_MCU_STATE_TX_SUC_AR_FP       = BIT6,
    RF_MCU_STATE_TX_SUC_AR          = BIT7,
} RF_MCU_STATE;


typedef void (* COMM_SUBSYSTEM_ISR_t)(uint8_t interrupt_state_value);
typedef RF_MCU_INIT_STATUS (* RF_MCU_PATCH_ENTRY)(const uint8_t *, uint32_t);

typedef struct __attribute__((packed)) patch_cfg_ctrl_s
{
    uint16_t cfg_sfr;
    uint8_t cfg_val[4];
} patch_cfg_ctrl_t;

typedef struct
{
    /*Because the callback function feature, the COMM_SUBSYSTEM_Handler NOW would not take responsibilty for cleaning the interrupt status.
        The ISR must take charge for cleaning the interrupt status itself.
        The ISR writer must take care this thing.
    */
    COMM_SUBSYSTEM_ISR_t  commsubsystem_isr;           /**< user application ISR handler. */
    uint32_t  content;
} COMM_SUBSYSTEM_ISR_CONFIG;


typedef union
{
    uint8_t u8;
    struct _comm_subsystem_interrupt
    {
        uint8_t EVENT_DONE : 1;
        uint8_t TX_DONE : 1;
        uint8_t RFB_TRAP : 1;
        uint8_t RX_TIMEOUT : 1;
        uint8_t MHR_DONE : 1;
        uint8_t RX_DONE : 1;
        uint8_t SPI_DATA_TRANSFER_ERROR : 1;
        uint8_t RTC_WAKEUP : 1;
    } bf; // bit-field
} COMM_SUBSYSTEM_INTERRUPT;

#ifdef __cplusplus
}
#endif

#endif /* __RF_MCU_TYPES_H__ */





