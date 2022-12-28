/**************************************************************************//**
 * @file     aes.h
 * @version
 * @brief    AES driver header file
 *
 * @copyright
 ******************************************************************************/
/** @defgroup    AES_Driver   AES Driver
 *  @ingroup     peripheral_group
 *  @brief  AES driver header file
 *  @{
 *  @details
 *   AES driver for ECB encryption/decryption. This driver also supports AES CCM mode.
 *   Please notice this driver is designed for single thread driver, if your system is
 *   multi-thread OS, like FreeRTOS, you need define the following setting in project_config.h
 *
 *        #define SUPPORT_MULTITASKING               1
 *
 *   And you should implement these two functions
 *
 *   void crypto_mutex_lock(void);
 *   void crypto_mutex_unlock(void);
 *
 *   to protect the accelerator resource only serivice one request.
 *   We do NOT implement it, because these two functions are OS-dependent.
 *
*/

#ifndef ___AES_H_
#define ___AES_H_

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/

#include <stdint.h>

#include "status.h"

#ifdef __cplusplus
extern "C"
{
#endif
/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/

/**
 *  @brief AES BLOCK LENGTH Constant Definitions
 */
#define AES_BLOCKLEN              16     /*!< Block length in bytes, AES is 128bits per block  */

/**
 *  @brief AES_KEY SIZE Constant Definitions
 */
#define  AES_KEY128               128    /*!< AES key size is 128bits   */
#define  AES_KEY192               192    /*!< AES key size is 192bits   */
#define  AES_KEY256               256    /*!< AES key size is 256bits   */


#define  NONCE_LEN                13     /*!< AES CCM NONCE LENGTH (most used length in application)  */



#define AES_Round_Key_Size        240    /*!< for AES256, the maximum round key size is 240. */

#define AES_keyExpSize            AES_Round_Key_Size

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**
 *@brief  AES ENCRYPTION DECRYPTION operation definitions.
 */

typedef  enum
{
    AES_CRYPTO_DECRYPT = 0,     /*!< AES operation for decryption   */
    AES_CRYPTO_ENCRYPT = 1,     /*!< AES operation for encryption   */
} crypto_operation_t;


/**
 *  @brief  Structure for the AES Context definition.
 *
 *  @details  This structure will use more than 256 bytes, so please be careful for the stack size.
 *
 */

struct aes_ctx
{
    uint8_t RoundKey[AES_keyExpSize];       /*!< AES roud key */

    uint8_t Iv[AES_BLOCKLEN];               /*!< AES initialization vecotr buffer*/

    uint8_t round;                          /*!< The number of rounds */
};
/**
 *@brief  Structure for the AES CCM packet structure definition.
 *
 *@details  This aes_ccm_encryption_packet only supports length field is 2 bytes
 *
 *           Please notice: hdr_len+data_len must be less than 480 bytes.
 *
 */
struct aes_ccm_encryption_packet
{
    uint8_t   *nonce;      /*!< Nonce N -- Here it must be 13 bytes.*/

    uint8_t   *hdr;        /*!< Additional data authenticated but not encrypted*/
    uint32_t  hdr_len;     /*!< data length for authenticated but not encrypted*/

    uint8_t   *data;       /*!< message to be encrypted*/
    uint32_t  data_len;    /*!< data length for message to be encrypted */

    uint32_t  mlen;        /*!< mac length, valid value are 4,6,8,10,12,14,16 */

    uint8_t   *out_buf;       /*!< ccm output message buffer*/
    uint32_t  *out_buf_len;   /*!< number bytes of ccm output message buffer.*/

};


/**
 *@brief  Structure for the AES CCM decryption packet structure definition.
 *
 *@details  This aes_ccm_decryption_packet only supports length field is 2 bytes
 *
 *           Please notice: hdr_len+data_len must be less than 480 bytes.
 *
 */

struct aes_ccm_decryption_packet
{
    uint8_t   *payload_buf;      /*!< ccm payload buffer for decryption and verification*/
    uint32_t  payload_length;    /*!< ccm payload buffer bytes length */

    uint8_t   *nonce;            /*!< Nonce N -- Here it must be 13 bytes.*/

    uint32_t  hdr_len;           /*!< data length for authenticated but not encrypted*/

    uint32_t  data_len;          /*!< data length for message to be encrypted */

    uint32_t  mlen;              /*!< mac length, valid value are 4,6,8,10,12,14,16 */

    uint8_t   *out_buf;          /*!< ccm output message buffer for decryption */
    uint32_t  *out_buf_len;      /*!< number bytes of ccm output message buffer of decryption.*/

};
/**************************************************************************************************
*    GLOBAL PROTOTYPES
*************************************************************************************************/



/**
 * @brief Init AES  key expansion
 *
 * @param[in]  ctx        The AES context to initialize.
 * @param[in]  key        key for encryption or decryption
 * @param[in]  keybits    must be AES_KEY128, AES_KEY192, or AES_KEY256
 *
 * @return    NONE
 *
 * @details
 *       This is software function to expansion aes key to aes round key.
 *       Please be notice expansion key size is 240 bytes in CTX.
 *       Save expansion key in CTX, so system can switch different AES context for different connection.
 *
 *       Please also notice that ECC and AES using the same crypto accelerator, so only one
 *       hardware function can be used at anytime. And if you want to switch ECC to AES or AES to ECC,
 *       You should reload related firmware.
 *
 */
extern uint32_t aes_key_init(struct aes_ctx *ctx, const uint8_t *key, uint32_t keybits);

/**
 * @brief Load AES expansion round key to engine
 *
 * @param[in]  ctx        The AES context
 *
 * @return
 * @retval      STATUS_SUCCESS
 * @retval      STATUS_INVALID_PARAM --- keybits must be 128/192/256 bits only.
 *
 * @details
 *     Load expansion round key to crypto accelerator. It is possible that one system has many AES context
 *     (for different connection).
 *     System can switch round key for each connection by calling this function.
 */

extern void aes_load_round_key(struct aes_ctx *ctx);

/**
 * @brief This function performs an AES single-block encryption operation
 *
 * @param[in]  ctx        The AES context to use for encryption
 * @param[in]  in_buf     The 16-byte buffer holding the input data.
 * @param[out] out_buf    The 16-byte buffer holding the output data
 *
 * @return
 * @retval      STATUS_SUCCESS
 * @retval      STATUS_INVALID_PARAM --- ctx is NULL.
 *
 * @details
 *       It performs the encryption operation on input data buffer defined in the in_buf
 *       This function is block function. that is, this function will return when
 *       crypto engine finish the encryption.
 *
 *
 *
 */
extern uint32_t aes_ecb_encrypt(struct aes_ctx *ctx, uint8_t *in_buf, uint8_t *out_buf);


/**
 * @brief This function performs an AES single-block decryption operation
 *
 * @param[in]  ctx        The AES context to use for decryption
 * @param[in]  in_buf     The 16-byte buffer holding the input data.
 * @param[out] out_buf    The 16-byte buffer holding the output data
 *
 * @return
 * @retval      STATUS_SUCCESS
 * @retval      STATUS_INVALID_PARAM --- ctx is NULL.
 *
 * @details
 *       It performs the decryption operation on input data buffer defined in the in_buf
 *       This function is block function. that is, this function will return when
 *       crypto engine finish the decryption.
 *
 */

extern uint32_t aes_ecb_decrypt(struct aes_ctx *ctx, uint8_t *in_buf, uint8_t *out_buf);

/**
 * @brief This function release AES crypto engine
 *
 * @param[in]  ctx    AES context
 *
 * @return    STATUS_SUCCESS
 *
 * @details   This function only used in multi-tasking OS.
 *             If the task acquired the hardware engine before, it should call this function
 *             to release the resource.
 *             In single thread environment (FSM), aes_release function always return  STATUS_SUCCESS
 *             Notice: semaphore/mutex protect function depends on OS implemented.
 *
 */
extern uint32_t aes_release(struct aes_ctx *ctx);


/**
 * @brief This function acquire AES crypto engine
 *
 * @param[in]  ctx    AES context
 *
 * @return    STATUS_SUCCESS
 *
 * @details   This function only used in multi-tasking OS.
 *             Only one AES encryption/decryption request can be serviced at any time.
 *             To avoid more than two tasks request the hardware accelerator resource at the same time,
 *             there should be a semaphore/mutex to protect to protect the hardware request run one by one.
 *             In single thread environment (FSM), aes_acquire() function is always return STATUS_SUCCESS
 *             Notice: semaphore/mutex protect function depends on OS implemented.
 *
 *
 */
extern uint32_t aes_acquire(struct aes_ctx *ctx);

/**
 * @brief     xorwith buffer1 and buffer2
 *
 * @param[out]  result  the buffer to hold the result
 * @param[in]   buf1
 * @param[in]   buf2
 *
 * @details
 *             result[i] = buf1[i] ^ buf2[i];
 *
 */
extern void aes_xorwith(uint32_t *result, uint32_t *buf1, const uint32_t *buf2);

/**
 * @brief     Set initialization vector for encryption or decryption.
 *
 * @param[in]  ctx  The AES context to use for encryption or decryption.
 * @param[in]  iv   The initialization vector.
 *
 * @return
 * @retval    STATUS_SUCCESS
 * @retval    STATUS_INVALID_PARAM  --- ctx is NULL
 *
 * @details
 *             Some AES mode, like CBC or CTR, need to set IV before encryption.
 *             This function provide user to update IV before operation.
 *
 */

extern uint32_t aes_iv_set(struct aes_ctx *ctx, const uint8_t *iv);


/**
 * @brief     Get initialization vector for encryption or decryption.
 *
 * @param[in]  ctx  The AES context to use for encryption or decryption.
 * @param[out] iv   Buffer for saving initialization vector result.
 *
 * @return
 * @retval    STATUS_SUCCESS
 * @retval    STATUS_INVALID_PARAM  --- ctx is NULL
 *
 * @details
 *             Many AES mode, like CTR or CBC mode, need to get IV result for next encryption.
 *             This function provides user to get IV result.
 *
 */
extern uint32_t aes_iv_get(struct aes_ctx *ctx, uint8_t  *iv);

/**
 * @brief     This function performs an AES-CBC encryption operation on full blocks.
 *
 * @param[in]  ctx        The AES context to use for encryption.
 * @param[in]  in_buf     The buffer holding the input data.
 * @param[out] out_buf    The buffer holding the output data.
 * @param[in]  length     The length of the input data in Bytes. This must be a multiple for the block size (16 Bytes).
 *
 * @return
 * @retval     STATUS_INVALID_PARAM    if length is 0 or not 16*N bytes
 * @retval     STATUS_SUCCESS
 *
 *
 * @details
 *             This function perfroms AES CBC mode. It is block function, that is,
 *             this function will return when all data has been encrypted.
 *
 */

extern uint32_t aes_cbc_buffer_encrypt(struct aes_ctx *ctx, uint8_t *in_buf, uint8_t *out_buf, uint32_t length);

/**
 * @brief     This function performs an AES-CBC decryption operation on full blocks.
 *
 * @param[in]  ctx        The AES context to use for decryption.
 * @param[in]  in_buf     The buffer holding the input data.
 * @param[out] out_buf    The buffer holding the output data.
 * @param[in]  length     The length of the input data in Bytes. This must be a multiple for the block size (16 Bytes).
 *
 * @return
 * @retval    STATUS_INVALID_PARAM    if length is 0 or not 16*N bytes
 * @retval    STATUS_SUCCESS
 *
 *
 * @details
 *             This function perfroms AES CBC mode. It is block function, that is,
 *             this function will return when all data has been decrypted.
 *
 *
 */

extern uint32_t aes_cbc_buffer_decrypt(struct aes_ctx *ctx, uint8_t *in_buf, uint8_t *out_buf, uint32_t length);

/**
 * @brief     This function performs an AES-CRT cryption operation
 *
 * @param[in]   ctx           The AES context to use for cryption.
 * @param[in]   in_buf        The buffer holding the input data.
 * @param[out]  out_buf       The buffer holding the output data.
 * @param[out]  stream_block  The saved stream block fir resuming. This is overwritten by the function.
 * @param[in]   nc_offset     The offset in the current stream_block, for resuming within the current cipher steam.
 *                            The offset pointer should be 0 at the start if a stream.
 * @param[in]   length        The length of the input data
 *
 * @return
 * @retval     STATUS_INVALID_PARAM
 * @retval     STATUS_SUCCESS
 *
 *
 * @details
 *             This function perfroms AES CBC mode. It is block function, that is,
 *             this function will return when all data has been decrypted.
 *
 *
 */

extern uint32_t aes_ctr_buffer_crypt(struct aes_ctx *ctx,
                                     uint8_t   *in_buf,
                                     uint8_t   *out_buf,
                                     uint8_t   *stream_block,
                                     uint32_t  *nc_offset,
                                     uint32_t  length );


/**
 * @brief      This function will do AES CCM encryption according to ccm_packet setting
 *
 * @param[in]  aes_ccm_encryption_packet --- points to aes_ccm_encryption_packet structure
 *
 * @return
 * @retval    STATUS_INVALID_PARAM  --- any input parameter invalid for CCM request.
 * @retval    STATUS_SUCCESS
 *
 * @details   aes_ccm_encryption_packet is very complext structure, it contains many
 *            members that should be set. Please refer the sample code to know how to set those
 *            parameters. Please notice: hdr_len+data_len must be less than 480 bytes.
 *
 */
extern uint32_t aes_ccm_encryption(struct aes_ccm_encryption_packet  *ccm_packet);

/**
 * @brief    This function will do AES CCM verification according to ccm_packet setting
 *
 * @param[in]    aes_ccm_decryption_packet --- points to aes_ccm_decryption_packet structure
 *
 * @return
 * @retval    STATUS_INVALID_PARAM  --- any input parameter invalid for CCM request.
 * @retval    STATUS_SUCCESS
 *
 *
 * @details   aes_ccm_decryption_packet is very complext structure, it contains many
 *            members that should be set. Please refer the sample code to know how to set those
 *            parameters. Please notice: hdr_len+data_len must be less than 480 bytes.
 *
 *
 */
extern uint32_t aes_ccm_decryption_verification(struct aes_ccm_decryption_packet  *ccm_packet);

/*@}*/ /* end of peripheral_group AES_Driver */

#ifdef __cplusplus
}
#endif

#endif /* end of peripheral_group _AES_H_ */


