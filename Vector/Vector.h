#ifndef CMLIB_VECTOR_H_
#define CMLIB_VECTOR_H_

#include <stdlib.h>
#include <string.h>

#include "Logger.h"

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
#define GET_HEADER(vec) (&((VHeader_*)(vec))[-1])

/**
 * @brief Creates a Vector
 *
 * @param [in] elemSize
 * @param [in] capacity
 *
 * @return void* vector
 */
INLINE void* vecCtor_(size_t elemSize, size_t capacity)
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

/**
 * @brief Destroys a vector
 *
 * @param [in] vec
 */
INLINE void VecDtor(void* vec)
{
    if (vec) free(GET_HEADER(vec));
}

/**
 * @brief Get vector size
 *
 * @param [in] vec
 *
 * @return size_t size
 */
INLINE size_t VecSize(void* vec)
{
    if (!vec) return 0;

    VHeader_* header = GET_HEADER(vec);
    return header->size;
}

/**
 * @brief Get vector capacity
 *
 * @param [in] vec
 *
 * @return size_t size
 */
INLINE size_t VecCapacity(void* vec)
{
    if (!vec) return 0;

    VHeader_* header = GET_HEADER(vec);
    return header->capacity;
}

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
INLINE void* vecRealloc_(void* vec, size_t elemSize)
{
    if (!vec)
        return vecCtor_(elemSize, DEFAULT_CAPACITY);

    VHeader_* header = GET_HEADER(vec);

    if (header->size < header->capacity)
        return vec;

    size_t newCap = header->capacity * 3 / 2;

    void* newVec = vecCtor_(elemSize, newCap);
    if (!newVec) return NULL;

    memcpy(newVec, vec, elemSize * header->size);

    VHeader_* newHeader = GET_HEADER(newVec);
    *newHeader = *header;
    free(header);

    return newVec;
}

/**
 * @brief Add value at the end of vec
 *
 * Safe to VecAdd(NULL, ...)
 *
 * @param [out] vec
 * @param [in] value
 *
 * @return ErrorCode
 *
 * @see ErrorCode
 */
#define VecAdd(vec, value)                                                      \
({                                                                              \
    ErrorCode vecAddError_ = ERROR_NO_MEMORY;                                   \
    void* temp = vecRealloc_((vec), sizeof(*vec));                               \
    if (temp)                                                                   \
    {                                                                           \
        vecAddError_ = EVERYTHING_FINE;                                         \
        (vec) = temp;                                                           \
        VHeader_* header = GET_HEADER(vec);                                     \
        (vec)[header->size++] = value;                                          \
    }                                                                           \
    vecAddError_;                                                               \
})

/**
 * @brief Pop a value from the end of the vec
 *
 * Safe to VecPop(NULL, ...).
 * If empty return (typeof(*vec)){}
 *
 * @param [in] vec
 *
 * @return value
 */
#define VecPop(vec)                                                             \
({                                                                              \
    __typeof__(*vec) ret = {};                                                  \
    if (vec)                                                                    \
    {                                                                           \
        VHeader_* header = GET_HEADER(vec);                                     \
        ret = (vec)[--header->size];                                            \
    }                                                                           \
    ret;                                                                        \
})

/**
 * @brief Expand vector capacity
 *
 * Safe to VecExpand(NULL, ...).
 *
 * @param [out] vec
 *
 * @return ErrorCode
 *
 * @see ErrorCode
 */
#define VecExpand(vec, newCapacity)                                             \
({                                                                              \
    ErrorCode vecExpandError_ = ERROR_NO_MEMORY;                                \
    void* temp = vecCtor_(sizeof(*(vec)), newCapacity);                          \
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

#endif // CMLIB_VECTOR_H_
