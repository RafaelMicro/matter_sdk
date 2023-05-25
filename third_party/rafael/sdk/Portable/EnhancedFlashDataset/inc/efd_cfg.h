
#ifndef EFD_CFG_H_
#define EFD_CFG_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "chip_define.h"

/* using ENV function, default is NG (Next Generation) mode start from V4.0 */
#define EFD_USING_ENV
#ifdef EFD_USING_ENV
/* Auto update ENV to latest default when current ENV version number is changed. */
/* #define EFD_ENV_AUTO_UPDATE */
/**
 * ENV version number defined by user.
 * Please change it when your firmware add a new ENV to default_env_set.
 */
#define EFD_ENV_VER_NUM 0 /* @note you must define it for a value, such as 0 */

#endif                   /* EFD_USING_ENV */

#define EFD_STR_ENV_VALUE_MAX_SIZE (128)
#define EFD_ENV_NAME_MAX (64)

/* using IAP function */
/* #define EFD_USING_IAP */

/* using save log function */
/* #define EFD_USING_LOG */

/* The minimum size of flash erasure. May be a flash sector size. */
#define EFD_ERASE_MIN_SIZE (0x1000) /* @note you must define it for a value */

/* the flash write granularity, unit: bit
 * only support 1(nor flash)/ 8(stm32f4)/ 32(stm32f1) */
#define EFD_WRITE_GRAN (8) /* @note you must define it for a value */

/* The size of read_env and continue_ff_addr function used*/
#define EFD_READ_BUF_SIZE                                                                                                           \
    32 /* @default 32, Larger numbers can improve first-time speed of alloc_env but require more stack                             \
          space*/

#if (RAFAEL_BOARD == RT582)
/* backup area start address */
#define EFD_START_ADDR          0xF3000 /* @note you must define it for a value */
/* ENV area size. It's at least one empty sector for GC. So it's definition must more then or equal 2 flash sector size. */
#define ENV_AREA_SIZE           0x8000 /* @note you must define it for a value if you used ENV */
#define EFD_GC_EMPTY_SEC_THRESHOLD 2
#elif (RAFAEL_BOARD == RT583)
#define EFD_START_ADDR          0x1D0000
/* ENV area size. It's at least one empty sector for GC. So it's definition must more then or equal 2 flash sector size. */
#define ENV_AREA_SIZE           0x20000 /* @note you must define it for a value if you used ENV */
#define EFD_GC_EMPTY_SEC_THRESHOLD 8
#endif

/* print debug information of flash */
// #define PRINT_DEBUG

#endif /* EFD_CFG_H_ */
