/**************************************************************************//**
 * @file     assert_help.h
 * @version
 * @brief    assert help to check input parameter is valid or not. (DEBUG Build)
 *
 * @copyright
 ******************************************************************************/

#ifndef ___ASSERT_H__
#define ___ASSERT_H__

#ifdef  DEBUG

#include <stdint.h>
#include "stdio.h"

#define dprintf(msg, ...)  do { printf(msg, __VA_ARGS__); }while(0)
#define dprintf0(msg)     do { printf(msg);}while(0)

#define assert_param(expr)    \
        do {                  \
            if(expr)          \
            {                 \
            }                 \
            else              \
            {                 \
                printf("ASSERT Failed in File %s line %d \n", __FILE__, __LINE__ ); \
                while(1);   \
            }                 \
        } while(0)

#else
#define assert_param(expr)     ((void)0)
#define dprintf(msg, ...)      ((void)0)
#define dprintf0(msg)          ((void)0)
#endif

#endif
