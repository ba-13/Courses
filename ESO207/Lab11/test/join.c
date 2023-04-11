#include "rb_tree_adv.h"

int main()
{
    node *S1 = insert_node(NULL, 69);
    S1 = insert_node(S1, 4);
    S1 = insert_node(S1, 21);
    S1 = insert_node(S1, 37);
    S1 = insert_node(S1, 75);
    S1 = insert_node(S1, 76);
    S1 = insert_node(S1, 33);
    S1 = insert_node(S1, 49);
    S1 = insert_node(S1, 34);
    printLevelOrder(S1);
    printInOrder(S1);
    printf("\n\n");

    node *S2 = insert_node(NULL, 169);
    S2 = insert_node(S2, 114);
    S2 = insert_node(S2, 121);
    S2 = insert_node(S2, 137);
    S2 = insert_node(S2, 175);
    S2 = insert_node(S2, 176);
    S2 = insert_node(S2, 133);
    S2 = insert_node(S2, 149);
    S2 = insert_node(S2, 134);
    printLevelOrder(S2);
    printInOrder(S2);
    printf("\n\n");

    node *I = insert_node(NULL, 100);
    node *root = join(S1, I, S2);
    printLevelOrder(root);
    printInOrder(root);
    printf("\n\n");
    return 0;
}