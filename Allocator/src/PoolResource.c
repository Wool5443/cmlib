#include "PoolResource.h"

DECLARE_RESULT_SOURCE(PoolResource);

static void*
pool_resource_allocate(void* resource, size_t size, size_t alignment);
static void pool_resource_deallocate(void* resource, void* ptr);

Result_PoolResource pool_resource_ctor(size_t count)
{
    Pool* pool = pool_ctor(count);
    if (!pool)
    {
        return Result_PoolResource_ctor((PoolResource) {}, ERROR_NULLPTR);
    }

    return Result_PoolResource_ctor(pool_to_resource(pool), EVERYTHING_FINE);
}

PoolResource pool_to_resource(Pool* pool)
{
    if (!pool)
    {
        return (PoolResource) {};
    }

    return (PoolResource) {
        .base =
            (MemoryResource) {
                .allocate = pool_resource_allocate,
                .deallocate = pool_resource_deallocate,
            },
        .pool = pool,
    };
}

void pool_resource_dtor(PoolResource* resource)
{
    if (!resource)
    {
        return;
    }

    pool_dtor(resource->pool);
}

static void*
pool_resource_allocate(void* resource, size_t size, size_t alignment)
{
    assert(resource);
    PoolResource* pr = (PoolResource*)(resource);
    return pool_allocate(pr->pool, size, alignment);
}

static void pool_resource_deallocate(void* resource, void* ptr)
{
    assert(resource);
    PoolResource* pr = (PoolResource*)(resource);
    return pool_deallocate(pr->pool, ptr);
}
