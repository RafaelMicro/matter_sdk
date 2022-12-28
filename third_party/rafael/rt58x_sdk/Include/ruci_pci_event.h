/******************************************************************************
*
* @File         ruci_pci_event.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_PCI_EVENT_H
#define _RUCI_PCI_EVENT_H

#include "ruci_head.h"

#if (RUCI_ENABLE_PCI)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_PCI_EVENT_HEADER 0x16

// RUCI: cnf_event -------------------------------------------------------------
#define RUCI_CNF_EVENT                          RUCI_NUM_CNF_EVENT, ruci_elmt_type_cnf_event, ruci_elmt_num_cnf_event
#define RUCI_CODE_CNF_EVENT                     0x01
#define RUCI_LEN_CNF_EVENT                      6
#define RUCI_NUM_CNF_EVENT                      6
#define RUCI_PARA_LEN_CNF_EVENT                 3
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_cnf_event[];
extern const uint8_t ruci_elmt_num_cnf_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_cnf_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         pci_cmd_header;
    uint8_t         pci_cmd_subheader;
    uint8_t         status;
} ruci_para_cnf_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_cnf_event_t) */
#define SET_RUCI_PARA_CNF_EVENT(msg, pci_cmd_header_in, pci_cmd_subheader_in, status_in)        \
        do{                                                                                                            \
        ((ruci_para_cnf_event_t *)msg)->ruci_header.u8                 = RUCI_PCI_EVENT_HEADER;                  \
        ((ruci_para_cnf_event_t *)msg)->sub_header                     = RUCI_CODE_CNF_EVENT;                    \
        ((ruci_para_cnf_event_t *)msg)->length                         = RUCI_PARA_LEN_CNF_EVENT;                \
        ((ruci_para_cnf_event_t *)msg)->pci_cmd_header                 = pci_cmd_header_in;                      \
        ((ruci_para_cnf_event_t *)msg)->pci_cmd_subheader              = pci_cmd_subheader_in;                   \
        ((ruci_para_cnf_event_t *)msg)->status                         = status_in;                              \
        }while(0)

// RUCI: get_reg_event ---------------------------------------------------------
#define RUCI_GET_REG_EVENT                      RUCI_NUM_GET_REG_EVENT, ruci_elmt_type_get_reg_event, ruci_elmt_num_get_reg_event
#define RUCI_CODE_GET_REG_EVENT                 0x02
#define RUCI_LEN_GET_REG_EVENT                  7
#define RUCI_NUM_GET_REG_EVENT                  4
#define RUCI_PARA_LEN_GET_REG_EVENT             4
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_reg_event[];
extern const uint8_t ruci_elmt_num_get_reg_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_reg_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint32_t        value;
} ruci_para_get_reg_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_reg_event_t) */
#define SET_RUCI_PARA_GET_REG_EVENT(msg, value_in)        \
        do{                                                                                                            \
        ((ruci_para_get_reg_event_t *)msg)->ruci_header.u8                 = RUCI_PCI_EVENT_HEADER;                  \
        ((ruci_para_get_reg_event_t *)msg)->sub_header                     = RUCI_CODE_GET_REG_EVENT;                \
        ((ruci_para_get_reg_event_t *)msg)->length                         = RUCI_PARA_LEN_GET_REG_EVENT;            \
        ((ruci_para_get_reg_event_t *)msg)->value                          = value_in;                               \
        }while(0)

