#ifndef CMLIB_POOL_H_
#define CMLIB_POOL_H_

#include <stddef.h>

typedef struct Pool Pool;

Pool* pool_ctor(size_t count);

void pool_dtor(Pool* pool);

void* pool_allocate(Pool* pool, size_t size, size_t alignment);

#define pool_allocate_type(pool, type)                               \
    (pool_allocate(pool, sizeof(type), alignof(type)))

void pool_deallocate(Pool* pool, void* ptr);

#endif // CMLIB_POOL_H_
