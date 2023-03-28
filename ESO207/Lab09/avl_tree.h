#include <stdio.h>
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

node *minimum_node(node *x);

void printCurrentLevel_(node *root, int level);

int height_(node *node);

void printLevelOrder(node *root);

void printInOrder(node *root);

node *insert_bst_tree(node *root, int val);

void transplant_(node **root, node *u, node *v);

node *delete_bst_tree(node **root, node *z);

int right_rotate(node *v);

int left_rotate(node *v);

node *trinode(node *x, node *p, node *g);

node *search_node(node *root, int val);

node *insert_node(node *root, int val);

int check_valid(node *root);

void delete_rank_update_(node *t, node *s, node *p);

void delete_node(node *root, node *n);
