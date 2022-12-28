/**************************************************************************//**
 * @file     ringbuffer.h
 * @version
 * @brief    ring buffer API
 *
 * @copyright
 ******************************************************************************/



#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#define bufsize_mask(X)    (X-1)

typedef struct
{
    uint8_t  *ring_buf;
    uint16_t  bufsize_mask;     /*buffer size should be 2^N, this value is buffersize-1 */
    volatile uint16_t  wr_idx;  /*Notice: this write index must be volatile*/
    volatile uint16_t  rd_idx;  /*Notice: this read index must be volatile*/
} ring_buf_t;

#endif
