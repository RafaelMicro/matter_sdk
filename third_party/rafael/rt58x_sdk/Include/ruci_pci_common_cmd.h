/******************************************************************************
*
* @File         ruci_pci_common_cmd.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_PCI_COMMON_CMD_H
#define _RUCI_PCI_COMMON_CMD_H

#include "ruci_head.h"

#if (RUCI_ENABLE_PCI)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_PCI_COMMON_CMD_HEADER 0x10

// RUCI: set_rf_frequency ------------------------------------------------------
#define RUCI_SET_RF_FREQUENCY                   RUCI_NUM_SET_RF_FREQUENCY, ruci_elmt_type_set_rf_frequency, ruci_elmt_num_set_rf_frequency
#define RUCI_CODE_SET_RF_FREQUENCY              0x01
#define RUCI_LEN_SET_RF_FREQUENCY               7
#define RUCI_NUM_SET_RF_FREQUENCY               4
#define RUCI_PARA_LEN_SET_RF_FREQUENCY          4
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_rf_frequency[];
extern const uint8_t ruci_elmt_num_set_rf_frequency[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_rf_frequency_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint32_t        rf_frequency;
} ruci_para_set_rf_frequency_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_rf_frequency_t) */
#define SET_RUCI_PARA_SET_RF_FREQUENCY(msg, rf_frequency_in)        \
        do{                                                                                                            \
        ((ruci_para_set_rf_frequency_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_rf_frequency_t *)msg)->sub_header                     = RUCI_CODE_SET_RF_FREQUENCY;             \
        ((ruci_para_set_rf_frequency_t *)msg)->length                         = RUCI_PARA_LEN_SET_RF_FREQUENCY;         \
        ((ruci_para_set_rf_frequency_t *)msg)->rf_frequency                   = rf_frequency_in;                        \
        }while(0)

// RUCI: set_rx_enable ---------------------------------------------------------
#define RUCI_SET_RX_ENABLE                      RUCI_NUM_SET_RX_ENABLE, ruci_elmt_type_set_rx_enable, ruci_elmt_num_set_rx_enable
#define RUCI_CODE_SET_RX_ENABLE                 0x03
#define RUCI_LEN_SET_RX_ENABLE                  7
#define RUCI_NUM_SET_RX_ENABLE                  4
#define RUCI_PARA_LEN_SET_RX_ENABLE             4
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_rx_enable[];
extern const uint8_t ruci_elmt_num_set_rx_enable[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_rx_enable_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint32_t        rx_on_time;
} ruci_para_set_rx_enable_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_rx_enable_t) */
#define SET_RUCI_PARA_SET_RX_ENABLE(msg, rx_on_time_in)        \
        do{                                                                                                            \
        ((ruci_para_set_rx_enable_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_rx_enable_t *)msg)->sub_header                     = RUCI_CODE_SET_RX_ENABLE;                \
        ((ruci_para_set_rx_enable_t *)msg)->length                         = RUCI_PARA_LEN_SET_RX_ENABLE;            \
        ((ruci_para_set_rx_enable_t *)msg)->rx_on_time                     = rx_on_time_in;                          \
        }while(0)

// RUCI: set_single_tone_mode --------------------------------------------------
#define RUCI_SET_SINGLE_TONE_MODE               RUCI_NUM_SET_SINGLE_TONE_MODE, ruci_elmt_type_set_single_tone_mode, ruci_elmt_num_set_single_tone_mode
#define RUCI_CODE_SET_SINGLE_TONE_MODE          0x04
#define RUCI_LEN_SET_SINGLE_TONE_MODE           4
#define RUCI_NUM_SET_SINGLE_TONE_MODE           4
#define RUCI_PARA_LEN_SET_SINGLE_TONE_MODE      1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_single_tone_mode[];
extern const uint8_t ruci_elmt_num_set_single_tone_mode[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_single_tone_mode_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         st_mode;
} ruci_para_set_single_tone_mode_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_single_tone_mode_t) */
#define SET_RUCI_PARA_SET_SINGLE_TONE_MODE(msg, st_mode_in)        \
        do{                                                                                                            \
        ((ruci_para_set_single_tone_mode_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_single_tone_mode_t *)msg)->sub_header                     = RUCI_CODE_SET_SINGLE_TONE_MODE;         \
        ((ruci_para_set_single_tone_mode_t *)msg)->length                         = RUCI_PARA_LEN_SET_SINGLE_TONE_MODE;     \
        ((ruci_para_set_single_tone_mode_t *)msg)->st_mode                        = st_mode_in;                             \
        }while(0)

