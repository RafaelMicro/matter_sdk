#include <stdint.h>
#include <string.h>
#include "hosal_crypto_ecc.h"
#include "hosal_crypto_sha256.h"
#include "hosal_status.h"
#include "hw_ecc_lock.h"
/*
 *  secp256r1
 *
 *  Remark: This is demo sample only.
 *  HERE PRIVATE KEY IS TEST ONLY. Private Key should be random generated.
 *  It can NOT be hardcode, otherwise key will be extracted from binary or hex file.
 *
 *     Please Notice: Share key is in little endian format...
 *  So if you want to use the share key for some cryption, please notice
 *  key format is little-endian or big-endian
 */

/*Remark: data is little endian format. that is MSB is the last byte. */
/*little endian for SECP256R1 generator.*/

int rt583_ecc_public_key_gen(uint8_t * pt_pri_k, uint8_t * pt_pub_k_x, uint8_t * pt_pub_k_y)
{
    hosal_crypto_ecc_p256_t ecc_p256;
    ECPoint_P256 Public_key;


    /* initial the ECC Engine */
    hw_ecc_lock();
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);

    /* generate the publick key by */
    ecc_p256.crypto_operation = HOSAL_GFP_P256_MULTI;
    ecc_p256.result = (ECPoint_P256*) &Public_key;
    ecc_p256.base =  (ECPoint_P256*) &Curve_Gx_p256;
    ecc_p256.p_key = (uint32_t *) pt_pri_k;

    hosal_crypto_ecc_p256(&ecc_p256);
    hw_ecc_unlock();

    memcpy(pt_pub_k_x, Public_key.x, (secp256r1_op_num << 2));
    memcpy(pt_pub_k_y, Public_key.y, (secp256r1_op_num << 2));

    return 0;
}

int rt583_ecc_shared_secert_gen(uint8_t * pt_sh_k_x, uint8_t * pt_sh_k_y, uint8_t * pt_pub_k_x, uint8_t * pt_pub_k_y,
                                uint8_t * pt_prv_k_y)
{

    hosal_crypto_ecc_p256_t ecc_p256;
    ECPoint_P256 Share_key, Public_key;


    // printf("    caculate shared secret by using hardware ECDH\n");

    memcpy(Public_key.x, pt_pub_k_x, (secp256r1_op_num << 2));
    memcpy(Public_key.y, pt_pub_k_y, (secp256r1_op_num << 2));

    /* initial the curve data */
    hw_ecc_lock();
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);

    /* generate the publick key by */
    ecc_p256.crypto_operation = HOSAL_GFP_P256_MULTI;
    ecc_p256.result = (ECPoint_P256*) &Share_key;
    ecc_p256.base =  (ECPoint_P256*) &Public_key;
    ecc_p256.p_key = (uint32_t *) pt_prv_k_y;

    hosal_crypto_ecc_p256(&ecc_p256);
    hw_ecc_unlock();

    memcpy(pt_sh_k_x, Share_key.x, (secp256r1_op_num << 2));
    memcpy(pt_sh_k_y, Share_key.y, (secp256r1_op_num << 2));

    return 0;
}
