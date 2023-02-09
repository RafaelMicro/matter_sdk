/**
 * @file util_string.h
 * @author Rex Huang (rex.huang@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-03-17
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
* @defgroup String String handler API Definition
* Define String handler definitions, structures, and functions.
* @ingroup Utility
* @{
*/

#ifndef __util_string_H_wvsMcgCi_lk2Z_HajD_syQf_u1Yfwot7R8PD__
#define __util_string_H_wvsMcgCi_lk2Z_HajD_syQf_u1Yfwot7R8PD__

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>

/**
 * @brief               Convents the initial part of the string in str to a long int value.
 *
 * @param str           This is the string containing the representation of an integral number.
 * @param ep            This is the reference to an object of type char*, whose value is set by the function to the next
 *                      character in str after the numerical value.
 * @return long         The converted integral number as a long int value, else zero values is returned.
 */
long utility_strtol(const char *str, char **ep);

/**
 * @brief                   Convert string to unsigned long integer.
 *
 * @param str               C-String containing the representation of an integral number.
 * @param ep                This is the reference to an object of type char*, whose value is set by the function to the next
 *                          character in str after the numerical value.
 * @return unsigned long    The converted integral number as an unsigned long int value, else zero values is returned.
 */
unsigned long utility_strtoul(const char *str, char **ep);

/**
 * @brief                   Convert string to hexadecimal value.
 *
 * @param str               C-String containing the representation of a hexadecimal value.
 * @param ep                This is the reference to an object of type char*, whose value is set by the function to the next
 *                          character in str after the numerical value.
 * @return unsigned long    The converted integral number as an unsigned long hexadecimal value, else zero values is returned.
 */
unsigned long utility_strtox(const char *str, char **ep);

/**
 * @brief               Convert a integer value to a null-terminated string.
 *
 * @param a             Pointer of the string.
 * @param len           String len.
 * @param l             Value to be converted to a string.
 * @param w_sign        Show the signed of the intger.
 */
void utility_ltoa(char *a, unsigned long *len, long l, bool w_sign);

/**
 * @brief               Convert an unsigned integer value to a null-terminated string.
 *
 * @param a             Pointer of the string.
 * @param len           String len.
 * @param u             Value to be converted to a string.
 */
void utility_ultoa(char *a, unsigned long *len, unsigned long u);

/**
 * @brief               Convert a hexadecimal number to a null-terminated string.
 *
 * @param a             Pointer of the string.
 * @param len           String len.
 * @param x             Value to be converted to a string.
 * @param capitalized   True of big.
 */
void utility_xtoa(char *a, unsigned long *len, unsigned long x, bool capitalized);

/**
 * @brief               Get string length
 *
 * @param s             C string.
 * @return long         The length of string.
 */
long utility_strlen(const char *s);

/**
 * @brief                   Convert a float number to a null-terminated string.
 *
 * @param a                 Pointer of the string.
 * @param len               String len.
 * @param f                 Value to be converted to a string.
 * @param frc_precision     Precision of th value.
 */
void utility_ftoa(char *a, unsigned long *len, double f, unsigned long frc_precision);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
