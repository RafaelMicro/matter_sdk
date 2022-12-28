/******************************************************************************
*
* @File         ruci_pci_slink_cmd.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_PCI_SLINK_CMD_H
#define _RUCI_PCI_SLINK_CMD_H

#include "ruci_head.h"

#if (RUCI_ENABLE_PCI)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_PCI_SLINK_CMD_HEADER 0x15

// RUCI: initiate_slink --------------------------------------------------------
#define RUCI_INITIATE_SLINK                     RUCI_NUM_INITIATE_SLINK, ruci_elmt_type_initiate_slink, ruci_elmt_num_initiate_slink
#define RUCI_CODE_INITIATE_SLINK                0x01
#define RUCI_LEN_INITIATE_SLINK                 3
#define RUCI_NUM_INITIATE_SLINK                 3
#define RUCI_PARA_LEN_INITIATE_SLINK            0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_initiate_slink[];
extern const uint8_t ruci_elmt_num_initiate_slink[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_initiate_slink_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_initiate_slink_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_initiate_slink_t) */
#define SET_RUCI_PARA_INITIATE_SLINK(msg)        \
        do{                                                                                                            \
        ((ruci_para_initiate_slink_t *)msg)->ruci_header.u8                 = RUCI_PCI_SLINK_CMD_HEADER;              \
        ((ruci_para_initiate_slink_t *)msg)->sub_header                     = RUCI_CODE_INITIATE_SLINK;               \
        ((ruci_para_initiate_slink_t *)msg)->length                         = RUCI_PARA_LEN_INITIATE_SLINK;           \
        }while(0)

// RUCI: set_slink_modem -------------------------------------------------------
#define RUCI_SET_SLINK_MODEM                    RUCI_NUM_SET_SLINK_MODEM, ruci_elmt_type_set_slink_modem, ruci_elmt_num_set_slink_modem
#define RUCI_CODE_SET_SLINK_MODEM               0x02
#define RUCI_LEN_SET_SLINK_MODEM                5
#define RUCI_NUM_SET_SLINK_MODEM                5
#define RUCI_PARA_LEN_SET_SLINK_MODEM           2
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_slink_modem[];
extern const uint8_t ruci_elmt_num_set_slink_modem[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_slink_modem_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         bandwidth;
    uint8_t         auto_config_mode;
} ruci_para_set_slink_modem_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_slink_modem_t) */
#define SET_RUCI_PARA_SET_SLINK_MODEM(msg, bandwidth_in, auto_config_mode_in)        \
        do{                                                                                                            \
        ((ruci_para_set_slink_modem_t *)msg)->ruci_header.u8                 = RUCI_PCI_SLINK_CMD_HEADER;              \
        ((ruci_para_set_slink_modem_t *)msg)->sub_header                     = RUCI_CODE_SET_SLINK_MODEM;              \
        ((ruci_para_set_slink_modem_t *)msg)->length                         = RUCI_PARA_LEN_SET_SLINK_MODEM;          \
        ((ruci_para_set_slink_modem_t *)msg)->bandwidth                      = bandwidth_in;                           \
        ((ruci_para_set_slink_modem_t *)msg)->auto_config_mode               = auto_config_mode_in;                    \
        }while(0)

// RUCI: set_slink_cad ---------------------------------------------------------
#define RUCI_SET_SLINK_CAD                      RUCI_NUM_SET_SLINK_CAD, ruci_elmt_type_set_slink_cad, ruci_elmt_num_set_slink_cad
#define RUCI_CODE_SET_SLINK_CAD                 0x03
#define RUCI_LEN_SET_SLINK_CAD                  10
#define RUCI_NUM_SET_SLINK_CAD                  7
#define RUCI_PARA_LEN_SET_SLINK_CAD             7
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_slink_cad[];
extern const uint8_t ruci_elmt_num_set_slink_cad[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_slink_cad_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         status_count_number;
    uint8_t         status_threshold;
    uint8_t         preamble_count_number;
    uint32_t        cad_on_time;
} ruci_para_set_slink_cad_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_slink_cad_t) */
#define SET_RUCI_PARA_SET_SLINK_CAD(msg, status_count_number_in, status_threshold_in, preamble_count_number_in, cad_on_time_in)        \
        do{                                                                                                            \
        ((ruci_para_set_slink_cad_t *)msg)->ruci_header.u8                 = RUCI_PCI_SLINK_CMD_HEADER;              \
        ((ruci_para_set_slink_cad_t *)msg)->sub_header                     = RUCI_CODE_SET_SLINK_CAD;                \
        ((ruci_para_set_slink_cad_t *)msg)->length                         = RUCI_PARA_LEN_SET_SLINK_CAD;            \
        ((ruci_para_set_slink_cad_t *)msg)->status_count_number            = status_count_number_in;                 \
        ((ruci_para_set_slink_cad_t *)msg)->status_threshold               = status_threshold_in;                    \
        ((ruci_para_set_slink_cad_t *)msg)->preamble_count_number          = preamble_count_number_in;               \
        ((ruci_para_set_slink_cad_t *)msg)->cad_on_time                    = cad_on_time_in;                         \
        }while(0)

// RUCI: set_slink_cmn_para ----------------------------------------------------
#define RUCI_SET_SLINK_CMN_PARA                 RUCI_NUM_SET_SLINK_CMN_PARA, ruci_elmt_type_set_slink_cmn_para, ruci_elmt_num_set_slink_cmn_para
#define RUCI_CODE_SET_SLINK_CMN_PARA            0x04
#define RUCI_LEN_SET_SLINK_CMN_PARA             6
#define RUCI_NUM_SET_SLINK_CMN_PARA             6
#define RUCI_PARA_LEN_SET_SLINK_CMN_PARA        3
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_slink_cmn_para[];
extern const uint8_t ruci_elmt_num_set_slink_cmn_para[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_slink_cmn_para_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         sf;
    uint8_t         code_rate;
    uint8_t         de;
} ruci_para_set_slink_cmn_para_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_slink_cmn_para_t) */
#define SET_RUCI_PARA_SET_SLINK_CMN_PARA(msg, sf_in, code_rate_in, de_in)        \
        do{                                                                                                            \
        ((ruci_para_set_slink_cmn_para_t *)msg)->ruci_header.u8                 = RUCI_PCI_SLINK_CMD_HEADER;              \
        ((ruci_para_set_slink_cmn_para_t *)msg)->sub_header                     = RUCI_CODE_SET_SLINK_CMN_PARA;           \
        ((ruci_para_set_slink_cmn_para_t *)msg)->length                         = RUCI_PARA_LEN_SET_SLINK_CMN_PARA;       \
        ((ruci_para_set_slink_cmn_para_t *)msg)->sf                             = sf_in;                                  \
        ((ruci_para_set_slink_cmn_para_t *)msg)->code_rate                      = code_rate_in;                           \
        ((ruci_para_set_slink_cmn_para_t *)msg)->de                             = de_in;                                  \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_PCI */
#endif /* _RUCI_PCI_SLINK_CMD_H */
