#include "FreeList.h"

DECLARE_RESULT_SOURCE(FreeList);

Result_FreeList free_list_ctor(size_t pool_size);
void free_list_dtor(FreeList* allocator);
void* free_list_allocate(FreeList* allocator, size_t size);
void free_list_free(FreeList* allocator, void* ptr);

FreeListMemoryPool* free_list_pool_ctor(size_t size);
void free_list_pool_dtor(FreeListMemoryPool* pool);
void* free_list_pool_allocate(FreeListMemoryPool* pool, size_t size);
bool free_list_pool_check_ptr(FreeListMemoryPool* pool, void* ptr);
bool free_list_pool_free(FreeListMemoryPool* pool, void* ptr);
