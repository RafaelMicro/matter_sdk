#include "cm3_mcu.h"
#include "mp_tool_phy.h"
#include "comm_subsystem_ahb_phy.h"
#include <stdio.h>
#include <string.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"



extern uint8_t commsubsystem_host_mode_rx[];

/* Flag toggled when event received */
volatile uint8_t wait_notification_flag = 1;
volatile ruci_para_initiate_zigbee_t gdtm_init_z_cmd_s = {0};

volatile ruci_para_set_ble_mac_t gdtm_set_mac_cmd_s = {0};
volatile ruci_para_set_rf_frequency_t gdtm_set_freq_cmd_s = {0};
volatile ruci_para_set_rfe_tx_enable_t gdtm_set_tx_en_cmd_s = {0};
volatile ruci_para_set_rfe_rx_enable_t gdtm_rx_en_cmd_s = {0};

volatile ruci_para_set_clock_mode_t gdtm_set_clock_cmd_s = {0};

volatile ruci_para_dtm_burst_tx_done_event_t gdtm_tx_done_event = {0};



#define REGISTER_LENGTH (4)

#define SUBG_VCO_LB_UQ8         (0x69492000)    //(SubG VCO lower bound = 6900.000MHz). (6900000KHz<<8) = 0x69492000
#define SUBG_VCO_LB_UQ8_FOR_430M         (0x6689FFFF)    //(SubG VCO lower bound for 432MHz = 6719.999MHz). (6719999KHz<<8) = 0x6689FFFF
#define SUBG_VCO_LB_UQ8_FOR_860M         (0x68FAFF00)    //(SubG VCO lower bound for 860MHz = 6879.999MHz). (6879999KHz<<8) = 0x68FAFF00
#define SUBG_VCO_HB_UQ8         (0x9DEDB000)    //(SubG VCO highter bound = 10350.000MHz). (10350000KHz<<8) = 0x9DEDB000
#define HAL_BLE_CH2FREQ(ch)                                     ((uint16_t)(2402 + (ch << 1)))
#define HAL_ZIGBEE_CH2FREQ(ch)                                  ((uint16_t)(2350 + (ch << 2) + (ch)))



bool commsubsystem_host_mode_set_ruci_cmd(uint8_t *cmd, uint32_t cmd_length, uint8_t *result)
{
    TX_CMDQ_ERROR txCmdqError;
    RX_CMDQ_ERROR rx_cmd_error;
    uint32_t ulNotifiedValue;
    rx_cmd_error = READ_RX_CMDQ_SUCCESS;
    txCmdqError = TX_CMDQ_ERROR_INIT;



    txCmdqError = commsubsystem_send_cmd_queue(cmd, cmd_length);

    if (txCmdqError != SEND_TO_CMDQ_SUCCESS)
    {
        //printf(("SEND TO CMDQ FAILS\n");
        return false;
    }
    xTaskNotifyWait( 0, COMMUNICATION_ISR_EVEN_DONE, &ulNotifiedValue, portMAX_DELAY );
    if ((ulNotifiedValue & COMMUNICATION_ISR_EVEN_DONE) != COMMUNICATION_ISR_EVEN_DONE)
    {
        //printf(("Not Even done\n");
        return false;
    }
    else
    {

        commsubsystem_read_cmd_queue(result, &rx_cmd_error);


        if (rx_cmd_error != READ_RX_CMDQ_SUCCESS)
        {
            //printf(("READ_RX_CMDQ_FIALURE:%d\n", rx_cmd_error);
            return false;
        }
    }


    return true;

}

/* RUCI event validation check */
bool commsubsystem_host_mode_cmd_event_check(uint8_t cmd_hdr, uint8_t cmd_sub_hdr, volatile ruci_para_cnf_event_t *pdtm_cnf_event)
{
    /* check event type & sub header */
    if (pdtm_cnf_event->ruci_header.u8 != RUCI_PCI_EVENT_HEADER ||
            pdtm_cnf_event->sub_header != RUCI_CODE_CNF_EVENT ||
            pdtm_cnf_event->length != sRUCI_LENGTH(RUCI_LEN_CNF_EVENT))
    {
        return false;
    }

    /* check if command header match */
    if (pdtm_cnf_event->pci_cmd_header != cmd_hdr ||
            pdtm_cnf_event->pci_cmd_subheader != cmd_sub_hdr)
    {
        return false;
    }

    /* check status */
    return (pdtm_cnf_event->status == 0) ? true : false;

}


