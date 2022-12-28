/******************************************************************************
*
* @File         ruci_pci_fsk_cmd.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_PCI_FSK_CMD_H
#define _RUCI_PCI_FSK_CMD_H

#include "ruci_head.h"

#if (RUCI_ENABLE_PCI)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_PCI_FSK_CMD_HEADER 0x11

// RUCI: initiate_fsk ----------------------------------------------------------
#define RUCI_INITIATE_FSK                       RUCI_NUM_INITIATE_FSK, ruci_elmt_type_initiate_fsk, ruci_elmt_num_initiate_fsk
#define RUCI_CODE_INITIATE_FSK                  0x01
#define RUCI_LEN_INITIATE_FSK                   4
#define RUCI_NUM_INITIATE_FSK                   4
#define RUCI_PARA_LEN_INITIATE_FSK              1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_initiate_fsk[];
extern const uint8_t ruci_elmt_num_initiate_fsk[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_initiate_fsk_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         band_type;
} ruci_para_initiate_fsk_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_initiate_fsk_t) */
#define SET_RUCI_PARA_INITIATE_FSK(msg, band_type_in)        \
        do{                                                                                                            \
        ((ruci_para_initiate_fsk_t *)msg)->ruci_header.u8                 = RUCI_PCI_FSK_CMD_HEADER;                \
        ((ruci_para_initiate_fsk_t *)msg)->sub_header                     = RUCI_CODE_INITIATE_FSK;                 \
        ((ruci_para_initiate_fsk_t *)msg)->length                         = RUCI_PARA_LEN_INITIATE_FSK;             \
        ((ruci_para_initiate_fsk_t *)msg)->band_type                      = band_type_in;                           \
        }while(0)

// RUCI: set_fsk_modem ---------------------------------------------------------
#define RUCI_SET_FSK_MODEM                      RUCI_NUM_SET_FSK_MODEM, ruci_elmt_type_set_fsk_modem, ruci_elmt_num_set_fsk_modem
#define RUCI_CODE_SET_FSK_MODEM                 0x02
#define RUCI_LEN_SET_FSK_MODEM                  5
#define RUCI_NUM_SET_FSK_MODEM                  5
#define RUCI_PARA_LEN_SET_FSK_MODEM             2
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_fsk_modem[];
extern const uint8_t ruci_elmt_num_set_fsk_modem[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_fsk_modem_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         data_rate;
    uint8_t         mod_index;
} ruci_para_set_fsk_modem_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_fsk_modem_t) */
#define SET_RUCI_PARA_SET_FSK_MODEM(msg, data_rate_in, mod_index_in)        \
        do{                                                                                                            \
        ((ruci_para_set_fsk_modem_t *)msg)->ruci_header.u8                 = RUCI_PCI_FSK_CMD_HEADER;                \
        ((ruci_para_set_fsk_modem_t *)msg)->sub_header                     = RUCI_CODE_SET_FSK_MODEM;                \
        ((ruci_para_set_fsk_modem_t *)msg)->length                         = RUCI_PARA_LEN_SET_FSK_MODEM;            \
        ((ruci_para_set_fsk_modem_t *)msg)->data_rate                      = data_rate_in;                           \
        ((ruci_para_set_fsk_modem_t *)msg)->mod_index                      = mod_index_in;                           \
        }while(0)

// RUCI: set_fsk_mac -----------------------------------------------------------
#define RUCI_SET_FSK_MAC                        RUCI_NUM_SET_FSK_MAC, ruci_elmt_type_set_fsk_mac, ruci_elmt_num_set_fsk_mac
#define RUCI_CODE_SET_FSK_MAC                   0x03
#define RUCI_LEN_SET_FSK_MAC                    5
#define RUCI_NUM_SET_FSK_MAC                    5
#define RUCI_PARA_LEN_SET_FSK_MAC               2
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_fsk_mac[];
extern const uint8_t ruci_elmt_num_set_fsk_mac[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_fsk_mac_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         crc_type;
    uint8_t         whitening_en;
} ruci_para_set_fsk_mac_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_fsk_mac_t) */
#define SET_RUCI_PARA_SET_FSK_MAC(msg, crc_type_in, whitening_en_in)        \
        do{                                                                                                            \
        ((ruci_para_set_fsk_mac_t *)msg)->ruci_header.u8                 = RUCI_PCI_FSK_CMD_HEADER;                \
        ((ruci_para_set_fsk_mac_t *)msg)->sub_header                     = RUCI_CODE_SET_FSK_MAC;                  \
        ((ruci_para_set_fsk_mac_t *)msg)->length                         = RUCI_PARA_LEN_SET_FSK_MAC;              \
        ((ruci_para_set_fsk_mac_t *)msg)->crc_type                       = crc_type_in;                            \
        ((ruci_para_set_fsk_mac_t *)msg)->whitening_en                   = whitening_en_in;                        \
        }while(0)

