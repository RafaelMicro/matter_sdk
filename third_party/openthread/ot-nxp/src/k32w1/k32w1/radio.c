/*
 *  Copyright (c) 2022-2023, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file implements the OpenThread platform abstraction for radio communication.
 *
 */
#if (defined(__GNUC__))
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#include OPENTHREAD_PROJECT_CORE_CONFIG_FILE

#include "EmbeddedTypes.h"
#include "FunctionLib.h"
#include "Phy.h"
#include "PhyInterface.h"
#include "fsl_component_messaging.h"
#include "fsl_component_timer_manager.h"
#if defined(HDI_MODE) && (HDI_MODE == 1)
#include "hdi.h"
#endif
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "openthread-system.h"
#include "utils/code_utils.h"
#include "utils/link_metrics.h"
#include "utils/mac_frame.h"

#include <openthread/platform/alarm-milli.h>
#include <openthread/platform/diag.h>
#include <openthread/platform/radio.h>
#include <openthread/platform/time.h>

#include "PWR_Interface.h"

#include "fwk_platform_ot.h"

#if USE_NBU
void PLATFORM_RemoteActiveReq();
void PLATFORM_RemoteActiveRel();
#else /* USE_NBU */
#define PLATFORM_RemoteActiveReq()
#define PLATFORM_RemoteActiveRel()
#endif /* USE_NBU */

#if OPENTHREAD_CONFIG_MAC_CSL_RECEIVER_ENABLE

#define CMP_OVHD (4 * IEEE802154_SYMBOL_TIME_US) /* 2 LPTRM (32 kHz) ticks */

static bool_t csl_rx = FALSE;

static void set_csl_sample_time();
static void start_csl_receiver();
static void stop_csl_receiver();

#else /* OPENTHREAD_CONFIG_MAC_CSL_RECEIVER_ENABLE */
#define start_csl_receiver()
#define stop_csl_receiver()
#endif /* OPENTHREAD_CONFIG_MAC_CSL_RECEIVER_ENABLE */

#define TX_ENCRYPT_DELAY_SYM 200

// clang-format off
#define DEFAULT_CHANNEL                  (11)
#define DEFAULT_CCA_MODE                 (gPhyCCAMode1_c)
#define IEEE802154_ACK_REQUEST           (1 << 5)
#define IEEE802154_MIN_LENGTH            (5)
#define IEEE802154_FRM_CTL_LO_OFFSET     (0)
#define IEEE802154_DSN_OFFSET            (2)
#define IEEE802154_FRM_TYPE_MASK         (0x7)
#define IEEE802154_FRM_TYPE_ACK          (0x2)
#define IEEE802154_SYMBOL_TIME_US        (16)
#define IEEE802154_TURNAROUND_LEN_SYM    (12)
#define IEEE802154_CCA_LEN_SYM           (8)
#define IEEE802154_PHY_SHR_LEN_SYM       (10)
#define IEEE802154_IMM_ACK_WAIT_SYM      (54)
#define IEEE802154_ENH_ACK_WAIT_SYM      (90)

#define NMAX_RXRING_BUFFERS              (8)
#define RX_ON_IDLE_START                 (1)
#define RX_ON_IDLE_STOP                  (0)

#define PHY_TMR_MAX_VALUE                (0x00FFFFFF)

#define CSL_UNCERT 32 ///< The Uncertainty of the scheduling CSL of transmission by the parent, in ±10 us units.

typedef struct
{
    otRadioFrame RxFrame;
    void * pPhyBuffer; // keep in this pointer variable the pointer to Phy allocated buffer to be free after receive processing of the frame
} extendedRadioFrame;

typedef struct
{
    extendedRadioFrame *   head;
    extendedRadioFrame *   tail;
    extendedRadioFrame     extRxFrame[NMAX_RXRING_BUFFERS];
} rxRingBuffer;

extern void PWR_DisallowDeviceToSleep(void);
extern void PWR_AllowDeviceToSleep(void);

// clang-format on
static otRadioState sState = OT_RADIO_STATE_DISABLED;
static uint16_t     sPanId;
static uint8_t      sChannel = DEFAULT_CHANNEL;
static int8_t       sMaxED;
static bool_t       sPromiscuousEnable = FALSE;
static int8_t       sAutoTxPwrLevel    = 0;

/* ISR Signaling Flags */
static bool    sTxDone     = false;
static bool    sRxDone     = false;
static bool    sEdScanDone = false;
static otError sTxStatus;

static otRadioFrame sTxFrame;
static uint8_t      sTxData[sizeof(macToPdDataMessage_t) + OT_RADIO_FRAME_MAX_SIZE];

static volatile uint32_t sunRxMode = RX_ON_IDLE_START;
static rxRingBuffer      sRxRing;     /* Receive Ring Buffer */
static otRadioFrame      sRxAckFrame; /* RX Ack Buffers */
static uint8_t           sRxAckData[OT_RADIO_FRAME_MAX_SIZE];
static otRadioCaps       caps;

#if OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2
static uint32_t sCslPeriod;
static uint32_t sCslSampleTimeUs;
#endif

/* Private functions */
static uint16_t rf_compute_csl_phase(uint32_t aTimeUs);
static void     rf_abort(void);
static void     rf_set_channel(uint8_t channel);
static void     rf_set_tx_power(int8_t tx_power);
static uint64_t rf_adjust_tstamp_from_phy(uint64_t ts);
static uint32_t rf_adjust_tstamp_from_ot(uint32_t time);

static void                rf_rx_on_idle(uint32_t newValue);
static void                ResetRxRingBuffer(rxRingBuffer *aRxRing);
static void                PushRxRingBuffer(rxRingBuffer *aRxRing);
static extendedRadioFrame *PopRxRingBuffer(rxRingBuffer *aRxRing);
static bool                IsEmptyRxRingBuffer(rxRingBuffer *aRxRing);
static unsigned char       NAvailableRxBuffers(rxRingBuffer *aRxRing);

static uint8_t ot_phy_ctx = (uint8_t)(-1);

