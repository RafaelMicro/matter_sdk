/******************************************************************************
*
* @File         ruci_sf_host_sys_cmd.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_SF_HOST_SYS_CMD_H
#define _RUCI_SF_HOST_SYS_CMD_H

#include "ruci_head.h"

#if (RUCI_ENABLE_SF)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_SF_HOST_SYS_CMD_HEADER 0xF2

// RUCI: tx_pwr_comp_cfg -------------------------------------------------------
#define RUCI_TX_PWR_COMP_CFG                    RUCI_NUM_TX_PWR_COMP_CFG, ruci_elmt_type_tx_pwr_comp_cfg, ruci_elmt_num_tx_pwr_comp_cfg
#define RUCI_CODE_TX_PWR_COMP_CFG               0x01
#define RUCI_LEN_TX_PWR_COMP_CFG                5
#define RUCI_NUM_TX_PWR_COMP_CFG                4
#define RUCI_PARA_LEN_TX_PWR_COMP_CFG           1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_tx_pwr_comp_cfg[];
extern const uint8_t ruci_elmt_num_tx_pwr_comp_cfg[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_tx_pwr_comp_cfg_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint16_t        length;
    uint8_t         interval;
} ruci_para_tx_pwr_comp_cfg_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_tx_pwr_comp_cfg_t) */
#define SET_RUCI_PARA_TX_PWR_COMP_CFG(msg, interval_in)        \
        do{                                                                                                            \
        ((ruci_para_tx_pwr_comp_cfg_t *)msg)->ruci_header.u8                 = RUCI_SF_HOST_SYS_CMD_HEADER;            \
        ((ruci_para_tx_pwr_comp_cfg_t *)msg)->sub_header                     = RUCI_CODE_TX_PWR_COMP_CFG;              \
        ((ruci_para_tx_pwr_comp_cfg_t *)msg)->length                         = RUCI_PARA_LEN_TX_PWR_COMP_CFG;          \
        ((ruci_para_tx_pwr_comp_cfg_t *)msg)->interval                       = interval_in;                            \
        }while(0)

// RUCI: set_verify_mode -------------------------------------------------------
#define RUCI_SET_VERIFY_MODE                    RUCI_NUM_SET_VERIFY_MODE, ruci_elmt_type_set_verify_mode, ruci_elmt_num_set_verify_mode
#define RUCI_CODE_SET_VERIFY_MODE               0x02
#define RUCI_LEN_SET_VERIFY_MODE                7
#define RUCI_NUM_SET_VERIFY_MODE                6
#define RUCI_PARA_LEN_SET_VERIFY_MODE           4
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_verify_mode[];
extern const uint8_t ruci_elmt_num_set_verify_mode[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_verify_mode_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         mode;
    uint8_t         tx_enable;
    uint16_t        report_time;
} ruci_para_set_verify_mode_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_verify_mode_t) */
#define SET_RUCI_PARA_SET_VERIFY_MODE(msg, mode_in, tx_enable_in, report_time_in)        \
        do{                                                                                                            \
        ((ruci_para_set_verify_mode_t *)msg)->ruci_header.u8                 = RUCI_SF_HOST_SYS_CMD_HEADER;            \
        ((ruci_para_set_verify_mode_t *)msg)->sub_header                     = RUCI_CODE_SET_VERIFY_MODE;              \
        ((ruci_para_set_verify_mode_t *)msg)->length                         = RUCI_PARA_LEN_SET_VERIFY_MODE;          \
        ((ruci_para_set_verify_mode_t *)msg)->mode                           = mode_in;                                \
        ((ruci_para_set_verify_mode_t *)msg)->tx_enable                      = tx_enable_in;                           \
        ((ruci_para_set_verify_mode_t *)msg)->report_time                    = report_time_in;                         \
        }while(0)

