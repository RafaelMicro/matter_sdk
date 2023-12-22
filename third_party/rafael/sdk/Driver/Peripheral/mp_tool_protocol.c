

#include "cm3_mcu.h"
#include "mp_tool_protocol.h"
#include "mp_tool_phy.h"
#include "comm_subsystem_ahb_phy.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
extern void commsubsystem_write_memory(uint16_t reg_address, const uint8_t *p_tx_data, uint16_t tx_data_length);
extern void commsubsystem_read_memory(uint16_t reg_address, uint8_t *p_rx_data, uint16_t rx_data_length);




#define RX_ON (1)
#define RX_Q_AVAILABLE  (1<<0)
#define RX_QUEUE_ID (0)
#define DEFAULT_PAYLOAD_LENGTH (37)
#define DEFAULT_DATA_TYPE (PRBS9)
#define DEFAULT_SFD (0x71764129)
#define DEFAULT_CRC_INIT (0x00555555)
#define REGISTER_LENGTH (4)

#define PACKET_CONTENT_LENGTH (512)
#define SET_RUCI_PARA_SET_TX_CONTROL_FIELD_TEST(msg,packet_length, mac_control_in, mac_dsn_in)        \
        do{                                                                                                            \
        ((ruci_para_set_tx_control_field_t *)msg)->ruci_header.u8                 = RUCI_PCI_DATA_HEADER;                   \
        ((ruci_para_set_tx_control_field_t *)msg)->sub_header                     = RUCI_CODE_SET_TX_CONTROL_FIELD;         \
        ((ruci_para_set_tx_control_field_t *)msg)->length                         = RUCI_PARA_LEN_SET_TX_CONTROL_FIELD+packet_length;     \
        ((ruci_para_set_tx_control_field_t *)msg)->mac_control                    = mac_control_in;                         \
        ((ruci_para_set_tx_control_field_t *)msg)->mac_dsn                        = mac_dsn_in;                             \
        }while(0)


static const uint8_t      prbs_content[]    = {0xFF, 0xC1, 0xFB, 0xE8, 0x4C, 0x90, 0x72, 0x8B,
                                               0xE7, 0xB3, 0x51, 0x89, 0x63, 0xAB, 0x23, 0x23,
                                               0x02, 0x84, 0x18, 0x72, 0xAA, 0x61, 0x2F, 0x3B,
                                               0x51, 0xA8, 0xE5, 0x37, 0x49, 0xFB, 0xC9, 0xCA,
                                               0x0C, 0x18, 0x53, 0x2C, 0xFD, 0x45, 0xE3, 0x9A,
                                               0xE6, 0xF1, 0x5D, 0xB0, 0xB6, 0x1B, 0xB4, 0xBE,
                                               0x2A, 0x50, 0xEA, 0xE9, 0x0E, 0x9C, 0x4B, 0x5E,
                                               0x57, 0x24, 0xCC, 0xA1, 0xB7, 0x59, 0xB8, 0x87,
                                               0xFF, 0xE0, 0x7D, 0x74, 0x26, 0x48, 0xB9, 0xC5,
                                               0xF3, 0xD9, 0xA8, 0xC4, 0xB1, 0xD5, 0x91, 0x11,
                                               0x01, 0x42, 0x0C, 0x39, 0xD5, 0xB0, 0x97, 0x9D,
                                               0x28, 0xD4, 0xF2, 0x9B, 0xA4, 0xFD, 0x64, 0x65,
                                               0x06, 0x8C, 0x29, 0x96, 0xFE, 0xA2, 0x71, 0x4D,
                                               0xF3, 0xF8, 0x2E, 0x58, 0xDB, 0x0D, 0x5A, 0x5F,
                                               0x15, 0x28, 0xF5, 0x74, 0x07, 0xCE, 0x25, 0xAF,
                                               0x2B, 0x12, 0xE6, 0xD0, 0xDB, 0x2C, 0xDC, 0xC3,
                                               0x7F, 0xF0, 0x3E, 0x3A, 0x13, 0xA4, 0xDC, 0xE2,
                                               0xF9, 0x6C, 0x54, 0xE2, 0xD8, 0xEA, 0xC8, 0x88,
                                               0x00, 0x21, 0x86, 0x9C, 0x6A, 0xD8, 0xCB, 0x4E,
                                               0x14, 0x6A, 0xF9, 0x4D, 0xD2, 0x7E, 0xB2, 0x32,
                                               0x03, 0xC6, 0x14, 0x4B, 0x7F, 0xD1, 0xB8, 0xA6,
                                               0x79, 0x7C, 0x17, 0xAC, 0xED, 0x06, 0xAD, 0xAF,
                                               0x0A, 0x94, 0x7A, 0xBA, 0x03, 0xE7, 0x92, 0xD7,
                                               0x15, 0x09, 0x73, 0xE8, 0x6D, 0x16, 0xEE, 0xE1,
                                               0x3F, 0x78, 0x1F, 0x9D, 0x09, 0x52, 0x6E, 0xF1,
                                               0x7C, 0x36, 0x2A, 0x71, 0x6C, 0x75, 0x64, 0x44,
                                               0x80, 0x10, 0x43, 0x4E, 0x35, 0xEC, 0x65, 0x27,
                                               0x0A, 0xB5, 0xFC, 0x26, 0x69, 0x3F, 0x59, 0x99,
                                               0x01, 0x63, 0x8A, 0xA5, 0xBF, 0x68, 0x5C, 0xD3,
                                               0x3C, 0xBE, 0x0B, 0xD6, 0x76, 0x83, 0xD6, 0x57,
                                               0x05, 0x4A, 0x3D, 0xDD, 0x81, 0x73, 0xC9, 0xEB,
                                               0x8A, 0x84, 0x39, 0xF4, 0x36, 0x0B, 0xF7
                                              };
