/**
 * @file rfb_comm.h
 * @author
 * @date
 * @brief Brief single line description use for indexing
 *
 * More detailed description can go here
 *
 *
 * @see http://
 */
#ifndef _RFB_COMM_H_
#define _RFB_COMM_H_
/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "rfb_comm_common.h"
//#if (defined RFB_ZIGBEE_ENABLED && RFB_ZIGBEE_ENABLED == 1)
#if ((defined RFB_ZIGBEE_ENABLED && RFB_ZIGBEE_ENABLED == 1) || (defined RFB_15p4_MAC_ENABLED && RFB_15p4_MAC_ENABLED == 1))
#include "rfb_comm_15p4Mac.h"
#endif
#if (defined RFB_WISUN_ENABLED && RFB_WISUN_ENABLED == 1)
#include "rfb_comm_wisun.h"
#endif
#if (defined RFB_BLE_ENABLED && RFB_BLE_ENABLED == 1)
#include "rfb_comm_ble.h"
#endif
/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/**************************************************************************************************
 *    Global Prototypes
 *************************************************************************************************/

#endif

