/******************************************************************************
*
* @File         ruci_pci15p4_mac_cmd.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_PCI15P4_MAC_CMD_H
#define _RUCI_PCI15P4_MAC_CMD_H

#include "ruci_head.h"

#if (RUCI_ENABLE_PCI)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_PCI15P4_MAC_CMD_HEADER 0x13

// RUCI: initiate_zigbee -------------------------------------------------------
#define RUCI_INITIATE_ZIGBEE                    RUCI_NUM_INITIATE_ZIGBEE, ruci_elmt_type_initiate_zigbee, ruci_elmt_num_initiate_zigbee
#define RUCI_CODE_INITIATE_ZIGBEE               0x01
#define RUCI_LEN_INITIATE_ZIGBEE                3
#define RUCI_NUM_INITIATE_ZIGBEE                3
#define RUCI_PARA_LEN_INITIATE_ZIGBEE           0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_initiate_zigbee[];
extern const uint8_t ruci_elmt_num_initiate_zigbee[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_initiate_zigbee_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_initiate_zigbee_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_initiate_zigbee_t) */
#define SET_RUCI_PARA_INITIATE_ZIGBEE(msg)        \
        do{                                                                                                            \
        ((ruci_para_initiate_zigbee_t *)msg)->ruci_header.u8                 = RUCI_PCI15P4_MAC_CMD_HEADER;            \
        ((ruci_para_initiate_zigbee_t *)msg)->sub_header                     = RUCI_CODE_INITIATE_ZIGBEE;              \
        ((ruci_para_initiate_zigbee_t *)msg)->length                         = RUCI_PARA_LEN_INITIATE_ZIGBEE;          \
        }while(0)

// RUCI: set15p4_address_filter ------------------------------------------------
#define RUCI_SET15P4_ADDRESS_FILTER             RUCI_NUM_SET15P4_ADDRESS_FILTER, ruci_elmt_type_set15p4_address_filter, ruci_elmt_num_set15p4_address_filter
#define RUCI_CODE_SET15P4_ADDRESS_FILTER        0x02
#define RUCI_LEN_SET15P4_ADDRESS_FILTER         17
#define RUCI_NUM_SET15P4_ADDRESS_FILTER         8
#define RUCI_PARA_LEN_SET15P4_ADDRESS_FILTER    14
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set15p4_address_filter[];
extern const uint8_t ruci_elmt_num_set15p4_address_filter[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set15p4_address_filter_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         mac_promiscuous_mode;
    uint16_t        short_source_address;
    uint32_t        long_source_address[2];
    uint16_t        pan_id;
    uint8_t         is_coordinator;
} ruci_para_set15p4_address_filter_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set15p4_address_filter_t) */
#define SET_RUCI_PARA_SET15P4_ADDRESS_FILTER(msg, mac_promiscuous_mode_in, short_source_address_in, long_source_address_0_in, long_source_address_1_in, pan_id_in, is_coordinator_in)        \
        do{                                                                                                            \
        ((ruci_para_set15p4_address_filter_t *)msg)->ruci_header.u8                 = RUCI_PCI15P4_MAC_CMD_HEADER;            \
        ((ruci_para_set15p4_address_filter_t *)msg)->sub_header                     = RUCI_CODE_SET15P4_ADDRESS_FILTER;       \
        ((ruci_para_set15p4_address_filter_t *)msg)->length                         = RUCI_PARA_LEN_SET15P4_ADDRESS_FILTER;   \
        ((ruci_para_set15p4_address_filter_t *)msg)->mac_promiscuous_mode           = mac_promiscuous_mode_in;                \
        ((ruci_para_set15p4_address_filter_t *)msg)->short_source_address           = short_source_address_in;                \
        ((ruci_para_set15p4_address_filter_t *)msg)->long_source_address[0]         = long_source_address_0_in;               \
        ((ruci_para_set15p4_address_filter_t *)msg)->long_source_address[1]         = long_source_address_1_in;               \
        ((ruci_para_set15p4_address_filter_t *)msg)->pan_id                         = pan_id_in;                              \
        ((ruci_para_set15p4_address_filter_t *)msg)->is_coordinator                 = is_coordinator_in;                      \
        }while(0)

