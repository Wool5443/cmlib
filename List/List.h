/**
 * @file List.h
 * @brief Circular doubly linked list with inline payload nodes.
 *
 * A `List` owns its sentinel node and memory resource. `ListNode` values store
 * only links; node storage is released through the owning list.
 */

#ifndef CMLIB_LIST_H_
#define CMLIB_LIST_H_

#include <assert.h>
#include <stdalign.h>

#include "Allocator.h"

/**
 * @brief Link header embedded in the list sentinel and value nodes.
 */
typedef struct ListNode ListNode;
struct ListNode
{
    ListNode* prev; /**< Previous node. */
    ListNode* next; /**< Next node. */
};

/**
 * @brief List container owning the sentinel node and memory resource.
 *
 * The resource is stored only here, not in individual nodes.
 */
typedef struct List
{
    ListNode base;                   /**< Sentinel node. */
    MemoryResource* memory_resource; /**< Resource used for list and nodes. */
} List;

/**
 * @brief Creates an empty list.
 * @param memory_resource Resource pointer; must be non-NULL.
 * @return List pointer, or NULL on failure.
 */
List* list_ctor(void* memory_resource);

/**
 * @brief Destroys entire list.
 * @param list List pointer returned by `list_ctor`; NULL is accepted.
 */
void list_dtor(List* list);

/**
 * @brief Returns first data node or sentinel for an empty list.
 * @param list List pointer.
 * @return First data node, sentinel, or NULL for NULL input.
 */
ListNode* list_begin(List* list);

/**
 * @brief Returns list sentinel.
 * @param list List pointer.
 * @return Sentinel pointer, or NULL for NULL input.
 */
ListNode* list_end(List* list);

/**
 * @brief Extracts and frees a linked node.
 * @param list Owning list.
 * @param node Node pointer; NULL is accepted.
 */
void list_erase(List* list, ListNode* node);

/**
 * @brief Unlinks node from its list without freeing it.
 * @param list Owning list.
 * @param node Linked node.
 * @return Extracted node, or NULL for invalid input.
 */
ListNode* list_extract(List* list, ListNode* node);

/**
 * @brief Inserts an existing node immediately after another node.
 * @param list Owning list.
 * @param node Existing list node.
 * @param insert_node Detached node to insert.
 * @return Inserted node, or NULL on invalid input.
 *
 * Detached nodes may be exchanged between lists only when both lists use the
 * same `MemoryResource`.
 */
ListNode*
list_insert_node_after(List* list, ListNode* node, ListNode* insert_node);

/**
 * @brief Inserts an existing node immediately before another node.
 * @param list Owning list.
 * @param node Existing list node.
 * @param insert_node Detached node to insert.
 * @return Inserted node, or NULL on invalid input.
 *
 * Detached nodes may be exchanged between lists only when both lists use the
 * same `MemoryResource`.
 */
ListNode*
list_insert_node_before(List* list, ListNode* node, ListNode* insert_node);

/**
 * @brief Allocates a detached node header plus payload storage.
 * @param list Owning list; must be non-NULL.
 * @param payload_size Payload byte count.
 * @param payload_alignment Payload alignment.
 * @return Detached node, or NULL on failure.
 */
ListNode* cmlib_details_list_node_ctor(List* list,
    size_t payload_size,
    size_t payload_alignment);

/**
 * @brief Iterates forward over data nodes.
 */
#define LIST_ITER(list, iter_name, ...)                                        \
    assert(list && "Iterating over NULL list");                                \
    for (ListNode* iter_name = list_begin(list),                               \
                   *iter_name##_end = list_end(list);                          \
        iter_name != iter_name##_end;                                          \
        iter_name = iter_name->next)

/**
 * @brief Iterates backward over data nodes.
 */
#define LIST_REVERSE_ITER(list, iter_name, ...)                                \
    assert(list && "Iterating over NULL list");                                \
    for (ListNode* iter_name = list_end(list)->prev,                           \
                   *iter_name##_end = list_end(list);                          \
        iter_name != iter_name##_end;                                          \
        iter_name = iter_name->prev)

/**
 * @brief Returns pointer to payload after node header.
 */
#define list_node_get_value(node, type)                                        \
    ((type*)((node) ? (void*)((node) + 1) : NULL))

/**
 * @brief Allocates a detached node with inline payload copy.
 * @param list List pointer; must be non-NULL.
 * @param value Payload expression.
 * @return Detached node, or NULL on failure.
 */
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

/**
 * @brief Allocates payload node and inserts it after a node.
 * @param list Owning list.
 * @param node Existing list node.
 * @param value Payload expression.
 * @return Inserted node, or NULL on failure.
 */
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

/**
 * @brief Allocates payload node and inserts it before a node.
 * @param list Owning list.
 * @param node Existing list node.
 * @param value Payload expression.
 * @return Inserted node, or NULL on failure.
 */
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