bool commsubsystem_host_mode_rx_disable_cmd(void)
{
#if ENABLE_RFE_TRX
    ruci_para_set_rfe_rx_disable_t gdtm_set_rx_disable_cmd_s = {0};
    ruci_para_cnf_event_t gdtm_cnf_event_s = {0};
    /* Package RUCI RFE RX Disable command */
    SET_RUCI_PARA_SET_RFE_RX_DISABLE(&gdtm_set_rx_disable_cmd_s);

    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&gdtm_set_rx_disable_cmd_s, RUCI_NUM_SET_RFE_RX_DISABLE, (uint8_t *)&gdtm_cnf_event_s);

    RUCI_ENDIAN_CONVERT((uint8_t *)&gdtm_set_rx_disable_cmd_s, RUCI_SET_RFE_RX_DISABLE);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_COMMON_CMD_HEADER, RUCI_CODE_SET_RFE_RX_DISABLE, &gdtm_cnf_event_s) == false)
    {
        //printf(("\r\nBLE DTM Rx disable fails");
        return false;
    }

    return true;


#else




    ruci_para_set_rfb_auto_state_t rx_disable_cmd = {0};
    ruci_para_cnf_event_t sCnfEvent = {0};

    /* Package RUCI RFE RX Disable command */
    SET_RUCI_PARA_SET_RFB_AUTO_STATE(&rx_disable_cmd, 0);
    //printf(("rx_disable\n");
    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&rx_disable_cmd, RUCI_LEN_SET_RFB_AUTO_STATE, (uint8_t *)&sCnfEvent);


    RUCI_ENDIAN_CONVERT((uint8_t *)&gdtm_set_rx_disable_cmd_s, RUCI_SET_RFE_RX_DISABLE);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_COMMON_CMD_HEADER, RUCI_CODE_SET_RFB_AUTO_STATE, &sCnfEvent) == false)
    {

        return false;
    }

    return true;

#endif
}

bool commsubsystem_host_mode_get_crc_count(uint32_t *crc_count_get, uint32_t *crc_fail_count_get)
{
    ruci_para_get_crc_count_t get_crc_cmd = {0};
    ruci_para_cnf_event_t sCnfEvent = {0};
    ruci_para_get_crc_report_event_t get_crc_event_s = {0};
    RX_CMDQ_ERROR rx_cmd_error;
    uint32_t ulNotifiedValue;

    /* deliver get CRC command to RT569 */
    SET_RUCI_PARA_GET_CRC_COUNT(&get_crc_cmd);

    RUCI_ENDIAN_CONVERT((uint8_t *)&gdtm_get_crc_cmd_s, RUCI_GET_CRC_COUNT);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&get_crc_cmd, RUCI_LEN_GET_CRC_COUNT, (uint8_t *)&sCnfEvent);

    /* to wait CRC response */
    //gdtm_wait_notification_flag = 1;

    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_COMMON_CMD_HEADER, RUCI_CODE_GET_CRC_COUNT, &sCnfEvent) == false)
    {
        //printf(("\r\nFails to send a CRC cmd");
        return false;
    }
    else
    {
        //printf(("\r\nSend a CRC cmd");
    }

    xTaskNotifyWait( 0, COMMUNICATION_ISR_EVEN_DONE, &ulNotifiedValue, portMAX_DELAY );
    if ((ulNotifiedValue & COMMUNICATION_ISR_EVEN_DONE) != COMMUNICATION_ISR_EVEN_DONE)
    {
        //printf(("CRC Not Even done\n");
        return false;
    }
    else
    {

        commsubsystem_read_cmd_queue((uint8_t *)&get_crc_event_s, &rx_cmd_error);


        if (rx_cmd_error != READ_RX_CMDQ_SUCCESS)
        {
            //printf(("RSSI READ_RX_CMDQ_FIALURE:%d\n", rx_cmd_error);
            return false;
        }
    }


    if ((get_crc_event_s.ruci_header.u8 == RUCI_PCI_EVENT_HEADER) &&
            (get_crc_event_s.sub_header == RUCI_CODE_GET_CRC_REPORT_EVENT) &&
            (get_crc_event_s.length == sRUCI_LENGTH(RUCI_LEN_GET_CRC_REPORT_EVENT)))
    {
        *crc_count_get = get_crc_event_s.crc_success_count;
        *crc_fail_count_get = get_crc_event_s.crc_fail_count;
        //printf(("\r\nCRC = 0x%x", *crc_count_get);
    }
    else
    {
        //printf(("\r\nCRC event error");
        return false;
    }

    return true;
}










