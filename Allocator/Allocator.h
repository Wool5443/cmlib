#ifndef CMLIB_ALLOCATOR_H_
#define CMLIB_ALLOCATOR_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../common.h"

#define MALLOC_ALLOCATOR (Allocator) { malloc, free }
#define CALLOC_ALLOCATOR (Allocator) { cmlib_calloc_proxy, free }

typedef void* (*allocate_function)(size_t);
typedef void  (*free_function)(void*);

typedef struct Allocator
{
    allocate_function allocate;
    free_function     free;
} Allocator;

INLINE void* cmlib_calloc_proxy(size_t size);
INLINE size_t align_size(size_t size);
INLINE void* align_ptr(void* ptr);

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

INLINE void* align_ptr(void* ptr)
{
    size_t align = sizeof(void*);
    auto p = (uintptr_t)ptr;
    p = (p + align - 1) & ~(align - 1);
    return (void*)p;
}

#endif // CMLIB_ALLOCATOR_H_