// RUCI: get_crc_report_event --------------------------------------------------
#define RUCI_GET_CRC_REPORT_EVENT               RUCI_NUM_GET_CRC_REPORT_EVENT, ruci_elmt_type_get_crc_report_event, ruci_elmt_num_get_crc_report_event
#define RUCI_CODE_GET_CRC_REPORT_EVENT          0x03
#define RUCI_LEN_GET_CRC_REPORT_EVENT           11
#define RUCI_NUM_GET_CRC_REPORT_EVENT           5
#define RUCI_PARA_LEN_GET_CRC_REPORT_EVENT      8
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_crc_report_event[];
extern const uint8_t ruci_elmt_num_get_crc_report_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_crc_report_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint32_t        crc_success_count;
    uint32_t        crc_fail_count;
} ruci_para_get_crc_report_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_crc_report_event_t) */
#define SET_RUCI_PARA_GET_CRC_REPORT_EVENT(msg, crc_success_count_in, crc_fail_count_in)        \
        do{                                                                                                            \
        ((ruci_para_get_crc_report_event_t *)msg)->ruci_header.u8                 = RUCI_PCI_EVENT_HEADER;                  \
        ((ruci_para_get_crc_report_event_t *)msg)->sub_header                     = RUCI_CODE_GET_CRC_REPORT_EVENT;         \
        ((ruci_para_get_crc_report_event_t *)msg)->length                         = RUCI_PARA_LEN_GET_CRC_REPORT_EVENT;     \
        ((ruci_para_get_crc_report_event_t *)msg)->crc_success_count              = crc_success_count_in;                   \
        ((ruci_para_get_crc_report_event_t *)msg)->crc_fail_count                 = crc_fail_count_in;                      \
        }while(0)

// RUCI: dtm_burst_tx_done_event -----------------------------------------------
#define RUCI_DTM_BURST_TX_DONE_EVENT            RUCI_NUM_DTM_BURST_TX_DONE_EVENT, ruci_elmt_type_dtm_burst_tx_done_event, ruci_elmt_num_dtm_burst_tx_done_event
#define RUCI_CODE_DTM_BURST_TX_DONE_EVENT       0x04
#define RUCI_LEN_DTM_BURST_TX_DONE_EVENT        3
#define RUCI_NUM_DTM_BURST_TX_DONE_EVENT        3
#define RUCI_PARA_LEN_DTM_BURST_TX_DONE_EVENT   0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_dtm_burst_tx_done_event[];
extern const uint8_t ruci_elmt_num_dtm_burst_tx_done_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_dtm_burst_tx_done_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_dtm_burst_tx_done_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_dtm_burst_tx_done_event_t) */
#define SET_RUCI_PARA_DTM_BURST_TX_DONE_EVENT(msg)        \
        do{                                                                                                            \
        ((ruci_para_dtm_burst_tx_done_event_t *)msg)->ruci_header.u8                 = RUCI_PCI_EVENT_HEADER;                  \
        ((ruci_para_dtm_burst_tx_done_event_t *)msg)->sub_header                     = RUCI_CODE_DTM_BURST_TX_DONE_EVENT;      \
        ((ruci_para_dtm_burst_tx_done_event_t *)msg)->length                         = RUCI_PARA_LEN_DTM_BURST_TX_DONE_EVENT;  \
        }while(0)

// RUCI: get_rssi_event --------------------------------------------------------
#define RUCI_GET_RSSI_EVENT                     RUCI_NUM_GET_RSSI_EVENT, ruci_elmt_type_get_rssi_event, ruci_elmt_num_get_rssi_event
#define RUCI_CODE_GET_RSSI_EVENT                0x05
#define RUCI_LEN_GET_RSSI_EVENT                 4
#define RUCI_NUM_GET_RSSI_EVENT                 4
#define RUCI_PARA_LEN_GET_RSSI_EVENT            1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_rssi_event[];
extern const uint8_t ruci_elmt_num_get_rssi_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_rssi_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         rssi;
} ruci_para_get_rssi_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_rssi_event_t) */
#define SET_RUCI_PARA_GET_RSSI_EVENT(msg, rssi_in)        \
        do{                                                                                                            \
        ((ruci_para_get_rssi_event_t *)msg)->ruci_header.u8                 = RUCI_PCI_EVENT_HEADER;                  \
        ((ruci_para_get_rssi_event_t *)msg)->sub_header                     = RUCI_CODE_GET_RSSI_EVENT;               \
        ((ruci_para_get_rssi_event_t *)msg)->length                         = RUCI_PARA_LEN_GET_RSSI_EVENT;           \
        ((ruci_para_get_rssi_event_t *)msg)->rssi                           = rssi_in;                                \
        }while(0)

