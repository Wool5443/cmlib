/**
 * @file Arena.h
 * @author Misha Solodilov (mihsolodilov2015@gmail.com)
 * @brief Header file for the Arena memory allocator.
 *
 * This header defines the data structures and functions required to implement
 * an arena-based memory allocator. An arena allocator is a simple memory
 * allocator that allocates a large block of memory upfront and then provides a
 * simple way to allocate memory from that block in a way that is efficient for
 * certain use cases. It is particularly useful for scenarios where many
 * allocations and deallocations of similar size occur.
 *
 * The arena allocator works by maintaining a single buffer of memory and a
 * "current" pointer that tracks the next available space in the buffer. When
 * memory is allocated, the current pointer is incremented, and the allocator
 * ensures that no allocations exceed the capacity of the arena. If needed, the
 * arena can be flushed, which resets the current pointer to the beginning of
 * the buffer.
 *
 * @version 1.0
 * @date 12.06.2025
 *
 * @copyright Copyright (c) 2025
 */

#ifndef CMLIB_ARENA_H_
#define CMLIB_ARENA_H_

#include <stddef.h>

#include "../Logger/Logger.h"
#include "Allocator.h"

/**
 * @brief Structure to represent an arena allocator.
 *
 * The arena allocator maintains a buffer of memory with pointers to the current
 * and end positions in the buffer. Memory is allocated sequentially from this
 * buffer.
 */
typedef struct Arena
{
    char* buffer;  /**< Pointer to the start of the buffer */
    char* current; /**< Pointer to the current available position in the buffer
                    */
    char* end;     /**< Pointer to the end of the buffer */
} Arena;

DECLARE_RESULT_HEADER(Arena);

/**
 * @brief Constructor for the Arena allocator.
 *
 * This function initializes the arena allocator with a buffer of the specified
 * capacity. The buffer is allocated using `calloc` and initialized to zero.
 *
 * @param capacity Size of the arena's memory buffer.
 * @return A result containing either the successfully created arena or an error
 * code.
 */
INLINE Result_Arena arena_ctor(size_t capacity);

/**
 * @brief Allocates memory from the Arena allocator.
 *
 * This function allocates a block of memory from the arena's buffer. It
 * advances the `current` pointer by the requested size, ensuring that the
 * memory is allocated sequentially from the buffer.
 *
 * @param arena Pointer to the arena allocator.
 * @param size Size of the memory block to be allocated.
 * @return Pointer to the allocated memory block, or NULL if allocation fails
 * due to insufficient space.
 */
INLINE void* arena_allocate(Arena* arena, size_t size);

/**
 * @brief Resets the arena allocator.
 *
 * This function resets the arena's `current` pointer to the beginning of the
 * buffer, effectively "flushing" all previously allocated memory. It does not
 * free any memory but prepares the arena for reuse.
 *
 * @param arena Pointer to the arena allocator.
 */
INLINE void arena_flush(Arena* arena);

/**
 * @brief Destructor for the Arena allocator.
 *
 * This function deallocates the memory buffer used by the arena allocator.
 *
 * @param arena Pointer to the arena allocator to be destroyed.
 */
INLINE void arena_dtor(Arena* arena);

INLINE Result_Arena arena_ctor(size_t capacity)
{
    ERROR_CHECKING();

    char* buf = calloc(1, capacity);

    if (!buf)
    {
        HANDLE_ERRNO_ERROR(ERROR_NO_MEMORY, "Failed to calloc arena buf: %s");
    }

    return Result_Arena_ctor(
        (Arena){
            buf,
            buf,
            buf + capacity,
        },
        EVERYTHING_FINE);

    ERROR_CASE

    return Result_Arena_ctor((Arena){}, err);
}

INLINE void* arena_allocate(Arena* arena, size_t size)
{
    if (!arena)
    {
        int err = ERROR_NULLPTR;
        log_error("Trying to flush NULL arena");
        return NULL;
    }

    size = align_size(size);

    if (arena->current + size >= arena->end)
    {
        return NULL;
    }

    void* allocated = arena->current;
    arena->current += size;

    return allocated;
}

INLINE void arena_flush(Arena* arena)
{
    if (!arena)
    {
        int err = ERROR_NULLPTR;
        log_error("Trying to flush NULL arena");
        return;
    }

    arena->current = arena->buffer;
}

INLINE void arena_dtor(Arena* arena)
{
    if (arena)
    {
        free(arena->buffer);
        arena->buffer = NULL;
    }
}

#endif // CMLIB_ARENA_H_