// RUCI: set_verify_ble_para ---------------------------------------------------
#define RUCI_SET_VERIFY_BLE_PARA                RUCI_NUM_SET_VERIFY_BLE_PARA, ruci_elmt_type_set_verify_ble_para, ruci_elmt_num_set_verify_ble_para
#define RUCI_CODE_SET_VERIFY_BLE_PARA           0x03
#define RUCI_LEN_SET_VERIFY_BLE_PARA            26
#define RUCI_NUM_SET_VERIFY_BLE_PARA            13
#define RUCI_PARA_LEN_SET_VERIFY_BLE_PARA       23
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_verify_ble_para[];
extern const uint8_t ruci_elmt_num_set_verify_ble_para[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_verify_ble_para_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint16_t        conn_interval;
    uint16_t        pkt_interval;
    uint16_t        pkt_count;
    uint8_t         phy_select;
    uint8_t         encrypt_enable;
    uint8_t         pktt_length;
    uint8_t         pkt_step_size;
    uint8_t         pkt_type;
    uint8_t         device_addr[6];
    uint8_t         find_addr[6];
} ruci_para_set_verify_ble_para_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_verify_ble_para_t) */
#define SET_RUCI_PARA_SET_VERIFY_BLE_PARA(msg, conn_interval_in, pkt_interval_in, pkt_count_in, phy_select_in, encrypt_enable_in, pktt_length_in, pkt_step_size_in, pkt_type_in, device_addr_0_in, device_addr_1_in, device_addr_2_in, device_addr_3_in, device_addr_4_in, device_addr_5_in, find_addr_0_in, find_addr_1_in, find_addr_2_in, find_addr_3_in, find_addr_4_in, find_addr_5_in)        \
        do{                                                                                                            \
        ((ruci_para_set_verify_ble_para_t *)msg)->ruci_header.u8                 = RUCI_SF_HOST_SYS_CMD_HEADER;            \
        ((ruci_para_set_verify_ble_para_t *)msg)->sub_header                     = RUCI_CODE_SET_VERIFY_BLE_PARA;          \
        ((ruci_para_set_verify_ble_para_t *)msg)->length                         = RUCI_PARA_LEN_SET_VERIFY_BLE_PARA;      \
        ((ruci_para_set_verify_ble_para_t *)msg)->conn_interval                  = conn_interval_in;                       \
        ((ruci_para_set_verify_ble_para_t *)msg)->pkt_interval                   = pkt_interval_in;                        \
        ((ruci_para_set_verify_ble_para_t *)msg)->pkt_count                      = pkt_count_in;                           \
        ((ruci_para_set_verify_ble_para_t *)msg)->phy_select                     = phy_select_in;                          \
        ((ruci_para_set_verify_ble_para_t *)msg)->encrypt_enable                 = encrypt_enable_in;                      \
        ((ruci_para_set_verify_ble_para_t *)msg)->pktt_length                    = pktt_length_in;                         \
        ((ruci_para_set_verify_ble_para_t *)msg)->pkt_step_size                  = pkt_step_size_in;                       \
        ((ruci_para_set_verify_ble_para_t *)msg)->pkt_type                       = pkt_type_in;                            \
        ((ruci_para_set_verify_ble_para_t *)msg)->device_addr[0]                 = device_addr_0_in;                       \
        ((ruci_para_set_verify_ble_para_t *)msg)->device_addr[1]                 = device_addr_1_in;                       \
        ((ruci_para_set_verify_ble_para_t *)msg)->device_addr[2]                 = device_addr_2_in;                       \
        ((ruci_para_set_verify_ble_para_t *)msg)->device_addr[3]                 = device_addr_3_in;                       \
        ((ruci_para_set_verify_ble_para_t *)msg)->device_addr[4]                 = device_addr_4_in;                       \
        ((ruci_para_set_verify_ble_para_t *)msg)->device_addr[5]                 = device_addr_5_in;                       \
        ((ruci_para_set_verify_ble_para_t *)msg)->find_addr[0]                   = find_addr_0_in;                         \
        ((ruci_para_set_verify_ble_para_t *)msg)->find_addr[1]                   = find_addr_1_in;                         \
        ((ruci_para_set_verify_ble_para_t *)msg)->find_addr[2]                   = find_addr_2_in;                         \
        ((ruci_para_set_verify_ble_para_t *)msg)->find_addr[3]                   = find_addr_3_in;                         \
        ((ruci_para_set_verify_ble_para_t *)msg)->find_addr[4]                   = find_addr_4_in;                         \
        ((ruci_para_set_verify_ble_para_t *)msg)->find_addr[5]                   = find_addr_5_in;                         \
        }while(0)

