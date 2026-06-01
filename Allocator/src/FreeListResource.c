#include "FreeListResource.h"

DECLARE_RESULT_SOURCE(FreeListResource);

Result_FreeListResource free_list_resource_ctor(size_t);
FreeListResource free_list_to_resource(FreeList*);
void free_list_resource_dtor(FreeListResource*);

static void*
free_list_resource_allocate(void* resource, size_t size, size_t alignment)
{
    if (!resource)
    {
        return NULL;
    }

    FreeListResource* free_list_resource = (FreeListResource*)resource;
    return free_list_allocate(&free_list_resource->free_list, size, alignment);
}

static void free_list_resource_deallocate(void* resource, void* ptr)
{
    if (!resource)
    {
        return;
    }

    FreeListResource* free_list_resource = (FreeListResource*)resource;
    free_list_deallocate(&free_list_resource->free_list, ptr);
}

Result_FreeListResource free_list_resource_ctor(size_t pool_size)
{
    Result_FreeList free_list = free_list_ctor(pool_size);
    if (free_list.error_code)
    {
        return Result_FreeListResource_ctor((FreeListResource) {},
            free_list.error_code);
    }

    return Result_FreeListResource_ctor(free_list_to_resource(&free_list.value),
        EVERYTHING_FINE);
}

FreeListResource free_list_to_resource(FreeList* free_list)
{
    FreeListResource resource = {
        .base = (MemoryResource) {
            .allocate = free_list_resource_allocate,
            .deallocate = free_list_resource_deallocate,
        },
    };

    if (free_list)
    {
        resource.free_list = *free_list;
        *free_list = (FreeList) {};
    }

    return resource;
}

void free_list_resource_dtor(FreeListResource* resource)
{
    if (!resource)
    {
        return;
    }

    free_list_dtor(&resource->free_list);
    *resource = (FreeListResource) {};
}
