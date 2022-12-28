
#ifndef _HOST_MODE_BLE_H__
#define _HOST_MODE_BLE_H__

#include "mp_tool_phy.h"

typedef enum _packet_data_type
{
    PRBS9 = 0,
    ONE_ZERO = 1,
    ONE_ZERO_INTERLEAVER = 2,
    RESERVE = 3,
    ALL_ONE = 4, //not supported
    ALL_ZERO,
    ZERO_ONE,
    ZERO_ONE_INTERLEAVER,
    VENDOR_SPECIFIC,
    //TBD modified by documemt
} PACKET_DATA_TYPE;

void Single_tone_start(uint32_t channel_idx, COMM_SUBSYSTEM_HOST_MODE_DATA_RATE radio_mode, eHAL_MODE porotocol);

void Single_tone_Stop(void);

uint8_t commsubsystem_host_mode_Tx_Test( uint32_t channel_idx, COMM_SUBSYSTEM_HOST_MODE_DATA_RATE radio_mode, PACKET_DATA_TYPE dataType, eHAL_MODE porotocol);

void commsubsystem_host_mode_Rx_Test(uint32_t channel_idx, COMM_SUBSYSTEM_HOST_MODE_DATA_RATE radio_mode, COMM_SUBSYSTEM_HOST_RX_MODE rx_mode, POWER_TRIM_TYPE power_trim_en, eHAL_MODE porotocol);

uint8_t Read_Tx_power_idx(void);

void Write_Tx_power_idx(uint8_t tx_power_idx);

uint8_t Report_Rx_RSSI(void);

uint16_t Report_Rx_CRC(void);

void RF_Cablibtration_Enable(eHAL_MODE porotocol, uint8_t rf_band, ruci_para_set_calibration_enable_event_t *rf_calib_report);
#endif













