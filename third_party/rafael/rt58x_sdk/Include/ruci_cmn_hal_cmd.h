/******************************************************************************
*
* @File         ruci_cmn_hal_cmd.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_CMN_HAL_CMD_H
#define _RUCI_CMN_HAL_CMD_H

#include "ruci_head.h"

#if (RUCI_ENABLE_CMN)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_CMN_HAL_CMD_HEADER 0x31

// RUCI: set_agc ---------------------------------------------------------------
#define RUCI_SET_AGC                            RUCI_NUM_SET_AGC, ruci_elmt_type_set_agc, ruci_elmt_num_set_agc
#define RUCI_CODE_SET_AGC                       0x01
#define RUCI_LEN_SET_AGC                        7
#define RUCI_NUM_SET_AGC                        7
#define RUCI_PARA_LEN_SET_AGC                   4
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_agc[];
extern const uint8_t ruci_elmt_num_set_agc[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_agc_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         agc_enable;
    uint8_t         lna_gain;
    uint8_t         vga_gain;
    uint8_t         tia_gain;
} ruci_para_set_agc_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_agc_t) */
#define SET_RUCI_PARA_SET_AGC(msg, agc_enable_in, lna_gain_in, vga_gain_in, tia_gain_in)        \
        do{                                                                                                            \
        ((ruci_para_set_agc_t *)msg)->ruci_header.u8                 = RUCI_CMN_HAL_CMD_HEADER;                \
        ((ruci_para_set_agc_t *)msg)->sub_header                     = RUCI_CODE_SET_AGC;                      \
        ((ruci_para_set_agc_t *)msg)->length                         = RUCI_PARA_LEN_SET_AGC;                  \
        ((ruci_para_set_agc_t *)msg)->agc_enable                     = agc_enable_in;                          \
        ((ruci_para_set_agc_t *)msg)->lna_gain                       = lna_gain_in;                            \
        ((ruci_para_set_agc_t *)msg)->vga_gain                       = vga_gain_in;                            \
        ((ruci_para_set_agc_t *)msg)->tia_gain                       = tia_gain_in;                            \
        }while(0)

// RUCI: set_calibration_enable ------------------------------------------------
#define RUCI_SET_CALIBRATION_ENABLE             RUCI_NUM_SET_CALIBRATION_ENABLE, ruci_elmt_type_set_calibration_enable, ruci_elmt_num_set_calibration_enable
#define RUCI_CODE_SET_CALIBRATION_ENABLE        0x02
#define RUCI_LEN_SET_CALIBRATION_ENABLE         4
#define RUCI_NUM_SET_CALIBRATION_ENABLE         4
#define RUCI_PARA_LEN_SET_CALIBRATION_ENABLE    1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_calibration_enable[];
extern const uint8_t ruci_elmt_num_set_calibration_enable[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_calibration_enable_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         rf_band;
} ruci_para_set_calibration_enable_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_calibration_enable_t) */
#define SET_RUCI_PARA_SET_CALIBRATION_ENABLE(msg, rf_band_in)        \
        do{                                                                                                            \
        ((ruci_para_set_calibration_enable_t *)msg)->ruci_header.u8                 = RUCI_CMN_HAL_CMD_HEADER;                \
        ((ruci_para_set_calibration_enable_t *)msg)->sub_header                     = RUCI_CODE_SET_CALIBRATION_ENABLE;       \
        ((ruci_para_set_calibration_enable_t *)msg)->length                         = RUCI_PARA_LEN_SET_CALIBRATION_ENABLE;   \
        ((ruci_para_set_calibration_enable_t *)msg)->rf_band                        = rf_band_in;                             \
        }while(0)

