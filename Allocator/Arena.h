/**
 * @file Arena.h
 * @brief Monotonic arena allocator.
 *
 * Arena allocations are bump-pointer based, individually non-freeable, and
 * invalidated as a group by `arena_flush` or `arena_dtor`.
 */

#ifndef CMLIB_ARENA_H_
#define CMLIB_ARENA_H_

#include <stddef.h>
#include <stdlib.h>

#include "Result.h"

/**
 * @brief Monotonic arena allocator state.
 */
typedef struct Arena
{
    char* buffer;  /**< Start of owned storage. */
    char* current; /**< Next available byte. */
    char* end;     /**< One-past-end pointer. */
} Arena;

DECLARE_RESULT_HEADER(Arena);

/**
 * @brief Creates an arena with the requested capacity.
 * @param capacity Storage capacity in bytes.
 * @return Initialized arena, or `ERROR_NO_MEMORY`.
 */
Result_Arena arena_ctor(size_t capacity);

/**
 * @brief Allocates bytes from an arena.
 * @param arena Arena allocator.
 * @param size Requested byte count.
 * @param alignment Required pointer alignment.
 * @return Aligned pointer, or NULL on invalid input or exhaustion.
 */
void* arena_allocate(Arena* arena, size_t size, size_t alignment);

/**
 * @brief Allocates one object of the requested type from an arena.
 * @param arena Allocator pointer.
 * @param type Type of object to allocate.
 * @return Allocated pointer or NULL on failure.
 */
#define arena_allocate_type(arena, type)                                       \
    (arena_allocate(arena, sizeof(type), alignof(type)))

/**
 * @brief Accepts an arena allocation back as a no-op.
 * @param arena Arena allocator.
 * @param ptr Pointer previously returned by arena allocation.
 */
void arena_deallocate(Arena* arena, void* ptr);

/**
 * @brief Resets all arena allocations without freeing storage.
 * @param arena Arena allocator.
 */
void arena_flush(Arena* arena);

/**
 * @brief Frees arena storage and clears state.
 * @param arena Arena allocator.
 */
void arena_dtor(Arena* arena);

#endif // CMLIB_ARENA_H_