uint8_t commsubsystem_host_mode_tx[PACKET_CONTENT_LENGTH] = {0};
uint8_t commsubsystem_host_mode_rx[PACKET_CONTENT_LENGTH] = {0};


void commsubsystem_host_mode_DTM_dataGen(uint8_t *pbuf, uint16_t len, PACKET_DATA_TYPE dataType)
{
    uint8_t dataTemp;
    if (dataType == 0)
    {
        // payload = PRBS9
        memcpy(pbuf, &(prbs_content[0]), len);
    }
    else
    {
        switch (dataType)
        {
        case (0x1): // Payload = 11110000
            dataTemp = 0x0F;
            break;
        case (0x2): // Payload = 10101010
            dataTemp = 0x55;
            break;
        case (0x4): // Payload = 11111111
            dataTemp = 0xFF;
            break;
        case (0x5): // Payload = 00000000
            dataTemp = 0x00;
            break;
        case (0x6): // Payload = 00001111
            dataTemp = 0xF0;
            break;
        case (0x7): // Payload = 10101010
            dataTemp = 0xAA;
            break;
        default: //normal Tx mode
            break;
        }
        memset(pbuf, dataTemp, len);
    }
}

#if 0



static uint16_t commsubsystem_host_mode_packet_interval_calculate(uint32_t test_payload_length, COMM_SUBSYSTEM_HOST_MODE_DATA_RATE mode)
{
    uint16_t packet_interval    = 0; // us
    uint32_t test_packet_length = 0; // [us] NOTE: bits are us at 1Mbit

    uint32_t overhead_bits      = 0; // bits
    uint32_t i = 0;
    uint32_t timeout = 0;

    /* packet overhead
     * see BLE [Vol 6, Part F] page 213
     * 4.1 LE TEST PACKET FORMAT */

    if (mode == COMM_SUBSYSTEM_HOST_MODE_PHY_2M)
    {
        // 16 preamble
        // 32 sync word
        //  8 PDU header, actually packetHeaderS0len * 8
        //  8 PDU length, actually packetHeaderLFlen
        // 24 CRC
        overhead_bits = 44; // 11 bytes
    }
    else if (mode == COMM_SUBSYSTEM_HOST_MODE_PHY_1M)
    {
        //  8 preamble
        // 32 sync word
        //  8 PDU header, actually packetHeaderS0len * 8
        //  8 PDU length, actually packetHeaderLFlen
        // 24 CRC
        overhead_bits = 80; // 10 bytes
    }
    else if (mode == COMM_SUBSYSTEM_HOST_MODE_PHY_CODED_PHY_S2)
    {
        // 80 us for preamble
        // 256 us for sync word
        // 16 us for coded indicator
        // 24 us for TERM1
        // 8*2 us for PDU header, actually packetHeaderS0len * 8
        // 8*2 us for PDU length, actually packetHeaderLFlen
        // 24*2 CRC
        // 3*2 us for TERM2
        overhead_bits = 462;

    }
    else //mode == RF_MOD_RADIO_PHY_CODED_PHY_S8
    {
        // 80 us for preamble
        // 256 us for sync word
        // 16 us for coded indicator
        // 24 us for TERM1
        // 8*8 us for PDU header, actually packetHeaderS0len * 8
        // 8*8 us for PDU length, actually packetHeaderLFlen
        // 24*8 CRC
        // 3*8 us for TERM2
        overhead_bits = 720;

    }

    /* add PDU payload test_payload length */
    if (mode == COMM_SUBSYSTEM_HOST_MODE_PHY_2M)
    {
        test_packet_length = (test_payload_length * 8) / 2; // in us
    }
    else if (mode == COMM_SUBSYSTEM_HOST_MODE_PHY_1M)
    {
        test_packet_length = (test_payload_length * 8); // in us
    }
    else if (mode == COMM_SUBSYSTEM_HOST_MODE_PHY_CODED_PHY_S2)
    {
        test_packet_length = (test_payload_length * 8) * 2; // in us

    }
    else
    {
        test_packet_length = (test_payload_length * 8) * 8; // in us

    }

    // add overhead calculated above
    test_packet_length += overhead_bits;
    // we remember this bits are us in 1Mbit



    /*
     * packet_interval = ceil((test_packet_length+249)/625)*625
     * NOTE: To avoid floating point an equivalent calculation is used.
     */
    //uint32_t i       = 0;
    //uint32_t timeout = 0;
    do
    {
        i++;
        timeout = i * 625;
    } while (test_packet_length + 249 > timeout);
    packet_interval = i * 625 - test_packet_length;

    return packet_interval;
}
#endif
void Single_tone_start(uint32_t channel_idx, COMM_SUBSYSTEM_HOST_MODE_DATA_RATE radio_mode, eHAL_MODE porotocol)
{

    uint8_t reg[4] = {0};
    printf("RF_Freq\n");
    commsubsystem_host_mode_set_RF_Freq(porotocol, channel_idx);
    if (porotocol == HAL_BLE)
    {
        printf("BLE\n");
        commsubsystem_host_mode_ble_phy_init(radio_mode);

    }
    else if (porotocol == HAL_SUBG)
    {
        commsubsystem_host_mode_subg_phy_init(radio_mode);
    }

    commsubsystem_read_memory(0x3B4, reg, 4);
    reg[2] |= 0x1;
    commsubsystem_write_memory(0x3B4, reg, 4);
    printf("single_tone_init\n");
    commsubsystem_host_mode_single_tone_init();
    printf("single_tone_en\n");
    commsubsystem_host_mode_single_tone_en();


}
void Single_tone_Stop(void)
{

    commsubsystem_host_mode_single_tone_disable();

}


