#include "Allocator.h"

#include "details/CountingMalloc.h"

static void* malloc_resource_allocate(MemoryResource* resource,
    size_t size,
    size_t alignment)
{
    (void)resource;
    (void)alignment;
    return cmlib_details_malloc(size);
}

static void* calloc_resource_allocate(MemoryResource* resource,
    size_t size,
    size_t alignment)
{
    (void)resource;
    (void)alignment;
    return cmlib_details_calloc(1, size);
}

static void malloc_resource_deallocate(MemoryResource* resource, void* ptr)
{
    (void)resource;
    free(ptr);
}

static MemoryResource malloc_resource = {
    .allocate = malloc_resource_allocate,
    .deallocate = malloc_resource_deallocate,
};

static MemoryResource calloc_resource = {
    .allocate = calloc_resource_allocate,
    .deallocate = malloc_resource_deallocate,
};

size_t align_size(size_t, size_t);
void* align_ptr(void*, size_t);

MemoryResource* get_malloc_resource(void)
{
    return &malloc_resource;
}

MemoryResource* get_calloc_resource(void)
{
    return &calloc_resource;
}