/**
 * Stub function used for controlling low power mode
 *
 */
WEAK void App_AllowDeviceToSleep()
{
}

/**
 * Stub function used for controlling low power mode
 *
 */
WEAK void App_DisallowDeviceToSleep()
{
}

otRadioState otPlatRadioGetState(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    return sState;
}

void otPlatRadioGetIeeeEui64(otInstance *aInstance, uint8_t *aIeeeEui64)
{
    OT_UNUSED_VARIABLE(aInstance);

    PLATFORM_GetIeee802_15_4Addr(aIeeeEui64);
}

void otPlatRadioSetPanId(otInstance *aInstance, uint16_t aPanId)
{
    OT_UNUSED_VARIABLE(aInstance);

    macToPlmeMessage_t msg;

    msg.msgType                          = gPlmeSetReq_c;
    msg.msgData.setReq.PibAttribute      = gPhyPibPanId_c;
    msg.msgData.setReq.PibAttributeValue = (uint64_t)aPanId;

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);

    sPanId = aPanId;
}

void otPlatRadioSetExtendedAddress(otInstance *aInstance, const otExtAddress *aExtAddress)
{
    OT_UNUSED_VARIABLE(aInstance);

    macToPlmeMessage_t msg;

    msg.msgType                          = gPlmeSetReq_c;
    msg.msgData.setReq.PibAttribute      = gPhyPibLongAddress_c;
    msg.msgData.setReq.PibAttributeValue = *(uint64_t *)aExtAddress->m8;

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);
}

void otPlatRadioSetShortAddress(otInstance *aInstance, uint16_t aShortAddress)
{
    OT_UNUSED_VARIABLE(aInstance);

    macToPlmeMessage_t msg;

    msg.msgType                          = gPlmeSetReq_c;
    msg.msgData.setReq.PibAttribute      = gPhyPibShortAddress_c;
    msg.msgData.setReq.PibAttributeValue = (uint64_t)aShortAddress;

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);
}

otError otPlatRadioEnable(otInstance *aInstance)
{
    otEXPECT(!otPlatRadioIsEnabled(aInstance));

#if !USE_NBU
    PHY_Enable();
#endif
    sState = OT_RADIO_STATE_SLEEP;

exit:
    return OT_ERROR_NONE;
}

otError otPlatRadioDisable(otInstance *aInstance)
{
    otEXPECT(otPlatRadioIsEnabled(aInstance));

    stop_csl_receiver();

#if !USE_NBU
    PHY_Disable();
#endif
    sState = OT_RADIO_STATE_DISABLED;

exit:
    return OT_ERROR_NONE;
}

bool otPlatRadioIsEnabled(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    return sState != OT_RADIO_STATE_DISABLED;
}

otError otPlatRadioSleep(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    otError status = OT_ERROR_NONE;

    otEXPECT_ACTION(((sState != OT_RADIO_STATE_TRANSMIT) && (sState != OT_RADIO_STATE_DISABLED)),
                    status = OT_ERROR_INVALID_STATE);

    rf_abort();

    stop_csl_receiver();

    App_AllowDeviceToSleep();

    sState = OT_RADIO_STATE_SLEEP;

exit:
    return status;
}

otError otPlatRadioReceive(otInstance *aInstance, uint8_t aChannel)
{
    OT_UNUSED_VARIABLE(aInstance);

    macToPlmeMessage_t msg;

    otError     status = OT_ERROR_NONE;
    phyStatus_t phy_status;

    App_DisallowDeviceToSleep();

    otEXPECT_ACTION((sState != OT_RADIO_STATE_DISABLED), status = OT_ERROR_INVALID_STATE);

    sState = OT_RADIO_STATE_RECEIVE;

    if (sChannel != aChannel)
    {
        rf_abort();
        rf_set_channel(aChannel);
    }

    if (sunRxMode)
    {
        start_csl_receiver();

        // restart Rx on idle only if it was enabled
        msg.msgType                          = gPlmeSetReq_c;
        msg.msgData.setReq.PibAttribute      = gPhyPibRxOnWhenIdle;
        msg.msgData.setReq.PibAttributeValue = (uint64_t)1;

        phy_status = MAC_PLME_SapHandler(&msg, ot_phy_ctx);
        if (phy_status != gPhySuccess_c)
        {
            status = OT_ERROR_INVALID_STATE;
        }
    }
exit:
    return status;
}

void otPlatRadioEnableSrcMatch(otInstance *aInstance, bool aEnable)
{
    OT_UNUSED_VARIABLE(aInstance);

    macToPlmeMessage_t msg;

    msg.msgType          = gPlmeSetSAMState_c;
    msg.msgData.SAMState = aEnable;

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);
}

otError otPlatRadioAddSrcMatchShortEntry(otInstance *aInstance, const uint16_t aShortAddress)
{
    OT_UNUSED_VARIABLE(aInstance);

    otError            status = OT_ERROR_NONE;
    macToPlmeMessage_t msg;

    msg.msgType                  = gPlmeAddToSapTable_c;
    msg.msgData.deviceAddr.mode  = 2;
    msg.msgData.deviceAddr.panId = sPanId;

    memcpy(msg.msgData.deviceAddr.addr, (uint8_t *)&aShortAddress, 2);

    if (gPhySuccess_c != MAC_PLME_SapHandler(&msg, ot_phy_ctx))
    {
        /* the status is not returned from PHY over RPMSG */
        status = OT_ERROR_NO_BUFS;
    }

    return status;
}

otError otPlatRadioAddSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress)
{
    OT_UNUSED_VARIABLE(aInstance);

    otError            status = OT_ERROR_NONE;
    macToPlmeMessage_t msg;

    msg.msgType                  = gPlmeAddToSapTable_c;
    msg.msgData.deviceAddr.mode  = 3;
    msg.msgData.deviceAddr.panId = sPanId;

    memcpy(msg.msgData.deviceAddr.addr, (uint8_t *)aExtAddress, 8);

    if (gPhySuccess_c != MAC_PLME_SapHandler(&msg, ot_phy_ctx))
    {
        /* the status is not returned from PHY over RPMSG */
        status = OT_ERROR_NO_BUFS;
    }

    return status;
}

