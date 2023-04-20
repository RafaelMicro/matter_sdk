/**
 * Copyright (c) 2021 All Rights Reserved.
 */
/** @file mib_counter.c
 *
 * @author
 * @version
 * @date
 * @license
 * @description
 */

//=============================================================================
//                Include
//=============================================================================
#include "mib_counters.h"
#include "sys_arch.h"

//=============================================================================
//                Private Definitions of const value
//=============================================================================

//=============================================================================
//                Private ENUM
//=============================================================================

//=============================================================================
//                Private Struct
//=============================================================================
typedef struct MESH_MIB_COUNTER
{
    uint32_t ThreadRxDoneCount;
    uint32_t ThreadTrigTxDoneCount;
    uint32_t ThreadTxDoneCount;
    uint32_t ThreadTXRequestCount;
    uint32_t ThreadRxNotifyCount;

    uint32_t RFBTxSentCount;
    uint32_t RFMCUWriteTxCount;

    uint32_t RfFwCtrlTxWriteCount;
    uint32_t RfFwCtrlTxDoneCount;

    uint32_t RfIsrTxDoneCount;

} mesh_mib_t;

//=============================================================================
//                Private Function Declaration
//=============================================================================

//=============================================================================
//                Private Global Variables
//=============================================================================
static mesh_mib_t g_mesh_mib;

//=============================================================================
//                Public Global Variables
//=============================================================================

//=============================================================================
//                Private Definition of Compare/Operation/Inline funciton/
//=============================================================================

//=============================================================================
//                Functions
//=============================================================================
#define GET_MIB(type) g_mesh_mib.type
#define MIB_UPDATE(type)                                                                                                           \
    case type:                                                                                                                     \
        GET_MIB(type)++;                                                                                                           \
        break

void mib_counter_clear(void)
{
    memset(&g_mesh_mib, 0x0, sizeof(g_mesh_mib));
}

void mib_counter_increase(mib_type_t t_type)
{
    switch (t_type)
    {
        MIB_UPDATE(ThreadRxDoneCount);
        MIB_UPDATE(ThreadTrigTxDoneCount);
        MIB_UPDATE(ThreadTxDoneCount);
        MIB_UPDATE(ThreadTXRequestCount);
        MIB_UPDATE(ThreadRxNotifyCount);

        MIB_UPDATE(RFBTxSentCount);
        MIB_UPDATE(RFMCUWriteTxCount);

        MIB_UPDATE(RfFwCtrlTxWriteCount);
        MIB_UPDATE(RfFwCtrlTxDoneCount);

        MIB_UPDATE(RfIsrTxDoneCount);

    default:
        break;
    }
}

void mib_counter_printf(void)
{
    SYS_PRINTF(DEBUG_INFO, "\nRFB Event:     Total        \n");
    SYS_PRINTF(DEBUG_INFO, "    Rx Done    %5u\n", GET_MIB(ThreadRxDoneCount));
    SYS_PRINTF(DEBUG_INFO, "    Tx Done    %5u\n", GET_MIB(ThreadTxDoneCount));

    SYS_PRINTF(DEBUG_INFO, "\nThread Proc:   Total        \n");
    SYS_PRINTF(DEBUG_INFO, "    Rx Com    %5u\n", GET_MIB(ThreadRxNotifyCount));
    SYS_PRINTF(DEBUG_INFO, "    Tx Req    %5u\n", GET_MIB(ThreadTXRequestCount));
    SYS_PRINTF(DEBUG_INFO, "    Tx Com    %5u\n", GET_MIB(ThreadTrigTxDoneCount));

    SYS_PRINTF(DEBUG_INFO, "\nRFB:           Total\n");
    SYS_PRINTF(DEBUG_INFO, "    Tx Req    %5u\n", GET_MIB(RFBTxSentCount));

    SYS_PRINTF(DEBUG_INFO, "\nRFFW:          Total\n");
    SYS_PRINTF(DEBUG_INFO, "    Tx Req    %5u\n", GET_MIB(RfFwCtrlTxWriteCount));
    SYS_PRINTF(DEBUG_INFO, "    Tx Com    %5u\n", GET_MIB(RfFwCtrlTxDoneCount));

    SYS_PRINTF(DEBUG_INFO, "\nRFISR:         Total\n");
    SYS_PRINTF(DEBUG_INFO, "    Tx Done   %5u\n", GET_MIB(RfIsrTxDoneCount));
}