// RUCI: get_phy_status_event --------------------------------------------------
#define RUCI_GET_PHY_STATUS_EVENT               RUCI_NUM_GET_PHY_STATUS_EVENT, ruci_elmt_type_get_phy_status_event, ruci_elmt_num_get_phy_status_event
#define RUCI_CODE_GET_PHY_STATUS_EVENT          0x07
#define RUCI_LEN_GET_PHY_STATUS_EVENT           10
#define RUCI_NUM_GET_PHY_STATUS_EVENT           10
#define RUCI_PARA_LEN_GET_PHY_STATUS_EVENT      7
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_phy_status_event[];
extern const uint8_t ruci_elmt_num_get_phy_status_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_phy_status_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         wb_rssi;
    uint8_t         ib_rssi;
    uint8_t         lna_idx;
    uint8_t         tia_idx;
    uint8_t         vga_idx;
    uint8_t         lqi;
    uint8_t         rssi;
} ruci_para_get_phy_status_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_phy_status_event_t) */
#define SET_RUCI_PARA_GET_PHY_STATUS_EVENT(msg, wb_rssi_in, ib_rssi_in, lna_idx_in, tia_idx_in, vga_idx_in, lqi_in, rssi_in)        \
        do{                                                                                                            \
        ((ruci_para_get_phy_status_event_t *)msg)->ruci_header.u8                 = RUCI_PCI_EVENT_HEADER;                  \
        ((ruci_para_get_phy_status_event_t *)msg)->sub_header                     = RUCI_CODE_GET_PHY_STATUS_EVENT;         \
        ((ruci_para_get_phy_status_event_t *)msg)->length                         = RUCI_PARA_LEN_GET_PHY_STATUS_EVENT;     \
        ((ruci_para_get_phy_status_event_t *)msg)->wb_rssi                        = wb_rssi_in;                             \
        ((ruci_para_get_phy_status_event_t *)msg)->ib_rssi                        = ib_rssi_in;                             \
        ((ruci_para_get_phy_status_event_t *)msg)->lna_idx                        = lna_idx_in;                             \
        ((ruci_para_get_phy_status_event_t *)msg)->tia_idx                        = tia_idx_in;                             \
        ((ruci_para_get_phy_status_event_t *)msg)->vga_idx                        = vga_idx_in;                             \
        ((ruci_para_get_phy_status_event_t *)msg)->lqi                            = lqi_in;                                 \
        ((ruci_para_get_phy_status_event_t *)msg)->rssi                           = rssi_in;                                \
        }while(0)

// RUCI: htrp_conn_timeout_event -----------------------------------------------
#define RUCI_HTRP_CONN_TIMEOUT_EVENT            RUCI_NUM_HTRP_CONN_TIMEOUT_EVENT, ruci_elmt_type_htrp_conn_timeout_event, ruci_elmt_num_htrp_conn_timeout_event
#define RUCI_CODE_HTRP_CONN_TIMEOUT_EVENT       0x08
#define RUCI_LEN_HTRP_CONN_TIMEOUT_EVENT        4
#define RUCI_NUM_HTRP_CONN_TIMEOUT_EVENT        4
#define RUCI_PARA_LEN_HTRP_CONN_TIMEOUT_EVENT   1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_htrp_conn_timeout_event[];
extern const uint8_t ruci_elmt_num_htrp_conn_timeout_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_htrp_conn_timeout_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         status;
} ruci_para_htrp_conn_timeout_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_htrp_conn_timeout_event_t) */
#define SET_RUCI_PARA_HTRP_CONN_TIMEOUT_EVENT(msg, status_in)        \
        do{                                                                                                            \
        ((ruci_para_htrp_conn_timeout_event_t *)msg)->ruci_header.u8                 = RUCI_PCI_EVENT_HEADER;                  \
        ((ruci_para_htrp_conn_timeout_event_t *)msg)->sub_header                     = RUCI_CODE_HTRP_CONN_TIMEOUT_EVENT;      \
        ((ruci_para_htrp_conn_timeout_event_t *)msg)->length                         = RUCI_PARA_LEN_HTRP_CONN_TIMEOUT_EVENT;  \
        ((ruci_para_htrp_conn_timeout_event_t *)msg)->status                         = status_in;                              \
        }while(0)

