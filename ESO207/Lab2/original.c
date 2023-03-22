#include <stdio.h>
#include <stdlib.h>

typedef struct DLLNode
{
    int val;
    struct DLLNode *prev, *next;
} DLLNode;

DLLNode *insert_DLLNode_atTail(DLLNode *tail, int val)
{
    DLLNode *newNode = (DLLNode *)malloc(sizeof(DLLNode));
    newNode->val = val;
    // newNode->prev = NULL;
    newNode->next = NULL;

    newNode->prev = tail;
    if (tail != NULL)
        tail->next = newNode;

    return newNode;
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

DLLNode *delete_DLLNode_atHead(DLLNode *head)
{
    DLLNode *newHead = head->next;
    if (newHead != NULL)
        newHead->prev = NULL;
    free(head);
    return newHead;
}

DLLNode *delete_DLLNode_atTail(DLLNode *tail)
{
    DLLNode *newTail = tail->prev;
    if (newTail != NULL)
        newTail->next = NULL;
    free(tail);
    return newTail;
}

int delete_DLLNode(DLLNode *head, int val)
{
    DLLNode *curr = head;
    while (curr != NULL)
    {
        if (curr->val == val)
        {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            free(curr);
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

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

int traverse_DLL_backward(DLLNode *tail)
{
    DLLNode *curr = tail;
    while (curr != NULL)
    {
        printf("%d->", curr->val);
        curr = curr->prev;
    }
    printf("\n");
    return 0;
}

/* Stack implementation */
DLLNode *push_stack(DLLNode *head, int val)
{
    head = insert_DLLNode_atHead(head, val);
    return head;
}

DLLNode *pop_stack(DLLNode *head)
{
    if (head == NULL)
    {
        printf("Stack is empty\n");
        return (DLLNode *)NULL;
    }
    head = delete_DLLNode_atHead(head);
    return head;
}

/* Queue Implementation */
DLLNode *en_queue(DLLNode *tail, int val)
{
    tail = insert_DLLNode_atTail(tail, val);
    return tail;
}

DLLNode *de_queue(DLLNode *head)
{
    if (head == NULL)
    {
        printf("Queue is empty\n");
        return (DLLNode *)NULL;
    }
    head = delete_DLLNode_atHead(head);
    return head;
}
