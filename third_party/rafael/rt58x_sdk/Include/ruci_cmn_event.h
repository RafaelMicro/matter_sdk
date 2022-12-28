/******************************************************************************
*
* @File         ruci_cmn_event.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_CMN_EVENT_H
#define _RUCI_CMN_EVENT_H

#include "ruci_head.h"

#if (RUCI_ENABLE_CMN)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_CMN_EVENT_HEADER 0x3F

// RUCI: cmn_cnf_event ---------------------------------------------------------
#define RUCI_CMN_CNF_EVENT                      RUCI_NUM_CMN_CNF_EVENT, ruci_elmt_type_cmn_cnf_event, ruci_elmt_num_cmn_cnf_event
#define RUCI_CODE_CMN_CNF_EVENT                 0x01
#define RUCI_LEN_CMN_CNF_EVENT                  6
#define RUCI_NUM_CMN_CNF_EVENT                  6
#define RUCI_PARA_LEN_CMN_CNF_EVENT             3
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_cmn_cnf_event[];
extern const uint8_t ruci_elmt_num_cmn_cnf_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_cmn_cnf_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         cmn_cmd_header;
    uint8_t         cmn_cmd_subheader;
    uint8_t         status;
} ruci_para_cmn_cnf_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_cmn_cnf_event_t) */
#define SET_RUCI_PARA_CMN_CNF_EVENT(msg, cmn_cmd_header_in, cmn_cmd_subheader_in, status_in)        \
        do{                                                                                                            \
        ((ruci_para_cmn_cnf_event_t *)msg)->ruci_header.u8                 = RUCI_CMN_EVENT_HEADER;                  \
        ((ruci_para_cmn_cnf_event_t *)msg)->sub_header                     = RUCI_CODE_CMN_CNF_EVENT;                \
        ((ruci_para_cmn_cnf_event_t *)msg)->length                         = RUCI_PARA_LEN_CMN_CNF_EVENT;            \
        ((ruci_para_cmn_cnf_event_t *)msg)->cmn_cmd_header                 = cmn_cmd_header_in;                      \
        ((ruci_para_cmn_cnf_event_t *)msg)->cmn_cmd_subheader              = cmn_cmd_subheader_in;                   \
        ((ruci_para_cmn_cnf_event_t *)msg)->status                         = status_in;                              \
        }while(0)

// RUCI: get_fw_ver_event ------------------------------------------------------
#define RUCI_GET_FW_VER_EVENT                   RUCI_NUM_GET_FW_VER_EVENT, ruci_elmt_type_get_fw_ver_event, ruci_elmt_num_get_fw_ver_event
#define RUCI_CODE_GET_FW_VER_EVENT              0x02
#define RUCI_LEN_GET_FW_VER_EVENT               14
#define RUCI_NUM_GET_FW_VER_EVENT               8
#define RUCI_PARA_LEN_GET_FW_VER_EVENT          11
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_fw_ver_event[];
extern const uint8_t ruci_elmt_num_get_fw_ver_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_fw_ver_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint32_t        fw_ver;
    uint8_t         chip_model;
    uint8_t         chip_ver;
    uint8_t         feature_id;
    uint32_t        ruci_fw_ver;
} ruci_para_get_fw_ver_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_fw_ver_event_t) */
#define SET_RUCI_PARA_GET_FW_VER_EVENT(msg, fw_ver_in, chip_model_in, chip_ver_in, feature_id_in, ruci_fw_ver_in)        \
        do{                                                                                                            \
        ((ruci_para_get_fw_ver_event_t *)msg)->ruci_header.u8                 = RUCI_CMN_EVENT_HEADER;                  \
        ((ruci_para_get_fw_ver_event_t *)msg)->sub_header                     = RUCI_CODE_GET_FW_VER_EVENT;             \
        ((ruci_para_get_fw_ver_event_t *)msg)->length                         = RUCI_PARA_LEN_GET_FW_VER_EVENT;         \
        ((ruci_para_get_fw_ver_event_t *)msg)->fw_ver                         = fw_ver_in;                              \
        ((ruci_para_get_fw_ver_event_t *)msg)->chip_model                     = chip_model_in;                          \
        ((ruci_para_get_fw_ver_event_t *)msg)->chip_ver                       = chip_ver_in;                            \
        ((ruci_para_get_fw_ver_event_t *)msg)->feature_id                     = feature_id_in;                          \
        ((ruci_para_get_fw_ver_event_t *)msg)->ruci_fw_ver                    = ruci_fw_ver_in;                         \
        }while(0)

