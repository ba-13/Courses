#include "linked_list.h"

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

int main()
{
    DLLNode *top = push_stack(NULL, 1);
    top = push_stack(top, 2);
    top = push_stack(top, 3);
    top = push_stack(top, 4);
    traverse_DLL_forward(top);
    top = pop_stack(top);
    top = pop_stack(top);
    traverse_DLL_forward(top);
    top = pop_stack(top);
    top = pop_stack(top);
    top = pop_stack(top);
    printf("Trying printing:");
    traverse_DLL_forward(top);
    return 0;
}