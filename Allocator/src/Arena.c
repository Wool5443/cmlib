#include "Arena.h"

#include "Allocator.h"
#include "details/CountingMalloc.h"

DECLARE_RESULT_SOURCE(Arena);

Result_Arena arena_ctor(size_t);
void* arena_allocate(Arena*, size_t, size_t);
void arena_deallocate(Arena*, void*);
void arena_flush(Arena*);
void arena_dtor(Arena*);

Result_Arena arena_ctor(size_t capacity)
{
    if (capacity == 0)
    {
        return Result_Arena_ctor((Arena) {}, ERROR_BAD_VALUE);
    }

    char* buf = (char*)cmlib_details_malloc(capacity);

    if (!buf)
    {
        return Result_Arena_ctor((Arena) {}, ERROR_NO_MEMORY);
    }

    return Result_Arena_ctor(
        (Arena) {
            buf,
            buf,
            buf + capacity,
        },
        EVERYTHING_FINE);
}

void* arena_allocate(Arena* arena, size_t size, size_t alignment)
{
    if (!arena || size == 0 || alignment == 0)
    {
        return NULL;
    }

    char* allocated_ptr = align_ptr(arena->current, alignment);

    if (allocated_ptr + size > arena->end)
    {
        return NULL;
    }

    arena->current = allocated_ptr + size;

    return (void*)allocated_ptr;
}

void arena_deallocate(Arena* arena, void* ptr)
{
    (void)arena;
    (void)ptr;
}

void arena_flush(Arena* arena)
{
    if (!arena)
    {
        return;
    }

    arena->current = arena->buffer;
}

void arena_dtor(Arena* arena)
{
    if (!arena)
    {
        return;
    }

    free(arena->buffer);
    *arena = (Arena) {};
}
