#include <stdio.h>

#include "Arena.h"
#include "ArenaResource.h"
#include "Vector.h"

int main(void)
{
    Result_Arena arena_res = arena_ctor(128);
    if (arena_res.error_code)
    {
        return 1;
    }

    Arena arena = arena_res.value;
    int* direct = arena_allocate_type(&arena, int);
    if (!direct)
    {
        arena_dtor(&arena);
        return 1;
    }
    *direct = 42;
    printf("arena int: %d\n", *direct);
    arena_dtor(&arena);

    Result_Arena vector_arena_res = arena_ctor(10000);
    if (vector_arena_res.error_code)
    {
        return 1;
    }

    Arena vector_arena = vector_arena_res.value;
    ArenaResource res = arena_to_resource(&vector_arena);

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
