#include <EnhancedFlashDataset.h>

#if !defined(EFD_START_ADDR)
#error "Please configure backup area start address (in efd_cfg.h)"
#endif

#if !defined(EFD_ERASE_MIN_SIZE)
#error "Please configure minimum size of flash erasure (in efd_cfg.h)"
#endif

/**
 * Enhanced FlashDataset system initialize.
 *
 * @return result
 */
EfErrCode enhanced_flash_dataset_init(void)
{
    extern EfErrCode efd_port_init(efd_env const ** default_env, size_t * default_env_size);
    extern EfErrCode efd_env_init(efd_env const * default_env, size_t default_env_size);
    extern EfErrCode efd_iap_init(void);
    extern EfErrCode efd_log_init(void);

    size_t default_env_set_size = 0;
    const efd_env * default_env_set;
    EfErrCode result    = EFD_NO_ERR;
    static bool init_ok = false;

    if (init_ok)
    {
        return EFD_NO_ERR;
    }

    result = efd_port_init(&default_env_set, &default_env_set_size);

#ifdef EFD_USING_ENV
    if (result == EFD_NO_ERR)
    {
        result = efd_env_init(default_env_set, default_env_set_size);
    }
#endif

#ifdef EFD_USING_IAP
    if (result == EFD_NO_ERR)
    {
        result = efd_iap_init();
    }
#endif

#ifdef EFD_USING_LOG
    if (result == EFD_NO_ERR)
    {
        result = efd_log_init();
    }
#endif

    if (result == EFD_NO_ERR)
    {
        init_ok = true;
        EFD_INFO("EnhancedFlashDataset V%s is initialize success.\n", EFD_SW_VERSION);
    }
    else
    {
        EFD_INFO("EnhancedFlashDataset V%s is initialize fail.\n", EFD_SW_VERSION);
    }
    return result;
}
