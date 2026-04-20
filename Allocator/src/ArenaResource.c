#include "ArenaResource.h"

DECLARE_RESULT_SOURCE(ArenaResource);

Result_ArenaResource arena_resource_ctor(size_t);
ArenaResource arena_to_resource(Arena*);
void arena_resource_dtor(ArenaResource*);

static void* arena_resource_allocate(MemoryResource* arena_resource,
    size_t size,
    size_t alignment)
{
    ArenaResource* resource = (ArenaResource*)arena_resource;

    return arena_allocate(&resource->arena, size, alignment);
}

static void arena_resource_deallocate(MemoryResource* resource, void* ptr)
{
    ArenaResource* arena_resource = (ArenaResource*)resource;

    arena_deallocate(&arena_resource->arena, ptr);
}

Result_ArenaResource arena_resource_ctor(size_t capacity)
{
    Result_Arena arena = arena_ctor(capacity);
    if (arena.error_code)
    {
        return Result_ArenaResource_ctor((ArenaResource) {}, arena.error_code);
    }

    return Result_ArenaResource_ctor(
        arena_to_resource(&arena.value),
        EVERYTHING_FINE);
}

ArenaResource arena_to_resource(Arena* arena)
{
    ArenaResource resource = {
        .base =
            (MemoryResource) {
                .allocate = arena_resource_allocate,
                .deallocate = arena_resource_deallocate,
            },
    };

    if (arena)
    {
        resource.arena = *arena;
        *arena = (Arena) {};
    }

    return resource;
}

void arena_resource_dtor(ArenaResource* resource)
{
    if (!resource)
    {
        return;
    }

    arena_dtor(&resource->arena);
    *resource = (ArenaResource) {};
}
