/*
 * Copyright (c) 2022-2025 Rafael Microelectronics Inc. All rights reserved.
 * 
 * SPDX-License-Identifier: LicenseRef-RafaelMicro-Proprietary-1.0
 *
 */

/**
 */
/** @file zigbee_data.c
 *
 * @author Rex
 * @version 0.1
 * @date 2021/12/09
 * @license
 * @description
 */

//=============================================================================
//                Include
//=============================================================================
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "zigbee_platform.h"
#include "zigbee_api.h"
//=============================================================================
//                Private Global Variables
//=============================================================================
/* Basic cluster attributes */
static const uint8_t attr_zcl_version = ZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE;
static const uint8_t attr_app_version = 0;
static const uint8_t attr_stack_version = 0;
static const uint8_t attr_hw_version = 0;
static const char attr_mf_name[] = {11,  'R', 'a', 'f', 'a', 'e',
                                    'l', 'M', 'i', 'c', 'r', 'o'};
static const char attr_model_id[] = {9,   'L', 'i', 'g', 'h',
                                     't', '0', '0', '0', '1'};
static const char attr_date_code[] = {8,   '2', '0', '2', '2',
                                      '0', '1', '2', '1'};
static const char attr_sw_build_id[] = {8,   '2', '0', '2', '2',
                                        '0', '1', '2', '1'};
static const uint8_t attr_power_source =
    ZB_ZCL_BASIC_POWER_SOURCE_DEFAULT_VALUE;

static uint8_t attr_location_id[] = {16,  ' ', ' ', ' ', ' ', ' ',
                                     ' ', ' ', ' ', ' ', ' ', ' ',
                                     ' ', ' ', ' ', ' ', ' '};
static uint8_t attr_ph_env = 0;

static uint8_t attr_device_class =
    ZB_ZCL_BASIC_GENERIC_DEVICE_CLASS_DEFAULT_VALUE;
static uint8_t attr_device_type =
    ZB_ZCL_BASIC_GENERIC_DEVICE_TYPE_DEFAULT_VALUE;
static const char attr_product_code[] = ZB_ZCL_BASIC_PRODUCT_CODE_DEFAULT_VALUE;
static const char attr_product_url[] = ZB_ZCL_BASIC_PRODUCT_URL_DEFAULT_VALUE;

