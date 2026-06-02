#include "List.h"

#include <stdio.h>

#include "Error.h"

int main(void)
{
    ERROR_CHECKING();

    list_ctor(list, get_malloc_resource());
    if (!list)
    {
        err = ERROR_NO_MEMORY;
        ERROR_LEAVE();
    }

    if (!list_insert_after(list, list_end(list), 10))
    {
        err = ERROR_NO_MEMORY;
        ERROR_LEAVE();
    }
    if (!list_insert_after(list, list_begin(list), 20))
    {
        err = ERROR_NO_MEMORY;
        ERROR_LEAVE();
    }
    if (!list_insert_before(list, list_begin(list), 5))
    {
        err = ERROR_NO_MEMORY;
        ERROR_LEAVE();
    }

    printf("list:");
    LIST_ITER(list, node)
    {
        printf(" %d", *list_node_get_value(node, int));
    }
    printf("\n");

    ERROR_CASE

    list_dtor(list);
    return err;
}
