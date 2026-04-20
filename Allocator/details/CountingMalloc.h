#ifndef CMLIB_COUNTING_MALLOC_H_
#define CMLIB_COUNTING_MALLOC_H_

#include <stddef.h>

extern size_t standard_allocations_count;
extern size_t standard_frees_count;

void* cmlib_details_malloc(size_t size);
void* cmlib_details_calloc(size_t nmemb, size_t size);

void cmlib_details_free(void* ptr);

#endif // CMLIB_COUNTING_MALLOC_H_
