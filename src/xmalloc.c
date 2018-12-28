/*
 * src/xmalloc.c
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#include <stdlib.h>
#include <string.h>

#include <sic/xmalloc.h>
#include <sic/error.h>

void *
xmemset(void *ptr, int ch, size_t size)
{
	return memset(ptr, 0, size);
}

void *
xmalloc(size_t size)
{
	void *ptr;

	ptr = (void *) malloc(size);

	if (ptr == NULL) {
		error("Memory Exhausted");
	}

	xmemset(ptr, 0, size);

	return ptr;
}

void *
xrealloc(void *ptr, size_t size)
{
	void *tmp;

	if (ptr) {
		tmp = realloc(ptr, size);
	} else {
		tmp = malloc(size);
	}

	if (tmp == NULL) {
		error("Memory Exhausted");
	}

	return tmp;
}

void
xfree(void *ptr)
{
	if (ptr) {
		free(ptr);
	}
}
