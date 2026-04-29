/*
 * Copyright (c) 2022-2025 Rafael Microelectronics Inc. All rights reserved.
 * 
 * SPDX-License-Identifier: LicenseRef-RafaelMicro-Proprietary-1.0
 *
 */

/**
 * @file zigbee_zcl_msg_handler.h
 * @brief
 * @version 0.1
 * @date 2023-08-30
 * 
 */

#ifndef __ZIGBEE_ZCL_MSG_HANDLER_H__
#define __ZIGBEE_ZCL_MSG_HANDLER_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "zb_common.h"
#include "zb_mac_globals.h"
#include "zboss_api.h"

zb_uint8_t zigbee_zcl_msg_handler(zb_uint8_t param);

#ifdef __cplusplus
};
#endif
#endif // __ZIGBEE_ZCL_MSG_HANDLER_H__