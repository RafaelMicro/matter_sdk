/************************************************************************
 *
 * File Name  : ble_profile_app.c
 * Description: This file contains the definitions and functions of BLE profiles for application.
 *
 ************************************************************************/
#include "stdio.h"
#include "ble_profile.h"


/**************************************************************************
 * Profile Application Public Definitions and Variables
 **************************************************************************/

/* link0 information */
ble_info_link0_t ble_info_link0;
ble_info_link1_t ble_info_link1;


/* BLE application links information which is order by host id. */
ble_app_link_info_t ble_app_link_info[BLE_SUPPORT_NUM_CONN_MAX] =
    {
        // Link 0, host id = 0
        {
            .gap_role = BLE_GAP_ROLE_PERIPHERAL,
            .state = 0x00,
            .profile_info = (void *)&ble_info_link0,
        },
        // Link 1, host id = 1
        {
            .gap_role = BLE_GAP_ROLE_PERIPHERAL,
            .state = 0x00,
            .profile_info = (void *)&ble_info_link1,
        },
};

/**************************************************************************
 * Profile Application GENERAL Public Functions
 **************************************************************************/
