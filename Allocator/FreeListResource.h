/**
 * @file FreeListResource.h
 * @brief cmlib free-list memory resource.
 */

#ifndef CMLIB_FREE_LIST_RESOURCE_H_
#define CMLIB_FREE_LIST_RESOURCE_H_

#include <stddef.h>

#include "Allocator.h"
#include "FreeList.h"
#include "Result.h"

/**
 * @class FreeListResource
 * @brief Memory resource managing a free-list.
 */
typedef struct FreeListResource
{
    MemoryResource base;
    FreeList* free_list;
} FreeListResource;

DECLARE_RESULT_HEADER(FreeListResource);

/**
 * @brief Constructs a free-list resource with specified pool size.
 *
 * @param pool_size
 * @return result object with resource and error_code.
 */
Result_FreeListResource free_list_resource_ctor(size_t pool_size);

/**
 * @brief Converts existing free-list into resource.
 *
 * @param free_list
 * @return resource
 */
FreeListResource free_list_to_resource(FreeList* free_list);

/**
 * @brief Destroys free-list resource.
 * Do not destroy the same free-list twice if you used free_list_to_resource.
 *
 * @param resource
 */
void free_list_resource_dtor(FreeListResource* resource);

#endif // CMLIB_FREE_LIST_RESOURCE_H_
