/******************************************************************************
*
* @File         ruci_pci_ble_cmd.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_PCI_BLE_CMD_H
#define _RUCI_PCI_BLE_CMD_H

#include "ruci_head.h"

#if (RUCI_ENABLE_PCI)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_PCI_BLE_CMD_HEADER 0x12

// RUCI: initiate_ble ----------------------------------------------------------
#define RUCI_INITIATE_BLE                       RUCI_NUM_INITIATE_BLE, ruci_elmt_type_initiate_ble, ruci_elmt_num_initiate_ble
#define RUCI_CODE_INITIATE_BLE                  0x01
#define RUCI_LEN_INITIATE_BLE                   3
#define RUCI_NUM_INITIATE_BLE                   3
#define RUCI_PARA_LEN_INITIATE_BLE              0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_initiate_ble[];
extern const uint8_t ruci_elmt_num_initiate_ble[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_initiate_ble_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_initiate_ble_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_initiate_ble_t) */
#define SET_RUCI_PARA_INITIATE_BLE(msg)        \
        do{                                                                                                            \
        ((ruci_para_initiate_ble_t *)msg)->ruci_header.u8                 = RUCI_PCI_BLE_CMD_HEADER;                \
        ((ruci_para_initiate_ble_t *)msg)->sub_header                     = RUCI_CODE_INITIATE_BLE;                 \
        ((ruci_para_initiate_ble_t *)msg)->length                         = RUCI_PARA_LEN_INITIATE_BLE;             \
        }while(0)

// RUCI: set_ble_modem ---------------------------------------------------------
#define RUCI_SET_BLE_MODEM                      RUCI_NUM_SET_BLE_MODEM, ruci_elmt_type_set_ble_modem, ruci_elmt_num_set_ble_modem
#define RUCI_CODE_SET_BLE_MODEM                 0x02
#define RUCI_LEN_SET_BLE_MODEM                  5
#define RUCI_NUM_SET_BLE_MODEM                  5
#define RUCI_PARA_LEN_SET_BLE_MODEM             2
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_ble_modem[];
extern const uint8_t ruci_elmt_num_set_ble_modem[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_ble_modem_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         data_rate;
    uint8_t         coded_scheme;
} ruci_para_set_ble_modem_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_ble_modem_t) */
#define SET_RUCI_PARA_SET_BLE_MODEM(msg, data_rate_in, coded_scheme_in)        \
        do{                                                                                                            \
        ((ruci_para_set_ble_modem_t *)msg)->ruci_header.u8                 = RUCI_PCI_BLE_CMD_HEADER;                \
        ((ruci_para_set_ble_modem_t *)msg)->sub_header                     = RUCI_CODE_SET_BLE_MODEM;                \
        ((ruci_para_set_ble_modem_t *)msg)->length                         = RUCI_PARA_LEN_SET_BLE_MODEM;            \
        ((ruci_para_set_ble_modem_t *)msg)->data_rate                      = data_rate_in;                           \
        ((ruci_para_set_ble_modem_t *)msg)->coded_scheme                   = coded_scheme_in;                        \
        }while(0)

