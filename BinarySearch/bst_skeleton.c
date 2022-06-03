#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

Tree_Node *create_node(char data)
{
    Tree_Node *temp = (Tree_Node *)malloc(sizeof(Tree_Node));
    temp->data = data;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

void tree_insert(Tree_Node **root, char data)
{
    if (*root == NULL)
    {
        *root = create_node(data);
        return;
    }
    if (data < (*root)->data)
    {
        tree_insert(&(*root)->left, data);
    }
    else
    {
        tree_insert(&(*root)->right, data);
    }
}

Tree_Node *create_bst(char data[])
{
    int i = 0;
    Tree_Node *root = NULL;
    tree_insert(&root, data[i++]);
    while (data[i] != '\0')
    {
        tree_insert(&root, data[i++]);
    }
    return root;
}

Tree_Node *tree_search(Tree_Node *root, char data)
{
    if (root == NULL || root->data == data)
    {
        return root;
    }
    else
    {
        if (data < root->data)
        {
            return tree_search(root->left, data);
        }
        else
        {
            return tree_search(root->right, data);
        }
    }
    return NULL;
}

void tree_print_sorted(Tree_Node *root)
{
    if (root == NULL)
    {
        return;
    }
    tree_print_sorted(root->left);
    printf("%c", root->data);
    tree_print_sorted(root->right);
}

void tree_delete(Tree_Node *root)
{
    if (root == NULL)
    {
        return;
    }
    tree_delete(root->left);
    tree_delete(root->right);
    free(root);
}
