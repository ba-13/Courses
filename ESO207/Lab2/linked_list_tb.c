#include "linked_list.h"

int main()
{
    DLLNode *head = insert_DLLNode_atTail(NULL, 1);
    DLLNode *tail = head;
    traverse_DLL_forward(head);
    tail = insert_DLLNode_atTail(tail, 2);
    tail = insert_DLLNode_atTail(tail, 3);
    tail = insert_DLLNode_atTail(tail, 4);
    head = insert_DLLNode_atHead(head, 5);
    traverse_DLL_forward(head);
    traverse_DLL_backward(tail);
    return 0;
}
