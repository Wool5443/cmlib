#include "FreeList.h"
#include "FreeListResource.h"

#include <stdio.h>

int main(void)
{
    Result_FreeList free_list_res = free_list_ctor(256);
    if (free_list_res.error_code != EVERYTHING_FINE)
    {
        return 1;
    }

    FreeList free_list = free_list_res.value;

    int* first =
        free_list_allocate(&free_list, sizeof(*first), alignof(*first));
    int* second = free_list_allocate_type(&free_list, int);
    if (!first || !second)
    {
        free_list_dtor(&free_list);
        return 1;
    }

    *first = 10;
    *second = 20;
    printf("%d + %d = %d\n", *first, *second, *first + *second);

    free_list_deallocate(&free_list, second);
    free_list_deallocate(&free_list, first);

    FILE* dump_file = fopen("exampledump", "w");
    if (!dump_file)
    {
        free_list_dtor(&free_list);
        return 1;
    }

    free_list_dump_dot(&free_list, dump_file);
    fclose(dump_file);

    FreeListResource resource = free_list_to_resource(&free_list);
    int* third =
        resource.base.allocate(&resource.base, sizeof(*third), alignof(*third));
    if (!third)
    {
        free_list_resource_dtor(&resource);
        return 1;
    }

    *third = 30;
    printf("resource int: %d\n", *third);
    resource.base.deallocate(&resource.base, third);
    free_list_resource_dtor(&resource);
    return 0;
}
