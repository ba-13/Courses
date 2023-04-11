#include "utils.h"

// helper function for calculating height of tree to print ONLY
int height_(struct node *node)
{
    if (node == NULL)
        return 0;
    else
    {
        /* compute the height of each subtree */
        int lheight = height_(node->left);
        int rheight = height_(node->right);

        /* use the larger one */
        if (lheight > rheight)
            return (lheight + 1);
        else
            return (rheight + 1);
    }
}

// printf the in order of a binary tree
void printInOrder(node *root)
{
    if (root == NULL || root->rank == 0)
        return;
    printInOrder(root->left);
    printf("%d ", root->val);
    printInOrder(root->right);
}

// prints the level order of a binary tree
void printLevelOrder(struct node *root)
{
    int h = height_(root);
    printf("height: %d\n", h);
    int i;
    for (i = 1; i <= h; i++)
    {
        printCurrentLevel_(root, i);
        printf("\n");
    }
}

// helper function to print a single level of a tree
void printCurrentLevel_(struct node *root, int level)
{
    if (root == NULL)
    {
        if (level == 1)
            printf("\t");
        return;
    }
    if (level == 1)
        if (root->rank == 0)
            printf("\t");
        else
            printf("(%d)%d\t", root->rank, root->val);
    else if (level > 1)
    {
        printCurrentLevel_(root->left, level - 1);
        printCurrentLevel_(root->right, level - 1);
    }
}

node *minimum_node(node *x)
{
    while (x->left->rank != 0)
        x = x->left;
    return x;
}

// used only for debugging
node *insert_bst_tree(node *root, int val)
{
    // don't pass a NULL to this function
    if (root->val > val)
    {
        if (root->left == NULL)
        {
            // insert val as left child
            node *tmp = create_node(val, root, 1);
            root->left = tmp;
            return tmp;
        }
        else
        {
            insert_bst_tree(root->left, val);
        }
    }
    else if (root->val < val)
    {
        if (root->right == NULL)
        {
            // insert val as right child
            node *tmp = create_node(val, root, 1);
            root->right = tmp;
            return tmp;
        }
        else
        {
            insert_bst_tree(root->right, val);
        }
    }
    return NULL;
}