// RUCI: set15p4_mac_pib -------------------------------------------------------
#define RUCI_SET15P4_MAC_PIB                    RUCI_NUM_SET15P4_MAC_PIB, ruci_elmt_type_set15p4_mac_pib, ruci_elmt_num_set15p4_mac_pib
#define RUCI_CODE_SET15P4_MAC_PIB               0x03
#define RUCI_LEN_SET15P4_MAC_PIB                19
#define RUCI_NUM_SET15P4_MAC_PIB                10
#define RUCI_PARA_LEN_SET15P4_MAC_PIB           16
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set15p4_mac_pib[];
extern const uint8_t ruci_elmt_num_set15p4_mac_pib[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set15p4_mac_pib_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint32_t        a_unit_backoff_period;
    uint32_t        mac_ack_wait_duration;
    uint8_t         mac_max_be;
    uint8_t         mac_max_csma_backoffs;
    uint32_t        mac_max_frame_total_wait_time;
    uint8_t         mac_max_frame_retries;
    uint8_t         mac_min_be;
} ruci_para_set15p4_mac_pib_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set15p4_mac_pib_t) */
#define SET_RUCI_PARA_SET15P4_MAC_PIB(msg, a_unit_backoff_period_in, mac_ack_wait_duration_in, mac_max_be_in, mac_max_csma_backoffs_in, mac_max_frame_total_wait_time_in, mac_max_frame_retries_in, mac_min_be_in)        \
        do{                                                                                                            \
        ((ruci_para_set15p4_mac_pib_t *)msg)->ruci_header.u8                 = RUCI_PCI15P4_MAC_CMD_HEADER;            \
        ((ruci_para_set15p4_mac_pib_t *)msg)->sub_header                     = RUCI_CODE_SET15P4_MAC_PIB;              \
        ((ruci_para_set15p4_mac_pib_t *)msg)->length                         = RUCI_PARA_LEN_SET15P4_MAC_PIB;          \
        ((ruci_para_set15p4_mac_pib_t *)msg)->a_unit_backoff_period          = a_unit_backoff_period_in;               \
        ((ruci_para_set15p4_mac_pib_t *)msg)->mac_ack_wait_duration          = mac_ack_wait_duration_in;               \
        ((ruci_para_set15p4_mac_pib_t *)msg)->mac_max_be                     = mac_max_be_in;                          \
        ((ruci_para_set15p4_mac_pib_t *)msg)->mac_max_csma_backoffs          = mac_max_csma_backoffs_in;               \
        ((ruci_para_set15p4_mac_pib_t *)msg)->mac_max_frame_total_wait_time  = mac_max_frame_total_wait_time_in;       \
        ((ruci_para_set15p4_mac_pib_t *)msg)->mac_max_frame_retries          = mac_max_frame_retries_in;               \
        ((ruci_para_set15p4_mac_pib_t *)msg)->mac_min_be                     = mac_min_be_in;                          \
        }while(0)

// RUCI: set15p4_auto_ack ------------------------------------------------------
#define RUCI_SET15P4_AUTO_ACK                   RUCI_NUM_SET15P4_AUTO_ACK, ruci_elmt_type_set15p4_auto_ack, ruci_elmt_num_set15p4_auto_ack
#define RUCI_CODE_SET15P4_AUTO_ACK              0x04
#define RUCI_LEN_SET15P4_AUTO_ACK               4
#define RUCI_NUM_SET15P4_AUTO_ACK               4
#define RUCI_PARA_LEN_SET15P4_AUTO_ACK          1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set15p4_auto_ack[];
extern const uint8_t ruci_elmt_num_set15p4_auto_ack[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set15p4_auto_ack_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         auto_ack_enable_flag;
} ruci_para_set15p4_auto_ack_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set15p4_auto_ack_t) */
#define SET_RUCI_PARA_SET15P4_AUTO_ACK(msg, auto_ack_enable_flag_in)        \
        do{                                                                                                            \
        ((ruci_para_set15p4_auto_ack_t *)msg)->ruci_header.u8                 = RUCI_PCI15P4_MAC_CMD_HEADER;            \
        ((ruci_para_set15p4_auto_ack_t *)msg)->sub_header                     = RUCI_CODE_SET15P4_AUTO_ACK;             \
        ((ruci_para_set15p4_auto_ack_t *)msg)->length                         = RUCI_PARA_LEN_SET15P4_AUTO_ACK;         \
        ((ruci_para_set15p4_auto_ack_t *)msg)->auto_ack_enable_flag           = auto_ack_enable_flag_in;                \
        }while(0)