otError otPlatRadioClearSrcMatchShortEntry(otInstance *aInstance, const uint16_t aShortAddress)
{
    OT_UNUSED_VARIABLE(aInstance);

    otError            status = OT_ERROR_NONE;
    macToPlmeMessage_t msg;

    msg.msgType                  = gPlmeRemoveFromSAMTable_c;
    msg.msgData.deviceAddr.mode  = 2;
    msg.msgData.deviceAddr.panId = sPanId;

    memcpy(msg.msgData.deviceAddr.addr, (uint8_t *)&aShortAddress, 2);

    if (gPhySuccess_c != MAC_PLME_SapHandler(&msg, ot_phy_ctx))
    {
        /* the status is not returned from PHY over RPMSG */
        status = OT_ERROR_NO_ADDRESS;
    }

    return status;
}

otError otPlatRadioClearSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress)
{
    OT_UNUSED_VARIABLE(aInstance);

    otError            status = OT_ERROR_NONE;
    macToPlmeMessage_t msg;

    msg.msgType                  = gPlmeRemoveFromSAMTable_c;
    msg.msgData.deviceAddr.mode  = 3;
    msg.msgData.deviceAddr.panId = sPanId;

    memcpy(msg.msgData.deviceAddr.addr, (uint8_t *)aExtAddress, 8);

    if (gPhySuccess_c != MAC_PLME_SapHandler(&msg, ot_phy_ctx))
    {
        /* the status is not returned from PHY over RPMSG */
        status = OT_ERROR_NO_ADDRESS;
    }

    return status;
}

void otPlatRadioClearSrcMatchShortEntries(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    /* This must be implemented */
}

void otPlatRadioClearSrcMatchExtEntries(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    /* This must be implemented */
}

otRadioFrame *otPlatRadioGetTransmitBuffer(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    return &sTxFrame;
}

otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aFrame)
{
    otError status = OT_ERROR_NONE;
    /* sTxData bufer has reserved memory for both macToPdDataMessage_t and actual data frame after */
    macToPdDataMessage_t *msg = (macToPdDataMessage_t *)sTxData;
    phyStatus_t           phy_status;

    App_DisallowDeviceToSleep();

    otEXPECT_ACTION((sState != OT_RADIO_STATE_DISABLED), status = OT_ERROR_INVALID_STATE);

    rf_set_channel(aFrame->mChannel);

    msg->msgType                     = gPdDataReq_c;
    msg->msgData.dataReq.slottedTx   = gPhyUnslottedMode_c;
    msg->msgData.dataReq.psduLength  = aFrame->mLength;
    msg->msgData.dataReq.CCABeforeTx = DEFAULT_CCA_MODE;

    /* aFrame->mPsdu will point to sTxData data buffer after macToPdDataMessage_t structure */
    msg->msgData.dataReq.pPsdu = aFrame->mPsdu;

    if (aFrame->mPsdu[IEEE802154_FRM_CTL_LO_OFFSET] & IEEE802154_ACK_REQUEST)
    {
        msg->msgData.dataReq.ackRequired = gPhyRxAckRqd_c;
        /* The 3 bytes are 1 byte frame length and 2 bytes FCS */
        msg->msgData.dataReq.txDuration = IEEE802154_CCA_LEN_SYM + IEEE802154_PHY_SHR_LEN_SYM +
                                          (3 + aFrame->mLength) * OT_RADIO_SYMBOLS_PER_OCTET +
                                          IEEE802154_TURNAROUND_LEN_SYM;

        if (otMacFrameIsVersion2015(aFrame))
        {
            /* Because enhaced ack can be of variable length we need to set the timeout value to
               account for the FCF and addressing fields only, and stop the timeout timer after
               they are received and validated as a valid ACK */
            msg->msgData.dataReq.txDuration += IEEE802154_ENH_ACK_WAIT_SYM;
        }
        else
        {
            msg->msgData.dataReq.txDuration += IEEE802154_IMM_ACK_WAIT_SYM;
        }
    }
    else
    {
        msg->msgData.dataReq.ackRequired = gPhyNoAckRqd_c;
        msg->msgData.dataReq.txDuration  = 0xFFFFFFFFU;
    }

    if ((aFrame->mInfo.mTxInfo.mTxDelay > 0) && (caps & OT_RADIO_CAPS_TRANSMIT_TIMING))
    {
        msg->msgData.dataReq.startTime =
            rf_adjust_tstamp_from_ot(aFrame->mInfo.mTxInfo.mTxDelayBaseTime + aFrame->mInfo.mTxInfo.mTxDelay);
        msg->msgData.dataReq.startTime /= IEEE802154_SYMBOL_TIME_US;
    }
    else
    {
        msg->msgData.dataReq.startTime = gPhySeqStartAsap_c;
    }

    msg->msgData.dataReq.flags = 0;

#if OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2
    if (otMacFrameIsSecurityEnabled(aFrame) && otMacFrameIsKeyIdMode1(aFrame))
    {
        if (!aFrame->mInfo.mTxInfo.mIsSecurityProcessed)
        {
            msg->msgData.dataReq.flags |= gPhyEncFrame;

            if (!aFrame->mInfo.mTxInfo.mIsHeaderUpdated)
            {
                msg->msgData.dataReq.flags |= gPhyUpdHDr;

#if OPENTHREAD_CONFIG_MAC_CSL_RECEIVER_ENABLE
                if (aFrame->mInfo.mTxInfo.mCslPresent)
                {
                    uint32_t hdrTimeUs;

                    start_csl_receiver();

                    /* Add TX_ENCRYPT_DELAY_SYM symbols delay to allow encryption to finish */
                    msg->msgData.dataReq.startTime = PhyTime_ReadClock() + TX_ENCRYPT_DELAY_SYM;

                    hdrTimeUs = otPlatTimeGet() +
                                (TX_ENCRYPT_DELAY_SYM + IEEE802154_PHY_SHR_LEN_SYM) * IEEE802154_SYMBOL_TIME_US;
                    otMacFrameSetCslIe(aFrame, sCslPeriod, rf_compute_csl_phase(hdrTimeUs));
                }
#endif
            }
        }
    }
#endif

    phy_status = MAC_PD_SapHandler(msg, ot_phy_ctx);
    if (phy_status == gPhySuccess_c)
    {
        sTxStatus = OT_ERROR_NONE;
        sState    = OT_RADIO_STATE_TRANSMIT;
        otPlatRadioTxStarted(aInstance, aFrame);
    }
    else
    {
        status = OT_ERROR_INVALID_STATE;
    }
exit:
    return status;
}

