#include "linked_list.h"

/// @brief Inserts a new node at the end of list
/// @param tail
/// @param val
/// @return new tail
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

/// @brief Inserts a new node at the beginning of list
/// @param head
/// @param val
/// @return new head
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

/// @brief Deletes the head node
/// @param head
/// @return new head
DLLNode *delete_DLLNode_atHead(DLLNode *head)
{
    DLLNode *newHead = head->next;
    if (newHead != NULL)
        newHead->prev = NULL;
    free(head);
    return newHead;
}

/// @brief Deletes the tail node
/// @param tail
/// @return new tail
DLLNode *delete_DLLNode_atTail(DLLNode *tail)
{
    DLLNode *newTail = tail->prev;
    if (newTail != NULL)
        newTail->next = NULL;
    free(tail);
    return newTail;
}

/// @brief Deletes a node from the list
/// @param head
/// @param val
/// @return 1 if successful, 0 if not found
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

/// @brief Traverses the list, forward
/// @param head
/// @return 0
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