// RUCI: set_calibration_setting -----------------------------------------------
#define RUCI_SET_CALIBRATION_SETTING            RUCI_NUM_SET_CALIBRATION_SETTING, ruci_elmt_type_set_calibration_setting, ruci_elmt_num_set_calibration_setting
#define RUCI_CODE_SET_CALIBRATION_SETTING       0x03
#define RUCI_LEN_SET_CALIBRATION_SETTING        15
#define RUCI_NUM_SET_CALIBRATION_SETTING        9
#define RUCI_PARA_LEN_SET_CALIBRATION_SETTING   12
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_calibration_setting[];
extern const uint8_t ruci_elmt_num_set_calibration_setting[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_calibration_setting_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         rf_band;
    uint8_t         status;
    uint8_t         rx_filter;
    uint8_t         tx_lo[2];
    uint8_t         tx_sb[3];
    uint8_t         r_x_tia_dc[4];
} ruci_para_set_calibration_setting_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_calibration_setting_t) */
#define SET_RUCI_PARA_SET_CALIBRATION_SETTING(msg, rf_band_in, status_in, rx_filter_in, tx_lo_0_in, tx_lo_1_in, tx_sb_0_in, tx_sb_1_in, tx_sb_2_in, r_x_tia_dc_0_in, r_x_tia_dc_1_in, r_x_tia_dc_2_in, r_x_tia_dc_3_in)        \
        do{                                                                                                            \
        ((ruci_para_set_calibration_setting_t *)msg)->ruci_header.u8                 = RUCI_CMN_HAL_CMD_HEADER;                \
        ((ruci_para_set_calibration_setting_t *)msg)->sub_header                     = RUCI_CODE_SET_CALIBRATION_SETTING;      \
        ((ruci_para_set_calibration_setting_t *)msg)->length                         = RUCI_PARA_LEN_SET_CALIBRATION_SETTING;  \
        ((ruci_para_set_calibration_setting_t *)msg)->rf_band                        = rf_band_in;                             \
        ((ruci_para_set_calibration_setting_t *)msg)->status                         = status_in;                              \
        ((ruci_para_set_calibration_setting_t *)msg)->rx_filter                      = rx_filter_in;                           \
        ((ruci_para_set_calibration_setting_t *)msg)->tx_lo[0]                       = tx_lo_0_in;                             \
        ((ruci_para_set_calibration_setting_t *)msg)->tx_lo[1]                       = tx_lo_1_in;                             \
        ((ruci_para_set_calibration_setting_t *)msg)->tx_sb[0]                       = tx_sb_0_in;                             \
        ((ruci_para_set_calibration_setting_t *)msg)->tx_sb[1]                       = tx_sb_1_in;                             \
        ((ruci_para_set_calibration_setting_t *)msg)->tx_sb[2]                       = tx_sb_2_in;                             \
        ((ruci_para_set_calibration_setting_t *)msg)->r_x_tia_dc[0]                  = r_x_tia_dc_0_in;                        \
        ((ruci_para_set_calibration_setting_t *)msg)->r_x_tia_dc[1]                  = r_x_tia_dc_1_in;                        \
        ((ruci_para_set_calibration_setting_t *)msg)->r_x_tia_dc[2]                  = r_x_tia_dc_2_in;                        \
        ((ruci_para_set_calibration_setting_t *)msg)->r_x_tia_dc[3]                  = r_x_tia_dc_3_in;                        \
        }while(0)

// RUCI: set_tx_power ----------------------------------------------------------
#define RUCI_SET_TX_POWER                       RUCI_NUM_SET_TX_POWER, ruci_elmt_type_set_tx_power, ruci_elmt_num_set_tx_power
#define RUCI_CODE_SET_TX_POWER                  0x04
#define RUCI_LEN_SET_TX_POWER                   5
#define RUCI_NUM_SET_TX_POWER                   5
#define RUCI_PARA_LEN_SET_TX_POWER              2
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_tx_power[];
extern const uint8_t ruci_elmt_num_set_tx_power[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_tx_power_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         rf_band;
    uint8_t         tx_power;
} ruci_para_set_tx_power_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_tx_power_t) */
#define SET_RUCI_PARA_SET_TX_POWER(msg, rf_band_in, tx_power_in)        \
        do{                                                                                                            \
        ((ruci_para_set_tx_power_t *)msg)->ruci_header.u8                 = RUCI_CMN_HAL_CMD_HEADER;                \
        ((ruci_para_set_tx_power_t *)msg)->sub_header                     = RUCI_CODE_SET_TX_POWER;                 \
        ((ruci_para_set_tx_power_t *)msg)->length                         = RUCI_PARA_LEN_SET_TX_POWER;             \
        ((ruci_para_set_tx_power_t *)msg)->rf_band                        = rf_band_in;                             \
        ((ruci_para_set_tx_power_t *)msg)->tx_power                       = tx_power_in;                            \
        }while(0)