// RUCI: set_ble_mac -----------------------------------------------------------
#define RUCI_SET_BLE_MAC                        RUCI_NUM_SET_BLE_MAC, ruci_elmt_type_set_ble_mac, ruci_elmt_num_set_ble_mac
#define RUCI_CODE_SET_BLE_MAC                   0x03
#define RUCI_LEN_SET_BLE_MAC                    13
#define RUCI_NUM_SET_BLE_MAC                    7
#define RUCI_PARA_LEN_SET_BLE_MAC               10
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_ble_mac[];
extern const uint8_t ruci_elmt_num_set_ble_mac[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_ble_mac_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint32_t        sfd_content;
    uint8_t         whitening_en;
    uint8_t         whitening_init_value;
    uint32_t        crc_init_value;
} ruci_para_set_ble_mac_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_ble_mac_t) */
#define SET_RUCI_PARA_SET_BLE_MAC(msg, sfd_content_in, whitening_en_in, whitening_init_value_in, crc_init_value_in)        \
        do{                                                                                                            \
        ((ruci_para_set_ble_mac_t *)msg)->ruci_header.u8                 = RUCI_PCI_BLE_CMD_HEADER;                \
        ((ruci_para_set_ble_mac_t *)msg)->sub_header                     = RUCI_CODE_SET_BLE_MAC;                  \
        ((ruci_para_set_ble_mac_t *)msg)->length                         = RUCI_PARA_LEN_SET_BLE_MAC;              \
        ((ruci_para_set_ble_mac_t *)msg)->sfd_content                    = sfd_content_in;                         \
        ((ruci_para_set_ble_mac_t *)msg)->whitening_en                   = whitening_en_in;                        \
        ((ruci_para_set_ble_mac_t *)msg)->whitening_init_value           = whitening_init_value_in;                \
        ((ruci_para_set_ble_mac_t *)msg)->crc_init_value                 = crc_init_value_in;                      \
        }while(0)

// RUCI: set_htrp_para ---------------------------------------------------------
#define RUCI_SET_HTRP_PARA                      RUCI_NUM_SET_HTRP_PARA, ruci_elmt_type_set_htrp_para, ruci_elmt_num_set_htrp_para
#define RUCI_CODE_SET_HTRP_PARA                 0x04
#define RUCI_LEN_SET_HTRP_PARA                  24
#define RUCI_NUM_SET_HTRP_PARA                  14
#define RUCI_PARA_LEN_SET_HTRP_PARA             21
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_htrp_para[];
extern const uint8_t ruci_elmt_num_set_htrp_para[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_htrp_para_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         type;
    uint8_t         block_num;
    uint8_t         min_drift_time;
    uint8_t         max_drift_time;
    uint16_t        drift_low_bond;
    uint16_t        drift_high_bond;
    uint16_t        rx_time;
    uint16_t        ifs;
    uint32_t        p_slot;
    uint32_t        slot;
    uint8_t         conn_timeout_limit;
} ruci_para_set_htrp_para_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_htrp_para_t) */
#define SET_RUCI_PARA_SET_HTRP_PARA(msg, type_in, block_num_in, min_drift_time_in, max_drift_time_in, drift_low_bond_in, drift_high_bond_in, rx_time_in, ifs_in, p_slot_in, slot_in, conn_timeout_limit_in)        \
        do{                                                                                                            \
        ((ruci_para_set_htrp_para_t *)msg)->ruci_header.u8                 = RUCI_PCI_BLE_CMD_HEADER;                \
        ((ruci_para_set_htrp_para_t *)msg)->sub_header                     = RUCI_CODE_SET_HTRP_PARA;                \
        ((ruci_para_set_htrp_para_t *)msg)->length                         = RUCI_PARA_LEN_SET_HTRP_PARA;            \
        ((ruci_para_set_htrp_para_t *)msg)->type                           = type_in;                                \
        ((ruci_para_set_htrp_para_t *)msg)->block_num                      = block_num_in;                           \
        ((ruci_para_set_htrp_para_t *)msg)->min_drift_time                 = min_drift_time_in;                      \
        ((ruci_para_set_htrp_para_t *)msg)->max_drift_time                 = max_drift_time_in;                      \
        ((ruci_para_set_htrp_para_t *)msg)->drift_low_bond                 = drift_low_bond_in;                      \
        ((ruci_para_set_htrp_para_t *)msg)->drift_high_bond                = drift_high_bond_in;                     \
        ((ruci_para_set_htrp_para_t *)msg)->rx_time                        = rx_time_in;                             \
        ((ruci_para_set_htrp_para_t *)msg)->ifs                            = ifs_in;                                 \
        ((ruci_para_set_htrp_para_t *)msg)->p_slot                         = p_slot_in;                              \
        ((ruci_para_set_htrp_para_t *)msg)->slot                           = slot_in;                                \
        ((ruci_para_set_htrp_para_t *)msg)->conn_timeout_limit             = conn_timeout_limit_in;                  \
        }while(0)

