#include "details/CountingMalloc.h"

#include <stdlib.h>

size_t standard_allocations_count = 0;
size_t standard_frees_count = 0;

void* cmlib_details_malloc(size_t size)
{
    standard_allocations_count++;
    return malloc(size);
}

void* cmlib_details_calloc(size_t nmemb, size_t size)
{
    standard_allocations_count++;
    return calloc(nmemb, size);
}

void cmlib_details_free(void* ptr)
{
    standard_frees_count++;
    free(ptr);
}
