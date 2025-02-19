#ifndef CMLIB_ARENA_H_
#define CMLIB_ARENA_H_

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "../Logger/Logger.h"
#include "../common.h"

typedef struct Arena
{
    char* buffer;
    char* current;
    char* end;
} Arena;

DECLARE_RESULT_HEADER(Arena);

INLINE Result_Arena arena_ctor(size_t capacity);
INLINE void* arena_allocate(Arena* restrict arena, size_t size);
INLINE void arena_flush(Arena* restrict arena);
INLINE void arena_dtor(Arena* restrict arena);

INLINE Result_Arena arena_ctor(size_t capacity)
{
    ERROR_CHECKING();

    char* buf = calloc(1, capacity);

    if (!buf)
    {
        HANDLE_ERRNO_ERROR(ERROR_NO_MEMORY, "Failed to calloc arena buf: %s");
    }

    return Result_Arena_ctor(
        (Arena) {
            buf,
            buf,
            buf + capacity,
        },
        EVERYTHING_FINE
    );

ERROR_CASE

    return Result_Arena_ctor((Arena){}, err);
}

INLINE void* arena_allocate(Arena* arena, size_t size)
{
    assert(arena);

    size += (sizeof(void*) - (size % sizeof(void*))) % sizeof(void*);
    if (arena->current + size >= arena->end)
    {
        return NULL;
    }

    void* allocated = arena->current;
    arena->current += size;

    return allocated;
}

INLINE void arena_flush(Arena* restrict arena)
{
    assert(arena);

    arena->current = arena->buffer;
}

INLINE void arena_dtor(Arena* restrict arena)
{
    if (arena)
    {
        free(arena->buffer);
        arena->buffer = NULL;
    }
}

#endif // CMLIB_ARENA_H_
