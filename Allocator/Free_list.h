/**
 * @file Free_list.h
 * @author Misha Solodilov (mihsolodilov2015@gmail.com)
 * @brief Header file for the Free List memory allocator.
 *
 * This header defines the data structures and functions required to implement a
 * free list-based memory allocator. The allocator uses multiple memory pools to
 * manage blocks of memory, allowing for efficient allocation and deallocation
 * of memory in a system with potentially varying allocation sizes. It provides
 * functions for memory pool creation, memory allocation, and memory freeing,
 * ensuring that blocks of memory are reused efficiently to reduce
 * fragmentation.
 *
 * The allocator works by managing free blocks of memory using a linked list
 * structure. When a memory block is allocated, the corresponding free block is
 * removed from the list. When memory is freed, it is added back to the list of
 * free blocks.
 *
 * @version 1.0
 * @date 12.06.2025
 *
 * @copyright Copyright (c) 2025
 */

#ifndef CMLIB_FREE_LIST_H_
#define CMLIB_FREE_LIST_H_

#include <stdlib.h>

#include "../Logger/Logger.h"
#include "Allocator.h"

/**
 * @brief Structure to represent a free block in the free list.
 *
 * This structure holds the size of the block and a pointer to the next free
 * block in the list.
 */
typedef struct Free_list_free_block_header Free_list_free_block_header;
struct Free_list_free_block_header
{
    size_t size;                       /**< Size of the free block */
    Free_list_free_block_header* next; /**< Pointer to the next free block */
};

/**
 * @brief Structure to represent an occupied block header.
 *
 * This structure holds the size of an allocated block of memory.
 */
typedef struct Free_list_occupied_block_header
{
    size_t size; /**< Size of the occupied block */
} Free_list_occupied_block_header;

/**
 * @brief Structure to represent a memory pool in the allocator.
 *
 * This structure holds a pointer to the next pool, a pointer to the next free
 * block in the pool, and a pointer to the end of the pool.
 */
typedef struct Free_list_memory_pool Free_list_memory_pool;
struct Free_list_memory_pool
{
    Free_list_memory_pool* next_pool; /**< Pointer to the next memory pool */
    Free_list_free_block_header* next_block; /**< Pointer to the next free block
                                                in the pool */
    void* pool_end; /**< Pointer to the end of the pool */
};

/**
 * @brief Main structure representing the free list memory allocator.
 *
 * This structure holds a pointer to the first memory pool in the allocator.
 */
typedef struct Free_list
{
    Free_list_memory_pool* pool; /**< Pointer to the first memory pool */
} Free_list;

DECLARE_RESULT_HEADER(Free_list);

/**
 * @brief Constructor for the Free List allocator.
 *
 * This function creates and initializes the Free List allocator with a
 * specified pool size.
 *
 * @param pool_size Size of the memory pool to be created.
 * @return A result containing either the successfully created allocator or an
 * error code.
 */
INLINE Result_Free_list free_list_ctor(size_t pool_size);

/**
 * @brief Destructor for the Free List allocator.
 *
 * This function deallocates all memory pools managed by the Free List
 * allocator.
 *
 * @param allocator Pointer to the allocator to be destroyed.
 */
INLINE void free_list_dtor(Free_list* allocator);

/**
 * @brief Allocates memory from the Free List allocator.
 *
 * This function allocates a block of memory from the Free List allocator. It
 * first tries to allocate from the existing memory pools. If no suitable free
 * block is available, a new memory pool is created.
 *
 * @param allocator Pointer to the allocator.
 * @param size Size of the memory block to be allocated.
 * @return Pointer to the allocated memory block, or NULL if allocation fails.
 */
INLINE void* free_list_allocate(Free_list* allocator, size_t size);

/**
 * @brief Frees a previously allocated block of memory.
 *
 * This function frees a block of memory previously allocated by the Free List
 * allocator. It adds the block back to the free list for reuse.
 *
 * @param allocator Pointer to the allocator.
 * @param ptr Pointer to the memory block to be freed.
 */
INLINE void free_list_free(Free_list* allocator, void* ptr);

/**
 * @brief Constructor for a memory pool.
 *
 * This function creates and initializes a memory pool with the specified size.
 *
 * @param size Size of the memory pool to be created.
 * @return Pointer to the created memory pool.
 */
INLINE Free_list_memory_pool* free_list_pool_ctor(size_t size);

/**
 * @brief Destructor for a memory pool.
 *
 * This function deallocates the memory pool and its associated memory blocks.
 *
 * @param pool Pointer to the memory pool to be destroyed.
 */
INLINE void free_list_pool_dtor(Free_list_memory_pool* pool);

/**
 * @brief Allocates memory from a memory pool.
 *
 * This function allocates a block of memory from the specified memory pool. If
 * no suitable free block is found, it returns NULL.
 *
 * @param pool Pointer to the memory pool.
 * @param size Size of the memory block to be allocated.
 * @return Pointer to the allocated memory block, or NULL if allocation fails.
 */
INLINE void* free_list_pool_allocate(Free_list_memory_pool* pool, size_t size);

/**
 * @brief Checks if a pointer is valid within a memory pool.
 *
 * This function checks if a pointer lies within the valid range of a memory
 * pool.
 *
 * @param pool Pointer to the memory pool.
 * @param ptr Pointer to the memory block to be checked.
 * @return True if the pointer is valid, false otherwise.
 */
INLINE bool free_list_pool_check_ptr(Free_list_memory_pool* pool, void* ptr);

/**
 * @brief Frees a memory block within a memory pool.
 *
 * This function frees a previously allocated memory block and returns it to the
 * pool of free blocks.
 *
 * @param pool Pointer to the memory pool.
 * @param ptr Pointer to the memory block to be freed.
 * @return True if the memory was successfully freed, false otherwise.
 */
INLINE bool free_list_pool_free(Free_list_memory_pool* pool, void* ptr);

INLINE Result_Free_list free_list_ctor(size_t pool_size)
{
    Free_list_memory_pool* pool = free_list_pool_ctor(pool_size);

    if (!pool)
    {
        return (Result_Free_list){
            .error_code = ERROR_NO_MEMORY,
        };
    }

    return (Result_Free_list){
        .value =
            (Free_list){
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

    auto pool = (Free_list_memory_pool*)calloc(
        size + sizeof(Free_list_memory_pool)
            + sizeof(Free_list_occupied_block_header),
        1);
    Free_list_free_block_header* block =
        (Free_list_free_block_header*)(pool + 1);

    block->size = size;
    block->next = NULL;

    *block = (Free_list_free_block_header){
        .size = size,
        .next = NULL,
    };

    void* pool_start = pool + 1;

    *pool = (Free_list_memory_pool){
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

    Free_list_occupied_block_header* block =
        (Free_list_occupied_block_header*)curr_block;
    if (curr_block->size > size + sizeof(Free_list_free_block_header))
    {
        auto new_free =
            (Free_list_free_block_header*)((char*)(block + 1) + size);
        *new_free = (Free_list_free_block_header){
            .next = curr_block->next,
            .size = curr_block->size - size
                    - sizeof(Free_list_occupied_block_header),
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

    auto block =
        (Free_list_free_block_header*)((Free_list_occupied_block_header*)ptr
                                       - 1);
    block->next = pool->next_block;

    pool->next_block = block;

    return true;
}

#endif // CMLIB_FREE_LIST_H_
