#ifndef CMLIB_ARENA_RESOURCE_H_
#define CMLIB_ARENA_RESOURCE_H_

#include <stddef.h>

#include "Allocator.h"
#include "Arena.h"
#include "Result.h"

typedef struct ArenaResource
{
    MemoryResource base;
    Arena* arena;
} ArenaResource;

DECLARE_RESULT_HEADER(ArenaResource);

Result_ArenaResource arena_resource_ctor(size_t capacity);

ArenaResource arena_to_resource(Arena* arena);

void arena_resource_dtor(ArenaResource* resource);

#endif // CMLIB_ARENA_RESOURCE_H_
