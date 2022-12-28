/**
 * @file util.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __util_H_w8DPG1Gy_liGX_H8Ru_sv7X_uAObZ0b9bP8Q__
#define __util_H_w8DPG1Gy_liGX_H8Ru_sv7X_uAObZ0b9bP8Q__

#ifdef __cplusplus
extern "C" {
#endif


#include <string.h>
#include "types.h"
#include "util_bstream.h"
//=============================================================================
//                  Constant Definition
//=============================================================================

//=============================================================================
//                  Macro Definition
//=============================================================================


#define __MIN__(a,b)        ((a) < (b) ? (a) : (b))

#define __MAX__(a,b)        ((a) > (b) ? (a) : (b))

#define BE2LE16(n) ((((n) << 8) & 0xff00) | (((n) >> 8) & 0x00ff))  /**< Converts a 16-bit big-endian number to little-endian. */
#define LE2BE16(n) BE2LE16(n)                                       /**< Converts a 16-bit little-endian number to big-endian. */
#define BE2LE32(n) LE2BE32(n)                                       /**< Converts a 32-bit big-endian number to little-endian. */
#define LE2BE32(n) (((uint32_t) (n) << 24) | (((uint32_t) (n) << 8) & 0xff0000) \
        | (((n) >> 8) & 0xff00) | ((n) >> 24))

#define LE2BE24(n) ((((n) & 0x00FF0000) >> 16) |        \
                    ( (n) & 0x0000FF00)        |        \
                    (((n) & 0x000000FF) << 16))

#define BE2LE24(n) LE2BE24(n)

#define U16_HIGHBYTE(x)                 (uint8_t)((x >> 8) & 0xFF)
#define U16_LOWBYTE(x)                  (uint8_t)(x & 0xFF)

#define DEFINE_BFIELD(bit_size)                         \
    typedef struct bfield_ {                            \
        unsigned short bits_field[((bit_size) + 0xF) >> 4]; \
    } bfield_t;

#define BFIELD_SET(pBSet, bit_order)        ((pBSet)->bits_field[(bit_order) >> 4] |=  (1 << ((bit_order) & 0xF)))
#define BFIELD_CLR(pBSet, bit_order)        ((pBSet)->bits_field[(bit_order) >> 4] &= ~(1 << ((bit_order) & 0xF)))
#define BFIELD_IS_SET(pBSet, bit_order)     ((pBSet)->bits_field[(bit_order) >> 4] &   (1 << ((bit_order) & 0xF)))
#define BFIELD_ZERO(pBSet)                  memset((void*)(pBSet), 0x0, sizeof(bfield_t))

#define bit_field_t                         bfield_t

#define _stringize(s)                       #s
#define util_to_string(a)                 _stringize(a)

#ifndef CEIL_DIV
#define CEIL_DIV(A, B)   (((A) + (B) - 1) / (B))
#endif
//=============================================================================
//                  Structure Definition
//=============================================================================

//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================

//=============================================================================
//                  Public Function Definition
//=============================================================================

#ifdef __cplusplus
}
#endif

#endif