int8_t otPlatRadioGetRssi(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);
    macToPlmeMessage_t msg;

    msg.msgType                          = gPlmeGetReq_c;
    msg.msgData.getReq.PibAttribute      = gPhyGetRSSILevel_c;
    msg.msgData.getReq.PibAttributeValue = 127; /* RSSI is invalid*/

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);

    return (int8_t)msg.msgData.getReq.PibAttributeValue;
}

otRadioCaps otPlatRadioGetCaps(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    caps = OT_RADIO_CAPS_ACK_TIMEOUT | OT_RADIO_CAPS_SLEEP_TO_TX | OT_RADIO_CAPS_ENERGY_SCAN;

#if OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2
    caps |= OT_RADIO_CAPS_TRANSMIT_SEC | OT_RADIO_CAPS_TRANSMIT_TIMING;
#endif

    return caps;
}

bool otPlatRadioGetPromiscuous(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    return sPromiscuousEnable;
}

void otPlatRadioSetPromiscuous(otInstance *aInstance, bool aEnable)
{
    OT_UNUSED_VARIABLE(aInstance);

    macToPlmeMessage_t msg;
    sPromiscuousEnable = aEnable;

    msg.msgType                          = gPlmeSetReq_c;
    msg.msgData.setReq.PibAttribute      = gPhyPibPromiscuousMode_c;
    msg.msgData.setReq.PibAttributeValue = (uint64_t)aEnable;

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);
}

otError otPlatRadioEnergyScan(otInstance *aInstance, uint8_t aScanChannel, uint16_t aScanDuration)
{
    OT_UNUSED_VARIABLE(aInstance);

    otError            status = OT_ERROR_NONE;
    phyStatus_t        phy_status;
    macToPlmeMessage_t msg;

    App_DisallowDeviceToSleep();

    otEXPECT_ACTION(((sState != OT_RADIO_STATE_TRANSMIT) && (sState != OT_RADIO_STATE_DISABLED)),
                    status = OT_ERROR_INVALID_STATE);

    rf_abort();

    sMaxED = -128;
    rf_set_channel(aScanChannel);

    msg.msgType                          = gPlmeEdReq_c;
    msg.msgData.edReq.startTime          = gPhySeqStartAsap_c;
    msg.msgData.edReq.measureDurationSym = (aScanDuration * 1000) / OT_RADIO_SYMBOL_TIME;

    phy_status = MAC_PLME_SapHandler(&msg, ot_phy_ctx);
    if (phy_status != gPhySuccess_c)
    {
        status = OT_ERROR_INVALID_STATE;
    }
exit:
    return status;
}

otError otPlatRadioGetTransmitPower(otInstance *aInstance, int8_t *aPower)
{
    otError error = OT_ERROR_NONE;

    otEXPECT_ACTION(aPower != NULL, error = OT_ERROR_INVALID_ARGS);
    *aPower = sAutoTxPwrLevel;

exit:
    return error;
}

otError otPlatRadioSetTransmitPower(otInstance *aInstance, int8_t aPower)
{
    OT_UNUSED_VARIABLE(aInstance);

    /* Set Power level for TX */
    rf_set_tx_power(aPower);
    sAutoTxPwrLevel = aPower;

    return OT_ERROR_NONE;
}

otError otPlatRadioGetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t *aThreshold)
{
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aThreshold);

    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioSetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t aThreshold)
{
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aThreshold);

    return OT_ERROR_NOT_IMPLEMENTED;
}

int8_t otPlatRadioGetReceiveSensitivity(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    return -102;
}

#if OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2
#if OPENTHREAD_CONFIG_MLE_LINK_METRICS_SUBJECT_ENABLE
otError otPlatRadioConfigureEnhAckProbing(otInstance          *aInstance,
                                          otLinkMetrics        aLinkMetrics,
                                          const otShortAddress aShortAddress,
                                          const otExtAddress  *aExtAddress)
{
    OT_UNUSED_VARIABLE(aInstance);

    otError      error     = OT_ERROR_NONE;
    uint32_t     ieDataLen = 0;
    uint32_t     ieParam   = 0;
    otMacAddress macAddress;
    macAddress.mType                  = OT_MAC_ADDRESS_TYPE_SHORT;
    macAddress.mAddress.mShortAddress = aShortAddress;
    macToPlmeMessage_t msg;

    error = otLinkMetricsConfigureEnhAckProbing(aShortAddress, aExtAddress, aLinkMetrics);
    otEXPECT(error == OT_ERROR_NONE);

    ieDataLen = otLinkMetricsEnhAckGetDataLen(&macAddress);

    if (ieDataLen > 0)
    {
        ieDataLen = otMacFrameGenerateEnhAckProbingIe(msg.msgData.AckIeData.data, NULL, ieDataLen);
    }

    ieParam = (aLinkMetrics.mLqi > 0 ? IeData_Lqi_c : 0) | (aLinkMetrics.mLinkMargin > 0 ? IeData_LinkMargin_c : 0) |
              (aLinkMetrics.mRssi > 0 ? IeData_Rssi_c : 0);

    /* If ieDataLen remains 0 we will delete the IE data */
    msg.msgType                 = gPlmeConfigureAckIeData_c;
    msg.msgData.AckIeData.param = (ieDataLen > 0 ? IeData_MSB_VALID_DATA : 0);
    msg.msgData.AckIeData.param |= ieParam;
    msg.msgData.AckIeData.shortAddr = aShortAddress;
    memcpy(msg.msgData.AckIeData.extAddr, aExtAddress, 8);

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);

