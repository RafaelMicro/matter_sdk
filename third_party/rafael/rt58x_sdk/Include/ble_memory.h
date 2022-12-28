// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
#ifndef __BLE_MEMORY_H__
#define __BLE_MEMORY_H__

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
//                Include (Better to prevent)
//=============================================================================
#include "host_management.h"

typedef struct MQUEUE
{
    MBLK *QIn;
    MBLK *QOut;
} MQUEUE;

typedef struct BufProcess
{
    MBLK *BufPrcsF;
    MBLK *BufPrcsN;
} BufProcess;

typedef struct
{
    MBLK    *pDst;
    uint8_t *pSrc;
    uint16_t total_length;
    uint16_t src_length;
    uint16_t received_length;
} get_acl_data_t;

void ble_memory_init(void);
MBLK *get_msgblks(uint16_t length);
MBLK *get_msgblks_L1(uint16_t length);
MBLK *get_msgblks_L2(uint16_t length);
int8_t check_msgblk_L1_size(uint16_t length);
int8_t check_msgblk_L2_size(uint16_t length);
void msgblks_free(MBLK *pMsgBlk);
void acl_data_get(get_acl_data_t *p_param);
void get_acl_data_from_msgblks(uint8_t *pDst, MBLK *pSrc);
void acl_data2msgblk(MBLK *pDst, uint8_t *p_data, uint16_t length);
MBLK *acl_data2msgblk_L2(uint8_t *p_data, uint16_t length);

#ifdef __cplusplus
};
#endif

#endif    /* __BLE_MEMORY_H__ */
