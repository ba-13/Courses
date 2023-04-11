#include "rb_tree.h"

int main()
{
    // don't care about ranks right now
    node *root = create_node(46, NULL, 1);
    node *tmp = insert_bst_tree(root, 67);
    node *tmp2 = insert_bst_tree(root, 28);
    insert_bst_tree(root, 5);
    insert_bst_tree(root, 2);
    insert_bst_tree(root, 25);
    insert_bst_tree(root, 35);
    insert_bst_tree(root, 31);
    insert_bst_tree(root, 41);
    insert_bst_tree(root, 49);

    insert_bst_tree(root, 54);

    printLevelOrder(root);
    printInOrder(root);
    printf("\n\n");

    printf("Right Rotation about %d:\n", tmp->val);
    right_rotate(tmp);

    printLevelOrder(root);
    printInOrder(root);
    printf("\n\n");

    printf("Left Rotation about %d:\n", tmp2->val);
    left_rotate(tmp2);

    printLevelOrder(root);
    printInOrder(root);
    printf("\n");

    return 0;
}