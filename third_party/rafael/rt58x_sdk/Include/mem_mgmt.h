// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
#ifndef __MEM_MGMT_H__
#define __MEM_MGMT_H__

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
//                Include (Better to prevent)
//=============================================================================
#include <stdio.h>
#include <stdint.h>

//=============================================================================
//                Public Definitions of const value
//=============================================================================
#define MEM_MGMT_HEAP_SIZE           ((size_t)(12 << 10))
//=============================================================================
//                Public ENUM
//=============================================================================

//=============================================================================
//                Public Struct
//=============================================================================


//=============================================================================
//                Public Function Declaration
//=============================================================================

uint32_t mem_GetHeapStartAddress(void);
uint32_t mem_GetHeapEndAddress(void);
size_t mem_GetFreeHeapSize(void);
size_t mem_GetMinimumEverFreeHeapSize(void);

//==================================
//     Memory functions
//==================================
void mem_mgmt_show_info(void);

/** Memory allocation
 * @param u32_size memory size in bytes
 */
//void *mem_malloc_fn(uint32_t u32_size, const char *pc_func_ptr, uint32_t u32_line);
#define mem_malloc(len) sys_malloc_fn(len, __FUNCTION__, __LINE__)

/** Memory free
 * @param p_pointer
 */
//void mem_free_fn(void *p_pointer, const char *pc_func_ptr, uint32_t u32_line);
#define mem_free(ptr) sys_free_fn(ptr, __FUNCTION__, __LINE__)

#ifdef __cplusplus
};
#endif
#endif /* __MEM_MGMT_H__ */
