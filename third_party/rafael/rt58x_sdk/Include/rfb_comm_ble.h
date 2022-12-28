/**
 * @file rfb_comm_ble.h
 * @author
 * @date
 * @brief Brief single line description use for indexing
 *
 * More detailed description can go here
 *
 *
 * @see http://
 */
#ifndef _RFB_COMM_BLE_H_
#define _RFB_COMM_BLE_H_
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
RFB_EVENT_STATUS rfb_comm_ble_initiate(void);
RFB_EVENT_STATUS rfb_comm_ble_modem_set(uint8_t data_rate, uint8_t coded_scheme);
RFB_EVENT_STATUS rfb_comm_ble_mac_set(uint32_t sfd_content, uint8_t whitening_en, uint8_t whitening_init_value, uint32_t crc_init_value);

#endif
