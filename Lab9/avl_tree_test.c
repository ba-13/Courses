#include "avl_tree.h"

int main(int argc, char **argv)
{
    node *root = insert_node(NULL, 69);
    root = insert_node(root, 4);
    root = insert_node(root, 21);
    printLevelOrder(root);
    root = insert_node(root, 37);
    printLevelOrder(root);
    root = insert_node(root, 75);
    printLevelOrder(root);
    root = insert_node(root, 76);
    printLevelOrder(root);
    root = insert_node(root, 33);
    printLevelOrder(root);
    root = insert_node(root, 49);
    printLevelOrder(root);
    root = insert_node(root, 34);
    printLevelOrder(root);
    printInOrder(root);
    printf("\n");

    printf("Starting deletion:\n");
    node *delNode = search_node(root, 33);
    delete_node(root, delNode);
    printLevelOrder(root);

    printInOrder(root);
    printf("\n");

    return 0;
}
