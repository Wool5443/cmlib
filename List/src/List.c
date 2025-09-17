#include "../List.h"

Allocator Current_list_allocator = MALLOC_ALLOCATOR;

List_node* list_ctor();
void list_dtor(List_node* node);
void list_set_allocator(Allocator allocator);
void list_reset_allocator();
List_node* list_begin(List_node* node);
List_node* list_end(List_node* node);
