/******************************************************************************
*
* @File         ruci.h
* @Version
* $Revision: 5065
* $Date: 2022-09-08
* @Brief
* @Note
* Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef RUCI_H
#define RUCI_H

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "ruci_head.h"
#include "ruci_pci_common_cmd.h"
#include "ruci_pci_fsk_cmd.h"
#include "ruci_pci_ble_cmd.h"
#include "ruci_pci15p4_mac_cmd.h"
#include "ruci_pci_oqpsk_cmd.h"
#include "ruci_pci_slink_cmd.h"
#include "ruci_pci_event.h"
#include "ruci_pci_data.h"
#include "ruci_cmn_sys_cmd.h"
#include "ruci_cmn_hal_cmd.h"
#include "ruci_cmn_event.h"
#include "ruci_sf_host_cmd.h"
#include "ruci_sf_host_event.h"
#include "ruci_sf_host_sys_cmd.h"
#include "ruci_sf_event.h"

/*****************************************************************************
* DEFINES
******************************************************************************/
#define RUCI_VERSION  (5065)

#if (RUCI_ENDIAN_INVERSE)
#define RUCI_ENDIAN_CONVERT(pData, para)        RUCI_endianConvert(pData, para)
#else
#define RUCI_ENDIAN_CONVERT(pData, para)
#endif


/*****************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************/
#if (RUCI_ENDIAN_INVERSE)
void ruci_endian_convert(
    uint8_t         *p_data_in,
    uint8_t         para_num,
    const uint8_t   *p_type_map_in,
    const uint8_t   *pElemNumMapIn
);
#endif

#endif /* _RUCI_H */