// RUCI: set_verify_pci_para ---------------------------------------------------
#define RUCI_SET_VERIFY_PCI_PARA                RUCI_NUM_SET_VERIFY_PCI_PARA, ruci_elmt_type_set_verify_pci_para, ruci_elmt_num_set_verify_pci_para
#define RUCI_CODE_SET_VERIFY_PCI_PARA           0x04
#define RUCI_LEN_SET_VERIFY_PCI_PARA            34
#define RUCI_NUM_SET_VERIFY_PCI_PARA            15
#define RUCI_PARA_LEN_SET_VERIFY_PCI_PARA       31
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_verify_pci_para[];
extern const uint8_t ruci_elmt_num_set_verify_pci_para[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_verify_pci_para_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint16_t        pkt_interval;
    uint16_t        pktt_length;
    uint16_t        pkt_step_size;
    uint16_t        pkt_count;
    uint8_t         pkt_type;
    uint8_t         phy_type;
    uint8_t         crc_type;
    uint8_t         mac_control;
    uint8_t         addr_type;
    uint16_t        pan_i_d;
    uint8_t         source_addr[8];
    uint8_t         destination_addr[8];
} ruci_para_set_verify_pci_para_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_verify_pci_para_t) */
#define SET_RUCI_PARA_SET_VERIFY_PCI_PARA(msg, pkt_interval_in, pktt_length_in, pkt_step_size_in, pkt_count_in, pkt_type_in, phy_type_in, crc_type_in, mac_control_in, addr_type_in, pan_i_d_in, source_addr_0_in, source_addr_1_in, source_addr_2_in, source_addr_3_in, source_addr_4_in, source_addr_5_in, source_addr_6_in, source_addr_7_in, destination_addr_0_in, destination_addr_1_in, destination_addr_2_in, destination_addr_3_in, destination_addr_4_in, destination_addr_5_in, destination_addr_6_in, destination_addr_7_in)        \
        do{                                                                                                            \
        ((ruci_para_set_verify_pci_para_t *)msg)->ruci_header.u8                 = RUCI_SF_HOST_SYS_CMD_HEADER;            \
        ((ruci_para_set_verify_pci_para_t *)msg)->sub_header                     = RUCI_CODE_SET_VERIFY_PCI_PARA;          \
        ((ruci_para_set_verify_pci_para_t *)msg)->length                         = RUCI_PARA_LEN_SET_VERIFY_PCI_PARA;      \
        ((ruci_para_set_verify_pci_para_t *)msg)->pkt_interval                   = pkt_interval_in;                        \
        ((ruci_para_set_verify_pci_para_t *)msg)->pktt_length                    = pktt_length_in;                         \
        ((ruci_para_set_verify_pci_para_t *)msg)->pkt_step_size                  = pkt_step_size_in;                       \
        ((ruci_para_set_verify_pci_para_t *)msg)->pkt_count                      = pkt_count_in;                           \
        ((ruci_para_set_verify_pci_para_t *)msg)->pkt_type                       = pkt_type_in;                            \
        ((ruci_para_set_verify_pci_para_t *)msg)->phy_type                       = phy_type_in;                            \
        ((ruci_para_set_verify_pci_para_t *)msg)->crc_type                       = crc_type_in;                            \
        ((ruci_para_set_verify_pci_para_t *)msg)->mac_control                    = mac_control_in;                         \
        ((ruci_para_set_verify_pci_para_t *)msg)->addr_type                      = addr_type_in;                           \
        ((ruci_para_set_verify_pci_para_t *)msg)->pan_i_d                        = pan_i_d_in;                             \
        ((ruci_para_set_verify_pci_para_t *)msg)->source_addr[0]                 = source_addr_0_in;                       \
        ((ruci_para_set_verify_pci_para_t *)msg)->source_addr[1]                 = source_addr_1_in;                       \
        ((ruci_para_set_verify_pci_para_t *)msg)->source_addr[2]                 = source_addr_2_in;                       \
        ((ruci_para_set_verify_pci_para_t *)msg)->source_addr[3]                 = source_addr_3_in;                       \
        ((ruci_para_set_verify_pci_para_t *)msg)->source_addr[4]                 = source_addr_4_in;                       \
        ((ruci_para_set_verify_pci_para_t *)msg)->source_addr[5]                 = source_addr_5_in;                       \
        ((ruci_para_set_verify_pci_para_t *)msg)->source_addr[6]                 = source_addr_6_in;                       \
        ((ruci_para_set_verify_pci_para_t *)msg)->source_addr[7]                 = source_addr_7_in;                       \
        ((ruci_para_set_verify_pci_para_t *)msg)->destination_addr[0]            = destination_addr_0_in;                  \
        ((ruci_para_set_verify_pci_para_t *)msg)->destination_addr[1]            = destination_addr_1_in;                  \
        ((ruci_para_set_verify_pci_para_t *)msg)->destination_addr[2]            = destination_addr_2_in;                  \
        ((ruci_para_set_verify_pci_para_t *)msg)->destination_addr[3]            = destination_addr_3_in;                  \
        ((ruci_para_set_verify_pci_para_t *)msg)->destination_addr[4]            = destination_addr_4_in;                  \
        ((ruci_para_set_verify_pci_para_t *)msg)->destination_addr[5]            = destination_addr_5_in;                  \
        ((ruci_para_set_verify_pci_para_t *)msg)->destination_addr[6]            = destination_addr_6_in;                  \
        ((ruci_para_set_verify_pci_para_t *)msg)->destination_addr[7]            = destination_addr_7_in;                  \
        }while(0)