// RUCI: set15p4_phy_pib -------------------------------------------------------
#define RUCI_SET15P4_PHY_PIB                    RUCI_NUM_SET15P4_PHY_PIB, ruci_elmt_type_set15p4_phy_pib, ruci_elmt_num_set15p4_phy_pib
#define RUCI_CODE_SET15P4_PHY_PIB               0x05
#define RUCI_LEN_SET15P4_PHY_PIB                9
#define RUCI_NUM_SET15P4_PHY_PIB                7
#define RUCI_PARA_LEN_SET15P4_PHY_PIB           6
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set15p4_phy_pib[];
extern const uint8_t ruci_elmt_num_set15p4_phy_pib[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set15p4_phy_pib_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint16_t        a_turnaround_time;
    uint8_t         phy_cca_mode;
    uint8_t         phy_cca_threshold;
    uint16_t        phy_cca_duration;
} ruci_para_set15p4_phy_pib_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set15p4_phy_pib_t) */
#define SET_RUCI_PARA_SET15P4_PHY_PIB(msg, a_turnaround_time_in, phy_cca_mode_in, phy_cca_threshold_in, phy_cca_duration_in)        \
        do{                                                                                                            \
        ((ruci_para_set15p4_phy_pib_t *)msg)->ruci_header.u8                 = RUCI_PCI15P4_MAC_CMD_HEADER;            \
        ((ruci_para_set15p4_phy_pib_t *)msg)->sub_header                     = RUCI_CODE_SET15P4_PHY_PIB;              \
        ((ruci_para_set15p4_phy_pib_t *)msg)->length                         = RUCI_PARA_LEN_SET15P4_PHY_PIB;          \
        ((ruci_para_set15p4_phy_pib_t *)msg)->a_turnaround_time              = a_turnaround_time_in;                   \
        ((ruci_para_set15p4_phy_pib_t *)msg)->phy_cca_mode                   = phy_cca_mode_in;                        \
        ((ruci_para_set15p4_phy_pib_t *)msg)->phy_cca_threshold              = phy_cca_threshold_in;                   \
        ((ruci_para_set15p4_phy_pib_t *)msg)->phy_cca_duration               = phy_cca_duration_in;                    \
        }while(0)

// RUCI: set15p4_pending_bit ---------------------------------------------------
#define RUCI_SET15P4_PENDING_BIT                RUCI_NUM_SET15P4_PENDING_BIT, ruci_elmt_type_set15p4_pending_bit, ruci_elmt_num_set15p4_pending_bit
#define RUCI_CODE_SET15P4_PENDING_BIT           0x06
#define RUCI_LEN_SET15P4_PENDING_BIT            4
#define RUCI_NUM_SET15P4_PENDING_BIT            4
#define RUCI_PARA_LEN_SET15P4_PENDING_BIT       1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set15p4_pending_bit[];
extern const uint8_t ruci_elmt_num_set15p4_pending_bit[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set15p4_pending_bit_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         frame_pending_bit;
} ruci_para_set15p4_pending_bit_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set15p4_pending_bit_t) */
#define SET_RUCI_PARA_SET15P4_PENDING_BIT(msg, frame_pending_bit_in)        \
        do{                                                                                                            \
        ((ruci_para_set15p4_pending_bit_t *)msg)->ruci_header.u8                 = RUCI_PCI15P4_MAC_CMD_HEADER;            \
        ((ruci_para_set15p4_pending_bit_t *)msg)->sub_header                     = RUCI_CODE_SET15P4_PENDING_BIT;          \
        ((ruci_para_set15p4_pending_bit_t *)msg)->length                         = RUCI_PARA_LEN_SET15P4_PENDING_BIT;      \
        ((ruci_para_set15p4_pending_bit_t *)msg)->frame_pending_bit              = frame_pending_bit_in;                   \
        }while(0)

