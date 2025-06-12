/**
 * @file Vector.h
 * @author Misha Solodilov (mihsolodilov2015@gmail.com)
 * @brief Header file for a vector implementation.
 *
 * This header defines the `VHeader_` structure, along with functions and macros
 * for dynamically managing vectors. A vector is a dynamic array that
 * automatically grows in size when more elements are added. The vector
 * functions handle dynamic memory allocation, resizing, and deallocation
 * efficiently. The `Allocator` structure is used to manage memory for vectors,
 * and custom allocators can be set using `vec_set_allocator`.
 *
 * @version 1.0
 * @date 12.06.2025
 *
 * @copyright Copyright (c) 2025
 */

#ifndef CMLIB_VECTOR_H_
#define CMLIB_VECTOR_H_

#include <stdlib.h>
#include <string.h>

#include "../Allocator/Allocator.h"
#include "../Logger/Logger.h" // IWYU pragma: keep

constexpr size_t DEFAULT_CAPACITY = 8; /**< Default capacity of the vector */

/**
 * @struct VHeader_
 * @brief A header structure for vector management.
 *
 * This structure stores the metadata for a vector, including its allocator,
 * current size, and capacity. It is used internally to manage the vector's
 * dynamic memory allocation and resizing.
 */
typedef struct VHeader_
{
    Allocator allocator; ///< The allocator used for vector memory management.
    size_t size;     ///< The current size of the vector (number of elements).
    size_t capacity; ///< The total capacity of the vector (number of elements
                     ///< it can store).
} VHeader_;

extern Allocator Current_vector_allocator; /**< The global allocator used for
                                              vectors */

/**
 * @brief Retrieves the header of the vector.
 *
 * This macro calculates the address of the vector header by using pointer
 * arithmetic. The header is located just before the actual vector data.
 *
 * @param vec The pointer to the vector.
 * @return A pointer to the vector's header (`VHeader_*`).
 */
#define GET_VEC_HEADER(vec) (&((VHeader_*)(vec))[-1])

/**
 * @brief Retrieves the allocator used for the vector.
 *
 * This macro retrieves the allocator used to allocate memory for the vector. If
 * the vector is `NULL`, it uses the default allocator
 * `Current_vector_allocator`.
 *
 * @param vec The pointer to the vector.
 * @return The allocator used by the vector.
 */
#define GET_VEC_ALLOCATOR(vec)                                                 \
    ((vec) ? GET_VEC_HEADER(vec)->allocator : Current_vector_allocator)

/**
 * @brief Frees the memory allocated for the vector.
 *
 * This macro frees the memory allocated for the vector, including its header
 * and data.
 *
 * @param vec The pointer to the vector to free.
 */
#define VEC_FREE(vec) GET_VEC_ALLOCATOR(vec).free(GET_VEC_HEADER(vec))

/**
 * @brief Iterator for the vector.
 *
 * This macro provides a way to iterate over a vector. It can be customized with
 * a start index and an end index.
 *
 * @param vec The vector to iterate over.
 * @param iter_name The name of the iterator variable.
 * @param ... Optional arguments to customize the iteration range.
 */
#define VEC_ITER(vec, iter_name, ...)                                          \
    SWITCH_EMPTY(                                                              \
        for (size_t iter_name = 0, iter_name##_end = vec_size(vec);            \
             iter_name < iter_name##_end;                                      \
             iter_name++),                                                     \
        for (size_t iter_name = FIRST(__VA_ARGS__),                            \
             iter_name##_end = MIN((size_t)EXPAND_BUT_FIRST(__VA_ARGS__),      \
                                   vec_size(vec));                             \
             iter_name < iter_name##_end;                                      \
             iter_name++),                                                     \
        __VA_ARGS__)

/**
 * @brief Sets the allocator for the vector.
 *
 * This function allows the user to set a custom allocator for the vector.
 *
 * @param allocator The custom allocator to use for vectors.
 */
INLINE void vec_set_allocator(Allocator allocator);

/**
 * @brief Resets the vector allocator to the default allocator (`malloc`).
 *
 * This function resets the allocator to the default allocator (`malloc`), which
 * is used for vector memory allocations.
 */
INLINE void vec_reset_allocator();

/**
 * @brief Destroys a vector.
 *
 * This function deallocates the memory used by the vector, including the header
 * and data.
 *
 * @param vec The pointer to the vector to destroy.
 */
INLINE void vec_dtor(void* vec);

/**
 * @brief Retrieves the size of the vector.
 *
 * This function returns the current size (number of elements) of the vector.
 *
 * @param vec The pointer to the vector.
 * @return The size of the vector.
 */
INLINE size_t vec_size(void* vec);

/**
 * @brief Retrieves the capacity of the vector.
 *
 * This function returns the current capacity (total number of elements the
 * vector can hold).
 *
 * @param vec The pointer to the vector.
 * @return The capacity of the vector.
 */
