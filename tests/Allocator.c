#include "Vector.h"
#include "Arena.h"

static Arena arena;

static void* myallocate(size_t size)
{
    return arena_allocate(&arena, size);
}

static void myfree(void* p)
{

}

int main()
{
    ERROR_CHECKING();

    logger_init_console();

    arena = arena_ctor(1024).value;

    Allocator myallocator = {
        myallocate,
        myfree,
    };

    Current_vector_allocator = myallocator;

    int* vec = NULL;

    for (int i = 0; i < 100; i++)
    {
        CHECK_ERROR_LOG(vec_add(vec, i));
    }

ERROR_CASE
    free_arena(&arena);
}