// RUCI: get_crc_count ---------------------------------------------------------
#define RUCI_GET_CRC_COUNT                      RUCI_NUM_GET_CRC_COUNT, ruci_elmt_type_get_crc_count, ruci_elmt_num_get_crc_count
#define RUCI_CODE_GET_CRC_COUNT                 0x0C
#define RUCI_LEN_GET_CRC_COUNT                  3
#define RUCI_NUM_GET_CRC_COUNT                  3
#define RUCI_PARA_LEN_GET_CRC_COUNT             0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_crc_count[];
extern const uint8_t ruci_elmt_num_get_crc_count[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_crc_count_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_get_crc_count_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_crc_count_t) */
#define SET_RUCI_PARA_GET_CRC_COUNT(msg)        \
        do{                                                                                                            \
        ((ruci_para_get_crc_count_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_get_crc_count_t *)msg)->sub_header                     = RUCI_CODE_GET_CRC_COUNT;                \
        ((ruci_para_get_crc_count_t *)msg)->length                         = RUCI_PARA_LEN_GET_CRC_COUNT;            \
        }while(0)

// RUCI: set_rf_sleep ----------------------------------------------------------
#define RUCI_SET_RF_SLEEP                       RUCI_NUM_SET_RF_SLEEP, ruci_elmt_type_set_rf_sleep, ruci_elmt_num_set_rf_sleep
#define RUCI_CODE_SET_RF_SLEEP                  0x0D
#define RUCI_LEN_SET_RF_SLEEP                   4
#define RUCI_NUM_SET_RF_SLEEP                   4
#define RUCI_PARA_LEN_SET_RF_SLEEP              1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_rf_sleep[];
extern const uint8_t ruci_elmt_num_set_rf_sleep[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_rf_sleep_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         enable_flag;
} ruci_para_set_rf_sleep_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_rf_sleep_t) */
#define SET_RUCI_PARA_SET_RF_SLEEP(msg, enable_flag_in)        \
        do{                                                                                                            \
        ((ruci_para_set_rf_sleep_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_rf_sleep_t *)msg)->sub_header                     = RUCI_CODE_SET_RF_SLEEP;                 \
        ((ruci_para_set_rf_sleep_t *)msg)->length                         = RUCI_PARA_LEN_SET_RF_SLEEP;             \
        ((ruci_para_set_rf_sleep_t *)msg)->enable_flag                    = enable_flag_in;                         \
        }while(0)

// RUCI: set_rf_idle -----------------------------------------------------------
#define RUCI_SET_RF_IDLE                        RUCI_NUM_SET_RF_IDLE, ruci_elmt_type_set_rf_idle, ruci_elmt_num_set_rf_idle
#define RUCI_CODE_SET_RF_IDLE                   0x0E
#define RUCI_LEN_SET_RF_IDLE                    3
#define RUCI_NUM_SET_RF_IDLE                    3
#define RUCI_PARA_LEN_SET_RF_IDLE               0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_rf_idle[];
extern const uint8_t ruci_elmt_num_set_rf_idle[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_rf_idle_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_set_rf_idle_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_rf_idle_t) */
#define SET_RUCI_PARA_SET_RF_IDLE(msg)        \
        do{                                                                                                            \
        ((ruci_para_set_rf_idle_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_rf_idle_t *)msg)->sub_header                     = RUCI_CODE_SET_RF_IDLE;                  \
        ((ruci_para_set_rf_idle_t *)msg)->length                         = RUCI_PARA_LEN_SET_RF_IDLE;              \
        }while(0)

