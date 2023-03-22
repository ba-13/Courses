#include "linked_list.h"

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

int main()
{
    DLLNode *tail;
    tail = en_queue(NULL, 1);
    DLLNode *head = tail;
    tail = en_queue(tail, 2);
    tail = en_queue(tail, 3);
    tail = en_queue(tail, 4);
    traverse_DLL_forward(head);
    head = de_queue(head);
    head = de_queue(head);
    traverse_DLL_forward(head);
    head = de_queue(head);
    head = de_queue(head);
    head = de_queue(head);
    printf("Trying printing:");
    traverse_DLL_forward(head);
    return 0;
}