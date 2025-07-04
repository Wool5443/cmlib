/**
 * @file List.h
 * @author Misha Solodilov (mihsolodilov2015@gmail.com)
 * @brief Header file for doubly linked list utilities.
 *
 * This header defines the data structures and functions required to implement
 * and manage a doubly linked list. The list is implemented using nodes that
 * store data and pointers to the previous and next nodes. The allocator is
 * abstracted using the `Allocator` structure to provide flexibility in the
 * memory allocation strategy.
 *
 * The file provides functions for inserting nodes, getting node values, setting
 * the allocator, and resetting the allocator to its default state. The list
 * implementation is designed to support flexible memory allocation and
 * efficient insertion and deletion operations.
 *
 * @version 1.0
 * @date 12.06.2025
 *
 * @copyright Copyright (c) 2025
 */

#ifndef CMLIB_LIST_H_
#define CMLIB_LIST_H_

#include "Allocator.h"
#include "Logger.h"

/**
 * @brief Structure to represent the header of a list node.
 *
 * Each node in the list contains a `ListNode` that holds pointers to the
 * previous and next nodes in the list. It also contains an allocator to manage
 * memory for the node.
 */
typedef struct ListNode ListNode;
struct ListNode
{
    ListNode* prev;      /**< Pointer to the previous node in the list */
    ListNode* next;      /**< Pointer to the next node in the list */
    Allocator allocator; /**< The allocator used to manage memory for this node
                          */
};

extern Allocator Current_list_allocator; /**< The current allocator used by the
                                            list */

/**
 * @brief Sets the allocator to be used for list node allocations.
 *
 * This function allows the user to specify a custom allocator for all future
 * list node allocations.
 *
 * @param allocator The allocator to be set for the list.
 */
INLINE void list_set_allocator(Allocator allocator);

/**
 * @brief Resets the list allocator to the default allocator (malloc).
 *
 * This function resets the allocator to the default `malloc` allocator for
 * memory allocation.
 */
INLINE void list_reset_allocator();

/**
 * @brief Creates a list.
 *
 * Allocates an empty utility node using current list allocator for a list.
 *
 * @return
 */
INLINE ListNode* list_ctor();

/**
 * @brief Destroys a list node.
 *
 * This function deallocates the memory for the specified list node.
 *
 * @param node Pointer to the node to be destroyed.
 */
INLINE void list_dtor(ListNode* node);

/**
 * @brief Retrieves the first node in the list.
 *
 * This function returns the starting node of the list, which is the node that
 * does not have any previous node.
 *
 * @param list Pointer to the list.
 * @return Pointer to the first node in the list.
 */
INLINE ListNode* list_begin(ListNode* list);

/**
 * @brief Retrieves the last node in the list.
 *
 * This function returns the ending node of the list, which is the node that
 * does not have any next node.
 *
 * @param list Pointer to the list.
 * @return Pointer to the last node in the list.
 */
INLINE ListNode* list_end(ListNode* list);

#define LIST_ITER(list__, iter_name__, ...)                                    \
    assert(list__ && "Iterating over NULL list");                              \
    for (ListNode* iter_name__ = list_begin(list__),                           \
                   *iter_name__##_end = list_end(list__);                      \
         iter_name__ != iter_name__##_end;                                     \
         iter_name__ = iter_name__->next)

#define LIST_REVERSE_ITER(list__, iter_name__, ...)                            \
    assert(list__ && "Iterating over NULL list");                              \
    for (ListNode* iter_name__ = list_end(list__)->prev,                       \
                   *iter_name__##_end = list_end(list__);                      \
         iter_name__ != iter_name__##_end;                                     \
         iter_name__ = iter_name__->prev)

INLINE void list_set_allocator(Allocator allocator)
{
    Current_list_allocator = allocator;
}

INLINE void list_reset_allocator()
{
    Current_list_allocator = MALLOC_ALLOCATOR;
}

INLINE ListNode* list_ctor()
{
    ListNode* list = Current_list_allocator.allocate(sizeof(ListNode));
    if (list)
    {
        *list = (ListNode) {
            .allocator = Current_list_allocator,
            .next = list,
            .prev = list,
        };
    }
    else
    {
        int err = ERROR_NO_MEMORY;
        log_error("Could not create list");
    }
    return list;
}

INLINE void list_dtor(ListNode* list)
{
    if (!list)
    {
        return;
    }

    ListNode* to_delete = list->next;
    ListNode* current = to_delete;

    while (current != list)
    {
        current = current->next;
        to_delete->allocator.free(to_delete);
        to_delete = current;
    }
    to_delete->allocator.free(current);
}