// RUCI: set_htrp_map_channel --------------------------------------------------
#define RUCI_SET_HTRP_MAP_CHANNEL               RUCI_NUM_SET_HTRP_MAP_CHANNEL, ruci_elmt_type_set_htrp_map_channel, ruci_elmt_num_set_htrp_map_channel
#define RUCI_CODE_SET_HTRP_MAP_CHANNEL          0x05
#define RUCI_LEN_SET_HTRP_MAP_CHANNEL           45
#define RUCI_NUM_SET_HTRP_MAP_CHANNEL           6
#define RUCI_PARA_LEN_SET_HTRP_MAP_CHANNEL      42
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_htrp_map_channel[];
extern const uint8_t ruci_elmt_num_set_htrp_map_channel[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_htrp_map_channel_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         map_num;
    uint8_t         start_num;
    uint8_t         map[40];
} ruci_para_set_htrp_map_channel_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_htrp_map_channel_t) */
#define SET_RUCI_PARA_SET_HTRP_MAP_CHANNEL(msg, map_num_in, start_num_in, map_0_in, map_1_in, map_2_in, map_3_in, map_4_in, map_5_in, map_6_in, map_7_in, map_8_in, map_9_in, map_10_in, map_11_in, map_12_in, map_13_in, map_14_in, map_15_in, map_16_in, map_17_in, map_18_in, map_19_in, map_20_in, map_21_in, map_22_in, map_23_in, map_24_in, map_25_in, map_26_in, map_27_in, map_28_in, map_29_in, map_30_in, map_31_in, map_32_in, map_33_in, map_34_in, map_35_in, map_36_in, map_37_in, map_38_in, map_39_in)        \
        do{                                                                                                            \
        ((ruci_para_set_htrp_map_channel_t *)msg)->ruci_header.u8                 = RUCI_PCI_BLE_CMD_HEADER;                \
        ((ruci_para_set_htrp_map_channel_t *)msg)->sub_header                     = RUCI_CODE_SET_HTRP_MAP_CHANNEL;         \
        ((ruci_para_set_htrp_map_channel_t *)msg)->length                         = RUCI_PARA_LEN_SET_HTRP_MAP_CHANNEL;     \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map_num                        = map_num_in;                             \
        ((ruci_para_set_htrp_map_channel_t *)msg)->start_num                      = start_num_in;                           \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[0]                         = map_0_in;                               \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[1]                         = map_1_in;                               \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[2]                         = map_2_in;                               \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[3]                         = map_3_in;                               \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[4]                         = map_4_in;                               \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[5]                         = map_5_in;                               \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[6]                         = map_6_in;                               \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[7]                         = map_7_in;                               \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[8]                         = map_8_in;                               \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[9]                         = map_9_in;                               \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[10]                        = map_10_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[11]                        = map_11_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[12]                        = map_12_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[13]                        = map_13_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[14]                        = map_14_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[15]                        = map_15_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[16]                        = map_16_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[17]                        = map_17_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[18]                        = map_18_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[19]                        = map_19_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[20]                        = map_20_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[21]                        = map_21_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[22]                        = map_22_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[23]                        = map_23_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[24]                        = map_24_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[25]                        = map_25_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[26]                        = map_26_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[27]                        = map_27_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[28]                        = map_28_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[29]                        = map_29_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[30]                        = map_30_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[31]                        = map_31_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[32]                        = map_32_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[33]                        = map_33_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[34]                        = map_34_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[35]                        = map_35_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[36]                        = map_36_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[37]                        = map_37_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[38]                        = map_38_in;                              \
        ((ruci_para_set_htrp_map_channel_t *)msg)->map[39]                        = map_39_in;                              \
        }while(0)

