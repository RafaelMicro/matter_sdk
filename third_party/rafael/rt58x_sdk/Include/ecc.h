/**************************************************************************//**
 * @file     ecc.h
 * @version
 * @brief    ECC driver header file
 *
 * @copyright
 ******************************************************************************/

/** @defgroup    ECC_Driver   ECC Driver
 *  @ingroup     peripheral_group
 *  @brief  ECC driver header information
 *  @{
 *  @details
 *  ECC is elliptic curve cryptography algorithm
 *  This driver supports multiple ECC curves, including
 *             SECP192R1 (aka NIST P-192 or prime192v1 in X9.62)
 *             SECT163R1
 *             SECP256R1 (aka NIST P-256 or prime256v1 in X9.62)
 *             X25519    (defined in RFC7748)
 *
 *  You can choice some of those curves in in your project_config.h by
 *
 *   Support ECC SECP192R1/P192 curve
 *         #define CRYPTO_SECP192R1_ENABLE      1
 *
 *   Support ECC SECP256R1/P256 curve
 *        #define CRYPTO_SECP256R1_ENABLE       1
 *
 *   Support ECC SECT163R2/B163 curve
 *   #define CRYPTO_SECT163R2_ENABLE            1
 *
 *   Support ECDHE for curve25519 (X25519)
 *   #define CRYPTO_C25519_ENABLE
 *
 *  Plese notice, we support ecc kernel --- "point multiplication algorithm" over those curves.
 *  Based this ecc point multiplication for ECDH algorithm or key generation.
 *
 *  For ECDSA verification, we only support SECP256R1(PRIME256V1).
 *  The chip supports hardware secure boot based this SECP256R1 curves.
 *
 *  Please also notice: all numbers/parameters used in this driver is LITTLE ENDIAN format!
 *  But network spec or standard all using BIG ENDIAN format. Please remember to convert the
 *  big endian key to little endian. And in sometimes, public key look like (04||X||Y), Here
 *  04 is uncompressed form. Please remember to remove this "PC" value first.
 *
 *  ECC and AES share the same crypto accelerator, so if your system is multi-threads, like FreeRTOS,
 *  you must be implement a semaphore/mutexto protect crypto acclerator resource.
 *
 *  You should implement the following functions
 *      void crypto_mutex_lock(void);
 *      void crypto_mutex_unlock(void);
 *
 *  These two functions are OS-dependent.
 *
 *  If your system is single thread (FSM), then both of these functions can be ignored.
 *
 *
*/
#ifndef ___ECC_H__
#define ___ECC_H__

