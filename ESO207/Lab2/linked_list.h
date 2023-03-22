#include <stdio.h>
#include <stdlib.h>

typedef struct DLLNode
{
    int val;
    struct DLLNode *prev, *next;
} DLLNode;

DLLNode *insert_DLLNode_atTail(DLLNode *tail, int val);

DLLNode *insert_DLLNode_atHead(DLLNode *head, int val);

DLLNode *delete_DLLNode_atHead(DLLNode *head);

DLLNode *delete_DLLNode_atTail(DLLNode *tail);

int delete_DLLNode(DLLNode *head, int val);

int traverse_DLL_forward(DLLNode *head);

int traverse_DLL_backward(DLLNode *tail);
