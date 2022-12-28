/**
 * @file rfb_comm_zigbee.h
 * @author
 * @date
 * @brief Brief single line description use for indexing
 *
 * More detailed description can go here
 *
 *
 * @see http://
 */
#ifndef _RFB_COMM_ZIGBEE_H_
#define _RFB_COMM_SIGBEE_H_
/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "rf_mcu.h"
#include "rfb.h"
/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/**************************************************************************************************
 *    Global Prototypes
 *************************************************************************************************/
RFB_EVENT_STATUS rfb_comm_zigbee_initiate(void);
RFB_EVENT_STATUS rfb_comm_15p4_address_filter_set(uint8_t mac_promiscuous_mode, uint16_t short_source_address, uint32_t long_source_address_0, uint32_t long_source_address_1, uint16_t pan_id, uint8_t is_coordinator);
RFB_EVENT_STATUS rfb_comm_15p4_mac_pib_set(uint32_t a_unit_backoff_period, uint32_t mac_ack_wait_duration, uint8_t mac_max_BE, uint8_t mac_max_CSMA_backoffs,
        uint32_t mac_max_frame_total_wait_time, uint8_t mac_max_frame_retries, uint8_t mac_min_BE);
RFB_EVENT_STATUS rfb_comm_15p4_phy_pib_set(uint16_t a_turnaround_time, uint8_t phy_cca_mode, uint8_t phy_cca_threshold, uint16_t phy_cca_duration);
RFB_EVENT_STATUS rfb_comm_15p4_auto_ack_set(uint8_t auto_ack_enable);
RFB_EVENT_STATUS rfb_comm_15p4_pending_bit_set(uint8_t pending_bit_enable);
RFB_EVENT_STATUS rfb_comm_15p4_src_match_ctrl(uint8_t enable);
RFB_EVENT_STATUS rfb_comm_15p4_src_match_short_entry(uint8_t control_type, uint8_t *short_addr);
RFB_EVENT_STATUS rfb_comm_15p4_src_match_extended_entry(uint8_t control_type, uint8_t *extended_addr);
RFB_EVENT_STATUS rfb_comm_15p4_csl_receiver_ctrl(uint8_t csl_receiver_ctrl, uint16_t csl_period);
RFB_EVENT_STATUS rfb_comm_15p4_csl_accuracy_get(uint8_t *csl_accuracy);
RFB_EVENT_STATUS rfb_comm_15p4_csl_uncertainty_get(uint8_t *csl_uncertainty);
RFB_EVENT_STATUS rfb_comm_15p4_csl_sample_time_update(uint32_t csl_sample_time);
#endif

