#include "List.h"

void* cool_alloc(size_t size)
{
    // void* p = malloc(size);
    // log_debug("Allocated[%p] of %zu bytes", p, size);
    return NULL;
}

void cool_free(void* p)
{
    // log_debug("Deallocated[%p]", p);
    // free(p);
}

int main()
{
    ERROR_CHECKING();

    logger_init_console();

    list_set_allocator((Allocator) {cool_alloc, cool_free});
    ListNode* list = list_ctor();

    int val = 1;
    ListNode* new_node = list_insert_after(list, val);

    val = 2;
    new_node = list_insert_after(list, val);
    val = 3;
    new_node = list_insert_after(list, val);
    val = -1;
    new_node = list_insert_before(list, val);
    val = -2;
    new_node = list_insert_before(list, val);

    LIST_ITER(list, current)
    {
        int val = *(int*)list_node_get_value(current);
        printf("%d\n", val);
    }

    LIST_REVERSE_ITER(list, current)
    {
        int val = *(int*)list_node_get_value(current);
        printf("%d\n", val);
    }

    list_dtor(list);

    ListNode* empty_list = list_ctor();
    list_dtor(empty_list);
}
