/******************************************************************************
*
* @File         ruci_head.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_HEAD_H
#define _RUCI_HEAD_H

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "stdbool.h"

/******************************************************************************
* DEFINES
******************************************************************************/
// RUCI type
#define RUCI_ENABLE_BLE                 (1)
#define RUCI_ENABLE_PCI                 (1)
#define RUCI_ENABLE_CMN                 (1)
#define RUCI_ENABLE_SF                  (1)

// Endian configuration
#define RUCI_ENDIAN_INVERSE             false

// Header configuration
#define RUCI_LEN_HEADER                 sizeof(ruci_head_t)

// Type
#define RUCI_TYPE_BLE                   (0x0)
#define RUCI_TYPE_PCI                   (0x1)
#define RUCI_TYPE_CMN                   (0x3)
#define RUCI_TYPE_SF                    (0xf)

// RUCI type offset
#define RUCI_TYPE_OFFSET                (0x04)

// RUCI OPCODE
#define RUCI_OPCODE_MERGE(subcode, len) (subcode | (len << 8))

// Sub-type
#define RUCI_SUBTYPE_PCI_COMMON_CMD                         (0x0)
#define RUCI_SUBTYPE_PCI_FSK_CMD                            (0x1)
#define RUCI_SUBTYPE_PCI_BLE_CMD                            (0x2)
#define RUCI_SUBTYPE_PCI15P4_MAC_CMD                        (0x3)
#define RUCI_SUBTYPE_PCI_OQPSK_CMD                          (0x4)
#define RUCI_SUBTYPE_PCI_SLINK_CMD                          (0x5)
#define RUCI_SUBTYPE_PCI_EVENT                              (0x6)
#define RUCI_SUBTYPE_PCI_DATA                               (0x7)
#define RUCI_SUBTYPE_CMN_SYS_CMD                            (0x0)
#define RUCI_SUBTYPE_CMN_HAL_CMD                            (0x1)
#define RUCI_SUBTYPE_CMN_EVENT                              (0xf)
#define RUCI_SUBTYPE_SF_HOST_CMD                            (0x0)
#define RUCI_SUBTYPE_SF_HOST_EVENT                          (0x1)
#define RUCI_SUBTYPE_SF_HOST_SYS_CMD                        (0x2)
#define RUCI_SUBTYPE_SF_EVENT                               (0xf)

// RUCI header
typedef union
{
    uint8_t u8;
    struct ruci_head_s
    {
        uint8_t sub_type: 4;
        uint8_t type: 4;
    } bf;
} ruci_head_t, *p_ruci_head_t;
#endif /* _RUCI_HEAD_H */
