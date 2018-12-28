/*
 * include/sic/assemble.h
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#ifndef _ASSEMBLE_H
#define _ASSEMBLE_H

/* sic flags */
struct sic_t {
	char *name;
	int start;
	int counter;
	int length;
};

struct sic_t sic;

char **assemble_tokens;

void assemble_init();
void assemble(FILE *stream);
void assemble_pass_one(FILE *stream, FILE *int_file);
void assemble_pass_two(FILE *stream, FILE *list_file);
void assemble_line_set();
void assemble_cleanup();

#endif
