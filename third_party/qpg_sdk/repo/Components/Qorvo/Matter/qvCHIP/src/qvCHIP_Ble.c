/*
 * Copyright (c) 2020-2022, Qorvo Inc
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 * CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 * LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * QORVO INC. SHALL NOT, IN ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 * FOR ANY REASON WHATSOEVER.
 *
 */

/** @file "qvCHIP_Ble.c"
 *
 *  CHIP wrapper API for BLE
 *
 *  Implementation of qvCHIP BLE
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_QVCHIP
#define GP_LOCAL_LOG

#include "qvCHIP.h"

#include "gpBsp.h"
#include "gpBleComps.h"
#include "gpLog.h"
#include "hal.h"
#include "hal_BleFreeRTOS.h"
#include "gpSched_runOn.h"
#include "timers.h"

#include "wsf_types.h"
#include "wsf_msg.h"
#include "dm_api.h"
#include "att_api.h"
#include "app_api.h"

#include "bstream.h"
#include "gpHci_types.h"
#include "cordioBleHost.h"
#include "dm_main.h"
#include "dm_conn.h"
#include "svc_ch.h"
#include "svc_core.h"

#include "l2c_defs.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/* <CodeGenerator Placeholder> Macro */
// Check define matches BLE Host stack define
GP_COMPILE_TIME_VERIFY(DM_CONN_MAX == QVCHIP_DM_CONN_MAX);

/** @brief CHIPoBLE service len */
#define CHIPoBLE_SERVICE_LEN 2
/** @brief CHIPoBLE characteristics len */
#define CHIPoBLE_CHARACTERISTIC_LEN 16
/** @brief Offset for UUID data in the characteristic ID */
#define CHIPoBLE_CHARACTERISTIC_UUID_OFFSET 3

/** @brief GAP Advertising Channel Map codes */
#define QVCHIP_ADV_CHAN_37 0x01
#define QVCHIP_ADV_CHAN_38 0x02
#define QVCHIP_ADV_CHAN_39 0x03

/** @brief All advertising channels */
#define QVCHIP_ADV_CHAN_ALL (QVCHIP_ADV_CHAN_37 | QVCHIP_ADV_CHAN_38 | QVCHIP_ADV_CHAN_39)

#define QVCHIP_ADV_DISCO_FLAGS_LEN (0x02)
#define QVCHIP_ADV_DISCO_DATA_LEN  (0x11)

/** @brief Default static header length
 * length of flags 1 byte
 * flags 2 bytes
 * length of data 1 byte
 * type of data 1 byte
 */
#define QVCHIP_ADV_DISCO_PREAMBLE_LEN (1 + QVCHIP_ADV_DISCO_FLAGS_LEN + 1 + 1)

#define QVCHIP_LOCAL_DEVICE_NAME_LEN (HCI_ADV_DATA_LEN - QVCHIP_ADV_DISCO_PREAMBLE_LEN)

/*!< \brief Local storage for device name */
static char qvCHIP_LocalDeviceName[QVCHIP_LOCAL_DEVICE_NAME_LEN];

/*!< \brief Flag to indicate module's inititialization */
static bool qvCHIP_isBleInitiated = false;

/*!< \brief Maximum lengths for characteristics values  - CHIP spec 4.10 */
#define CHIPOBLE_TX_MAX_LEN 255
#define CHIPOBLE_RX_MAX_LEN 255

/*!< \brief Start and end handle values for CHIP over BLE service */
#define CHIPOBLESRV_START_HDL 0x1000
#define CHIPOBLESRV_END_HDL   (HDL_CHIPOBLE_SRV_MAX - 1)

/*!< \brief RunOn API success return value */
#define RUNON_STATUS_OK (0)

/*!< \brief Definition of CHIP over BLE service handle values */
enum {
    CHIPOBLE_SVC_HDL = CHIPOBLESRV_START_HDL, /* CHIPoBLE service declaration */

    CHIPOBLE_RX_CH_HDL,          /* CHIPoBLE RX characteristic */
    CHIPOBLE_RX_HDL,             /* CHIPoBLE RX value */
    CHIPOBLE_RX_HDL_CH_USR_DESC, /* CHIPoBLE RX user description */

    CHIPOBLE_TX_CH_HDL,          /* CHIPoBLE TX characteristic */
    CHIPOBLE_TX_HDL,             /* CHIPoBLE TX value */
    CHIPOBLE_TX_CCC_HDL,         /* CHIPoBLE TX CCC descriptor */
    CHIPOBLE_TX_HDL_CH_USR_DESC, /* CHIPoBLE TX user description */

    HDL_CHIPOBLE_SRV_MAX,

    CHIPOBLE_C3_CH_HDL = HDL_CHIPOBLE_SRV_MAX, /* CHIPoBLE C3 characteristic */
    CHIPOBLE_C3_HDL,                           /* CHIPoBLE C3 value */
};

/*!< \brief Definition of internal state of BLE interface */
typedef enum qvCHIP_Ble_State_ {
    QV_BLE_STATE_NOT_INITIALIZED,
    QV_BLE_STATE_RESET,
    QV_BLE_STATE_ADV_STOPPED,
    QV_BLE_STATE_ADV_STARTED,
} qvCHIP_Ble_State_t;

