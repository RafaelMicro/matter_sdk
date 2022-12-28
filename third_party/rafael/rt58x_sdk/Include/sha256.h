/**************************************************************************//**
 * @file     sha256.h
 * @version
 * @brief    sha256 driver header file
 *
 * @copyright
 ******************************************************************************/
/** @defgroup    SHA256_Driver   SHA256 Driver
*   @ingroup     peripheral_group
*   @brief  SHA256 driver header file
*   @{
*
*
*/
#ifndef _INCLUDE_SHA256_DEFINE_
#define _INCLUDE_SHA256_DEFINE_
/**************************************************************************************************
*    CONSTANTS AND DEFINES
*************************************************************************************************/
#define  SHA256_DIGEST_SIZE                32
#define  SHA256_BLOCK_SIZE                 64

#define  ROM_START_FUN_ADDR        0x10000024
#define  ROM_UPDATE_FUN_ADDR       0x10000020
#define  ROM_FINISH_FUN_ADDR       0x1000001C

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/**
* @brief          The SHA-256 context structure.
*
*                 The structure is used both for SHA-256
*                 checksum calculations.
*/
typedef struct
{
    uint32_t total[2];      /*!< The number of Bytes processed.  */
    uint32_t state[8];      /*!< The intermediate digest state.  */
    uint8_t buffer[64];     /*!< The data block being processed. */
} sha256_context;

/**
* @brief          The HMAC-SHA256 context structure.
*
*                 The structure is used for HMAC  based on SHA-256
*                 caculation
*
*/
typedef struct
{
    uint32_t total[2];          /*!< The number of Bytes processed.  */
    uint32_t state[8];          /*in fact, this variable is useless...*/

    uint8_t  buffer[SHA256_BLOCK_SIZE];    /*!< The data block being processed. */
    uint8_t  opad[SHA256_BLOCK_SIZE];      /*!< opad data. Notice this data should be clean after finish hmac operation. */
} hmac_sha256_context;



typedef struct pbkdf2_struct
{
    uint8_t   *password;           /*!< pbkdf2 password */
    uint32_t  password_length;     /*!< pbkdf2 password  length*/

    uint8_t   *salt;               /*!< pbkdf2 salt*/
    uint32_t  salt_length;         /*!< pbkdf2 salt  length*/

    uint32_t  iteration;           /*!< pbkdf2 iteration  --- according matter, this value must between 1000~100000*/

    uint32_t  key_length;          /*!< pbkdf2 output key length*/
    uint8_t   *key_output;         /*!< pbkdf2 generted key output*/

} pbkdf2_st;


typedef void (*sha256_starts_rom_t)(sha256_context *ctx);
typedef void (*sha256_update_rom_t)(sha256_context *ctx, uint8_t *input, uint32_t length);
typedef void (*sha256_finish_rom_t)(sha256_context *ctx, uint8_t digest[SHA256_DIGEST_SIZE]);


extern sha256_starts_rom_t    sha256_init;
extern sha256_update_rom_t    sha256_update;
extern sha256_finish_rom_t    sha256_finish;


#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

extern void sha256_vector_init(void);

/**
 * @brief          Generate Keyed-Hash Message Authentication code (HMAC)
 *
 * @param[in]  key           key buffer for HMAC
 * @param[in]  key_length    The length of the key buffer
 * @param[in]  msg           The buffer to be caculated HMAC tag
 * @param[in]  msg_length    The length of msg buffer
 * @param[out] output        The buffer for HMAC result.
 *
 * @details
 *       The algorithm of this HMAC is based on FIPS 198-1  or RFC 4231 HMAC-SHA-256
 *       This function will create a sha256_context and two 64 bytes
 *       key (ipad key and opad key) inside the function stack, so this function will
 *       use more than 264 bytes. So please be careful for the stack size allocation.
 *
 *
 */
extern uint32_t hmac_sha256(const uint8_t *key,
                            uint32_t       key_length,
                            const uint8_t *msg,
                            uint32_t       msg_length,
                            uint8_t       *output);



extern uint32_t hmac_sha256_init_ex(hmac_sha256_context  *cntx,
                                    const uint8_t   *key, uint32_t key_length);
/**
 * @brief     Generate hmac based sha256
 *
 *
 *
 *
 * @details
 *
 *
 */

extern uint32_t hkdf_sha256(uint8_t *out_key, uint32_t out_len,
                            const uint8_t *secret, uint32_t secret_len,
                            const uint8_t *salt, uint32_t salt_len,
                            const uint8_t *info, uint32_t info_len);


/**
 * @brief     Generate pbkdf2_hmac
 *
 * @param     pbkdf_vector  pointer of struct pbkdf2_struct
 *                           password is defined in member "password", and it's length is password_length.
 *                           salt is defined in member "salt", and it's length is salt_length length.
 *                           key_output is output result buffer, and it's length is key_length
 *
 * @details
 *       pbkdf2_hmac is based the NIST 800-132 section 5.3
 *       Notice: This function is not necessary for matter..
 *
 */
extern uint32_t pbkdf2_hmac(pbkdf2_st  *pbkdf_vector);

#ifdef __cplusplus
}
#endif


#endif /* end of _INCLUDE_SHA256_DEFINE_ */

/*@}*/ /* end of peripheral_group SHA256_Driver */


