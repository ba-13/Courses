#include "avl_tree.h"

node *create_node(int val, node *parent, int rank)
{
    node *tmp = (node *)malloc(sizeof(node));
    tmp->val = val;
    tmp->parent = parent;
    tmp->rank = rank;
    // node *sentinel1 = (node *)malloc(sizeof(node));
    // node *sentinel2 = (node *)malloc(sizeof(node));
    node *sentinel1 = NULL;
    node *sentinel2 = NULL;
    tmp->left = sentinel1;
    tmp->right = sentinel2;
    return tmp;
}

void promote(node *curr)
{
    curr->rank += 1;
}

void demote(node *curr)
{
    curr->rank -= 1;
}

node *parent(node *curr)
{
    return curr->parent;
}

node *sibling(node *curr)
{
    if (curr->parent->left == curr)
        return curr->parent->right;
    else
        return curr->parent->left;
}

node *grandparent(node *curr)
{
    if (parent(curr) == NULL)
    {
        return NULL;
    }
    node *p = parent(curr);
    if (parent(p) == NULL)
    {
        return NULL;
    }
    return parent(p);
}

node *uncle(node *curr)
{
    node *p = parent(curr);
    node *g = grandparent(curr);
    if (g != NULL)
    {
        if (g->left == p)
            return g->right;
        else
            return g->left;
    }
    else
        return NULL;
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
        printf("(%d)%d\t", root->rank, root->val);
    else if (level > 1)
    {
        printCurrentLevel_(root->left, level - 1);
        printCurrentLevel_(root->right, level - 1);
    }
}

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

// transplant v at u's place
void transplant_(node **root, node *u, node *v)
{
    if (u->parent == NULL)
        *root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != NULL)
        v->parent = u->parent;
}

// delete function of bst without balancing
// returns the node transplanted at its position
node *delete_bst_tree(node **root, node *z)
{
    node *t;
    if (z->left->rank == 0 && z->right->rank == 0)
    {
        t = z->left;
        // has rank 0, z would have 1. r-2 scenario created
        transplant_(root, z, t);
        return t;
    }
    else if (z->left->rank == 0)
    {
        t = z->right;
        transplant_(root, z, z->right);
        return t;
    }
    else if (z->right->rank == 0)
    {
        t = z->left;
        transplant_(root, z, t);
        return t;
    }
    else
    {
        node *y = minimum_node(z);
        if (y->parent == z)
        {
            transplant_(root, z, y);
            z->left->parent = y;
            return y;
        }
        else
        {
            transplant_(root, y, y->right);
            transplant_(root, z, y);
            y->right = z->right;
            y->left = z->left;
            z->right->parent = z->left->parent = y;
            return y;
        }
    }
}

int right_rotate(node *v)
{
    // case 0:
    // v has no left child
    if (v->left == NULL)
    {
        return 0;
    }

    node *u = v->left;
    u->right->parent = v;
    v->left = u->right;

    // case 1:
    // v has no parent i.e. root, and left child u
    if (v->parent == NULL)
    {
        u->parent = NULL;
    }
    // case 2:
    // v has parent x, and left child u
    else
    {
        node *x = parent(v);
        u->parent = x;

        int v_is_left_child = 0;
        if (x->left == v)
            v_is_left_child = 1;

        if (v_is_left_child)
            x->left = u;
        else
            x->right = u;
    }

    v->parent = u;
    u->right = v;

    return 1;
}

int left_rotate(node *v)
{
    // case 0:
    // v has no right child
    if (v->right == NULL)
    {
        return 0;
    }

    node *u = v->right;
    u->left->parent = v;
    v->right = u->left;

    // case 1:
    // v has no parent i.e. root, and right child u
    if (v->parent == NULL)
    {
        u->parent = NULL;
    }
    // case 2:
    // v has parent x, and right child u
    else
    {
        node *x = parent(v);
        u->parent = x;

        int v_is_left_child = 0;
        if (x->left == v)
            v_is_left_child = 1;

        if (v_is_left_child)
            x->left = u;
        else
            x->right = u;
    }

    v->parent = u;
    u->left = v;

    return 1;
}

node *trinode(node *x, node *p, node *g)
{
    int mode = 0;
    if (g->val > x->val && x->val > p->val)
        mode = 1;
    else if (g->val > p->val && p->val > x->val)
        mode = 2;
    else if (p->val > x->val && x->val > g->val)
        mode = 3;
    else if (x->val > p->val && p->val > g->val)
        mode = 4;

    switch (mode)
    {
    case 1:
        // left p, right g
        left_rotate(p);
        right_rotate(g);
        return x;
    case 2:
        // right g
        right_rotate(g);
        return p;
    case 3:
        // left g, right p
        right_rotate(p);
        left_rotate(g);
        return x;
    case 4:
        left_rotate(g);
        return p;

    default:
        printf("[ERR]: Didn't come under any mode\n");
        break;
    }
    return NULL;
}

node *search_node(node *root, int val)
{
    node *curr = root;
    while (curr->rank != 0)
    { // rank = 0 are sentinel nodes
        int curr_val = curr->val;
        if (curr_val < val)
            curr = curr->right;
        else if (curr_val > val)
            curr = curr->left;
        else
            return curr;
    }
    return curr;
}