// RUCI: set_src_match_enable --------------------------------------------------
#define RUCI_SET_SRC_MATCH_ENABLE               RUCI_NUM_SET_SRC_MATCH_ENABLE, ruci_elmt_type_set_src_match_enable, ruci_elmt_num_set_src_match_enable
#define RUCI_CODE_SET_SRC_MATCH_ENABLE          0x07
#define RUCI_LEN_SET_SRC_MATCH_ENABLE           4
#define RUCI_NUM_SET_SRC_MATCH_ENABLE           4
#define RUCI_PARA_LEN_SET_SRC_MATCH_ENABLE      1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_src_match_enable[];
extern const uint8_t ruci_elmt_num_set_src_match_enable[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_src_match_enable_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         enable;
} ruci_para_set_src_match_enable_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_src_match_enable_t) */
#define SET_RUCI_PARA_SET_SRC_MATCH_ENABLE(msg, enable_in)        \
        do{                                                                                                            \
        ((ruci_para_set_src_match_enable_t *)msg)->ruci_header.u8                 = RUCI_PCI15P4_MAC_CMD_HEADER;            \
        ((ruci_para_set_src_match_enable_t *)msg)->sub_header                     = RUCI_CODE_SET_SRC_MATCH_ENABLE;         \
        ((ruci_para_set_src_match_enable_t *)msg)->length                         = RUCI_PARA_LEN_SET_SRC_MATCH_ENABLE;     \
        ((ruci_para_set_src_match_enable_t *)msg)->enable                         = enable_in;                              \
        }while(0)

// RUCI: set_src_match_short_entry_ctrl ----------------------------------------
#define RUCI_SET_SRC_MATCH_SHORT_ENTRY_CTRL     RUCI_NUM_SET_SRC_MATCH_SHORT_ENTRY_CTRL, ruci_elmt_type_set_src_match_short_entry_ctrl, ruci_elmt_num_set_src_match_short_entry_ctrl
#define RUCI_CODE_SET_SRC_MATCH_SHORT_ENTRY_CTRL 0x08
#define RUCI_LEN_SET_SRC_MATCH_SHORT_ENTRY_CTRL 6
#define RUCI_NUM_SET_SRC_MATCH_SHORT_ENTRY_CTRL 5
#define RUCI_PARA_LEN_SET_SRC_MATCH_SHORT_ENTRY_CTRL 3
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_src_match_short_entry_ctrl[];
extern const uint8_t ruci_elmt_num_set_src_match_short_entry_ctrl[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_src_match_short_entry_ctrl_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         control_type;
    uint8_t         short_source_address[2];
} ruci_para_set_src_match_short_entry_ctrl_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_src_match_short_entry_ctrl_t) */
#define SET_RUCI_PARA_SET_SRC_MATCH_SHORT_ENTRY_CTRL(msg, control_type_in, short_source_address_0_in, short_source_address_1_in)        \
        do{                                                                                                            \
        ((ruci_para_set_src_match_short_entry_ctrl_t *)msg)->ruci_header.u8                 = RUCI_PCI15P4_MAC_CMD_HEADER;            \
        ((ruci_para_set_src_match_short_entry_ctrl_t *)msg)->sub_header                     = RUCI_CODE_SET_SRC_MATCH_SHORT_ENTRY_CTRL;\
        ((ruci_para_set_src_match_short_entry_ctrl_t *)msg)->length                         = RUCI_PARA_LEN_SET_SRC_MATCH_SHORT_ENTRY_CTRL;\
        ((ruci_para_set_src_match_short_entry_ctrl_t *)msg)->control_type                   = control_type_in;                        \
        ((ruci_para_set_src_match_short_entry_ctrl_t *)msg)->short_source_address[0]        = short_source_address_0_in;              \
        ((ruci_para_set_src_match_short_entry_ctrl_t *)msg)->short_source_address[1]        = short_source_address_1_in;              \
        }while(0)

