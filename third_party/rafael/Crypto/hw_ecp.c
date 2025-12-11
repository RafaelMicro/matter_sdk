#include <stdint.h>
#include <string.h>
#include "hosal_crypto_ecc.h"
#include "hosal_crypto_sha256.h"
#include "hosal_status.h"
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

/* spake2+ compute the L for verifier*/
int rt583_spake2p_compute_L(uint8_t * L, uint8_t * w1)
{
    hosal_crypto_ecc_p256_t ecc_p256;
    /*computer L = w1*P ... P is generator...*/

    ECPoint_P256 Result_point;
    uint8_t w1_le[32];

    /*change w1 to little endian*/
    buffer_endian_exchange((uint32_t *) w1_le, (uint32_t *) w1, 8);

    /* initial the ECC Engine */
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);
    ecc_p256.crypto_operation = HOSAL_GFP_P256_MULTI;
    ecc_p256.result = (ECPoint_P256*) &Result_point;
    ecc_p256.base =  (ECPoint_P256*) &Curve_Gx_p256;
    ecc_p256.p_key = (uint32_t *) w1_le;

    hosal_crypto_ecc_p256(&ecc_p256);

    buffer_endian_exchange((uint32_t *) L, (uint32_t *) Result_point.x, 8);
    buffer_endian_exchange((uint32_t *) (L + (secp256r1_op_num << 2)), (uint32_t *) Result_point.y, 8);

    return 0;
}

/* doing the (R =  m * P + n * Q ) */
int rt583_ecc_multi_add(uint8_t * R_x, uint8_t * R_y, uint8_t * m, uint8_t * P_x, uint8_t * P_y, uint8_t * n, uint8_t * Q_x,
                        uint8_t * Q_y)

{
    hosal_crypto_ecc_p256_t ecc_p256_1, ecc_p256_2, ecc_p256_3;
    ECPoint_P256 R, P, Q, tmp_1, tmp_2;

    /* init the P, Q values*/
    memcpy(P.x, P_x, (secp256r1_op_num << 2));
    memcpy(P.y, P_y, (secp256r1_op_num << 2));
    memcpy(Q.x, Q_x, (secp256r1_op_num << 2));
    memcpy(Q.y, Q_y, (secp256r1_op_num << 2));

    /* (tmp_1 = m * P) */
    /* initial the ECC Engine */
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);
    /* generate the publick key by */

    ecc_p256_1.crypto_operation = HOSAL_GFP_P256_MULTI;
    ecc_p256_1.result = (ECPoint_P256*) &tmp_1;
    ecc_p256_1.base =  (ECPoint_P256*) &P;
    ecc_p256_1.p_key = (uint32_t *) m;

    hosal_crypto_ecc_p256(&ecc_p256_1);
    /* (tmp_2 = n * Q) */
    /* initial the ECC Engine */
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);
    /* generate the publick key by */
    ecc_p256_2.crypto_operation = HOSAL_GFP_P256_MULTI;
    ecc_p256_2.result = (ECPoint_P256*) &tmp_2;
    ecc_p256_2.base =  (ECPoint_P256*) &Q;
    ecc_p256_2.p_key = (uint32_t *) n;

    hosal_crypto_ecc_p256(&ecc_p256_2);

    /* initial the ECC Engine */
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);

    ecc_p256_3.crypto_operation = HOSAL_GFP_P256_ADD;
    ecc_p256_3.p_point_result = (ECPoint_P256*) &R;
    ecc_p256_3.p_point_x1 =  (ECPoint_P256*) &tmp_1;
    ecc_p256_3.p_point_x2 = (ECPoint_P256*) &tmp_2;

    hosal_crypto_ecc_p256(&ecc_p256_3);

    memcpy(R_x, R.x, (secp256r1_op_num << 2));
    memcpy(R_y, R.y, (secp256r1_op_num << 2));

    /*change little endian format ECC point to big endian format... temp use result_le and result_be */
    //    buffer_endian_exchange((uint32_t *) R_x, (uint32_t *) &(R.x), 8);
    //    buffer_endian_exchange((uint32_t *) R_y, (uint32_t *) &(R.y), 8);

    return 0;
}

int rt583_ecc_mul(uint8_t * R_x, uint8_t * R_y, uint8_t * m, uint8_t * P_x, uint8_t * P_y)
{
    hosal_crypto_ecc_p256_t ecc_p256;
    ECPoint_P256 R, P;

    memcpy(P.x, P_x, (secp256r1_op_num << 2));
    memcpy(P.y, P_y, (secp256r1_op_num << 2));

    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);
    ecc_p256.crypto_operation = HOSAL_GFP_P256_MULTI;
    ecc_p256.result = (ECPoint_P256*) &R;
    ecc_p256.base =  (ECPoint_P256*) &P;
    ecc_p256.p_key = (uint32_t *) m;

    hosal_crypto_ecc_p256(&ecc_p256);

    memcpy(R_x, R.x, (secp256r1_op_num << 2));
    memcpy(R_y, R.y, (secp256r1_op_num << 2));

    return 0;
}

/* special function for SPAKE2+ , Z = y*(X-w0*M) */

