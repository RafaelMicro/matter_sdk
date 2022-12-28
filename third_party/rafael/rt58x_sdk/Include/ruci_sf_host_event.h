/******************************************************************************
*
* @File         ruci_sf_host_event.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_SF_HOST_EVENT_H
#define _RUCI_SF_HOST_EVENT_H

#include "ruci_head.h"

#if (RUCI_ENABLE_SF)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_SF_HOST_EVENT_HEADER 0xF1

// RUCI: mcu_state -------------------------------------------------------------
#define RUCI_MCU_STATE                          RUCI_NUM_MCU_STATE, ruci_elmt_type_mcu_state, ruci_elmt_num_mcu_state
#define RUCI_CODE_MCU_STATE                     0x01
#define RUCI_LEN_MCU_STATE                      4
#define RUCI_NUM_MCU_STATE                      4
#define RUCI_PARA_LEN_MCU_STATE                 1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_mcu_state[];
extern const uint8_t ruci_elmt_num_mcu_state[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_mcu_state_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         mcu_state;
} ruci_para_mcu_state_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_mcu_state_t) */
#define SET_RUCI_PARA_MCU_STATE(msg, mcu_state_in)        \
        do{                                                                                                            \
        ((ruci_para_mcu_state_t *)msg)->ruci_header.u8                 = RUCI_SF_HOST_EVENT_HEADER;              \
        ((ruci_para_mcu_state_t *)msg)->sub_header                     = RUCI_CODE_MCU_STATE;                    \
        ((ruci_para_mcu_state_t *)msg)->length                         = RUCI_PARA_LEN_MCU_STATE;                \
        ((ruci_para_mcu_state_t *)msg)->mcu_state                      = mcu_state_in;                           \
        }while(0)

// RUCI: host_cnf_event --------------------------------------------------------
#define RUCI_HOST_CNF_EVENT                     RUCI_NUM_HOST_CNF_EVENT, ruci_elmt_type_host_cnf_event, ruci_elmt_num_host_cnf_event
#define RUCI_CODE_HOST_CNF_EVENT                0x02
#define RUCI_LEN_HOST_CNF_EVENT                 6
#define RUCI_NUM_HOST_CNF_EVENT                 6
#define RUCI_PARA_LEN_HOST_CNF_EVENT            3
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_host_cnf_event[];
extern const uint8_t ruci_elmt_num_host_cnf_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_host_cnf_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         cmd_header;
    uint8_t         cmd_subheader;
    uint8_t         status;
} ruci_para_host_cnf_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_host_cnf_event_t) */
#define SET_RUCI_PARA_HOST_CNF_EVENT(msg, cmd_header_in, cmd_subheader_in, status_in)        \
        do{                                                                                                            \
        ((ruci_para_host_cnf_event_t *)msg)->ruci_header.u8                 = RUCI_SF_HOST_EVENT_HEADER;              \
        ((ruci_para_host_cnf_event_t *)msg)->sub_header                     = RUCI_CODE_HOST_CNF_EVENT;               \
        ((ruci_para_host_cnf_event_t *)msg)->length                         = RUCI_PARA_LEN_HOST_CNF_EVENT;           \
        ((ruci_para_host_cnf_event_t *)msg)->cmd_header                     = cmd_header_in;                          \
        ((ruci_para_host_cnf_event_t *)msg)->cmd_subheader                  = cmd_subheader_in;                       \
        ((ruci_para_host_cnf_event_t *)msg)->status                         = status_in;                              \
        }while(0)

// RUCI: apci_int_state --------------------------------------------------------
#define RUCI_APCI_INT_STATE                     RUCI_NUM_APCI_INT_STATE, ruci_elmt_type_apci_int_state, ruci_elmt_num_apci_int_state
#define RUCI_CODE_APCI_INT_STATE                0x03
#define RUCI_LEN_APCI_INT_STATE                 5
#define RUCI_NUM_APCI_INT_STATE                 5
#define RUCI_PARA_LEN_APCI_INT_STATE            2
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_apci_int_state[];
extern const uint8_t ruci_elmt_num_apci_int_state[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_apci_int_state_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         int_state;
    uint8_t         mcu_state;
} ruci_para_apci_int_state_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_apci_int_state_t) */
#define SET_RUCI_PARA_APCI_INT_STATE(msg, int_state_in, mcu_state_in)        \
        do{                                                                                                            \
        ((ruci_para_apci_int_state_t *)msg)->ruci_header.u8                 = RUCI_SF_HOST_EVENT_HEADER;              \
        ((ruci_para_apci_int_state_t *)msg)->sub_header                     = RUCI_CODE_APCI_INT_STATE;               \
        ((ruci_para_apci_int_state_t *)msg)->length                         = RUCI_PARA_LEN_APCI_INT_STATE;           \
        ((ruci_para_apci_int_state_t *)msg)->int_state                      = int_state_in;                           \
        ((ruci_para_apci_int_state_t *)msg)->mcu_state                      = mcu_state_in;                           \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_SF */
#endif /* _RUCI_SF_HOST_EVENT_H */
