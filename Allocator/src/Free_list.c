#include "Free_list.h"

DECLARE_RESULT_SOURCE(Free_list);

Result_Free_list free_list_ctor(size_t pool_size);
void free_list_dtor(Free_list* allocator);
void* free_list_allocate(Free_list* allocator, size_t size);
void free_list_free(Free_list* allocator, void* ptr);

Free_list_memory_pool* free_list_pool_ctor(size_t size);
void free_list_pool_dtor(Free_list_memory_pool* pool);
void* free_list_pool_allocate(Free_list_memory_pool* pool, size_t size);
bool free_list_pool_check_ptr(Free_list_memory_pool* pool, void* ptr);
bool free_list_pool_free(Free_list_memory_pool* pool, void* ptr);