// RUCI: set_calibration_enable_event ------------------------------------------
#define RUCI_SET_CALIBRATION_ENABLE_EVENT       RUCI_NUM_SET_CALIBRATION_ENABLE_EVENT, ruci_elmt_type_set_calibration_enable_event, ruci_elmt_num_set_calibration_enable_event
#define RUCI_CODE_SET_CALIBRATION_ENABLE_EVENT  0x03
#define RUCI_LEN_SET_CALIBRATION_ENABLE_EVENT   14
#define RUCI_NUM_SET_CALIBRATION_ENABLE_EVENT   8
#define RUCI_PARA_LEN_SET_CALIBRATION_ENABLE_EVENT 11
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_calibration_enable_event[];
extern const uint8_t ruci_elmt_num_set_calibration_enable_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_calibration_enable_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         status;
    uint8_t         rx_filter;
    uint8_t         rx_tia_dc[4];
    uint8_t         tx_lo[2];
    uint8_t         tx_sb[3];
} ruci_para_set_calibration_enable_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_calibration_enable_event_t) */
#define SET_RUCI_PARA_SET_CALIBRATION_ENABLE_EVENT(msg, status_in, rx_filter_in, rx_tia_dc_0_in, rx_tia_dc_1_in, rx_tia_dc_2_in, rx_tia_dc_3_in, tx_lo_0_in, tx_lo_1_in, tx_sb_0_in, tx_sb_1_in, tx_sb_2_in)        \
        do{                                                                                                            \
        ((ruci_para_set_calibration_enable_event_t *)msg)->ruci_header.u8                 = RUCI_CMN_EVENT_HEADER;                  \
        ((ruci_para_set_calibration_enable_event_t *)msg)->sub_header                     = RUCI_CODE_SET_CALIBRATION_ENABLE_EVENT; \
        ((ruci_para_set_calibration_enable_event_t *)msg)->length                         = RUCI_PARA_LEN_SET_CALIBRATION_ENABLE_EVENT;\
        ((ruci_para_set_calibration_enable_event_t *)msg)->status                         = status_in;                              \
        ((ruci_para_set_calibration_enable_event_t *)msg)->rx_filter                      = rx_filter_in;                           \
        ((ruci_para_set_calibration_enable_event_t *)msg)->rx_tia_dc[0]                   = rx_tia_dc_0_in;                         \
        ((ruci_para_set_calibration_enable_event_t *)msg)->rx_tia_dc[1]                   = rx_tia_dc_1_in;                         \
        ((ruci_para_set_calibration_enable_event_t *)msg)->rx_tia_dc[2]                   = rx_tia_dc_2_in;                         \
        ((ruci_para_set_calibration_enable_event_t *)msg)->rx_tia_dc[3]                   = rx_tia_dc_3_in;                         \
        ((ruci_para_set_calibration_enable_event_t *)msg)->tx_lo[0]                       = tx_lo_0_in;                             \
        ((ruci_para_set_calibration_enable_event_t *)msg)->tx_lo[1]                       = tx_lo_1_in;                             \
        ((ruci_para_set_calibration_enable_event_t *)msg)->tx_sb[0]                       = tx_sb_0_in;                             \
        ((ruci_para_set_calibration_enable_event_t *)msg)->tx_sb[1]                       = tx_sb_1_in;                             \
        ((ruci_para_set_calibration_enable_event_t *)msg)->tx_sb[2]                       = tx_sb_2_in;                             \
        }while(0)

