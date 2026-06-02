/**
 * @file Pool.h
 * @brief cmlib pool allocator.
 */

#ifndef CMLIB_POOL_H_
#define CMLIB_POOL_H_

#include <stddef.h>

/**
 * @class Pool
 * @brief Fixed-count allocator for equal-sized blocks.
 */
typedef struct Pool Pool;

/**
 * @brief Constructs a pool with specified element count per subpool.
 *
 * @param count
 * @return pool or NULL on failure.
 */
Pool* pool_ctor(size_t count);

/**
 * @brief Frees the pool's memory.
 *
 * @param pool
 */
void pool_dtor(Pool* pool);

/**
 * @brief Allocates memory in the pool.
 *
 * @param pool
 * @param size
 * @param alignment
 *
 * @return pointer to allocated memory or NULL on failure.
 */
void* pool_allocate(Pool* pool, size_t size, size_t alignment);

/**
 * @brief Allocates memory for specific type in the pool.
 *
 * @param pool
 * @param type
 *
 * @return pointer to allocated memory or NULL on failure.
 */
#define pool_allocate_type(pool, type)                               \
    (pool_allocate(pool, sizeof(type), alignof(type)))

/**
 * @brief Deallocates memory in the pool.
 *
 * @param pool
 * @param ptr
 */
void pool_deallocate(Pool* pool, void* ptr);

#endif // CMLIB_POOL_H_
