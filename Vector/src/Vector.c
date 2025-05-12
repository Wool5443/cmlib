#include "../Vector.h"

Allocator Current_vector_allocator = MALLOC_ALLOCATOR;

void vec_set_allocator(Allocator allocator);
void vec_reset_allocator();
void vec_dtor(void* vec);
size_t vec_size(void* vec);
size_t vec_capacity(void* vec);
void vec_clear(void* vec);

void* vec_ctor_(Allocator allocator, size_t elem_size, size_t capacity);
void* vec_realloc_(void* vec, size_t elemSize);