/* RUCI command to disable RFIC TX */
bool commsubsystem_host_mode_tx_disable_cmd(void)
{

    ruci_para_set_rfe_tx_disable_t tx_disable_cmd = {0};
    ruci_para_cnf_event_t sCnfEvent = {0};

    /* Package RUCI RFE TX Disable command */
    SET_RUCI_PARA_SET_RFE_TX_DISABLE(&tx_disable_cmd);
    RUCI_ENDIAN_CONVERT((uint8_t *)&gdtm_set_tx_disable_cmd_s, RUCI_SET_RFE_TX_DISABLE);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&tx_disable_cmd, RUCI_NUM_SET_RFE_TX_DISABLE, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_COMMON_CMD_HEADER, RUCI_CODE_SET_RFE_TX_DISABLE, &sCnfEvent) == false)
    {
        return false;
    }

    return true;
}

uint8_t commsubsystem_host_mode_report_rssi(void)
{
    uint8_t tmp = 0;

    ruci_para_get_rssi_t sGetRSSICmd = {0};
    ruci_para_get_rssi_event_t sRSSIEvent = {0};
    ruci_para_cnf_event_t sCnfEvent = {0};
    RX_CMDQ_ERROR rx_cmd_error;
    uint32_t ulNotifiedValue;

    ////printf(("wr_idx = %d, rd_idx = %d\n",commu_cmd_queue.wr_idx,commu_cmd_queue.rd_idx);


    SET_RUCI_PARA_GET_RSSI(&sGetRSSICmd);
    RUCI_ENDIAN_CONVERT((uint8_t *)&sGetRSSICmd, RUCI_NUM_GET_RSSI);

    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&sGetRSSICmd, RUCI_LEN_GET_RSSI, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_COMMON_CMD_HEADER, RUCI_CODE_GET_RSSI, &sCnfEvent) == false)
    {

        return false;
    }
    xTaskNotifyWait( 0, COMMUNICATION_ISR_EVEN_DONE, &ulNotifiedValue, portMAX_DELAY );
    if ((ulNotifiedValue & COMMUNICATION_ISR_EVEN_DONE) != COMMUNICATION_ISR_EVEN_DONE)
    {
        //printf(("RSSI Not Even done\n");
        return false;
    }
    else
    {

        commsubsystem_read_cmd_queue((uint8_t *)&sRSSIEvent, &rx_cmd_error);


        if (rx_cmd_error != READ_RX_CMDQ_SUCCESS)
        {
            //printf(("RSSI READ_RX_CMDQ_FIALURE:%d\n", rx_cmd_error);
            return false;
        }
    }
    tmp = sRSSIEvent.rssi;


    return (uint8_t) tmp;
}

