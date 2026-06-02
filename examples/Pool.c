#include <stdio.h>
#include <stdlib.h>

#include "List.h"
#include "PoolResource.h"

typedef struct Node Node;
struct Node
{
    Node *left, *right, *parent;
    int val;
};

static void insert_node(Node* root, Node* node)
{
    if (node->val < root->val)
    {
        if (!root->left)
        {
            root->left = node;
            node->parent = root;
            return;
        }
        insert_node(root->left, node);
    }
    else if (node->val > root->val)
    {
        if (!root->right)
        {
            root->right = node;
            node->parent = root;
            return;
        }
        insert_node(root->right, node);
    }
}

static void print_tree(Node* root)
{
    if (root->left)
    {
        print_tree(root->left);
    }
    printf("%d\n", root->val);
    if (root->right)
    {
        print_tree(root->right);
    }
}

int main()
{
    constexpr size_t size = 10;
    constexpr size_t max = 100;

    Pool* pool = pool_ctor(size);

    Node* root = pool_allocate_type(pool, Node);
    *root = (Node) {.val = rand() % max};

    printf("Random:\n");
    for (size_t i = 0; i < size - 1; i++)
    {
        int val = rand() % max;
        printf("%d\n", val);

        Node* node = pool_allocate_type(pool, Node);
        *node = (Node) {.val = val};

        insert_node(root, node);
    }

    printf("\nSorted:\n");
    print_tree(root);

    PoolResource res = pool_to_resource(pool);
    List* list = list_ctor(&res);

    for (size_t i = 0; i < size; i++)
    {
        list_insert_after(list, list_end(list), i);
    }

    LIST_ITER(list, it)
    {
        printf("%d\n", *list_node_get_value(it, int));
    }

    pool_dtor(pool);
    // or
    // pool_resource_dtor(&res);
}