// RUCI: set_duty_cycle --------------------------------------------------------
#define RUCI_SET_DUTY_CYCLE                     RUCI_NUM_SET_DUTY_CYCLE, ruci_elmt_type_set_duty_cycle, ruci_elmt_num_set_duty_cycle
#define RUCI_CODE_SET_DUTY_CYCLE                0x0F
#define RUCI_LEN_SET_DUTY_CYCLE                 11
#define RUCI_NUM_SET_DUTY_CYCLE                 5
#define RUCI_PARA_LEN_SET_DUTY_CYCLE            8
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_duty_cycle[];
extern const uint8_t ruci_elmt_num_set_duty_cycle[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_duty_cycle_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint32_t        rx_on_time;
    uint32_t        sleep_time;
} ruci_para_set_duty_cycle_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_duty_cycle_t) */
#define SET_RUCI_PARA_SET_DUTY_CYCLE(msg, rx_on_time_in, sleep_time_in)        \
        do{                                                                                                            \
        ((ruci_para_set_duty_cycle_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_duty_cycle_t *)msg)->sub_header                     = RUCI_CODE_SET_DUTY_CYCLE;               \
        ((ruci_para_set_duty_cycle_t *)msg)->length                         = RUCI_PARA_LEN_SET_DUTY_CYCLE;           \
        ((ruci_para_set_duty_cycle_t *)msg)->rx_on_time                     = rx_on_time_in;                          \
        ((ruci_para_set_duty_cycle_t *)msg)->sleep_time                     = sleep_time_in;                          \
        }while(0)

// RUCI: get_rssi --------------------------------------------------------------
#define RUCI_GET_RSSI                           RUCI_NUM_GET_RSSI, ruci_elmt_type_get_rssi, ruci_elmt_num_get_rssi
#define RUCI_CODE_GET_RSSI                      0x12
#define RUCI_LEN_GET_RSSI                       3
#define RUCI_NUM_GET_RSSI                       3
#define RUCI_PARA_LEN_GET_RSSI                  0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_rssi[];
extern const uint8_t ruci_elmt_num_get_rssi[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_rssi_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_get_rssi_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_rssi_t) */
#define SET_RUCI_PARA_GET_RSSI(msg)        \
        do{                                                                                                            \
        ((ruci_para_get_rssi_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_get_rssi_t *)msg)->sub_header                     = RUCI_CODE_GET_RSSI;                     \
        ((ruci_para_get_rssi_t *)msg)->length                         = RUCI_PARA_LEN_GET_RSSI;                 \
        }while(0)

// RUCI: get_phy_status --------------------------------------------------------
#define RUCI_GET_PHY_STATUS                     RUCI_NUM_GET_PHY_STATUS, ruci_elmt_type_get_phy_status, ruci_elmt_num_get_phy_status
#define RUCI_CODE_GET_PHY_STATUS                0x14
#define RUCI_LEN_GET_PHY_STATUS                 3
#define RUCI_NUM_GET_PHY_STATUS                 3
#define RUCI_PARA_LEN_GET_PHY_STATUS            0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_phy_status[];
extern const uint8_t ruci_elmt_num_get_phy_status[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_phy_status_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_get_phy_status_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_phy_status_t) */
#define SET_RUCI_PARA_GET_PHY_STATUS(msg)        \
        do{                                                                                                            \
        ((ruci_para_get_phy_status_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_get_phy_status_t *)msg)->sub_header                     = RUCI_CODE_GET_PHY_STATUS;               \
        ((ruci_para_get_phy_status_t *)msg)->length                         = RUCI_PARA_LEN_GET_PHY_STATUS;           \
        }while(0)

// RUCI: set_clock_mode --------------------------------------------------------
#define RUCI_SET_CLOCK_MODE                     RUCI_NUM_SET_CLOCK_MODE, ruci_elmt_type_set_clock_mode, ruci_elmt_num_set_clock_mode
#define RUCI_CODE_SET_CLOCK_MODE                0x15
#define RUCI_LEN_SET_CLOCK_MODE                 6
#define RUCI_NUM_SET_CLOCK_MODE                 6
#define RUCI_PARA_LEN_SET_CLOCK_MODE            3
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_clock_mode[];
extern const uint8_t ruci_elmt_num_set_clock_mode[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_clock_mode_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         modem_type;
    uint8_t         band_type;
    uint8_t         clock_mode;
} ruci_para_set_clock_mode_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_clock_mode_t) */
#define SET_RUCI_PARA_SET_CLOCK_MODE(msg, modem_type_in, band_type_in, clock_mode_in)        \
        do{                                                                                                            \
        ((ruci_para_set_clock_mode_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_clock_mode_t *)msg)->sub_header                     = RUCI_CODE_SET_CLOCK_MODE;               \
        ((ruci_para_set_clock_mode_t *)msg)->length                         = RUCI_PARA_LEN_SET_CLOCK_MODE;           \
        ((ruci_para_set_clock_mode_t *)msg)->modem_type                     = modem_type_in;                          \
        ((ruci_para_set_clock_mode_t *)msg)->band_type                      = band_type_in;                           \
        ((ruci_para_set_clock_mode_t *)msg)->clock_mode                     = clock_mode_in;                          \
        }while(0)

