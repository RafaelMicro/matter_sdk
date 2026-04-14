#include "DTM_RF.h"

#define PHY_PIB_TURNAROUND_TIMER    192
#define PHY_PIB_CCA_DETECTED_TIME   128 // 8 symbols
#define PHY_PIB_CCA_DETECT_MODE     0
#define PHY_PIB_CCA_THRESHOLD       50
#define MAC_PIB_UNIT_BACKOFF_PERIOD 320
#define MAC_PIB_MAC_ACK_WAIT_DURATION                                          \
    544 // non-beacon mode; 864 for beacon mode
#define MAC_PIB_MAC_MAX_BE                    5
#define MAC_PIB_MAC_MAX_FRAME_TOTAL_WAIT_TIME 16416
#define MAC_PIB_MAC_MAX_FRAME_RETRIES         4
#define MAC_PIB_MAC_MAX_CSMACA_BACKOFFS       5
#define MAC_PIB_MAC_MIN_BE                    2

#define TX_DATA_LENGTH (100)
static lmac154_channel_t sCurrentChannel = LMAC154_CHANNEL_11;
static TaskHandle_t sRFTaskHandle;
static StackType_t rfStack[1024 / sizeof(StackType_t)];
static StaticTask_t rfTaskStruct;
static uint16_t sShortAddress = 0x1234;
static uint32_t sExtendAddr_0 = 0x01020304;
static uint32_t sExtendAddr_1 = 0x05060709;
static uint16_t sPANID = 0xABCD;
static uint8_t tx_data_buf[TX_DATA_LENGTH] = {0};
static volatile uint8_t tx_done = 1;
uint32_t crc_correct_pkt_count = 0;
static uint32_t sAutoState = 0;


static void _TxDoneEvent(uint32_t tx_status) {
    tx_done = 1;
    if (0 == tx_status) {
        // No Ack
    }
    else if (0x10 == tx_status)
    {
        // CCA Fail
    }
    else if (0x20 == tx_status)
    {
        // No Ack
    }
    else if (0x40 == tx_status || 0x80 == tx_status)
    {
        // Acked
    }
}

static void _RxDoneEvent(uint16_t packet_length, uint8_t* rx_data_address,
                         uint8_t crc_status, uint8_t rssi, uint8_t snr) {
    if (crc_status == 0) {
        uint8_t payload_len = packet_length - 11;
        if(memcmp(rx_data_address+8, tx_data_buf, payload_len) == 0)
        {
            crc_correct_pkt_count++;
            printf("RX packet counts %d\r\n", crc_correct_pkt_count);
        }
    }
}
void RFTaskMain(void * pvParameter)
{
    for(uint8_t i = 0; i < TX_DATA_LENGTH;i++)
    {
        tx_data_buf[i] = i;
    }
    hosal_rf_init(HOSAL_RF_MODE_MULTI_PROTOCOL);
    //Start Uart task.
    lmac15p4_callback_t mac_cb;
    lmac15p4_init(LMAC15P4_2P4G_OQPSK, 0);
    /* PHY PIB */
    lmac15p4_phy_pib_set(PHY_PIB_TURNAROUND_TIMER, PHY_PIB_CCA_DETECT_MODE,
                         PHY_PIB_CCA_THRESHOLD, PHY_PIB_CCA_DETECTED_TIME);
    /* MAC PIB */
    lmac15p4_mac_pib_set(MAC_PIB_UNIT_BACKOFF_PERIOD,
                         MAC_PIB_MAC_ACK_WAIT_DURATION, MAC_PIB_MAC_MAX_BE,
                         MAC_PIB_MAC_MAX_CSMACA_BACKOFFS,
                         MAC_PIB_MAC_MAX_FRAME_TOTAL_WAIT_TIME,
                         MAC_PIB_MAC_MAX_FRAME_RETRIES, MAC_PIB_MAC_MIN_BE);
    mac_cb.rx_cb = _RxDoneEvent;
    mac_cb.tx_cb = _TxDoneEvent;
    lmac15p4_cb_set(0, &mac_cb);
    lmac15p4_channel_set(sCurrentChannel);

    /* Auto ACK */
    lmac15p4_auto_ack_set(true);
    /* Auto State */
    sAutoState = 0;
    lmac15p4_auto_state_set(false);
    lmac15p4_ack_pending_bit_set(0, 0);

    lmac15p4_address_filter_set(0, 0, sShortAddress, sExtendAddr_0,
                                sExtendAddr_1, sPANID, 0);
    log_info("RF Init Done.");
    vTaskDelete(sRFTaskHandle);    
}
void rf_init()
{
    sRFTaskHandle = xTaskCreateStatic(RFTaskMain, "RF",
                                    (1024/sizeof(StackType_t)), NULL, 5, rfStack, &rfTaskStruct);
}
void set_channel(lmac154_channel_t channel)
{
    if(channel != sCurrentChannel)
    {
        sCurrentChannel = channel;
        lmac15p4_channel_set(sCurrentChannel);
    }
}
void set_auto_state(uint32_t enable)
{
    if(enable != sAutoState)
    {
        sAutoState = enable;
        crc_correct_pkt_count = 0;
        lmac15p4_auto_state_set(enable);
    }
}
void tx_single_tone(uint8_t enable)
{
    lmac15p4_tx_continuous_wave_send(enable);
}
void tx_pkt_cnt(uint32_t pkt_cnt)
{
    static uint8_t sn = 0;
    for(uint8_t i = 0; i < pkt_cnt;i++)
    {
        while(!tx_done)
        {
            vTaskDelay(1);
        }
        tx_done = 0;
        if(lmac15p4_tx_data_send(0, tx_data_buf,
                            TX_DATA_LENGTH,
                            2,
                            sn) != 0)
        {
            tx_done = 1;
        }
        sn++;
        vTaskDelay(20);
    }
    return ;
}