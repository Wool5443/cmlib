#include "FreeList.h"

FreeListMemoryPool* free_list_pool_ctor(size_t size);
void free_list_pool_dtor(FreeListMemoryPool* pool);
void* free_list_memory_pool_allocate(FreeListMemoryPool* pool, size_t size);
bool free_list_memory_pool_check_ptr(FreeListMemoryPool* pool, void* ptr);
void free_list_memory_pool_free(FreeListMemoryPool* pool, void* ptr);