/*!< \brief Proprietary service UUID for CHIPoBLE */
#define ATT_UUID_CHIPOBLE_SERVICE 0xF6, 0xFF

/*!< \brief Proprietary characteristics UUID for CHIPoBLE */
#define ATT_UUID_CHIPOBLE_RX_CHR                                                                                       \
    0x11, 0x9D, 0x9F, 0x42, 0x9C, 0x4F, 0x9F, 0x95, 0x59, 0x45, 0x3D, 0x26, 0xF5, 0x2E, 0xEE, 0x18

#define ATT_UUID_CHIPOBLE_TX_CHR                                                                                       \
    0x12, 0x9D, 0x9F, 0x42, 0x9C, 0x4F, 0x9F, 0x95, 0x59, 0x45, 0x3D, 0x26, 0xF5, 0x2E, 0xEE, 0x18

#define ATT_UUID_CHIPOBLE_C3_CHR                                                                                       \
    0x04, 0x8F, 0x21, 0x83, 0x8A, 0x74, 0x7D, 0xB8, 0xF2, 0x45, 0x72, 0x87, 0x38, 0x02, 0x63, 0x64

/*! GATT UUIDs */
static const uint8_t _attPrimSvcUuid[ATT_16_UUID_LEN] = {UINT16_TO_BYTES(ATT_UUID_PRIMARY_SERVICE)};
static const uint8_t _attChUuid[ATT_16_UUID_LEN] = {UINT16_TO_BYTES(ATT_UUID_CHARACTERISTIC)};

/*! Descriptor UUIDs */
static const uint8_t _attChUserDescUuid[ATT_16_UUID_LEN] = {UINT16_TO_BYTES(ATT_UUID_CHAR_USER_DESC)};
static const uint8_t _attCliChCfgUuid[ATT_16_UUID_LEN] = {UINT16_TO_BYTES(ATT_UUID_CLIENT_CHAR_CONFIG)};

/*!< \brief CHIPoBLE service declaration */
static uint8_t CHIPoBLEValSvc[CHIPoBLE_SERVICE_LEN] = {ATT_UUID_CHIPOBLE_SERVICE};
static const uint16_t CHIPoBLELenSvc = sizeof(CHIPoBLEValSvc);

/*!< \brief CHIPoBLE Client RX characteristic */
static uint8_t CHIPoBLERXCh[] = {ATT_PROP_WRITE | ATT_PROP_WRITE_NO_RSP, UINT16_TO_BYTES(CHIPOBLE_RX_HDL),
                                 ATT_UUID_CHIPOBLE_RX_CHR};
static const uint16_t CHIPoBLELenRXCh = sizeof(CHIPoBLERXCh);

static uint8_t CHIPoBLERXValue[CHIPOBLE_RX_MAX_LEN] = "";
static const uint16_t CHIPoBLERXValueLen = sizeof(CHIPoBLERXValue);

static const uint8_t CHIPoBLECfgValRXChrUsrDescr[] = "CHIPoBLE Client RX";
static const uint16_t CHIPoBLECfgLenRXChrUsrDescr = sizeof(CHIPoBLECfgValRXChrUsrDescr) - 1u;

/*!< \brief CHIPoBLE Client TX characteristic */
static uint8_t CHIPoBLETXCh[] = {ATT_PROP_INDICATE, UINT16_TO_BYTES(CHIPOBLE_TX_HDL), ATT_UUID_CHIPOBLE_TX_CHR};
static const uint16_t CHIPoBLELenTXCh = sizeof(CHIPoBLETXCh);

static uint8_t CHIPoBLETXValue[CHIPOBLE_TX_MAX_LEN] = "";
static const uint16_t CHIPoBLETXValueLen = sizeof(CHIPoBLETXValue);

static uint8_t CHIPoBLETXChCcc[] = {UINT16_TO_BYTES(0x0000)};
static const uint16_t CHIPoBLELenTXChCcc = sizeof(CHIPoBLETXChCcc);

static const uint8_t CHIPoBLECfgValTXChrUsrDescr[] = "CHIPoBLE Client TX";
static const uint16_t CHIPoBLECfgLenTXChrUsrDescr = sizeof(CHIPoBLECfgValTXChrUsrDescr) - 1u;

/*!< \brief CHIPoBLE Client C3 characteristic */
static uint8_t CHIPoBLEC3Ch[] = {ATT_PROP_READ, UINT16_TO_BYTES(CHIPOBLE_C3_HDL), ATT_UUID_CHIPOBLE_C3_CHR};
static const uint16_t CHIPoBLELenC3Ch = sizeof(CHIPoBLEC3Ch);

static uint8_t CHIPoBLEC3Value[CHIPOBLE_TX_MAX_LEN] = "";
static const uint16_t CHIPoBLEC3ValueLen = sizeof(CHIPoBLEC3Value);