// RUCI: get_temperature_rpt ---------------------------------------------------
#define RUCI_GET_TEMPERATURE_RPT                RUCI_NUM_GET_TEMPERATURE_RPT, ruci_elmt_type_get_temperature_rpt, ruci_elmt_num_get_temperature_rpt
#define RUCI_CODE_GET_TEMPERATURE_RPT           0x05
#define RUCI_LEN_GET_TEMPERATURE_RPT            3
#define RUCI_NUM_GET_TEMPERATURE_RPT            3
#define RUCI_PARA_LEN_GET_TEMPERATURE_RPT       0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_temperature_rpt[];
extern const uint8_t ruci_elmt_num_get_temperature_rpt[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_temperature_rpt_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_get_temperature_rpt_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_temperature_rpt_t) */
#define SET_RUCI_PARA_GET_TEMPERATURE_RPT(msg)        \
        do{                                                                                                            \
        ((ruci_para_get_temperature_rpt_t *)msg)->ruci_header.u8                 = RUCI_CMN_HAL_CMD_HEADER;                \
        ((ruci_para_get_temperature_rpt_t *)msg)->sub_header                     = RUCI_CODE_GET_TEMPERATURE_RPT;          \
        ((ruci_para_get_temperature_rpt_t *)msg)->length                         = RUCI_PARA_LEN_GET_TEMPERATURE_RPT;      \
        }while(0)

// RUCI: get_voltage_rpt -------------------------------------------------------
#define RUCI_GET_VOLTAGE_RPT                    RUCI_NUM_GET_VOLTAGE_RPT, ruci_elmt_type_get_voltage_rpt, ruci_elmt_num_get_voltage_rpt
#define RUCI_CODE_GET_VOLTAGE_RPT               0x06
#define RUCI_LEN_GET_VOLTAGE_RPT                3
#define RUCI_NUM_GET_VOLTAGE_RPT                3
#define RUCI_PARA_LEN_GET_VOLTAGE_RPT           0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_voltage_rpt[];
extern const uint8_t ruci_elmt_num_get_voltage_rpt[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_voltage_rpt_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_get_voltage_rpt_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_voltage_rpt_t) */
#define SET_RUCI_PARA_GET_VOLTAGE_RPT(msg)        \
        do{                                                                                                            \
        ((ruci_para_get_voltage_rpt_t *)msg)->ruci_header.u8                 = RUCI_CMN_HAL_CMD_HEADER;                \
        ((ruci_para_get_voltage_rpt_t *)msg)->sub_header                     = RUCI_CODE_GET_VOLTAGE_RPT;              \
        ((ruci_para_get_voltage_rpt_t *)msg)->length                         = RUCI_PARA_LEN_GET_VOLTAGE_RPT;          \
        }while(0)

// RUCI: set_rssi_offset -------------------------------------------------------
#define RUCI_SET_RSSI_OFFSET                    RUCI_NUM_SET_RSSI_OFFSET, ruci_elmt_type_set_rssi_offset, ruci_elmt_num_set_rssi_offset
#define RUCI_CODE_SET_RSSI_OFFSET               0x07
#define RUCI_LEN_SET_RSSI_OFFSET                5
#define RUCI_NUM_SET_RSSI_OFFSET                5
#define RUCI_PARA_LEN_SET_RSSI_OFFSET           2
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_rssi_offset[];
extern const uint8_t ruci_elmt_num_set_rssi_offset[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_rssi_offset_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         rf_band;
    uint8_t         rssi_offset;
} ruci_para_set_rssi_offset_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_rssi_offset_t) */
#define SET_RUCI_PARA_SET_RSSI_OFFSET(msg, rf_band_in, rssi_offset_in)        \
        do{                                                                                                            \
        ((ruci_para_set_rssi_offset_t *)msg)->ruci_header.u8                 = RUCI_CMN_HAL_CMD_HEADER;                \
        ((ruci_para_set_rssi_offset_t *)msg)->sub_header                     = RUCI_CODE_SET_RSSI_OFFSET;              \
        ((ruci_para_set_rssi_offset_t *)msg)->length                         = RUCI_PARA_LEN_SET_RSSI_OFFSET;          \
        ((ruci_para_set_rssi_offset_t *)msg)->rf_band                        = rf_band_in;                             \
        ((ruci_para_set_rssi_offset_t *)msg)->rssi_offset                    = rssi_offset_in;                         \
        }while(0)

