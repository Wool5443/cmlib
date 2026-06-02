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

/**
 * @brief Constructs a List with specified memory resource
 *
 * @param list
 *
 * @return creates a List* with specified name which is a valid list.
 */
#define list_ctor(name, memory_resource__)                                     \
    List cmlib_list_ctor_##name##__ =                                          \
        (List) {                                                               \
            .base =                                                            \
                (ListNode) {                                                   \
                    &cmlib_list_ctor_##name##__.base,                          \
                    &cmlib_list_ctor_##name##__.base                           \
                },                                                             \
            .memory_resource = (MemoryResource*)memory_resource__              \
        }                                                                      \
                                                                               \
    ;                                                                          \
    List* name = &cmlib_list_ctor_##name##__;

// List* list_ctor(void* memory_resource);

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
                   *cmlib_list_iter_##iter_name##_end__ = list_end(list);      \
        iter_name != cmlib_list_iter_##iter_name##_end__;                      \
        iter_name = iter_name->next)

#define LIST_REVERSE_ITER(list, iter_name, ...)                                \
    assert(list && "Iterating over NULL list");                                \
    for (ListNode* iter_name = list_end(list)->prev,                           \
                   *cmlib_list_reverse_iter_##iter_name##_end__ =              \
                       list_end(list);                                         \
        iter_name != cmlib_list_reverse_iter_##iter_name##_end__;              \
        iter_name = iter_name->prev)

#define list_node_get_value(node, type)                                        \
    ((type*)((node) ? (void*)((node) + 1) : NULL))

// NOLINTBEGIN(bugprone-sizeof-expression)
#define list_node_ctor_(list, value)                                           \
    ({                                                                         \
        ListNode* cmlib_list_node_ctor_list_node__ =                           \
            cmlib_details_list_node_ctor(list,                                 \
            sizeof(value),                                                     \
            alignof(typeof(value)));                                           \
        if (cmlib_list_node_ctor_list_node__)                                  \
        {                                                                      \
            *(typeof(value)*)(cmlib_list_node_ctor_list_node__ + 1) = value;    \
        }                                                                      \
        cmlib_list_node_ctor_list_node__;                                      \
    })
// NOLINTEND(bugprone-sizeof-expression)

#define list_insert_after(list, node, value)                                   \
    ({                                                                         \
        List* cmlib_list_insert_after_list__ = (list);                         \
        ListNode* cmlib_list_insert_after_node__ = (node);                     \
        ListNode* cmlib_list_insert_after_new_node__ =                         \
            cmlib_list_insert_after_list__ && cmlib_list_insert_after_node__   \
            ? list_node_ctor_(cmlib_list_insert_after_list__, value)           \
            : NULL;                                                            \
        list_insert_node_after(cmlib_list_insert_after_list__,                 \
            cmlib_list_insert_after_node__,                                    \
            cmlib_list_insert_after_new_node__);                               \
    })

#define list_insert_before(list, node, value)                                  \
    ({                                                                         \
        List* cmlib_list_insert_before_list__ = (list);                        \
        ListNode* cmlib_list_insert_before_node__ = (node);                    \
        ListNode* cmlib_list_insert_before_new_node__ =                        \
            cmlib_list_insert_before_list__ && cmlib_list_insert_before_node__ \
            ? list_node_ctor_(cmlib_list_insert_before_list__, value)          \
            : NULL;                                                            \
        list_insert_node_before(cmlib_list_insert_before_list__,               \
            cmlib_list_insert_before_node__,                                   \
            cmlib_list_insert_before_new_node__);                              \
    })

#endif // CMLIB_LIST_H_