// clang-format off
/*!< \brief CHIPoBLE GATT server structure */
static const attsAttr_t CHIPoBLEList[] =
{
    /* CHIPoBLE service */
    {
        _attPrimSvcUuid,
        (uint8_t*)CHIPoBLEValSvc,
        (uint16_t*)&CHIPoBLELenSvc,
        sizeof(CHIPoBLEValSvc),
        0,
        ATTS_PERMIT_READ
    },
    /* CHIPoBLE Client RX characteristic */
    {
        _attChUuid,
        (uint8_t*)CHIPoBLERXCh,
        (uint16_t*)&CHIPoBLELenRXCh,
        sizeof(CHIPoBLERXCh),
        0,
        ATTS_PERMIT_READ
    },
    {
        &CHIPoBLERXCh[CHIPoBLE_CHARACTERISTIC_UUID_OFFSET],
        CHIPoBLERXValue,
        (uint16_t*)&CHIPoBLERXValueLen,
        sizeof(CHIPoBLERXValue),
        (ATTS_SET_UUID_128 | ATTS_SET_VARIABLE_LEN | ATTS_SET_WRITE_CBACK),
        ATTS_PERMIT_READ | ATTS_PERMIT_WRITE
    },
    {
        _attChUserDescUuid,
        (uint8_t*)CHIPoBLECfgValRXChrUsrDescr,
        (uint16_t*)&CHIPoBLECfgLenRXChrUsrDescr,
        sizeof(CHIPoBLECfgValRXChrUsrDescr) - 1,
        0,
        ATTS_PERMIT_READ
    },
    /* CHIPoBLE Client TX characteristic */
    {
        _attChUuid,
        (uint8_t*)CHIPoBLETXCh,
        (uint16_t*)&CHIPoBLELenTXCh,
        sizeof(CHIPoBLETXCh),
        0,
        ATTS_PERMIT_READ
    },
    {
        &CHIPoBLETXCh[CHIPoBLE_CHARACTERISTIC_UUID_OFFSET],
        CHIPoBLETXValue,
        (uint16_t*)&CHIPoBLETXValueLen,
        sizeof(CHIPoBLETXValue),
        (ATTS_SET_UUID_128 | ATTS_SET_VARIABLE_LEN | ATTS_SET_READ_CBACK),
        ATTS_PERMIT_READ
    },
    {
        _attCliChCfgUuid,
        CHIPoBLETXChCcc,
        (uint16_t*)&CHIPoBLELenTXChCcc,
        sizeof(CHIPoBLETXChCcc),
        ATTS_SET_CCC,
        ATTS_PERMIT_READ | ATTS_PERMIT_WRITE
    },
    {
        _attChUserDescUuid,
        (uint8_t*)CHIPoBLECfgValTXChrUsrDescr,
        (uint16_t*)&CHIPoBLECfgLenTXChrUsrDescr,
        sizeof(CHIPoBLECfgValTXChrUsrDescr) - 1,
        0,
        ATTS_PERMIT_READ
    },
    /* CHIPoBLE Client C3 characteristic */
    {
        _attChUuid,
        (uint8_t*)CHIPoBLEC3Ch,
        (uint16_t*)&CHIPoBLELenC3Ch,
        sizeof(CHIPoBLEC3Ch),
        0,
        ATTS_PERMIT_READ
    },
    {
        &CHIPoBLEC3Ch[CHIPoBLE_CHARACTERISTIC_UUID_OFFSET],
        CHIPoBLEC3Value,
        (uint16_t*)&CHIPoBLEC3ValueLen,
        sizeof(CHIPoBLEC3Value),
        (ATTS_SET_UUID_128 | ATTS_SET_VARIABLE_LEN | ATTS_SET_READ_CBACK),
        ATTS_PERMIT_READ
    },
};
// clang-format on

/*!< \brief CHIPoBLE group structure */
static attsGroup_t svcCHIPoBLEGroup = {
    NULL, (attsAttr_t*)CHIPoBLEList, NULL, NULL, CHIPOBLESRV_START_HDL, CHIPOBLESRV_END_HDL,
};

/*!< \brief Index of ccc register call back array */
enum {
    GATT_SC_CCC_IDX,     /*! GATT service, service changed characteristic */
    CHIPOBLE_RX_CCC_IDX, /*! CHIPoBLE Client RX characteristic */
    NUM_CCC_IDX
};

/*!< \brief Local structure containing external callbacks to the application */
static qvCHIP_Ble_Callbacks_t qvCHIP_IntCb;

/*! client characteristic configuration descriptors settings, indexed by above enumeration */
static const attsCccSet_t qvCHIP_CccSet[NUM_CCC_IDX] = {
    /* cccd handle          value range               security level */
    {GATT_SC_CH_CCC_HDL, ATT_CLIENT_CFG_INDICATE, DM_SEC_LEVEL_NONE}, /* FIT_GATT_SC_CCC_IDX */
    {CHIPOBLE_TX_CCC_HDL, ATT_CLIENT_CFG_INDICATE, DM_SEC_LEVEL_NONE} /* CHIPOBLE_RX_CCC_IDX */
};

/*!< \brief Cordio message handler ID */
static wsfHandlerId_t qvCHIP_Ble_MsgHandlerId;

