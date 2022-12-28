/**
 * @file rfb_port.h
 * @author
 * @date
 * @brief rfb porting function header.
 *
 * More detailed description can go here
 *
 *
 * @see http://
 */

#ifndef __RFB_PORT__
#define __RFB_PORT__

/**************************************************************************************************
*    INCLUDES
*************************************************************************************************/
#include "rfb.h"


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
typedef struct _rfb_t
{
    rfb_modem_type_t modem_type;
    uint32_t rx_timeout;
    bool rx_continuous;
} rfb_t;


/**************************************************************************************************
 *    GLOBAL FUNCTIONS
 *************************************************************************************************/
void rfb_port_modem_set(rfb_modem_type_t modem);
void rfb_port_frequency_set(uint32_t rf_frequency);
uint32_t rfb_port_data_send(uint8_t *tx_data_address, uint16_t packet_length, uint8_t InitialCwAckRequest, uint8_t Dsn);
void rfb_port_tx_continuous_wave_set(uint32_t rf_frequency, tx_power_level_t tx_power);
uint8_t rfb_port_rssi_read(rfb_modem_type_t modem);
void rfb_port_auto_state_set(bool rxOnWhenIdle);
uint32_t rfb_port_version_get(void);

#if (defined RFB_ZIGBEE_ENABLED && RFB_ZIGBEE_ENABLED == 1)
void rfb_port_zb_init(rfb_interrupt_event_t *_rfb_interrupt_event);
bool rfb_port_zb_is_channel_free(uint32_t rf_frequency, uint8_t rssi_threshold);
#endif
#if ((defined RFB_ZIGBEE_ENABLED && RFB_ZIGBEE_ENABLED == 1) || (defined RFB_15p4_MAC_ENABLED && RFB_15p4_MAC_ENABLED == 1))
void rfb_port_15p4_address_filter_set(uint8_t mac_promiscuous_mode, uint16_t short_source_address, uint32_t long_source_address_0, uint32_t long_source_address_1, uint16_t pan_id, uint8_t is_coordinator);
void rfb_port_15p4_mac_pib_set(uint32_t a_unit_backoff_period, uint32_t mac_ack_wait_duration, uint8_t mac_max_BE, uint8_t mac_max_CSMA_backoffs,
                               uint32_t mac_max_frame_total_wait_time, uint8_t mac_max_frame_retries, uint8_t mac_min_BE);
void rfb_port_15p4_phy_pib_set(uint16_t a_turnaround_time, uint8_t phy_cca_mode, uint8_t phy_cca_threshold, uint16_t phy_cca_duration);
void rfb_port_15p4_auto_ack_set(uint8_t auto_ack_enable);
void rfb_port_15p4_pending_bit_set(uint8_t pending_bit_enable);
#endif

#if (defined RFB_WISUN_ENABLED && RFB_WISUN_ENABLED == 1)
void rfb_port_wisun_init(rfb_interrupt_event_t *_rfb_interrupt_event);
bool rfb_port_wisun_is_channel_free(uint32_t rf_frequency, uint8_t rssi_threshold);
void rfb_port_wisun_rx_config_set(uint8_t data_rate, uint16_t preamble_len, fsk_mod_t mod_idx, fsk_crc_type_t crc_type,
                                  whiten_enable_t whiten_enable, uint32_t rx_timeout, bool rx_continuous, uint8_t filter_type);
void rfb_port_wisun_tx_config_set(tx_power_level_t tx_power, uint8_t data_rate, uint16_t preamble_len, fsk_mod_t mod_idx,
                                  fsk_crc_type_t crc_type, whiten_enable_t whiten_enable, uint8_t filter_type);
void rfb_port_sleep_set(void);
void rfb_port_idle_set(void);
void rfb_port_rx_start(void);
#endif

#if (defined RFB_BLE_ENABLED && RFB_BLE_ENABLED == 1)
void rfb_port_ble_init(rfb_interrupt_event_t *_rfb_interrupt_event);
void rfb_port_ble_modem_set(uint8_t data_rate, uint8_t coded_scheme);
void rfb_port_ble_mac_set(uint32_t sfd_content, uint8_t whitening_en, uint8_t whitening_init_value, uint32_t crc_init_value);
void rfb_port_rx_start(void);
#endif
void rfb_port_15p4_src_addr_match_ctrl(uint8_t ctrl_type);
void rfb_port_15p4_short_addr_ctrl(uint8_t ctrl_type, uint8_t *short_addr);
void rfb_port_15p4_extend_addr_ctrl(uint8_t ctrl_type, uint8_t *extend_addr);
void rfb_port_key_set(uint8_t *key_addr);
void rfb_port_csl_receiver_ctrl(uint8_t csl_receiver_ctrl, uint16_t csl_period);
uint8_t rfb_port_csl_accuracy_get(void);
uint8_t rfb_port_csl_uncertainty_get(void);
void rfb_port_csl_sample_time_update(uint32_t csl_sample_time);
uint32_t rfb_port_rtc_time_read(void);
#endif

#if (defined RFB_MULTI_ENABLED && RFB_MULTI_ENABLED == 1)
void rfb_port_multi_init(rfb_interrupt_event_t *_rfb_interrupt_event);
#endif