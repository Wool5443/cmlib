#ifndef CMLIB_VECTOR_H_
#define CMLIB_VECTOR_H_

#include <stdlib.h>
#include <string.h>

#include "../Logger/Logger.h"

#define DEFAULT_CAPACITY 8

/**
 * @struct VHeader_
 *
 * @brief A header for working with vectors
 */
typedef struct VHeader_
{
    size_t size; ///<vector size
    size_t capacity; ///< vector capacity
} VHeader_;

/**
 * @brief Get pointer to vectors header
 */
#define GET_VEC_HEADER(vec) (&((VHeader_*)(vec))[-1])

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
 * @param [in] elemSize
 * @param [in] capacity
 *
 * @return void* vector
 */
INLINE void* vec_ctor_(size_t elemSize, size_t capacity);

/**
 * @brief Reallocate a vector if it is full
 *
 * Safe to realloc NULL
 *
 * @param [in] vec
 * @param [in] elemSize
 *
 * @return void* vec or NULL on error
 */
INLINE void* vec_realloc_(void* vec, size_t elemSize);

INLINE void vec_dtor(void* vec)
{
    if (vec) free(GET_VEC_HEADER(vec));
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
 * @return ErrorCode
 *
 * @see ErrorCode
 */
#define vec_add(vec, value)                                                     \
({                                                                              \
    ErrorCode vecAddError_ = ERROR_NO_MEMORY;                                   \
    void* temp = vecRealloc_((vec), sizeof(*vec));                              \
    if (temp)                                                                   \
    {                                                                           \
        vecAddError_ = EVERYTHING_FINE;                                         \
        (vec) = temp;                                                           \
        VHeader_* header = GET_VEC_HEADER(vec);                                 \
        (vec)[header->size++] = value;                                          \
    }                                                                           \
    vecAddError_;                                                               \
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
 * @return ErrorCode
 *
 * @see ErrorCode
 */
#define vec_reserve(vec, newCapacity)                                           \
({                                                                              \
    ErrorCode vecExpandError_ = ERROR_NO_MEMORY;                                \
    void* temp = vecCtor_(sizeof(*(vec)), newCapacity);                         \
    if (temp)                                                                   \
    {                                                                           \
        vecExpandError_ = EVERYTHING_FINE;                                      \
        size_t vecSize = VecSize(vec);                                          \
        if ((vec) && vecSize)                                                   \
            memcpy(temp, vec, sizeof(*(vec)) * vecSize);                        \
        VecDtor(vec);                                                           \
        (vec) = temp;                                                           \
    }                                                                           \
    vecExpandError_;                                                            \
})

INLINE void* vec_ctor_(size_t elemSize, size_t capacity)
{
    ERROR_CHECKING();
    assert(elemSize);

    capacity = capacity ? capacity : DEFAULT_CAPACITY;

    VHeader_* header = calloc(capacity * elemSize + sizeof(VHeader_), 1);
    if (!header)
    {
        HANDLE_ERRNO_ERROR(ERROR_NO_MEMORY, "Error allocating vector: %s");
    }

    header->capacity = capacity;

    return &header[1];

 ERROR_CASE
    return NULL;
}

INLINE void* vec_realloc_(void* vec, size_t elemSize)
{
    if (!vec)
        return vec_ctor_(elemSize, DEFAULT_CAPACITY);

    VHeader_ header = *GET_VEC_HEADER(vec);

    if (header.size < header.capacity)
        return vec;

    size_t newCap = header.capacity * 2;

    void* newVec = vec_ctor_(elemSize, newCap);
    if (!newVec) return NULL;

    memcpy(newVec, vec, elemSize * header.size);

    VHeader_* newHeader = GET_VEC_HEADER(newVec);
    *newHeader = (VHeader_){ header.size, newCap };
    free(GET_VEC_HEADER(vec));

    return newVec;
}

#endif // CMLIB_VECTOR_H_