/*!< \brief qvCHIP_Ble internal state */
static volatile qvCHIP_Ble_State_t qvCHIP_Ble_State = QV_BLE_STATE_NOT_INITIALIZED;

/*!< \brief Params for internal API */
typedef struct {
    uint8_t u8param1;
    uint8_t u8param2;
    uint16_t u16param1;
    uint16_t u16param2;
    uint16_t u16param3;
    uint32_t ptrParam; // all pointer types on this uC are 4 bytes, so we can aggregate
    qvStatus_t outParam;
} runOnParam_t;

static void string_reverse(const uint8_t* a, uint8_t* b, uint8_t len)
{
    uint8_t i;

    for(i = 0; i < len; i++)
    {
        b[i] = a[len - i - 1];
    }
}

uint8_t qvCHIP_Ble_ReadCallback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset,
                                attsAttr_t* pAttr)
{
    if(qvCHIP_IntCb.chrReadCallback != NULL)
    {
        qvCHIP_IntCb.chrReadCallback(connId, handle, operation, offset, (qvCHIP_Ble_Attr_t*)pAttr);
    }

    return ATT_SUCCESS;
}

/*! Attribute group write callback */
static uint8_t qvCHIP_Ble_WriteCallback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset,
                                        uint16_t len, uint8_t* pValue, attsAttr_t* pAttr)
{
    if(qvCHIP_IntCb.chrWriteCallback != NULL)
    {
        qvCHIP_IntCb.chrWriteCallback(connId, handle, operation, offset, len, pValue, (qvCHIP_Ble_Attr_t*)pAttr);
    }

    return ATT_SUCCESS;
}

static void qvCHIP_Ble_ATTS_CccCallback(attsCccEvt_t* pEvt)
{
    if((pEvt->handle == CHIPOBLE_TX_CCC_HDL) && (qvCHIP_IntCb.cccCallback != NULL))
    {
        qvCHIP_IntCb.cccCallback((qvCHIP_Ble_AttsCccEvt_t*)pEvt);
    }
}

/* DM callback */
static void qvCHIP_Ble_DmCallback(dmEvt_t* pDmEvt)
{
    dmEvt_t* pMsg;
    uint16_t len;

    GP_ASSERT_SYSTEM(pDmEvt);

    len = DmSizeOfEvt(pDmEvt);

    switch(pDmEvt->hdr.event)
    {
        case DM_RESET_CMPL_IND:
        {
#if(CORDIO_BLE_HOST_ATT_MAX_MTU > ATT_DEFAULT_MTU)

            uint16_t rxAclLen;

            rxAclLen = HciGetMaxRxAclLen();
            if(CORDIO_BLE_HOST_ATT_MAX_MTU > (rxAclLen - L2C_HDR_LEN))
            {
                HciSetMaxRxAclLen(CORDIO_BLE_HOST_ATT_MAX_MTU + L2C_HDR_LEN);
            }
#endif // CORDIO_BLE_HOST_ATT_MAX_MTU > ATT_DEFAULT_MTU

            // Populate hash correctly
            AttsCalculateDbHash();
            qvCHIP_Ble_State = QV_BLE_STATE_RESET;
            break;
        }
        case DM_ADV_START_IND:
        {
            qvCHIP_Ble_State = QV_BLE_STATE_ADV_STARTED;
            break;
        }
        // on connection we automatically stop advertising, but no DM_ADV_STOP_IND is sent
        case DM_CONN_OPEN_IND:
        case DM_ADV_STOP_IND:
        {
            qvCHIP_Ble_State = QV_BLE_STATE_ADV_STOPPED;
            break;
        }
        default:
            break;
    }

    if((pMsg = WsfMsgAlloc(len)) != NULL)
    {
        MEMCPY(pMsg, pDmEvt, len);
        WsfMsgSend(qvCHIP_Ble_MsgHandlerId, pMsg);
    }
}

/* ATT callback */
static void qvCHIP_Ble_AttCallback(attEvt_t* pAttEvt)
{
    attEvt_t* pMsg;
    GP_ASSERT_SYSTEM(pAttEvt);

    if((pMsg = WsfMsgAlloc(sizeof(attEvt_t) + pAttEvt->valueLen)) != NULL)
    {
        MEMCPY(pMsg, pAttEvt, sizeof(attEvt_t));
        pMsg->pValue = (uint8_t*)(pMsg + 1);
        MEMCPY(pMsg->pValue, pAttEvt->pValue, pAttEvt->valueLen);
        WsfMsgSend(qvCHIP_Ble_MsgHandlerId, pMsg);
    }
}

/*************************************************************************************************/
/*!
 *  \brief  ATT connection callback for app framework.
 *
 *  \param  pDmEvt  DM callback event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppServerConnCallback(dmEvt_t* pDmEvt)
{
    // appDbHdl_t  dbHdl;
    dmConnId_t connId = (dmConnId_t)pDmEvt->hdr.param;
    GP_ASSERT_SYSTEM(pDmEvt);

    if(pDmEvt->hdr.event == DM_CONN_OPEN_IND)
    {
        /* set up CCC table with uninitialized (all zero) values. */
        AttsCccInitTable(connId, NULL);

        /* set CSF values to default */
        AttsCsfConnOpen(connId, TRUE, NULL);

        /* set peer's data signing info */
        // appServerSetSigningInfo(connId);
    }
    else if(pDmEvt->hdr.event == DM_SEC_PAIR_CMPL_IND)
    {
        /* set peer's data signing info */
        // appServerSetSigningInfo(connId);
    }
    else if(pDmEvt->hdr.event == DM_CONN_CLOSE_IND)
    {
        /* clear CCC table on connection close */
        AttsCccClearTable(connId);
    }
}

