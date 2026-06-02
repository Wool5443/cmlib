/**
 * @file Arena.h
 * @brief cmlib monotonic arena.
 */

#ifndef CMLIB_ARENA_H_
#define CMLIB_ARENA_H_

#include <stddef.h>

/**
 * @class Arena
 * @brief Monotonic allocator.
 */
typedef struct Arena Arena;

/**
 * @brief Constructs an arena with specified size.
 *
 * @param capacity must be > 0.
 * @return arena or NULL on failure.
 */
Arena* arena_ctor(size_t capacity);

/**
 * @brief Allocates memory in the arena.
 *
 * @param arena
 * @param size
 * @param alignment
 *
 * @return pointer to allocated memory or NULL on failure.
 */
void* arena_allocate(Arena* arena, size_t size, size_t alignment);

/**
 * @brief Allocates memory for specific type in the arena.
 *
 * @param arena
 * @param type
 *
 * @return pointer to allocated memory or NULL on failure.
 */
#define arena_allocate_type(arena, type)                                       \
    (arena_allocate(arena, sizeof(type), alignof(type)))

/**
 * @brief Dellocates memory in arena, basically a no-op.
 *
 * @param arena
 * @param ptr
 */
void arena_deallocate(Arena* arena, void* ptr);

/**
 * @brief Clears the arena for reuse.
 *
 * @param arena
 */
void arena_flush(Arena* arena);

/**
 * @brief Frees the arena's memory.
 *
 * @param arena
 */
void arena_dtor(Arena* arena);

#endif // CMLIB_ARENA_H_