/* OTA Upgrade client cluster attributes */
uint16_t raf_image_type = 321;
/* UpgradeServerID attribute */
uint8_t upgrade_server[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
/* FileOffset attribute */
uint32_t file_offset = ZB_ZCL_OTA_UPGRADE_FILE_OFFSET_DEF_VALUE;
/* CurrentFileVersion attribute (custom data) */
uint32_t file_version = 0x01010101;
/* CurrentZigbeeStackVersion attribute */
uint16_t stack_version = ZB_ZCL_OTA_UPGRADE_FILE_HEADER_STACK_PRO;
/* DownloadedFileVersion attribute */
uint32_t downloaded_file_ver =
    ZB_ZCL_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_DEF_VALUE;
/* DownloadedZigbeeStackVersion attribute */
uint16_t downloaded_stack_ver = ZB_ZCL_OTA_UPGRADE_DOWNLOADED_STACK_DEF_VALUE;
/* ImageUpgradeStatus attribute */
uint8_t image_status = ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_DEF_VALUE;
/* Manufacturer ID attribute (custom data) */
uint16_t manufacturer = 123;
/* Image Type ID attribute (custom data) */
uint16_t image_type = 0xFFFF;
/* MinimumBlockReque attribute */
uint16_t min_block_reque = 0;
/* Image Stamp attribute */
uint16_t image_stamp = ZB_ZCL_OTA_UPGRADE_IMAGE_STAMP_MIN_VALUE;
/* Server short address attribute */
uint16_t server_addr;
/* Server endpoint attribute */
uint8_t server_ep;

/*! Define a default global trust center link key */
uint8_t ZB_STANDARD_TC_KEY[] = {0x5A, 0x69, 0x67, 0x42, 0x65, 0x65, 0x41, 0x6C,
                                0x6C, 0x69, 0x61, 0x6E, 0x63, 0x65, 0x30, 0x39};
uint8_t install_code_key[18] = {0x83, 0xFE, 0xD3, 0x40, 0x7A, 0x93,
                                0x97, 0x23, 0xA5, 0xC6, 0x39, 0xB2,
                                0x69, 0x16, 0xD5, 0x05, 0xC3, 0xB5};
//=============================================================================
//                Global Variables
//=============================================================================

uint16_t g_attr_identify_time = 0;

/* Group cluster attributes data */
uint8_t g_attr_name_support = 0;

/* Scenes cluster attribute data */
uint8_t g_attr_scenes_scene_count =
    ZB_ZCL_SCENES_SCENE_COUNT_DEFAULT_VALUE; /* Number of scenes currently in the device's scene table */
uint8_t g_attr_scenes_current_scene =
    ZB_ZCL_SCENES_CURRENT_SCENE_DEFAULT_VALUE; /* Scene ID of the scene last invoked */
uint8_t g_attr_scenes_scene_valid =
    ZB_ZCL_SCENES_SCENE_VALID_DEFAULT_VALUE; /* Indicates whether the state of the device corresponds to
                                          * that associated with the CurrentScene and CurrentGroup attributes*/
uint8_t g_attr_scenes_name_support = 0; /* Indicates support for scene names */
uint16_t g_attr_scenes_current_group =
    ZB_ZCL_SCENES_CURRENT_GROUP_DEFAULT_VALUE; /* Group ID of the scene last invoked */

/* On/Off cluster attributes data */
uint32_t g_attr_on_off_on_off = ZB_ZCL_ON_OFF_ON_OFF_DEFAULT_VALUE;
uint32_t g_attr_global_scene_ctrl =
    ZB_ZCL_ON_OFF_GLOBAL_SCENE_CONTROL_DEFAULT_VALUE;
uint16_t g_attr_on_off_on_time = ZB_ZCL_ON_OFF_ON_TIME_DEFAULT_VALUE;
uint16_t g_attr_on_off_off_wait_time =
    ZB_ZCL_ON_OFF_OFF_WAIT_TIME_DEFAULT_VALUE;
uint32_t g_attr_on_off_startup_onoff = ZB_ZCL_ON_OFF_ON_OFF_DEFAULT_VALUE;

/* Level cluster attributes data */
uint8_t g_attr_level_current_level = 0x1;
uint16_t g_attr_level_remaining_time = 0;
uint8_t g_attr_options = ZB_ZCL_LEVEL_CONTROL_OPTIONS_DEFAULT_VALUE;
uint8_t g_attr_start_up_current_level = 0x1;

zb_zcl_cvc_alarm_variables_t lev_ctrl_ctx[1];
zb_zcl_reporting_info_t rep_ctx[8];

//=============================================================================
//                Attribute definitions
//=============================================================================
ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT(basic_attr_list, &attr_zcl_version,
                                     &attr_app_version, &attr_stack_version,
                                     &attr_hw_version, &attr_mf_name,
                                     &attr_model_id, &attr_date_code,
                                     &attr_power_source, &attr_location_id,
                                     &attr_ph_env, &attr_sw_build_id,
                                     &attr_device_class, &attr_device_type,
                                     &attr_product_code, &attr_product_url);

ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST(identify_attr_list, &g_attr_identify_time);

ZB_ZCL_DECLARE_GROUPS_ATTRIB_LIST(groups_attr_list, &g_attr_name_support);

ZB_ZCL_DECLARE_SCENES_ATTRIB_LIST(scenes_attr_list, &g_attr_scenes_scene_count,
                                  &g_attr_scenes_current_scene,
                                  &g_attr_scenes_current_group,
                                  &g_attr_scenes_scene_valid,
                                  &g_attr_scenes_name_support);

ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST_EXT(on_off_attr_list, &g_attr_on_off_on_off,
                                      &g_attr_global_scene_ctrl,
                                      &g_attr_on_off_on_time,
                                      &g_attr_on_off_off_wait_time);

ZB_ZCL_DECLARE_LEVEL_CONTROL_ATTRIB_LIST_EXT(level_control_attr_list,
                                             &g_attr_level_current_level,
                                             &g_attr_level_remaining_time,
                                             &g_attr_start_up_current_level,
                                             &g_attr_options);

ZB_ZCL_DECLARE_OTA_UPGRADE_ATTRIB_LIST(
    ota_upgrade_cli_attr_list, &upgrade_server, &file_offset, &file_version,
    &stack_version, &downloaded_file_ver, &downloaded_stack_ver, &image_status,
    &manufacturer, &image_type, &min_block_reque, &image_stamp, &server_addr,
    &server_ep, 0x0101, 64, ZB_ZCL_OTA_UPGRADE_QUERY_TIMER_COUNT_DEF);

zb_zcl_cluster_desc_t g_zigbee_cluster_list[] = {
    ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_BASIC,
                        ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),
                        (basic_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE,
                        ZB_ZCL_MANUF_CODE_INVALID),
    ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_IDENTIFY,
                        ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),
                        (identify_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE,
                        ZB_ZCL_MANUF_CODE_INVALID),
    ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_GROUPS,
                        ZB_ZCL_ARRAY_SIZE(groups_attr_list, zb_zcl_attr_t),
                        (groups_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE,
                        ZB_ZCL_MANUF_CODE_INVALID),
    ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_SCENES,
                        ZB_ZCL_ARRAY_SIZE(scenes_attr_list, zb_zcl_attr_t),
                        (scenes_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE,
                        ZB_ZCL_MANUF_CODE_INVALID),
    ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_ON_OFF,
                        ZB_ZCL_ARRAY_SIZE(on_off_attr_list, zb_zcl_attr_t),
                        (on_off_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE,
                        ZB_ZCL_MANUF_CODE_INVALID),
    ZB_ZCL_CLUSTER_DESC(
        ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
        ZB_ZCL_ARRAY_SIZE(level_control_attr_list, zb_zcl_attr_t),
        (level_control_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE,
        ZB_ZCL_MANUF_CODE_INVALID),
};