static void qvCHIP_Ble_StackCallback(void* pMsg, uint32_t ulParameter2)
{
    qvCHIP_IntCb.stackCallback((qvCHIP_Ble_MsgHdr_t*)pMsg);
}

/* Process All Messages */
static void qvCHIP_Ble_MsgHandler(wsfEventMask_t event, wsfMsgHdr_t* pMsg)
{
    if(pMsg == NULL)
    {
        return;
    }

    if(pMsg->event == ATTS_DB_HASH_CALC_CMPL_IND)
    {
        // BLE ready for use
        GP_LOG_SYSTEM_PRINTF("Initiated -> True", 0);
        qvCHIP_isBleInitiated = true;
    }

    if(qvCHIP_IntCb.stackCallback != NULL)
    {
        /* We defer only advertising start/stop events, as they are low-frequency.
           The characteristic notifications/indications must be processed as they are now,
           otherwise BLE commissioning is broken.
        */
        if((pMsg->event == DM_ADV_START_IND) || (pMsg->event == DM_ADV_STOP_IND))
        {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            /* process the callback on the timer thread instead of BLE thread */
            xTimerPendFunctionCallFromISR(qvCHIP_Ble_StackCallback, (void*)pMsg, 0, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
        else
        {
            qvCHIP_IntCb.stackCallback((qvCHIP_Ble_MsgHdr_t*)pMsg);
        }
    }
}

qvStatus_t qvCHIP_BleInit(qvCHIP_Ble_Callbacks_t* callbacks)
{
    if(NULL == callbacks)
    {
        return QV_STATUS_INVALID_ARGUMENT;
    }

    /* Store callbacks from app */
    memcpy(&qvCHIP_IntCb, callbacks, sizeof(qvCHIP_Ble_Callbacks_t));

    gpBleComps_StackInit();
    cordioBleHost_Init();

    /* Register for stack callbacks */
    /* register callback to the device manager Scan and Advertisement messages */
    DmRegister(qvCHIP_Ble_DmCallback);
    /* register callback to the DM connection manager */
    DmConnRegister(DM_CLIENT_ID_APP, qvCHIP_Ble_DmCallback);
    AttRegister(qvCHIP_Ble_AttCallback);

    /* register callbacks to the attribute server */
    AttConnRegister(AppServerConnCallback);

    /* handle events */
    qvCHIP_Ble_MsgHandlerId = WsfOsSetNextHandler(qvCHIP_Ble_MsgHandler);

    /* GATT/GAP service */
    SvcCoreAddGroup();

    /* CHIP over BLE service */
    svcCHIPoBLEGroup.readCback = qvCHIP_Ble_ReadCallback;
    svcCHIPoBLEGroup.writeCback = qvCHIP_Ble_WriteCallback;
    AttsAddGroup(&svcCHIPoBLEGroup);

    /* CHIPoBLE */
    AttsCccRegister(NUM_CCC_IDX, (attsCccSet_t*)qvCHIP_CccSet, qvCHIP_Ble_ATTS_CccCallback);

    /* Reset the device */
    DmDevReset();

    return QV_STATUS_NO_ERROR;
}

qvStatus_t qvCHIP_BleSetUUIDs(const uint8_t* chipOBLE_UUID, const uint8_t* txChar_UUID, const uint8_t* rxChar_UUID,
                              const uint8_t* c3Char_UUID)
{
    if((chipOBLE_UUID == NULL) || (txChar_UUID == NULL) || (rxChar_UUID == NULL))
    {
        return QV_STATUS_INVALID_ARGUMENT;
    }

    /* UUIDs coming from CHIP are in human-readable string and need to be converted to LSB (little endian) for Cordio
     * stack */
    string_reverse(chipOBLE_UUID, CHIPoBLEValSvc, CHIPoBLE_SERVICE_LEN);
    string_reverse(txChar_UUID, &CHIPoBLETXCh[CHIPoBLE_CHARACTERISTIC_UUID_OFFSET], CHIPoBLE_CHARACTERISTIC_LEN);
    string_reverse(rxChar_UUID, &CHIPoBLERXCh[CHIPoBLE_CHARACTERISTIC_UUID_OFFSET], CHIPoBLE_CHARACTERISTIC_LEN);

    if(c3Char_UUID != NULL)
    {
        string_reverse(c3Char_UUID, &CHIPoBLEC3Ch[CHIPoBLE_CHARACTERISTIC_UUID_OFFSET], CHIPoBLE_CHARACTERISTIC_LEN);
        svcCHIPoBLEGroup.endHandle = CHIPOBLE_C3_HDL;
    }

    return QV_STATUS_NO_ERROR;
}

qvStatus_t qvCHIP_BleGetDeviceName(char* buf, size_t bufSize)
{
    if(NULL == buf)
    {
        return QV_STATUS_INVALID_ARGUMENT;
    }

    if(strlen(qvCHIP_LocalDeviceName) > bufSize)
    {
        return QV_STATUS_BUFFER_TOO_SMALL;
    }

    memcpy(buf, qvCHIP_LocalDeviceName, strlen(qvCHIP_LocalDeviceName));

    return QV_STATUS_NO_ERROR;
}

qvStatus_t qvCHIP_BleSetDeviceName(const char* devName)
{
    uint8_t localNameLen;

    if(NULL == devName)
    {
        return QV_STATUS_INVALID_ARGUMENT;
    }

    localNameLen = strlen(devName);

    if(0 == localNameLen)
    {
        return QV_STATUS_INVALID_ARGUMENT;
    }

    if(localNameLen > sizeof(qvCHIP_LocalDeviceName))
    {
        return QV_STATUS_BUFFER_TOO_SMALL;
    }

    memset(qvCHIP_LocalDeviceName, 0x0, sizeof(qvCHIP_LocalDeviceName));
    memcpy(qvCHIP_LocalDeviceName, devName, localNameLen);
    SvcCoreGapDeviceNameUpdate(qvCHIP_LocalDeviceName);

    return QV_STATUS_NO_ERROR;
}

static uint32_t qvCHIP_BleCloseConnection_Internal(void* arg)
{
    GP_ASSERT_DEV_EXT(arg != NULL);

    uint16_t conId = ((runOnParam_t*)arg)->u16param1;

    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->u16param1) == sizeof(conId));

    if(conId > DM_CONN_MAX || conId == DM_CONN_ID_NONE)
    {
        ((runOnParam_t*)arg)->outParam = QV_STATUS_INVALID_ARGUMENT;
        return RUNON_STATUS_OK;
    }

    DmConnClose(DM_CLIENT_ID_APP, (dmConnId_t)conId, HCI_ERR_REMOTE_TERMINATED);

    ((runOnParam_t*)arg)->outParam = QV_STATUS_NO_ERROR;

    return RUNON_STATUS_OK;
}

