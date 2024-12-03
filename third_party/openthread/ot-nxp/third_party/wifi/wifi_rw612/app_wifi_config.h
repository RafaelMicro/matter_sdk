/*
 *  Copyright 2023 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

/* Template file comming from */
#include "app_config.h"


/* Re-define particular wifi flags that have wrong value in template file*/
#undef CONFIG_WIFI_MAX_PRIO
#define CONFIG_WIFI_MAX_PRIO 8
#undef CONFIG_IPV6
#define CONFIG_IPV6 1
#undef CONFIG_MAX_IPV6_ADDRESSES
#define CONFIG_MAX_IPV6_ADDRESSES 8

/* flag to undef that are already defined wifi files */
#undef CONFIG_11K
#undef CONFIG_11V
#undef CONFIG_UAP_STA_MAC_ADDR_FILTER
#undef LWIPERF_REVERSE_MODE
#undef CONFIG_MLAN_WMSDK   
#undef CONFIG_11N          
#undef STA_SUPPORT         
#undef UAP_SUPPORT         
#undef WPA                 
#undef KEY_MATERIAL_WEP    
#undef KEY_PARAM_SET_V2    
#undef ENABLE_802_11W      
#undef ENABLE_GCMP_SUPPORT 
#undef CONFIG_STA_AMPDU_RX 
#undef CONFIG_STA_AMPDU_TX 
#undef CONFIG_ENABLE_AMSDU_RX
#undef CONFIG_UAP_AMPDU_TX 
#undef CONFIG_UAP_AMPDU_RX 
#undef CONFIG_WNM_PS       
#undef CONFIG_SCAN_CHANNEL_GAP
#undef CONFIG_COMBO_SCAN   
#undef CONFIG_BG_SCAN      
#undef CONFIG_HOST_MLME    
#undef UAP_HOST_MLME       
#undef CONFIG_WIFI_MAX_CLIENTS_CNT
#undef CONFIG_WIFI_RTS_THRESHOLD
#undef CONFIG_UAP_STA_MAC_ADDR_FILTER
#undef CONFIG_WIFI_FRAG_THRESHOLD
#undef CONFIG_WIFI_FORCE_RTS
#undef CONFIG_TX_AMPDU_PROT_MODE
#undef CONFIG_WIFI_RECOVERY

/* Disable particular feature */
#undef CONFIG_HOST_SLEEP