// RUCI: set_htrp_sec ----------------------------------------------------------
#define RUCI_SET_HTRP_SEC                       RUCI_NUM_SET_HTRP_SEC, ruci_elmt_type_set_htrp_sec, ruci_elmt_num_set_htrp_sec
#define RUCI_CODE_SET_HTRP_SEC                  0x06
#define RUCI_LEN_SET_HTRP_SEC                   36
#define RUCI_NUM_SET_HTRP_SEC                   6
#define RUCI_PARA_LEN_SET_HTRP_SEC              33
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_htrp_sec[];
extern const uint8_t ruci_elmt_num_set_htrp_sec[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_htrp_sec_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         need_sec;
    uint8_t         nonce[16];
    uint8_t         key[16];
} ruci_para_set_htrp_sec_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_htrp_sec_t) */
#define SET_RUCI_PARA_SET_HTRP_SEC(msg, need_sec_in, nonce_0_in, nonce_1_in, nonce_2_in, nonce_3_in, nonce_4_in, nonce_5_in, nonce_6_in, nonce_7_in, nonce_8_in, nonce_9_in, nonce_10_in, nonce_11_in, nonce_12_in, nonce_13_in, nonce_14_in, nonce_15_in, key_0_in, key_1_in, key_2_in, key_3_in, key_4_in, key_5_in, key_6_in, key_7_in, key_8_in, key_9_in, key_10_in, key_11_in, key_12_in, key_13_in, key_14_in, key_15_in)        \
        do{                                                                                                            \
        ((ruci_para_set_htrp_sec_t *)msg)->ruci_header.u8                 = RUCI_PCI_BLE_CMD_HEADER;                \
        ((ruci_para_set_htrp_sec_t *)msg)->sub_header                     = RUCI_CODE_SET_HTRP_SEC;                 \
        ((ruci_para_set_htrp_sec_t *)msg)->length                         = RUCI_PARA_LEN_SET_HTRP_SEC;             \
        ((ruci_para_set_htrp_sec_t *)msg)->need_sec                       = need_sec_in;                            \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[0]                       = nonce_0_in;                             \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[1]                       = nonce_1_in;                             \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[2]                       = nonce_2_in;                             \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[3]                       = nonce_3_in;                             \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[4]                       = nonce_4_in;                             \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[5]                       = nonce_5_in;                             \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[6]                       = nonce_6_in;                             \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[7]                       = nonce_7_in;                             \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[8]                       = nonce_8_in;                             \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[9]                       = nonce_9_in;                             \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[10]                      = nonce_10_in;                            \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[11]                      = nonce_11_in;                            \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[12]                      = nonce_12_in;                            \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[13]                      = nonce_13_in;                            \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[14]                      = nonce_14_in;                            \
        ((ruci_para_set_htrp_sec_t *)msg)->nonce[15]                      = nonce_15_in;                            \
        ((ruci_para_set_htrp_sec_t *)msg)->key[0]                         = key_0_in;                               \
        ((ruci_para_set_htrp_sec_t *)msg)->key[1]                         = key_1_in;                               \
        ((ruci_para_set_htrp_sec_t *)msg)->key[2]                         = key_2_in;                               \
        ((ruci_para_set_htrp_sec_t *)msg)->key[3]                         = key_3_in;                               \
        ((ruci_para_set_htrp_sec_t *)msg)->key[4]                         = key_4_in;                               \
        ((ruci_para_set_htrp_sec_t *)msg)->key[5]                         = key_5_in;                               \
        ((ruci_para_set_htrp_sec_t *)msg)->key[6]                         = key_6_in;                               \
        ((ruci_para_set_htrp_sec_t *)msg)->key[7]                         = key_7_in;                               \
        ((ruci_para_set_htrp_sec_t *)msg)->key[8]                         = key_8_in;                               \
        ((ruci_para_set_htrp_sec_t *)msg)->key[9]                         = key_9_in;                               \
        ((ruci_para_set_htrp_sec_t *)msg)->key[10]                        = key_10_in;                              \
        ((ruci_para_set_htrp_sec_t *)msg)->key[11]                        = key_11_in;                              \
        ((ruci_para_set_htrp_sec_t *)msg)->key[12]                        = key_12_in;                              \
        ((ruci_para_set_htrp_sec_t *)msg)->key[13]                        = key_13_in;                              \
        ((ruci_para_set_htrp_sec_t *)msg)->key[14]                        = key_14_in;                              \
        ((ruci_para_set_htrp_sec_t *)msg)->key[15]                        = key_15_in;                              \
        }while(0)

