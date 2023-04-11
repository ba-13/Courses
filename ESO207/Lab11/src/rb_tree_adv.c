#include "rb_tree_adv.h"

// assuming S1 contains items less than I, S2 contains items greater than I
node *join(node *S1, node *I, node *S2)
{
    //* check if either S1 or S2 are NULL
    if (S1 == NULL)
    {
        // S2 is greater than I
        I->parent = S2->parent;
        S2->parent = I;
        I->right = S2;
        I->left = create_node(0, I, 0);
        I->rank = S2->rank + 1;
        return I;
    }
    else if (S2 == NULL)
    {
        // S1 is less than I
        I->parent = S1->parent;
        S1->parent = I;
        I->left = S1;
        I->right = create_node(0, I, 0);
        I->rank = S1->rank + 1;
        return I;
    }

    node *upper_levels_ptr = NULL;
    //* if S1 and S2 are at least sentinels
    // S1->parent = NULL;
    // S2->parent = NULL;
    node *root;
    if (S1->rank >= S2->rank)
    {
        upper_levels_ptr = S1->parent;
        // S2 becomes a subtree of S1, I being it's parent
        root = S1;
        node *x = S1;
        while (x->rank != S2->rank)
            x = x->right;

        // insert I in place of x, with x as left child and S2 as right
        I->rank = S2->rank + 1;
        I->left = x;
        I->right = S2;
        I->parent = x->parent;
        S2->parent = I;

        if (x->parent != upper_levels_ptr)
        {
            int x_is_left_child = 0;
            if (x->parent->left == x)
                x_is_left_child = 1;

            if (x_is_left_child)
                x->parent->left = I;
            else
                x->parent->right = I;
            x->parent = I;
        }
        else
        {
            x->parent = I;
            return I;
        }
    }
    else
    {
        upper_levels_ptr = S2->parent;
        // S1 becomes a subtree of S2, I being it's parent
        root = S2;
        node *x = S2;
        while (x->rank != S1->rank)
            x = x->left;

        // insert I in place of x, with x as left child and S2 as right
        I->rank = S1->rank + 1;
        I->left = S1;
        I->right = x;
        I->parent = x->parent;
        S1->parent = I;

        if (x->parent != upper_levels_ptr)
        {
            int x_is_left_child = 0;
            if (x->parent->left == x)
                x_is_left_child = 1;

            if (x_is_left_child)
                x->parent->left = I;
            else
                x->parent->right = I;
            x->parent = I;
        }
        else
        {
            x->parent = I;
            return I;
        }
    }
    // correct rank like in insert
    node *tmp = I;
loop:;
    node *X = tmp;
    node *P = parent(tmp);
    if (P == upper_levels_ptr)
        return root;
    node *G = parent(P);
    if (G == upper_levels_ptr)
        return root;

    if (P->rank > X->rank || G->rank > X->rank)
        return root; // things are good

    node *U = uncle(X);
    if (U->rank == G->rank - 1)
    {
        int g_was_root = 0;
        if (G == root)
            g_was_root = 1;
        node *tmp_ = trinode(X, P, G);
        if (g_was_root)
            root = tmp_;
        return root;
    }
    else if (U->rank == P->rank)
    {
        promote(G);
        tmp = G;
        goto loop;
    }

    // should never reach here
    return NULL;
}

// returns an RB tree with root as i, if found
// S1 being left child of root
// S2 being right child of root
node **split(node *S, int i)
{
    node *I = search_node(S, i);
    if (I->rank == 0) // reached a sentinel, so can't split
        return NULL;

    node *S1 = I->left;
    node *S2 = I->right;
    node *x = I;
    if (x->parent == NULL)
        return x;
    node *P = parent(I);
    node *G;

    while (parent(x) != NULL)
    {
        printLevelOrder(S);
        P = parent(x);
        G = parent(P);
        if (G != NULL)
        {
            // P is to be placed in one of the subtrees
            // so x should become the child of G
            if (G->left == P)
                G->left = x;
            else
                G->right = x;
        }

        // also x should have G as parent
        x->parent = G;

        if (x == P->left)
        {
            S2 = join(S2, P, P->right);
            // printLevelOrder(S2);
        }
        else
        {
            S1 = join(P->left, P, S1);
            // printLevelOrder(S1);
        }

        if (G == NULL)
            break;
    }

    node **return_value = (node **)malloc(3 * sizeof(node *));
    return_value[0] = S1;
    return_value[1] = x;
    return_value[2] = S2;
    return return_value;
}