// RUCI: set_rfb_auto_state ----------------------------------------------------
#define RUCI_SET_RFB_AUTO_STATE                 RUCI_NUM_SET_RFB_AUTO_STATE, ruci_elmt_type_set_rfb_auto_state, ruci_elmt_num_set_rfb_auto_state
#define RUCI_CODE_SET_RFB_AUTO_STATE            0x16
#define RUCI_LEN_SET_RFB_AUTO_STATE             4
#define RUCI_NUM_SET_RFB_AUTO_STATE             4
#define RUCI_PARA_LEN_SET_RFB_AUTO_STATE        1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_rfb_auto_state[];
extern const uint8_t ruci_elmt_num_set_rfb_auto_state[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_rfb_auto_state_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         rx_on_when_idle;
} ruci_para_set_rfb_auto_state_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_rfb_auto_state_t) */
#define SET_RUCI_PARA_SET_RFB_AUTO_STATE(msg, rx_on_when_idle_in)        \
        do{                                                                                                            \
        ((ruci_para_set_rfb_auto_state_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_rfb_auto_state_t *)msg)->sub_header                     = RUCI_CODE_SET_RFB_AUTO_STATE;           \
        ((ruci_para_set_rfb_auto_state_t *)msg)->length                         = RUCI_PARA_LEN_SET_RFB_AUTO_STATE;       \
        ((ruci_para_set_rfb_auto_state_t *)msg)->rx_on_when_idle                = rx_on_when_idle_in;                     \
        }while(0)

// RUCI: set_rfe_security ------------------------------------------------------
#define RUCI_SET_RFE_SECURITY                   RUCI_NUM_SET_RFE_SECURITY, ruci_elmt_type_set_rfe_security, ruci_elmt_num_set_rfe_security
#define RUCI_CODE_SET_RFE_SECURITY              0x17
#define RUCI_LEN_SET_RFE_SECURITY               32
#define RUCI_NUM_SET_RFE_SECURITY               5
#define RUCI_PARA_LEN_SET_RFE_SECURITY          29
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_rfe_security[];
extern const uint8_t ruci_elmt_num_set_rfe_security[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_rfe_security_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         sec_key[16];
    uint8_t         sec_nonce[13];
} ruci_para_set_rfe_security_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_rfe_security_t) */
#define SET_RUCI_PARA_SET_RFE_SECURITY(msg, sec_key_0_in, sec_key_1_in, sec_key_2_in, sec_key_3_in, sec_key_4_in, sec_key_5_in, sec_key_6_in, sec_key_7_in, sec_key_8_in, sec_key_9_in, sec_key_10_in, sec_key_11_in, sec_key_12_in, sec_key_13_in, sec_key_14_in, sec_key_15_in, sec_nonce_0_in, sec_nonce_1_in, sec_nonce_2_in, sec_nonce_3_in, sec_nonce_4_in, sec_nonce_5_in, sec_nonce_6_in, sec_nonce_7_in, sec_nonce_8_in, sec_nonce_9_in, sec_nonce_10_in, sec_nonce_11_in, sec_nonce_12_in)        \
        do{                                                                                                            \
        ((ruci_para_set_rfe_security_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_rfe_security_t *)msg)->sub_header                     = RUCI_CODE_SET_RFE_SECURITY;             \
        ((ruci_para_set_rfe_security_t *)msg)->length                         = RUCI_PARA_LEN_SET_RFE_SECURITY;         \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[0]                     = sec_key_0_in;                           \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[1]                     = sec_key_1_in;                           \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[2]                     = sec_key_2_in;                           \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[3]                     = sec_key_3_in;                           \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[4]                     = sec_key_4_in;                           \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[5]                     = sec_key_5_in;                           \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[6]                     = sec_key_6_in;                           \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[7]                     = sec_key_7_in;                           \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[8]                     = sec_key_8_in;                           \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[9]                     = sec_key_9_in;                           \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[10]                    = sec_key_10_in;                          \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[11]                    = sec_key_11_in;                          \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[12]                    = sec_key_12_in;                          \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[13]                    = sec_key_13_in;                          \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[14]                    = sec_key_14_in;                          \
        ((ruci_para_set_rfe_security_t *)msg)->sec_key[15]                    = sec_key_15_in;                          \
        ((ruci_para_set_rfe_security_t *)msg)->sec_nonce[0]                   = sec_nonce_0_in;                         \
        ((ruci_para_set_rfe_security_t *)msg)->sec_nonce[1]                   = sec_nonce_1_in;                         \
        ((ruci_para_set_rfe_security_t *)msg)->sec_nonce[2]                   = sec_nonce_2_in;                         \
        ((ruci_para_set_rfe_security_t *)msg)->sec_nonce[3]                   = sec_nonce_3_in;                         \
        ((ruci_para_set_rfe_security_t *)msg)->sec_nonce[4]                   = sec_nonce_4_in;                         \
        ((ruci_para_set_rfe_security_t *)msg)->sec_nonce[5]                   = sec_nonce_5_in;                         \
        ((ruci_para_set_rfe_security_t *)msg)->sec_nonce[6]                   = sec_nonce_6_in;                         \
        ((ruci_para_set_rfe_security_t *)msg)->sec_nonce[7]                   = sec_nonce_7_in;                         \
        ((ruci_para_set_rfe_security_t *)msg)->sec_nonce[8]                   = sec_nonce_8_in;                         \
        ((ruci_para_set_rfe_security_t *)msg)->sec_nonce[9]                   = sec_nonce_9_in;                         \
        ((ruci_para_set_rfe_security_t *)msg)->sec_nonce[10]                  = sec_nonce_10_in;                        \
        ((ruci_para_set_rfe_security_t *)msg)->sec_nonce[11]                  = sec_nonce_11_in;                        \
        ((ruci_para_set_rfe_security_t *)msg)->sec_nonce[12]                  = sec_nonce_12_in;                        \
        }while(0)

