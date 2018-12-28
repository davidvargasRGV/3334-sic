/*
 * include/sic/parse.h
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#ifndef _PARSE_H
#define _PARSE_H

#define S_EOF -1
#define S_CMT  1
#define S_LBL  2

struct line_t {
	char *line;
	int status;
};

struct line_t line;

void parse_init();
void parse(FILE *stream, char *orig_line, char **argv, int length);
void parse_deinit();

#endif
