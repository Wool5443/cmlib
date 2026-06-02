#ifndef CMLIB_LIST_H_
#define CMLIB_LIST_H_

#include "Allocator.h"

typedef struct ListNode ListNode;
struct ListNode
{
    ListNode* prev;
    ListNode* next;
};

typedef struct List
{
    ListNode base;
    MemoryResource* memory_resource;
} List;

List* list_ctor(void* memory_resource);

void list_dtor(List* list);

ListNode* list_begin(List* list);

ListNode* list_end(List* list);

void list_erase(List* list, ListNode* node);

ListNode* list_extract(List* list, ListNode* node);

ListNode*
list_insert_node_after(List* list, ListNode* node, ListNode* insert_node);

ListNode*
list_insert_node_before(List* list, ListNode* node, ListNode* insert_node);

ListNode* cmlib_details_list_node_ctor(List* list,
    size_t payload_size,
    size_t payload_alignment);

#define LIST_ITER(list, iter_name, ...)                                        \
    assert(list && "Iterating over NULL list");                                \
    for (ListNode* iter_name = list_begin(list),                               \
                   *iter_name##_end = list_end(list);                          \
        iter_name != iter_name##_end;                                          \
        iter_name = iter_name->next)

#define LIST_REVERSE_ITER(list, iter_name, ...)                                \
    assert(list && "Iterating over NULL list");                                \
    for (ListNode* iter_name = list_end(list)->prev,                           \
                   *iter_name##_end = list_end(list);                          \
        iter_name != iter_name##_end;                                          \
        iter_name = iter_name->prev)

#define list_node_get_value(node, type)                                        \
    ((type*)((node) ? (void*)((node) + 1) : NULL))

// NOLINTBEGIN(bugprone-sizeof-expression)
#define list_node_ctor_(list, value)                                           \
    ({                                                                         \
        ListNode* list_node_ = cmlib_details_list_node_ctor(list,              \
            sizeof(value),                                                     \
            alignof(typeof(value)));                                           \
        if (list_node_)                                                        \
        {                                                                      \
            *(typeof(value)*)(list_node_ + 1) = value;                         \
        }                                                                      \
        list_node_;                                                            \
    })
// NOLINTEND(bugprone-sizeof-expression)

#define list_insert_after(list, node, value)                                   \
    ({                                                                         \
        List* list_insert_list_ = (list);                                      \
        ListNode* list_insert_node_ = (node);                                  \
        ListNode* list_new_node_ = list_insert_list_ && list_insert_node_      \
            ? list_node_ctor_(list_insert_list_, value)                        \
            : NULL;                                                            \
        list_insert_node_after(list_insert_list_,                              \
            list_insert_node_,                                                 \
            list_new_node_);                                                   \
    })

#define list_insert_before(list, node, value)                                  \
    ({                                                                         \
        List* list_before_list_ = (list);                                      \
        ListNode* list_before_node_ = (node);                                  \
        ListNode* list_new_before_node_ = list_before_list_                    \
                && list_before_node_                                           \
            ? list_node_ctor_(list_before_list_, value)                        \
            : NULL;                                                            \
        list_insert_node_before(list_before_list_,                             \
            list_before_node_,                                                 \
            list_new_before_node_);                                            \
    })

#endif // CMLIB_LIST_H_