// RUCI: set_rfe_tx_enable -----------------------------------------------------
#define RUCI_SET_RFE_TX_ENABLE                  RUCI_NUM_SET_RFE_TX_ENABLE, ruci_elmt_type_set_rfe_tx_enable, ruci_elmt_num_set_rfe_tx_enable
#define RUCI_CODE_SET_RFE_TX_ENABLE             0x18
#define RUCI_LEN_SET_RFE_TX_ENABLE              23
#define RUCI_NUM_SET_RFE_TX_ENABLE              14
#define RUCI_PARA_LEN_SET_RFE_TX_ENABLE         20
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_rfe_tx_enable[];
extern const uint8_t ruci_elmt_num_set_rfe_tx_enable[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_rfe_tx_enable_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint16_t        pkt_interval;
    uint16_t        pkt_length;
    uint16_t        pkt_step_size;
    uint16_t        pkt_count;
    uint8_t         pkt_type;
    uint8_t         pkt_phr_type;
    uint8_t         ack_enable;
    uint32_t        ack_timeout;
    uint8_t         sec_level;
    uint16_t        sec_a_data_len;
    uint16_t        sec_m_data_len;
} ruci_para_set_rfe_tx_enable_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_rfe_tx_enable_t) */
#define SET_RUCI_PARA_SET_RFE_TX_ENABLE(msg, pkt_interval_in, pkt_length_in, pkt_step_size_in, pkt_count_in, pkt_type_in, pkt_phr_type_in, ack_enable_in, ack_timeout_in, sec_level_in, sec_a_data_len_in, sec_m_data_len_in)        \
        do{                                                                                                            \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->sub_header                     = RUCI_CODE_SET_RFE_TX_ENABLE;            \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->length                         = RUCI_PARA_LEN_SET_RFE_TX_ENABLE;        \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->pkt_interval                   = pkt_interval_in;                        \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->pkt_length                     = pkt_length_in;                          \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->pkt_step_size                  = pkt_step_size_in;                       \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->pkt_count                      = pkt_count_in;                           \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->pkt_type                       = pkt_type_in;                            \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->pkt_phr_type                   = pkt_phr_type_in;                        \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->ack_enable                     = ack_enable_in;                          \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->ack_timeout                    = ack_timeout_in;                         \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->sec_level                      = sec_level_in;                           \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->sec_a_data_len                 = sec_a_data_len_in;                      \
        ((ruci_para_set_rfe_tx_enable_t *)msg)->sec_m_data_len                 = sec_m_data_len_in;                      \
        }while(0)

