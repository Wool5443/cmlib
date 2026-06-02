#include "List.h"

#include "../../common.h"
#include "List.h"

// List* list_ctor(void* memory_resource)
// {
//     List* ptr = NULL;
//     MemoryResource* resource = (MemoryResource*)memory_resource;
//
//     List cmlib_details_list = {
//         .base =
//             (ListNode) {
//                 .prev = &cmlib_details_list.base,
//                 .next = &cmlib_details_list.base
//             },
//         .memory_resource = resource,
//     };
//
//     if (resource)
//     {
//         ptr = &cmlib_details_list;
//     }
//
//     ptr;
// }

void list_dtor(List* list)
{
    if (!list)
    {
        return;
    }

    MemoryResource* resource = list->memory_resource;
    ListNode* current = list->base.next;
    while (current != &list->base)
    {
        ListNode* next = current->next;
        resource->deallocate(resource, current);
        current = next;
    }
}

ListNode* list_begin(List* list)
{
    return list ? list->base.next : NULL;
}

ListNode* list_end(List* list)
{
    return list ? &list->base : NULL;
}

void list_erase(List* list, ListNode* node)
{
    if (list_extract(list, node))
    {
        list->memory_resource->deallocate(list->memory_resource, node);
    }
}

ListNode* list_extract(List* list, ListNode* node)
{
    if (!list || !node || node == &list->base || !node->prev || !node->next)
    {
        return NULL;
    }

    node->prev->next = node->next;
    node->next->prev = node->prev;

    node->prev = NULL;
    node->next = NULL;

    return node;
}

ListNode*
list_insert_node_after(List* list, ListNode* node, ListNode* insert_node)
{
    if (!list || !node || !insert_node)
    {
        return NULL;
    }

    insert_node->prev = node;
    insert_node->next = node->next;
    node->next->prev = insert_node;
    node->next = insert_node;

    return insert_node;
}

ListNode*
list_insert_node_before(List* list, ListNode* node, ListNode* insert_node)
{
    return node ? list_insert_node_after(list, node->prev, insert_node) : NULL;
}

ListNode* cmlib_details_list_node_ctor(List* list,
    size_t payload_size,
    size_t payload_alignment)
{
    if (!list || !list->memory_resource)
    {
        return NULL;
    }

    MemoryResource* resource = list->memory_resource;
    ListNode* node = resource->allocate(resource,
        sizeof(ListNode) + payload_size,
        MAX(alignof(ListNode), payload_alignment));
    if (!node)
    {
        return NULL;
    }

    *node = (ListNode) {};
    return node;
}
