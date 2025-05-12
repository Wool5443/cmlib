#include <stdlib.h>

#include "Vector.h"
#include "Arena.h"

static Arena arena;

static void* myallocate(size_t size);
static int cmp(const void* a, const void* b);

static void* myallocate(size_t size)
{
    return arena_allocate(&arena, size);
}

static void myfree(void*)
{
}

static int cmp(const void* a, const void* b)
{
    void* ap = *(void**)a;
    void* bp = *(void**)b;

    return ap - bp;
}

int main()
{
    ERROR_CHECKING();

    logger_init_console();

    arena = arena_ctor(1024 * 1024 * 10).value;

    Allocator myallocator = {
        myallocate,
        myfree,
    };

    Current_vector_allocator = myallocator;

    int* vec = NULL;
    void** allocations = NULL;

    for (int i = 0; i < 100; i++)
    {
        CHECK_ERROR_LOG(vec_add(vec, i));
    }

    Current_vector_allocator = CALLOC_ALLOCATOR;

    for (void* p = arena_allocate(&arena, 56); p; p = arena_allocate(&arena, 34))
    {
        CHECK_ERROR_LOG(vec_add(allocations, p));
    }

    qsort(allocations, vec_size(allocations), sizeof(*allocations), cmp);

    VEC_ITER(allocations, i, 0, vec_size(allocations) - 1)
    {
        if (allocations[i] == allocations[i + 1])
        {
            printf("%p\n", allocations[i]);
        }
    }

    log_info("ArenaAllocator test succeed!");

ERROR_CASE
    vec_dtor(vec);
    vec_dtor(allocations);
    arena_dtor(&arena);

    return err;
}