// RUCI: set_htrp_tx_enable ----------------------------------------------------
#define RUCI_SET_HTRP_TX_ENABLE                 RUCI_NUM_SET_HTRP_TX_ENABLE, ruci_elmt_type_set_htrp_tx_enable, ruci_elmt_num_set_htrp_tx_enable
#define RUCI_CODE_SET_HTRP_TX_ENABLE            0x07
#define RUCI_LEN_SET_HTRP_TX_ENABLE             3
#define RUCI_NUM_SET_HTRP_TX_ENABLE             3
#define RUCI_PARA_LEN_SET_HTRP_TX_ENABLE        0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_htrp_tx_enable[];
extern const uint8_t ruci_elmt_num_set_htrp_tx_enable[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_htrp_tx_enable_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_set_htrp_tx_enable_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_htrp_tx_enable_t) */
#define SET_RUCI_PARA_SET_HTRP_TX_ENABLE(msg)        \
        do{                                                                                                            \
        ((ruci_para_set_htrp_tx_enable_t *)msg)->ruci_header.u8                 = RUCI_PCI_BLE_CMD_HEADER;                \
        ((ruci_para_set_htrp_tx_enable_t *)msg)->sub_header                     = RUCI_CODE_SET_HTRP_TX_ENABLE;           \
        ((ruci_para_set_htrp_tx_enable_t *)msg)->length                         = RUCI_PARA_LEN_SET_HTRP_TX_ENABLE;       \
        }while(0)

// RUCI: set_htrp_rx_enable ----------------------------------------------------
#define RUCI_SET_HTRP_RX_ENABLE                 RUCI_NUM_SET_HTRP_RX_ENABLE, ruci_elmt_type_set_htrp_rx_enable, ruci_elmt_num_set_htrp_rx_enable
#define RUCI_CODE_SET_HTRP_RX_ENABLE            0x08
#define RUCI_LEN_SET_HTRP_RX_ENABLE             3
#define RUCI_NUM_SET_HTRP_RX_ENABLE             3
#define RUCI_PARA_LEN_SET_HTRP_RX_ENABLE        0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_htrp_rx_enable[];
extern const uint8_t ruci_elmt_num_set_htrp_rx_enable[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_htrp_rx_enable_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_set_htrp_rx_enable_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_htrp_rx_enable_t) */
#define SET_RUCI_PARA_SET_HTRP_RX_ENABLE(msg)        \
        do{                                                                                                            \
        ((ruci_para_set_htrp_rx_enable_t *)msg)->ruci_header.u8                 = RUCI_PCI_BLE_CMD_HEADER;                \
        ((ruci_para_set_htrp_rx_enable_t *)msg)->sub_header                     = RUCI_CODE_SET_HTRP_RX_ENABLE;           \
        ((ruci_para_set_htrp_rx_enable_t *)msg)->length                         = RUCI_PARA_LEN_SET_HTRP_RX_ENABLE;       \
        }while(0)