exit:
    return error;
}
#endif

void otPlatRadioSetMacKey(otInstance             *aInstance,
                          uint8_t                 aKeyIdMode,
                          uint8_t                 aKeyId,
                          const otMacKeyMaterial *aPrevKey,
                          const otMacKeyMaterial *aCurrKey,
                          const otMacKeyMaterial *aNextKey,
                          otRadioKeyType          aKeyType)
{
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aKeyIdMode);

    macToPlmeMessage_t msg;

    assert(aKeyType == OT_KEY_TYPE_LITERAL_KEY);
    assert(aPrevKey != NULL && aCurrKey != NULL && aNextKey != NULL);

    msg.msgType                  = gPlmeSetMacKey_c;
    msg.msgData.MacKeyData.keyId = aKeyId;

    memcpy(msg.msgData.MacKeyData.prevKey, aPrevKey, 16);
    memcpy(msg.msgData.MacKeyData.currKey, aCurrKey, 16);
    memcpy(msg.msgData.MacKeyData.nextKey, aNextKey, 16);

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);
}

void otPlatRadioSetMacFrameCounter(otInstance *aInstance, uint32_t aMacFrameCounter)
{
    OT_UNUSED_VARIABLE(aInstance);

    macToPlmeMessage_t msg;

    msg.msgType                 = gPlmeSetMacFrameCounter_c;
    msg.msgData.MacFrameCounter = aMacFrameCounter;

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);
}

void otPlatRadioSetMacFrameCounterIfLarger(otInstance *aInstance, uint32_t aMacFrameCounter)
{
    OT_UNUSED_VARIABLE(aInstance);

    macToPlmeMessage_t msg;

    msg.msgType                 = gPlmeSetMacFrameCounterIfLarger_c;
    msg.msgData.MacFrameCounter = aMacFrameCounter;

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);
}

uint64_t otPlatRadioGetNow(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    return otPlatTimeGet();
}

uint8_t otPlatRadioGetCslAccuracy(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    return CONFIG_PLATFORM_CSL_ACCURACY;
}

uint8_t otPlatRadioGetCslUncertainty(otInstance *aInstance)
{
    OT_UNUSED_VARIABLE(aInstance);

    return CSL_UNCERT;
}

#if OPENTHREAD_CONFIG_MAC_CSL_RECEIVER_ENABLE
otError otPlatRadioReceiveAt(otInstance *aInstance, uint8_t aChannel, uint32_t aStart, uint32_t aDuration)
{
    OT_UNUSED_VARIABLE(aInstance);
    macToPlmeMessage_t msg;
    otError            status = OT_ERROR_NONE;

    otEXPECT_ACTION((sState == OT_RADIO_STATE_SLEEP), status = OT_ERROR_FAILED);
    sState = OT_RADIO_STATE_RECEIVE;

    /* checks internally if the channel needs to be changed */
    rf_set_channel(aChannel);

    aStart = rf_adjust_tstamp_from_ot(aStart);

    msg.msgType                            = gPlmeSetTRxStateReq_c;
    msg.msgData.setTRxStateReq.slottedMode = gPhyUnslottedMode_c;
    msg.msgData.setTRxStateReq.state       = gPhySetRxOn_c;
    msg.msgData.setTRxStateReq.rxDuration  = aDuration / IEEE802154_SYMBOL_TIME_US;
    msg.msgData.setTRxStateReq.startTime   = aStart / IEEE802154_SYMBOL_TIME_US;

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);

exit:
    return status;
}

otError otPlatRadioEnableCsl(otInstance         *aInstance,
                             uint32_t            aCslPeriod,
                             otShortAddress      aShortAddr,
                             const otExtAddress *aExtAddr)
{
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aExtAddr);
    OT_UNUSED_VARIABLE(aShortAddr);

    sCslPeriod = aCslPeriod;

    macToPlmeMessage_t msg;

    msg.msgType           = gPlmeCslEnable_c;
    msg.msgData.cslPeriod = aCslPeriod;

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);

    return OT_ERROR_NONE;
}

void otPlatRadioUpdateCslSampleTime(otInstance *aInstance, uint32_t aCslSampleTime)
{
    OT_UNUSED_VARIABLE(aInstance);

    sCslSampleTimeUs = aCslSampleTime;
}

static void set_csl_sample_time()
{
    if (!sCslPeriod)
    {
        return;
    }

    macToPlmeMessage_t msg;
    uint32_t           csl_period = sCslPeriod * 10 * IEEE802154_SYMBOL_TIME_US;
    uint32_t           dt         = sCslSampleTimeUs - (uint32_t)otPlatTimeGet();

    /* next channel sample should be in the future */
    while ((dt <= CMP_OVHD) || (dt > (CMP_OVHD + 2 * csl_period)))
    {
        sCslSampleTimeUs += csl_period;
        dt = sCslSampleTimeUs - (uint32_t)otPlatTimeGet();
    }

    /* The CSL sample time is in microseconds and PHY function expects also microseconds */
    msg.msgType               = gPlmeCslSetSampleTime_c;
    msg.msgData.cslSampleTime = rf_adjust_tstamp_from_ot(sCslSampleTimeUs);

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);
}

static void start_csl_receiver()
{
    if (!sCslPeriod)
    {
        return;
    }

    /* NBU has to be awake during CSL receiver trx  so that conversion from
       PHY timebase (NBU) to TMR timebase (host) is valid */
    if (!csl_rx)
    {
        PLATFORM_RemoteActiveReq();

        csl_rx = TRUE;
    }

    /* sample time is converted to PHY time */
    set_csl_sample_time();
}

