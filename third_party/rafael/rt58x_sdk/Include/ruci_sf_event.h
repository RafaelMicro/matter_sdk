/******************************************************************************
*
* @File         ruci_sf_event.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_SF_EVENT_H
#define _RUCI_SF_EVENT_H

#include "ruci_head.h"

#if (RUCI_ENABLE_SF)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_SF_EVENT_HEADER 0xFF

// RUCI: sf_cnf_event ----------------------------------------------------------
#define RUCI_SF_CNF_EVENT                       RUCI_NUM_SF_CNF_EVENT, ruci_elmt_type_sf_cnf_event, ruci_elmt_num_sf_cnf_event
#define RUCI_CODE_SF_CNF_EVENT                  0x01
#define RUCI_LEN_SF_CNF_EVENT                   6
#define RUCI_NUM_SF_CNF_EVENT                   6
#define RUCI_PARA_LEN_SF_CNF_EVENT              3
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_sf_cnf_event[];
extern const uint8_t ruci_elmt_num_sf_cnf_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_sf_cnf_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         sf_cmd_header;
    uint8_t         sf_cmd_subheader;
    uint8_t         status;
} ruci_para_sf_cnf_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_sf_cnf_event_t) */
#define SET_RUCI_PARA_SF_CNF_EVENT(msg, sf_cmd_header_in, sf_cmd_subheader_in, status_in)        \
        do{                                                                                                            \
        ((ruci_para_sf_cnf_event_t *)msg)->ruci_header.u8                 = RUCI_SF_EVENT_HEADER;                   \
        ((ruci_para_sf_cnf_event_t *)msg)->sub_header                     = RUCI_CODE_SF_CNF_EVENT;                 \
        ((ruci_para_sf_cnf_event_t *)msg)->length                         = RUCI_PARA_LEN_SF_CNF_EVENT;             \
        ((ruci_para_sf_cnf_event_t *)msg)->sf_cmd_header                  = sf_cmd_header_in;                       \
        ((ruci_para_sf_cnf_event_t *)msg)->sf_cmd_subheader               = sf_cmd_subheader_in;                    \
        ((ruci_para_sf_cnf_event_t *)msg)->status                         = status_in;                              \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_SF */
#endif /* _RUCI_SF_EVENT_H */
