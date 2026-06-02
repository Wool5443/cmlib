#ifndef CMLIB_FREE_LIST_H_
#define CMLIB_FREE_LIST_H_

#include <stdio.h>

typedef struct FreeList FreeList;

FreeList* free_list_ctor(size_t pool_size);

void free_list_dtor(FreeList* free_list);

void* free_list_allocate(FreeList* free_list, size_t size, size_t alignment);

#define free_list_allocate_type(free_list, type)                               \
    (free_list_allocate(free_list, sizeof(type), alignof(type)))

void free_list_deallocate(FreeList* free_list, void* ptr);

void free_list_dump_dot(const FreeList* free_list, FILE* out);

#endif // CMLIB_FREE_LIST_H_