static void stop_csl_receiver()
{
    if (csl_rx)
    {
        PLATFORM_RemoteActiveRel();

        csl_rx = FALSE;
    }
}
#endif /* OPENTHREAD_CONFIG_MAC_CSL_RECEIVER_ENABLE */

/*
 * Compute the CSL Phase for the aTimeUs - i.e. the time from the aTimeUs to sCslSampleTimeUs.
 * The assumption is that sCslSampleTimeUs > aTimeUs. Since the time is kept with a limited timer in reality
 * it means that sometimes sCslSampleTimeUs < aTimeUs, when the timer overflows. Therefore the formula should be:
 *
 * if (aTimeUs <= sCslSampleTimeUs)
 *         cslPhaseUs = sCslSampleTimeUs - aTimeUs;
 * else
 *         cslPhaseUs = MAX_TIMER_VALUE - aTimeUs + sCslSampleTimeUs;
 *
 * For simplicity the formula below has been used.
 */
static uint16_t rf_compute_csl_phase(uint32_t aTimeUs)
{
    /* convert CSL Period in microseconds - it was given in 10 symbols */
    uint32_t cslPeriodUs = sCslPeriod * 10 * IEEE802154_SYMBOL_TIME_US;
    uint32_t cslPhaseUs  = (cslPeriodUs - (aTimeUs % cslPeriodUs) + (sCslSampleTimeUs % cslPeriodUs)) % cslPeriodUs;

    return (uint16_t)(cslPhaseUs / (10 * IEEE802154_SYMBOL_TIME_US) + 1);
}
#endif /* OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2 */

/*************************************************************************************************/
static void rf_abort(void)
{
    macToPlmeMessage_t msg;

    sunRxMode                            = RX_ON_IDLE_START;
    msg.msgType                          = gPlmeSetReq_c;
    msg.msgData.setReq.PibAttribute      = gPhyPibRxOnWhenIdle;
    msg.msgData.setReq.PibAttributeValue = (uint64_t)0;

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);

    msg.msgType                      = gPlmeSetTRxStateReq_c;
    msg.msgData.setTRxStateReq.state = gPhyForceTRxOff_c;

    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);
}

static void rf_set_channel(uint8_t channel)
{
    /* change channel only if it is different */
    if (channel != sChannel)
    {
        macToPlmeMessage_t msg;

        msg.msgType                          = gPlmeSetReq_c;
        msg.msgData.setReq.PibAttribute      = gPhyPibCurrentChannel_c;
        msg.msgData.setReq.PibAttributeValue = (uint64_t)channel;

        (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);

        sChannel = channel;
    }
}

static void rf_set_tx_power(int8_t tx_power)
{
    macToPlmeMessage_t msg;

    msg.msgType                          = gPlmeSetReq_c;
    msg.msgData.setReq.PibAttribute      = gPhyPibTransmitPower_c;
    msg.msgData.setReq.PibAttributeValue = (uint64_t)tx_power;

    MAC_PLME_SapHandler(&msg, ot_phy_ctx);
}

/* Used to convert from phy clock timestamp (in symbols) to platform time (in us)
   the reception timestamp which must use a true 64bit timestamp source */
static uint64_t rf_adjust_tstamp_from_phy(uint64_t ts)
{
    uint64_t now = PhyTime_ReadClock();
    uint64_t delta;

    delta = (now >= ts) ? (now - ts) : ((PHY_TMR_MAX_VALUE + now) - ts);
    delta *= IEEE802154_SYMBOL_TIME_US;

    return otPlatTimeGet() - delta;
}

static uint32_t rf_adjust_tstamp_from_ot(uint32_t time)
{
    /* The phy timestamp is in symbols so we need to convert it to microseconds */
    uint64_t ts    = PhyTime_ReadClock() * IEEE802154_SYMBOL_TIME_US;
    uint32_t delta = time - (uint32_t)otPlatTimeGet();

    return (uint32_t)(ts + delta);
}

/* Phy Data Service Access Point handler
 * Called by Phy to notify when Tx has been done or Rx data is available */
phyStatus_t PD_OT_MAC_SapHandler(void *pMsg, instanceId_t instance)
{
    pdDataToMacMessage_t *pDataMsg = (pdDataToMacMessage_t *)pMsg;
    extendedRadioFrame   *pRxFrame = NULL;

    assert(pMsg != NULL);

    PWR_DisallowDeviceToSleep();

    switch (pDataMsg->msgType)
    {
    case gPdDataCnf_c:
        /* TX activity is done */
#if OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2
        if (otMacFrameIsSecurityEnabled(&sTxFrame) && otMacFrameIsKeyIdMode1(&sTxFrame) &&
            !sTxFrame.mInfo.mTxInfo.mIsSecurityProcessed)
        {
            otMacFrameSetFrameCounter(&sTxFrame, pDataMsg->fc);
        }
#endif

        sTxStatus            = OT_ERROR_NONE;
        sState               = OT_RADIO_STATE_RECEIVE;
        sRxAckFrame.mChannel = sChannel;
        sRxAckFrame.mLength  = pDataMsg->msgData.dataCnf.ackLength;
        FLib_MemCpy(sRxAckFrame.mPsdu, pDataMsg->msgData.dataCnf.ackData, sRxAckFrame.mLength);
        sTxDone = true;
        MSG_Free(pMsg); // for Ack we can free PHY Allocated buffer
        break;

    case gPdDataInd_c:
        /* RX activity is done */
        sRxDone = true;
        OSA_InterruptDisable();
        pRxFrame = sRxRing.head;

        if (pRxFrame)
        {
            /* Retrieve frame information and data */
            pRxFrame->RxFrame.mChannel                 = sChannel;
            pRxFrame->RxFrame.mInfo.mRxInfo.mLqi       = pDataMsg->msgData.dataInd.ppduLinkQuality;
            pRxFrame->RxFrame.mInfo.mRxInfo.mRssi      = pDataMsg->msgData.dataInd.ppduRssi;
            pRxFrame->RxFrame.mInfo.mRxInfo.mTimestamp = rf_adjust_tstamp_from_phy(pDataMsg->msgData.dataInd.timeStamp);
            pRxFrame->RxFrame.mInfo.mRxInfo.mAckedWithFramePending = pDataMsg->msgData.dataInd.rxAckFp;
            pRxFrame->RxFrame.mLength                              = pDataMsg->msgData.dataInd.psduLength;
            pRxFrame->RxFrame.mPsdu                                = pDataMsg->msgData.dataInd.pPsdu;
            // keep Phy Allocated buffer and free it after OT process the packet in otPlatRadioProcess
            // after otPlatRadioReceiveDone is completed
            pRxFrame->pPhyBuffer = (void *)pMsg;

#if OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2
            pRxFrame->RxFrame.mInfo.mRxInfo.mAckedWithSecEnhAck = pDataMsg->msgData.dataInd.ackedWithSecEnhAck;
            if (true == pDataMsg->msgData.dataInd.ackedWithSecEnhAck)
            {
                pRxFrame->RxFrame.mInfo.mRxInfo.mAckFrameCounter = pDataMsg->msgData.dataInd.ackFrameCounter;
                pRxFrame->RxFrame.mInfo.mRxInfo.mAckKeyId        = pDataMsg->msgData.dataInd.ackKeyId;
            }
#endif
            // Push received frame
            PushRxRingBuffer(&sRxRing);
        }
        else
        {
            /*
             * Since RxOnIdle is stopped when ring buffer is almost full should never happen
             */
        }
        OSA_InterruptEnable();
        break;
    default:
        PWR_AllowDeviceToSleep();
        MSG_Free(pMsg);
        break;
    }

    otSysEventSignalPending();

    stop_csl_receiver();

    return gPhySuccess_c;
}

