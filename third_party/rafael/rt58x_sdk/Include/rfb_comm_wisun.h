/**
 * @file rfb_comm_wisun.h
 * @author
 * @date
 * @brief Brief single line description use for indexing
 *
 * More detailed description can go here
 *
 *
 * @see http://
 */
#ifndef _RFB_COMM_WISUN_H_
#define _RFB_COMM_WISUN_H_
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
RFB_EVENT_STATUS rfb_comm_fsk_initiate(uint8_t band_type);
RFB_EVENT_STATUS rfb_comm_fsk_modem_set(uint8_t data_rate, uint8_t modulation_index);
RFB_EVENT_STATUS rfb_comm_fsk_mac_set(uint8_t crc_type, uint8_t whitening_enable);
RFB_EVENT_STATUS rfb_comm_fsk_preamble_set(uint8_t preamble_length);
RFB_EVENT_STATUS rfb_comm_fsk_sfd_set(uint32_t sfd_content);
RFB_EVENT_STATUS rfb_comm_fsk_type_set(uint8_t filter_type);
#endif

