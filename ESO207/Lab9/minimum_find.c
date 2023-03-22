#include "avl_tree.h"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Arguments: [n] [fileName]\n");
        return 1;
    }
    int n = atoi(argv[1]);
    int arr[n];
    FILE *fp = fopen(argv[2], "r");
    for (int i = 0; i < n; i++)
    {
        int tmp = fscanf(fp, "%d ", &arr[i]);
        tmp++;
    }
    node *root = insert_node(NULL, arr[0]);
    for (int i = 1; i < n; i++)
        root = insert_node(root, arr[i]);

    printf("Minimum numbers:\n");
    for (int i = 0; i < n / 5; i++)
    {
        node *tmp = minimum_node(root);
        printf("%d ", tmp->val);
        delete_node(root, tmp);
    }
    printf("\n");

    if (n <= 100)
        printLevelOrder(root);
    printInOrder(root);
    printf("\n");

    return 0;
}