// RUCI: get_temperature_rpt_event ---------------------------------------------
#define RUCI_GET_TEMPERATURE_RPT_EVENT          RUCI_NUM_GET_TEMPERATURE_RPT_EVENT, ruci_elmt_type_get_temperature_rpt_event, ruci_elmt_num_get_temperature_rpt_event
#define RUCI_CODE_GET_TEMPERATURE_RPT_EVENT     0x04
#define RUCI_LEN_GET_TEMPERATURE_RPT_EVENT      8
#define RUCI_NUM_GET_TEMPERATURE_RPT_EVENT      5
#define RUCI_PARA_LEN_GET_TEMPERATURE_RPT_EVENT 5
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_temperature_rpt_event[];
extern const uint8_t ruci_elmt_num_get_temperature_rpt_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_temperature_rpt_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint32_t        time_stamp;
    uint8_t         termperature;
} ruci_para_get_temperature_rpt_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_temperature_rpt_event_t) */
#define SET_RUCI_PARA_GET_TEMPERATURE_RPT_EVENT(msg, time_stamp_in, termperature_in)        \
        do{                                                                                                            \
        ((ruci_para_get_temperature_rpt_event_t *)msg)->ruci_header.u8                 = RUCI_CMN_EVENT_HEADER;                  \
        ((ruci_para_get_temperature_rpt_event_t *)msg)->sub_header                     = RUCI_CODE_GET_TEMPERATURE_RPT_EVENT;    \
        ((ruci_para_get_temperature_rpt_event_t *)msg)->length                         = RUCI_PARA_LEN_GET_TEMPERATURE_RPT_EVENT;\
        ((ruci_para_get_temperature_rpt_event_t *)msg)->time_stamp                     = time_stamp_in;                          \
        ((ruci_para_get_temperature_rpt_event_t *)msg)->termperature                   = termperature_in;                        \
        }while(0)

// RUCI: get_voltage_rpt_event -------------------------------------------------
#define RUCI_GET_VOLTAGE_RPT_EVENT              RUCI_NUM_GET_VOLTAGE_RPT_EVENT, ruci_elmt_type_get_voltage_rpt_event, ruci_elmt_num_get_voltage_rpt_event
#define RUCI_CODE_GET_VOLTAGE_RPT_EVENT         0x05
#define RUCI_LEN_GET_VOLTAGE_RPT_EVENT          8
#define RUCI_NUM_GET_VOLTAGE_RPT_EVENT          5
#define RUCI_PARA_LEN_GET_VOLTAGE_RPT_EVENT     5
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_voltage_rpt_event[];
extern const uint8_t ruci_elmt_num_get_voltage_rpt_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_voltage_rpt_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint32_t        time_stamp;
    uint8_t         voltage;
} ruci_para_get_voltage_rpt_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_voltage_rpt_event_t) */
#define SET_RUCI_PARA_GET_VOLTAGE_RPT_EVENT(msg, time_stamp_in, voltage_in)        \
        do{                                                                                                            \
        ((ruci_para_get_voltage_rpt_event_t *)msg)->ruci_header.u8                 = RUCI_CMN_EVENT_HEADER;                  \
        ((ruci_para_get_voltage_rpt_event_t *)msg)->sub_header                     = RUCI_CODE_GET_VOLTAGE_RPT_EVENT;        \
        ((ruci_para_get_voltage_rpt_event_t *)msg)->length                         = RUCI_PARA_LEN_GET_VOLTAGE_RPT_EVENT;    \
        ((ruci_para_get_voltage_rpt_event_t *)msg)->time_stamp                     = time_stamp_in;                          \
        ((ruci_para_get_voltage_rpt_event_t *)msg)->voltage                        = voltage_in;                             \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_CMN */
#endif /* _RUCI_CMN_EVENT_H */
