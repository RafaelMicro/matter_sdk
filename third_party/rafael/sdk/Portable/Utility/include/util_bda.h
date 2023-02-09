/**
 * @file util_bda.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
* @defgroup BDA BDA API Definition
* Define BDA definitions, structures, and functions.
* @ingroup Utility
* @{
*/

#ifndef BDA_H
#define BDA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "util.h"

/**
 * @brief  BD address length
 *
 */
#define BDA_ADDR_LEN                  6

/**
 * @brief  BD address string length
 *
 */
#define BDA_ADDR_STR_LEN              (BDA_ADDR_LEN * 2)

/**
 * @brief BDA RPA check
 *
 */
#define BDA_ADDR_IS_RPA(bda)          (((bda)[5] & 0xC0) == 0x40)

/**
 * @brief BDA NRPA check
 *
 */
#define BDA_ADDR_IS_NRPA(bda)         (((bda)[5] & 0xC0) == 0x00)

/**
 * @brief BDA static random check
 *
 */
#define BDA_ADDR_IS_STATIC(bda)       (((bda)[5] & 0xC0) == 0xC0)

/**
 * @brief BDA64 RPA check
 *
 */
#define BDA64_ADDR_IS_RPA(bda64)      ((((bda64) >> 40) & 0xC0) == 0x40)

/**
 * @brief BDA64 NRPA check
 *
 */
#define BDA64_ADDR_IS_NRPA(bda64)     ((((bda64) >> 40) & 0xC0) == 0x00)

/**
 * @brief BDA64 static random check
 *
 */
#define BDA64_ADDR_IS_STATIC(bda64)   ((((bda64) >> 40) & 0xC0) == 0xC0)

/**
 * @brief BD address data type
 *
 */
typedef uint8_t bdAddr_t[BDA_ADDR_LEN];

/**
 * @brief Copy a BD address from source to destination.
 *
 * @param pDst Pointer to destination.
 * @param pSrc Pointer to source.
 */
void BdaCpy(uint8_t *pDst, const uint8_t *pSrc);

/**
 * @brief Compare two BD addresses.
 *
 * @param pAddr1 First address.
 * @param pAddr2 Second address.
 * @return bool_t TRUE if addresses match, FALSE otherwise.
 */
bool_t BdaCmp(const uint8_t *pAddr1, const uint8_t *pAddr2);

/**
 * @brief Set a BD address to all zeros.
 *
 * @param pDst Pointer to destination.
 * @return uint8_t* pDst + BDA_ADDR_LEN
 */
uint8_t *BdaClr(uint8_t *pDst);

/**
 * @brief Check if a BD address is all zeros.
 *
 * @param pAddr Pointer to address.
 * @return bool_t TRUE if address is all zeros, FALSE otherwise.
 */
bool_t BdaIsZeros(const uint8_t *pAddr);

/**
 * @brief Convert a BD address to a string.
 *
 * @param pAddr Pointer to BD address.
 * @return char* Pointer to string.
 */
char *Bda2Str(const uint8_t *pAddr);

/** @} */

#ifdef __cplusplus
};
#endif

#endif /* BDA_H */
