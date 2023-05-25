
#include <EnhancedFlashDataset.h>

#ifdef EFD_USING_IAP

/* IAP section backup application section start address in flash */
static uint32_t bak_app_start_addr = 0;

/**
 * Flash IAP function initialize.
 *
 * @return result
 */
EfErrCode efd_iap_init(void)
{
    EfErrCode result = EFD_NO_ERR;

    bak_app_start_addr = EFD_START_ADDR;

#if defined(EFD_USING_ENV)
    bak_app_start_addr += ENV_AREA_SIZE;
#endif

#if defined(EFD_USING_LOG)
    bak_app_start_addr += LOG_AREA_SIZE;
#endif

    return result;
}

/**
 * Erase backup area application data.
 *
 * @param app_size application size
 *
 * @return result
 */
EfErrCode efd_erase_bak_app(size_t app_size)
{
    EfErrCode result = EFD_NO_ERR;

    result = efd_port_erase(efd_get_bak_app_start_addr(), app_size);
    switch (result)
    {
    case EFD_NO_ERR: {
        EFD_INFO("Erased backup area application OK.\n");
        break;
    }
    case EFD_ERASE_ERR: {
        EFD_INFO("Warning: Erase backup area application fault!\n");
        /* will return when erase fault */
        return result;
    }
    }

    return result;
}

/**
 * Erase user old application by using specified erase function.
 *
 * @param user_app_addr application entry address
 * @param app_size application size
 * @param app_erase user specified application erase function
 *
 * @return result
 */
EfErrCode efd_erase_spec_user_app(uint32_t user_app_addr, size_t app_size, EfErrCode (*app_erase)(uint32_t addr, size_t size))
{
    EfErrCode result = EFD_NO_ERR;

    result = app_erase(user_app_addr, app_size);
    switch (result)
    {
    case EFD_NO_ERR: {
        EFD_INFO("Erased user application OK.\n");
        break;
    }
    case EFD_ERASE_ERR: {
        EFD_INFO("Warning: Erase user application fault!\n");
        /* will return when erase fault */
        return result;
    }
    }

    return result;
}

/**
 * Erase user old application by using default `efd_port_erase` function.
 *
 * @param user_app_addr application entry address
 * @param app_size application size
 *
 * @return result
 */
EfErrCode efd_erase_user_app(uint32_t user_app_addr, size_t app_size)
{
    return efd_erase_spec_user_app(user_app_addr, app_size, efd_port_erase);
}

/**
 * Erase old bootloader
 *
 * @param bl_addr bootloader entry address
 * @param bl_size bootloader size
 *
 * @return result
 */
EfErrCode efd_erase_bl(uint32_t bl_addr, size_t bl_size)
{
    EfErrCode result = EFD_NO_ERR;

    result = efd_port_erase(bl_addr, bl_size);
    switch (result)
    {
    case EFD_NO_ERR: {
        EFD_INFO("Erased bootloader OK.\n");
        break;
    }
    case EFD_ERASE_ERR: {
        EFD_INFO("Warning: Erase bootloader fault!\n");
        /* will return when erase fault */
        return result;
    }
    }

    return result;
}

/**
 * Write data of application to backup area.
 *
 * @param data a part of application
 * @param size data size
 * @param cur_size current write application size
 * @param total_size application total size
 *
 * @return result
 */
EfErrCode efd_write_data_to_bak(uint8_t * data, size_t size, size_t * cur_size, size_t total_size)
{
    EfErrCode result = EFD_NO_ERR;

    /* make sure don't write excess data */
    if (*cur_size + size > total_size)
    {
        size = total_size - *cur_size;
    }

    result = efd_port_write(efd_get_bak_app_start_addr() + *cur_size, (uint32_t *) data, size);
    switch (result)
    {
    case EFD_NO_ERR: {
        *cur_size += size;
        EFD_DEBUG("Write data to backup area OK.\n");
        break;
    }
    case EFD_WRITE_ERR: {
        EFD_INFO("Warning: Write data to backup area fault!\n");
        break;
    }
    }

    return result;
}

/**
 * Copy backup area application to application entry by using specified write function.
 *
 * @param user_app_addr application entry address
 * @param app_size application size
 * @param app_write user specified application write function
 *
 * @return result
 */
EfErrCode efd_copy_spec_app_from_bak(uint32_t user_app_addr, size_t app_size,
                                     EfErrCode (*app_write)(uint32_t addr, const uint32_t * buf, size_t size))
{
    size_t cur_size;
    uint32_t app_cur_addr, bak_cur_addr;
    EfErrCode result = EFD_NO_ERR;
    /* 32 words size buffer */
    uint32_t buff[32];

    /* cycle copy data */
    for (cur_size = 0; cur_size < app_size; cur_size += sizeof(buff))
    {
        app_cur_addr = user_app_addr + cur_size;
        bak_cur_addr = efd_get_bak_app_start_addr() + cur_size;
        efd_port_read(bak_cur_addr, buff, sizeof(buff));
        result = app_write(app_cur_addr, buff, sizeof(buff));
        if (result != EFD_NO_ERR)
        {
            break;
        }
    }

    switch (result)
    {
    case EFD_NO_ERR: {
        EFD_INFO("Write data to application entry OK.\n");
        break;
    }
    case EFD_WRITE_ERR: {
        EFD_INFO("Warning: Write data to application entry fault!\n");
        break;
    }
    }

    return result;
}

/**
 * Copy backup area application to application entry by using default `efd_port_write` function.
 *
 * @param user_app_addr application entry address
 * @param app_size application size
 *
 * @return result
 */
EfErrCode efd_copy_app_from_bak(uint32_t user_app_addr, size_t app_size)
{
    return efd_copy_spec_app_from_bak(user_app_addr, app_size, efd_port_write);
}

/**
 * Copy backup area bootloader to bootloader entry.
 *
 * @param bl_addr bootloader entry address
 * @param bl_size bootloader size
 *
 * @return result
 */
EfErrCode efd_copy_bl_from_bak(uint32_t bl_addr, size_t bl_size)
{
    size_t cur_size;
    uint32_t bl_cur_addr, bak_cur_addr;
    EfErrCode result = EFD_NO_ERR;
    /* 32 words buffer */
    uint32_t buff[32];

    /* cycle copy data by 32bytes buffer */
    for (cur_size = 0; cur_size < bl_size; cur_size += sizeof(buff))
    {
        bl_cur_addr  = bl_addr + cur_size;
        bak_cur_addr = efd_get_bak_app_start_addr() + cur_size;
        efd_port_read(bak_cur_addr, buff, sizeof(buff));
        result = efd_port_write(bl_cur_addr, buff, sizeof(buff));
        if (result != EFD_NO_ERR)
        {
            break;
        }
    }

    switch (result)
    {
    case EFD_NO_ERR: {
        EFD_INFO("Write data to bootloader entry OK.\n");
        break;
    }
    case EFD_WRITE_ERR: {
        EFD_INFO("Warning: Write data to bootloader entry fault!\n");
        break;
    }
    }

    return result;
}

/**
 * Get IAP section start address in flash.
 *
 * @return size
 */
uint32_t efd_get_bak_app_start_addr(void)
{
    return bak_app_start_addr;
}

#endif /* EFD_USING_IAP */
