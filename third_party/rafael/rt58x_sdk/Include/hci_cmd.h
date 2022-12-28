/** @file hci_cmd.h
 *
 * @brief Supported HCI commands for the controller.
 *
 * @addtogroup BLE
 * @{
 * @addtogroup hci HCI
 * @{
 * @addtogroup hci_cmd HCI commands
 * @}
 * @}
 */
#ifndef __HCI_CMD_H__
#define __HCI_CMD_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
/*
 * hci_cmd_controller.h includes
 *  [OGF = 0x03]
 *  - OCF = 0x0001 : Set Event Mask command
 *  - OCF = 0x0003 : Reset command
 *
 *  [OGF = 0x04]
 *  - OCF = 0x0001 : Read Local Version Information command
 *  - OCF = 0x0002 : Read Local Supported Commands command
 *  - OCF = 0x0003 : Read Local Supported Features command
 *  - OCF = 0x0005 : Read Buffer Size command
 *  - OCF = 0x0009 : Read BD_ADDR command
 *
 *  [OGF = 0x08]
 *  - OCF = 0x0001 : LE Set Event Mask command
 *  - OCF = 0x0002 : LE Read Buffer Size command
 *  - OCF = 0x0003 : LE Read Local Supported Features command
 *  - OCF = 0x0005 : LE Set Random Address command
 *  - OCF = 0x000F : LE Read Filter Accept List Size command
 *  - OCF = 0x0010 : LE Clear Filter Accept List command
 *  - OCF = 0x0011 : LE Add Device To Filter Accept List command
 *  - OCF = 0x0012 : LE Remove Device From Filter Accept List command
 *  - OCF = 0x001C : LE Read Supported States command
 *  - OCF = 0x0058 : LE Read Antenna Information command
 */
#include "hci_cmd_controller.h"


/*
 * hci_cmd_controller.h includes
 *  [OGF = 0x3F]
 *  - OCF = 0x0001 : Set Controller Information vendor command
 */
#include "hci_cmd_vendor.h"


/*  hci_cmd_advertising.h includes
 *  [OGF = 0x08]
 *  - OCF = 0x0006 : LE Set Advertising Parameters command
 *  - OCF = 0x0008 : LE Set Advertising Data command
 *  - OCF = 0x0009 : LE Set Scan Response Data command
 *  - OCF = 0x000A : LE Set Advertising Enable command
 */
#include "hci_cmd_advertising.h"


/*  hci_cmd_scan.h includes
 *  [OGF = 0x08]
 *  - OCF = 0x000B : LE Set Scan Parameters command
 *  - OCF = 0x000C : LE Set Scan Enable command
 */
#include "hci_cmd_scan.h"


/*
 * hci_cmd_connect.h includes
 *  [OGF = 0x01]
 *  - OCF = 0x0001 : Disconnect command
 *
 *  [OGF = 0x05]
 *  - OCF = 0x0005 : Read RSSI command
 *
 *  [OGF = 0x08]
 *  - OCF = 0x000D : LE Create Connection command
 *  - OCF = 0x000E : LE Create Connection Cancel command
 *  - OCF = 0x0013 : LE Connection Update command
 *  - OCF = 0x0014 : LE Set Host Channel Classification command
 *  - OCF = 0x0015 : LE Read Channel Map command
 *  - OCF = 0x0016 : LE Read Remote Features command
 *  - OCF = 0x0020 : LE Remote Connection Parameter Request Reply command
 *  - OCF = 0x0021 : LE Remote Connection Parameter Request Negative Reply command
 *  - OCF = 0x0022 : LE Set Data Length command
 *  - OCF = 0x0023 : LE Read Suggested Default Data Length command
 *  - OCF = 0x0024 : LE Write Suggested Default Data Length command
 *  - OCF = 0x002F : LE Read Maximum Data Length command
 *  - OCF = 0x0030 : LE Read PHY command
 *  - OCF = 0x0031 : LE Set Default PHY command
 *  - OCF = 0x0032 : LE Set PHY command
 */
#include "hci_cmd_connect.h"


/*  hci_cmd_security.h includes
 *  [OGF = 0x08]
 *  - OCF = 0x0017 : LE Encrypt command
 *  - OCF = 0x0018 : LE Rand command
 *  - OCF = 0x0019 : LE Enable Encryption command
 *  - OCF = 0x001A : LE Long Term Key Request Reply command
 *  - OCF = 0x001B : LE Long Term Key Request Negative Reply command
 */
#include "hci_cmd_security.h"


/*  hci_cmd_privacy.h includes
 *  [OGF = 0x08]
 *  - OCF = 0x0017 : LE Encrypt command
 *  - OCF = 0x0018 : LE Rand command
 *  - OCF = 0x0019 : LE Enable Encryption command
 *  - OCF = 0x001A : LE Long Term Key Request Reply command
 *  - OCF = 0x001B : LE Long Term Key Request Negative Reply command
 */
#include "hci_cmd_privacy.h"


/*  hci_cmd_connect_cte.h includes
 *  [OGF = 0x08]
 *  - OCF = 0x0054 : LE Set Connection CTE Receive Parameters command
 *  - OCF = 0x0055 : LE Set Connection CTE Transmit Parameters command
 *  - OCF = 0x0056 : LE Connection CTE Request Enable command
 *  - OCF = 0x0057 : LE Connection CTE Response Enable command
 */
#include "hci_cmd_connect_cte.h"


#endif // #define __HCI_CMD_H__
