#include "Arena.h"

DECLARE_RESULT_SOURCE(Arena);

Result_Arena arena_ctor(size_t capacity);
void* arena_allocate(Arena* arena, size_t size);
void free_arena(Arena* restrict arena);