zb_zcl_cluster_desc_t g_zigbee_cluster_ota_list[] = {ZB_ZCL_CLUSTER_DESC(
    ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,
    ZB_ZCL_ARRAY_SIZE(ota_upgrade_cli_attr_list, zb_zcl_attr_t),
    (ota_upgrade_cli_attr_list), ZB_ZCL_CLUSTER_CLIENT_ROLE,
    ZB_ZCL_MANUF_CODE_INVALID)};
//=============================================================================
//                Simple desc definitions
//=============================================================================
ZB_DECLARE_SIMPLE_DESC(6, 0);
ZB_DECLARE_SIMPLE_DESC(0, 1);

ZB_AF_SIMPLE_DESC_TYPE(6, 0)
simple_desc_light = {
    LIGHT_EP,   /* Endpoint */
    ZB_AF_HA_PROFILE_ID,            /* Application profile identifier */
    ZB_HA_DIMMABLE_LIGHT_DEVICE_ID, /* Application device identifier */
    1,                              /* Application device version */
    0,                              /* Reserved */
    6,                              /* Application input cluster count */
    0,                              /* Application output cluster count */
    /* Application input and output cluster list */
    {
        ZB_ZCL_CLUSTER_ID_BASIC,
        ZB_ZCL_CLUSTER_ID_IDENTIFY,
        ZB_ZCL_CLUSTER_ID_SCENES,
        ZB_ZCL_CLUSTER_ID_GROUPS,
        ZB_ZCL_CLUSTER_ID_ON_OFF,
        ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
    }};

ZB_AF_SIMPLE_DESC_TYPE(0, 1)
simple_desc_light_ota = {
    10,                             /* Endpoint */
    ZB_AF_HA_PROFILE_ID,            /* Application profile identifier */
    ZB_HA_DIMMABLE_LIGHT_DEVICE_ID, /* Application device identifier */
    1,                              /* Application device version */
    0,                              /* Reserved */
    0,                              /* Application input cluster count */
    1,                              /* Application output cluster count */
    /* Application input and output cluster list */
    {
        ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,
    }};

