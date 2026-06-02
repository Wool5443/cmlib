#ifndef CMLIB_ALLOCATOR_H_
#define CMLIB_ALLOCATOR_H_

#include <stddef.h>
#include <stdint.h>

#include "../common.h"

typedef struct MemoryResource MemoryResource;

typedef void* (*memory_resource_allocate_func)(void* mem_resource,
    size_t size,
    size_t alignment);

typedef void (*memory_resource_deallocate_func)(void* mem_resource, void* ptr);

struct MemoryResource
{
    memory_resource_allocate_func allocate;
    memory_resource_deallocate_func deallocate;
};

#define CMLIB_DETAILS_ALIGN(value, alignment)                                  \
    (((value) + (alignment) - 1) & ~((alignment) - 1))

INLINE size_t align_size(size_t size, size_t alignment)
{
    if (size == 0)
    {
        size = 1;
    }

    size = CMLIB_DETAILS_ALIGN(size, alignment);
    return size;
}

INLINE void* align_ptr(void* ptr, size_t alignment)
{
    auto p = (uintptr_t)ptr;
    p = CMLIB_DETAILS_ALIGN(p, alignment);
    return (void*)p;
}

#undef CMLIB_DETAILS_ALIGN

MemoryResource* get_malloc_resource(void);

MemoryResource* get_calloc_resource(void);

#endif // CMLIB_ALLOCATOR_H_