node *insert_node(node *root, int val)
{
    node *curr = root;
    // if tree is empty
    if (curr == NULL)
    {
        node *tmp = create_node(val, NULL, 1);
        node *sentinel1 = create_node(0, tmp, 0);
        node *sentinel2 = create_node(0, tmp, 0);
        tmp->left = sentinel1;
        tmp->right = sentinel2;
        return tmp;
    }

    while (curr->rank != 0)
    {
        // already exists
        if (curr->val == val)
            return root;
        else if (curr->val > val)
            curr = curr->left;
        else
            curr = curr->right;
    }
    // loop exits with a sentinel node

    // insert new node, here curr is sentinel itself
    node *tmp = create_node(val, curr->parent, 1);
    int sentinel_is_left_child = 0;
    if (curr->parent->left == curr)
        sentinel_is_left_child = 1;

    if (sentinel_is_left_child)
        curr->parent->left = tmp;
    else
        curr->parent->right = tmp;
    node *sentinel = create_node(0, tmp, 0);
    curr->parent = tmp;
    tmp->left = curr;
    tmp->right = sentinel;

    // check rank validity
loop:;
    node *x = tmp;
    node *p = parent(x);
    if (p == NULL)
        return root;
    node *g = parent(p);
    if (g == NULL)
        return p;

    int rank = x->rank;

    if (p->rank > rank || g->rank > rank)
        return root; // everything is fine

    node *u = uncle(x);
    // uncle would at least be a sentinel
    if (u->rank == g->rank - 1)
    {
        int g_was_root = 0;
        if (g == root)
            g_was_root = 1;
        node *tmp_ = trinode(x, p, g);
        if (g_was_root)
            root = tmp_;
        return root;
    }
    else if (u->rank == p->rank)
    {
        promote(g);
        tmp = g;
        goto loop;
    }
    return NULL;
}

// helper function that handles ranks during case with rank(s) = r-1
void delete_rank_update_(node *t, node *s, node *p)
{
    int mode = 0;
    if (p->val > t->val && t->val > s->val)
        mode = 1;
    else if (p->val > s->val && s->val > t->val)
        mode = 2;
    else if (s->val > t->val && t->val > p->val)
        mode = 3;
    else if (t->val > s->val && s->val > p->val)
        mode = 4;

    switch (mode)
    {
    case 1:
        demote(p);
        promote(t);
        break;
    case 2:
        demote(p);
        promote(s);
        break;
    case 3:
        demote(p);
        promote(t);
        break;
    case 4:
        demote(p);
        promote(s);
        break;
    default:
        printf("[ERR]: Didn't come under any mode during rank update\n");
        break;
    }
}

int check_valid(node *root)
{
    if (root->left->rank == 0 || root->right->rank == 0)
    {
        if (root->rank == 1)
            return 1;
        else
            return 0;
    }
    if (root->parent != NULL)
    {
        if (root->parent->rank == root->rank || root->parent->rank == root->rank + 1)
            ;
        else
        {
            return 0;
        }
    }
    return check_valid(root->left) || check_valid(root->right);
}

void delete_node(node *root, node *n)
{
    node *x = delete_bst_tree(&root, n);
    // printf("[DBG]: Within delete node\n");
    // printLevelOrder(root);
case1:;
    node *p = parent(x);
    node *s = sibling(x);
    if (p->rank - x->rank < 2)
        return; // good case
    int r = p->rank;
    if (r - x->rank > 2)
    {
        printf("\nWeirdly enough, rank between transplanted and original parent is greater than 2\n");
        return;
    }

    int rank_s = s->rank;
    int rank_sl = s->left->rank;
    int rank_sr = s->right->rank;
    // Case 1:
    // s has rank (r-1), and has children of (r-2)s
    if (rank_s == r - 1 && rank_sl == r - 2 && rank_sr == r - 2)
    {
        // printf("[DBG]: Case1\n");
        demote(p);
        x = p;
        goto case1;
    }

    // Case 2:
    // s has rank (r-1), and has children of (r-1)s
    if (rank_s == r - 1 && rank_sl == r - 1 && rank_sr == r - 1)
    {
        // printf("[DBG]: Case2\n");
        // t is farthest from x
        node *t;
        int x_is_left_child = 0;
        if (x->parent->left == x)
            x_is_left_child = 1;
        if (x_is_left_child)
        {
            t = s->right;
        }
        else
        {
            t = s->left;
        }

        delete_rank_update_(t, s, p);
        trinode(t, s, p);
    }
    // Case 3:
    // s has rank (r-1), and it's child c has rank (r-1)
    else if (rank_s == r - 1 && (rank_sl == r - 1 || rank_sr == r - 1))
    {
        // printf("[DBG]: Case3\n");
        // c is node with rank = r-1
        node *c;
        if (rank_sl == r - 1)
            c = s->left;
        else
            c = s->right;

        delete_rank_update_(c, s, p);
        trinode(c, s, p);
    }
    // Case 4:
    // s has rank r
    else if (rank_s == r)
    {
        // printf("[DBG]: Case4\n");
        node *t;
        int x_is_left_child = 0;
        if (x->parent->left == x)
            x_is_left_child = 1;
        if (x_is_left_child)
        {
            t = s->right;
        }
        else
        {
            t = s->left;
        }
        trinode(t, s, p);
    }
}
