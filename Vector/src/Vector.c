#include "../Vector.h"

Allocator Current_vector_allocator = MALLOC_ALLOCATOR;

void vec_set_allocator(Allocator allocator);
void vec_reset_allocator();
void vec_dtor(void* vec);
size_t vec_size(void* vec);
size_t vec_capacity(void* vec);
void vec_clear(void* vec);

void* vec_realloc_(void* vec, size_t elem_size);

void* vec_ctor_(Allocator allocator, size_t elem_size, size_t capacity)
{
    ERROR_CHECKING();

    if (elem_size == 0)
    {
        err = ERROR_BAD_VALUE;
        log_error("0 passed as elem_size");
        ERROR_LEAVE();
    }

    capacity = capacity ? capacity : DEFAULT_CAPACITY;

    VHeader_* header =
        allocator.allocate(capacity * elem_size + sizeof(VHeader_));
    if (!header)
    {
        HANDLE_ERRNO_ERROR(ERROR_NO_MEMORY, "Error allocating vector: %s");
    }

    header->allocator = allocator;
    header->capacity = capacity;
    *header = (VHeader_) {allocator, 0, capacity};

    return &header[1];

    ERROR_CASE
    return NULL;
}

void* vec_realloc_(void* vec, size_t elem_size)
{
    if (!vec)
    {
        return vec_ctor_(Current_vector_allocator, elem_size, DEFAULT_CAPACITY);
    }

    VHeader_ header = *GET_VEC_HEADER(vec);

    if (header.size < header.capacity)
    {
        return vec;
    }

    size_t new_capacity = header.capacity * 2;

    void* new_vec = vec_ctor_(header.allocator, elem_size, new_capacity);
    if (!new_vec)
        return NULL;

    memcpy(new_vec, vec, elem_size * header.size);

    *GET_VEC_HEADER(new_vec) =
        (VHeader_) {header.allocator, header.size, new_capacity};
    VEC_FREE(vec);

    return new_vec;
}
