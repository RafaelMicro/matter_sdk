
#ifndef EFD_DEFD_H_
#define EFD_DEFD_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "util_log.h"
/* EnhancedFlashDataset software version number */
#define EFD_SW_VERSION "1.0.0"
#define EFD_SW_VERSION_NUM 0x10000

/*
 * ENV version number defined by user.
 * Please change it when your firmware add a new ENV to default_env_set.
 */
#ifndef EFD_ENV_VER_NUM
#define EFD_ENV_VER_NUM 0
#endif

/* the ENV max name length must less then it */
#ifndef EFD_ENV_NAME_MAX
#define EFD_ENV_NAME_MAX 64
#endif

/* EnhancedFlashDataset debug print function. Must be implement by user. */
#ifdef PRINT_DEBUG
#define EFD_DEBUG(...) info(__VA_ARGS__)
#else
#define EFD_DEBUG(...)
#endif
/* EnhancedFlashDataset routine print function. Must be implement by user. */
#define EFD_INFO(...) info(__VA_ARGS__)
/* EnhancedFlashDataset assert for developer. */
#define EFD_ASSERT(EXPR)                                                                                                           \
    if (!(EXPR))                                                                                                                   \
    {                                                                                                                              \
        \            
        err("assert: %s:%ld\n", __FILE__, __LINE__);                                                                               \
        while (1)                                                                                                                  \
            ;                                                                                                                      \
    }

typedef struct _efd_env
{
    char * key;
    void * value;
    size_t value_len;
} efd_env, *efd_env_t;

/* EnhancedFlashDataset error code */
typedef enum
{
    EFD_NO_ERR,
    EFD_ERASE_ERR,
    EFD_READ_ERR,
    EFD_WRITE_ERR,
    EFD_ENV_NAME_ERR,
    EFD_ENV_NAME_EXIST,
    EFD_ENV_FULL,
    EFD_ENV_INIT_FAILED,
    EFD_ENV_ARG_ERR,
} EfErrCode;

/* the flash sector current status */
typedef enum
{
    EFD_SECTOR_EMPTY,
    EFD_SECTOR_USING,
    EFD_SECTOR_FULL,
} EfSecrorStatus;

enum env_status
{
    ENV_UNUSED,
    ENV_PRE_WRITE,
    ENV_WRITE,
    ENV_PRE_DELETE,
    ENV_DELETED,
    ENV_ERR_HDR,
    ENV_STATUS_NUM,
};
typedef enum env_status env_status_t;

struct env_node_obj
{
    env_status_t status;         /**< ENV node status, @see node_status_t */
    bool crc_is_ok;              /**< ENV node CRC32 check is OK */
    uint8_t name_len;            /**< name length */
    uint32_t magic;              /**< magic word(`K`, `V`, `4`, `0`) */
    uint32_t len;                /**< ENV node total length (header + name + value), must align by EFD_WRITE_GRAN */
    uint32_t value_len;          /**< value length */
    char name[EFD_ENV_NAME_MAX]; /**< name */
    struct
    {
        uint32_t start; /**< ENV node start address */
        uint32_t value; /**< value start address */
    } addr;
} __attribute__((__packed__));
typedef struct env_node_obj * env_node_obj_t;

#ifdef __cplusplus
}
#endif

#endif /* EFD_DEFD_H_ */
