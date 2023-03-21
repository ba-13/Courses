#include "shared.h"

const int MAX = 100000000;

// Define a structure for a node in a doubly linked list
struct Node
{
    int data;
    struct Node *next;
    struct Node *prev;
};

// Function to insert a node at the end of a doubly linked list
void insert_node(struct Node **head, int data)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (*head == NULL)
    {
        *head = new_node;
        return;
    }

    struct Node *last = *head;
    while (last->next != NULL)
    {
        last = last->next;
    }

    last->next = new_node;
    new_node->prev = last;
}

// Function to sort the elements in a doubly linked list using bucket sort
void bucketSort(int arr[], int n, int max)
{
    // Create an array of pointers to linked lists, where each linked list represents a bucket
    struct Node **buckets = (struct Node **)malloc(max * sizeof(struct Node));
    for (int i = 0; i < max; i++)
    {
        buckets[i] = NULL;
    }

    // Distribute the elements in the input array into the linked lists (buckets)
    for (int i = 0; i < n; i++)
    {
        insert_node(&buckets[arr[i]], arr[i]);
    }

    // Collect the elements from the linked lists (buckets) and store them back in the input array
    int index = 0;
    for (int i = 0; i < max; i++)
    {
        struct Node *head = buckets[i];
        while (head != NULL)
        {
            arr[index++] = head->data;
            head = head->next;
        }
    }
}

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

// Main function to test the bucket sort implementation
int main(int argc, char **argv)
{
    int n = 10;
    int arr[] = {4, 6, 7, 8, 9, 8, 6, 0, 1, 6};
    int max = find_maxima(arr, n) + 1;
    bucketSort(arr, n, max);
    displayArr(arr, n);
    return 0;
}
