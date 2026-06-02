/**
 * @file FreeList.h
 * @brief cmlib free-list allocator.
 */

#ifndef CMLIB_FREE_LIST_H_
#define CMLIB_FREE_LIST_H_

#include <stdio.h>

/**
 * @class FreeList
 * @brief Free-list allocator like malloc.
 */
typedef struct FreeList FreeList;

/**
 * @brief Constructs a free-list with specified pool_size.
 *
 * @param pool_size must be > 0.
 * @return free-list or NULL on failure.
 */
FreeList* free_list_ctor(size_t pool_size);

/**
 * @brief Frees the free-list's memory.
 *
 * @param free_list
 */
void free_list_dtor(FreeList* free_list);

/**
 * @brief Allocates memory in the free-list.
 *
 * @param free_list
 * @param size
 * @param alignment
 *
 * @return pointer to allocated memory or NULL on failure.
 */
void* free_list_allocate(FreeList* free_list, size_t size, size_t alignment);

/**
 * @brief Allocates memory for specific type in the free-list.
 *
 * @param free_list
 * @param type
 *
 * @return pointer to allocated memory or NULL on failure.
 */
#define free_list_allocate_type(free_list, type)                               \
    (free_list_allocate(free_list, sizeof(type), alignof(type)))

/**
 * @brief Deallocates memory in the free-list.
 *
 * @param free_list
 * @param ptr
 */
void free_list_deallocate(FreeList* free_list, void* ptr);

/**
 * @brief Dumps free-list internals in dot format.
 *
 * @param free_list
 * @param out
 */
void free_list_dump_dot(const FreeList* free_list, FILE* out);

#endif // CMLIB_FREE_LIST_H_
