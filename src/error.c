/*
 * src/error.c
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <sic/error.h>

void
error(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);

	fprintf(stderr, "Error: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");

	va_end(args);

	exit(2);	/* conventional value for failed execution */
}
