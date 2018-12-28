/*
 * src/ctype.c
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#include <string.h>

#include <sic/ctype.h>

int
isdigit(int c)
{
	if (c >= 48 && c <= 57) {		/* 0 - 9 */
		return 1;
	}

	return 0;
}

int
isalpha(int c)
{
	if (c >= 65 && c <= 90) {		/* A - Z */
		return 1;
	} else if (c >= 97 && c <= 122) {	/* a - z */
		return 1;
	}

	return 0;
}

int
ishex(int c)
{
	if (isdigit(c)) {			/* 0 - 9 */
		return 1;
	} else if (c >= 65 && c <= 70) {	/* A - F */
		return 1;
	} else if (c >= 97 && c <= 102) {	/* a - f */
		return 1;
	}

	return 0;
}

int
isvalid(char *str)
{
	int i;

	for (i = 0; i < strlen(str); i++) {
		if (!ishex(str[i])) {
			return 0;
		}
	}

	return 1;
}

int
isvalidint(char *str)
{
	int i;

	for (i = 0; i < strlen(str); i++) {
		if (!isdigit(str[i])) {
			return 0;
		}
	}

	return 1;
}

int
isdelim(int c)
{
	switch (c) {
		case -1:			/* EOF character */
		case 10:			/* newline character */
			return 1;
	}

	return 0;
}

int
isspace(int c)
{
	switch ( c ) {
		case  9:			/* tab character */
		case 11:			/* vertical tab character */
		case 12:			/* form feed character */
		case 13:			/* carriage return character */
		case 32:			/* space character */
			return 1;
	}

	return 0;
}

int
stoh(char *str)
{
	int i;
	int sum;
	int multiplier;

	sum = 0;
	multiplier = 1;

	for (i = strlen(str) - 1; i >= 0; i--) {
		switch (str[i]) {
			case '0':
				sum += 0 * multiplier;
				break;
			case '1':
				sum += 1 * multiplier;
				break;
			case '2':
				sum += 2 * multiplier;
				break;
			case '3':
				sum += 3 * multiplier;
				break;
			case '4':
				sum += 4 * multiplier;
				break;
			case '5':
				sum += 5 * multiplier;
				break;
			case '6':
				sum += 6 * multiplier;
				break;
			case '7':
				sum += 7 * multiplier;
				break;
			case '8':
				sum += 8 * multiplier;
				break;
			case '9':
				sum += 9 * multiplier;
				break;
			case 'A':
			case 'a':
				sum += 10 * multiplier;
				break;
			case 'B':
			case 'b':
				sum += 11 * multiplier;
				break;
			case 'C':
			case 'c':
				sum += 12 * multiplier;
				break;
			case 'D':
			case 'd':
				sum += 13 * multiplier;
				break;
			case 'E':
			case 'e':
				sum += 14 * multiplier;
				break;
			case 'F':
			case 'f':
				sum += 15 * multiplier;
				break;
		}

		multiplier *= 16;
	}

	return sum;
}

int
stoi(char *str)
{
	int i;
	int sum;
	int multiplier;

	sum = 0;
	multiplier = 1;

	for (i = strlen(str) - 1; i >= 0; i--) {
		switch (str[i]) {
			case '0':
				sum += 0 * multiplier;
				break;
			case '1':
				sum += 1 * multiplier;
				break;
			case '2':
				sum += 2 * multiplier;
				break;
			case '3':
				sum += 3 * multiplier;
				break;
			case '4':
				sum += 4 * multiplier;
				break;
			case '5':
				sum += 5 * multiplier;
				break;
			case '6':
				sum += 6 * multiplier;
				break;
			case '7':
				sum += 7 * multiplier;
				break;
			case '8':
				sum += 8 * multiplier;
				break;
			case '9':
				sum += 9 * multiplier;
				break;
		}

		multiplier *= 10;
	}

	return sum;
}
