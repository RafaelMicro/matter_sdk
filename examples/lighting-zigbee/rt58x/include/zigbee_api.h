/*
 * Copyright (c) 2022-2025 Rafael Microelectronics Inc. All rights reserved.
 * 
 * SPDX-License-Identifier: LicenseRef-RafaelMicro-Proprietary-1.0
 *
 */

/**
 * @file zigbee_api.h
 * @brief
 *  
 * @version 0.1
 * 
 * @date 
 * 
 */

#ifndef __ZIGBEE_API_H
#define __ZIGBEE_API_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <zigbee_platform.h>
#include "fota_define.h"
#include "hosal_gpio.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "timers.h"
#include "flashctl.h"
#include "sysctrl.h"
#include "EnhancedFlashDataset.h"

#define LIGHT_EP 1
#define SCENE_TABLE_SIZE 16
#define GROUP_TABLE_SIZE 16

#define ZIGBEE_APP_NOTIFY_ISR(ebit)                                            \
    (g_zb_app_evt_var = (zb_app_event_t)(g_zb_app_evt_var | (ebit)));         \
    zb_app_signal()
#define ZIGBEE_APP_NOTIFY(ebit)                                                \
    vPortEnterCritical();                                                      \
    g_zb_app_evt_var = (zb_app_event_t)(g_zb_app_evt_var | (ebit));           \
    vPortExitCritical();                                                       \
    zb_app_signal()
#define ZIGBEE_APP_GET_NOTIFY(ebit)                                            \
    vPortEnterCritical();                                                      \
    ebit = g_zb_app_evt_var;                                                   \
    g_zb_app_evt_var = ZB_APP_EVENT_NONE;                                      \
    vPortExitCritical()

typedef enum {
    ZB_APP_EVENT_NONE = 0,
    ZB_APP_EVENT_INIT = 0x00000001,
    ZB_APP_EVENT_NOT_JOINED = 0x00000002,
    ZB_APP_EVENT_JOINED = 0x00000004,
    ZB_APP_EVENT_FACTORY_RESET = 0x00000008,
    ZB_APP_EVENT_HANDLE_BLE_MESSAGE = 0x00000010,

    ZB_APP_EVENT_ALL = 0xffffffff,
} zb_app_event_t;

typedef struct {
    uint8_t *data;
    uint8_t len;
} ble_msg_t;

extern QueueHandle_t g_ble_msg_queue;

typedef struct SCENE_ENTRY
{
    uint8_t occupied;
    uint8_t scene_id;
    uint8_t onoff_stat;
    uint8_t level;
    uint8_t scene_trans_time_100ms;
    uint16_t group_id;
    uint16_t scene_trans_time;
} scene_entry_t;

typedef struct STARTUP_ENTRY
{
    uint8_t last_onoff_stat;
    uint8_t last_level;
    uint8_t startup_onoff;
    uint8_t start_up_current_level;
} startup_entry_t;
typedef struct DEVICE_DB
{
    scene_entry_t global_scene;
    scene_entry_t scene_table[SCENE_TABLE_SIZE];
    uint16_t group_table[GROUP_TABLE_SIZE];
} scene_db_t;

extern zb_af_device_ctx_t simple_desc_light_ctx;

void zigbee_app_init(void);
void app_main_loop(void* parameters_ptr);
void zb_app_signal(void);
void zigbee_app_nwk_start(uint32_t channel_mask, uint32_t max_child, uint32_t reset);
void zigbee_start_identify(void);
void zigbee_zcl_set_attrubute(uint8_t ep, uint16_t cluster, uint8_t role, uint16_t attr_id, uint8_t* val);

extern zb_app_event_t g_zb_app_evt_var;
extern scene_db_t scene_table_db;
extern startup_entry_t startup_db;
void scene_db_check(void);
void scene_db_update(void);
void startup_db_check(void);
void startup_db_update(void);
void set_startup_status(void);

uint32_t get_current_level(void);
void set_current_level(uint32_t level);
uint16_t get_level_remaining_time(void);
uint32_t get_on_off_status(void);
void set_on_off_status(uint32_t status);
uint32_t get_on_off_on_time(void);
void set_on_off_on_time(uint16_t time);
uint32_t get_on_off_off_wait_time(void);
void set_on_off_off_wait_time(uint16_t time);
uint8_t get_level_option(void);
uint32_t get_startup_onoff(void);
void set_startup_onoff(uint32_t status);
uint32_t get_start_up_current_level(void);
void set_start_up_current_level(uint32_t status);
uint32_t get_identify_time(void);
void reset_attr(void);
void set_scene_count(uint8_t scene_count);
void z_get_upgrade_server_addr(uint8_t* in);
uint32_t z_get_file_version(void);
void z_set_file_version(uint32_t ver);
void z_set_OTA_status(uint8_t ver);
void z_set_OTA_downloaded_file_version(uint32_t ver);
void z_set_OTA_downloaded_file_offset(uint32_t ver);

#ifdef __cplusplus
}
#endif

#endif // __ZIGBEE_API_H
