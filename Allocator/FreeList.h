#ifndef CMLIB_FREE_LIST_H_
#define CMLIB_FREE_LIST_H_

#include <stdlib.h>

#include "../Logger/Logger.h"
#include "Allocator.h"

typedef struct FreeListFreeBlockHeader FreeListFreeBlockHeader;
struct FreeListFreeBlockHeader
{
    size_t size;
    FreeListFreeBlockHeader* next;
};

typedef struct FreeListOccupiedBlockHeader
{
    size_t size;
} FreeListOccupiedBlockHeader;

typedef struct FreeListMemoryPool FreeListMemoryPool;
struct FreeListMemoryPool
{
    FreeListMemoryPool* next_pool;
    FreeListFreeBlockHeader* next_block;
    void* pool_end;
};

typedef struct FreeList
{
    FreeListMemoryPool* pool;
} FreeList;
DECLARE_RESULT_HEADER(FreeList);

INLINE Result_FreeList free_list_ctor(size_t pool_size);
INLINE void free_list_dtor(FreeList* allocator);
INLINE void* free_list_allocate(FreeList* allocator, size_t size);
INLINE void free_list_free(FreeList* allocator, void* ptr);

INLINE FreeListMemoryPool* free_list_pool_ctor(size_t size);
INLINE void free_list_pool_dtor(FreeListMemoryPool* pool);
INLINE void* free_list_pool_allocate(FreeListMemoryPool* pool, size_t size);
INLINE bool free_list_pool_check_ptr(FreeListMemoryPool* pool, void* ptr);
INLINE bool free_list_pool_free(FreeListMemoryPool* pool, void* ptr);

INLINE Result_FreeList free_list_ctor(size_t pool_size)
{
    FreeListMemoryPool* pool = free_list_pool_ctor(pool_size);

    if (!pool)
    {
        return (Result_FreeList) {
            .error_code = ERROR_NO_MEMORY,
        };
    }

    return (Result_FreeList) {
        .value = (FreeList) {
            .pool = pool,
        },
    };
}

INLINE void free_list_dtor(FreeList* allocator)
{
    if (!allocator)
    {
        return;
    }

    FreeListMemoryPool* curr_pool = allocator->pool;

    while (curr_pool)
    {
        FreeListMemoryPool* next_pool = curr_pool->next_pool;
        free_list_pool_dtor(curr_pool);
        curr_pool = next_pool;
    }

    *allocator = (FreeList){};
}

INLINE void* free_list_allocate(FreeList* allocator, size_t size)
{
    assert(allocator);

    void* allocated = NULL;
    FreeListMemoryPool* prev_pool = NULL;
    FreeListMemoryPool* curr_pool = allocator->pool;

    while (curr_pool && !allocated)
    {
        allocated = free_list_pool_allocate(curr_pool, size);
        prev_pool = curr_pool;
        curr_pool = curr_pool->next_pool;
    }

    if (allocated)
    {
        return allocated;
    }

    FreeListMemoryPool* new_pool = free_list_pool_ctor(size);
    if (!new_pool)
    {
        return NULL;
    }

    prev_pool->next_pool = new_pool;

    return free_list_pool_allocate(new_pool, size);
}

INLINE void free_list_free(FreeList* allocator, void* ptr)
{
    assert(allocator);

    if (!ptr)
    {
        return;
    }

    FreeListMemoryPool* curr_pool = allocator->pool;

    while (curr_pool && !free_list_pool_free(curr_pool, ptr))
    {
        curr_pool = curr_pool->next_pool;
    }
}

INLINE FreeListMemoryPool* free_list_pool_ctor(size_t size)
{
    size = align_size(size);

    auto pool = (FreeListMemoryPool*)calloc(size
        + sizeof(FreeListMemoryPool)
        + sizeof(FreeListOccupiedBlockHeader),
        1);
    FreeListFreeBlockHeader* block = (FreeListFreeBlockHeader*)(pool + 1);

    block->size = size;
    block->next = NULL;

    *block = (FreeListFreeBlockHeader) {
        .size = size,
        .next = NULL,
    };

    void* pool_start = pool + 1;

    *pool = (FreeListMemoryPool) {
        .next_pool = NULL,
        .next_block = block,
        .pool_end = (char*)pool_start + size - sizeof(void*),
    };

    return pool;
}

INLINE void free_list_pool_dtor(FreeListMemoryPool* pool)
{
    free(pool);
}

INLINE void* free_list_pool_allocate(FreeListMemoryPool* pool, size_t size)
{
    assert(pool);

    size = align_size(size);
    FreeListFreeBlockHeader* prev_block = NULL;
    FreeListFreeBlockHeader* curr_block = pool->next_block;

    while (curr_block && curr_block->size < size)
    {
        prev_block = curr_block;
        curr_block = curr_block->next;
    }

    if (!curr_block || curr_block->size < size)
    {
        return NULL;
    }

    FreeListFreeBlockHeader** prev_block_next = NULL;
    if (prev_block)
    {
        prev_block_next = &prev_block->next;
    }
    else
    {
        prev_block_next = &pool->next_block;
    }

    FreeListOccupiedBlockHeader* block = (FreeListOccupiedBlockHeader*)curr_block;
    if (curr_block->size > size + sizeof(FreeListFreeBlockHeader))
    {
        auto new_free = (FreeListFreeBlockHeader*)((char*)(block + 1) + size);
        *new_free = (FreeListFreeBlockHeader) {
            .next = curr_block->next,
            .size = curr_block->size - size - sizeof(FreeListOccupiedBlockHeader),
        };
        *prev_block_next = new_free;
    }
    else
    {
        *prev_block_next = curr_block->next;
    }

    block->size = size;

    void* allocated = block + 1;

    return allocated;
}

INLINE bool free_list_pool_check_ptr(FreeListMemoryPool* pool, void* ptr)
{
    assert(pool);

    if (!ptr)
    {
        return false;
    }

    void* pool_start = pool + 1;

    return ptr == align_ptr(ptr) && pool_start <= ptr && ptr <= pool->pool_end;
}

INLINE bool free_list_pool_free(FreeListMemoryPool* pool, void* ptr)
{
    assert(pool);

    if (!free_list_pool_check_ptr(pool, ptr))
    {
        return false;
    }

    auto block = (FreeListFreeBlockHeader*)((FreeListOccupiedBlockHeader*)ptr - 1);
    block->next = pool->next_block;

    pool->next_block = block;

    return true;
}

#endif // CMLIB_FREE_LIST_H_
