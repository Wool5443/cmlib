#ifndef CMLIB_VECTOR_H_
#define CMLIB_VECTOR_H_

#include <stdlib.h>
#include <string.h>

#include "../common.h"
#include "Allocator.h"
#include "Error.h" // IWYU pragma: keep

static constexpr size_t CMLIB_VEC_DEFAULT_CAPACITY = 8;

typedef struct VHeader_
{
    MemoryResource* memory_resource;
    size_t size;
    size_t capacity;
} cmlib_details_VHeader_;

INLINE cmlib_details_VHeader_* cmlib_details_get_vec_header(void* vec);
void* cmlib_details_vec_realloc(void* vec, size_t elem_size);

void* cmlib_details_vec_ctor(void* memory_resource,
    size_t elem_size,
    size_t capacity);
#define vec_ctor(memory_resource, type)                                        \
    (cmlib_details_vec_ctor(memory_resource,                                   \
        sizeof(type),                                                          \
        CMLIB_VEC_DEFAULT_CAPACITY))

INLINE void vec_dtor(void* vec);

INLINE size_t vec_size(void* vec);

INLINE size_t vec_capacity(void* vec);

INLINE void vec_clear(void* vec);

#define vec_add(vec, value)                                                    \
    ({                                                                         \
        Error_code vec_add_error = ERROR_NO_MEMORY;                            \
        void* temp = cmlib_details_vec_realloc((vec), sizeof(*vec));           \
        if (temp)                                                              \
        {                                                                      \
            vec_add_error = EVERYTHING_FINE;                                   \
            (vec) = temp;                                                      \
            auto header = cmlib_details_get_vec_header(vec);                   \
            (vec)[header->size++] = (value);                                   \
        }                                                                      \
        vec_add_error;                                                         \
    })

#define vec_pop(vec)                                                           \
    ({                                                                         \
        typeof(*vec) ret = {};                                                 \
        if (vec)                                                               \
        {                                                                      \
            auto header = cmlib_details_get_vec_header(vec);                   \
            ret = (vec)[--header->size];                                       \
        }                                                                      \
        ret;                                                                   \
    })

#define vec_reserve(vec, new_capacity)                                         \
    ({                                                                         \
        size_t vec_reserve_new_capacity = (new_capacity);                      \
        Error_code vec_reserve_error = ERROR_NO_MEMORY;                        \
        MemoryResource* vec_reserve_resource =                                 \
            cmlib_details_get_vec_header(vec)->memory_resource;                \
        void* temp = cmlib_details_vec_ctor(vec_reserve_resource,              \
            sizeof(*vec),                                                      \
            vec_reserve_new_capacity);                                         \
        if (temp)                                                              \
        {                                                                      \
            vec_reserve_error = EVERYTHING_FINE;                               \
            size_t size = MIN(vec_size(vec), vec_reserve_new_capacity);        \
            if ((vec) && size)                                                 \
            {                                                                  \
                memcpy(temp, vec, sizeof(*(vec)) * size);                      \
            }                                                                  \
            vec_dtor(vec);                                                     \
            (vec) = temp;                                                      \
            cmlib_details_get_vec_header(vec)->size = size;                    \
        }                                                                      \
        vec_reserve_error;                                                     \
    })

#define VEC_ITER(vec, iter_name, ...)                                          \
    assert(vec);                                                               \
    SWITCH_EMPTY(for (size_t iter_name = 0, iter_name##_end = vec_size(vec);   \
                     iter_name < iter_name##_end;                              \
                     iter_name++),                                             \
        for (size_t iter_name = FIRST(__VA_ARGS__),                            \
            iter_name##_end = MIN((size_t)EXPAND_BUT_FIRST(__VA_ARGS__),       \
                vec_size(vec));                                                \
            iter_name < iter_name##_end;                                       \
            iter_name++),                                                      \
        __VA_ARGS__)

INLINE void vec_dtor(void* vec)
{
    if (vec)
    {
        cmlib_details_VHeader_* header = cmlib_details_get_vec_header(vec);
        header->memory_resource->deallocate(header->memory_resource, header);
    }
}

INLINE size_t vec_size(void* vec)
{
    if (!vec)
    {
        return 0;
    }

    cmlib_details_VHeader_* header = cmlib_details_get_vec_header(vec);
    return header->size;
}

INLINE size_t vec_capacity(void* vec)
{
    if (!vec)
    {
        return 0;
    }

    cmlib_details_VHeader_* header = cmlib_details_get_vec_header(vec);
    return header->capacity;
}

INLINE void vec_clear(void* vec)
{
    if (!vec)
    {
        return;
    }

    cmlib_details_VHeader_* header = cmlib_details_get_vec_header(vec);
    header->size = 0;
}

INLINE cmlib_details_VHeader_* cmlib_details_get_vec_header(void* vec)
{
    return &((cmlib_details_VHeader_*)vec)[-1];
}

#endif // CMLIB_VECTOR_H_
