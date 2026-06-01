/**
 * @file FreeList.h
 * @brief Standalone free-list allocator with pool growth.
 */

#ifndef CMLIB_FREE_LIST_H_
#define CMLIB_FREE_LIST_H_

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @class FreeList
 * @brief Free-list allocator state.
 */
typedef struct FreeList FreeList;

/**
 * @brief Creates a free-list allocator with one initial pool.
 * @param pool_size Requested initial pool capacity in bytes.
 * @return Initialized allocator, or `ERROR_NO_MEMORY`.
 */
FreeList* free_list_ctor(size_t pool_size);

/**
 * @brief Frees all pools and clears allocator state.
 * @param free_list Allocator pointer; NULL is accepted.
 */
void free_list_dtor(FreeList* free_list);

/**
 * @brief Allocates bytes from a free-list allocator.
 * @param free_list Allocator pointer.
 * @param size Requested byte count.
 * @param alignment Required pointer alignment.
 * @return Allocated pointer or NULL on failure.
 */
void* free_list_allocate(FreeList* free_list, size_t size, size_t alignment);

/**
 * @brief Allocates one object of the requested type from a free list.
 * @param free_list Allocator pointer.
 * @param type Type of object to allocate.
 * @return Allocated pointer or NULL on failure.
 */
#define free_list_allocate_type(free_list, type)                               \
    (free_list_allocate(free_list, sizeof(type), alignof(type)))

/**
 * @brief Deallocates a pointer through a free-list allocator.
 * @param free_list Allocator pointer.
 * @param ptr Pointer previously returned by this allocator, or NULL.
 */
void free_list_deallocate(FreeList* free_list, void* ptr);

/**
 * @brief Dumps free-list pools and free blocks as a DOT graph.
 * @param free_list Allocator pointer, or NULL for an empty graph.
 * @param out Output stream; NULL is accepted as a no-op.
 */
void free_list_dump_dot(const FreeList* free_list, FILE* out);

#endif // CMLIB_FREE_LIST_H_
