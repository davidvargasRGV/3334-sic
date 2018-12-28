/*
 * src/load.c
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <sic/load.h>
#include <sic/sic.h>
#include <sic/xmalloc.h>
#include <sic/ctype.h>

void
load_parse(FILE *fp)
{
	ADDRESS addy;

	WORD value;

	char *str;
	char *itr;

	char ch;

	int len;
	int i;
	int j;

	str = (char *) xmalloc(20 * sizeof(char));

	ch = fgetc(fp);

	/* loop until EOF */
	while (ch != -1) {
		/* header record found */
		if ( ch == 'H' ) {
			/* clear the rest of the line */
			while (ch != '\n') {
				ch = fgetc(fp);
			}
		/* text record found */
		} else if (ch == 'T') {
			ch = fgetc(fp);

			itr = str;
			xmemset(str, 0, 20 * sizeof(char));

			/* get the load address */
			for (i = 0; i < 6; i++) {
				*itr = ch;
				itr++;

				ch = fgetc(fp);
			}

			addy = stoh(str);

			itr = str;
			xmemset(str, 0, 20 * sizeof(char));

			/* get the number of bytes */
			for (i = 0; i < 2; i++) {
				*itr = ch;
				itr++;

				ch = fgetc(fp);
			}

			len = stoh(str);

			/* get each byte and put it into memory */
			for (i = 0; i < len; i++, addy++) {
				itr = str;
				xmemset(str, 0, 20 * sizeof(char));

				for (j = 0; j < 2; j++) {
					*itr = ch;
					itr++;

					ch = fgetc(fp);
				}

				value[0] = stoh(str);

				PutMem(addy, value, 0);
			}

			/* clear the rest of the line */
			while (ch != '\n') {
				ch = fgetc(fp);
			}
		/* end record found */
		} else if (ch == 'E') {
			ch = fgetc(fp);

			itr = str;
			xmemset(str, 0, 20 * sizeof(char));

			/* get the start_address */
			for (i = 0; i < 6; i++) {
				*itr = ch;
				itr++;

				ch = fgetc(fp);
			}

			start_address[0] = stoh(str);

			/* clear the rest of the line */
			while (ch != '\n') {
				ch = fgetc(fp);
			}
		}

		ch = fgetc ( fp );
	}
}

void
load(FILE *fp)
{
	load_parse(fp);
}

void
load_init()
{
	/* initialize start address for execute at 32K */
	start_address[0] = 32768;
}
