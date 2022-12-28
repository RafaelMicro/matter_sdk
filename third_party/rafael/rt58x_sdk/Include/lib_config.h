/************************************************************************
 *
 * File Name  : Lib_Config.h
 * Description:
 *
 *
 *
 ************************************************************************
 *
 *      Copyright (c) 2020, All Right Reserved
 *      Rafael Micro. Co. Ltd.
 *      Taiwan, R.O.C.
 *
 ************************************************************************/
#ifndef __LIB_CONFIG_H__
#define __LIB_CONFIG_H__


#define BLE_MODULE_ENABLE(module)     (module > 0)


// <<< Use Configuration Wizard in Context Menu >>>\n

/*
 * =======================|=================================================|
 *                        |   Adv. | Scan  | Initiator |     Connection     |
 *                        |----------------------------|--------------------|
 *                        |        |       |           |  Server |  Client  |
 * =======================|=================================================|
 * ADV_SCAN_LIB           |   V    |   V   |           |         |          |
 * -----------------------|--------|-------|-----------|---------|----------|
 * PERIPHERAL_SERVER_LIB  |   V    |       |           |    V    |          |
 * -----------------------|--------|-------|-----------|---------|----------|
 * CENTRAL_CLIENT_LIB     |        |   V   |     V     |         |     V    |
 * -----------------------|--------|-------|-----------|---------|----------|
 * FULL_FUNC_LIB          |   V    |   V   |     V     |    V    |     V    |
 * =======================|=================================================|
*/
/* Features supported definitions. */
#define ADV_SCAN_LIB                              0x01    // supports adv and scan only, no connection supports
#define PERIPHERAL_SERVER_LIB                     0x02    // supports adv and server role.
#define CENTRAL_CLIENT_LIB                        0x03    // supports scan and client role.
#define FULL_FUNC_LIB                             0x04    // adv, scan, initiator, server and client role.

/* Select the features supported for BLE stack. */
// <o> BLE_FEATURE_CONFIG
// <1=> 0x01 : ADV_SCAN_LIB
// <2=> 0x02 : PERIPHERAL_SERVER_LIB
// <3=> 0x03 : CENTRAL_CLIENT_LIB
// <4=> 0x04 : FULL_FUNC_LIB
#define BLE_FEATURE_CONFIG                        0x04



// <h> BLE Features Configuration
#if (BLE_FEATURE_CONFIG == ADV_SCAN_LIB) // ADV_SCAN_LIB
// <h> BLE_FEATURE_CONFIG = ADV_SCAN_LIB
// <q> _ADV_SUPPORT_
#define _ADV_SUPPORT_                             1
// <q> _SCAN_SUPPORT_
#define _SCAN_SUPPORT_                            1
// <q> _INIT_SUPPORT_
#define _INIT_SUPPORT_                            0
// <q> _CONN_SUPPORT_
#define _CONN_SUPPORT_                            0
// <q> _CONN_PHERIPHERAL_ROLE_
#define _CONN_PHERIPHERAL_ROLE_                   0
// <q> _CONN_CENTRAL_ROLE_
#define _CONN_CENTRAL_ROLE_                       0
// <q> _HOST_SERVER_
#define _HOST_SERVER_                             0
// <q> _HOST_CLIENT_
#define _HOST_CLIENT_                             0
// </h>
#elif (BLE_FEATURE_CONFIG == PERIPHERAL_SERVER_LIB) // PERIPHERAL_SERVER_LIB
// <h> BLE_FEATURE_CONFIG = LL_CUSTOMER_HOST
// <q> _ADV_SUPPORT_
#define _ADV_SUPPORT_                             1
// <q> _SCAN_SUPPORT_
#define _SCAN_SUPPORT_                            0
// <q> _INIT_SUPPORT_
#define _INIT_SUPPORT_                            0
// <q> _CONN_SUPPORT_
#define _CONN_SUPPORT_                            1
// <q> _CONN_PHERIPHERAL_ROLE_
#define _CONN_PHERIPHERAL_ROLE_                   1
// <q> _CONN_CENTRAL_ROLE_
#define _CONN_CENTRAL_ROLE_                       0
// <q> _HOST_SERVER_
#define _HOST_SERVER_                             1
// <q> _HOST_CLIENT_
#define _HOST_CLIENT_                             0
// </h>
#elif (BLE_FEATURE_CONFIG == CENTRAL_CLIENT_LIB) // CENTRAL_CLIENT_LIB
// <h> BLE_FEATURE_CONFIG = CENTRAL_CLIENT_LIB
// <q> _ADV_SUPPORT_
#define _ADV_SUPPORT_                             0
// <q> _SCAN_SUPPORT_
#define _SCAN_SUPPORT_                            1
// <q> _INIT_SUPPORT_
#define _INIT_SUPPORT_                            1
// <q> _CONN_SUPPORT_
#define _CONN_SUPPORT_                            1
// <q> _CONN_PHERIPHERAL_ROLE_
#define _CONN_PHERIPHERAL_ROLE_                   0
// <q> _CONN_CENTRAL_ROLE_
#define _CONN_CENTRAL_ROLE_                       1
// <q> _HOST_SERVER_
#define _HOST_SERVER_                             0
// <q> _HOST_CLIENT_
#define _HOST_CLIENT_                             1
// </h>
#elif (BLE_FEATURE_CONFIG == FULL_FUNC_LIB) // FULL_FUNC_LIB
// <h> BLE_FEATURE_CONFIG = FULL_FUNC_LIB
// <q> _ADV_SUPPORT_
#define _ADV_SUPPORT_                             1
// <q> _SCAN_SUPPORT_
#define _SCAN_SUPPORT_                            1
// <q> _INIT_SUPPORT_
#define _INIT_SUPPORT_                            1
// <q> _CONN_SUPPORT_
#define _CONN_SUPPORT_                            1
// <q> _CONN_PHERIPHERAL_ROLE_
#define _CONN_PHERIPHERAL_ROLE_                   1
// <q> _CONN_CENTRAL_ROLE_
#define _CONN_CENTRAL_ROLE_                       1
// <q> _HOST_SERVER_
#define _HOST_SERVER_                             1
// <q> _HOST_CLIENT_
#define _HOST_CLIENT_                             1
// </h>
#else
#error "Error configuration of BLE_FEATURE_CONFIG."
#endif
// </h>


// <<< end of configuration section >>>

#endif //__LIB_CONFIG_H__