// RUCI: set_tx_power_compensation ---------------------------------------------
#define RUCI_SET_TX_POWER_COMPENSATION          RUCI_NUM_SET_TX_POWER_COMPENSATION, ruci_elmt_type_set_tx_power_compensation, ruci_elmt_num_set_tx_power_compensation
#define RUCI_CODE_SET_TX_POWER_COMPENSATION     0x08
#define RUCI_LEN_SET_TX_POWER_COMPENSATION      6
#define RUCI_NUM_SET_TX_POWER_COMPENSATION      6
#define RUCI_PARA_LEN_SET_TX_POWER_COMPENSATION 3
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_tx_power_compensation[];
extern const uint8_t ruci_elmt_num_set_tx_power_compensation[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_tx_power_compensation_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         tx_power_offset;
    uint8_t         tx_poly_gain;
    uint8_t         tx_pa_pw_pre;
} ruci_para_set_tx_power_compensation_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_tx_power_compensation_t) */
#define SET_RUCI_PARA_SET_TX_POWER_COMPENSATION(msg, tx_power_offset_in, tx_poly_gain_in, tx_pa_pw_pre_in)        \
        do{                                                                                                            \
        ((ruci_para_set_tx_power_compensation_t *)msg)->ruci_header.u8                 = RUCI_CMN_HAL_CMD_HEADER;                \
        ((ruci_para_set_tx_power_compensation_t *)msg)->sub_header                     = RUCI_CODE_SET_TX_POWER_COMPENSATION;    \
        ((ruci_para_set_tx_power_compensation_t *)msg)->length                         = RUCI_PARA_LEN_SET_TX_POWER_COMPENSATION;\
        ((ruci_para_set_tx_power_compensation_t *)msg)->tx_power_offset                = tx_power_offset_in;                     \
        ((ruci_para_set_tx_power_compensation_t *)msg)->tx_poly_gain                   = tx_poly_gain_in;                        \
        ((ruci_para_set_tx_power_compensation_t *)msg)->tx_pa_pw_pre                   = tx_pa_pw_pre_in;                        \
        }while(0)

// RUCI: set_tx_power_channel_compensation -------------------------------------
#define RUCI_SET_TX_POWER_CHANNEL_COMPENSATION  RUCI_NUM_SET_TX_POWER_CHANNEL_COMPENSATION, ruci_elmt_type_set_tx_power_channel_compensation, ruci_elmt_num_set_tx_power_channel_compensation
#define RUCI_CODE_SET_TX_POWER_CHANNEL_COMPENSATION 0x09
#define RUCI_LEN_SET_TX_POWER_CHANNEL_COMPENSATION 7
#define RUCI_NUM_SET_TX_POWER_CHANNEL_COMPENSATION 7
#define RUCI_PARA_LEN_SET_TX_POWER_CHANNEL_COMPENSATION 4
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_tx_power_channel_compensation[];
extern const uint8_t ruci_elmt_num_set_tx_power_channel_compensation[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_tx_power_channel_compensation_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         tx_power_offset0;
    uint8_t         tx_power_offset1;
    uint8_t         tx_power_offset2;
    uint8_t         tx_power_offset3;
} ruci_para_set_tx_power_channel_compensation_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_tx_power_channel_compensation_t) */
#define SET_RUCI_PARA_SET_TX_POWER_CHANNEL_COMPENSATION(msg, tx_power_offset0_in, tx_power_offset1_in, tx_power_offset2_in, tx_power_offset3_in)        \
        do{                                                                                                            \
        ((ruci_para_set_tx_power_channel_compensation_t *)msg)->ruci_header.u8                 = RUCI_CMN_HAL_CMD_HEADER;                \
        ((ruci_para_set_tx_power_channel_compensation_t *)msg)->sub_header                     = RUCI_CODE_SET_TX_POWER_CHANNEL_COMPENSATION;\
        ((ruci_para_set_tx_power_channel_compensation_t *)msg)->length                         = RUCI_PARA_LEN_SET_TX_POWER_CHANNEL_COMPENSATION;\
        ((ruci_para_set_tx_power_channel_compensation_t *)msg)->tx_power_offset0               = tx_power_offset0_in;                    \
        ((ruci_para_set_tx_power_channel_compensation_t *)msg)->tx_power_offset1               = tx_power_offset1_in;                    \
        ((ruci_para_set_tx_power_channel_compensation_t *)msg)->tx_power_offset2               = tx_power_offset2_in;                    \
        ((ruci_para_set_tx_power_channel_compensation_t *)msg)->tx_power_offset3               = tx_power_offset3_in;                    \
        }while(0)

