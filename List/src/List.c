#include "../List.h"

List* list_ctor(void* memory_resource)
{
    MemoryResource* resource = (MemoryResource*)memory_resource;
    if (!resource)
    {
        return NULL;
    }

    List* list = resource->allocate(resource, sizeof(List), alignof(List));
    if (!list)
    {
        return NULL;
    }

    *list = (List) {
        .base = {.next = &list->base, .prev = &list->base},
        .memory_resource = resource,
    };
    return list;
}

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

    resource->deallocate(resource, list);
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

ListNode* list_insert_node_after(List* list, ListNode* node, ListNode* insert_node)
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
