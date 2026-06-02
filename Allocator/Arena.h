#ifndef CMLIB_ARENA_H_
#define CMLIB_ARENA_H_

#include <stddef.h>

typedef struct Arena Arena;

Arena* arena_ctor(size_t capacity);

void* arena_allocate(Arena* arena, size_t size, size_t alignment);

#define arena_allocate_type(arena, type)                                       \
    (arena_allocate(arena, sizeof(type), alignof(type)))

void arena_deallocate(Arena* arena, void* ptr);

void arena_flush(Arena* arena);

void arena_dtor(Arena* arena);

#endif // CMLIB_ARENA_H_
