/**
 * @file ArenaResource.h
 * @brief cmlib arena memory resource.
 */

#ifndef CMLIB_ARENA_RESOURCE_H_
#define CMLIB_ARENA_RESOURCE_H_

#include <stddef.h>

#include "Allocator.h"
#include "Arena.h"
#include "Result.h"

/**
 * @class ArenaResource
 * @brief Memory resource managing an arena.
 */
typedef struct ArenaResource
{
    MemoryResource base;
    Arena* arena;
} ArenaResource;

DECLARE_RESULT_HEADER(ArenaResource);

/**
 * @brief Constructs an arena resource with specified capacity.
 *
 * @param capacity
 * @return result object with resource and error_code.
 */
Result_ArenaResource arena_resource_ctor(size_t capacity);

/**
 * @brief Converts existing arena into resource.
 *
 * @param arena
 * @return resource
 */
ArenaResource arena_to_resource(Arena* arena);

/**
 * @brief Destroys arena resource.
 * Do not destroy the same arena twice if you used arena_to_resource.
 *
 * @param resource
 */
void arena_resource_dtor(ArenaResource* resource);

#endif // CMLIB_ARENA_RESOURCE_H_