uint8_t Read_Tx_power_idx(void)
{
    uint8_t reg[4] = {0};
    //void commsubsystem_read_memory(uint16_t reg_address, uint8_t *p_rx_data, uint16_t rx_data_length)
    commsubsystem_read_memory(0x3A8, reg, 4);
    //printf("0x3A8 : 0x%X, 0x%X, 0x%X, 0x%X\n",reg[0],reg[1],reg[2],reg[3]);
    return reg[0];

}

void Write_Tx_power_idx(uint8_t tx_power_idx)
{
    uint8_t reg[4] = {0};
    //void commsubsystem_read_memory(uint16_t reg_address, uint8_t *p_rx_data, uint16_t rx_data_length)
    commsubsystem_read_memory(0x3A8, reg, 4);
    reg[0] = tx_power_idx;
    commsubsystem_write_memory(0x3A8, reg, 4);

}

uint8_t Report_Rx_RSSI(void)
{

    return commsubsystem_host_mode_report_rssi();
}

uint16_t Report_Rx_CRC(void)
{
    uint32_t crc_count_get;
    uint32_t crc_fail_count_get;
    if (commsubsystem_host_mode_get_crc_count(&crc_count_get, &crc_fail_count_get) == false)
    {
        return 0;

    }

    return (crc_count_get & 0xFFFF);
}

void RF_Cablibtration_Enable(eHAL_MODE porotocol, uint8_t rf_band, ruci_para_set_calibration_enable_event_t *rf_calib_report)
{
    //RF calibration needs about 100ms
    commsubsystem_host_mode_rf_calibration(porotocol, rf_band, rf_calib_report);


}

