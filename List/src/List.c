#include "../List.h"

Allocator Current_list_allocator = MALLOC_ALLOCATOR;

ListNode* list_ctor();
void list_dtor(ListNode* node);
void list_set_allocator(Allocator allocator);
void list_reset_allocator();
ListNode* list_begin(ListNode* node);
ListNode* list_end(ListNode* node);
