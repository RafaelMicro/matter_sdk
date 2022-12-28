/**************************************************************************//**
 * @file     CRYPTO.h
 * @version
 * @brief    CRYPTO ACCELEATOR driver header file
 *
 * @copyright
 ******************************************************************************/

#ifndef ___CRYPTO_H__
#define ___CRYPTO_H__

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "project_config.h"
#include "cm3_mcu.h"

#if  (MODULE_ENABLE(CRYPTO_FreeRTOS_ISR_ENABLE) || MODULE_ENABLE(SUPPORT_MULTITASKING))

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

#endif

#include "aes.h"

#include "ecc.h"

#include "sha256.h"

#include "hmac_drbg.h"

#include "crypto_util.h"

#if MODULE_ENABLE(CRYPTO_SECP256R1_ENABLE)
#include "ecjpake.h"
#endif

#define   AES_FIRMWARE              0x000000AE
#define   ECC_FIRMWARE              0xECC00000
#define   ECC_FIRMWARE_VER          0xECC50000
#define   ECC_FIRMWARE_SIG          0xECC80000
#define   ECC_FIRMWARE_C25519       0xECCC0000
#endif

