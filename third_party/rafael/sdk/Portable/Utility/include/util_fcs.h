/**
 * @file util_fcs.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
* @defgroup FCS FCS API Definition
* Define FCS definitions, structures, and functions.
* @ingroup Utility
* @{
*/

#ifndef FCS_H
#define FCS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Calculate the FCS of the given buffer.
 *
 * @param len Length of the buffer.
 * @param pBuf Buffer to compute the CRC.
 * @return uint8_t FCS value.
 */
uint8_t FcsCalc(uint32_t len, const uint8_t *pBuf);


/**
 * @brief Computes resultant CRC by appending one byte.
 *
 * @param pFcs CRC value on which to append the byte
 * @param byte Byte to be added to CRC computation.
 */
void FcsAddByte(uint8_t *pFcs, uint8_t byte);


/** @} */
#ifdef __cplusplus
};
#endif

#endif /* FCS_H */
