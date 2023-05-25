
#ifndef ENHANCEDFLASHDTATSET_H_
#define ENHANCEDFLASHDTATSET_H_

#include <efd_cfg.h>
#include <efd_def.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EnhancedFlashDataset.c */
EfErrCode enhanced_flash_dataset_init(void);

#ifdef EFD_USING_ENV
/* only supported on efd_env.c */
size_t efd_get_env_blob(const char * key, void * value_buf, size_t buf_len, size_t * saved_value_len);
bool efd_get_env_obj(const char * key, env_node_obj_t env);
size_t efd_read_env_value(env_node_obj_t env, uint8_t * value_buf, size_t buf_len);
EfErrCode efd_set_env_blob(const char * key, const void * value_buf, size_t buf_len);
void info_env(void);

/* efd_env.c, efd_env_legacy_wl.c and efd_env_legacy.c */
EfErrCode efd_load_env(void);
void efd_print_env(void);
char * efd_get_env(const char * key);
EfErrCode efd_set_env(const char * key, const char * value);
EfErrCode efd_del_env(const char * key);
EfErrCode efd_save_env(void);
EfErrCode efd_env_set_default(void);
size_t efd_get_env_write_bytes(void);
EfErrCode efd_set_and_save_env(const char * key, const char * value);
EfErrCode efd_del_and_save_env(const char * key);
#endif

#ifdef EFD_USING_IAP
/* efd_iap.c */
EfErrCode efd_erase_bak_app(size_t app_size);
EfErrCode efd_erase_user_app(uint32_t user_app_addr, size_t user_app_size);
EfErrCode efd_erase_spec_user_app(uint32_t user_app_addr, size_t app_size, EfErrCode (*app_erase)(uint32_t addr, size_t size));
EfErrCode efd_erase_bl(uint32_t bl_addr, size_t bl_size);
EfErrCode efd_write_data_to_bak(uint8_t * data, size_t size, size_t * cur_size, size_t total_size);
EfErrCode efd_copy_app_from_bak(uint32_t user_app_addr, size_t app_size);
EfErrCode efd_copy_spec_app_from_bak(uint32_t user_app_addr, size_t app_size,
                                     EfErrCode (*app_write)(uint32_t addr, const uint32_t * buf, size_t size));
EfErrCode efd_copy_bl_from_bak(uint32_t bl_addr, size_t bl_size);
uint32_t efd_get_bak_app_start_addr(void);
#endif

#ifdef EFD_USING_LOG
/* efd_log.c */
EfErrCode efd_log_read(size_t index, uint32_t * log, size_t size);
EfErrCode efd_log_write(const uint32_t * log, size_t size);
EfErrCode efd_log_clean(void);
size_t efd_log_get_used_size(void);
size_t efd_log_get_total_size(void);
#endif

/* efd_utils.c */
uint32_t efd_calc_crc32(uint32_t crc, const void * buf, size_t size);

/* efd_port.c */
EfErrCode efd_port_read(uint32_t addr, uint32_t * buf, size_t size);
EfErrCode efd_port_erase(uint32_t addr, size_t size);
EfErrCode efd_port_write(uint32_t addr, const uint32_t * buf, size_t size);
void efd_port_env_lock(void);
void efd_port_env_unlock(void);
void efd_log_debug(const char * file, const long line, const char * format, ...);
void efd_log_info(const char * format, ...);
void efd_print(const char * format, ...);

#ifdef __cplusplus
}
#endif

#endif /* ENHANCEDFLASHDTATSET_H_ */