qvStatus_t qvCHIP_BleCloseConnection(uint16_t conId)
{
    runOnParam_t param = {
        .u16param1 = conId,
        .outParam = QV_STATUS_WRONG_STATE,
    };

    uint32_t ret = gpSched_RunOn(qvCHIP_BleCloseConnection_Internal, (void*)&param);

    GP_ASSERT_DEV_EXT(ret == RUNON_STATUS_OK);

    return param.outParam;
}

qvStatus_t qvCHIP_BleGetMTU(uint16_t conId, uint16_t* pMTUSize)
{
    if(conId > DM_CONN_MAX || conId == DM_CONN_ID_NONE || pMTUSize == NULL)
    {
        return QV_STATUS_INVALID_ARGUMENT;
    }

    *pMTUSize = AttGetMtu(conId);
    return QV_STATUS_NO_ERROR;
}

static uint32_t qvCHIP_BleSendIndication_Internal(void* arg)
{
    GP_ASSERT_DEV_EXT(arg != NULL);

    uint16_t conId = ((runOnParam_t*)arg)->u16param1;
    uint16_t handle = ((runOnParam_t*)arg)->u16param2;
    uint16_t length = ((runOnParam_t*)arg)->u16param3;
    uint8_t* data = (uint8_t*)((runOnParam_t*)arg)->ptrParam;

    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->u16param1) == sizeof(conId));
    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->u16param2) == sizeof(handle));
    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->u16param3) == sizeof(length));
    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->ptrParam) == sizeof(data));

    if(conId > DM_CONN_MAX || conId == DM_CONN_ID_NONE || NULL == data)
    {
        ((runOnParam_t*)arg)->outParam = QV_STATUS_INVALID_ARGUMENT;
        return RUNON_STATUS_OK;
    }

    AttsHandleValueInd((dmConnId_t)conId, handle, length, data);

    ((runOnParam_t*)arg)->outParam = QV_STATUS_NO_ERROR;

    return RUNON_STATUS_OK;
}

qvStatus_t qvCHIP_BleSendIndication(uint16_t conId, uint16_t handle, uint16_t length, uint8_t* data)
{
    runOnParam_t param = {
        .u16param1 = conId,
        .u16param2 = handle,
        .u16param3 = length,
        .ptrParam = (uint32_t)data,
        .outParam = QV_STATUS_WRONG_STATE,
    };

    uint32_t ret = gpSched_RunOn(qvCHIP_BleSendIndication_Internal, (void*)&param);

    GP_ASSERT_DEV_EXT(ret == RUNON_STATUS_OK);

    return param.outParam;
}

