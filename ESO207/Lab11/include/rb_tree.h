#ifndef RB_TREE_H
#define RB_TREE_H

#include "definition.h"
#include "utils.h"

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

#endif