// RUCI: set_verify_test -------------------------------------------------------
#define RUCI_SET_VERIFY_TEST                    RUCI_NUM_SET_VERIFY_TEST, ruci_elmt_type_set_verify_test, ruci_elmt_num_set_verify_test
#define RUCI_CODE_SET_VERIFY_TEST               0x05
#define RUCI_LEN_SET_VERIFY_TEST                4
#define RUCI_NUM_SET_VERIFY_TEST                4
#define RUCI_PARA_LEN_SET_VERIFY_TEST           1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_verify_test[];
extern const uint8_t ruci_elmt_num_set_verify_test[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_verify_test_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         test_enable;
} ruci_para_set_verify_test_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_verify_test_t) */
#define SET_RUCI_PARA_SET_VERIFY_TEST(msg, test_enable_in)        \
        do{                                                                                                            \
        ((ruci_para_set_verify_test_t *)msg)->ruci_header.u8                 = RUCI_SF_HOST_SYS_CMD_HEADER;            \
        ((ruci_para_set_verify_test_t *)msg)->sub_header                     = RUCI_CODE_SET_VERIFY_TEST;              \
        ((ruci_para_set_verify_test_t *)msg)->length                         = RUCI_PARA_LEN_SET_VERIFY_TEST;          \
        ((ruci_para_set_verify_test_t *)msg)->test_enable                    = test_enable_in;                         \
        }while(0)

