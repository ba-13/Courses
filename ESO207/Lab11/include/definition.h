#ifndef DEFINITION_H
#define DEFINITION_H

#include <stdlib.h>

typedef struct node
{
    int val;
    int rank;
    struct node *parent;
    struct node *left, *right;
} node;

node *create_node(int val, node *parent, int rank);

void promote(node *curr);

void demote(node *curr);

node *parent(node *curr);

node *sibling(node *curr);

node *grandparent(node *curr);

node *uncle(node *curr);

#endif