/* Phy Layer Management Entities Service Access Point handler
 * Called by Phy to notify PLME event */
phyStatus_t PLME_OT_MAC_SapHandler(void *pMsg, instanceId_t instance)
{
    plmeToMacMessage_t *pPlmeMsg = (plmeToMacMessage_t *)pMsg;

    assert(pMsg != NULL);

    PWR_DisallowDeviceToSleep();

    switch (pPlmeMsg->msgType)
    {
    case gPlmeCcaCnf_c:
        if (pPlmeMsg->msgData.ccaCnf.status == gPhyChannelBusy_c)
        {
            /* Channel is busy */
            sTxStatus = OT_ERROR_CHANNEL_ACCESS_FAILURE;
            sState    = OT_RADIO_STATE_RECEIVE;
            sTxDone   = true;
        }
        break;
    case gPlmeEdCnf_c:
        /* Scan done */
        sEdScanDone = true;
        sMaxED      = pPlmeMsg->msgData.edCnf.maxEnergyLeveldB;
        break;
    case gPlmeTimeoutInd_c:
        if (OT_RADIO_STATE_TRANSMIT == sState)
        {
            /* Ack timeout */
#if OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2
            if (otMacFrameIsSecurityEnabled(&sTxFrame) && otMacFrameIsKeyIdMode1(&sTxFrame) &&
                !sTxFrame.mInfo.mTxInfo.mIsSecurityProcessed)
            {
                otMacFrameSetFrameCounter(&sTxFrame, pPlmeMsg->fc);
            }
#endif

            sState    = OT_RADIO_STATE_RECEIVE;
            sTxStatus = OT_ERROR_NO_ACK;
            sTxDone   = true;
        }
        else if (OT_RADIO_STATE_RECEIVE == sState)
        {
            /* CSL Receive AT state has ended with timeout and we are returning to SLEEP state */
            sState = OT_RADIO_STATE_SLEEP;
            PWR_AllowDeviceToSleep();
        }
        break;
    case gPlmeAbortInd_c:
        /* TX Packet was loaded into TX Packet RAM but the TX/TR seq did not ended ok */
#if OPENTHREAD_CONFIG_THREAD_VERSION >= OT_THREAD_VERSION_1_2
        if (otMacFrameIsSecurityEnabled(&sTxFrame) && otMacFrameIsKeyIdMode1(&sTxFrame) &&
            !sTxFrame.mInfo.mTxInfo.mIsSecurityProcessed)
        {
            otMacFrameSetFrameCounter(&sTxFrame, pPlmeMsg->fc);
        }
#endif

        sState    = OT_RADIO_STATE_RECEIVE;
        sTxStatus = OT_ERROR_ABORT;
        sTxDone   = true;
        break;
    default:
        PWR_AllowDeviceToSleep();
        break;
    }
    /* The message has been allocated by the Phy, we have to free it */
    MSG_Free(pMsg);

    otSysEventSignalPending();

    stop_csl_receiver();

    return gPhySuccess_c;
}

void otPlatRadioInit(void)
{
    macToPlmeMessage_t msg;

    Phy_Init();

    ot_phy_ctx = PHY_get_ctx();

    /* Register Phy Data Service Access Point (PD_SAP) and Phy Layer Management Entities Service Access Point (PLME_SAP)
     * handlers */
    Phy_RegisterSapHandlers((PD_MAC_SapHandler_t)PD_OT_MAC_SapHandler, (PLME_MAC_SapHandler_t)PLME_OT_MAC_SapHandler,
                            ot_phy_ctx);

    msg.msgType = gPlmeEnableEncryption_c;
    (void)MAC_PLME_SapHandler(&msg, ot_phy_ctx);

    rf_set_channel(sChannel);

    sTxFrame.mLength = 0;
    /* Make the mPsdu point to the space after macToPdDataMessage_t in the data buffer. OT will put
       the data packet in this zone when requesting a data transmission */
    sTxFrame.mPsdu = sTxData + sizeof(macToPdDataMessage_t);

    ResetRxRingBuffer(&sRxRing);

    memset(&sRxAckFrame, 0, sizeof(sRxAckFrame));
    sRxAckFrame.mPsdu = sRxAckData;
}