ZB_AF_DECLARE_ENDPOINT_DESC(
    light_ep, LIGHT_EP, ZB_AF_HA_PROFILE_ID, 0, NULL,
    ZB_ZCL_ARRAY_SIZE(g_zigbee_cluster_list, zb_zcl_cluster_desc_t),
    g_zigbee_cluster_list, (zb_af_simple_desc_1_1_t*)&simple_desc_light, 8,
    rep_ctx, 1, lev_ctrl_ctx);

ZB_AF_DECLARE_ENDPOINT_DESC(light_ota_ep, 10, ZB_AF_HA_PROFILE_ID, 0, NULL,
                            ZB_ZCL_ARRAY_SIZE(g_zigbee_cluster_ota_list,
                                              zb_zcl_cluster_desc_t),
                            g_zigbee_cluster_ota_list,
                            (zb_af_simple_desc_1_1_t*)&simple_desc_light_ota, 0,
                            NULL, 0, NULL);

// zb_af_endpoint_desc_t* light_ep_list[] = {&light_ep, &light_ota_ep};

// zb_af_device_ctx_t simple_desc_light_ctx = {2, light_ep_list};

ZBOSS_DECLARE_DEVICE_CTX_2_EP(simple_desc_light_ctx, light_ep, light_ota_ep);

uint32_t get_current_level(void) {
    return g_attr_level_current_level;
}

uint16_t get_level_remaining_time(void) {
    return g_attr_level_remaining_time;
}
void set_current_level(uint32_t level) {
    g_attr_level_current_level = level;
}

uint32_t get_on_off_status(void) {
    return g_attr_on_off_on_off;
}

void set_on_off_status(uint32_t status) {
    g_attr_on_off_on_off = status;
}

uint32_t get_on_off_on_time(void) {
    return g_attr_on_off_on_time;
}

void set_on_off_on_time(uint16_t time) {
    g_attr_on_off_on_time = time;
}

uint32_t get_on_off_off_wait_time(void) {
    return g_attr_on_off_off_wait_time;
}

void set_on_off_off_wait_time(uint16_t time) {
    g_attr_on_off_off_wait_time = time;
}

uint8_t get_level_option(void) { return g_attr_options; }

uint32_t get_startup_onoff(void) {
    return g_attr_on_off_startup_onoff;
}

void set_startup_onoff(uint32_t status) {
    g_attr_on_off_startup_onoff = status;
}

uint32_t get_start_up_current_level(void) {
    return g_attr_start_up_current_level;
}

void set_start_up_current_level(uint32_t status) {
    g_attr_start_up_current_level = status;
}

uint32_t get_identify_time(void) {
    return g_attr_identify_time;
}

void reset_attr(void) {
    g_attr_identify_time = 0;
    g_attr_name_support = 0;
    g_attr_scenes_scene_count = ZB_ZCL_SCENES_SCENE_COUNT_DEFAULT_VALUE;
    g_attr_scenes_current_scene = ZB_ZCL_SCENES_CURRENT_SCENE_DEFAULT_VALUE;
    g_attr_scenes_scene_valid = ZB_ZCL_SCENES_SCENE_VALID_DEFAULT_VALUE;
    g_attr_scenes_name_support = 0;
    g_attr_scenes_current_group = ZB_ZCL_SCENES_CURRENT_GROUP_DEFAULT_VALUE;
    g_attr_on_off_on_off = ZB_ZCL_ON_OFF_ON_OFF_DEFAULT_VALUE;
    g_attr_level_current_level = 0;
    g_attr_level_remaining_time = 0;
}

void set_scene_count(uint8_t scene_count) {
    g_attr_scenes_scene_count = scene_count;
}

void z_get_upgrade_server_addr(uint8_t* in) {
    memcpy(in, upgrade_server, 8);
}

uint32_t z_get_file_version(void) { return file_version; }

void z_set_file_version(uint32_t ver) { file_version = ver; }

void z_set_OTA_status(uint8_t ver) { image_status = ver; }

void z_set_OTA_downloaded_file_version(uint32_t ver) {
    downloaded_file_ver = ver;
}

void z_set_OTA_downloaded_file_offset(uint32_t ver) {
    file_offset = ver;
}