// RUCI: set_src_match_extended_entry_ctrl -------------------------------------
#define RUCI_SET_SRC_MATCH_EXTENDED_ENTRY_CTRL  RUCI_NUM_SET_SRC_MATCH_EXTENDED_ENTRY_CTRL, ruci_elmt_type_set_src_match_extended_entry_ctrl, ruci_elmt_num_set_src_match_extended_entry_ctrl
#define RUCI_CODE_SET_SRC_MATCH_EXTENDED_ENTRY_CTRL 0x09
#define RUCI_LEN_SET_SRC_MATCH_EXTENDED_ENTRY_CTRL 12
#define RUCI_NUM_SET_SRC_MATCH_EXTENDED_ENTRY_CTRL 5
#define RUCI_PARA_LEN_SET_SRC_MATCH_EXTENDED_ENTRY_CTRL 9
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_src_match_extended_entry_ctrl[];
extern const uint8_t ruci_elmt_num_set_src_match_extended_entry_ctrl[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_src_match_extended_entry_ctrl_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         control_type;
    uint8_t         extended_source_address[8];
} ruci_para_set_src_match_extended_entry_ctrl_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_src_match_extended_entry_ctrl_t) */
#define SET_RUCI_PARA_SET_SRC_MATCH_EXTENDED_ENTRY_CTRL(msg, control_type_in, extended_source_address_0_in, extended_source_address_1_in, extended_source_address_2_in, extended_source_address_3_in, extended_source_address_4_in, extended_source_address_5_in, extended_source_address_6_in, extended_source_address_7_in)        \
        do{                                                                                                            \
        ((ruci_para_set_src_match_extended_entry_ctrl_t *)msg)->ruci_header.u8                 = RUCI_PCI15P4_MAC_CMD_HEADER;            \
        ((ruci_para_set_src_match_extended_entry_ctrl_t *)msg)->sub_header                     = RUCI_CODE_SET_SRC_MATCH_EXTENDED_ENTRY_CTRL;\
        ((ruci_para_set_src_match_extended_entry_ctrl_t *)msg)->length                         = RUCI_PARA_LEN_SET_SRC_MATCH_EXTENDED_ENTRY_CTRL;\
        ((ruci_para_set_src_match_extended_entry_ctrl_t *)msg)->control_type                   = control_type_in;                        \
        ((ruci_para_set_src_match_extended_entry_ctrl_t *)msg)->extended_source_address[0]     = extended_source_address_0_in;           \
        ((ruci_para_set_src_match_extended_entry_ctrl_t *)msg)->extended_source_address[1]     = extended_source_address_1_in;           \
        ((ruci_para_set_src_match_extended_entry_ctrl_t *)msg)->extended_source_address[2]     = extended_source_address_2_in;           \
        ((ruci_para_set_src_match_extended_entry_ctrl_t *)msg)->extended_source_address[3]     = extended_source_address_3_in;           \
        ((ruci_para_set_src_match_extended_entry_ctrl_t *)msg)->extended_source_address[4]     = extended_source_address_4_in;           \
        ((ruci_para_set_src_match_extended_entry_ctrl_t *)msg)->extended_source_address[5]     = extended_source_address_5_in;           \
        ((ruci_para_set_src_match_extended_entry_ctrl_t *)msg)->extended_source_address[6]     = extended_source_address_6_in;           \
        ((ruci_para_set_src_match_extended_entry_ctrl_t *)msg)->extended_source_address[7]     = extended_source_address_7_in;           \
        }while(0)

// RUCI: set_csl_receiver_ctrl -------------------------------------------------
#define RUCI_SET_CSL_RECEIVER_CTRL              RUCI_NUM_SET_CSL_RECEIVER_CTRL, ruci_elmt_type_set_csl_receiver_ctrl, ruci_elmt_num_set_csl_receiver_ctrl
#define RUCI_CODE_SET_CSL_RECEIVER_CTRL         0x0A
#define RUCI_LEN_SET_CSL_RECEIVER_CTRL          8
#define RUCI_NUM_SET_CSL_RECEIVER_CTRL          5
#define RUCI_PARA_LEN_SET_CSL_RECEIVER_CTRL     5
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_csl_receiver_ctrl[];
extern const uint8_t ruci_elmt_num_set_csl_receiver_ctrl[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_csl_receiver_ctrl_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         csl_receiver_ctrl;
    uint32_t        csl_period;
} ruci_para_set_csl_receiver_ctrl_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_csl_receiver_ctrl_t) */
#define SET_RUCI_PARA_SET_CSL_RECEIVER_CTRL(msg, csl_receiver_ctrl_in, csl_period_in)        \
        do{                                                                                                            \
        ((ruci_para_set_csl_receiver_ctrl_t *)msg)->ruci_header.u8                 = RUCI_PCI15P4_MAC_CMD_HEADER;            \
        ((ruci_para_set_csl_receiver_ctrl_t *)msg)->sub_header                     = RUCI_CODE_SET_CSL_RECEIVER_CTRL;        \
        ((ruci_para_set_csl_receiver_ctrl_t *)msg)->length                         = RUCI_PARA_LEN_SET_CSL_RECEIVER_CTRL;    \
        ((ruci_para_set_csl_receiver_ctrl_t *)msg)->csl_receiver_ctrl              = csl_receiver_ctrl_in;                   \
        ((ruci_para_set_csl_receiver_ctrl_t *)msg)->csl_period                     = csl_period_in;                          \
        }while(0)

