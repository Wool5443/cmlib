/**
 * @file Allocator.h
 * @author Misha Solodilov (mihsolodilov2015@gmail.com)
 * @brief Header file for memory allocation functions and the Allocator
 * structure.
 *
 * This header defines the `Allocator` structure and associated functions for
 * memory allocation and deallocation. The `Allocator` structure encapsulates
 * the memory allocation and freeing functions, allowing for flexible management
 * of memory allocation strategies. Various predefined allocators are also
 * provided, such as `MALLOC_ALLOCATOR` and `CALLOC_ALLOCATOR`, which are based
 * on standard `malloc` and `calloc` functions, respectively.
 *
 * The allocator can be customized by defining different allocation and
 * deallocation strategies using function pointers.
 *
 * The `Allocator` structure also provides utility functions for memory
 * alignment, which ensure that allocated memory adheres to alignment
 * requirements for various platforms or performance optimization.
 *
 * @version 1.0
 * @date 12.06.2025
 *
 * @copyright Copyright (c) 2025
 */

#ifndef CMLIB_ALLOCATOR_H_
#define CMLIB_ALLOCATOR_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../common.h"

/**
 * @brief Predefined allocator using `malloc` and `free`.
 *
 * This allocator uses the standard `malloc` for memory allocation and `free`
 * for memory deallocation.
 */
#define MALLOC_ALLOCATOR                                                       \
    (Allocator)                                                                \
    {                                                                          \
        malloc, free                                                           \
    }

/**
 * @brief Predefined allocator using `cmlib_calloc_proxy` and `free`.
 *
 * This allocator uses a custom memory allocation function
 * (`cmlib_calloc_proxy`) and standard `free` for deallocation. The proxy is
 * designed to handle memory allocation with zero-initialization.
 */
#define CALLOC_ALLOCATOR                                                       \
    (Allocator)                                                                \
    {                                                                          \
        cmlib_calloc_proxy, free                                               \
    }

/**
 * @brief Predefined empty allocator.
 *
 * This allocator has both the `allocate` and `free` functions set to `NULL`,
 * indicating no allocation strategy.
 */
#define EMPTY_ALLOCATOR                                                        \
    (Allocator)                                                                \
    {                                                                          \
        NULL, NULL                                                             \
    }

/**
 * @brief Function pointer type for memory allocation.
 *
 * This function type represents the signature of a memory allocation function.
 */
typedef void* (*allocate_function)(size_t);

/**
 * @brief Function pointer type for memory deallocation.
 *
 * This function type represents the signature of a memory deallocation
 * function.
 */
typedef void (*free_function)(void*);

/**
 * @brief Structure to represent a memory allocator.
 *
 * The `Allocator` structure holds function pointers to memory allocation and
 * deallocation functions, allowing for flexible management of memory
 * operations. Different allocators can be defined by setting these function
 * pointers.
 */
typedef struct Allocator
{
    allocate_function allocate; /**< Pointer to the memory allocation function
                                 */
    free_function free; /**< Pointer to the memory deallocation function */
} Allocator;

/**
 * @brief Proxy function for memory allocation with zero-initialization.
 *
 * This function is used as a proxy for `calloc` and handles memory allocation
 * with zero initialization. It is used in the `CALLOC_ALLOCATOR` predefined
 * allocator.
 *
 * @param size The size of memory to allocate.
 * @return Pointer to the allocated and zero-initialized memory block.
 */
INLINE void* cmlib_calloc_proxy(size_t size);

/**
 * @brief Aligns a size to the platform's memory alignment.
 *
 * This function adjusts the given size to the nearest aligned size. Alignment
 * is typically needed for performance reasons or to meet hardware-specific
 * constraints.
 *
 * @param size The size to be aligned.
 * @return The aligned size.
 */
INLINE size_t align_size(size_t size);

/**
 * @brief Aligns a pointer to the platform's memory alignment.
 *
 * This function adjusts the given pointer to the nearest aligned memory
 * address. Alignment is needed for performance optimization or to meet
 * hardware-specific constraints.
 *
 * @param ptr The pointer to be aligned.
 * @return The aligned pointer.
 */
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
