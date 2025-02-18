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

static const Allocator Malloc_allocator = {
    malloc,
    free,
};

static const Allocator Calloc_allocator = {
    cmlib_calloc_proxy,
    free,
};

INLINE void* cmlib_calloc_proxy(size_t size)
{
    return calloc(1, size);
}

#endif // CMLIB_ALLOCATOR_H_
