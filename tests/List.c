#include "List.h"

int main()
{
    ERROR_CHECKING();

    logger_init_console();

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