// RUCI: set_htrp_test_para ----------------------------------------------------
#define RUCI_SET_HTRP_TEST_PARA                 RUCI_NUM_SET_HTRP_TEST_PARA, ruci_elmt_type_set_htrp_test_para, ruci_elmt_num_set_htrp_test_para
#define RUCI_CODE_SET_HTRP_TEST_PARA            0x06
#define RUCI_LEN_SET_HTRP_TEST_PARA             56
#define RUCI_NUM_SET_HTRP_TEST_PARA             16
#define RUCI_PARA_LEN_SET_HTRP_TEST_PARA        53
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_htrp_test_para[];
extern const uint8_t ruci_elmt_num_set_htrp_test_para[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_htrp_test_para_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         htrp_mode;
    uint8_t         htrp_role;
    uint16_t        report_time;
    uint8_t         data_rate;
    uint8_t         block_num;
    uint8_t         encrypt_enable;
    uint32_t        s_f_d;
    uint32_t        slot_time;
    uint32_t        periodic_slot_time;
    uint8_t         ch_num;
    uint8_t         conn_timeout_limit;
    uint8_t         nonce[16];
    uint8_t         key[16];
} ruci_para_set_htrp_test_para_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_htrp_test_para_t) */
#define SET_RUCI_PARA_SET_HTRP_TEST_PARA(msg, htrp_mode_in, htrp_role_in, report_time_in, data_rate_in, block_num_in, encrypt_enable_in, s_f_d_in, slot_time_in, periodic_slot_time_in, ch_num_in, conn_timeout_limit_in, nonce_0_in, nonce_1_in, nonce_2_in, nonce_3_in, nonce_4_in, nonce_5_in, nonce_6_in, nonce_7_in, nonce_8_in, nonce_9_in, nonce_10_in, nonce_11_in, nonce_12_in, nonce_13_in, nonce_14_in, nonce_15_in, key_0_in, key_1_in, key_2_in, key_3_in, key_4_in, key_5_in, key_6_in, key_7_in, key_8_in, key_9_in, key_10_in, key_11_in, key_12_in, key_13_in, key_14_in, key_15_in)        \
        do{                                                                                                            \
        ((ruci_para_set_htrp_test_para_t *)msg)->ruci_header.u8                 = RUCI_SF_HOST_SYS_CMD_HEADER;            \
        ((ruci_para_set_htrp_test_para_t *)msg)->sub_header                     = RUCI_CODE_SET_HTRP_TEST_PARA;           \
        ((ruci_para_set_htrp_test_para_t *)msg)->length                         = RUCI_PARA_LEN_SET_HTRP_TEST_PARA;       \
        ((ruci_para_set_htrp_test_para_t *)msg)->htrp_mode                      = htrp_mode_in;                           \
        ((ruci_para_set_htrp_test_para_t *)msg)->htrp_role                      = htrp_role_in;                           \
        ((ruci_para_set_htrp_test_para_t *)msg)->report_time                    = report_time_in;                         \
        ((ruci_para_set_htrp_test_para_t *)msg)->data_rate                      = data_rate_in;                           \
        ((ruci_para_set_htrp_test_para_t *)msg)->block_num                      = block_num_in;                           \
        ((ruci_para_set_htrp_test_para_t *)msg)->encrypt_enable                 = encrypt_enable_in;                      \
        ((ruci_para_set_htrp_test_para_t *)msg)->s_f_d                          = s_f_d_in;                               \
        ((ruci_para_set_htrp_test_para_t *)msg)->slot_time                      = slot_time_in;                           \
        ((ruci_para_set_htrp_test_para_t *)msg)->periodic_slot_time             = periodic_slot_time_in;                  \
        ((ruci_para_set_htrp_test_para_t *)msg)->ch_num                         = ch_num_in;                              \
        ((ruci_para_set_htrp_test_para_t *)msg)->conn_timeout_limit             = conn_timeout_limit_in;                  \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[0]                       = nonce_0_in;                             \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[1]                       = nonce_1_in;                             \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[2]                       = nonce_2_in;                             \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[3]                       = nonce_3_in;                             \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[4]                       = nonce_4_in;                             \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[5]                       = nonce_5_in;                             \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[6]                       = nonce_6_in;                             \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[7]                       = nonce_7_in;                             \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[8]                       = nonce_8_in;                             \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[9]                       = nonce_9_in;                             \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[10]                      = nonce_10_in;                            \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[11]                      = nonce_11_in;                            \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[12]                      = nonce_12_in;                            \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[13]                      = nonce_13_in;                            \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[14]                      = nonce_14_in;                            \
        ((ruci_para_set_htrp_test_para_t *)msg)->nonce[15]                      = nonce_15_in;                            \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[0]                         = key_0_in;                               \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[1]                         = key_1_in;                               \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[2]                         = key_2_in;                               \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[3]                         = key_3_in;                               \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[4]                         = key_4_in;                               \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[5]                         = key_5_in;                               \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[6]                         = key_6_in;                               \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[7]                         = key_7_in;                               \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[8]                         = key_8_in;                               \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[9]                         = key_9_in;                               \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[10]                        = key_10_in;                              \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[11]                        = key_11_in;                              \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[12]                        = key_12_in;                              \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[13]                        = key_13_in;                              \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[14]                        = key_14_in;                              \
        ((ruci_para_set_htrp_test_para_t *)msg)->key[15]                        = key_15_in;                              \
        }while(0)

// RUCI: set_htrp_test ---------------------------------------------------------
#define RUCI_SET_HTRP_TEST                      RUCI_NUM_SET_HTRP_TEST, ruci_elmt_type_set_htrp_test, ruci_elmt_num_set_htrp_test
#define RUCI_CODE_SET_HTRP_TEST                 0x07
#define RUCI_LEN_SET_HTRP_TEST                  4
#define RUCI_NUM_SET_HTRP_TEST                  4
#define RUCI_PARA_LEN_SET_HTRP_TEST             1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_htrp_test[];
extern const uint8_t ruci_elmt_num_set_htrp_test[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_htrp_test_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         test_enable;
} ruci_para_set_htrp_test_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_htrp_test_t) */
#define SET_RUCI_PARA_SET_HTRP_TEST(msg, test_enable_in)        \
        do{                                                                                                            \
        ((ruci_para_set_htrp_test_t *)msg)->ruci_header.u8                 = RUCI_SF_HOST_SYS_CMD_HEADER;            \
        ((ruci_para_set_htrp_test_t *)msg)->sub_header                     = RUCI_CODE_SET_HTRP_TEST;                \
        ((ruci_para_set_htrp_test_t *)msg)->length                         = RUCI_PARA_LEN_SET_HTRP_TEST;            \
        ((ruci_para_set_htrp_test_t *)msg)->test_enable                    = test_enable_in;                         \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_SF */
#endif /* _RUCI_SF_HOST_SYS_CMD_H */
