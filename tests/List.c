#include "List.h"

int main()
{
    ERROR_CHECKING();

    logger_init_console();

    List_node* list = list_ctor();

    int val = 1;
     list_insert_after(list, val);

    val = 2;
    list_insert_after(list, val);
    val = 3;
    list_insert_after(list, val);
    val = -1;
    list_insert_before(list, val);
    val = -2;
    list_insert_before(list, val);

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

    List_node* empty_list = list_ctor();
    list_dtor(empty_list);
}
