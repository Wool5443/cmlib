#include "../Vector.h"

void* vecCtor_(size_t elemSize, size_t capacity);
void VecDtor(void* vec);
size_t VecSize(void* vec);
size_t VecCapacity(void* vec);
void VecClear(void* vec);
void* vecRealloc_(void* vec, size_t elemSize);