static uint32_t qvCHIP_BleSendNotification_Internal(void* arg)
{
    GP_ASSERT_DEV_EXT(arg != NULL);

    uint16_t conId = ((runOnParam_t*)arg)->u16param1;
    uint16_t handle = ((runOnParam_t*)arg)->u16param2;
    uint16_t length = ((runOnParam_t*)arg)->u16param3;
    uint8_t* data = (uint8_t*)((runOnParam_t*)arg)->ptrParam;

    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->u16param1) == sizeof(conId));
    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->u16param2) == sizeof(handle));
    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->u16param3) == sizeof(length));
    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->ptrParam) == sizeof(data));

    if(conId > DM_CONN_MAX || conId == DM_CONN_ID_NONE || NULL == data)
    {
        ((runOnParam_t*)arg)->outParam = QV_STATUS_INVALID_ARGUMENT;
        return RUNON_STATUS_OK;
    }

    AttsHandleValueNtf((dmConnId_t)conId, handle, length, data);

    ((runOnParam_t*)arg)->outParam = QV_STATUS_NO_ERROR;

    return RUNON_STATUS_OK;
}

qvStatus_t qvCHIP_BleSendNotification(uint16_t conId, uint16_t handle, uint16_t length, uint8_t* data)
{
    runOnParam_t param = {
        .u16param1 = conId,
        .u16param2 = handle,
        .u16param3 = length,
        .ptrParam = (uint32_t)data,
        .outParam = QV_STATUS_WRONG_STATE,
    };

    uint32_t ret = gpSched_RunOn(qvCHIP_BleSendNotification_Internal, (void*)&param);

    GP_ASSERT_DEV_EXT(ret == RUNON_STATUS_OK);

    return param.outParam;
}

static uint32_t qvCHIP_BleWriteC3Attr_Internal(void* arg)
{
    GP_ASSERT_DEV_EXT(arg != NULL);

    uint16_t length = ((runOnParam_t*)arg)->u16param1;
    uint8_t* data = (uint8_t*)((runOnParam_t*)arg)->ptrParam;

    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->u16param1) == sizeof(length));
    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->ptrParam) == sizeof(data));

    if(CHIPOBLE_C3_HDL != svcCHIPoBLEGroup.endHandle)
    {
        ((runOnParam_t*)arg)->outParam = QV_STATUS_NOT_IMPLEMENTED;
        return RUNON_STATUS_OK;
    }

    ((runOnParam_t*)arg)->outParam = AttsSetAttr(CHIPOBLE_C3_HDL, length, data);

    return RUNON_STATUS_OK;
}

qvStatus_t qvCHIP_BleWriteC3Attr(uint16_t length, uint8_t* data)
{
    runOnParam_t param = {
        .u16param1 = length,
        .ptrParam = (uint32_t)data,
        .outParam = QV_STATUS_WRONG_STATE,
    };

    uint32_t ret = gpSched_RunOn(qvCHIP_BleWriteC3Attr_Internal, (void*)&param);

    GP_ASSERT_DEV_EXT(ret == RUNON_STATUS_OK);

    return param.outParam;
}

static uint32_t qvCHIP_BleSetAdvInterval_Internal(void* arg)
{
    GP_ASSERT_DEV_EXT(arg != NULL);

    uint16_t intervalMin = ((runOnParam_t*)arg)->u16param1;
    uint16_t intervalMax = ((runOnParam_t*)arg)->u16param2;

    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->u16param1) == sizeof(intervalMin));
    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->u16param2) == sizeof(intervalMax));

    if(intervalMin > intervalMax)
    {
        ((runOnParam_t*)arg)->outParam = QV_STATUS_INVALID_ARGUMENT;
        return RUNON_STATUS_OK;
    }

    uint8_t advHandle = DM_ADV_HANDLE_DEFAULT;

    DmAdvSetInterval(advHandle, intervalMin, intervalMax);

    ((runOnParam_t*)arg)->outParam = QV_STATUS_NO_ERROR;

    return RUNON_STATUS_OK;
}

qvStatus_t qvCHIP_BleSetAdvInterval(uint16_t intervalMin, uint16_t intervalMax)
{
    runOnParam_t param = {
        .u16param1 = intervalMin,
        .u16param2 = intervalMax,
        .outParam = QV_STATUS_WRONG_STATE,
    };

    uint32_t ret = gpSched_RunOn(qvCHIP_BleSetAdvInterval_Internal, (void*)&param);

    GP_ASSERT_DEV_EXT(ret == RUNON_STATUS_OK);

    return param.outParam;
}
static uint32_t qvCHIP_BleSetAdvData_Internal(void* arg)
{
    GP_ASSERT_DEV_EXT(arg != NULL);

    qvAdvLocation_t location = (qvAdvLocation_t)((runOnParam_t*)arg)->u8param1;
    uint8_t len = ((runOnParam_t*)arg)->u8param2;
    uint8_t* pData = (uint8_t*)((runOnParam_t*)arg)->ptrParam;

    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->u8param1) == sizeof(location));
    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->u8param2) == sizeof(len));
    GP_ASSERT_DEV_EXT(sizeof(((runOnParam_t*)arg)->ptrParam) == sizeof(pData));

    if(NULL == pData)
    {
        ((runOnParam_t*)arg)->outParam = QV_STATUS_INVALID_ARGUMENT;
        return RUNON_STATUS_OK;
    }

    uint8_t advHandle = DM_ADV_HANDLE_DEFAULT;

    /* Set the advertising data */
    DmAdvSetData(advHandle, HCI_ADV_DATA_OP_COMP_FRAG, location, len, pData);

    ((runOnParam_t*)arg)->outParam = QV_STATUS_NO_ERROR;

    return RUNON_STATUS_OK;
}

