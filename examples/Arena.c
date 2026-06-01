#include "Arena.h"

#include <stdio.h>

#include "ArenaResource.h"
#include "Vector.h"

int main(void)
{
    Arena* arena = arena_ctor(128);
    if (!arena)
    {
        return 1;
    }

    int* direct = arena_allocate_type(arena, int);
    if (!direct)
    {
        arena_dtor(arena);
        return 1;
    }
    *direct = 42;
    printf("arena int: %d\n", *direct);
    arena_dtor(arena);

    Arena* vector_arena = arena_ctor(10000);
    if (!vector_arena)
    {
        return 1;
    }

    ArenaResource res = arena_to_resource(vector_arena);

    int* vec = vec_ctor(&res, int);

    for (int i = 0; i < 100; i++)
    {
        vec_add(vec, i);
    }

    VEC_ITER(vec, i)
    {
        printf("%d ", vec[i]);
    }

    printf("\n");

    arena_resource_dtor(&res);
}
