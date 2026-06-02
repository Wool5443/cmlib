#include "FreeListResource.h"

DECLARE_RESULT_SOURCE(FreeListResource);

static void*
free_list_resource_allocate(void* resource, size_t size, size_t alignment);
static void free_list_resource_deallocate(void* resource, void* ptr);

Result_FreeListResource free_list_resource_ctor(size_t pool_size)
{
    FreeList* free_list = free_list_ctor(pool_size);
    if (!free_list)
    {
        return Result_FreeListResource_ctor((FreeListResource) {},
            ERROR_NULLPTR);
    }

    return Result_FreeListResource_ctor(free_list_to_resource(free_list),
        EVERYTHING_FINE);
}

FreeListResource free_list_to_resource(FreeList* free_list)
{
    if (!free_list)
    {
        return (FreeListResource) {};
    }

    return (FreeListResource) {
        .base =
            (MemoryResource) {
                .allocate = free_list_resource_allocate,
                .deallocate = free_list_resource_deallocate,
            },
        .free_list = free_list,
    };
}

void free_list_resource_dtor(FreeListResource* resource)
{
    if (!resource)
    {
        return;
    }

    free_list_dtor(resource->free_list);
}

static void*
free_list_resource_allocate(void* resource, size_t size, size_t alignment)
{
    assert(resource);
    FreeListResource* free_list_resource = (FreeListResource*)resource;
    return free_list_allocate(free_list_resource->free_list, size, alignment);
}

static void free_list_resource_deallocate(void* resource, void* ptr)
{
    assert(resource);
    FreeListResource* free_list_resource = (FreeListResource*)resource;
    free_list_deallocate(free_list_resource->free_list, ptr);
}