// RUCI: htrp_per_event --------------------------------------------------------
#define RUCI_HTRP_PER_EVENT                     RUCI_NUM_HTRP_PER_EVENT, ruci_elmt_type_htrp_per_event, ruci_elmt_num_htrp_per_event
#define RUCI_CODE_HTRP_PER_EVENT                0x09
#define RUCI_LEN_HTRP_PER_EVENT                 19
#define RUCI_NUM_HTRP_PER_EVENT                 4
#define RUCI_PARA_LEN_HTRP_PER_EVENT            16
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_htrp_per_event[];
extern const uint8_t ruci_elmt_num_htrp_per_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_htrp_per_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         per[16];
} ruci_para_htrp_per_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_htrp_per_event_t) */
#define SET_RUCI_PARA_HTRP_PER_EVENT(msg, per_0_in, per_1_in, per_2_in, per_3_in, per_4_in, per_5_in, per_6_in, per_7_in, per_8_in, per_9_in, per_10_in, per_11_in, per_12_in, per_13_in, per_14_in, per_15_in)        \
        do{                                                                                                            \
        ((ruci_para_htrp_per_event_t *)msg)->ruci_header.u8                 = RUCI_PCI_EVENT_HEADER;                  \
        ((ruci_para_htrp_per_event_t *)msg)->sub_header                     = RUCI_CODE_HTRP_PER_EVENT;               \
        ((ruci_para_htrp_per_event_t *)msg)->length                         = RUCI_PARA_LEN_HTRP_PER_EVENT;           \
        ((ruci_para_htrp_per_event_t *)msg)->per[0]                         = per_0_in;                               \
        ((ruci_para_htrp_per_event_t *)msg)->per[1]                         = per_1_in;                               \
        ((ruci_para_htrp_per_event_t *)msg)->per[2]                         = per_2_in;                               \
        ((ruci_para_htrp_per_event_t *)msg)->per[3]                         = per_3_in;                               \
        ((ruci_para_htrp_per_event_t *)msg)->per[4]                         = per_4_in;                               \
        ((ruci_para_htrp_per_event_t *)msg)->per[5]                         = per_5_in;                               \
        ((ruci_para_htrp_per_event_t *)msg)->per[6]                         = per_6_in;                               \
        ((ruci_para_htrp_per_event_t *)msg)->per[7]                         = per_7_in;                               \
        ((ruci_para_htrp_per_event_t *)msg)->per[8]                         = per_8_in;                               \
        ((ruci_para_htrp_per_event_t *)msg)->per[9]                         = per_9_in;                               \
        ((ruci_para_htrp_per_event_t *)msg)->per[10]                        = per_10_in;                              \
        ((ruci_para_htrp_per_event_t *)msg)->per[11]                        = per_11_in;                              \
        ((ruci_para_htrp_per_event_t *)msg)->per[12]                        = per_12_in;                              \
        ((ruci_para_htrp_per_event_t *)msg)->per[13]                        = per_13_in;                              \
        ((ruci_para_htrp_per_event_t *)msg)->per[14]                        = per_14_in;                              \
        ((ruci_para_htrp_per_event_t *)msg)->per[15]                        = per_15_in;                              \
        }while(0)