// RUCI: set_htrp_disable ------------------------------------------------------
#define RUCI_SET_HTRP_DISABLE                   RUCI_NUM_SET_HTRP_DISABLE, ruci_elmt_type_set_htrp_disable, ruci_elmt_num_set_htrp_disable
#define RUCI_CODE_SET_HTRP_DISABLE              0x09
#define RUCI_LEN_SET_HTRP_DISABLE               3
#define RUCI_NUM_SET_HTRP_DISABLE               3
#define RUCI_PARA_LEN_SET_HTRP_DISABLE          0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_htrp_disable[];
extern const uint8_t ruci_elmt_num_set_htrp_disable[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_htrp_disable_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_set_htrp_disable_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_htrp_disable_t) */
#define SET_RUCI_PARA_SET_HTRP_DISABLE(msg)        \
        do{                                                                                                            \
        ((ruci_para_set_htrp_disable_t *)msg)->ruci_header.u8                 = RUCI_PCI_BLE_CMD_HEADER;                \
        ((ruci_para_set_htrp_disable_t *)msg)->sub_header                     = RUCI_CODE_SET_HTRP_DISABLE;             \
        ((ruci_para_set_htrp_disable_t *)msg)->length                         = RUCI_PARA_LEN_SET_HTRP_DISABLE;         \
        }while(0)

// RUCI: initiate_htrp ---------------------------------------------------------
#define RUCI_INITIATE_HTRP                      RUCI_NUM_INITIATE_HTRP, ruci_elmt_type_initiate_htrp, ruci_elmt_num_initiate_htrp
#define RUCI_CODE_INITIATE_HTRP                 0x0A
#define RUCI_LEN_INITIATE_HTRP                  4
#define RUCI_NUM_INITIATE_HTRP                  4
#define RUCI_PARA_LEN_INITIATE_HTRP             1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_initiate_htrp[];
extern const uint8_t ruci_elmt_num_initiate_htrp[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_initiate_htrp_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         data_rate;
} ruci_para_initiate_htrp_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_initiate_htrp_t) */
#define SET_RUCI_PARA_INITIATE_HTRP(msg, data_rate_in)        \
        do{                                                                                                            \
        ((ruci_para_initiate_htrp_t *)msg)->ruci_header.u8                 = RUCI_PCI_BLE_CMD_HEADER;                \
        ((ruci_para_initiate_htrp_t *)msg)->sub_header                     = RUCI_CODE_INITIATE_HTRP;                \
        ((ruci_para_initiate_htrp_t *)msg)->length                         = RUCI_PARA_LEN_INITIATE_HTRP;            \
        ((ruci_para_initiate_htrp_t *)msg)->data_rate                      = data_rate_in;                           \
        }while(0)

// RUCI: get_htrp_status_report ------------------------------------------------
#define RUCI_GET_HTRP_STATUS_REPORT             RUCI_NUM_GET_HTRP_STATUS_REPORT, ruci_elmt_type_get_htrp_status_report, ruci_elmt_num_get_htrp_status_report
#define RUCI_CODE_GET_HTRP_STATUS_REPORT        0x0B
#define RUCI_LEN_GET_HTRP_STATUS_REPORT         4
#define RUCI_NUM_GET_HTRP_STATUS_REPORT         4
#define RUCI_PARA_LEN_GET_HTRP_STATUS_REPORT    1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_htrp_status_report[];
extern const uint8_t ruci_elmt_num_get_htrp_status_report[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_htrp_status_report_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         report_type;
} ruci_para_get_htrp_status_report_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_htrp_status_report_t) */
#define SET_RUCI_PARA_GET_HTRP_STATUS_REPORT(msg, report_type_in)        \
        do{                                                                                                            \
        ((ruci_para_get_htrp_status_report_t *)msg)->ruci_header.u8                 = RUCI_PCI_BLE_CMD_HEADER;                \
        ((ruci_para_get_htrp_status_report_t *)msg)->sub_header                     = RUCI_CODE_GET_HTRP_STATUS_REPORT;       \
        ((ruci_para_get_htrp_status_report_t *)msg)->length                         = RUCI_PARA_LEN_GET_HTRP_STATUS_REPORT;   \
        ((ruci_para_get_htrp_status_report_t *)msg)->report_type                    = report_type_in;                         \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_PCI */
#endif /* _RUCI_PCI_BLE_CMD_H */
