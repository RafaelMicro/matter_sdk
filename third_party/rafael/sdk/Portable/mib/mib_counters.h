// ---------------------------------------------------------------------------
// Copyright (c) 2021,
// All rights reserved.
//
// ---------------------------------------------------------------------------
#ifndef __MIB_COUNTER_H__
#define __MIB_COUNTER_H__

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
//                Include (Better to prevent)
//=============================================================================
#include <stdint.h>

//=============================================================================
//                Public Definitions of const value
//=============================================================================

//=============================================================================
//                Public ENUM
//=============================================================================
typedef enum MIB_COUNTER_TYPE
{

    ThreadRxDoneCount,
    ThreadTrigTxDoneCount,
    ThreadTxDoneCount,
    ThreadTXRequestCount,
    ThreadRxNotifyCount,

    RFBTxSentCount,
    RFMCUWriteTxCount,

    RfFwCtrlTxWriteCount,
    RfFwCtrlTxDoneCount,

    RfIsrTxDoneCount,

} mib_type_t;

//=============================================================================
//                Public Struct
//=============================================================================

//=============================================================================
//                Public Function Declaration
//=============================================================================
void mib_counter_clear(void);
void mib_counter_increase(mib_type_t t_type);
void mib_counter_printf(void);

#ifdef __cplusplus
};
#endif
#endif /* __MIB_COUNTER_H__ */
