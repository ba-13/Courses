#ifndef MYLIB_H
#define MYLIB_H
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void *memalloc(unsigned long size);

int memfree(void *ptr);

void print_free_list();

#endif
