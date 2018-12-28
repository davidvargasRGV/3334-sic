/*
 * include/sic/opcode.h
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#ifndef _OPCODE_H
#define _OPCODE_H

struct opcode_t {
	char *mnemonic;
	unsigned int opcode;
};

struct opcode_t **opcode_table;

void opcode_init();
struct opcode_t *opcode_fetch(register const char *str);
unsigned int hash_opcode(register const char *str, register unsigned int len);
void opcode_free();

#endif