// RUCI: htrp_rssi_event -------------------------------------------------------
#define RUCI_HTRP_RSSI_EVENT                    RUCI_NUM_HTRP_RSSI_EVENT, ruci_elmt_type_htrp_rssi_event, ruci_elmt_num_htrp_rssi_event
#define RUCI_CODE_HTRP_RSSI_EVENT               0x0A
#define RUCI_LEN_HTRP_RSSI_EVENT                4
#define RUCI_NUM_HTRP_RSSI_EVENT                4
#define RUCI_PARA_LEN_HTRP_RSSI_EVENT           1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_htrp_rssi_event[];
extern const uint8_t ruci_elmt_num_htrp_rssi_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_htrp_rssi_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         rssi;
} ruci_para_htrp_rssi_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_htrp_rssi_event_t) */
#define SET_RUCI_PARA_HTRP_RSSI_EVENT(msg, rssi_in)        \
        do{                                                                                                            \
        ((ruci_para_htrp_rssi_event_t *)msg)->ruci_header.u8                 = RUCI_PCI_EVENT_HEADER;                  \
        ((ruci_para_htrp_rssi_event_t *)msg)->sub_header                     = RUCI_CODE_HTRP_RSSI_EVENT;              \
        ((ruci_para_htrp_rssi_event_t *)msg)->length                         = RUCI_PARA_LEN_HTRP_RSSI_EVENT;          \
        ((ruci_para_htrp_rssi_event_t *)msg)->rssi                           = rssi_in;                                \
        }while(0)

// RUCI: htrp_fw_status_event --------------------------------------------------
#define RUCI_HTRP_FW_STATUS_EVENT               RUCI_NUM_HTRP_FW_STATUS_EVENT, ruci_elmt_type_htrp_fw_status_event, ruci_elmt_num_htrp_fw_status_event
#define RUCI_CODE_HTRP_FW_STATUS_EVENT          0x0B
#define RUCI_LEN_HTRP_FW_STATUS_EVENT           44
#define RUCI_NUM_HTRP_FW_STATUS_EVENT           14
#define RUCI_PARA_LEN_HTRP_FW_STATUS_EVENT      41
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_htrp_fw_status_event[];
extern const uint8_t ruci_elmt_num_htrp_fw_status_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_htrp_fw_status_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         mac_state;
    uint8_t         tx_state;
    uint8_t         rx_state;
    uint8_t         rx_conn_state;
    uint8_t         htrp_type;
    uint8_t         slot_cnt;
    uint8_t         ch_cnt;
    uint8_t         block_cnt;
    uint8_t         need_sec;
    uint8_t         nonce[16];
    uint8_t         key[16];
} ruci_para_htrp_fw_status_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_htrp_fw_status_event_t) */
#define SET_RUCI_PARA_HTRP_FW_STATUS_EVENT(msg, mac_state_in, tx_state_in, rx_state_in, rx_conn_state_in, htrp_type_in, slot_cnt_in, ch_cnt_in, block_cnt_in, need_sec_in, nonce_0_in, nonce_1_in, nonce_2_in, nonce_3_in, nonce_4_in, nonce_5_in, nonce_6_in, nonce_7_in, nonce_8_in, nonce_9_in, nonce_10_in, nonce_11_in, nonce_12_in, nonce_13_in, nonce_14_in, nonce_15_in, key_0_in, key_1_in, key_2_in, key_3_in, key_4_in, key_5_in, key_6_in, key_7_in, key_8_in, key_9_in, key_10_in, key_11_in, key_12_in, key_13_in, key_14_in, key_15_in)        \
        do{                                                                                                            \
        ((ruci_para_htrp_fw_status_event_t *)msg)->ruci_header.u8                 = RUCI_PCI_EVENT_HEADER;                  \
        ((ruci_para_htrp_fw_status_event_t *)msg)->sub_header                     = RUCI_CODE_HTRP_FW_STATUS_EVENT;         \
        ((ruci_para_htrp_fw_status_event_t *)msg)->length                         = RUCI_PARA_LEN_HTRP_FW_STATUS_EVENT;     \
        ((ruci_para_htrp_fw_status_event_t *)msg)->mac_state                      = mac_state_in;                           \
        ((ruci_para_htrp_fw_status_event_t *)msg)->tx_state                       = tx_state_in;                            \
        ((ruci_para_htrp_fw_status_event_t *)msg)->rx_state                       = rx_state_in;                            \
        ((ruci_para_htrp_fw_status_event_t *)msg)->rx_conn_state                  = rx_conn_state_in;                       \
        ((ruci_para_htrp_fw_status_event_t *)msg)->htrp_type                      = htrp_type_in;                           \
        ((ruci_para_htrp_fw_status_event_t *)msg)->slot_cnt                       = slot_cnt_in;                            \
        ((ruci_para_htrp_fw_status_event_t *)msg)->ch_cnt                         = ch_cnt_in;                              \
        ((ruci_para_htrp_fw_status_event_t *)msg)->block_cnt                      = block_cnt_in;                           \
        ((ruci_para_htrp_fw_status_event_t *)msg)->need_sec                       = need_sec_in;                            \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[0]                       = nonce_0_in;                             \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[1]                       = nonce_1_in;                             \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[2]                       = nonce_2_in;                             \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[3]                       = nonce_3_in;                             \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[4]                       = nonce_4_in;                             \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[5]                       = nonce_5_in;                             \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[6]                       = nonce_6_in;                             \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[7]                       = nonce_7_in;                             \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[8]                       = nonce_8_in;                             \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[9]                       = nonce_9_in;                             \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[10]                      = nonce_10_in;                            \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[11]                      = nonce_11_in;                            \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[12]                      = nonce_12_in;                            \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[13]                      = nonce_13_in;                            \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[14]                      = nonce_14_in;                            \
        ((ruci_para_htrp_fw_status_event_t *)msg)->nonce[15]                      = nonce_15_in;                            \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[0]                         = key_0_in;                               \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[1]                         = key_1_in;                               \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[2]                         = key_2_in;                               \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[3]                         = key_3_in;                               \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[4]                         = key_4_in;                               \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[5]                         = key_5_in;                               \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[6]                         = key_6_in;                               \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[7]                         = key_7_in;                               \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[8]                         = key_8_in;                               \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[9]                         = key_9_in;                               \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[10]                        = key_10_in;                              \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[11]                        = key_11_in;                              \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[12]                        = key_12_in;                              \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[13]                        = key_13_in;                              \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[14]                        = key_14_in;                              \
        ((ruci_para_htrp_fw_status_event_t *)msg)->key[15]                        = key_15_in;                              \
        }while(0)

