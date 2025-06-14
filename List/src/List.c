#include "../List.h"

Allocator Current_list_allocator = MALLOC_ALLOCATOR;

void list_set_allocator(Allocator allocator);
void list_reset_allocator();
void list_dtor(ListHeader* node);
ListHeader* list_start(ListHeader* node);
ListHeader* list_end(ListHeader* node);
Allocator get_list_node_allocator(void* node__);
ListHeader* hide_list_node_pointer_(void* node__);