#ifdef __cplusplus
extern "C"
{
#endif


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
/*The following define in size of "uint32_t" */

/*secp192r1 curve is 192 bytes, 6*uint32_t = 192 */
#define secp192r1_op_num      6             /*!< SECP192R1 key size is 6*sizeof(uint32_t)   */

/*secp256r1 curve is 256 bytes, 8*uint32_t = 256 */
#define secp256r1_op_num      8             /*!< SECP256R1 key size is 8*sizeof(uint32_t)   */

/*curve X25519/C25519 is 256 bytes data. 8*uint32_t = 256 */
#define C25519_Length         8             /*!< X25519/C25519 key size is 8*sizeof(uint32_t)  */

/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/

#if MODULE_ENABLE(CRYPTO_SECP192R1_ENABLE)

/**
 * @brief Set crypto accelerator to load ECC SECP192R1 curve algorithm.
 *
 * @param[in]  None
 *
 * @return
 * @retval    STATUS_SUCCESS
 *
 * @details  Crypto accelerator needs to load crypto algorithm before doing crypto operation.
 *            This initialize function must be call every time before calling gfp_point_p192_mult.
 *            In this function, it will try to call lock mutex for using crypto accelerator.
 *
 *
 */
extern uint32_t gfp_ecc_curve_p192_init(void);

/**
 * @brief   Use Crypto accelerator to caculate ECC point K*G based on SECP192R1 curve,
 *           G is the point on curve and K is a big number (private key).
 *
 * @param[out]  p_result_x   output buffer for the point x-coordinate of K*G
 * @param[out]  p_result_y   output buffer for the point y-coordinate of K*G
 * @param[in]   target_x     input buffer for the point Gx
 * @param[in]   target_y     input buffer for the point Gy
 * @param[in]   target_k     the private key.
 *
 * @return
 * @retval    STATUS_SUCCESS
 * @retval    STATUS_INVALID_PARAM  --- value K is invalid data.
 *
 * @details   Notice: ALL buffer data is little endian format.
 *             This function is block function.
 *             In this function, it will release lock mutex after multiplication finishs.
 *
 *
 */

extern uint32_t gfp_point_p192_mult(
    uint32_t *p_result_x,
    uint32_t *p_result_y,
    uint32_t *target_x,
    uint32_t *target_y,
    uint32_t *target_k
);

#endif

#if  MODULE_ENABLE(CRYPTO_SECP256R1_ENABLE)


#define secp256r1_op_num_in_byte        32


typedef struct
{
    uint8_t        x[secp256r1_op_num_in_byte];        /*EC point, x coordinate*/
    uint8_t        y[secp256r1_op_num_in_byte];        /*EC point, y coordinate*/
} ECPoint_P256;


/*
 * Please Notice: ECDSA signater value is NOT valid point on curver. it just
 * two big number r and s.
 *
 */
typedef struct
{
    uint8_t        r[secp256r1_op_num_in_byte];        /*SECP256R1 signature r */
    uint8_t        s[secp256r1_op_num_in_byte];        /*SECP256R1 signature s */
} Signature_P256;


/**
 * @brief Set Crypto accelerator to load ECC SECP256R1 curve algorithm.
 *
 * @param[in]  None
 *
 * @return
 * @retval    STATUS_SUCCESS
 *
 * @details  Crypto accelerator needs to load crypto algorithm before doing crypto operation.
 *            This initialize function must be call every time before calling gfp_point_p256_mult.
 *            In this function, it will try to call lock mutex for using crypto accelerator.
 *
 */
extern uint32_t gfp_ecc_curve_p256_init(void);

/**
 * @brief   Use Crypto accelerator to caculate ECC point K*G based on SECP256R1 curve,
 *           G is the point on the curve and k is a big number (private key).
 *
 * @param[out]  p_result_point     output point for the  k*G
 * @param[in]   p_based_point      input based based point G
 * @param[in]   p_target_k         input buffer for point multiplier k
 *
 * @return
 * @retval    STATUS_SUCCESS
 * @retval    STATUS_INVALID_PARAM  --- value K is invalid data.
 *
 * @details   Notice: ALL buffer data is little endian format.
 *             This function is block function.
 *             In this function, it will release lock mutex after multiplication finishs.
 *
 */

extern uint32_t gfp_point_p256_mult(
    ECPoint_P256    *p_result_point,
    ECPoint_P256    *p_based_point,
    uint32_t        *p_target_k
);


/**
 * @brief Set Crypto accelerator to load ECDSA Verify algorithm based on ECC SECP256R1 curve.
 *
 * @param[in]  None
 *
 * @return
 * @retval    STATUS_SUCCESS
 *
 * @details  Crypto accelerator needs to load crypto algorithm before doing crypto operation.
 *            This initialize function must be called before calling gfp_ecdsa_p256_verify.
 *
 */

extern void gfp_ecdsa_p256_verify_init(void);

/**
 * @brief   Use Crypto accelerator to verify ECDSA result based on SECP256R1 curve,
 *
 * @param[in]   p_signature     input buffer for the signature point,  r, s
 * @param[in]   p_hash          input buffer for the hash data, 32 bytes.
 * @param[in]   p_public_key    input buffer for the public key point
 *
 * @retval
 *             return STATUS_ERROR    for verification fail
 *             return STATUS_SUCCESS  for verification success.
 *
 * @details   Notice: ALL buffer data is little endian format.
 *             This function is block function.
 *             In this function, it will release lock mutex after verification finishs.
 *
 */

extern uint32_t gfp_ecdsa_p256_verify(Signature_P256 *p_signature, uint32_t *p_hash, ECPoint_P256 *p_public_key);


/**
 * @brief   Use Crypto Engine to signature ECDSA result based on SECP256R1 curve,
 *
 * @param[in]   p_signature        output buffer for the signature value.
 * @param[in]   p_hash_message     input buffer for the hash data, 32 bytes.
 * @param[in]   p_private_key      input buffer for the public key ECC256 Point
 * @param[in]   p_random_k         input buffer for random key
 *
 * @retval
 *             return STATUS_ERROR    for verification fail
 *             return STATUS_SUCCESS  for verification success.
 *
 * @details   Notice: ALL buffer data is little endian format. This ecdsa signature is nondeterminstic!
 *
 *
 */


extern uint32_t gfp_ecdsa_p256_signature(Signature_P256 *p_signature,
        uint32_t *p_hash_message, uint32_t *p_private_key,
        uint32_t *p_random_k);



/**
 * @brief   Use Crypto Engine to verify point (x,y) is on SECP256R1 curve or not.
 *
 * @param[in]   p_point_x            input buffer for the ECC256 point to be verification.
 *
 * @retval
 *             return STATUS_ERROR    for verification fail
 *             return STATUS_SUCCESS  for verification success.
 *
 * @details   Notice: ALL buffer data is little endian format.
 *
 */

extern uint32_t gfp_valid_point_p256_verify(ECPoint_P256 *p_point);

/**
 * @brief   Use Crypto Engine to add two point on curve
 *
 * @param[out]  p_point_result        input buffer for the ECC256 point to be verification.
 * @param[in]   p_point_x1            input buffer for the ECC256 point to be verification.
 * @param[in]   p_point_x2            input buffer for the ECC256 point to be verification.
 *
 *
 * @retval
 *
 * @details   Notice: ALL buffer data is little endian format.
 *
 */

extern uint32_t gfp_point_p256_add(ECPoint_P256 *p_point_result,
                                   ECPoint_P256 *p_point_x1, ECPoint_P256 *p_point_x2);

/**
 * @brief   Use Crypto Engine to caculate v-x*h mod order , this is special function required by ECJPAKE.
 *
 * @param[out]  p_result       output buffer for result
 * @param[in]   p_x            input buffer for x (ECJAPKE private key.)
 * @param[in]   p_hash         input buffer for hash value
 * @param[in]   p_v            input buffer for v (ECJPAKE ZKP random key)
 *
 *
 * @details   Notice: ALL buffer data is little endian format. This function is designed for
 *             SECP256R1, so order is the order of SECP256R1.
 */
extern void gfp_scalar_vxh_p256(uint32_t *p_result, uint32_t *p_x, uint32_t *p_hash, uint32_t *p_v);

/**
 * @brief   Use Crypto Engine to caculate x*y mod order , this is special function required by ECJPAKE.
 *
 * @param[out]  p_result       output buffer for result
 * @param[in]   p_x            input buffer for x
 * @param[in]   p_y            input buffer for y
 *
 *
 * @details   Notice: ALL buffer data is little endian format. This function is designed for
 *             SECP256R1, so order is the order of SECP256R1.
 */
extern void gfp_scalar_modmult_p256(uint32_t *p_result, uint32_t *p_x, uint32_t *p_y);


/**
 * @brief   Caculate ECC point invert based SECP256 curve
 *
 * @param[out]  p_invert       output point of the invert input point
 * @param[in]   p_point        input point
 *
 * @details   Notice: ALL buffer data is little endian format. This function is designed for
 *             SECP256R1
 */

extern uint32_t gfp_point_p256_invert(ECPoint_P256 *p_invert, ECPoint_P256 *p_point);




extern const ECPoint_P256  Curve_Gx_p256;
extern const ECPoint_P256  Curve_Gx_p256_BE;


#endif

#if MODULE_ENABLE(CRYPTO_SECT163R2_ENABLE)
/**
 * @brief Set Crypto accelerator to load ECC SECP163R2 curve algorithm.
 *
 * @param[in]  None
 *
 * @return
 * @retval   STATUS_SUCCESS
 *
 *
 * @details  Crypto accelerator needs to load crypto algorithm before doing crypto operation.
 *            This initialize function must be call every time before calling gfp_point_b163_mult.
 *
 *
 */
extern uint32_t gf2m_ecc_curve_b163_init(void);

/**
 * @brief   Use Crypto accelerator to caculate ECC point K*G based on SECT163R2 curve,
 *           G is the point on curve and K is a big number (private key).
 *
 * @param[out]  p_result_x   output buffer for the point x-coordinate of K*G
 * @param[out]  p_result_y   output buffer for the point y-coordinate of K*G
 * @param[in]   target_x     input buffer for the point Gx
 * @param[in]   target_y     input buffer for the point Gy
 * @param[in]   target_k     the private key.
 *
 * @return
 * @retval    STATUS_SUCCESS
 * @retval    STATUS_INVALID_PARAM  --- value K is invalid data.
 *
 * @details   Notice: ALL buffer data is little endian format.
 *             This function is block function.
 *             In this function, it will release lock mutex after verification finishs.
 *
 */
extern uint32_t gf2m_point_b163_mult(
    uint32_t *p_result_x,
    uint32_t *p_result_y,
    uint32_t *target_x,
    uint32_t *target_y,
    uint32_t *target_k
);

#endif

#if MODULE_ENABLE(CRYPTO_C25519_ENABLE)

/**
 * @brief Set Crypto accelerator to load ECC Curve25519 algorithm.
 *
 * @param[in]  None
 *
 * @return
 * @retval   STATUS_SUCCESS
 *
 * @details  Crypto accelerator needs to load crypto algorithm before doing crypto operation.
 *
 *
 */

extern void curve_c25519_init(void);

/**
 * @brief   Use Crypto accelerator to caculate ECC point K*G based on Curve25519,
 *           G is the point on curve and secret_key is a big number.
 *
 * @param[in]   blind_zr     a random number buffer for against side-channel attacks
 * @param[out]  public_key   output buffer for the point x-coordinate of K*G
 * @param[in]   secret_key   the private key.
 * @param[in]   base_point   input buffer for the point x-coordinate of G
 *
 * @return
 * @retval    STATUS_SUCCESS
 * @retval    STATUS_INVALID_PARAM  --- value secret_key is invalid data.
 *
 * @details  Notice: ALL buffer data is little endian format.
 *            Remark:  blind_zr is random 32 bytes, it used to add entropy, that outside can not guess
 *            key by "current"  or "caculate time". It can be NULL for using default blind pattern.
 *            This function is block function.
 *
 */

extern uint32_t curve25519_point_mul(uint32_t *blind_zr, uint32_t *public_key, uint32_t *secret_key, uint32_t *base_point);


#endif

/*@}*/ /* end of peripheral_group ECC_Driver */

#ifdef __cplusplus
}
#endif

#endif /* end of ECC_Driver */


