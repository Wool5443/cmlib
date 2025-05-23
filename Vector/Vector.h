#ifndef CMLIB_VECTOR_H_
#define CMLIB_VECTOR_H_

#include <stdlib.h>
#include <string.h>

#include "../Logger/Logger.h"
#include "../Allocator/Allocator.h"

#define DEFAULT_CAPACITY 8

// TODO: vec_iter

/**
 * @struct VHeader_
 *
 * @brief A header for working with vectors
 */
typedef struct VHeader_
{
    Allocator allocator; ///< allocator
    size_t size; ///< vector size
    size_t capacity; ///< vector capacity
} VHeader_;

extern Allocator Current_vector_allocator;

/**
 * @brief Get pointer to vectors header
 */
#define GET_VEC_HEADER(vec) (&((VHeader_*)(vec))[-1])
#define GET_VEC_ALLOCATOR(vec) ((vec) ? GET_VEC_HEADER(vec)->allocator : Current_vector_allocator)

#define VEC_FREE(vec) GET_VEC_ALLOCATOR(vec).free(GET_VEC_HEADER(vec))

#define VEC_ITER(vec, iter_name, ...)                                       \
SWITCH_EMPTY(                                                               \
    for (size_t iter_name = 0, iter_name ## _end = vec_size(vec);           \
        iter_name < iter_name ## _end;                                      \
        iter_name++                                                         \
    ),                                                                      \
    for (size_t iter_name = FIRST(__VA_ARGS__),                             \
        iter_name ## _end =                                                 \
            MIN((size_t)EXPAND_BUT_FIRST(__VA_ARGS__), vec_size(vec));      \
        iter_name < iter_name ## _end;                                      \
        iter_name++                                                         \
    ),                                                                      \
    __VA_ARGS__                                                             \
)

/**
 * @brief Set vec allocator
 *
 * @param [in] allocator
 */
INLINE void vec_set_allocator(Allocator allocator);

/**
 * @brief Reset default vec allocator(malloc)
 */
INLINE void vec_reset_allocator();

/**
 * @brief Destroys a vector
 *
 * @param [in] vec
 */
INLINE void vec_dtor(void* vec);

/**
 * @brief Get vector size
 *
 * @param [in] vec
 *
 * @return size_t size
 */
INLINE size_t vec_size(void* vec);

/**
 * @brief Get vector capacity
 *
 * @param [in] vec
 *
 * @return size_t size
 */
INLINE size_t vec_capacity(void* vec);

/**
 * @brief Clears the vector
 *
 * @param [in] vec
 */
INLINE void vec_clear(void* vec);

/**
 * @brief Creates a Vector
 *
 * @param [in] elem_size
 * @param [in] capacity
 *
 * @return void* vector
 */
void* vec_ctor_(Allocator allocator, size_t elem_size, size_t capacity);

/**
 * @brief Reallocate a vector if it is full
 *
 * Safe to realloc NULL
 *
 * @param [in] vec
 * @param [in] elem_size
 *
 * @return void* vec or NULL on error
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
    if (vec) VEC_FREE(vec);
}

INLINE size_t vec_size(void* vec)
{
    if (!vec) return 0;

    VHeader_* header = GET_VEC_HEADER(vec);
    return header->size;
}

INLINE size_t vec_capacity(void* vec)
{
    if (!vec) return 0;

    VHeader_* header = GET_VEC_HEADER(vec);
    return header->capacity;
}

INLINE void vec_clear(void* vec)
{
    if (!vec) return;

    VHeader_* header = GET_VEC_HEADER(vec);
    header->size = 0;
}

/**
 * @brief Add value at the end of vec
 *
 * Safe to vec_add(NULL, ...)
 *
 * @param [out] vec
 * @param [in] value
 *
 * @return Error_code
 *
 * @see Error_code
 */
#define vec_add(vec, value)                                                     \
({                                                                              \
    Error_code vec_add_error = ERROR_NO_MEMORY;                                 \
    void* temp = vec_realloc_((vec), sizeof(*vec));                             \
    if (temp)                                                                   \
    {                                                                           \
        vec_add_error = EVERYTHING_FINE;                                        \
        (vec) = temp;                                                           \
        VHeader_* header = GET_VEC_HEADER(vec);                                 \
        (vec)[header->size++] = value;                                          \
    }                                                                           \
    vec_add_error;                                                              \
})

/**
 * @brief Pop a value from the end of the vec
 *
 * Safe to vec_pop(NULL, ...).
 * If empty return (typeof(*vec)){}
 *
 * @param [in] vec
 *
 * @return value
 */
#define vec_pop(vec)                                                            \
({                                                                              \
    __typeof__(*vec) ret = {};                                                  \
    if (vec)                                                                    \
    {                                                                           \
        VHeader_* header = GET_VEC_HEADER(vec);                                 \
        ret = (vec)[--header->size];                                            \
    }                                                                           \
    ret;                                                                        \
})

/**
 * @brief Expand vector capacity
 *
 * Safe to vec_reserve(NULL, ...).
 *
 * @param [out] vec
 *
 * @return Error_code
 *
 * @see Error_code
 */
#define vec_reserve(vec, newCapacity)                                           \
({                                                                              \
    Error_code vec_reserve_error = ERROR_NO_MEMORY;                             \
    void* temp = vec_ctor_(GET_VEC_ALLOCATOR(vec), sizeof(*(vec)), newCapacity);\
    if (temp)                                                                   \
    {                                                                           \
        vec_reserve_error = EVERYTHING_FINE;                                    \
        size_t size = MIN(vec_size(vec), (size_t)newCapacity);                  \
        if ((vec) && size)                                                      \
            memcpy(temp, vec, sizeof(*(vec)) * size);                           \
        vec_dtor(vec);                                                          \
        (vec) = temp;                                                           \
        GET_VEC_HEADER(vec)->size = size;                                       \
    }                                                                           \
    vec_reserve_error;                                                          \
})

#endif // CMLIB_VECTOR_H_
