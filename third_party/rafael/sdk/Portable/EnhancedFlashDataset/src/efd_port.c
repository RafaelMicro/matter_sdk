#include "cm3_mcu.h"
#include "util_log.h"
#include <EnhancedFlashDataset.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <stdarg.h>
#include <stdint.h>
#include <task.h>

static uint8_t mpcache_buf[0x100] __attribute__((aligned(4)));

/* default environment variables set for user */
static const efd_env default_env_set[] = { { "boot_times", "3", 1 } };
/**
 * Flash port for hardware initialize.
 *
 * @param default_env default ENV set for user
 * @param default_env_size default ENV size
 *
 * @return result
 */
EfErrCode efd_port_init(efd_env const ** default_env, size_t * default_env_size)
{
    EfErrCode result = EFD_NO_ERR;

    *default_env      = default_env_set;
    *default_env_size = sizeof(default_env_set) / sizeof(default_env_set[0]);

    return result;
}

/**
 * Read data from flash.
 * @note This operation's units is word.
 *
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read bytes size
 *
 * @return result
 */
EfErrCode efd_port_read(uint32_t addr, uint32_t * buf, size_t size)
{
    EfErrCode result = EFD_NO_ERR;

    /* You can add your code under here. */
    uint8_t * ptr     = (uint8_t *) buf;
    uint8_t start_pos = 0;
    size_t r_size = 0, r_pos = 0, tmp = size;
    uint32_t tmp_addr = addr;

    start_pos = (addr % 0x100);

    do
    {
        /* code */
        if (tmp == 1)
        {
            while (flash_check_busy())
                ;
            ptr[r_pos] = flash_read_byte(addr);
            while (flash_check_busy())
                ;
            break;
        }

        addr -= start_pos;

        if ((tmp + start_pos) < 0x100)
        {
            r_size = tmp;
        }
        else
        {
            r_size = 0x100 - start_pos;
        }

        while (flash_check_busy())
            ;
        flash_read_page(mpcache_buf, addr);

        while (flash_check_busy())
            ;

        memcpy(&ptr[r_pos], &mpcache_buf[start_pos], r_size);

        tmp -= r_size;
        r_pos += r_size;
        start_pos = 0;
        addr += 0x100;

    } while (tmp > 0);

    return result;
}

/**
 * Erase data on flash.
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */
EfErrCode efd_port_erase(uint32_t addr, size_t size)
{
    EfErrCode result = EFD_NO_ERR;
    // info("erase addr 0x%08X, size %d\n", addr, size);

    /* make sure the start address is a multiple of EFD_ERASE_MIN_SIZE */
    EFD_ASSERT(addr % EFD_ERASE_MIN_SIZE == 0);

    /* You can add your code under here. */
    flash_erase(FLASH_ERASE_SECTOR, addr);
    while (flash_check_busy())
        ;

    return result;
}
/**
 * Write data to flash.
 * @note This operation's units is word.
 * @note This operation must after erase. @see flash_erase.
 *
 * @param addr flash address
 * @param buf the write data buffer
 * @param size write bytes size
 *
 * @return result
 */
EfErrCode efd_port_write(uint32_t addr, const uint32_t * buf, size_t size)
{
    EfErrCode result = EFD_NO_ERR;

    /* You can add your code under here. */
    uint8_t * ptr     = (uint8_t *) buf;
    uint8_t start_pos = 0;
    size_t w_size = 0, w_pos = 0, tmp = size;
    uint32_t tmp_addr = addr;

    start_pos = (addr % 0x100);

    do
    {
        /* code */
        if (tmp == 1)
        {
            while (flash_check_busy())
                ;
            flash_write_byte(addr, ptr[w_pos]);
            while (flash_check_busy())
                ;
            break;
        }

        addr -= start_pos;

        memset(mpcache_buf, 0xFF, 0x100);

        if ((tmp + start_pos) < 0x100)
        {
            w_size = tmp;
        }
        else
        {
            w_size = 0x100 - start_pos;
        }

        memcpy(&mpcache_buf[start_pos], &ptr[w_pos], w_size);
        while (flash_check_busy())
            ;
        flash_write_page(mpcache_buf, addr);

        while (flash_check_busy())
            ;

        tmp -= w_size;
        w_pos += w_size;
        start_pos = 0;
        addr += 0x100;

    } while (tmp > 0);

    return result;
}
/**
 * lock the ENV ram cache
 */
void efd_port_env_lock(void)
{
    /* You can add your code under here. */
    vTaskSuspendAll();
}

/**
 * unlock the ENV ram cache
 */
void efd_port_env_unlock(void)
{
    /* You can add your code under here. */
    xTaskResumeAll();
}

/**
 * This function is print flash debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 *
 */
void efd_log_debug(const char * file, const long line, const char * format, ...)
{

#ifdef PRINT_DEBUG
#if 0
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);

    /* You can add your code under here. */
    info(format);

    va_end(args);
#endif

#endif
}

/**
 * This function is print flash routine info.
 *
 * @param format output format
 * @param ... args
 */
void efd_log_info(const char * format, ...)
{
#if 0
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);

    /* You can add your code under here. */
    info(format);

    va_end(args);
#endif
}
/**
 * This function is print flash non-package info.
 *
 * @param format output format
 * @param ... args
 */
void efd_print(const char * format, ...)
{
#if 0
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);

    /* You can add your code under here. */
    info(format);

    va_end(args);
#endif
}
