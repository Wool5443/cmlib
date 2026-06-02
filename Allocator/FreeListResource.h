#ifndef CMLIB_FREE_LIST_RESOURCE_H_
#define CMLIB_FREE_LIST_RESOURCE_H_

#include <stddef.h>

#include "Allocator.h"
#include "FreeList.h"
#include "Result.h"

typedef struct FreeListResource
{
    MemoryResource base;
    FreeList* free_list;
} FreeListResource;

DECLARE_RESULT_HEADER(FreeListResource);

Result_FreeListResource free_list_resource_ctor(size_t pool_size);

FreeListResource free_list_to_resource(FreeList* free_list);

void free_list_resource_dtor(FreeListResource* resource);

#endif // CMLIB_FREE_LIST_RESOURCE_H_