// RUCI: set_fsk_preamble ------------------------------------------------------
#define RUCI_SET_FSK_PREAMBLE                   RUCI_NUM_SET_FSK_PREAMBLE, ruci_elmt_type_set_fsk_preamble, ruci_elmt_num_set_fsk_preamble
#define RUCI_CODE_SET_FSK_PREAMBLE              0x04
#define RUCI_LEN_SET_FSK_PREAMBLE               4
#define RUCI_NUM_SET_FSK_PREAMBLE               4
#define RUCI_PARA_LEN_SET_FSK_PREAMBLE          1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_fsk_preamble[];
extern const uint8_t ruci_elmt_num_set_fsk_preamble[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_fsk_preamble_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         preamble_length;
} ruci_para_set_fsk_preamble_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_fsk_preamble_t) */
#define SET_RUCI_PARA_SET_FSK_PREAMBLE(msg, preamble_length_in)        \
        do{                                                                                                            \
        ((ruci_para_set_fsk_preamble_t *)msg)->ruci_header.u8                 = RUCI_PCI_FSK_CMD_HEADER;                \
        ((ruci_para_set_fsk_preamble_t *)msg)->sub_header                     = RUCI_CODE_SET_FSK_PREAMBLE;             \
        ((ruci_para_set_fsk_preamble_t *)msg)->length                         = RUCI_PARA_LEN_SET_FSK_PREAMBLE;         \
        ((ruci_para_set_fsk_preamble_t *)msg)->preamble_length                = preamble_length_in;                     \
        }while(0)

// RUCI: set_fsk_sfd -----------------------------------------------------------
#define RUCI_SET_FSK_SFD                        RUCI_NUM_SET_FSK_SFD, ruci_elmt_type_set_fsk_sfd, ruci_elmt_num_set_fsk_sfd
#define RUCI_CODE_SET_FSK_SFD                   0x05
#define RUCI_LEN_SET_FSK_SFD                    7
#define RUCI_NUM_SET_FSK_SFD                    4
#define RUCI_PARA_LEN_SET_FSK_SFD               4
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_fsk_sfd[];
extern const uint8_t ruci_elmt_num_set_fsk_sfd[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_fsk_sfd_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint32_t        sfd_content;
} ruci_para_set_fsk_sfd_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_fsk_sfd_t) */
#define SET_RUCI_PARA_SET_FSK_SFD(msg, sfd_content_in)        \
        do{                                                                                                            \
        ((ruci_para_set_fsk_sfd_t *)msg)->ruci_header.u8                 = RUCI_PCI_FSK_CMD_HEADER;                \
        ((ruci_para_set_fsk_sfd_t *)msg)->sub_header                     = RUCI_CODE_SET_FSK_SFD;                  \
        ((ruci_para_set_fsk_sfd_t *)msg)->length                         = RUCI_PARA_LEN_SET_FSK_SFD;              \
        ((ruci_para_set_fsk_sfd_t *)msg)->sfd_content                    = sfd_content_in;                         \
        }while(0)

// RUCI: set_fsk_type ----------------------------------------------------------
#define RUCI_SET_FSK_TYPE                       RUCI_NUM_SET_FSK_TYPE, ruci_elmt_type_set_fsk_type, ruci_elmt_num_set_fsk_type
#define RUCI_CODE_SET_FSK_TYPE                  0x06
#define RUCI_LEN_SET_FSK_TYPE                   4
#define RUCI_NUM_SET_FSK_TYPE                   4
#define RUCI_PARA_LEN_SET_FSK_TYPE              1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_fsk_type[];
extern const uint8_t ruci_elmt_num_set_fsk_type[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_fsk_type_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         filter_type;
} ruci_para_set_fsk_type_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_fsk_type_t) */
#define SET_RUCI_PARA_SET_FSK_TYPE(msg, filter_type_in)        \
        do{                                                                                                            \
        ((ruci_para_set_fsk_type_t *)msg)->ruci_header.u8                 = RUCI_PCI_FSK_CMD_HEADER;                \
        ((ruci_para_set_fsk_type_t *)msg)->sub_header                     = RUCI_CODE_SET_FSK_TYPE;                 \
        ((ruci_para_set_fsk_type_t *)msg)->length                         = RUCI_PARA_LEN_SET_FSK_TYPE;             \
        ((ruci_para_set_fsk_type_t *)msg)->filter_type                    = filter_type_in;                         \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_PCI */
#endif /* _RUCI_PCI_FSK_CMD_H */
