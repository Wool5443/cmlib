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

      do {
        if ((err = (({
               Error_code vecAddError_ = ERROR_NO_MEMORY;
               void *temp = vec_realloc_((vec), sizeof(*vec));
               if (temp) {
                 vecAddError_ = EVERYTHING_FINE;
                 (vec) = temp;
                 VHeader_ *header = (&((VHeader_ *)(vec))[-1]);
                 (vec)[header->size++] = i;
               }
               vecAddError_;
             })))) {
          goto ERROR_CASE_;
        }
      } while (0);
    }

    vec_reserve(vec, 2000);

    int sum = 0;
    for (int i = 0; i < 1000; i++)
    {
        sum += vec[i];
    }

    printf("Got: %d\nShould be: %d\n", sum, 999 * 1000 / 2);

ERROR_CASE
}