// RUCI: htrp_hw_status_event --------------------------------------------------
#define RUCI_HTRP_HW_STATUS_EVENT               RUCI_NUM_HTRP_HW_STATUS_EVENT, ruci_elmt_type_htrp_hw_status_event, ruci_elmt_num_htrp_hw_status_event
#define RUCI_CODE_HTRP_HW_STATUS_EVENT          0x0C
#define RUCI_LEN_HTRP_HW_STATUS_EVENT           9
#define RUCI_NUM_HTRP_HW_STATUS_EVENT           9
#define RUCI_PARA_LEN_HTRP_HW_STATUS_EVENT      6
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_htrp_hw_status_event[];
extern const uint8_t ruci_elmt_num_htrp_hw_status_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_htrp_hw_status_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         tx_queue_status;
    uint8_t         rx_queue_full;
    uint8_t         hw_task_state;
    uint8_t         rx_busy;
    uint8_t         bmu_err;
    uint8_t         mac_rx_info;
} ruci_para_htrp_hw_status_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_htrp_hw_status_event_t) */
#define SET_RUCI_PARA_HTRP_HW_STATUS_EVENT(msg, tx_queue_status_in, rx_queue_full_in, hw_task_state_in, rx_busy_in, bmu_err_in, mac_rx_info_in)        \
        do{                                                                                                            \
        ((ruci_para_htrp_hw_status_event_t *)msg)->ruci_header.u8                 = RUCI_PCI_EVENT_HEADER;                  \
        ((ruci_para_htrp_hw_status_event_t *)msg)->sub_header                     = RUCI_CODE_HTRP_HW_STATUS_EVENT;         \
        ((ruci_para_htrp_hw_status_event_t *)msg)->length                         = RUCI_PARA_LEN_HTRP_HW_STATUS_EVENT;     \
        ((ruci_para_htrp_hw_status_event_t *)msg)->tx_queue_status                = tx_queue_status_in;                     \
        ((ruci_para_htrp_hw_status_event_t *)msg)->rx_queue_full                  = rx_queue_full_in;                       \
        ((ruci_para_htrp_hw_status_event_t *)msg)->hw_task_state                  = hw_task_state_in;                       \
        ((ruci_para_htrp_hw_status_event_t *)msg)->rx_busy                        = rx_busy_in;                             \
        ((ruci_para_htrp_hw_status_event_t *)msg)->bmu_err                        = bmu_err_in;                             \
        ((ruci_para_htrp_hw_status_event_t *)msg)->mac_rx_info                    = mac_rx_info_in;                         \
        }while(0)

