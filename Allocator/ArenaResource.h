/**
 * @file ArenaResource.h
 * @brief Memory resource adapter for monotonic arenas.
 */

#ifndef CMLIB_ARENA_RESOURCE_H_
#define CMLIB_ARENA_RESOURCE_H_

#include "Allocator.h"
#include "Arena.h"
#include "Result.h"

/**
 * @brief Polymorphic memory resource backed by an `Arena`.
 */
typedef struct ArenaResource
{
    MemoryResource base; /**< Embedded base resource. */
    Arena* arena;        /**< Backing arena allocator. */
} ArenaResource;

DECLARE_RESULT_HEADER(ArenaResource);

/**
 * @brief Creates an arena-backed memory resource.
 * @param capacity Arena capacity in bytes.
 * @return Initialized resource, or `ERROR_NO_MEMORY`.
 */
Result_ArenaResource arena_resource_ctor(size_t capacity);

/**
 * @brief Moves an existing arena into a memory resource wrapper.
 * @param arena Arena to move from; cleared on success. NULL yields an empty
 * resource.
 * @return Resource that owns the moved arena storage.
 */
ArenaResource arena_to_resource(Arena* arena);

/**
 * @brief Destroys an arena-backed memory resource.
 * @param resource Resource pointer; NULL is accepted.
 */
void arena_resource_dtor(ArenaResource* resource);

#endif // CMLIB_ARENA_RESOURCE_H_
