/*
 * src/dump.c
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#include <stdio.h>

#include <sic/dump.h>
#include <sic/ctype.h>
#include <sic/xmalloc.h>
#include <sic/sic.h>

void
dump(char *arg1, char *arg2)
{
	int i;
	int j;

	int start;
	int end;
	int offset;

	WORD mem;

	offset = stoi(arg1) % 10;

	start = stoh(arg1);
	end = stoh(arg2);

	for (i = start - offset; i <= end; i+= 16) {
		printf("%X: ", i);

		for (j = 0; j < 15; j++) {
			GetMem(i + j, mem, 0);
			printf("%02X ", mem[0]);
		}

		printf("\n");
	}
}
