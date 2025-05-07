#ifndef CMLIB_ALLOCATOR_H_
#define CMLIB_ALLOCATOR_H_

#include <stddef.h>
#include <stdlib.h>

#include "../common.h"

typedef void* (*allocate_function)(size_t);
typedef void  (*free_function)(void*);

typedef struct Allocator
{
    allocate_function allocate;
    free_function     free;
} Allocator;

INLINE void* cmlib_calloc_proxy(size_t size);
INLINE size_t align_size(size_t size);

UNUSED static Allocator Malloc_allocator = {
    malloc,
    free,
};

UNUSED static Allocator Calloc_allocator = {
    cmlib_calloc_proxy,
    free,
};

INLINE void* cmlib_calloc_proxy(size_t size)
{
    return calloc(1, size);
}

INLINE size_t align_size(size_t size)
{
    if (size == 0)
    {
        size = 1;
    }

    size_t align = sizeof(void*);
    size = (size + align - 1) & ~(align - 1);
    return size;
}

#endif // CMLIB_ALLOCATOR_H_