// RUCI: set_tx_power_channel_segment ------------------------------------------
#define RUCI_SET_TX_POWER_CHANNEL_SEGMENT       RUCI_NUM_SET_TX_POWER_CHANNEL_SEGMENT, ruci_elmt_type_set_tx_power_channel_segment, ruci_elmt_num_set_tx_power_channel_segment
#define RUCI_CODE_SET_TX_POWER_CHANNEL_SEGMENT  0x0A
#define RUCI_LEN_SET_TX_POWER_CHANNEL_SEGMENT   6
#define RUCI_NUM_SET_TX_POWER_CHANNEL_SEGMENT   6
#define RUCI_PARA_LEN_SET_TX_POWER_CHANNEL_SEGMENT 3
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_tx_power_channel_segment[];
extern const uint8_t ruci_elmt_num_set_tx_power_channel_segment[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_tx_power_channel_segment_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         segment_a;
    uint8_t         segment_b;
    uint8_t         segment_c;
} ruci_para_set_tx_power_channel_segment_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_tx_power_channel_segment_t) */
#define SET_RUCI_PARA_SET_TX_POWER_CHANNEL_SEGMENT(msg, segment_a_in, segment_b_in, segment_c_in)        \
        do{                                                                                                            \
        ((ruci_para_set_tx_power_channel_segment_t *)msg)->ruci_header.u8                 = RUCI_CMN_HAL_CMD_HEADER;                \
        ((ruci_para_set_tx_power_channel_segment_t *)msg)->sub_header                     = RUCI_CODE_SET_TX_POWER_CHANNEL_SEGMENT; \
        ((ruci_para_set_tx_power_channel_segment_t *)msg)->length                         = RUCI_PARA_LEN_SET_TX_POWER_CHANNEL_SEGMENT;\
        ((ruci_para_set_tx_power_channel_segment_t *)msg)->segment_a                      = segment_a_in;                           \
        ((ruci_para_set_tx_power_channel_segment_t *)msg)->segment_b                      = segment_b_in;                           \
        ((ruci_para_set_tx_power_channel_segment_t *)msg)->segment_c                      = segment_c_in;                           \
        }while(0)

// RUCI: set_pmu_mpk_setting ---------------------------------------------------
#define RUCI_SET_PMU_MPK_SETTING                RUCI_NUM_SET_PMU_MPK_SETTING, ruci_elmt_type_set_pmu_mpk_setting, ruci_elmt_num_set_pmu_mpk_setting
#define RUCI_CODE_SET_PMU_MPK_SETTING           0x0B
#define RUCI_LEN_SET_PMU_MPK_SETTING            16
#define RUCI_NUM_SET_PMU_MPK_SETTING            13
#define RUCI_PARA_LEN_SET_PMU_MPK_SETTING       13
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_pmu_mpk_setting[];
extern const uint8_t ruci_elmt_num_set_pmu_mpk_setting[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_pmu_mpk_setting_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint16_t        status;
    uint8_t         xtal;
    uint8_t         sldo_vosel[3];
    uint8_t         lldo_vosel;
    uint8_t         dcdc_vosel_h;
    uint8_t         ldomv_vosel_h;
    uint8_t         dcdc_vosel_l;
    uint8_t         ldomv_vosel_l;
    uint8_t         bg_os;
    uint8_t         bg_os_dir;
} ruci_para_set_pmu_mpk_setting_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_pmu_mpk_setting_t) */
#define SET_RUCI_PARA_SET_PMU_MPK_SETTING(msg, status_in, xtal_in, sldo_vosel_0_in, sldo_vosel_1_in, sldo_vosel_2_in, lldo_vosel_in, dcdc_vosel_h_in, ldomv_vosel_h_in, dcdc_vosel_l_in, ldomv_vosel_l_in, bg_os_in, bg_os_dir_in)        \
        do{                                                                                                            \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->ruci_header.u8                 = RUCI_CMN_HAL_CMD_HEADER;                \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->sub_header                     = RUCI_CODE_SET_PMU_MPK_SETTING;          \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->length                         = RUCI_PARA_LEN_SET_PMU_MPK_SETTING;      \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->status                         = status_in;                              \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->xtal                           = xtal_in;                                \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->sldo_vosel[0]                  = sldo_vosel_0_in;                        \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->sldo_vosel[1]                  = sldo_vosel_1_in;                        \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->sldo_vosel[2]                  = sldo_vosel_2_in;                        \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->lldo_vosel                     = lldo_vosel_in;                          \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->dcdc_vosel_h                   = dcdc_vosel_h_in;                        \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->ldomv_vosel_h                  = ldomv_vosel_h_in;                       \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->dcdc_vosel_l                   = dcdc_vosel_l_in;                        \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->ldomv_vosel_l                  = ldomv_vosel_l_in;                       \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->bg_os                          = bg_os_in;                               \
        ((ruci_para_set_pmu_mpk_setting_t *)msg)->bg_os_dir                      = bg_os_dir_in;                           \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_CMN */
#endif /* _RUCI_CMN_HAL_CMD_H */
