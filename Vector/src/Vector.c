#include "../Vector.h"
#undef vec_ctor

cmlib_details_VHeader_* cmlib_details_get_vec_header(void*);
void vec_dtor(void*);
size_t vec_size(void*);
size_t vec_capacity(void*);
void vec_clear(void*);

void* cmlib_details_vec_ctor(void* memory_resource, size_t elem_size, size_t capacity)
{
    if (elem_size == 0 || capacity == 0)
    {
        return NULL;
    }

    MemoryResource* resource = (MemoryResource*)memory_resource;
    if (!resource)
    {
        return NULL;
    }

    cmlib_details_VHeader_* header = resource->allocate(resource,
        capacity * elem_size + sizeof(cmlib_details_VHeader_),
        elem_size);

    if (!header)
    {
        return NULL;
    }

    *header = (cmlib_details_VHeader_) {resource, 0, capacity};

    return &header[1];
}

void* cmlib_details_vec_realloc(void* vec, size_t elem_size)
{
    if (!vec)
    {
        return NULL;
    }

    cmlib_details_VHeader_* header = cmlib_details_get_vec_header(vec);

    if (header->size < header->capacity)
    {
        return vec;
    }

    size_t new_capacity = header->capacity * 2;

    void* new_vec = cmlib_details_vec_ctor(header->memory_resource, elem_size, new_capacity);
    if (!new_vec)
    {
        return NULL;
    }

    memcpy(new_vec, vec, elem_size * header->size);

    *cmlib_details_get_vec_header(
        new_vec) = (cmlib_details_VHeader_) {header->memory_resource,
        header->size,
        new_capacity};

    header->memory_resource->deallocate(header->memory_resource, header);

    return new_vec;
}
