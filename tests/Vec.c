#include "Vector.h"

void* myalloc(size_t size)
{
    static char buf[1000000];
    return buf;
}

void myfree(void* ptr) {}

int main(void)
{
    ERROR_CHECKING();

    logger_init_console();

    int* vec = NULL;

    for (int i = 0; i < 1000; i++)
    {
        CHECK_ERROR(vec_add(vec, i));
    }

    CHECK_ERROR(vec_reserve(vec, 2000));

    int sum = 0;
    VEC_ITER(vec, i)
    {
        sum += vec[i];
    }
    printf("Got: %d\nShould be: %d\n\n", sum, 999 * 1000 / 2);

    CHECK_ERROR(vec_reserve(vec, 500));

    sum = 0;
    VEC_ITER(vec, i, 10, 15)
    {
        sum += vec[i];
    }
    printf("Got: %d\nShould be: %d\n", sum, 60);

ERROR_CASE
    vec_dtor(vec);
}
