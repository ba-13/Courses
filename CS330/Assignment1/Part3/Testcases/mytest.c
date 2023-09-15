#include "../mylib.h"

int main(int argc, char **argv)
{
    int num = 5;
    int *ptr1 = memalloc(num * sizeof(int));
    for (int i = 0; i < 5; i++)
    {
        *(ptr1 + i) = i;
    }
    int *ptr2 = memalloc(num * sizeof(int));
    int *ptr3 = memalloc(num * sizeof(int));
    int *ptr4 = memalloc(num * sizeof(int));
    int *ptr5 = memalloc(num * sizeof(int));
    int *ptr6 = memalloc(num * sizeof(int));
    print_free_list();
    memfree(ptr2);
    print_free_list();
    memfree(ptr4);
    print_free_list();
    memfree(ptr3);
    print_free_list();
}