COMM_SUBSYSTEM_HOST_MODE_EVENT_STATUS commsubsystem_host_mode_frequency_set(uint32_t rf_frequency)
{
    ruci_para_set_rf_frequency_t sSetFrequencyCmd = {0};
    ruci_para_cnf_event_t sCnfEvent = {0};


    SET_RUCI_PARA_SET_RF_FREQUENCY(&sSetFrequencyCmd, rf_frequency);
    RUCI_ENDIAN_CONVERT((uint8_t *)&sSetFrequencyCmd, RUCI_SET_RF_FREQUENCY);

    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&sSetFrequencyCmd, RUCI_LEN_SET_RF_FREQUENCY, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_COMMON_CMD_HEADER, RUCI_CODE_SET_RF_FREQUENCY, &sCnfEvent) == false)
    {

        return COMM_SUBSYSTEM_HOST_MODE_CNF_EVENT_CONTENT_ERROR;
    }

    return COMM_SUBSYSTEM_HOST_MODE_EVENT_SUCCESS;
}



void commsubsystem_host_mode_set_RF_Freq(eHAL_MODE halType, uint32_t target_freq)
{

    if (halType == HAL_BLE)
    {
        if (target_freq > 39)
        {
            target_freq = 39;
        }
        commsubsystem_host_mode_frequency_set(HAL_BLE_CH2FREQ(target_freq));//target_freq is2402~2480 MHz
    }
    else if (halType == HAL_SUBG)
    {
        commsubsystem_host_mode_frequency_set(target_freq);//target_freq (U(32, 0), 1KHz, freq - IF shoud be in range 161718KHz to 1000000KHz
    }
    else
    {
        // halType == HAL_ZIGBEE
        if (target_freq > 26)
        {
            target_freq = 26;
        }
        commsubsystem_host_mode_frequency_set(HAL_ZIGBEE_CH2FREQ(target_freq));//target_freq is2405~2480 MHz(11~26)

    }

}

bool  commsubsystem_host_mode_ble_mac_init(uint32_t SfdContent, uint8_t WhiteningEn, uint8_t WhiteningInitValue, uint32_t CrcInitValue)
{
    //To initialize 15.4MAC
    ruci_para_cnf_event_t sCnfEvent = {0};
    ruci_para_set_ble_mac_t BLEMacInitCmd = {0};


    SET_RUCI_PARA_SET_BLE_MAC(&BLEMacInitCmd, SfdContent, WhiteningEn, WhiteningInitValue, CrcInitValue)
    RUCI_ENDIAN_CONVERT((uint8_t *)&BLEMacInitCmd, RUCI_SET_BLE_MAC);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&BLEMacInitCmd, RUCI_LEN_SET_BLE_MAC, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_BLE_CMD_HEADER, RUCI_CODE_SET_BLE_MAC, &sCnfEvent) == false)
    {

        return false;
    }
    return true;
}








/* RUCI command to set RFIC PHY mode */
bool commsubsystem_host_mode_ble_phy_init(COMM_SUBSYSTEM_HOST_MODE_DATA_RATE radio_mode)
{


    ruci_para_cnf_event_t sCnfEvent = {0};
    ruci_para_set_ble_modem_t BLEModemInitCmd = {0};
    CODED_TYPE                BLE_coded_scheme = S2;


    switch (radio_mode)
    {
    case COMM_SUBSYSTEM_HOST_MODE_PHY_CODED_PHY_S8:
        BLE_coded_scheme = S8;
        break;
    case COMM_SUBSYSTEM_HOST_MODE_PHY_CODED_PHY_S2:
        BLE_coded_scheme = S2;
        break;
    default:
        break;
    }

    /* Package RUCI SET BLE MODEM command */
    SET_RUCI_PARA_SET_BLE_MODEM(&BLEModemInitCmd, radio_mode, BLE_coded_scheme);
    RUCI_ENDIAN_CONVERT((uint8_t *)&BLEModemInitCmd, RUCI_SET_BLE_MODEM);
    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&BLEModemInitCmd, RUCI_LEN_SET_BLE_MODEM, (uint8_t *)&sCnfEvent);

    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_BLE_CMD_HEADER, RUCI_CODE_SET_BLE_MODEM, &sCnfEvent) == false)
    {

        return false;
    }

    return true;

}
bool  commsubsystem_host_mode_single_tone_init(void)
{

    ruci_para_cnf_event_t sCnfEvent = {0};
    ruci_para_set_single_tone_mode_t SingleToneInitCmd = {0};


    SET_RUCI_PARA_SET_SINGLE_TONE_MODE(&SingleToneInitCmd, 0x02)
    RUCI_ENDIAN_CONVERT((uint8_t *)&SingleToneInitCmd, RUCI_SET_SINGLE_TONE_MODE);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&SingleToneInitCmd, RUCI_LEN_SET_SINGLE_TONE_MODE, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_COMMON_CMD_HEADER, RUCI_CODE_SET_SINGLE_TONE_MODE, &sCnfEvent) == false)
    {

        return false;
    }
    return true;
}
bool  commsubsystem_host_mode_single_tone_en(void)
{

    ruci_para_cnf_event_t sCnfEvent = {0};
    ruci_para_set_rfe_tx_enable_t SingleToneEnCmd = {0};


    SET_RUCI_PARA_SET_RFE_TX_ENABLE(&SingleToneEnCmd, 0x0, 0xFF, 0, 0xFFFF, 0xFF, 0, 0, 0, 0, 0, 0);
    RUCI_ENDIAN_CONVERT((uint8_t *)&SingleToneEnCmd, RUCI_SET_RFE_TX_ENABLE);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&SingleToneEnCmd, RUCI_LEN_SET_RFE_TX_ENABLE, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_COMMON_CMD_HEADER, RUCI_CODE_SET_RFE_TX_ENABLE, &sCnfEvent) == false)
    {

        return false;
    }
    return true;
}

