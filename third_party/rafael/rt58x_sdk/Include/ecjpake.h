
#ifndef _INCLUDE_ECJPAKE_ECC256_DEFINE_
#define _INCLUDE_ECJPAKE_ECC256_DEFINE_

#include "cm3_mcu.h"


#ifdef __cplusplus
extern "C"
{
#endif

/*ECJPAKE HERE only support SECP256R1.. so each ECC key must be 64 bytes.*/


#define server_role                   1
#define client_role                   0


/*in IETF document, this is ECSchnorrZKP struct*/

typedef struct
{
    ECPoint_P256    V;                               /*EC point, this is big endian key*/
    uint8_t         r[secp256r1_op_num_in_byte];     /*Big number... HERE only use 32 bytes for SECP256R1.*/
} ECJPAKE_ZKP;

typedef struct
{
    ECPoint_P256     X;             /*EC point, this is big endian key*/
    ECJPAKE_ZKP      zkp;
} ECJPAKEKeyKP;


/*
 * Notice: This is very large structure... so please use this struct in heap... and
 * avoid data leakage.. please clean the key information....
 *
 * All key/data in big endian format
 *
 * in fact, we can count ecc_point_X1 from private_key_x1, but we save it for time saving..
 * if you want to save memory, you can remove member private_key_x1
 * (because it will not used after first step)
 *
 */

typedef struct ecjpake_context
{

    ECPoint_P256  ecc_point_X1;        /*EC point, self public key X1 in step1 */
    ECPoint_P256  ecc_point_X2;        /*EC point, self public key X2 in step1 */
    ECPoint_P256  ecc_point_X3;        /*EC point, peer public key X3 in step1 */
    ECPoint_P256  ecc_point_X4;        /*EC point, peer public key X4 in step1 */

    uint8_t  private_key_x1[secp256r1_op_num_in_byte];      /*self key x1 */
    uint8_t  private_key_x2[secp256r1_op_num_in_byte];      /*self key x2 */

    ECPoint_P256  share_key;           /*share key*/

    uint32_t role;

    uint8_t    secret[secp256r1_op_num_in_byte];        /*secret must be 32 bytes.*/

} ECJPAKE_CTX;


extern void ecjpake_generate_zkp(ECJPAKE_CTX  *ctx, ECJPAKEKeyKP *key, ECPoint_P256 *gen, uint8_t *private_key);

extern uint32_t ecjpake_verify_zkp(ECJPAKE_CTX  *ctx, ECPoint_P256 *generator, ECJPAKEKeyKP *key);

extern uint32_t ecjpake_generate_step2_zkp(ECJPAKE_CTX  *ctx, ECJPAKEKeyKP *key);

extern uint32_t ecjpake_verify_step2_zkp(ECJPAKE_CTX  *ctx, ECJPAKEKeyKP *key);

extern uint32_t ecjpake_compute_key(ECJPAKE_CTX  *ctx, ECJPAKEKeyKP *key, uint8_t *pms);

#ifdef __cplusplus
}
#endif

#endif
