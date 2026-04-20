/**
 * @file FreeListResource.h
 * @brief Polymorphic memory resource wrapper for `FreeList`.
 */

#ifndef CMLIB_FREE_LIST_RESOURCE_H_
#define CMLIB_FREE_LIST_RESOURCE_H_

#include "Allocator.h"
#include "FreeList.h"

/**
 * @brief Memory resource wrapper around a standalone `FreeList`.
 */
typedef struct FreeListResource
{
    MemoryResource base; /**< @brief Embedded base resource. */
    FreeList free_list;  /**< @brief Wrapped free-list allocator. */
} FreeListResource;

DECLARE_RESULT_HEADER(FreeListResource);

/**
 * @brief Creates a memory resource backed by a `FreeList`.
 * @param pool_size Requested initial pool capacity in bytes.
 * @return Initialized resource, or `ERROR_NO_MEMORY`.
 */
Result_FreeListResource free_list_resource_ctor(size_t pool_size);

/**
 * @brief Moves an existing free list into a memory resource wrapper.
 * @param free_list Free list to move from; cleared on success. NULL yields an
 * empty resource.
 * @return Resource that owns the moved free-list pools.
 */
FreeListResource free_list_to_resource(FreeList* free_list);

/**
 * @brief Destroys wrapped free-list storage.
 * @param resource Resource pointer; NULL is accepted.
 */
void free_list_resource_dtor(FreeListResource* resource);

#endif // CMLIB_FREE_LIST_RESOURCE_H_