// RUCI: get_csl_accuracy_event ------------------------------------------------
#define RUCI_GET_CSL_ACCURACY_EVENT             RUCI_NUM_GET_CSL_ACCURACY_EVENT, ruci_elmt_type_get_csl_accuracy_event, ruci_elmt_num_get_csl_accuracy_event
#define RUCI_CODE_GET_CSL_ACCURACY_EVENT        0x0D
#define RUCI_LEN_GET_CSL_ACCURACY_EVENT         4
#define RUCI_NUM_GET_CSL_ACCURACY_EVENT         4
#define RUCI_PARA_LEN_GET_CSL_ACCURACY_EVENT    1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_csl_accuracy_event[];
extern const uint8_t ruci_elmt_num_get_csl_accuracy_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_csl_accuracy_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         get_csl_accuracy;
} ruci_para_get_csl_accuracy_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_csl_accuracy_event_t) */
#define SET_RUCI_PARA_GET_CSL_ACCURACY_EVENT(msg, get_csl_accuracy_in)        \
        do{                                                                                                            \
        ((ruci_para_get_csl_accuracy_event_t *)msg)->ruci_header.u8                 = RUCI_PCI_EVENT_HEADER;                  \
        ((ruci_para_get_csl_accuracy_event_t *)msg)->sub_header                     = RUCI_CODE_GET_CSL_ACCURACY_EVENT;       \
        ((ruci_para_get_csl_accuracy_event_t *)msg)->length                         = RUCI_PARA_LEN_GET_CSL_ACCURACY_EVENT;   \
        ((ruci_para_get_csl_accuracy_event_t *)msg)->get_csl_accuracy               = get_csl_accuracy_in;                    \
        }while(0)

// RUCI: get_csl_uncertainty_event ---------------------------------------------
#define RUCI_GET_CSL_UNCERTAINTY_EVENT          RUCI_NUM_GET_CSL_UNCERTAINTY_EVENT, ruci_elmt_type_get_csl_uncertainty_event, ruci_elmt_num_get_csl_uncertainty_event
#define RUCI_CODE_GET_CSL_UNCERTAINTY_EVENT     0x0E
#define RUCI_LEN_GET_CSL_UNCERTAINTY_EVENT      4
#define RUCI_NUM_GET_CSL_UNCERTAINTY_EVENT      4
#define RUCI_PARA_LEN_GET_CSL_UNCERTAINTY_EVENT 1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_csl_uncertainty_event[];
extern const uint8_t ruci_elmt_num_get_csl_uncertainty_event[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_csl_uncertainty_event_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         csl_uncertainty;
} ruci_para_get_csl_uncertainty_event_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_csl_uncertainty_event_t) */
#define SET_RUCI_PARA_GET_CSL_UNCERTAINTY_EVENT(msg, csl_uncertainty_in)        \
        do{                                                                                                            \
        ((ruci_para_get_csl_uncertainty_event_t *)msg)->ruci_header.u8                 = RUCI_PCI_EVENT_HEADER;                  \
        ((ruci_para_get_csl_uncertainty_event_t *)msg)->sub_header                     = RUCI_CODE_GET_CSL_UNCERTAINTY_EVENT;    \
        ((ruci_para_get_csl_uncertainty_event_t *)msg)->length                         = RUCI_PARA_LEN_GET_CSL_UNCERTAINTY_EVENT;\
        ((ruci_para_get_csl_uncertainty_event_t *)msg)->csl_uncertainty                = csl_uncertainty_in;                     \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_PCI */
#endif /* _RUCI_PCI_EVENT_H */