int rt583_spake2p_compute_Z(uint8_t * Z_x, uint8_t * Z_y, uint8_t * y, uint8_t * X_x, uint8_t * X_y, uint8_t * w0, uint8_t * M_x,
                            uint8_t * M_y)
{
    hosal_crypto_ecc_p256_t ecc_p256_1, ecc_p256_2, ecc_p256_3;
    ECPoint_P256 Z, X, M, tmp_1, tmp_2;

    /* init the X, M values*/
    memcpy(X.x, X_x, (secp256r1_op_num << 2));
    memcpy(X.y, X_y, (secp256r1_op_num << 2));
    memcpy(M.x, M_x, (secp256r1_op_num << 2));
    memcpy(M.y, M_y, (secp256r1_op_num << 2));

    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);

    /*Invert M...*/ /*Invert M = (-M).  Invert M is little endian */
    gfp_point_p256_invert(&M, &M);

    /*so M now is -w0*M,  little endian */
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);
    ecc_p256_1.crypto_operation = HOSAL_GFP_P256_MULTI;
    ecc_p256_1.result = (ECPoint_P256*) &M;
    ecc_p256_1.base =  (ECPoint_P256*) &M;
    ecc_p256_1.p_key = (uint32_t *) w0;

    hosal_crypto_ecc_p256(&ecc_p256_1);

    /*caculate tmp_1 = X-w0*M  */
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);
    ecc_p256_2.crypto_operation = HOSAL_GFP_P256_ADD;
    ecc_p256_2.p_point_result = (ECPoint_P256*) &tmp_1;
    ecc_p256_2.p_point_x1 =  (ECPoint_P256*) &M;
    ecc_p256_2.p_point_x2 = (ECPoint_P256*) &X;

    hosal_crypto_ecc_p256(&ecc_p256_2);

    /*get Z = y*(X-w0*M) */
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);
    ecc_p256_3.crypto_operation = HOSAL_GFP_P256_MULTI;
    ecc_p256_3.result = (ECPoint_P256*) &Z;
    ecc_p256_3.base =  (ECPoint_P256*) &tmp_1;
    ecc_p256_3.p_key = (uint32_t *) y;

    hosal_crypto_ecc_p256(&ecc_p256_3);


    memcpy(Z_x, Z.x, (secp256r1_op_num << 2));
    memcpy(Z_y, Z.y, (secp256r1_op_num << 2));

    return 0;
}

/* special function for SPAKE2+ , V = y*L */
int rt583_spake2p_verifier_V(uint8_t * V_x, uint8_t * V_y, uint8_t * y, uint8_t * L_x, uint8_t * L_y)
{
    hosal_crypto_ecc_p256_t ecc_p256;
    ECPoint_P256 V, L;

    /* init the X, M values*/
    memcpy(L.x, L_x, (secp256r1_op_num << 2));
    memcpy(L.y, L_y, (secp256r1_op_num << 2));

    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);
    ecc_p256.crypto_operation = HOSAL_GFP_P256_MULTI;
    ecc_p256.result = (ECPoint_P256*) &V;
    ecc_p256.base =  (ECPoint_P256*) &L;
    ecc_p256.p_key = (uint32_t *) y;

    hosal_crypto_ecc_p256(&ecc_p256);
    memcpy(V_x, V.x, (secp256r1_op_num << 2));
    memcpy(V_y, V.y, (secp256r1_op_num << 2));

#if 0
    {
        int i;
        printf("verifier V point:\n");
        for (i = 0; i < 32; i++)
            printf("%2x-", V_x[i]);
        printf("\n");
        for (i = 0; i < 32; i++)
            printf("%2x-", V_y[i]);
        printf("\n");
    }
#endif

    return 0;
}

/* special function for SPAKE2+ , V = w1*(Y-w0*N) */
int rt583_spake2p_prover_V(uint8_t * V_x, uint8_t * V_y, uint8_t * w0, uint8_t * w1, uint8_t * Y_x, uint8_t * Y_y, uint8_t * N_x,
                           uint8_t * N_y)
{
    hosal_crypto_ecc_p256_t ecc_p256_1, ecc_p256_2, ecc_p256_3;
    ECPoint_P256 V, Y, N, tmp_1, tmp_2;

    /* init the X, M values*/
    memcpy(Y.x, Y_x, (secp256r1_op_num << 2));
    memcpy(Y.y, Y_y, (secp256r1_op_num << 2));
    memcpy(N.x, N_x, (secp256r1_op_num << 2));
    memcpy(N.y, N_y, (secp256r1_op_num << 2));


    /*Invert N...*/ /*Invert N = (-N).  Invert M is little endian */
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);
    gfp_point_p256_invert(&N, &N);

    /*so M now is -w0*N,  little endian */
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);
    ecc_p256_1.crypto_operation = HOSAL_GFP_P256_MULTI;
    ecc_p256_1.result = (ECPoint_P256*) &N;
    ecc_p256_1.base =  (ECPoint_P256*) &N;
    ecc_p256_1.p_key = (uint32_t *) w0;

    hosal_crypto_ecc_p256(&ecc_p256_1);

    /*caculate tmp_1 = Y-w0*M  */
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);
    ecc_p256_2.crypto_operation = HOSAL_GFP_P256_ADD;
    ecc_p256_2.p_point_result = (ECPoint_P256*) &tmp_1;
    ecc_p256_2.p_point_x1 =  (ECPoint_P256*) &N;
    ecc_p256_2.p_point_x2 = (ECPoint_P256*) &Y;

    hosal_crypto_ecc_p256(&ecc_p256_2);
    /*get Z = w1*(X-w0*M) */
    hosal_crypto_ecc_init(HOSAL_ECC_CURVE_P256_INIT);
    ecc_p256_3.crypto_operation = HOSAL_GFP_P256_MULTI;
    ecc_p256_3.result = (ECPoint_P256*) &V;
    ecc_p256_3.base =  (ECPoint_P256*) &tmp_1;
    ecc_p256_3.p_key = (uint32_t *) w1;

    hosal_crypto_ecc_p256(&ecc_p256_3);


    memcpy(V_x, V.x, (secp256r1_op_num << 2));
    memcpy(V_y, V.y, (secp256r1_op_num << 2));

    return 0;
}