#define MBEDTLS_CONFIG_H

/* System support */
#define MBEDTLS_HAVE_ASM

/* mbed TLS feature support */
#define MBEDTLS_AES_ROM_TABLES
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
#define MBEDTLS_ECP_NIST_OPTIM
#define MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED
#define MBEDTLS_SSL_MAX_FRAGMENT_LENGTH
#define MBEDTLS_SSL_PROTO_TLS1_2
#define MBEDTLS_SSL_PROTO_DTLS
#define MBEDTLS_SSL_DTLS_ANTI_REPLAY
#define MBEDTLS_SSL_DTLS_HELLO_VERIFY
#define MBEDTLS_SSL_EXPORT_KEYS

/* mbed TLS modules */
#define MBEDTLS_AES_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_CCM_C
#define MBEDTLS_CIPHER_C
#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_CMAC_C
#define MBEDTLS_ECJPAKE_C
#define MBEDTLS_ECP_C
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_HMAC_DRBG_C
#define MBEDTLS_MD_C
#define MBEDTLS_OID_C
#define MBEDTLS_PK_C
#define MBEDTLS_PK_PARSE_C
#define MBEDTLS_SHA256_C
#define MBEDTLS_SSL_COOKIE_C
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_SRV_C
#define MBEDTLS_SSL_TLS_C

/* For tests using ssl-opt.sh */
// #define MBEDTLS_NET_C
// #define MBEDTLS_TIMING_C

/* Save RAM at the expense of ROM */
#define MBEDTLS_AES_ROM_TABLES

/* Save RAM by adjusting to our exact needs */
#define MBEDTLS_ECP_MAX_BITS             256
#define MBEDTLS_MPI_MAX_SIZE              32 // 256 bits is 32 bytes

/* Save ROM and a few bytes of RAM by specifying our own ciphersuite list */
#define MBEDTLS_SSL_CIPHERSUITES MBEDTLS_TLS_ECJPAKE_WITH_AES_128_CCM_8

#include "mbedtls/check_config.h"
#include "sys_arch.h"

// <<< Use Configuration Wizard in Context Menu >>>
// <h> TLS/DTLS configuration

// <o MBEDTLS_SSL_CIPHERSUITES> Complete list of ciphersuites to use, in order
// of preference. <i> Default: MBEDTLS_TLS_ECJPAKE_WITH_AES_128_CCM_8 <i>
// Complete list of ciphersuites to use, in order of preference. <i> The value
// of this configuration should be updated for the application needs.
#define MBEDTLS_SSL_CIPHERSUITES MBEDTLS_TLS_ECJPAKE_WITH_AES_128_CCM_8

// <o MBEDTLS_SSL_MAX_CONTENT_LEN> Maximum TLS/DTLS fragment length in bytes.
// <i> Default: 768
// <i> The size configured here determines the size of each of the two
// <i> internal I/O buffers used in mbedTLS when sending and receiving data.
#if OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE
#define MBEDTLS_SSL_MAX_CONTENT_LEN           \
    900 /**< Maximum fragment length in bytes \
         */
#else
#define MBEDTLS_SSL_MAX_CONTENT_LEN           \
    768 /**< Maximum fragment length in bytes \
         */
#endif

// Enable necessary mbedtls functions when ECDSA is enabled
#if OPENTHREAD_CONFIG_ECDSA_ENABLE
#define MBEDTLS_BASE64_C
#define MBEDTLS_ECDH_C
#define MBEDTLS_ECDSA_C
#define MBEDTLS_ECDSA_DETERMINISTIC
#define MBEDTLS_HMAC_DRBG_C
#define MBEDTLS_OID_C
#define MBEDTLS_PEM_PARSE_C
#define MBEDTLS_PK_WRITE_C
#define MBEDTLS_HKDF_C
#define MBEDTLS_PKCS5_C
#define MBEDTLS_X509_CREATE_C
#define MBEDTLS_X509_CSR_WRITE_C
#endif
// <q SL_MBEDTLS_SSL_MAX_FRAGMENT_LENGTH> Enable support for RFC 6066
// max_fragment_length extension in SSL. <i> Default: 1 <i> Enable support for
// RFC 6066 max_fragment_length extension in SSL.
#define SL_MBEDTLS_SSL_MAX_FRAGMENT_LENGTH 1

// <q SL_MBEDTLS_SSL_EXPORT_KEYS> Enable support for exporting key block and
// master secret. <i> Default: 1 <i> Enable support for exporting key block and
// master secret. <i> This is required for certain users of TLS, e.g. EAP-TLS.
#define SL_MBEDTLS_SSL_EXPORT_KEYS 1

// <q SL_MBEDTLS_KEY_EXCHANGE_PSK_ENABLED> Enable the PSK based ciphersuite
// modes in SSL / TLS. <i> Default: 0 <i> Enable the PSK based ciphersuite modes
// in SSL / TLS.
#define SL_MBEDTLS_KEY_EXCHANGE_PSK_ENABLED 1

// <q SL_MBEDTLS_KEY_EXCHANGE_ECDHE_PSK_ENABLED> Enable the ECDHE-PSK based
// ciphersuite modes in SSL / TLS. <i> Default: 0 <i> Enable the ECDHE-PSK based
// ciphersuite modes in SSL / TLS.
#define SL_MBEDTLS_KEY_EXCHANGE_ECDHE_PSK_ENABLED 0

// <q SL_MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED> Enable the ECDHE-ECDSA based
// ciphersuite modes in SSL / TLS. <i> Default: 0 <i> Enable the ECDHE-ECDSA
// based ciphersuite modes in SSL / TLS.
#define SL_MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED 1

// <q SL_MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED> Enable the ECDH-ECDSA based
// ciphersuite modes in SSL / TLS. <i> Default: 0 <i> Enable the ECDH-ECDSA
// based ciphersuite modes in SSL / TLS.
#define SL_MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED 0

// </h>
// <<< end of configuration section >>>

// Convert CMSIS Markup config defines to mbedTLS specific config defines

// Custom defines can be placed here before check_config.h is included.

#include "mbedtls/config_psa.h"

#include "mbedtls/check_config.h"