#ifndef UTILS_H
#define UTILS_H

#include "definition.h"
#include <stdio.h>

int height_(node *node);

void printCurrentLevel_(node *root, int level);

void printLevelOrder(node *root);

void printInOrder(node *root);

node *minimum_node(node *x);

node *insert_bst_tree(node *root, int val);

#endif