// RUCI: set_rfe_tx_disable ----------------------------------------------------
#define RUCI_SET_RFE_TX_DISABLE                 RUCI_NUM_SET_RFE_TX_DISABLE, ruci_elmt_type_set_rfe_tx_disable, ruci_elmt_num_set_rfe_tx_disable
#define RUCI_CODE_SET_RFE_TX_DISABLE            0x19
#define RUCI_LEN_SET_RFE_TX_DISABLE             3
#define RUCI_NUM_SET_RFE_TX_DISABLE             3
#define RUCI_PARA_LEN_SET_RFE_TX_DISABLE        0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_rfe_tx_disable[];
extern const uint8_t ruci_elmt_num_set_rfe_tx_disable[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_rfe_tx_disable_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_set_rfe_tx_disable_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_rfe_tx_disable_t) */
#define SET_RUCI_PARA_SET_RFE_TX_DISABLE(msg)        \
        do{                                                                                                            \
        ((ruci_para_set_rfe_tx_disable_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_rfe_tx_disable_t *)msg)->sub_header                     = RUCI_CODE_SET_RFE_TX_DISABLE;           \
        ((ruci_para_set_rfe_tx_disable_t *)msg)->length                         = RUCI_PARA_LEN_SET_RFE_TX_DISABLE;       \
        }while(0)

// RUCI: set_rfe_rx_enable -----------------------------------------------------
#define RUCI_SET_RFE_RX_ENABLE                  RUCI_NUM_SET_RFE_RX_ENABLE, ruci_elmt_type_set_rfe_rx_enable, ruci_elmt_num_set_rfe_rx_enable
#define RUCI_CODE_SET_RFE_RX_ENABLE             0x1A
#define RUCI_LEN_SET_RFE_RX_ENABLE              19
#define RUCI_NUM_SET_RFE_RX_ENABLE              13
#define RUCI_PARA_LEN_SET_RFE_RX_ENABLE         16
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_rfe_rx_enable[];
extern const uint8_t ruci_elmt_num_set_rfe_rx_enable[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_rfe_rx_enable_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         periodic_report_cycle;
    uint8_t         pkt_type;
    uint8_t         queue_type;
    uint8_t         rx_type;
    uint32_t        rx_timeout;
    uint8_t         ack_enable;
    uint16_t        ack_interval;
    uint8_t         sec_level;
    uint16_t        sec_a_data_len;
    uint16_t        sec_m_data_len;
} ruci_para_set_rfe_rx_enable_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_rfe_rx_enable_t) */
#define SET_RUCI_PARA_SET_RFE_RX_ENABLE(msg, periodic_report_cycle_in, pkt_type_in, queue_type_in, rx_type_in, rx_timeout_in, ack_enable_in, ack_interval_in, sec_level_in, sec_a_data_len_in, sec_m_data_len_in)        \
        do{                                                                                                            \
        ((ruci_para_set_rfe_rx_enable_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_rfe_rx_enable_t *)msg)->sub_header                     = RUCI_CODE_SET_RFE_RX_ENABLE;            \
        ((ruci_para_set_rfe_rx_enable_t *)msg)->length                         = RUCI_PARA_LEN_SET_RFE_RX_ENABLE;        \
        ((ruci_para_set_rfe_rx_enable_t *)msg)->periodic_report_cycle          = periodic_report_cycle_in;               \
        ((ruci_para_set_rfe_rx_enable_t *)msg)->pkt_type                       = pkt_type_in;                            \
        ((ruci_para_set_rfe_rx_enable_t *)msg)->queue_type                     = queue_type_in;                          \
        ((ruci_para_set_rfe_rx_enable_t *)msg)->rx_type                        = rx_type_in;                             \
        ((ruci_para_set_rfe_rx_enable_t *)msg)->rx_timeout                     = rx_timeout_in;                          \
        ((ruci_para_set_rfe_rx_enable_t *)msg)->ack_enable                     = ack_enable_in;                          \
        ((ruci_para_set_rfe_rx_enable_t *)msg)->ack_interval                   = ack_interval_in;                        \
        ((ruci_para_set_rfe_rx_enable_t *)msg)->sec_level                      = sec_level_in;                           \
        ((ruci_para_set_rfe_rx_enable_t *)msg)->sec_a_data_len                 = sec_a_data_len_in;                      \
        ((ruci_para_set_rfe_rx_enable_t *)msg)->sec_m_data_len                 = sec_m_data_len_in;                      \
        }while(0)

