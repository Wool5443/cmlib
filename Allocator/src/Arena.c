#include "Arena.h"

#include "Allocator.h"
#include "details/CountingMalloc.h"

struct Arena
{
    char* buffer;  /**< Start of owned storage. */
    char* current; /**< Next available byte. */
    char* end;     /**< One-past-end pointer. */
};

Arena* arena_ctor(size_t);
void* arena_allocate(Arena*, size_t, size_t);
void arena_deallocate(Arena*, void*);
void arena_flush(Arena*);
void arena_dtor(Arena*);

Arena* arena_ctor(size_t capacity)
{
    if (capacity == 0)
    {
        return NULL;
    }

    Arena* arena = (Arena*)cmlib_details_malloc(sizeof(Arena) + capacity);

    if (!arena)
    {
        return NULL;
    }

    char* buf = (char*)(arena + 1);

    *arena = (Arena) {
        .buffer = buf,
        .current = buf,
        .end = buf + capacity,
    };

    return arena;
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

void arena_deallocate(Arena*, void*) {}

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

    cmlib_details_free(arena);
}
