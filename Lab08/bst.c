#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int val;
    struct node *left, *right, *parent;
} node;

node *createNode(int val, node *parent)
{
    node *tmp = (node *)malloc(sizeof(node));
    tmp->val = val;
    tmp->parent = parent;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

void transplant_(node *root, node *u, node *v)
{
    if (u->parent == NULL)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != NULL)
        v->parent = u->parent;
}

void insert_tree_(node *root, int val)
{
    // don't pass a NULL to this function
    if (root->val > val)
    {
        if (root->left == NULL)
        {
            // insert val as left child
            node *tmp = createNode(val, root);
            root->left = tmp;
        }
        else
        {
            insert_tree_(root->left, val);
        }
    }
    else if (root->val < val)
    {
        if (root->right == NULL)
        {
            // insert val as right child
            node *tmp = createNode(val, root);
            root->right = tmp;
        }
        else
        {
            insert_tree_(root->right, val);
        }
    }
}

node *min(node *x)
{
    while (x->left != NULL)
        x = x->left;
    return x;
}

node *search_tree(node *curr, int val)
{
    if (curr == NULL || val == curr->val)
        return curr;
    if (val < curr->val)
        return search_tree(curr->left, val);
    else
        return search_tree(curr->right, val);
}

node *create_tree(int *arr, int n)
{
    // makes the first element as the root
    if (n < 1)
        return NULL;
    node *root = createNode(arr[0], NULL);
    for (int i = 1; i < n; i++)
    {
        insert_tree_(root, arr[i]);
    }
    return root;
}

int delete_tree_(node *root, node *z)
{
    if (z->left == NULL && z->right == NULL)
    {
        transplant_(root, z, NULL);
    }
    else if (z->left == NULL)
    {
        transplant_(root, z, z->right);
    }
    else if (z->right == NULL)
    {
        transplant_(root, z, z->left);
    }
    else
    {
        node *y = min(z); // obtain successor
        if (y->parent == z)
        {
            transplant_(root, z, y);
            z->left->parent = y;
        }
        else
        {
            transplant_(root, y, y->right);
            transplant_(root, z, y);
            y->right = z->right;
            y->left = z->left;
            z->right->parent = z->left->parent = y;
        }
    }
}

void inorder(node *curr)
{
    if (curr == NULL)
        return;
    inorder(curr->left);
    printf("%d ", curr->val);
    inorder(curr->right);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Argument: [n] [fileName]");
        return 1;
    }
    int n = atoi(argv[1]), tmp;
    int arr[n];
    FILE *fp = fopen(argv[2], "r");
    for (int i = 0; i < n; i++)
    {
        tmp = fscanf(fp, "%d ", &arr[i]);
    }
    node *root = create_tree(arr, n);
    if (n < 100)
    {
        inorder(root);
        printf("\n");
    }
    for (int i = 0; i < n / 5; i++)
    {
        node *minima = min(root);
        printf("%d ", minima->val);
        delete_tree_(root, minima);
    }

#if 0

    node *found = search_tree(root, 12);
    if (found != NULL)
    {
        delete_tree_(root, found);
    }
    inorder(root);
    printf("\n");
#endif

    return 0;
}
