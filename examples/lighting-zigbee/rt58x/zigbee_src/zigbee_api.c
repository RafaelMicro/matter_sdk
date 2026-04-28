/*
 * Copyright (c) 2022-2025 Rafael Microelectronics Inc. All rights reserved.
 * 
 * SPDX-License-Identifier: LicenseRef-RafaelMicro-Proprietary-1.0
 *
 */

/**
 * @file zigbee_api.c
 * @author 
 * @brief 
 * @version 0.1
 * @date 
 * 
 * 
 */
//=============================================================================
//                Include
//=============================================================================
#include "mcu.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "timers.h"

#include "zb_common.h"
#include "zb_mac_globals.h"
#include "zboss_api.h"

#include "log.h"
#include "zigbee_api.h"
#include "zigbee_platform.h"
#include "zigbee_zcl_msg_handler.h"
#include "hosal_gpio.h"
#include "hosal_sysctrl.h"
#include "init_device_environment.h"

//=============================================================================
//                Global variables
//=============================================================================
#define ZB_TRACE_FILE_ID 294
#ifdef ZB_USE_SLEEP
#define APP_KEEP_ALIVE_TIMEOUT  1000
#endif
zb_app_event_t g_zb_app_evt_var;
QueueHandle_t g_ble_msg_queue;
/*! Active scan duration, valid range 0 ~ 14, (15.36ms * (2^SD +1)) ms in one channel.  */
uint8_t ZB_RAF_SCAN_DURATION = 3;
static TimerHandle_t tmr_identify;

static TaskHandle_t zb_app_taskHandle;

//=============================================================================
//                Function
//=============================================================================
static void zb_app_ota_status_chk(void)
{
    fota_information_t t_bootloader_ota_info = {0};
    memcpy(&t_bootloader_ota_info, (uint8_t *)FOTA_UPDATE_BANK_INFO_ADDRESS, sizeof(t_bootloader_ota_info));

    if (t_bootloader_ota_info.fotabank_ready == FOTA_IMAGE_READY
        && t_bootloader_ota_info.fota_result == FOTA_RESULT_SUCCESS) {
        log_info("OTA success", t_bootloader_ota_info.reserved[0]);
        while (flash_check_busy());
        flash_erase(FLASH_ERASE_SECTOR, FOTA_UPDATE_BANK_INFO_ADDRESS);
    }
    log_info("ver : %08X", z_get_file_version());
}

void scene_db_check(void)
{
    size_t actual_len;
    efd_get_env_blob("scenedb", (void *) &scene_table_db, sizeof(scene_db_t), &actual_len);
    if(actual_len == 0) {
        log_info("scene table not found, create a new one");
        memset(&scene_table_db, 0, sizeof(scene_db_t));
        scene_db_update();
    }
}

void scene_db_update(void)
{
    efd_set_env_blob("scenedb", (void *) &scene_table_db, sizeof(scene_db_t));
}

void startup_db_check(void)
{
    size_t actual_len;
    efd_get_env_blob("startupdb", (void *) &startup_db, sizeof(startup_entry_t), &actual_len);
    if(actual_len == 0) {
        log_info("startup table not found, create a new one");
        memset(&startup_db, 0, sizeof(startup_entry_t));
    }
}

void startup_db_update(void)
{
    efd_set_env_blob("startupdb", (void *) &startup_db, sizeof(startup_entry_t));
}

void set_startup_status(void)
{
    uint8_t current_level, onoff_stat;
    /* set onoff*/
    switch (startup_db.startup_onoff)
    {
    case 0:
        onoff_stat = 0;
        break;
    case 1:
        onoff_stat = 1;
        break;
    case 2:
        onoff_stat = (startup_db.last_onoff_stat == 0) ? 1 : 0;
        break;
    case 0xff:
        onoff_stat = startup_db.last_onoff_stat;
        break;
    default:
        onoff_stat = 0;
        break;
    }
    /* set current level*/
    switch (startup_db.start_up_current_level)
    {
    case 0:
        current_level = 1;
        break;
    case 0xff:
        current_level = startup_db.last_level;
        break;
    default:
        current_level = startup_db.start_up_current_level;
        break;
    }

    set_startup_onoff(startup_db.startup_onoff);
    set_start_up_current_level(startup_db.start_up_current_level);
    set_on_off_status(onoff_stat);
    set_current_level(current_level);

    if (onoff_stat == 1)
    {
        pwm_set_color(current_level,current_level,current_level);
    }
    else
    {
        pwm_set_color(0,0,0);
    }
    log_info("startup status: onoff = %d, level = %d", onoff_stat, current_level);
    startup_db.last_onoff_stat = onoff_stat;
    startup_db.last_level = current_level;
    startup_db_update();
}
void zb_app_signal(void) {
    if (xPortIsInsideInterrupt()) {
        BaseType_t pxHigherPriorityTaskWoken = pdTRUE;
        vTaskNotifyGiveFromISR(zb_app_taskHandle, &pxHigherPriorityTaskWoken);
    } else {
        xTaskNotifyGive(zb_app_taskHandle);
    }
}

void zigbee_app_read_otp_mac_addr(uint8_t* addr) {
    uint8_t temp[0x100];
    flash_read_sec_register((uint32_t)temp, 0x1100);
    memcpy(addr, temp + 8, 8);
}