void otPlatRadioProcess(otInstance *aInstance)
{
    if (sTxDone)
    {
        if (sTxFrame.mPsdu[IEEE802154_FRM_CTL_LO_OFFSET] & IEEE802154_ACK_REQUEST)
        {
            otPlatRadioTxDone(aInstance, &sTxFrame, &sRxAckFrame, sTxStatus);
        }
        else
        {
            otPlatRadioTxDone(aInstance, &sTxFrame, NULL, sTxStatus);
        }

        sTxDone = false;
        PWR_AllowDeviceToSleep();
    }

    if (sRxDone)
    {
        extendedRadioFrame *pRxFrmProcessing = NULL;
#if OPENTHREAD_CONFIG_DIAG_ENABLE
        if (otPlatDiagModeGet())
        {
            while ((pRxFrmProcessing = PopRxRingBuffer(&sRxRing)) != NULL)
            {
                otPlatDiagRadioReceiveDone(aInstance, pRxFrmProcessing->RxFrame, OT_ERROR_NONE);
                MSG_Free(pRxFrmProcessing->pPhyBuffer); // free PHY Allocated buffer
                pRxFrmProcessing->pPhyBuffer = NULL;
            }
        }
        else
#endif
        {
            while ((pRxFrmProcessing = PopRxRingBuffer(&sRxRing)) != NULL)
            {
                otPlatRadioReceiveDone(aInstance, &pRxFrmProcessing->RxFrame, OT_ERROR_NONE);
                MSG_Free(pRxFrmProcessing->pPhyBuffer); // free PHY Allocated buffer
                pRxFrmProcessing->pPhyBuffer = NULL;
            }
        }

        if (NAvailableRxBuffers(&sRxRing) >= 2) // at least one Rx slot should be always free
        {
            // we need to rely on this function
            // for single protocol this function instruct PHY layer that RX On Idle MUST be started
            // for multi protocol this function instruct PHY layer that RX On Idle MUST be started for this protocol
            // and any incoming messages on this protocol should be received and acknowledged
            // but is the PHY job to ensure that
            rf_rx_on_idle(RX_ON_IDLE_START); // restart rx on idle
        }

        sRxDone = false;
        PWR_AllowDeviceToSleep();
    }

    if (sEdScanDone)
    {
        otPlatRadioEnergyScanDone(aInstance, sMaxED);
        sEdScanDone = false;
        PWR_AllowDeviceToSleep();
    }
}

static void rf_rx_on_idle(uint32_t newValue)
{
    macToPlmeMessage_t msg;
    phyStatus_t        phy_status;

    newValue %= 2;
    if (sunRxMode != newValue)
    {
        sunRxMode                            = newValue;
        msg.msgType                          = gPlmeSetReq_c;
        msg.msgData.setReq.PibAttribute      = gPhyPibRxOnWhenIdle;
        msg.msgData.setReq.PibAttributeValue = (uint64_t)sunRxMode;

        phy_status = MAC_PLME_SapHandler(&msg, ot_phy_ctx);

        assert(phy_status == gPhySuccess_c);
    }
}

static void ResetRxRingBuffer(rxRingBuffer *aRxRing)
{
    memset(aRxRing, 0, sizeof(rxRingBuffer));
    aRxRing->head = aRxRing->extRxFrame;
    aRxRing->tail = aRxRing->extRxFrame;
}

static void PushRxRingBuffer(rxRingBuffer *aRxRing)
{
    OSA_InterruptDisable();

    // increment head
    aRxRing->head++;

    // check were are still in place or need roll-over
    if (aRxRing->head >= aRxRing->extRxFrame + NMAX_RXRING_BUFFERS)
        aRxRing->head -= NMAX_RXRING_BUFFERS;

    // check available slots
    unsigned int h = aRxRing->head - aRxRing->extRxFrame;
    unsigned int t = aRxRing->tail - aRxRing->extRxFrame;

    assert(h < NMAX_RXRING_BUFFERS);
    assert(t < NMAX_RXRING_BUFFERS);

    // if available slots more than one is ok else (i.e. <=1 ) stop rx
    h += ((h >= t) ? 0 : NMAX_RXRING_BUFFERS);
    if (NMAX_RXRING_BUFFERS - (h - t) <= 1)
    {
        // we need to rely on this function
        // for single protocol this function instruct PHY layer that RX MUST be stopped
        // for multi protocol this function instruct PHY layer that RX MUST be stopped for this protocol
        // and any incoming messages on this protocol should not be received and/or acknowledged
        // but is the PHY job to enforce & ensure that
        rf_rx_on_idle(RX_ON_IDLE_STOP); // stop rx on idle
    }

    OSA_InterruptEnable();
}

static extendedRadioFrame *PopRxRingBuffer(rxRingBuffer *aRxRing)
{
    extendedRadioFrame *rxFrame = NULL;

    OSA_InterruptDisable();
    if (!IsEmptyRxRingBuffer(aRxRing))
    {
        rxFrame = aRxRing->tail;
        aRxRing->tail++;
        // check were are still in place or need roll-over
        if (aRxRing->tail >= aRxRing->extRxFrame + NMAX_RXRING_BUFFERS)
            aRxRing->tail -= NMAX_RXRING_BUFFERS;
    }
    OSA_InterruptEnable();

    return rxFrame;
}

static bool IsEmptyRxRingBuffer(rxRingBuffer *aRxRing)
{
    return (aRxRing->head == aRxRing->tail);
}

static unsigned char NAvailableRxBuffers(rxRingBuffer *aRxRing)
{
    OSA_InterruptDisable();
    unsigned char N = 0;
    unsigned int  h = aRxRing->head - aRxRing->extRxFrame;
    unsigned int  t = aRxRing->tail - aRxRing->extRxFrame;

    assert(h < NMAX_RXRING_BUFFERS);
    assert(t < NMAX_RXRING_BUFFERS);

    h += ((h >= t) ? 0 : NMAX_RXRING_BUFFERS);
    N = NMAX_RXRING_BUFFERS - (h - t);

    assert(N <= NMAX_RXRING_BUFFERS);
    OSA_InterruptEnable();

    return N;
}
