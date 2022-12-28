/******************************************************************************
*
* @File         ruci_pci_oqpsk_cmd.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_PCI_OQPSK_CMD_H
#define _RUCI_PCI_OQPSK_CMD_H

#include "ruci_head.h"

#if (RUCI_ENABLE_PCI)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_PCI_OQPSK_CMD_HEADER 0x14

// RUCI: initiate_oqpsk --------------------------------------------------------
#define RUCI_INITIATE_OQPSK                     RUCI_NUM_INITIATE_OQPSK, ruci_elmt_type_initiate_oqpsk, ruci_elmt_num_initiate_oqpsk
#define RUCI_CODE_INITIATE_OQPSK                0x01
#define RUCI_LEN_INITIATE_OQPSK                 3
#define RUCI_NUM_INITIATE_OQPSK                 3
#define RUCI_PARA_LEN_INITIATE_OQPSK            0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_initiate_oqpsk[];
extern const uint8_t ruci_elmt_num_initiate_oqpsk[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_initiate_oqpsk_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_initiate_oqpsk_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_initiate_oqpsk_t) */
#define SET_RUCI_PARA_INITIATE_OQPSK(msg)        \
        do{                                                                                                            \
        ((ruci_para_initiate_oqpsk_t *)msg)->ruci_header.u8                 = RUCI_PCI_OQPSK_CMD_HEADER;              \
        ((ruci_para_initiate_oqpsk_t *)msg)->sub_header                     = RUCI_CODE_INITIATE_OQPSK;               \
        ((ruci_para_initiate_oqpsk_t *)msg)->length                         = RUCI_PARA_LEN_INITIATE_OQPSK;           \
        }while(0)

// RUCI: set_oqpsk_modem -------------------------------------------------------
#define RUCI_SET_OQPSK_MODEM                    RUCI_NUM_SET_OQPSK_MODEM, ruci_elmt_type_set_oqpsk_modem, ruci_elmt_num_set_oqpsk_modem
#define RUCI_CODE_SET_OQPSK_MODEM               0x02
#define RUCI_LEN_SET_OQPSK_MODEM                4
#define RUCI_NUM_SET_OQPSK_MODEM                4
#define RUCI_PARA_LEN_SET_OQPSK_MODEM           1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_oqpsk_modem[];
extern const uint8_t ruci_elmt_num_set_oqpsk_modem[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_oqpsk_modem_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         data_rate;
} ruci_para_set_oqpsk_modem_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_oqpsk_modem_t) */
#define SET_RUCI_PARA_SET_OQPSK_MODEM(msg, data_rate_in)        \
        do{                                                                                                            \
        ((ruci_para_set_oqpsk_modem_t *)msg)->ruci_header.u8                 = RUCI_PCI_OQPSK_CMD_HEADER;              \
        ((ruci_para_set_oqpsk_modem_t *)msg)->sub_header                     = RUCI_CODE_SET_OQPSK_MODEM;              \
        ((ruci_para_set_oqpsk_modem_t *)msg)->length                         = RUCI_PARA_LEN_SET_OQPSK_MODEM;          \
        ((ruci_para_set_oqpsk_modem_t *)msg)->data_rate                      = data_rate_in;                           \
        }while(0)

// RUCI: set_oqpsk_mac ---------------------------------------------------------
#define RUCI_SET_OQPSK_MAC                      RUCI_NUM_SET_OQPSK_MAC, ruci_elmt_type_set_oqpsk_mac, ruci_elmt_num_set_oqpsk_mac
#define RUCI_CODE_SET_OQPSK_MAC                 0x03
#define RUCI_LEN_SET_OQPSK_MAC                  5
#define RUCI_NUM_SET_OQPSK_MAC                  5
#define RUCI_PARA_LEN_SET_OQPSK_MAC             2
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_oqpsk_mac[];
extern const uint8_t ruci_elmt_num_set_oqpsk_mac[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_oqpsk_mac_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         crc_type;
    uint8_t         whitening_en;
} ruci_para_set_oqpsk_mac_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_oqpsk_mac_t) */
#define SET_RUCI_PARA_SET_OQPSK_MAC(msg, crc_type_in, whitening_en_in)        \
        do{                                                                                                            \
        ((ruci_para_set_oqpsk_mac_t *)msg)->ruci_header.u8                 = RUCI_PCI_OQPSK_CMD_HEADER;              \
        ((ruci_para_set_oqpsk_mac_t *)msg)->sub_header                     = RUCI_CODE_SET_OQPSK_MAC;                \
        ((ruci_para_set_oqpsk_mac_t *)msg)->length                         = RUCI_PARA_LEN_SET_OQPSK_MAC;            \
        ((ruci_para_set_oqpsk_mac_t *)msg)->crc_type                       = crc_type_in;                            \
        ((ruci_para_set_oqpsk_mac_t *)msg)->whitening_en                   = whitening_en_in;                        \
        }while(0)

// RUCI: set_oqpsk_extra_preamble ----------------------------------------------
#define RUCI_SET_OQPSK_EXTRA_PREAMBLE           RUCI_NUM_SET_OQPSK_EXTRA_PREAMBLE, ruci_elmt_type_set_oqpsk_extra_preamble, ruci_elmt_num_set_oqpsk_extra_preamble
#define RUCI_CODE_SET_OQPSK_EXTRA_PREAMBLE      0x04
#define RUCI_LEN_SET_OQPSK_EXTRA_PREAMBLE       4
#define RUCI_NUM_SET_OQPSK_EXTRA_PREAMBLE       4
#define RUCI_PARA_LEN_SET_OQPSK_EXTRA_PREAMBLE  1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_oqpsk_extra_preamble[];
extern const uint8_t ruci_elmt_num_set_oqpsk_extra_preamble[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_oqpsk_extra_preamble_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         extra_pramble_length;
} ruci_para_set_oqpsk_extra_preamble_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_oqpsk_extra_preamble_t) */
#define SET_RUCI_PARA_SET_OQPSK_EXTRA_PREAMBLE(msg, extra_pramble_length_in)        \
        do{                                                                                                            \
        ((ruci_para_set_oqpsk_extra_preamble_t *)msg)->ruci_header.u8                 = RUCI_PCI_OQPSK_CMD_HEADER;              \
        ((ruci_para_set_oqpsk_extra_preamble_t *)msg)->sub_header                     = RUCI_CODE_SET_OQPSK_EXTRA_PREAMBLE;     \
        ((ruci_para_set_oqpsk_extra_preamble_t *)msg)->length                         = RUCI_PARA_LEN_SET_OQPSK_EXTRA_PREAMBLE; \
        ((ruci_para_set_oqpsk_extra_preamble_t *)msg)->extra_pramble_length           = extra_pramble_length_in;                \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_PCI */
#endif /* _RUCI_PCI_OQPSK_CMD_H */