INLINE ListNode* list_begin(ListNode* list)
{
    if (!list)
    {
        int err = ERROR_NULLPTR;
        log_error("NULL passed as list");
        return NULL;
    }
    return list->next;
}

INLINE ListNode* list_end(ListNode* list)
{
    if (!list)
    {
        int err = ERROR_NULLPTR;
        log_error("NULL passed as list");
        return NULL;
    }
    return list;
}

/**
 * @brief Gets the value of a list node.
 *
 * This macro extracts the value of a list node, which is stored immediately
 * after the `ListNode`.
 *
 * @param node Pointer to the list node.
 * @return Pointer to the value stored in the node.
 */
#define list_node_get_value(node__) ((node__) ? (void*)((node__) + 1) : NULL)

/**
 * @brief Creates and inserts a new node after a given node.
 *
 * This macro inserts a new node after the specified `node` and assigns the
 * provided value to it. If the node is `NULL`, a new node is created and
 * returned.
 *
 * @param node The node after which the new node should be inserted.
 * @param value The value to be assigned to the new node.
 * @return The newly inserted node.
 */
#define list_insert_after(node__, value__)                                     \
    ({                                                                         \
        ListNode* node_insert_after_ = (node__);                               \
        if (node_insert_after_)                                                \
        {                                                                      \
            Allocator allocator_insert_after_ = node_insert_after_->allocator; \
            ListNode* new_node_insert_after_ =                                 \
                list_node_ctor_(value__, allocator_insert_after_);             \
            if (new_node_insert_after_)                                        \
            {                                                                  \
                *new_node_insert_after_ = (ListNode) {                         \
                    .prev = node_insert_after_,                                \
                    .next = node_insert_after_->next,                          \
                    .allocator = allocator_insert_after_,                      \
                };                                                             \
                if (node_insert_after_->next)                                  \
                {                                                              \
                    node_insert_after_->next->prev = new_node_insert_after_;   \
                }                                                              \
                node_insert_after_->next = new_node_insert_after_;             \
                node_insert_after_ = new_node_insert_after_;                   \
            }                                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            int err = ERROR_NULLPTR;                                           \
            log_error("Attempted to insert after NULL node");                  \
        }                                                                      \
        node_insert_after_;                                                    \
    })

/**
 * @brief Creates and inserts a new node before a given node.
 *
 * This macro inserts a new node before the specified `node` and assigns the
 * provided value to it. If the node is `NULL`, a new node is created and
 * returned.
 *
 * @param node The node before which the new node should be inserted.
 * @param value The value to be assigned to the new node.
 * @return The newly inserted node.
 */
#define list_insert_before(node__, value__)                                    \
    ({                                                                         \
        ListNode* node_insert_before_ = (node__);                              \
        if (node_insert_before_)                                               \
        {                                                                      \
            node_insert_before_ = list_insert_after(node_insert_before_->prev, \
                                                    value__);                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            int err = ERROR_NULLPTR;                                           \
            log_error("Attempted to insert before NULL node");                 \
        }                                                                      \
        node_insert_before_;                                                   \
    })

/**
 * @brief Creates a new list node with a specified value and allocator.
 *
 * This macro allocates memory for a new list node, assigns the provided value
 * to the node, and initializes the `ListNode` portion of the node.
 *
 * @param value The value to be assigned to the node.
 * @param allocator_ The allocator used for memory allocation.
 * @return Pointer to the newly created node.
 */
// NOLINTBEGIN(bugprone-sizeof-expression)
#define list_node_ctor_(value__, allocator__)                                  \
    ({                                                                         \
        Allocator allocator_node_ctor_ = allocator__;                          \
        ListNode* node_node_ctor_ =                                            \
            allocator_node_ctor_.allocate(sizeof(ListNode) + sizeof(value__)); \
        if (node_node_ctor_)                                                   \
        {                                                                      \
            *node_node_ctor_ = (ListNode) {.allocator = allocator_node_ctor_}; \
            *(typeof(value__)*)(node_node_ctor_ + 1) = value__;                \
            node_node_ctor_->allocator = allocator_node_ctor_;                 \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            int err = ERROR_NO_MEMORY;                                         \
            log_error("Could not allocate a node");                            \
        }                                                                      \
        node_node_ctor_;                                                       \
    })
// NOLINTEND(bugprone-sizeof-expression)

#endif // CMLIB_LIST_H_
