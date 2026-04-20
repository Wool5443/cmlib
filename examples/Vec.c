#include <stdio.h>

#include "Vector.h"

int main(void)
{
    int* numbers = vec_ctor(get_malloc_resource(), int);
    if (!numbers)
    {
        return 1;
    }

    for (int i = 1; i <= 5; ++i)
    {
        if (vec_add(numbers, i) != EVERYTHING_FINE)
        {
            vec_dtor(numbers);
            return 1;
        }
    }

    printf("vector:");
    VEC_ITER(numbers, i)
    {
        printf(" %d", numbers[i]);
    }
    printf("\n");

    printf("last: %d\n", vec_pop(numbers));

    vec_dtor(numbers);
    return 0;
}