INLINE size_t vec_capacity(void* vec);

/**
 * @brief Clears the vector.
 *
 * This function resets the vector size to zero, effectively clearing all
 * elements in the vector.
 *
 * @param vec The pointer to the vector to clear.
 */
INLINE void vec_clear(void* vec);

/**
 * @brief Creates a new vector.
 *
 * This function allocates memory for a new vector with the specified element
 * size and initial capacity.
 *
 * @param allocator The allocator to use for memory management.
 * @param elem_size The size of each element in the vector.
 * @param capacity The initial capacity of the vector.
 * @return A pointer to the created vector.
 */
void* vec_ctor_(Allocator allocator, size_t elem_size, size_t capacity);

/**
 * @brief Reallocates the vector if it is full.
 *
 * This function reallocates the vector to a larger size if it is full, allowing
 * more elements to be added. If the vector is `NULL`, it is safely reallocated.
 *
 * @param vec The pointer to the vector.
 * @param elem_size The size of each element in the vector.
 * @return A pointer to the reallocated vector, or `NULL` if allocation fails.
 */
void* vec_realloc_(void* vec, size_t elem_size);

INLINE void vec_set_allocator(Allocator allocator)
{
    Current_vector_allocator = allocator;
}

INLINE void vec_reset_allocator()
{
    Current_vector_allocator = MALLOC_ALLOCATOR;
}

INLINE void vec_dtor(void* vec)
{
    if (vec)
        VEC_FREE(vec);
}

INLINE size_t vec_size(void* vec)
{
    if (!vec)
        return 0;

    VHeader_* header = GET_VEC_HEADER(vec);
    return header->size;
}

INLINE size_t vec_capacity(void* vec)
{
    if (!vec)
        return 0;

    VHeader_* header = GET_VEC_HEADER(vec);
    return header->capacity;
}

INLINE void vec_clear(void* vec)
{
    if (!vec)
        return;

    VHeader_* header = GET_VEC_HEADER(vec);
    header->size = 0;
}

/**
 * @brief Adds a value at the end of the vector.
 *
 * This macro appends a value to the vector. If the vector is full, it is
 * reallocated to accommodate the new element. If the vector is `NULL`, it is
 * reallocated safely.
 *
 * @param vec The pointer to the vector.
 * @param value The value to append to the vector.
 * @return An error code indicating success or failure.
 *
 * @see Error_code
 */
#define vec_add(vec, value)                                                    \
    ({                                                                         \
        Error_code vec_add_error = ERROR_NO_MEMORY;                            \
        void* temp = vec_realloc_((vec), sizeof(*vec));                        \
        if (temp)                                                              \
        {                                                                      \
            vec_add_error = EVERYTHING_FINE;                                   \
            (vec) = temp;                                                      \
            VHeader_* header = GET_VEC_HEADER(vec);                            \
            (vec)[header->size++] = value;                                     \
        }                                                                      \
        vec_add_error;                                                         \
    })

/**
 * @brief Pops a value from the end of the vector.
 *
 * This macro removes and returns the last element of the vector. If the vector
 * is empty, it returns an empty value.
 *
 * @param vec The pointer to the vector.
 * @return The popped value.
 */
#define vec_pop(vec)                                                           \
    ({                                                                         \
        __typeof__(*vec) ret = {};                                             \
        if (vec)                                                               \
        {                                                                      \
            VHeader_* header = GET_VEC_HEADER(vec);                            \
            ret = (vec)[--header->size];                                       \
        }                                                                      \
        ret;                                                                   \
    })

/**
 * @brief Expands the vector's capacity.
 *
 * This macro reallocates the vector to the specified new capacity, retaining
 * its current elements. If the vector is `NULL`, it is safely reallocated.
 *
 * @param vec The pointer to the vector.
 * @param newCapacity The new capacity for the vector.
 * @return An error code indicating success or failure.
 *
 * @see Error_code
 */
#define vec_reserve(vec, newCapacity)                                          \
    ({                                                                         \
        Error_code vec_reserve_error = ERROR_NO_MEMORY;                        \
        void* temp =                                                           \
            vec_ctor_(GET_VEC_ALLOCATOR(vec), sizeof(*(vec)), newCapacity);    \
        if (temp)                                                              \
        {                                                                      \
            vec_reserve_error = EVERYTHING_FINE;                               \
            size_t size = MIN(vec_size(vec), (size_t)newCapacity);             \
            if ((vec) && size)                                                 \
                memcpy(temp, vec, sizeof(*(vec)) * size);                      \
            vec_dtor(vec);                                                     \
            (vec) = temp;                                                      \
            GET_VEC_HEADER(vec)->size = size;                                  \
        }                                                                      \
        vec_reserve_error;                                                     \
    })

#endif // CMLIB_VECTOR_H_
