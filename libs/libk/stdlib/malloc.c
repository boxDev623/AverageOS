#include "stdlib.h"

extern void *kmalloc(size_t);
extern void *krealloc(void *, size_t);
extern void *kcalloc(size_t, size_t);
extern void kfree(void *);

void *malloc(size_t size)
{
	return kmalloc(size);
}

void *realloc(void *ptr, size_t size)
{
	return krealloc(ptr, size);
}

void *calloc(size_t nitems, size_t size)
{
	return kcalloc(nitems, size);
}

void free(void *ptr)
{
	kfree(ptr);
}