/*
 * include/sic/xmalloc.h
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#ifndef _XMALLOC_H
#define _XMALLOC_H

void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);
void *xmemset(void *ptr, int ch, size_t size);
void xfree(void *ptr);

#endif
