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
        Error_code cmlib_vec_add_error__ = ERROR_NO_MEMORY;                    \
        void* cmlib_vec_add_temp__ =                                           \
            cmlib_details_vec_realloc((vec), sizeof(*vec));                    \
        if (cmlib_vec_add_temp__)                                              \
        {                                                                      \
            cmlib_vec_add_error__ = EVERYTHING_FINE;                           \
            (vec) = cmlib_vec_add_temp__;                                      \
            auto cmlib_vec_add_header__ = cmlib_details_get_vec_header(vec);   \
            (vec)[cmlib_vec_add_header__->size++] = (value);                   \
        }                                                                      \
        cmlib_vec_add_error__;                                                 \
    })

#define vec_pop(vec)                                                           \
    ({                                                                         \
        typeof(*vec) cmlib_vec_pop_ret__ = {};                                 \
        if (vec)                                                               \
        {                                                                      \
            auto cmlib_vec_pop_header__ = cmlib_details_get_vec_header(vec);   \
            cmlib_vec_pop_ret__ = (vec)[--cmlib_vec_pop_header__->size];       \
        }                                                                      \
        cmlib_vec_pop_ret__;                                                   \
    })

#define vec_reserve(vec, new_capacity)                                         \
    ({                                                                         \
        size_t cmlib_vec_reserve_new_capacity__ = (new_capacity);              \
        Error_code cmlib_vec_reserve_error__ = ERROR_NO_MEMORY;                \
        MemoryResource* cmlib_vec_reserve_resource__ =                         \
            cmlib_details_get_vec_header(vec)->memory_resource;                \
        void* cmlib_vec_reserve_temp__ = cmlib_details_vec_ctor(               \
            cmlib_vec_reserve_resource__,                                      \
            sizeof(*vec),                                                      \
            cmlib_vec_reserve_new_capacity__);                                 \
        if (cmlib_vec_reserve_temp__)                                          \
        {                                                                      \
            cmlib_vec_reserve_error__ = EVERYTHING_FINE;                       \
            size_t cmlib_vec_reserve_size__ =                                  \
                MIN(vec_size(vec), cmlib_vec_reserve_new_capacity__);          \
            if ((vec) && cmlib_vec_reserve_size__)                             \
            {                                                                  \
                memcpy(cmlib_vec_reserve_temp__,                               \
                    vec,                                                       \
                    sizeof(*(vec)) * cmlib_vec_reserve_size__);                \
            }                                                                  \
            vec_dtor(vec);                                                     \
            (vec) = cmlib_vec_reserve_temp__;                                  \
            cmlib_details_get_vec_header(vec)->size =                          \
                cmlib_vec_reserve_size__;                                      \
        }                                                                      \
        cmlib_vec_reserve_error__;                                             \
    })

#define VEC_ITER(vec, iter_name, ...)                                          \
    assert(vec);                                                               \
    SWITCH_EMPTY(for (size_t iter_name = 0,                                    \
                         cmlib_vec_iter_##iter_name##_end__ = vec_size(vec);   \
                     iter_name < cmlib_vec_iter_##iter_name##_end__;           \
                     iter_name++),                                             \
        for (size_t iter_name = FIRST(__VA_ARGS__),                            \
            cmlib_vec_iter_##iter_name##_end__ =                               \
                MIN((size_t)EXPAND_BUT_FIRST(__VA_ARGS__), vec_size(vec));     \
            iter_name < cmlib_vec_iter_##iter_name##_end__;                    \
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
