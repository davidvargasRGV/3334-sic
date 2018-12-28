/*
 * src/opcode.c
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#include <stdlib.h>
#include <string.h>

#include <sic/opcode.h>
#include <sic/xmalloc.h>

void
opcode_free()
{
	int i;

	for (i = 0; i < 49; i++) {
		free(opcode_table[i]);
	}

	free(opcode_table);
}

unsigned int
hash_opcode(register const char *str, register unsigned int len)
{
	register int hval;

	unsigned char asso_values[] = {
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 10, 23, 15,  5,  5,
		49,  5, 49, 10,  8, 49,  0,  3, 30, 10,
		49,  5, 10,  0,  0,  0, 25, 20, 25, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
		49, 49, 49, 49, 49, 49
	};

	hval = len;

	switch ( hval ) {
		default:
			hval += asso_values[(unsigned char)str[2]];
		case 2:
			hval += asso_values[(unsigned char)str[1]];
		case 1:
			hval += asso_values[(unsigned char)str[0]];
			break;
	}

	return hval;
}

struct opcode_t *
opcode_fetch(register const char *str)
{
	struct opcode_t *opcode;
	register int key;

	opcode = (struct opcode_t *) xmalloc(sizeof(struct opcode_t));

	opcode->mnemonic = NULL;
	opcode->opcode = 0x0;

	key = hash_opcode(str, strlen(str));

	if (key < 49) {
		if (opcode_table[key]->mnemonic != NULL) {
			if (strncmp(opcode_table[key]->mnemonic, str, strlen(opcode_table[key]->mnemonic)) == 0) {
				opcode->mnemonic = (char *) xmalloc(strlen(str) * sizeof(char));
				strcpy(opcode->mnemonic, opcode_table[key]->mnemonic);

				opcode->opcode = opcode_table[key]->opcode;
			}
		}
	}

	return opcode;
}

void
opcode_init()
{
	int i;

	/* initialize opcode table */
	opcode_table = (struct opcode_t **) xmalloc(49 * sizeof(struct opcode_t *));

	for (i = 0; i < 49; i++) {
		opcode_table[i] = (struct opcode_t *) xmalloc(sizeof(struct opcode_t));
	}

	for (i = 0; i < 49; i++) {
		opcode_table[i]->mnemonic = NULL;
		opcode_table[i]->opcode = 0x0;
	}

	/* manually build opcode table */
	opcode_table[3]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[3]->mnemonic, "STL");
	opcode_table[3]->opcode = 0x14;

	opcode_table[4]->mnemonic = (char *) xmalloc(4 * sizeof(char));
	strcpy(opcode_table[4]->mnemonic, "STSW");
	opcode_table[4]->opcode = 0xE8;

	opcode_table[6]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[6]->mnemonic, "MUL");
	opcode_table[6]->opcode = 0x20;

	opcode_table[7]->mnemonic = (char *) xmalloc(2 * sizeof(char));
	strcpy(opcode_table[7]->mnemonic, "TD");
	opcode_table[7]->opcode = 0xE0;

	opcode_table[8]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[8]->mnemonic, "LDL");
	opcode_table[8]->opcode = 0x08;

	opcode_table[9]->mnemonic = (char *) xmalloc(1 * sizeof(char));
	strcpy(opcode_table[9]->mnemonic, "J");
	opcode_table[9]->opcode = 0x3C;

	opcode_table[11]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[11]->mnemonic, "JLT");
	opcode_table[11]->opcode = 0x38;

	opcode_table[12]->mnemonic = (char *) xmalloc(4 * sizeof(char));
	strcpy(opcode_table[12]->mnemonic, "JSUB");
	opcode_table[12]->opcode = 0x48;

	opcode_table[13]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[13]->mnemonic, "STA");
	opcode_table[13]->opcode = 0x0C;

	opcode_table[14]->mnemonic = (char *) xmalloc(4 * sizeof(char));
	strcpy(opcode_table[14]->mnemonic, "RSUB");
	opcode_table[14]->opcode = 0x4C;

	opcode_table[16]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[16]->mnemonic, "JGT");
	opcode_table[16]->opcode = 0x34;

	opcode_table[17]->mnemonic = (char *) xmalloc(2 * sizeof(char));
	strcpy(opcode_table[17]->mnemonic, "RD");
	opcode_table[17]->opcode = 0xD8;

	opcode_table[18]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[18]->mnemonic, "LDA");
	opcode_table[18]->opcode = 0x00;

	opcode_table[19]->mnemonic = (char *) xmalloc(4 * sizeof(char));
	strcpy(opcode_table[19]->mnemonic, "STCH");
	opcode_table[19]->opcode = 0x54;

	opcode_table[21]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[21]->mnemonic, "JEQ");
	opcode_table[21]->opcode = 0x30;

	opcode_table[22]->mnemonic = (char *) xmalloc(2 * sizeof(char));
	strcpy(opcode_table[22]->mnemonic, "OR");
	opcode_table[22]->opcode = 0x44;

	opcode_table[23]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[23]->mnemonic, "ADD");
	opcode_table[23]->opcode = 0x18;

	opcode_table[24]->mnemonic = (char *) xmalloc(4 * sizeof(char));
	strcpy(opcode_table[24]->mnemonic, "LDCH");
	opcode_table[24]->opcode = 0x50;

	opcode_table[26]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[26]->mnemonic, "SUB");
	opcode_table[26]->opcode = 0x1C;

	opcode_table[27]->mnemonic = (char *) xmalloc(2 * sizeof(char));
	strcpy(opcode_table[27]->mnemonic, "WD");
	opcode_table[27]->opcode = 0xDC;

	opcode_table[28]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[28]->mnemonic, "STX");
	opcode_table[28]->opcode = 0x10;

	opcode_table[32]->mnemonic = (char *) xmalloc(4 * sizeof(char));
	strcpy(opcode_table[32]->mnemonic, "COMP");
	opcode_table[32]->opcode = 0x28;

	opcode_table[33]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[33]->mnemonic, "LDX");
	opcode_table[33]->opcode = 0x04;

	opcode_table[38]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[38]->mnemonic, "TIX");
	opcode_table[38]->opcode = 0x2C;

	opcode_table[43]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[43]->mnemonic, "DIV");
	opcode_table[43]->opcode = 0x24;

	opcode_table[48]->mnemonic = (char *) xmalloc(3 * sizeof(char));
	strcpy(opcode_table[48]->mnemonic, "AND");
	opcode_table[48]->opcode = 0x58;
}
