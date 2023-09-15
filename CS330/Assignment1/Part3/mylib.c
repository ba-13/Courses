#include "mylib.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define MIN_NUM_PAGES 0x100
#define PAGE_SIZE 0x1000
#define MIN_BYTES 0x8
#define MAP_ANONYMOUS 0x20

typedef struct free_list
{
	unsigned long size;
	struct free_list *next;
	struct free_list *prev;
} free_list;

// function declarations
free_list *get_new_memory__(int num_pages);
int init_free_list_head__();
void *head_size_decision__(free_list *head, unsigned long size);

static free_list *free_list_head__ = NULL;
static char VERBOSE = 0; // level 0, 1, 2, 3 for different levels of debugging

free_list *get_new_memory__(int num_pages)
{
	free_list *ret = mmap(NULL, num_pages * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (ret)
	{
		if (VERBOSE > 0)
			printf("[DBG]: Allocated %d new pages\n", num_pages);
		ret->size = num_pages * PAGE_SIZE;
		ret->prev = NULL;
		ret->next = NULL;
	}
	else
		fprintf(stderr, "[ERR]: mmap failed %d\n", errno);
	return ret;
}

void remove_node(free_list *pt)
{
	if (pt->next)
		pt->next->prev = pt->prev;
	if (pt->prev)
		pt->prev->next = pt->next;
}

void replace_node(free_list *pt, free_list *new_pt)
{
	// printf("%p %p\n", pt, new_pt);
	new_pt->prev = pt->prev;
	new_pt->next = pt->next;
	if (pt->next)
		pt->next->prev = new_pt;
	if (pt->prev)
		pt->prev->next = new_pt;
}

void add_node_after(free_list *head, free_list *new_pt)
{
	new_pt->prev = head;
	new_pt->next = head->next;
	if (head->next)
		head->next->prev = new_pt;
	head->next = new_pt;
}

int init_free_list_head__()
{
	if (VERBOSE > 2)
		printf("[DBG]: Init free_list_head__ called\n");
	free_list_head__ = get_new_memory__(MIN_NUM_PAGES);
	if (!free_list_head__)
		return 1;
	free_list_head__->size = 0;	   // don't allow this to get alloted
	free_list_head__->prev = NULL; // head takes 24 bytes
	free_list *first_node = free_list_head__ + 1;
	first_node->size = MIN_NUM_PAGES * PAGE_SIZE - sizeof(free_list);
	first_node->next = NULL;
	first_node->prev = free_list_head__;
	free_list_head__->next = first_node;
	return 0;
}

void *head_size_decision__(free_list *head, unsigned long size)
{
	void *ret = head;
	// found free chunk with more than enough space
	if (head->size >= size + sizeof(free_list))
	{
		// split chunks
		if (VERBOSE > 1)
			printf("[DBG]: [%p] More than enough\t", head);
		free_list *tmp = (void *)head + size;
		// replace_node(head, tmp);
		remove_node(head);
		add_node_after(free_list_head__, tmp);
		tmp->size = head->size - size;
		head->size = size;
		ret = ret + sizeof(free_list_head__->size);
	}
	// found free chunk with just enough space
	else if (head->size >= size)
	{
		// take the whole chunk
		if (VERBOSE > 1)
			printf("[DBG]: [%p] Found just enough\t", head);
		remove_node(head);
		ret = ret + sizeof(free_list_head__->size); // return from free space address
	}
	// found free chunk with not enough space
	else
	{
		if (VERBOSE > 1)
			printf("[ERR]: [%p] Not enough space\t", head);
		ret = NULL;
	}
	if (VERBOSE > 1)
		printf("head->size=%ld size=%ld\n", head->size, size);
	return ret;
}

void print_free_list()
{
	free_list *head = free_list_head__;
	printf("[DBG]: Will print the free_list from %p:\n", head);
	while (head)
	{
		printf("\t[%p] size=%lX\t%ld\n", head, head->size, head->size);
		head = head->next;
	}
}

void *memalloc(unsigned long size)
{
	if (VERBOSE > 2)
		printf("[DBG]: memalloc() called\n");
	unsigned int buffer = size % 8;
	buffer = buffer ? 8 - buffer : buffer;
	size = sizeof(free_list_head__->size) + size + buffer; // guaranteed to be multiple of 8
	if (size < 24)
		size = 24; // makes sure that every chunk can at least have size, prev, next!
	if (VERBOSE > 1)
		printf("[DBG]: Needed %ld bytes\n", size);
	if (!free_list_head__ && init_free_list_head__())
		return NULL; // called when mmap fails

	free_list *head = free_list_head__;
	free_list *prev_head = head->prev;
	while (head)
	{
		void *decided_addr = head_size_decision__(head, size);
		if (decided_addr)
			return decided_addr;
		prev_head = head;
		head = head->next;
	}
	// reaches here if found no enough sized chunk
	int num_pages = size / (MIN_NUM_PAGES * PAGE_SIZE);
	num_pages = size % (MIN_NUM_PAGES * PAGE_SIZE) ? num_pages + 1 : num_pages; // if fractional
	if (VERBOSE > 2)
		printf("[DBG]: Didn't find any free chunk, requesting %d pages\n", num_pages);

	free_list *new_mem = get_new_memory__(num_pages * MIN_NUM_PAGES);
	if (!new_mem)
		return NULL;
	// link this new_mem to LL
	add_node_after(prev_head, new_mem); // append to linked list
	void *decided_addr = head_size_decision__(new_mem, size);
	if (!decided_addr)
		fprintf(stderr, "[ERR]: Didn't find enough space even after new allocation!\n"); // should never run
	return decided_addr;
}

int memfree(void *ptr)
{
	if (VERBOSE > 2)
		printf("[DBG]: memfree() called\n");
	free_list *base_ptr = (free_list *)(ptr - sizeof(free_list_head__->size));
	int size = base_ptr->size;
	if (VERBOSE > 1)
		printf("[DBG]: Found %d size\n", size);
	free_list *head = free_list_head__;
	if (!head)
	{
		fprintf(stderr, "[ERR]: Head not found\n");
		return -1;
	}
	free_list *prev = NULL, *next = NULL;
	if (VERBOSE > 2)
		printf("[DBG]: Request to free %p\n", ptr);
	while (head)
	{
		int is_prev_chunk = (void *)base_ptr == head->size + (void *)head;
		int is_next_chunk = (void *)base_ptr + base_ptr->size == (void *)head;
		if (is_prev_chunk)
			prev = head;
		if (is_next_chunk)
			next = head;
		head = head->next;
	}
	head = free_list_head__; // reset head
	if (!prev)
	{
		// when the freed new node needs insertion at start
		if (VERBOSE > 1)
			printf("[DBG]: Found no prev\n");
		add_node_after(head, base_ptr);
	}
	else
	{
		if (!next)
		{
			if (VERBOSE > 1)
				printf("[DBG]: Found prev, Found no next\n");
			// coalesce prev and base_ptr
			prev->size = prev->size + base_ptr->size;
		}
		else
		{
			if (VERBOSE > 1)
				printf("[DBG]: Found prev, Found next\n");
			// coalesce prev, base_ptr and next
			prev->size = prev->size + base_ptr->size + next->size;
			// remove next from linked list
			remove_node(next);
		}
		// remove prev from linked list
		remove_node(prev);
		// add at start of linked list
		add_node_after(head, prev);
	}
	return 0;
}
