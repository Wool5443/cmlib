/**
 * @file List.h
 * @author Misha Solodilov (mihsolodilov2015@gmail.com)
 * @brief Header file for doubly linked list utilities.
 *
 * This header defines the data structures and functions required to implement and manage a doubly linked list.
 * The list is implemented using nodes that store data and pointers to the previous and next nodes. The allocator
 * is abstracted using the `Allocator` structure to provide flexibility in the memory allocation strategy.
 *
 * The file provides functions for inserting nodes, getting node values, setting the allocator, and resetting the
 * allocator to its default state. The list implementation is designed to support flexible memory allocation and
 * efficient insertion and deletion operations.
 *
 * @version 1.0
 * @date 12.06.2025
 *
 * @copyright Copyright (c) 2025
 */

#ifndef CMLIB_LIST_H_
#define CMLIB_LIST_H_

#include <stdlib.h>
#include <string.h>

#include "../Logger/Logger.h"
#include "../Allocator/Allocator.h"

/**
 * @brief Structure to represent the header of a list node.
 *
 * Each node in the list contains a `ListHeader` that holds pointers to the previous and next nodes in the list.
 * It also contains an allocator to manage memory for the node.
 */
typedef struct ListHeader ListHeader;
struct ListHeader
{
    ListHeader* prev; /**< Pointer to the previous node in the list */
    ListHeader* next; /**< Pointer to the next node in the list */
    Allocator allocator; /**< The allocator used to manage memory for this node */
};

extern Allocator Current_list_allocator; /**< The current allocator used by the list */

/**
 * @brief Sets the allocator to be used for list node allocations.
 *
 * This function allows the user to specify a custom allocator for all future list node allocations.
 *
 * @param allocator The allocator to be set for the list.
 */
INLINE void list_set_allocator(Allocator allocator);

/**
 * @brief Resets the list allocator to the default allocator (malloc).
 *
 * This function resets the allocator to the default `malloc` allocator for memory allocation.
 */
INLINE void list_reset_allocator();

/**
 * @brief Destroys a list node.
 *
 * This function deallocates the memory for the specified list node.
 *
 * @param node Pointer to the node to be destroyed.
 */
INLINE void list_dtor(ListHeader* node);

/**
 * @brief Retrieves the first node in the list.
 *
 * This function returns the starting node of the list, which is the node that does not have any previous node.
 *
 * @param node Pointer to the node in the list.
 * @return Pointer to the first node in the list.
 */
INLINE ListHeader* list_start(ListHeader* node);

/**
 * @brief Retrieves the last node in the list.
 *
 * This function returns the ending node of the list, which is the node that does not have any next node.
 *
 * @param node Pointer to the node in the list.
 * @return Pointer to the last node in the list.
 */
INLINE ListHeader* list_end(ListHeader* node);

INLINE void list_set_allocator(Allocator allocator)
{
    Current_list_allocator = allocator;
}

INLINE void list_reset_allocator()
{
    Current_list_allocator = MALLOC_ALLOCATOR;
}

INLINE void list_dtor(ListHeader* node)
{
    if (!node)
    {
        return;
    }

    ListHeader* current = list_end(node);
    ListHeader* to_delete = current;

    while (current)
    {
        current = current->prev;
        to_delete->allocator.free(to_delete);
        to_delete = current;
    }
}

INLINE ListHeader* list_start(ListHeader* node)
{
    if (!node)
    {
        return NULL;
    }

    while (node->prev)
    {
        node = node->prev;
    }

    return node;
}

INLINE ListHeader* list_end(ListHeader* node)
{
    if (!node)
    {
        return NULL;
    }

    while (node->next)
    {
        node = node->next;
    }

    return node;
}

/**
 * @brief Creates a new list node with a specified value using the current allocator.
 *
 * This macro simplifies node creation using the current global allocator (`Current_list_allocator`).
 *
 * @param value The value to be assigned to the node.
 * @return Pointer to the newly created node.
 */
#define list_node_ctor(value) (list_node_ctor_(value, Current_list_allocator))

/**
 * @brief Gets the value of a list node.
 *
 * This macro extracts the value of a list node, which is stored immediately after the `ListHeader`.
 *
 * @param node Pointer to the list node.
 * @return Pointer to the value stored in the node.
 */
#define list_get_value(node) ((void*)((node) + 1))

/**
 * @brief Creates and inserts a new node after a given node.
 *
 * This macro inserts a new node after the specified `node` and assigns the provided value to it.
 * If the node is `NULL`, a new node is created and returned.
 *
 * @param node The node after which the new node should be inserted.
 * @param value The value to be assigned to the new node.
 * @return The newly inserted node.
 */
#define list_insert_after(node, value)                                          \
({                                                                              \
    ListHeader* node_ = (node);                                                 \
    if (node_)                                                                  \
    {                                                                           \
        ListHeader* new_node = list_node_ctor_(value, node_->allocator);        \
        if (new_node)                                                           \
        {                                                                       \
            *new_node = (ListHeader) {                                          \
                .prev = node_,                                                  \
                .next = node_->next,                                            \
                .allocator = node_->allocator,                                  \
            };                                                                  \
            if (node_->next)                                                    \
            {                                                                   \
                node_->next->prev = new_node;                                   \
            }                                                                   \
            node_->next = new_node;                                             \
            node_ = new_node;                                                   \
        }                                                                       \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        node_ = list_node_ctor(value);                                          \
    }                                                                           \
    node_;                                                                      \
})

/**
 * @brief Creates and inserts a new node before a given node.
 *
 * This macro inserts a new node before the specified `node` and assigns the provided value to it.
 * If the node is `NULL`, a new node is created and returned.
 *
 * @param node The node before which the new node should be inserted.
 * @param value The value to be assigned to the new node.
 * @return The newly inserted node.
 */
#define list_insert_before(node, value)                                         \
({                                                                              \
    ListHeader* node_ = (node);                                                 \
    if (node_)                                                                  \
    {                                                                           \
        ListHeader* new_node = list_node_ctor_(value, node_->allocator);        \
        if (new_node)                                                           \
        {                                                                       \
            *new_node = (ListHeader) {                                          \
                .prev = node_->prev,                                            \
                .next = node_,                                                  \
                .allocator = node_->allocator,                                  \
            };                                                                  \
            if (node_->prev)                                                    \
            {                                                                   \
                node_->prev->next = new_node;                                   \
            }                                                                   \
            node_->prev = new_node;                                             \
            node_ = new_node;                                                   \
        }                                                                       \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        node_ = list_node_ctor(value);                                          \
    }                                                                           \
    node_;                                                                      \
})

/**
 * @brief Creates a new list node with a specified value and allocator.
 *
 * This macro allocates memory for a new list node, assigns the provided value to the node,
 * and initializes the `ListHeader` portion of the node.
 *
 * @param value The value to be assigned to the node.
 * @param allocator_ The allocator used for memory allocation.
 * @return Pointer to the newly created node.
 */
#define list_node_ctor_(value, allocator_)                                      \
({                                                                              \
    auto value_ = (value);                                                      \
    Allocator allocator = (allocator_);                                         \
    ListHeader* node =                                                          \
        allocator.allocate(sizeof(ListHeader) + sizeof(value_));                \
    if (node)                                                                   \
    {                                                                           \
        *node = (ListHeader){};                                                 \
        *(typeof(value_) *)(node + 1) = value_;                                 \
        node->allocator = allocator;                                            \
    }                                                                           \
    node;                                                                       \
})

#endif // CMLIB_LIST_H_
