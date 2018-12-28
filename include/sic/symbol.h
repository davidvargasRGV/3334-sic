/*
 * include/sic/symbol.h
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#ifndef _SYMBOL_H
#define _SYMBOL_H

struct symbol_t {
	char *symbol;
	unsigned int address;

	struct symbol_t *next;
};

struct symbol_t **symbol_table;

void symbol_init();
struct symbol_t *symbol_fetch(register const char *str);
void symbol_insert(register const char *str, unsigned int address);
unsigned long hash_symbol(register const char *str);
void symbol_cleanup();

#endif
