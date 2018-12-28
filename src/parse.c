/*
 * src/parse.c
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#include <stdio.h>

#include <sic/parse.h>
#include <sic/sicdef.h>
#include <sic/assemble.h>
#include <sic/xmalloc.h>
#include <sic/ctype.h>

void
parse_deinit()
{
	xfree(line.line);
}

void
parse(FILE *stream, char *orig_line, char **tokens, int length)
{
	int i;
	int row;

	char ch;
	char *itr;
	char *ptr;

	ptr = orig_line;

	ch = fgetc(stream);

	if (isspace(ch)) {
		line.status = 0;
	} else if (ch == '.') {
		line.status = S_CMT;
	} else {
		line.status = S_LBL;
	}

	for (i = 0, row = 0; row < max_argc && i < length; i++) {
		/* eliminate preceeding spaces from tokens */
		while (isspace(ch)) {
			*ptr = ch;
			ptr++;

			ch = fgetc(stream);
		}

		if (isdelim(ch)) {
			if (ch == EOF) {
				line.status = S_EOF;
			}

			break;
		}

		itr = tokens[row];

		/* loop while non-space characters are found */
		while (!isdelim(ch) && !isspace(ch)) {
			*itr = ch;
			itr++;

			*ptr = ch;
			ptr++;

			ch = fgetc(stream);
		}

		row++;
	}
}

void
parse_init()
{
	line.line = (char *) xmalloc(max_argv * sizeof(char *));
	xmemset(line.line, 0, max_argv * sizeof(char));
}
