#include "../Vector.h"

void vec_dtor(void* vec);
size_t vec_size(void* vec);
size_t vec_capacity(void* vec);
void vec_clear(void* vec);

void* vec_ctor_(size_t elemSize, size_t capacity);
void* vec_realloc_(void* vec, size_t elemSize);