qvStatus_t qvCHIP_BleSetAdvData(qvAdvLocation_t location, uint8_t len, uint8_t* pData)
{
    runOnParam_t param = {
        .u8param1 = (uint8_t)location,
        .u8param2 = len,
        .ptrParam = (uint32_t)pData,
        .outParam = QV_STATUS_WRONG_STATE,
    };

    uint32_t ret = gpSched_RunOn(qvCHIP_BleSetAdvData_Internal, (void*)&param);

    GP_ASSERT_DEV_EXT(ret == RUNON_STATUS_OK);

    return param.outParam;
}

static uint32_t qvCHIP_BleStartAdvertising_Internal(void* arg)
{
    GP_ASSERT_DEV_EXT(arg != NULL);

    uint8_t advHandle = DM_ADV_HANDLE_DEFAULT;
    uint8_t maxEaEvents = 0;
    uint16_t advDuration = 0;
    uint8_t randAddr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    /* All zero peer address, i.e. no peer */
    const uint8_t peerAddr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    /* Set the local Advertising Address type to Random */
    DmAdvSetAddrType(DM_ADDR_RANDOM);
    /* Get a random Advertiser Address */
    if(QV_STATUS_NO_ERROR != qvCHIP_RandomGet(sizeof(randAddr), (uint8_t*)randAddr))
    {
        ((runOnParam_t*)arg)->outParam = QV_STATUS_INVALID_DATA;
        return RUNON_STATUS_OK;
    }
    /* Force the random static format on the randomly generated address */
    DM_RAND_ADDR_SET((uint8_t*)randAddr, DM_RAND_ADDR_STATIC);
    /* Set the Advertiser address in the host stack */
    DmDevSetRandAddr((uint8_t*)randAddr);

    /* Configure the advertising parameters */
    DmAdvConfig(advHandle, DM_ADV_CONN_UNDIRECT, DM_ADDR_PUBLIC, (uint8_t*)peerAddr);
    DmAdvSetChannelMap(advHandle, QVCHIP_ADV_CHAN_ALL);

    /* Scan advertising */
    DmAdvStart(DM_NUM_ADV_SETS, &advHandle, &advDuration, &maxEaEvents);

    ((runOnParam_t*)arg)->outParam = QV_STATUS_NO_ERROR;

    return RUNON_STATUS_OK;
}

qvStatus_t qvCHIP_BleStartAdvertising(void)
{
    runOnParam_t param = {
        .outParam = QV_STATUS_WRONG_STATE,
    };

    if(qvCHIP_isBleInitiated == false)
    {
        return QV_STATUS_WRONG_STATE;
    }

    qvCHIP_BleStopAdvertising();

    uint32_t ret = gpSched_RunOn(qvCHIP_BleStartAdvertising_Internal, (void*)&param);

    while(qvCHIP_Ble_State != QV_BLE_STATE_ADV_STARTED)
    {
        vTaskDelay(1);
    }

    GP_ASSERT_DEV_EXT(ret == RUNON_STATUS_OK);

    return param.outParam;
}

static uint32_t qvCHIP_BleStopAdvertising_Internal(void* arg)
{
    GP_ASSERT_DEV_EXT(arg != NULL);

    uint8_t advHandle = DM_ADV_HANDLE_DEFAULT;

    DmAdvStop(DM_NUM_ADV_SETS, &advHandle);

    ((runOnParam_t*)arg)->outParam = QV_STATUS_NO_ERROR;
    return RUNON_STATUS_OK;
}

qvStatus_t qvCHIP_BleStopAdvertising(void)
{
    runOnParam_t param = {
        .outParam = QV_STATUS_WRONG_STATE,
    };

    if(qvCHIP_isBleInitiated == false)
    {
        return QV_STATUS_WRONG_STATE;
    }

    uint32_t ret = gpSched_RunOn(qvCHIP_BleStopAdvertising_Internal, (void*)&param);

    while(qvCHIP_Ble_State == QV_BLE_STATE_ADV_STARTED)
    {
        vTaskDelay(1);
    }

    GP_ASSERT_DEV_EXT(ret == RUNON_STATUS_OK);

    return param.outParam;
}

uint16_t qvCHIP_BleGetHandle(bool rxHandle)
{
    if(rxHandle == true)
    {
        return CHIPOBLE_RX_HDL;
    }
    else
    {
        return CHIPOBLE_TX_HDL;
    }
}

qvStatus_t qvCHIP_BleTaskCreate(void)
{
    hal_BleTaskCreate();
    return QV_STATUS_NO_ERROR;
}

qvStatus_t qvCHIP_BleTaskDelete(void)
{
    hal_BleTaskDestroy();
    return QV_STATUS_NO_ERROR;
}

bool qvCHIP_IsBleTaskCreated(void)
{
    return hal_IsBleTaskCreated();
}