uint8_t commsubsystem_host_mode_Tx_Test( uint32_t channel_idx, COMM_SUBSYSTEM_HOST_MODE_DATA_RATE radio_mode, PACKET_DATA_TYPE dataType, eHAL_MODE porotocol)
{

    uint8_t packet_length = DEFAULT_PAYLOAD_LENGTH;
    uint32_t SfdContent = DEFAULT_SFD;
    uint8_t WhiteningEn = 0;
    uint8_t WhiteningInitValue = 0;
    uint32_t CrcInitValue = DEFAULT_CRC_INIT;


    commsubsystem_host_mode_set_RF_Freq(porotocol, channel_idx);
    if (porotocol == HAL_BLE)
    {
        commsubsystem_host_mode_ble_phy_init(radio_mode);
        commsubsystem_host_mode_ble_mac_init( SfdContent,  WhiteningEn, WhiteningInitValue, CrcInitValue);
    }
    else if (porotocol == HAL_SUBG)
    {
        commsubsystem_host_mode_subg_phy_init(radio_mode);
        commsubsystem_host_mode_subg_mac_init();
        commsubsystem_host_mode_subg_preamble_init();
        commsubsystem_host_mode_subg_sfd_init(0x00007209);
    }
    else if (porotocol == HAL_ZIGBEE)
    {
        commsubsystem_host_mode_zigbee_mac_init();
    }
    SET_RUCI_PARA_SET_TX_CONTROL_FIELD_TEST(commsubsystem_host_mode_tx, packet_length, 0, 0);//+3 for 15.4 header


    commsubsystem_host_mode_DTM_dataGen(&commsubsystem_host_mode_tx[RUCI_LEN_SET_TX_CONTROL_FIELD], packet_length, dataType); //+3 for 15.4 header
    //packet_length+=3;//+3 for 15.4 header
    packet_length += RUCI_LEN_SET_TX_CONTROL_FIELD;
    return packet_length;
}



void commsubsystem_host_mode_Rx_Test(uint32_t channel_idx, COMM_SUBSYSTEM_HOST_MODE_DATA_RATE radio_mode, COMM_SUBSYSTEM_HOST_RX_MODE rx_mode, POWER_TRIM_TYPE power_trim_en, eHAL_MODE porotocol)
{

    uint32_t SfdContent = DEFAULT_SFD;
    uint8_t WhiteningEn = 0;
    uint8_t WhiteningInitValue = 0;
    uint32_t CrcInitValue = DEFAULT_CRC_INIT;
#if ENABLE_RFE_TRX

    ruci_para_set_rfe_rx_enable_t rx_receive_cmd_test = {0};
#else
    ruci_para_set_rfb_auto_state_t rx_receive_cmd = {0};
#endif
    ruci_para_cmn_cnf_event_t sCnfEvent = {0};
    ruci_para_set_agc_t disable_agc = {0};
    commsubsystem_host_mode_set_RF_Freq(porotocol, channel_idx);
    if (porotocol == HAL_BLE)
    {
        commsubsystem_host_mode_ble_phy_init(radio_mode);
        commsubsystem_host_mode_ble_mac_init( SfdContent,  WhiteningEn, WhiteningInitValue, CrcInitValue);
    }
    else if (porotocol == HAL_SUBG)
    {
        commsubsystem_host_mode_subg_phy_init(radio_mode);
        commsubsystem_host_mode_subg_mac_init();
        commsubsystem_host_mode_subg_preamble_init();
        commsubsystem_host_mode_subg_sfd_init(0x00007209);
    }
    else
    {

        commsubsystem_host_mode_zigbee_mac_init();
    }
    if (power_trim_en == POWER_TRIM_ENABLE)
    {
        SET_RUCI_PARA_SET_AGC(&disable_agc, 0, 7, 8, 8); //To disable AGC and set LNA = 7, VGA = 8, TIA = 8.
        commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&disable_agc, RUCI_LEN_SET_AGC, (uint8_t *)&sCnfEvent);
    }
    else
    {
        SET_RUCI_PARA_SET_AGC(&disable_agc, 1, 15, 15, 15); //To enable AGC and set LNA = 0xF, VGA = 0xF, TIA = 0xF.
        commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&disable_agc, RUCI_LEN_SET_AGC, (uint8_t *)&sCnfEvent);


    }

#if ENABLE_RFE_TRX

    SET_RUCI_PARA_SET_RFE_RX_ENABLE(&rx_receive_cmd_test, 0, 0xFF, 0x1, rx_mode, 0x0000FFFF, 0x00, 0x0000, 0, 0, 0);
    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&rx_receive_cmd_test, RUCI_LEN_SET_RFE_RX_ENABLE, (uint8_t *)&sCnfEvent);

#else
    SET_RUCI_PARA_SET_RFB_AUTO_STATE(&rx_receive_cmd, RX_ON);
    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&rx_receive_cmd, RUCI_LEN_SET_RFB_AUTO_STATE, (uint8_t *)&sCnfEvent);
#endif
}

