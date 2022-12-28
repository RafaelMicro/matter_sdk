/******************************************************************************
*
* @File         ruci_cmn_sys_cmd.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_CMN_SYS_CMD_H
#define _RUCI_CMN_SYS_CMD_H

#include "ruci_head.h"

#if (RUCI_ENABLE_CMN)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_CMN_SYS_CMD_HEADER 0x30

// RUCI: get_fw_ver ------------------------------------------------------------
#define RUCI_GET_FW_VER                         RUCI_NUM_GET_FW_VER, ruci_elmt_type_get_fw_ver, ruci_elmt_num_get_fw_ver
#define RUCI_CODE_GET_FW_VER                    0x01
#define RUCI_LEN_GET_FW_VER                     3
#define RUCI_NUM_GET_FW_VER                     3
#define RUCI_PARA_LEN_GET_FW_VER                0
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_get_fw_ver[];
extern const uint8_t ruci_elmt_num_get_fw_ver[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_get_fw_ver_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
} ruci_para_get_fw_ver_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_get_fw_ver_t) */
#define SET_RUCI_PARA_GET_FW_VER(msg)        \
        do{                                                                                                            \
        ((ruci_para_get_fw_ver_t *)msg)->ruci_header.u8                 = RUCI_CMN_SYS_CMD_HEADER;                \
        ((ruci_para_get_fw_ver_t *)msg)->sub_header                     = RUCI_CODE_GET_FW_VER;                   \
        ((ruci_para_get_fw_ver_t *)msg)->length                         = RUCI_PARA_LEN_GET_FW_VER;               \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_CMN */
#endif /* _RUCI_CMN_SYS_CMD_H */
