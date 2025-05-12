#ifndef CMLIB_LIST_H_
#define CMLIB_LIST_H_

#include <stdlib.h>
#include <string.h>

#include "../Logger/Logger.h"
#include "../Allocator/Allocator.h"

typedef struct ListHeader ListHeader;
struct ListHeader
{
    ListHeader* prev;
    ListHeader* next;
    Allocator allocator;
};

extern Allocator Current_list_allocator;

/**
 * @brief Set list allocator
 *
 * @param [in] allocator
 */
INLINE void list_set_allocator(Allocator allocator);

/**
 * @brief Reset default list allocator(malloc)
 */
INLINE void list_reset_allocator();

INLINE void list_dtor(ListHeader* node);

INLINE ListHeader* list_start(ListHeader* node);

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

INLINE ListHeader* list_end(ListHeader* node);

#define list_get_value(node) ((void*)((node) + 1))

#define list_node_ctor(value) (list_node_ctor_(value, Current_list_allocator))

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
