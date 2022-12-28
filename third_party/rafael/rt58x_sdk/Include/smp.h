// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
#ifndef __SMP_H__
#define __SMP_H__

#include <stdint.h>
#include "host_management.h"
#ifdef __cplusplus
extern "C" {
#endif

#define CODE_SMP_RESERVED                                       0x00
#define CODE_SMP_PAIRING_REQUEST                                0x01
#define CODE_SMP_PAIRING_RESPONSE                               0x02
#define CODE_SMP_PAIRING_CONFIRM                                0x03
#define CODE_SMP_PAIRING_RANDOM                                 0x04
#define CODE_SMP_PAIRING_FAILED                                 0x05
#define CODE_SMP_ENCRYPTION_INFORMATION                         0x06
#define CODE_SMP_MASTER_IDENTIFICATION                          0x07
#define CODE_SMP_IDENTITY_INFORMATION                           0x08
#define CODE_SMP_IDENTITY_ADDRESS_INFORMATION                   0x09
#define CODE_SMP_SIGNING_INFORMATION                            0x0A
#define CODE_SMP_SECURITY_REQUEST                               0x0B
#define CODE_SMP_PAIRING_PUBLIC_KEY                             0x0C
#define CODE_SMP_PAIRING_DHKEY_CHECK                            0x0D
#define CODE_SMP_PAIRING_KEYPRESS_NOTIFICATION                  0x0E
#define CODE_SMP_MAX_HANDLE_RANGE                               CODE_SMP_SECURITY_REQUEST

#define SMP_SECURITY_MODE_1_NO_SECURITY                         0x00
#define SMP_SECURITY_MODE_1_UNAUTHEN_PAIRING_W_ENCYPT           0x01
#define SMP_SECURITY_MODE_1_AUTHEN_PAIRING_W_ENCYPT             0x02
#define SMP_SECURITY_MODE_2_UNAUTHEN_PAIRING_W_DATA_SIGN        0x03
#define SMP_SECURITY_MODE_2_AUTHEN_PAIRING_W_DATA_SIGN          0x04
#define SMP_SECURITY_MODE_MINIMUN                               SMP_SECURITY_MODE_1_UNAUTHEN_PAIRING_W_ENCYPT

#define IO_CAPABILITY_DISPLAY_ONLY                              0x00
#define IO_CAPABILITY_DISPLAY_YESNO                             0x01
#define IO_CAPABILITY_KEYBOARD_ONLY                             0x02
#define IO_CAPABILITY_NOINPUT_NOOUTPUT                          0x03
#define IO_CAPABILITY_KEYBOARD_DISPLAY                          0x04

#define OOB_AUTHENTICATION_DATA_NOT_PRESENT                     0x00
#define OOB_AUTHENTICATION_DATA_FROM_REMOTE_DEVICE_PRESENT      0x01

#define AUTHREQ_BONDING_FLAGS_NO_BONDING                        0x00
#define AUTHREQ_BONDING_FLAGS_BONDING                           0x01

#define MITM_PROTECTION_YES                                     0x04
#define MITM_PROTECTION_NO                                      0x00

#define SECURE_CONN_PAIRING_NO                                  0x00
#define SECURE_CONN_PAIRING_YES                                 0x08

#define SMP_PAIRING_KEYPRESS_NOTIFICATION_NO                    0x00
#define SMP_PAIRING_KEYPRESS_NOTIFICATION_YES                   0x10

#define CT2_SUPPORTS_H7_FUNCTION_NO                             0x00
#define CT2_SUPPORTS_H7_FUNCTION_YES                            0x20

#define KEY_DISTRIBUTION_ENCKEY_0                               0x00
#define KEY_DISTRIBUTION_ENCKEY_1                               0x01
#define KEY_DISTRIBUTION_IDKEY_0                                0x00
#define KEY_DISTRIBUTION_IDKEY_1                                0x02
#define KEY_DISTRIBUTION_SIGN_0                                 0x00
#define KEY_DISTRIBUTION_SIGN_1                                 0x04
#define KEY_DISTRIBUTION_LINK_0                                 0x00
#define KEY_DISTRIBUTION_LINK_1                                 0x08

#define STK_GEN_MTHD_JUST_WORKS                                 0x00
#define STK_GEN_MTHD_PASSKEY_ENTRY                              0x01
#define STK_GEN_MTHD_PASSKEY_ENTRY_DISP                         0x02

#define ERR_CODE_SMP_RESERVED                                   0x00
#define ERR_CODE_SMP_PASSKEY_ENTRY_FAILED                       0x01
#define ERR_CODE_SMP_OOB_NOT_AVAILABLE                          0x02
#define ERR_CODE_SMP_AUTHENTICATION_REQUIREMENTS                0x03
#define ERR_CODE_SMP_CONFIRM_VALUE_FAILED                       0x04
#define ERR_CODE_SMP_PAIRING_NOT_SUPPORTED                      0x05
#define ERR_CODE_SMP_ENCRYPTION_KEY_SIZE                        0x06
#define ERR_CODE_SMP_COMMAND_NOT_SUPPORTED                      0x07
#define ERR_CODE_SMP_UNSPECIFIED_REASON                         0x08
#define ERR_CODE_SMP_REPEATED_ATTEMPTS                          0x09
#define ERR_CODE_SMP_INVALID_PARAMETERS                         0x0A
#define ERR_CODE_SMP_DHKEY_CHECK_FAILED                         0x0B
#define ERR_CODE_SMP_NUMERIC_COMPARISON_FAILED                  0x0C
#define ERR_CODE_SMP_BR_EDR_PAIRING_IN_PROGRESS                 0x0D
#define ERR_CODE_SMP_CROSS_TRANS_KEY_GEN_NOT_ALLOWED            0x0E

#define SIZE_MINIMUM_ENCRYPTION_KEY                             16
#define SIZE_AES_KEY                                            16
#define SIZE_SMP_RAND                                           8
#define SIZE_SMP_EDIV                                           2
#define SIZE_SMP_SKDM                                           8
#define SIZE_SMP_IVM                                            4
#define SIZE_SMP_SKDS                                           8
#define SIZE_SMP_IVS                                            4
#define SIZE_SMP_IRK                                            16
#define SIZE_SMP_CSRK                                           16

#define STATE_GEN_RANDOM_NUMBER                                 0x01
#define STATE_GEN_RANDOM_VALUE                                  0x02
#define STATE_GEN_LTK                                           0x03
#define STATE_CONFIRM_VALUE_GEN_C1_P1                           0x04
#define STATE_CONFIRM_VALUE_GEN_C1_P2                           0x05
#define STATE_GEN_STK_S1                                        0x06
#define STATE_RANDOM_ADDRESS_HASH_AH_GEN                        0x07
#define STATE_RANDOM_ADDRESS_HASH_AH_COMP                       0x08
#define STATE_GEN_LOCAL_IRK                                     0x09
#define STATE_GEN_LOCAL_IRK_BY_SECURITY                         0x0A

#define SMP_KEY_USE_STK                                         0x00
#define SMP_KEY_USE_LTK                                         0x01

#define IRK_PREDEF_FIXED                0xB5, 0xB7, 0xE1, 0x48, \
                                        0xAC, 0x7E, 0x3B, 0x91, \
                                        0x8A, 0xA6, 0xD7, 0x1C, \
                                        0x45, 0x33, 0xE5, 0x0B

typedef struct smp_op_generic_format_t
{
    uint8_t   SmprOpcode;
    uint8_t   data[];
} smp_op_generic_format_t;

typedef struct AuthReq_t
{
    uint8_t   BondingFlags   : 2;
    uint8_t   MITM           : 1;
    uint8_t   SC             : 1;
    uint8_t   Keypress       : 1;
    uint8_t   CT             : 1;
    uint8_t   RFU            : 2;
} AuthReq_t;

typedef struct smp_op_pairing_request_t
{
    uint8_t   SmprOpcode;
    uint8_t   IOCapability;
    uint8_t   OOBDataFlag;
    AuthReq_t AuthReq;
    uint8_t   MaximumEncryptionKeySize;
    uint8_t   InitiatorKeyDistribution;
    uint8_t   ResponderKeyDistribution;
} smp_op_pairing_request_t;

typedef struct smp_op_pairing_response_t
{
    uint8_t   SmprOpcode;
    uint8_t   IOCapability;
    uint8_t   OOBDataFlag;
    AuthReq_t AuthReq;
    uint8_t   MaximumEncryptionKeySize;
    uint8_t   InitiatorKeyDistribution;
    uint8_t   ResponderKeyDistribution;
} smp_op_pairing_response_t;

typedef struct smp_op_pairing_confirm_t
{
    uint8_t   SmprOpcode;
    uint8_t   ConfirmValue[16];
} smp_op_pairing_confirm_t;

typedef struct smp_op_pairing_random_t
{
    uint8_t   SmprOpcode;
    uint8_t   RandomValue[16];
} smp_op_pairing_random_t;

typedef struct smp_op_pairing_failed_t
{
    uint8_t   SmprOpcode;
    uint8_t   Reason;
} smp_op_pairing_failed_t;

typedef struct smp_op_encryption_information_t
{
    uint8_t   SmprOpcode;
    uint8_t   LongTermKey[16];
} smp_op_encryption_information_t;

typedef struct smp_op_master_identification_t
{
    uint8_t   SmprOpcode;
    uint8_t   EDIV[2];
    uint8_t   RAND[8];
} smp_op_master_identification_t;

typedef struct smp_op_identity_information_t
{
    uint8_t   SmprOpcode;
    uint8_t   IdentityResolvingKey[16];
} smp_op_identity_information_t;

typedef struct smp_op_identity_addr_information_t
{
    uint8_t   SmprOpcode;
    uint8_t   AddrType;
    uint8_t   BDADDR[6];
} smp_op_identity_addr_information_t;

typedef struct smp_op_signing_information_t
{
    uint8_t   SmprOpcode;
    uint8_t   SignatureKey[16];
} smp_op_signing_information_t;

typedef struct smp_op_security_request_t
{
    uint8_t   SmprOpcode;
    AuthReq_t AuthReq;
} smp_op_security_request_t;

typedef struct smp_op_pairing_public_key_t
{
    uint8_t   SmprOpcode;
    uint8_t   PublicKeyX[32];
    uint8_t   PublicKeyY[32];
} smp_op_pairing_public_key_t;

typedef struct smp_op_pairing_dhkey_check_t
{
    uint8_t   SmprOpcode;
    uint8_t   DHKeyChek[16];
} smp_op_pairing_dhkey_check_t;

typedef struct l2cap_smp_pairing_req_rsp_t
{
    uint16_t  length;
    uint16_t  cid;
    union
    {
        smp_op_pairing_request_t        pairing_request;
        smp_op_pairing_response_t       pairing_response;
    } para;
} l2cap_smp_pairing_req_rsp_t;

typedef struct private_addr_gen_t
{
    uint8_t *private_addr24;
    uint8_t *irk;
} private_addr_gen_t;

extern const uint8_t IRK_FIXED[][16];
extern uint8_t identity_resolving_seed[16];
extern ble_irk_rpa_addr_t *param_rpa;
extern uint8_t resolvable_comp_list[10][3];
extern l2cap_smp_pairing_req_rsp_t app_prefer_smp_format;
extern int8_t (* const Prcss_BLE_CODE_SMP[])(void);
extern void setBLE_ConnTxData_SMP(uint16_t connID, uint8_t L2CAP_Code_SMP, uint8_t *L2CAP_RspDataSMP, uint8_t Length);
extern void memcpy_inv(uint8_t *pDst, uint8_t *pSrc, uint16_t len);

#ifdef __cplusplus
};
#endif
#endif    /* __SMP_H__ */
