/******************************************************************************
*
* @File         ruci_pci_data.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_PCI_DATA_H
#define _RUCI_PCI_DATA_H

#include "ruci_head.h"

#if (RUCI_ENABLE_PCI)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_PCI_DATA_HEADER 0x17

// RUCI: set_tx_control_field --------------------------------------------------
#define RUCI_SET_TX_CONTROL_FIELD               RUCI_NUM_SET_TX_CONTROL_FIELD, ruci_elmt_type_set_tx_control_field, ruci_elmt_num_set_tx_control_field
#define RUCI_CODE_SET_TX_CONTROL_FIELD          0x01
#define RUCI_LEN_SET_TX_CONTROL_FIELD           6
#define RUCI_NUM_SET_TX_CONTROL_FIELD           5
#define RUCI_PARA_LEN_SET_TX_CONTROL_FIELD      2
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_tx_control_field[];
extern const uint8_t ruci_elmt_num_set_tx_control_field[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_tx_control_field_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint16_t        length;
    uint8_t         mac_control;
    uint8_t         mac_dsn;
} ruci_para_set_tx_control_field_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_tx_control_field_t) */
#define SET_RUCI_PARA_SET_TX_CONTROL_FIELD(msg, mac_control_in, mac_dsn_in)        \
        do{                                                                                                            \
        ((ruci_para_set_tx_control_field_t *)msg)->ruci_header.u8                 = RUCI_PCI_DATA_HEADER;                   \
        ((ruci_para_set_tx_control_field_t *)msg)->sub_header                     = RUCI_CODE_SET_TX_CONTROL_FIELD;         \
        ((ruci_para_set_tx_control_field_t *)msg)->length                         = RUCI_PARA_LEN_SET_TX_CONTROL_FIELD;     \
        ((ruci_para_set_tx_control_field_t *)msg)->mac_control                    = mac_control_in;                         \
        ((ruci_para_set_tx_control_field_t *)msg)->mac_dsn                        = mac_dsn_in;                             \
        }while(0)

// RUCI: rx_control_field ------------------------------------------------------
#define RUCI_RX_CONTROL_FIELD                   RUCI_NUM_RX_CONTROL_FIELD, ruci_elmt_type_rx_control_field, ruci_elmt_num_rx_control_field
#define RUCI_CODE_RX_CONTROL_FIELD              0x02
#define RUCI_LEN_RX_CONTROL_FIELD               7
#define RUCI_NUM_RX_CONTROL_FIELD               6
#define RUCI_PARA_LEN_RX_CONTROL_FIELD          3
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_rx_control_field[];
extern const uint8_t ruci_elmt_num_rx_control_field[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_rx_control_field_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint16_t        length;
    uint8_t         crc_status;
    uint8_t         rssi;
    uint8_t         snr;
} ruci_para_rx_control_field_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_rx_control_field_t) */
#define SET_RUCI_PARA_RX_CONTROL_FIELD(msg, crc_status_in, rssi_in, snr_in)        \
        do{                                                                                                            \
        ((ruci_para_rx_control_field_t *)msg)->ruci_header.u8                 = RUCI_PCI_DATA_HEADER;                   \
        ((ruci_para_rx_control_field_t *)msg)->sub_header                     = RUCI_CODE_RX_CONTROL_FIELD;             \
        ((ruci_para_rx_control_field_t *)msg)->length                         = RUCI_PARA_LEN_RX_CONTROL_FIELD;         \
        ((ruci_para_rx_control_field_t *)msg)->crc_status                     = crc_status_in;                          \
        ((ruci_para_rx_control_field_t *)msg)->rssi                           = rssi_in;                                \
        ((ruci_para_rx_control_field_t *)msg)->snr                            = snr_in;                                 \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_PCI */
#endif /* _RUCI_PCI_DATA_H */
