#include <stdlib.h>

#include "Free_list.h"
#include "Vector.h"

static Free_list* free_list;

static void* free_list_allocator_allocate(size_t size)
{
    return free_list_allocate(free_list, size);
}

static void free_list_allocator_free(void* ptr)
{
    return free_list_free(free_list, ptr);
}

int main()
{
    ERROR_CHECKING();

    logger_init_console();
    log_info("Start Free_list_allocator test");

    Free_list_memory_pool* pool = free_list_pool_ctor(6000);

    for (size_t i = 0; i < 500; i++)
    {
        size_t* n = free_list_pool_allocate(pool, sizeof(n));
        if (!n)
        {
            err = ERROR_BAD_VALUE;
            log_error("n must not be NULL");
            ERROR_LEAVE();
        }
        *n = i;
        if (i % 4 == 0)
        {
            free_list_pool_free(pool, n);
        }
    }

    Result_Free_list free_list_res = free_list_ctor(200);
    CHECK_ERROR_LOG(free_list_res.error_code,
                    "Failed to create free list allocator");
    free_list = &free_list_res.value;

    for (size_t i = 0; i < 2000; i++)
    {
        size_t* n = free_list_allocate(free_list, sizeof(*n));
        if (!n)
        {
            err = ERROR_BAD_VALUE;
            log_error("n must not be NULL");
            ERROR_LEAVE();
        }
        *n = i;
        if (i % 11 == 0)
        {
            free_list_free(free_list, n);
        }
    }

    Allocator free_list_allocator = {
        .allocate = free_list_allocator_allocate,
        .free = free_list_allocator_free,
    };

    Current_vector_allocator = free_list_allocator;

    int* numbers = NULL;

    int max_i = 100000000;
    for (int i = 0; i < max_i; i++)
    {
        CHECK_ERROR_LOG(vec_add(numbers, i), "Failed to add %d to numbers", i);
    }

    uint64_t s = 0;
    uint64_t expected_s = (uint64_t)max_i * (max_i - 1) / 2;
    VEC_ITER(numbers, i)
    {
        s += numbers[i];
    }

    if (s != expected_s)
    {
        err = ERROR_BAD_VALUE;
        CHECK_ERROR_LOG(ERROR_BAD_VALUE,
                        "Bad sum %lu, %lu expected",
                        s,
                        expected_s);
    }
    log_info("Got good sum! %lu", s);

    log_info("Free_list_allocator test succeed!");

    ERROR_CASE
    free_list_dtor(free_list);
    free_list_pool_dtor(pool);

    return err;
}