// RUCI: get_csl_accuracy ------------------------------------------------------
#define RUCI_GET_CSL_ACCURACY                   RUCI_NUM_GET_CSL_ACCURACY, ruci_elmt_type_get_csl_accuracy, ruci_elmt_num_get_csl_accuracy
#define RUCI_CODE_GET_CSL_ACCURACY              0x0B
#define RUCI_LEN_GET_CSL_ACCURACY               3
#define RUCI_NUM_GET_CSL_ACCURACY               3
#define RUCI_PARA_LEN_GET_CSL_ACCURACY          0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_csl_accuracy[];
extern const uint8_t ruci_elmt_num_get_csl_accuracy[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_csl_accuracy_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_get_csl_accuracy_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_csl_accuracy_t) */
#define SET_RUCI_PARA_GET_CSL_ACCURACY(msg)        \
        do{                                                                                                            \
        ((ruci_para_get_csl_accuracy_t *)msg)->ruci_header.u8                 = RUCI_PCI15P4_MAC_CMD_HEADER;            \
        ((ruci_para_get_csl_accuracy_t *)msg)->sub_header                     = RUCI_CODE_GET_CSL_ACCURACY;             \
        ((ruci_para_get_csl_accuracy_t *)msg)->length                         = RUCI_PARA_LEN_GET_CSL_ACCURACY;         \
        }while(0)

// RUCI: get_csl_uncertainty ---------------------------------------------------
#define RUCI_GET_CSL_UNCERTAINTY                RUCI_NUM_GET_CSL_UNCERTAINTY, ruci_elmt_type_get_csl_uncertainty, ruci_elmt_num_get_csl_uncertainty
#define RUCI_CODE_GET_CSL_UNCERTAINTY           0x0C
#define RUCI_LEN_GET_CSL_UNCERTAINTY            3
#define RUCI_NUM_GET_CSL_UNCERTAINTY            3
#define RUCI_PARA_LEN_GET_CSL_UNCERTAINTY       0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_csl_uncertainty[];
extern const uint8_t ruci_elmt_num_get_csl_uncertainty[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_csl_uncertainty_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_get_csl_uncertainty_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_csl_uncertainty_t) */
#define SET_RUCI_PARA_GET_CSL_UNCERTAINTY(msg)        \
        do{                                                                                                            \
        ((ruci_para_get_csl_uncertainty_t *)msg)->ruci_header.u8                 = RUCI_PCI15P4_MAC_CMD_HEADER;            \
        ((ruci_para_get_csl_uncertainty_t *)msg)->sub_header                     = RUCI_CODE_GET_CSL_UNCERTAINTY;          \
        ((ruci_para_get_csl_uncertainty_t *)msg)->length                         = RUCI_PARA_LEN_GET_CSL_UNCERTAINTY;      \
        }while(0)

// RUCI: update_csl_sample_time ------------------------------------------------
#define RUCI_UPDATE_CSL_SAMPLE_TIME             RUCI_NUM_UPDATE_CSL_SAMPLE_TIME, ruci_elmt_type_update_csl_sample_time, ruci_elmt_num_update_csl_sample_time
#define RUCI_CODE_UPDATE_CSL_SAMPLE_TIME        0x0D
#define RUCI_LEN_UPDATE_CSL_SAMPLE_TIME         7
#define RUCI_NUM_UPDATE_CSL_SAMPLE_TIME         4
#define RUCI_PARA_LEN_UPDATE_CSL_SAMPLE_TIME    4
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_update_csl_sample_time[];
extern const uint8_t ruci_elmt_num_update_csl_sample_time[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_update_csl_sample_time_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint32_t        csl_sample_time;
} ruci_para_update_csl_sample_time_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_update_csl_sample_time_t) */
#define SET_RUCI_PARA_UPDATE_CSL_SAMPLE_TIME(msg, csl_sample_time_in)        \
        do{                                                                                                            \
        ((ruci_para_update_csl_sample_time_t *)msg)->ruci_header.u8                 = RUCI_PCI15P4_MAC_CMD_HEADER;            \
        ((ruci_para_update_csl_sample_time_t *)msg)->sub_header                     = RUCI_CODE_UPDATE_CSL_SAMPLE_TIME;       \
        ((ruci_para_update_csl_sample_time_t *)msg)->length                         = RUCI_PARA_LEN_UPDATE_CSL_SAMPLE_TIME;   \
        ((ruci_para_update_csl_sample_time_t *)msg)->csl_sample_time                = csl_sample_time_in;                     \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_PCI */
#endif /* _RUCI_PCI15P4_MAC_CMD_H */
