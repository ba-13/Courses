#include <sys/mman.h>
#include <stdio.h>
#include "../mylib.h"

#define PAGE_SIZE 0x1000
#define MIN_NUM_PAGES 0x100

int main()
{
    int num_pages = 14;
    int page_size = 0x1000;
    free_list *first_pages = mmap(NULL, MIN_NUM_PAGES * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    printf("Size of ulong: %ld\n", sizeof(unsigned long));
    printf("Size of free_list: %ld\n", sizeof(free_list));
    printf("Initial start memory allocated: %p\n", first_pages);
    first_pages->size = MIN_NUM_PAGES * PAGE_SIZE;
    printf("%ld\n", first_pages->size);
    first_pages->next = (free_list *)&num_pages;
    first_pages->prev = (free_list *)&page_size;
    for (int i = 0; i < 0x18; i++)
    {
        printf("%p %X\n", (void *)first_pages + i, *((char *)first_pages + i));
    }
    first_pages += 1;
    printf("Now start memory allocated: %p\n", first_pages);
    return 0;
}