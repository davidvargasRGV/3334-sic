/*
 * sic/symbol.c
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#include <stdlib.h>
#include <string.h>

#include <sic/symbol.h>
#include <sic/xmalloc.h>

void
symbol_cleanup()
{
	struct symbol_t *itr;

	int i;

	for (i = 0; i < 512; i++) {
		if (symbol_table[i]->next == NULL) {
			xfree(symbol_table[i]);
		} else {
			itr = symbol_table[i];

			while (itr->next != NULL) {
				itr = itr->next;
				xfree(itr);
			}

			xfree(symbol_table[i]);
		}
	}
}

/* djb2 */
unsigned long
hash_symbol(register const char *str)
{
	unsigned long hash;
	int c;

	hash = 5381;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) ^ c;	/* hash (i) = hash (i - 1) * 33 ^ str[i] */
	}

	return hash;
}

void
symbol_insert(register const char *str , unsigned int address)
{
	struct symbol_t *symbol;

	register unsigned int key;

	key = (unsigned int) hash_symbol(str);

	key = key % 512;

	if (symbol_table[key]->symbol == NULL) {
		symbol_table[key]->symbol = (char *) xmalloc(strlen(str));
		strcpy(symbol_table[key]->symbol, str);

		symbol_table[key]->address = address;
		symbol_table[key]->next = NULL;
	} else {
		symbol = (struct symbol_t *) xmalloc(sizeof(struct symbol_t));

		symbol->symbol = (char *) xmalloc(strlen(str));
		strcpy(symbol->symbol, str);

		symbol->address = address;
		symbol->next = symbol_table[key]->next;

		symbol_table[key]->next = symbol;
	}
}

struct symbol_t *
symbol_fetch(register const char *str)
{
	struct symbol_t *symbol;
	struct symbol_t *itr;

	register unsigned int key;

	symbol = (struct symbol_t *) xmalloc(sizeof(struct symbol_t));

	symbol->symbol = NULL;
	symbol->address = 0x0;
	symbol->next = NULL;

	key = (unsigned int) hash_symbol(str);

	key = key % 512;

	itr = symbol_table[key];

	if (itr->symbol == NULL) {
		return symbol;
	} else if (strncmp(str, itr->symbol, strlen(itr->symbol)) == 0) {
		symbol->symbol = (char *) xmalloc(strlen(itr->symbol));
		strcpy(symbol->symbol, itr->symbol);

		symbol->address = itr->address;
		symbol->next = NULL;
	}

	/* check linked list, in case of collisions */
	while (itr->next != NULL) {
		if (strncmp(str, itr->symbol, strlen(itr->symbol)) == 0) {
			symbol->symbol = (char *) xmalloc(strlen(itr->symbol));
			strcpy(symbol->symbol, itr->symbol);

			symbol->address = itr->address;
			symbol->next = NULL;
		}

		itr = itr->next;
	}

	return symbol;
}

void
symbol_init()
{
	int i;

	/* initialize symbol table  */
	symbol_table = (struct symbol_t **) xmalloc(512 * sizeof(struct symbol_t *));

	for (i = 0; i < 512; i++) {
		symbol_table[i] = (struct symbol_t *) xmalloc(sizeof(struct symbol_t));
	}

	for (i = 0; i < 512; i++) {
		symbol_table[i]->symbol = NULL;
		symbol_table[i]->address = 0x0;
		symbol_table[i]->next = NULL;
	}
}
