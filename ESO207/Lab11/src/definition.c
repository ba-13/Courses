#include "definition.h"

node *create_node(int val, node *parent, int rank)
{
    node *tmp = (node *)malloc(sizeof(node));
    tmp->val = val;
    tmp->parent = parent;
    tmp->rank = rank;
    node *sentinel1 = NULL;
    node *sentinel2 = NULL;
    tmp->left = sentinel1;
    tmp->right = sentinel2;
    return tmp;
}

void promote(node *curr)
{
    curr->rank += 1;
}

void demote(node *curr)
{
    curr->rank -= 1;
}

node *parent(node *curr)
{
    return curr->parent;
}

node *sibling(node *curr)
{
    if (curr->parent->left == curr)
        return curr->parent->right;
    else
        return curr->parent->left;
}

node *grandparent(node *curr)
{
    if (parent(curr) == NULL)
    {
        return NULL;
    }
    node *p = parent(curr);
    if (parent(p) == NULL)
    {
        return NULL;
    }
    return parent(p);
}

node *uncle(node *curr)
{
    node *p = parent(curr);
    node *g = grandparent(curr);
    if (g != NULL)
    {
        if (g->left == p)
            return g->right;
        else
            return g->left;
    }
    else
        return NULL;
}