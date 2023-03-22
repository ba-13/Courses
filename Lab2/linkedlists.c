#include <stdio.h>
#include <stdlib.h>

typedef struct DLLNode
{
    int val;
    struct DLLNode *prev, *next;
} DLLNode;

DLLNode *create_DLLNode(int val)
{
    DLLNode *newNode = (DLLNode *)malloc(sizeof(DLLNode));
    newNode->val = val;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void insert_DLLNode_atTail(DLLNode **tail, int val)
{
    DLLNode *newNode = create_DLLNode(val);

    newNode->prev = (*tail);
    if ((*tail) != NULL)
        (*tail)->next = newNode;
    *tail = newNode;
}

void insert_DLLNode_atHead(DLLNode **head, int val)
{
    DLLNode *newNode = create_DLLNode(val);

    newNode->next = (*head);
    if ((*head) != NULL)
        (*head)->prev = newNode;
    *head = newNode;
}

void delete_DLLNode_atHead(DLLNode **head)
{
    DLLNode *newHead = (*head)->next;
    if (newHead != NULL)
        newHead->prev = NULL;
    free(*head);
    *head = newHead;
}

void delete_DLLNode_atTail(DLLNode **tail)
{
    DLLNode *newTail = (*tail)->prev;
    if (newTail != NULL)
        newTail->next = NULL;
    free(*tail);
    *tail = newTail;
}

int delete_DLLNode(DLLNode **head, int val)
{
    DLLNode *curr = *head;
    while (curr != NULL)
    {
        if (curr->val == val)
        {
            if (curr == *head)
                *head = curr->next;
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            free(curr);
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

void traverse_DLL_forward(DLLNode *head)
{
    DLLNode *curr = head;
    while (curr != NULL)
    {
        printf("%d->", curr->val);
        curr = curr->next;
    }
    printf("\n");
}

void traverse_DLL_backward(DLLNode *tail)
{
    DLLNode *curr = tail;
    while (curr != NULL)
    {
        printf("%d->", curr->val);
        curr = curr->prev;
    }
    printf("\n");
}

/* Stack implementation */
void push_stack(DLLNode **head, int val)
{
    insert_DLLNode_atHead(head, val);
}

void pop_stack(DLLNode **head)
{
    if ((*head) == NULL)
    {
        printf("Stack is empty\n");
        return;
    }
    delete_DLLNode_atHead(head);
}

/* Queue Implementation */
void en_queue(DLLNode **head, DLLNode **tail, int val)
{
    insert_DLLNode_atTail(tail, val);
    // head included for the case when queue is empty
    if ((*head) == NULL)
    {
        *head = *tail;
    }
}

void de_queue(DLLNode **head)
{
    if ((*head) == NULL)
    {
        printf("Queue is empty\n");
        return;
    }
    delete_DLLNode_atHead(head);
}

int main()
{
    printf("Queue test:\n");
    DLLNode *head = NULL, *tail = NULL;
    en_queue(&head, &tail, 1);
    en_queue(&head, &tail, 2);
    en_queue(&head, &tail, 3);
    en_queue(&head, &tail, 3);
    en_queue(&head, &tail, 3);
    traverse_DLL_forward(head);
    de_queue(&head);
    traverse_DLL_forward(head);

    printf("Stack test:\n");
    DLLNode *top = NULL;
    push_stack(&top, 1);
    push_stack(&top, 2);
    push_stack(&top, 2);
    push_stack(&top, 3);
    traverse_DLL_forward(top);
    pop_stack(&top);
    traverse_DLL_forward(top);
    return 0;
}