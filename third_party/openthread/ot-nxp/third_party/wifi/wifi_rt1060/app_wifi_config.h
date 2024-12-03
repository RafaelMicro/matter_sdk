/*
 *  Copyright 2023 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "wifi_bt_module_config.h"
#include "wifi_config.h"


/* Re-define particular wifi flags */
#undef CONFIG_WIFI_MAX_PRIO
#define CONFIG_WIFI_MAX_PRIO 8
#undef CONFIG_IPV6
#define CONFIG_IPV6 1
#undef CONFIG_MAX_IPV6_ADDRESSES
#define CONFIG_MAX_IPV6_ADDRESSES 8

/* other flags */
#define WPL_NO_WLAN_INIT