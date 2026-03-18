#include <stdint.h>
#include <string.h>
#include "hosal_crypto_ecc.h"
#include "hosal_crypto_sha256.h"
#include "hosal_status.h"
static void endian_swap(uint8_t * dst, uint8_t * src, uint32_t size)
{
    uint32_t i = 0;

    while (i < size)
    {
        dst[i] = src[(size - 1) - i];

        i++;
    }
}

int rt583_ecdsa_p256_sign(uint8_t * s_r, uint8_t * s_s, uint8_t * hash, uint8_t * pri_k, uint8_t * mod_k)
{
    /* sha 256 size is 32 bytes */
    hosal_crypto_ecc_p256_t ecc_p256;
    uint8_t buf[32];
    Signature_P256 data_signature;

    /* swap the hash value endian */
    endian_swap(buf, hash, sizeof(buf));

    /* initial the ECC Engine */
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);

    /*ecdsa signature.*/
    ecc_p256.crypto_operation = HOSAL_ECDA_P256_SIGNATURE;
    ecc_p256.signatrue =  &data_signature;
    ecc_p256.p_hash = (uint32_t*) buf;
    ecc_p256.p_key =  (uint32_t*) pri_k;
    ecc_p256.p_k = (uint32_t*) mod_k;

    hosal_crypto_ecc_p256(&ecc_p256);

    memcpy(s_r, data_signature.r, (secp256r1_op_num << 2));
    memcpy(s_s, data_signature.s, (secp256r1_op_num << 2));

    return 0;
}

int rt583_ecdsa_p256_verify(uint8_t * s_r, uint8_t * s_s, uint8_t * hash, uint8_t * key_x, uint8_t * key_y)
{
    /* sha 256 size is 32 bytes */
    uint8_t buf[32];
    uint32_t status;
    Signature_P256 data_signature;
    ECPoint_P256 Public_key;

    /* swap the hash value endian */
    endian_swap(buf, hash, sizeof(buf));

    memcpy(data_signature.r, s_r, secp256r1_op_num_in_byte);
    memcpy(data_signature.s, s_s, secp256r1_op_num_in_byte);
    memcpy(Public_key.x, key_x, secp256r1_op_num_in_byte);
    memcpy(Public_key.y, key_y, secp256r1_op_num_in_byte);

    /*ecdsa signature.*/
    hosal_crypto_ecc_p256_t ecc_p256;

    hosal_crypto_ecc_init(HOSAL_ECDA_CURVE_P256_VERIFY_INIT);

    ecc_p256.crypto_operation = HOSAL_ECDA_P256_VERIFY;
    ecc_p256.signatrue = &data_signature;
    ecc_p256.p_hash = (uint32_t*)buf;
    ecc_p256.base = &Public_key;

    status = hosal_crypto_ecc_p256(&ecc_p256);

    return ((int) status);
}