bool  commsubsystem_host_mode_single_tone_disable(void)
{

    volatile ruci_para_cnf_event_t sCnfEvent = {0};
    ruci_para_set_rfe_tx_disable_t SingleToneDisableCmd = {0};


    SET_RUCI_PARA_SET_RFE_TX_DISABLE(&SingleToneDisableCmd);
    RUCI_ENDIAN_CONVERT((uint8_t *)&SingleToneDisableCmd, RUCI_SET_RFE_TX_DISABLE);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&SingleToneDisableCmd, RUCI_LEN_SET_RFE_TX_DISABLE, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_COMMON_CMD_HEADER, RUCI_CODE_SET_RFE_TX_DISABLE, &sCnfEvent) == false)
    {

        return false;
    }
    return true;
}


COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_ERROR  commsubsystem_host_mode_ble_init()
{
    /* Package RUCI BLE Init command */

    ruci_para_cnf_event_t sCnfEvent = {0};
    ruci_para_initiate_ble_t BLEInitCmd = {0};
    SET_RUCI_PARA_INITIATE_BLE(&BLEInitCmd);



    RUCI_ENDIAN_CONVERT((uint8_t *)&BLEInitCmd, RUCI_INITIATE_BLE);
    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&BLEInitCmd, RUCI_LEN_INITIATE_BLE, (uint8_t *)&sCnfEvent);

    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_BLE_CMD_HEADER, RUCI_CODE_INITIATE_BLE, &sCnfEvent) == false)
    {

        return BLE_INITIALIZATION_FAIL;
    }


    return COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_SUCCEED;
}

bool  commsubsystem_host_mode_zigbee_mac_init(void)
{
    //To initialize 15.4MAC
    ruci_para_cnf_event_t sCnfEvent = {0};
    ruci_para_set15p4_mac_pib_t ZigbeeMacInitCmd = {0};


    SET_RUCI_PARA_SET15P4_MAC_PIB(&ZigbeeMacInitCmd, 0x140, 0x220, 0x05, 0x4, 0x4020, 0x3, 0x3);
    RUCI_ENDIAN_CONVERT((uint8_t *)&ZigbeeMacInitCmd, RUCI_SET15P4_SW_MAC);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&ZigbeeMacInitCmd, RUCI_LEN_SET15P4_MAC_PIB, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI15P4_MAC_CMD_HEADER, RUCI_CODE_SET15P4_MAC_PIB, &sCnfEvent) == false)
    {

        return false;
    }
    return true;
}

COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_ERROR  commsubsystem_host_mode_zigbee_init(void)
{
    ruci_para_initiate_zigbee_t ZigbeeInitCmd = {0};
    ruci_para_cnf_event_t sCnfEvent = {0};


    SET_RUCI_PARA_INITIATE_ZIGBEE(&ZigbeeInitCmd);
    RUCI_ENDIAN_CONVERT((uint8_t *)&sZigbeeInitCmd, RUCI_INITIATE_ZIGBEE);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&ZigbeeInitCmd, RUCI_LEN_INITIATE_ZIGBEE, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI15P4_MAC_CMD_HEADER, RUCI_CODE_INITIATE_ZIGBEE, &sCnfEvent) == false)
    {

        return ZIGBEE_INITIALIZATION_FAIL;
    }

    return COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_SUCCEED;
}

bool  commsubsystem_host_mode_subg_sfd_init(uint32_t SfdContent)
{
    ruci_para_set_fsk_sfd_t  SubGsfdInitCmd = {0};
    ruci_para_cnf_event_t sCnfEvent = {0};


    SET_RUCI_PARA_SET_FSK_SFD(&SubGsfdInitCmd, SfdContent);
    RUCI_ENDIAN_CONVERT((uint8_t *)&SubGsfdInitCmd, RUCI_SET_FSK_SFD);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&SubGsfdInitCmd, RUCI_LEN_SET_FSK_SFD, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_FSK_CMD_HEADER, RUCI_CODE_SET_FSK_SFD, &sCnfEvent) == false)
    {

        return false;
    }

    return true;
}



bool commsubsystem_host_mode_subg_preamble_init(void)
{
    ruci_para_set_fsk_preamble_t  SubGPreambleInitCmd = {0};
    ruci_para_cnf_event_t sCnfEvent = {0};


    SET_RUCI_PARA_SET_FSK_PREAMBLE(&SubGPreambleInitCmd, 8); //Preamble length = 8 bytes
    RUCI_ENDIAN_CONVERT((uint8_t *)&SubGPreambleInitCmd, RUCI_SET_FSK_PREAMBLE);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&SubGPreambleInitCmd, RUCI_LEN_SET_FSK_PREAMBLE, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_FSK_CMD_HEADER, RUCI_CODE_SET_FSK_PREAMBLE, &sCnfEvent) == false)
    {

        return false;
    }

    return true;
}

bool  commsubsystem_host_mode_subg_mac_init(void)
{
    ruci_para_set_fsk_mac_t  SubGMacInitCmd = {0};
    ruci_para_cnf_event_t sCnfEvent = {0};


    SET_RUCI_PARA_SET_FSK_MAC(&SubGMacInitCmd, 0, 0); //0:CRC16,0:Disable Whitening
    RUCI_ENDIAN_CONVERT((uint8_t *)&SubGMacInitCmd, RUCI_SET_FSK_MAC);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&SubGMacInitCmd, RUCI_LEN_SET_FSK_MAC, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_FSK_CMD_HEADER, RUCI_CODE_SET_FSK_MAC, &sCnfEvent) == false)
    {

        return false;
    }

    return true;
}



bool  commsubsystem_host_mode_subg_phy_init(COMM_SUBSYSTEM_HOST_MODE_DATA_RATE radio_mode)
{
    ruci_para_set_fsk_modem_t SubGModemInitCmd = {0};
    ruci_para_cnf_event_t sCnfEvent = {0};

    SET_RUCI_PARA_SET_FSK_MODEM(&SubGModemInitCmd, radio_mode, 0); //0:modulation index = 0.5
    RUCI_ENDIAN_CONVERT((uint8_t *)&SubGModemInitCmd, RUCI_SET_FSK_MODEM);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&SubGModemInitCmd, RUCI_LEN_SET_FSK_MODEM, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_FSK_CMD_HEADER, RUCI_CODE_SET_FSK_MODEM, &sCnfEvent) == false)
    {

        return false;
    }

    return true;
}

COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_ERROR  commsubsystem_host_mode_subg_init(void)
{
    ruci_para_initiate_fsk_t SubGInitCmd = {0};
    ruci_para_cnf_event_t sCnfEvent = {0};


    SET_RUCI_PARA_INITIATE_FSK(&SubGInitCmd, 0); //0:SubG
    RUCI_ENDIAN_CONVERT((uint8_t *)&SubGInitCmd, RUCI_INITIATE_FSK);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&SubGInitCmd, RUCI_LEN_INITIATE_FSK, (uint8_t *)&sCnfEvent);
    if (commsubsystem_host_mode_cmd_event_check(RUCI_PCI_FSK_CMD_HEADER, RUCI_CODE_INITIATE_FSK, &sCnfEvent) == false)
    {

        return SUBG_INITIALIZATION_FAIL;
    }

    return COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_SUCCEED;
}

bool commsubsystem_host_mode_rf_calibration(eHAL_MODE porotocol, uint8_t rf_band, ruci_para_set_calibration_enable_event_t *rf_calib_report)
{

    ruci_para_set_calibration_enable_t RfCalibCmd = {0};
    ruci_para_cnf_event_t sCnfEvent = {0};
    uint32_t ulNotifiedValue;
    RX_CMDQ_ERROR rx_cmd_error;
    COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_ERROR error = COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_SUCCEED;


    if (porotocol == HAL_ZIGBEE) /// To avoid initial mistake
    {

        error = commsubsystem_host_mode_ble_init();
        if (error != COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_SUCCEED)
        {
            return false;
        }
    }

    SET_RUCI_PARA_SET_CALIBRATION_ENABLE(&RfCalibCmd, rf_band);


    RUCI_ENDIAN_CONVERT((uint8_t *)&RfCalibCmd, RUCI_SET_CALIBRATION_ENABLE);


    commsubsystem_host_mode_set_ruci_cmd((uint8_t *)&RfCalibCmd, RUCI_LEN_SET_CALIBRATION_ENABLE, (uint8_t *)&sCnfEvent);
    //if(commsubsystem_host_mode_cmd_event_check(RUCI_CMN_HAL_CMD_HEADER,RUCI_CODE_SET_CALIBRATION_ENABLE,&sCnfEvent) == false){

    //return false;
    //}

    xTaskNotifyWait( 0, COMMUNICATION_ISR_EVEN_DONE, &ulNotifiedValue, portMAX_DELAY );
    if ((ulNotifiedValue & COMMUNICATION_ISR_EVEN_DONE) != COMMUNICATION_ISR_EVEN_DONE)
    {
        //printf(("RF calib fails\n");
        return false;
    }
    else
    {

        commsubsystem_read_cmd_queue((uint8_t *)rf_calib_report, &rx_cmd_error);


        if (rx_cmd_error != READ_RX_CMDQ_SUCCESS)
        {
            //printf(("RF calib  READ_RX_CMDQ_FIALURE:%d\n", rx_cmd_error);
            return false;
        }
    }
    return true;
}




COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_ERROR commsubsystem_host_mode_phy_init(eHAL_MODE porotocol)
{


    COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_ERROR error = COMM_SUBSYSTEM_HOST_MODE_PHY_INIT_SUCCEED;
    if (porotocol == HAL_BLE)
    {
        //printf(("ble_init\n");
        error = commsubsystem_host_mode_ble_init();
    }
    else if (porotocol == HAL_SUBG)
    {
        commsubsystem_host_mode_subg_init();
    }
    else if (porotocol == HAL_ZIGBEE)
    {
        error = commsubsystem_host_mode_zigbee_init();
    }
    return error;

}



void commsubsystem_host_mode_trx_stop(void)
{
    //commsubsystem_host_mode_tx_disable_cmd();
    commsubsystem_host_mode_rx_disable_cmd();

}


void commsubsystem_host_mode_tx_queue_send(uint8_t *pTxQueue, uint16_t TxQueueLen)
{
    TX_TXQ_ERROR tx_q_error = SEND_TO_TXQ_SUCCESS;

    tx_q_error = commsubsystem_send_tx_queue(0, pTxQueue, TxQueueLen);
    if (tx_q_error == TXQ_FULL)
    {
        BREAK();
    }
}