// RUCI: set_rfe_rx_disable ----------------------------------------------------
#define RUCI_SET_RFE_RX_DISABLE                 RUCI_NUM_SET_RFE_RX_DISABLE, ruci_elmt_type_set_rfe_rx_disable, ruci_elmt_num_set_rfe_rx_disable
#define RUCI_CODE_SET_RFE_RX_DISABLE            0x1B
#define RUCI_LEN_SET_RFE_RX_DISABLE             3
#define RUCI_NUM_SET_RFE_RX_DISABLE             3
#define RUCI_PARA_LEN_SET_RFE_RX_DISABLE        0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_rfe_rx_disable[];
extern const uint8_t ruci_elmt_num_set_rfe_rx_disable[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_rfe_rx_disable_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_set_rfe_rx_disable_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_rfe_rx_disable_t) */
#define SET_RUCI_PARA_SET_RFE_RX_DISABLE(msg)        \
        do{                                                                                                            \
        ((ruci_para_set_rfe_rx_disable_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_rfe_rx_disable_t *)msg)->sub_header                     = RUCI_CODE_SET_RFE_RX_DISABLE;           \
        ((ruci_para_set_rfe_rx_disable_t *)msg)->length                         = RUCI_PARA_LEN_SET_RFE_RX_DISABLE;       \
        }while(0)

// RUCI: set_rfe_mode ----------------------------------------------------------
#define RUCI_SET_RFE_MODE                       RUCI_NUM_SET_RFE_MODE, ruci_elmt_type_set_rfe_mode, ruci_elmt_num_set_rfe_mode
#define RUCI_CODE_SET_RFE_MODE                  0x1E
#define RUCI_LEN_SET_RFE_MODE                   4
#define RUCI_NUM_SET_RFE_MODE                   4
#define RUCI_PARA_LEN_SET_RFE_MODE              1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_rfe_mode[];
extern const uint8_t ruci_elmt_num_set_rfe_mode[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_rfe_mode_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         mode;
} ruci_para_set_rfe_mode_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_rfe_mode_t) */
#define SET_RUCI_PARA_SET_RFE_MODE(msg, mode_in)        \
        do{                                                                                                            \
        ((ruci_para_set_rfe_mode_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_rfe_mode_t *)msg)->sub_header                     = RUCI_CODE_SET_RFE_MODE;                 \
        ((ruci_para_set_rfe_mode_t *)msg)->length                         = RUCI_PARA_LEN_SET_RFE_MODE;             \
        ((ruci_para_set_rfe_mode_t *)msg)->mode                           = mode_in;                                \
        }while(0)

// RUCI: set_gaussian_filter_type ----------------------------------------------
#define RUCI_SET_GAUSSIAN_FILTER_TYPE           RUCI_NUM_SET_GAUSSIAN_FILTER_TYPE, ruci_elmt_type_set_gaussian_filter_type, ruci_elmt_num_set_gaussian_filter_type
#define RUCI_CODE_SET_GAUSSIAN_FILTER_TYPE      0x1F
#define RUCI_LEN_SET_GAUSSIAN_FILTER_TYPE       4
#define RUCI_NUM_SET_GAUSSIAN_FILTER_TYPE       4
#define RUCI_PARA_LEN_SET_GAUSSIAN_FILTER_TYPE  1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_gaussian_filter_type[];
extern const uint8_t ruci_elmt_num_set_gaussian_filter_type[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_gaussian_filter_type_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         type;
} ruci_para_set_gaussian_filter_type_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_gaussian_filter_type_t) */
#define SET_RUCI_PARA_SET_GAUSSIAN_FILTER_TYPE(msg, type_in)        \
        do{                                                                                                            \
        ((ruci_para_set_gaussian_filter_type_t *)msg)->ruci_header.u8                 = RUCI_PCI_COMMON_CMD_HEADER;             \
        ((ruci_para_set_gaussian_filter_type_t *)msg)->sub_header                     = RUCI_CODE_SET_GAUSSIAN_FILTER_TYPE;     \
        ((ruci_para_set_gaussian_filter_type_t *)msg)->length                         = RUCI_PARA_LEN_SET_GAUSSIAN_FILTER_TYPE; \
        ((ruci_para_set_gaussian_filter_type_t *)msg)->type                           = type_in;                                \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_PCI */
#endif /* _RUCI_PCI_COMMON_CMD_H */
