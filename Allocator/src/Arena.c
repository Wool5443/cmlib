#include "Arena.h"

DECLARE_RESULT_SOURCE(Arena);

Result_Arena arena_ctor(size_t capacity);
void* arena_allocate(Arena* arena, size_t size);
void arena_flush(Arena* arena);
void arena_dtor(Arena* arena);
