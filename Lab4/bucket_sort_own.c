#include "shared.h"

typedef struct DLLNode
{
    int val;
    struct DLLNode *prev, *next;
} DLLNode;

const int MAX = 100000000;

int find_maxima(int arr[], int n)
{
    int max = -MAX;
    for (int i = 0; i < n; i++)
    {
        if (max < arr[i])
            max = arr[i];
    }
    return max;
}

int find_minima(int arr[], int n)
{
    int min = MAX;
    for (int i = 0; i < n; i++)
    {
        if (min > arr[i])
            min = arr[i];
    }
    return min;
}

DLLNode *insert_DLLNode_atHead(DLLNode *head, int val)
{
    DLLNode *newNode = (DLLNode *)malloc(sizeof(DLLNode));
    newNode->val = val;
    newNode->prev = NULL;
    // newNode->next = NULL;

    newNode->next = head;
    if (head != NULL)
        head->prev = newNode;

    return newNode;
}

/// @brief Traverses the list, forward
/// @param head
/// @return 0
int traverse_DLL_forward(DLLNode *head)
{
    DLLNode *curr = head;
    while (curr != NULL)
    {
        printf("%d->", curr->val);
        curr = curr->next;
    }
    printf("\n");
    return 0;
}

void insertionSort_DLL(DLLNode **head, DLLNode *tail)
{
    if ((*head)->next == NULL)
        return; // no items

    DLLNode *sortedTill = (*head)->next; // sorted till but not including sortedTill
    while (sortedTill != tail)
    {
        DLLNode *exchangeAbove = sortedTill;
        DLLNode *exchangeBelow = sortedTill->prev;
        // printf("To be inserted: %d\n", exchangeAbove->val);
        while (exchangeAbove->prev != *head)
        {
            if (exchangeAbove->val < exchangeBelow->val)
            {
                exchangeBelow->prev->next = exchangeAbove;
                exchangeAbove->next->prev = exchangeBelow;
                exchangeBelow->next = exchangeAbove->next;
                exchangeAbove->prev = exchangeBelow->prev;
                exchangeBelow->prev = exchangeAbove;
                exchangeAbove->next = exchangeBelow;

                // swap pointers back to order
                DLLNode *tmp = exchangeAbove;
                exchangeAbove = exchangeBelow;
                exchangeBelow = tmp;

                exchangeAbove = exchangeBelow;
                exchangeBelow = exchangeBelow->prev;
            }
            else
                break;
        }
        if (exchangeAbove->val < exchangeBelow->val)
        {
            // exchangeBelow is head rn
            exchangeAbove->next->prev = exchangeBelow;
            exchangeBelow->next = exchangeAbove->next;
            exchangeAbove->prev = exchangeBelow->prev; // NULL
            exchangeBelow->prev = exchangeAbove;
            exchangeAbove->next = exchangeBelow;
            *head = exchangeAbove;
        }
        // printf("Inserted\n");
        sortedTill = sortedTill->next;
    }
}

void bucketSort(int arr[], int n)
{
    int maxima = find_maxima(arr, n) + 1; // so maxima comes under numBuckets-1 bucket
    int minima = find_minima(arr, n);
    int numBuckets = 10;
    float binSize = (maxima - minima) / 10;
    DLLNode *bucketsHeads[numBuckets];
    DLLNode *bucketsTails[numBuckets];

    for (int i = 0; i < numBuckets; i++)
    {
        bucketsHeads[i] = insert_DLLNode_atHead(NULL, MAX); // sentinel node
        bucketsTails[i] = bucketsHeads[i];
    }
    for (int i = 0; i < n; i++)
    {
        int idx = (arr[i] - minima) / binSize;
        if (idx == numBuckets)
            idx--;
        bucketsHeads[idx] = insert_DLLNode_atHead(bucketsHeads[idx], arr[i]); // insert at Head
    }
    for (int i = 0; i < numBuckets; i++)
    {
        insertionSort_DLL(&bucketsHeads[i], bucketsTails[i]);
    }
    int k = 0;
    for (int i = 0; i < numBuckets; i++)
    {
        DLLNode *curr = bucketsHeads[i];
        while (curr != bucketsTails[i]) // skipping the sentinel tail
        {
            arr[k] = curr->val;
            curr = curr->next;
            k++;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Arguments needed: [fileName] [n]\n");
        fprintf(stderr, "fileName - file path containing numbers to be sorted\n");
        fprintf(stderr, "n - #numbers in file\n");
        return 1;
    }

    FILE *filePointer;
    filePointer = fopen(argv[1], "r");
    int n = atoi(argv[2]);
    int numbers[n];
    for (int i = 0; i < n; i++)
    {
        int tmp = fscanf(filePointer, "%d ", &numbers[i]);
    }
    fclose(filePointer);

    gettimeofday(&t1, NULL);
    bucketSort(numbers, n);
    gettimeofday(&t2, NULL);

    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;    // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms

    printf("%f ms.\n", elapsedTime);

    filePointer = fopen("_sorted_numbers.txt", "w");
    for (int i = 0; i < n; i++)
        fprintf(filePointer, "%d ", numbers[i]);
    fclose(filePointer);

    // DLLNode *head = insert_DLLNode_atHead(NULL, 1000), *tail = NULL;
    // head = insert_DLLNode_atHead(head, 1);
    // tail = head;
    // head = insert_DLLNode_atHead(head, 34);
    // head = insert_DLLNode_atHead(head, 155);
    // head = insert_DLLNode_atHead(head, 15);
    // head = insert_DLLNode_atHead(head, 23);

    // traverse_DLL_forward(head);

    // insertionSort_DLL(&head, tail);

    // traverse_DLL_forward(head);

    return 0;
}
