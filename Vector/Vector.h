#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>
#include <string.h>

#include "Logger.h"

#define DEFAULT_CAPACITY 8

typedef struct
{
    size_t capacity;
    size_t size;
} VHeader_;

#define GET_HEADER(ptr) (&((VHeader_*)(ptr))[-1])

INLINE void* VecCtor(size_t elemSize, size_t capacity)
{
    ERROR_CHECKING();
    assert(elemSize);

    capacity = capacity ? capacity : DEFAULT_CAPACITY;

    VHeader_* header = calloc(capacity * elemSize + sizeof(VHeader_), 1);
    if (!header)
    {
        err = ERROR_NO_MEMORY;
        LogError();
        return NULL;
    }

    header->capacity = capacity;

    return &header[1];
}

INLINE void VecDtor(void* vec)
{
    if (vec) free(GET_HEADER(vec));
}

INLINE size_t VecSize(void* vec)
{
    if (!vec) return 0;

    VHeader_* header = GET_HEADER(vec);
    return header->size;
}

INLINE void* VecRealloc(void* vec, size_t elemSize)
{
    if (!vec)
        return VecCtor(elemSize, DEFAULT_CAPACITY);

    VHeader_* header = GET_HEADER(vec);

    if (header->size < header->capacity)
        return vec;

    size_t newCap = header->capacity * 3 / 2;

    void* newVec = VecCtor(elemSize, newCap);
    if (!newVec) return vec;

    memcpy(newVec, vec, elemSize * header->size);

    VHeader_* newHeader = GET_HEADER(newVec);
    *newHeader = *header;
    free(header);

    return newVec;
}

#define VecAdd(vec, value)                                                      \
({                                                                              \
    ErrorCode vecAddError_ = ERROR_NO_MEMORY;                                   \
    void* temp = VecRealloc((vec), sizeof(*vec));                               \
    if (temp)                                                                   \
    {                                                                           \
        vecAddError_ = EVERYTHING_FINE;                                         \
        (vec) = temp;                                                           \
        VHeader_* header = GET_HEADER(vec);                                     \
        (vec)[header->size++] = value;                                          \
    }                                                                           \
    vecAddError_;                                                               \
})

#define VecExpand(vec, newCapacity)                                             \
({                                                                              \
    ErrorCode vecExpandError_ = ERROR_NO_MEMORY;                                \
    void* temp = VecCtor(sizeof(*(vec)), newCapacity);                          \
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

INLINE void VecPop(void* vec)
{
    if (!vec) return;

    VHeader_* header = GET_HEADER(vec);
    header->size--;
}

#endif
