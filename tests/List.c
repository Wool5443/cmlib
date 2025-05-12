#include "List.h"

int main()
{
    ERROR_CHECKING();

    logger_init_console();

    auto list = list_insert_after(NULL, 0);
    list = list_insert_after(list, 1);
    list = list_insert_after(list, 2);
    list = list_insert_after(list, 3);
    list = list_insert_before(list, -1);
    list = list_insert_before(list, -2);

    ListHeader* current = list_start(list);

    while (current)
    {
        int val = *(int*)list_get_value(current);
        printf("%d\n", val);
        current = current->next;
    }

    list_dtor(list);
}
