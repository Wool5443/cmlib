#ifndef CMLIB_FREE_LIST_H_
#define CMLIB_FREE_LIST_H_

#include <stdlib.h>

#include "../Logger/Logger.h"
#include "Allocator.h"

typedef struct Free_list_free_block_header Free_list_free_block_header;
struct Free_list_free_block_header
{
    size_t size;
    Free_list_free_block_header* next;
};

typedef struct Free_list_occupied_block_header
{
    size_t size;
} Free_list_occupied_block_header;

typedef struct Free_list_memory_pool Free_list_memory_pool;
struct Free_list_memory_pool
{
    Free_list_memory_pool* next_pool;
    Free_list_free_block_header* next_block;
    void* pool_end;
};

typedef struct Free_list
{
    Free_list_memory_pool* pool;
} Free_list;
DECLARE_RESULT_HEADER(Free_list);

INLINE Result_Free_list free_list_ctor(size_t pool_size);
INLINE void free_list_dtor(Free_list* allocator);
INLINE void* free_list_allocate(Free_list* allocator, size_t size);
INLINE void free_list_free(Free_list* allocator, void* ptr);

INLINE Free_list_memory_pool* free_list_pool_ctor(size_t size);
INLINE void free_list_pool_dtor(Free_list_memory_pool* pool);
INLINE void* free_list_pool_allocate(Free_list_memory_pool* pool, size_t size);
INLINE bool free_list_pool_check_ptr(Free_list_memory_pool* pool, void* ptr);
INLINE bool free_list_pool_free(Free_list_memory_pool* pool, void* ptr);

INLINE Result_Free_list free_list_ctor(size_t pool_size)
{
    Free_list_memory_pool* pool = free_list_pool_ctor(pool_size);

    if (!pool)
    {
        return (Result_Free_list) {
            .error_code = ERROR_NO_MEMORY,
        };
    }

    return (Result_Free_list) {
        .value = (Free_list) {
            .pool = pool,
        },
    };
}

INLINE void free_list_dtor(Free_list* allocator)
{
    if (!allocator)
    {
        return;
    }

    Free_list_memory_pool* curr_pool = allocator->pool;

    while (curr_pool)
    {
        Free_list_memory_pool* next_pool = curr_pool->next_pool;
        free_list_pool_dtor(curr_pool);
        curr_pool = next_pool;
    }

    *allocator = (Free_list){};
}

INLINE void* free_list_allocate(Free_list* allocator, size_t size)
{
    if (!allocator)
    {
        int err = ERROR_NULLPTR;
        log_error("Trying to allocate in NULL allocator");
        return NULL;
    }

    void* allocated = NULL;
    Free_list_memory_pool* prev_pool = NULL;
    Free_list_memory_pool* curr_pool = allocator->pool;

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

    Free_list_memory_pool* new_pool = free_list_pool_ctor(size);
    if (!new_pool)
    {
        return NULL;
    }

    prev_pool->next_pool = new_pool;

    return free_list_pool_allocate(new_pool, size);
}

INLINE void free_list_free(Free_list* allocator, void* ptr)
{
    if (!allocator)
    {
        int err = ERROR_NULLPTR;
        log_error("Trying to free in NULL allocator");
        return;
    }

    if (!ptr)
    {
        return;
    }

    Free_list_memory_pool* curr_pool = allocator->pool;

    while (curr_pool && !free_list_pool_free(curr_pool, ptr))
    {
        curr_pool = curr_pool->next_pool;
    }
}

INLINE Free_list_memory_pool* free_list_pool_ctor(size_t size)
{
    size = align_size(size);

    auto pool = (Free_list_memory_pool*)calloc(size
        + sizeof(Free_list_memory_pool)
        + sizeof(Free_list_occupied_block_header),
        1);
    Free_list_free_block_header* block = (Free_list_free_block_header*)(pool + 1);

    block->size = size;
    block->next = NULL;

    *block = (Free_list_free_block_header) {
        .size = size,
        .next = NULL,
    };

    void* pool_start = pool + 1;

    *pool = (Free_list_memory_pool) {
        .next_pool = NULL,
        .next_block = block,
        .pool_end = (char*)pool_start + size - sizeof(void*),
    };

    return pool;
}

INLINE void free_list_pool_dtor(Free_list_memory_pool* pool)
{
    free(pool);
}

INLINE void* free_list_pool_allocate(Free_list_memory_pool* pool, size_t size)
{
    if (!pool)
    {
        int err = ERROR_NULLPTR;
        log_error("Trying to alloacte in NULL pool");
        return NULL;
    }

    size = align_size(size);
    Free_list_free_block_header* prev_block = NULL;
    Free_list_free_block_header* curr_block = pool->next_block;

    while (curr_block && curr_block->size < size)
    {
        prev_block = curr_block;
        curr_block = curr_block->next;
    }

    if (!curr_block || curr_block->size < size)
    {
        return NULL;
    }

    Free_list_free_block_header** prev_block_next = NULL;
    if (prev_block)
    {
        prev_block_next = &prev_block->next;
    }
    else
    {
        prev_block_next = &pool->next_block;
    }

    Free_list_occupied_block_header* block = (Free_list_occupied_block_header*)curr_block;
    if (curr_block->size > size + sizeof(Free_list_free_block_header))
    {
        auto new_free = (Free_list_free_block_header*)((char*)(block + 1) + size);
        *new_free = (Free_list_free_block_header) {
            .next = curr_block->next,
            .size = curr_block->size - size - sizeof(Free_list_occupied_block_header),
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

INLINE bool free_list_pool_check_ptr(Free_list_memory_pool* pool, void* ptr)
{
    if (!pool)
    {
        int err = ERROR_NULLPTR;
        log_error("Trying to check ptr in NULL pool");
        return false;
    }

    if (!ptr)
    {
        return false;
    }

    void* pool_start = pool + 1;

    return ptr == align_ptr(ptr) && pool_start <= ptr && ptr <= pool->pool_end;
}

INLINE bool free_list_pool_free(Free_list_memory_pool* pool, void* ptr)
{
    if (!pool)
    {
        int err = ERROR_NULLPTR;
        log_error("Trying to free from NULL pool");
        return false;
    }

    if (!free_list_pool_check_ptr(pool, ptr))
    {
        return false;
    }

    auto block = (Free_list_free_block_header*)((Free_list_occupied_block_header*)ptr - 1);
    block->next = pool->next_block;

    pool->next_block = block;

    return true;
}

#endif // CMLIB_FREE_LIST_H_
