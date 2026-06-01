/**
 * @file Allocator.h
 * @brief Polymorphic memory resource and alignment helpers.
 */

#ifndef CMLIB_ALLOCATOR_H_
#define CMLIB_ALLOCATOR_H_

#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../common.h"

typedef struct MemoryResource MemoryResource;

/**
 * @brief Allocates memory from a memory resource.
 * @param mem_resource Resource object.
 * @param size Requested byte count.
 * @param alignment Required pointer alignment.
 * @return Allocated pointer, or NULL on failure.
 */
typedef void* (*memory_resource_allocate_func)(void* mem_resource,
    size_t size,
    size_t alignment);

/**
 * @brief Deallocates memory through a memory resource.
 * @param mem_resource Resource object.
 * @param ptr Pointer previously returned by the resource, or NULL.
 */
typedef void (
    *memory_resource_deallocate_func)(void* mem_resource, void* ptr);

/**
 * @brief Base object embedded as the first field in every concrete resource.
 */
struct MemoryResource
{
    memory_resource_allocate_func allocate;
    memory_resource_deallocate_func deallocate;
};

#define CMLIB_ALIGN(cmlib_macroarg_value, cmlib_macroarg_alignment)            \
    (((cmlib_macroarg_value) + (cmlib_macroarg_alignment) - 1)                 \
        & ~((cmlib_macroarg_alignment) - 1))

/**
 * @brief Aligns a size to the requested alignment.
 * @param size Input byte count.
 * @param alignment Required alignment.
 * @return Aligned size, at least one alignment unit for zero input.
 */
INLINE size_t align_size(size_t size, size_t alignment)
{
    if (size == 0)
    {
        size = 1;
    }

    size = CMLIB_ALIGN(size, alignment);
    return size;
}

/**
 * @brief Aligns a pointer upward to the requested alignment.
 * @param ptr Pointer to align.
 * @param alignment Required alignment.
 * @return Aligned pointer.
 */
INLINE void* align_ptr(void* ptr, size_t alignment)
{
    auto p = (uintptr_t)ptr;
    p = CMLIB_ALIGN(p, alignment);
    return (void*)p;
}

#undef CMLIB_ALIGN

/**
 * @brief Returns a stable resource backed by `malloc` and `free`.
 * @return Pointer to process-lifetime malloc resource.
 */
MemoryResource* get_malloc_resource(void);

/**
 * @brief Returns a stable resource backed by `calloc` and `free`.
 * @return Pointer to process-lifetime calloc resource.
 */
MemoryResource* get_calloc_resource(void);

#endif // CMLIB_ALLOCATOR_H_
