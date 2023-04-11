#include "rb_tree_adv.h"

int main()
{
    node *root = insert_node(NULL, 69);
    root = insert_node(root, 4);
    root = insert_node(root, 21);
    root = insert_node(root, 37);
    root = insert_node(root, 75);
    root = insert_node(root, 76);
    root = insert_node(root, 33);
    root = insert_node(root, 49);
    root = insert_node(root, 34);

    root = insert_node(root, 100);

    root = insert_node(root, 169);
    root = insert_node(root, 114);
    root = insert_node(root, 121);
    root = insert_node(root, 137);
    root = insert_node(root, 175);
    root = insert_node(root, 176);
    root = insert_node(root, 133);
    root = insert_node(root, 149);
    root = insert_node(root, 134);

    printLevelOrder(root);
    printInOrder(root);
    printf("\n\n");

    node **splitted = split(root, 100);
    node *S1 = splitted[0];
    node *I = splitted[1];
    node *S2 = splitted[2];

    printf("S1: ");
    printInOrder(S1);
    printf("\nI: %d\n", I->val);
    printf("S2: ");
    printInOrder(S2);

    return 0;
}