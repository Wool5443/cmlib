/**
 * @file PoolResource.h
 * @brief cmlib pool memory resource.
 */

#ifndef CMLIB_POOL_RESOURCE_H_
#define CMLIB_POOL_RESOURCE_H_

#include "Allocator.h"
#include "Pool.h"
#include "Result.h"

/**
 * @class PoolResource
 * @brief Memory resource managing a pool.
 */
typedef struct PoolResource
{
    MemoryResource base;
    Pool* pool;
} PoolResource;

DECLARE_RESULT_HEADER(PoolResource);

/**
 * @brief Constructs a pool resource with specified element count per subpool.
 *
 * @param count
 * @return result object with resource and error_code.
 */
Result_PoolResource pool_resource_ctor(size_t count);

/**
 * @brief Converts existing pool into resource.
 *
 * @param pool
 * @return resource
 */
PoolResource pool_to_resource(Pool* pool);

/**
 * @brief Destroys pool resource.
 * Do not destroy the same pool twice if you used pool_to_resource.
 *
 * @param resource
 */
void pool_resource_dtor(PoolResource* resource);

#endif // CMLIB_POOL_RESOURCE_H_