void zigbee_app_nwk_start(uint32_t channel_mask, uint32_t max_child,
                                 uint32_t reset) {
    uint8_t ieee_addr_invalid[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t ieee_addr[8];
    static uint8_t zb_started = 0;

    if(zb_started)
    {
        log_error("zb already started");
        return;
    }

    zb_started = 1;
    zigbee_app_read_otp_mac_addr(ieee_addr);

    if (memcmp(ieee_addr, ieee_addr_invalid, 8) == 0) {
        flash_get_unique_id((uint32_t)ieee_addr, 8);
    }

    log_info("15p4 MAC Address : %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
             ieee_addr[7], ieee_addr[6], ieee_addr[5], ieee_addr[4], ieee_addr[3],
             ieee_addr[2], ieee_addr[1], ieee_addr[0]);

    ZB_THREAD_SAFE(zb_set_long_address(ieee_addr);

                    zb_set_nvram_erase_at_start(reset);
#ifdef ZB_USE_SLEEP
                    zb_set_network_ed_role(channel_mask);
                    zb_set_ed_timeout(ED_AGING_TIMEOUT_64MIN);
                    zb_set_rx_on_when_idle(0);
                    zb_set_keepalive_timeout(ZB_MILLISECONDS_TO_BEACON_INTERVAL(APP_KEEP_ALIVE_TIMEOUT));
#else
                    zb_set_network_router_role(channel_mask);
                    zb_set_max_children(max_child);
#endif
                    zb_zdo_set_aps_unsecure_join(ZB_TRUE););
                    //zb_bdb_set_legacy_device_support(ZB_TRUE););
    zbStartRun();
}

static void tmr_identify_cb(TimerHandle_t t_timer)
{
    static uint8_t identify_onoff = 0;
    uint16_t remaining_time;

    remaining_time = get_identify_time();
    if(remaining_time != 0) {
        if (identify_onoff == 0)
        {
            pwm_set_color(128,128,128);
            identify_onoff = 1;
        }
        else
        {
            pwm_set_color(0,0,0);
            identify_onoff = 0;
        }
        xTimerStart(tmr_identify, 0);
    }
    else if (remaining_time == 0)
    {
        identify_onoff = 0;
        if (get_on_off_status() == 0)
        {
            pwm_set_color(0,0,0);
        }
        else
        {
            pwm_set_color(get_current_level(),get_current_level(),get_current_level());
        }
        log_info("Identify complete");
    }
}

void zigbee_start_identify(void) {
    if (!tmr_identify) {
        tmr_identify = xTimerCreate("t_id", pdMS_TO_TICKS(200), pdFALSE, (void *)0, tmr_identify_cb);
    }
    if (!xTimerIsTimerActive(tmr_identify)) {
        xTimerStart(tmr_identify, 0);
    }
}

void zigbee_zcl_set_attrubute(uint8_t ep, uint16_t cluster, uint8_t role, uint16_t attr_id, uint8_t* val) {
    ZB_ZCL_SET_ATTRIBUTE(ep, cluster, role, attr_id, val, ZB_FALSE);
}

void zboss_signal_handler(zb_uint8_t param) {
    zb_zdo_app_signal_hdr_t* sg_p = NULL;
    zb_zdo_app_signal_t sig = zb_get_app_signal(param, &sg_p);
    zb_ret_t z_ret = ZB_GET_APP_SIGNAL_STATUS(param);
    zb_zdo_signal_device_annce_params_t* dev_annce_params;

    //log_info(">>zdo_signal_handler: status %d signal %d", z_ret, sig);
    do {
        switch (sig) {
            case ZB_ZDO_SIGNAL_SKIP_STARTUP: zboss_start_continue(); break;

            case ZB_BDB_SIGNAL_DEVICE_FIRST_START:
            case ZB_BDB_SIGNAL_STEERING:
            case ZB_BDB_SIGNAL_DEVICE_REBOOT:
                if (z_ret != 0) {
                    ZIGBEE_APP_NOTIFY(ZB_APP_EVENT_NOT_JOINED);
                } else if (z_ret == 0) {
                    ZIGBEE_APP_NOTIFY(ZB_APP_EVENT_JOINED);
                    ZB_SCHEDULE_APP_ALARM(zb_zcl_ota_upgrade_init_client, param, ZB_MILLISECONDS_TO_BEACON_INTERVAL(2*1000));
                    param = 0;
                }
                break;
            case ZB_COMMON_SIGNAL_CAN_SLEEP: {
                #ifdef ZB_USE_SLEEP
                    zb_sleep_now();
                #endif
            } break;

            case ZB_NLME_STATUS_INDICATION: {

            } break;
            case ZB_ZDO_SIGNAL_PERMIT_JOIN: {
            } break;
            case ZB_ZDO_SIGNAL_LEAVE: {
                /* Notifies the application that the device itself has left the network.*/
                zb_zdo_signal_leave_params_t *leave_params = ZB_ZDO_SIGNAL_GET_PARAMS(sg_p, zb_zdo_signal_leave_params_t);
                if(leave_params->leave_type == ZB_NWK_LEAVE_TYPE_RESET) {
                    log_info("Device do factory reset");
                    efd_env_set_default();
                    flash_erase(FLASH_ERASE_SECTOR, FOTA_UPDATE_BANK_INFO_ADDRESS);
                    while (flash_check_busy());
                    delay_ms(300);
                    sys_software_reset();
                }
            } break;

            default: break;
        }
    } while (0);

    if (param) {
        zb_buf_free(param);
    }
}

void zigbee_app_init(void) {
    BaseType_t xReturned;
    g_ble_msg_queue = xQueueCreate(4, sizeof(ble_msg_t));
    xReturned = xTaskCreate(app_main_loop, "app-zigbee", 512, NULL,
                            E_TASK_PRIORITY_LOWEST, &zb_app_taskHandle);
    if (xReturned != pdPASS) {
        log_error("ZigBee APP task create fail");
    }
}
