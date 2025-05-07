#include <stdlib.h>

#include "Vector.h"
#include "FreeList.h"

int main()
{
    ERROR_CHECKING();

    logger_init_console();

    FreeListMemoryPool* pool = free_list_new_pool(6000);

    for (size_t i = 0; i < 500; i++)
    {
        int* n = free_list_memory_pool_allocate(pool, sizeof(n));
        if (!n)
        {
            log_error("n must not be NULL");
            ERROR_LEAVE();
        }
        *n = 5;
        if (i % 4 == 0)
        {
            free_list_memory_pool_free(pool, n);
        }
    }

    log_info("FreeListAllocator test succeed!");

ERROR_CASE
    return 1;
}
