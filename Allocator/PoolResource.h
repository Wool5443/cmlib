#ifndef CMLIB_POOL_RESOURCE_H_
#define CMLIB_POOL_RESOURCE_H_

#include "Allocator.h"
#include "Pool.h"
#include "Result.h"

typedef struct PoolResource
{
    MemoryResource base;
    Pool* pool;
} PoolResource;

DECLARE_RESULT_HEADER(PoolResource);

Result_PoolResource pool_resource_ctor(size_t count);

PoolResource pool_to_resource(Pool* pool);

void pool_resource_dtor(PoolResource* resource);

#endif // CMLIB_POOL_RESOURCE_H_
