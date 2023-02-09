
#ifndef _HOST_MODE_PHY_H__
#define _HOST_MODE_PHY_H__
#include "Ruci.h"

#define ENABLE_RFE_TRX (1)


#define sRUCI_LENGTH(a)     (a-3)

#define COMMUNICATION_ISR_EVEN_DONE (1)
#define COMMUNICATION_ISR_TX_DONE (2)
#define COMMUNICATION_ISR_RX_DONE (4)
/*******************************************************************************
*   GLOBAL FUNCTIONS
*******************************************************************************/
typedef enum comm_subsystem_host_mode_data_rate
{
    COMM_SUBSYSTEM_HOST_MODE_PHY_2M = 0,
    COMM_SUBSYSTEM_HOST_MODE_PHY_1M               = 1,
    COMM_SUBSYSTEM_HOST_MODE_PHY_CODED        = 2,
    COMM_SUBSYSTEM_HOST_MODE_PHY_500K             = 2,
    COMM_SUBSYSTEM_HOST_MODE_PHY_200K             = 3,
    COMM_SUBSYSTEM_HOST_MODE_PHY_100K             = 4,
    COMM_SUBSYSTEM_HOST_MODE_PHY_50K              = 5,
    COMM_SUBSYSTEM_HOST_MODE_PHY_300K             = 6,
    COMM_SUBSYSTEM_HOST_MODE_PHY_150K             = 7,
    COMM_SUBSYSTEM_HOST_MODE_PHY_75K              = 8,
    COMM_SUBSYSTEM_HOST_MODE_PHY_MAX,
    COMM_SUBSYSTEM_HOST_MODE_PHY_CODED_PHY_S2,
    COMM_SUBSYSTEM_HOST_MODE_PHY_CODED_PHY_S8
} COMM_SUBSYSTEM_HOST_MODE_DATA_RATE;
typedef enum _comm_subsystem_host_mode_phy_init_error
{
    COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_SUCCEED = 0,
    BLE_INITIALIZATION_FAIL,
    ZIGBEE_INITIALIZATION_FAIL,
    SUBG_INITIALIZATION_FAIL,
} COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_ERROR;

typedef enum coded_type
{
    S8 = 0,
    S2,
} CODED_TYPE;

typedef enum HAL_MODE
{
    HAL_BLE             = 0,
    HAL_SUBG           = 1,
    HAL_ZIGBEE          = 2,
} eHAL_MODE;

typedef enum
{
    COMM_SUBSYSTEM_HOST_MODE_EVENT_SUCCESS = 0,
    COMM_SUBSYSTEM_HOST_MODE_CNF_EVENT_INVALID_CMD,
    COMM_SUBSYSTEM_HOST_MODE_CNF_EVENT_RX_BUSY,
    COMM_SUBSYSTEM_HOST_MODE_CNF_EVENT_TX_BUSY,
    COMM_SUBSYSTEM_HOST_MODE_CNF_EVENT_UNSUPPORT_CMD,
    COMM_SUBSYSTEM_HOST_MODE_CNF_EVENT_NOT_AVAILABLE,
    COMM_SUBSYSTEM_HOST_MODE_CNF_EVENT_CONTENT_ERROR,
    COMM_SUBSYSTEM_HOST_MODE_RSP_EVENT_NOT_AVAILABLE,
    COMM_SUBSYSTEM_HOST_MODE_RSP_EVENT_CONTENT_ERROR
} COMM_SUBSYSTEM_HOST_MODE_EVENT_STATUS;
typedef enum
{
    BRX = 0,
    CRX,

} COMM_SUBSYSTEM_HOST_RX_MODE;
typedef enum
{
    POWER_TRIM_DISABLE = 0,
    POWER_TRIM_ENABLE,

} POWER_TRIM_TYPE;



void commsubsystem_host_mode_trx_stop(void);

void commsubsystem_host_mode_tx_queue_send(uint8_t *pTxQueue, uint16_t TxQueueLen);

void commsubsystem_host_mode_set_RF_Freq(eHAL_MODE halType, uint32_t target_freq);

bool commsubsystem_host_mode_ble_mac_init(uint32_t SfdContent, uint8_t WhiteningEn, uint8_t WhiteningInitValue, uint32_t CrcInitValue);

bool commsubsystem_host_mode_ble_phy_init(COMM_SUBSYSTEM_HOST_MODE_DATA_RATE radio_mode);

bool commsubsystem_host_mode_subg_sfd_init(uint32_t SfdContent);

bool commsubsystem_host_mode_subg_preamble_init(void);

bool commsubsystem_host_mode_subg_mac_init(void);

bool commsubsystem_host_mode_subg_phy_init(COMM_SUBSYSTEM_HOST_MODE_DATA_RATE radio_mode);

bool commsubsystem_host_mode_zigbee_mac_init(void);

bool  commsubsystem_host_mode_single_tone_init(void);

bool  commsubsystem_host_mode_single_tone_en(void);

bool  commsubsystem_host_mode_single_tone_disable(void);

bool commsubsystem_host_mode_set_ruci_cmd(uint8_t *cmd, uint32_t cmd_length, uint8_t *result);

COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_ERROR commsubsystem_host_mode_phy_init(eHAL_MODE porotocol);

uint8_t commsubsystem_host_mode_report_rssi(void);

bool commsubsystem_host_mode_rf_calibration(eHAL_MODE porotocol, uint8_t rf_band, ruci_para_set_calibration_enable_event_t *rf_calib_report);

bool commsubsystem_host_mode_get_crc_count(uint32_t *crc_count_get, uint32_t *crc_fail_count_get);
#endif













