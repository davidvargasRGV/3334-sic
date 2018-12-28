/*
 * src/assemble.c
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#include <stdio.h>
#include <string.h>

#include <sic/assemble.h>
#include <sic/sicdef.h>
#include <sic/xmalloc.h>
#include <sic/parse.h>
#include <sic/opcode.h>
#include <sic/symbol.h>
#include <sic/error.h>
#include <sic/ctype.h>

void
assemble_cleanup()
{
	opcode_free();
	symbol_cleanup();
}

void
assemble_line_set()
{
	int i;

	for (i = 0; i < max_argc; i++) {
		xmemset(assemble_tokens[i], 0, max_argv * sizeof(char));
	}

	xmemset(line.line, 0, max_argv * sizeof(char));

	line.status = 0;
}

int
assemble_byte_valid(char *str)
{
	int i;

	if (str[0] == 'C' || str[0] == 'c') {
		if (str[1] == '\'' || str[strlen (str)-1] == '\'') {
			return 1;
		} else {
			return 0;
		}
	} else if ( str[0] == 'X' || str[0] == 'x') {
		if (str[1] == '\'' || str[strlen (str)-1] == '\'') {
			if (((strlen(str) - 3) % 2) == 0) {
				for (i = 2; i < (strlen(str) - 1); i++) {
					if (!ishex(str[i])) {
						return 0;
					}
				}

				return 1;
			} else {
				return 0;
			}
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}

int
assemble_byte(char *str)
{
	if (str[0] == 'C' || str[0] == 'c') {
		return (strlen(str) - 3);
	} else if (str[0] == 'X' || str[0] == 'x'){
		return ((strlen(str) - 3) / 2);
	} else {
		return 0;
	}
}

void
assemble_pass_two(FILE *stream, FILE *list_file)
{
	FILE *obj_file;

	struct opcode_t *opcode;
	struct symbol_t *symbol;

	unsigned int instruction;
	unsigned int address;

	char *tmp_str;
	char tmp_ch;
	int tmp_int;

	int index;
	int offset;
	int size;
	int linenumber;
	int status;

	obj_file = NULL;

	opcode = NULL;
	symbol = NULL;

	instruction = 0;
	address = 0;

	tmp_str = NULL;
	tmp_ch = 0;
	tmp_int = 0;

	index = 3;
	offset = 0;
	linenumber = 1;
	status = 0;

	/* CREATE LISTING FILE */

	assemble_line_set(); /* always reset flags before parsing the line */
	parse(stream, line.line, assemble_tokens, 256);

	if (strncmp(assemble_tokens[0], "01", 2) == 0) {
		offset = 1;
	} else {
		offset = 0;
	}

	if (strncmp(assemble_tokens[index+offset], "START", strlen(assemble_tokens[index+offset])) == 0) {
		fprintf(list_file, "XXXXXX  %s\n", line.line);

		assemble_line_set(); /* always reset flags before parsing the line */
		parse(stream, line.line, assemble_tokens, 256);

		if (strncmp(assemble_tokens[0], "01", 2) == 0) {
			offset = 1;
		} else {
			offset = 0;
		}

		while (strncmp(assemble_tokens[0], "ERROR", strlen(assemble_tokens[0])) == 0) {
			printf("%s\n", line.line);
			fprintf(list_file, "%s\n", line.line);

			assemble_line_set(); /* always reset flags before parsing the line */
			parse(stream, line.line, assemble_tokens, 256);

			if (strncmp(assemble_tokens[0], "01", 2) == 0) {
				offset = 1;
			} else {
				offset = 0;
			}

			status = 1;
		}
	}

	linenumber++;

	while (line.status != S_EOF && (strncmp(assemble_tokens[3], "END", strlen(assemble_tokens[3])) != 0 || strlen(assemble_tokens[3]) != 3)) {
		while (strncmp(assemble_tokens[0], "ERROR", strlen(assemble_tokens[0])) == 0) {
			printf("%s\n", line.line);
			fprintf(list_file, "%s\n", line.line);

			assemble_line_set(); /* always reset flags before parsing the line */
			parse(stream, line.line, assemble_tokens, 256);

			if (strncmp(assemble_tokens[0], "01", 2) == 0) {
				offset = 1;
			} else {
				offset = 0;
			}

			status = 1;
		}

		/* gotta check for END, in case line prior to END has ERRORS... */
		if (status && strncmp(assemble_tokens[3], "END", strlen(assemble_tokens[3])) == 0) {
			if (strlen(assemble_tokens[3]) == 3) {
				break;
			}
		}

		if (strncmp(assemble_tokens[0], "99", 2) != 0 && strlen(assemble_tokens[3]) != 0) {
			opcode = opcode_fetch(assemble_tokens[index+offset]);

			if (opcode->mnemonic != NULL) {
				if (strncmp(opcode->mnemonic, "RSUB", 4) == 0) {
					instruction = stoh(assemble_tokens[2]);

					fprintf(list_file, "%06X  %s\n", instruction, line.line);
				} else {
					instruction = stoh(assemble_tokens[2]);

					if (strlen(assemble_tokens[index+offset+1]) == 0) {
						fprintf(list_file, "%06X  %s\n", instruction, line.line);
						printf("ERROR - Line %02i - Missing or Misplaced Operand\n", linenumber);
						fprintf(list_file, "ERROR - Line %02i - Missing or Misplaced Operand\n", linenumber);

						status = 1;
					} else if (strncmp(&assemble_tokens[index+offset+1][strlen(assemble_tokens[index+offset+1]) - 2], ",X", 2) == 0) {
						tmp_str = (char *) xmalloc(strlen(assemble_tokens[index+offset+1]) - 1);
						strncpy(tmp_str, assemble_tokens[index+offset+1], strlen(assemble_tokens[index+offset+1]) - 2);

						symbol = symbol_fetch(tmp_str);

						if (symbol->symbol == NULL) {
							address = 0;

							fprintf(list_file, "%06X  %s\n", instruction, line.line);
							printf("ERROR - Line %02i - Undefined Symbol\n", linenumber);
							fprintf(list_file, "ERROR - Line %02i - Undefined Symbol\n", linenumber);

							status = 1;
						} else {
							address = symbol->address;
							address += 0x8000;

							instruction += address;

							xfree(symbol->symbol);

							fprintf(list_file, "%06X  %s\n", instruction, line.line);
						}

						xfree(symbol);
						xfree(tmp_str);
					} else {
						symbol = symbol_fetch(assemble_tokens[index+offset+1]);

						if (symbol->symbol == NULL) {
							address = 0;

							fprintf(list_file, "%06X  %s\n", instruction, line.line);
							printf("ERROR - Line %02i - Undefined Symbol\n", linenumber);
							fprintf(list_file, "ERROR - Line %02i - Undefined Symbol\n", linenumber);

							status = 1;
						} else {
							address = symbol->address;

							instruction += address;

							xfree(symbol->symbol);

							fprintf(list_file, "%06X  %s\n", instruction, line.line);
						}

						xfree(symbol);
					}
				}

				xfree(opcode->mnemonic);
			} else {
				if (strncmp(assemble_tokens[index+offset], "WORD", 4) == 0) {
					if (!isvalidint(assemble_tokens[index+offset+1])) {
						printf("ERROR - Line %02i - Illegal Operand in WORD Statement\n", linenumber);
						fprintf(list_file, "ERROR - Line %02i - Illegal Operand in WORD Statement\n", linenumber);
					} else {
						instruction = stoi(assemble_tokens[index+offset+1]);
						fprintf(list_file, "%06X  %s\n", instruction, line.line);
					}
				} else if (strncmp(assemble_tokens[index+offset], "BYTE", 4) == 0) {
					if (assemble_tokens[index+offset+1][0] == 'C' || assemble_tokens[index+offset+1][0] == 'c') {
						tmp_str = (char *) xmalloc(strlen(assemble_tokens[index+offset+1]) - 2);
						strncpy(tmp_str, &assemble_tokens[index+offset+1][2], strlen(assemble_tokens[index+offset+1]) - 3);

						for (tmp_int = 0; tmp_int < strlen(tmp_str); tmp_int++) {
							tmp_ch = tmp_str[tmp_int];

							fprintf(list_file, "%X", tmp_ch);
						}

						fprintf(list_file, "  %s\n", line.line);

						xfree(tmp_str);
					} else if (assemble_tokens[index+offset+1][0] == 'X' || assemble_tokens[index+offset+1][0] == 'x') {
						tmp_str = (char *) xmalloc(strlen(assemble_tokens[index+offset+1]) - 2);
						strncpy(tmp_str, &assemble_tokens[index+offset+1][2], strlen(assemble_tokens[index+offset+1]) - 3);

						fprintf(list_file, "%-6s  %s\n", tmp_str, line.line);

						xfree(tmp_str);
					}
				} else {
					fprintf(list_file, "XXXXXX  %s\n", line.line);
				}
			}

			xfree(opcode);
		} else {
			fprintf(list_file, "XXXXXX  %s\n", line.line);
		}

		assemble_line_set();
		parse(stream, line.line, assemble_tokens, 256);

		if (strncmp(assemble_tokens[0], "01", 2 ) == 0) {
			offset = 1;
		} else {
			offset = 0;
		}

		linenumber++;
	}

	fprintf(list_file, "XXXXXX  %s\n", line.line);

	assemble_line_set();
	parse(stream, line.line, assemble_tokens, 256);

	while (strncmp(assemble_tokens[0], "ERROR", strlen(assemble_tokens[0])) == 0 && strlen(assemble_tokens[0]) != 0) {
		printf("%s\n", line.line);
		fprintf(list_file, "%s\n", line.line);

		assemble_line_set(); /* always reset flags before parsing the line */
		parse(stream, line.line, assemble_tokens, 256);

		status = 1;
	}

	fclose(stream);
	fclose(list_file);

	if (sic.length > 32768) {
		status = 1;
	}

	if (status == 0) {
		list_file = fopen("doc/listing.sic", "r");
		obj_file = fopen("doc/object.sic", "w");

		if (list_file == NULL || obj_file == NULL) {
			error("Unable to open file\n");
		} else {
			/* create header record */
			fprintf(obj_file, "H%-6s%06X%06X\n", sic.name, sic.start, sic.length);

			/* create text records */
			assemble_line_set();
			parse(stream, line.line, assemble_tokens, 256);

			tmp_str = NULL;
			address = 0;
			size = 0;

			while (line.status != S_EOF) {
				if (strncmp(assemble_tokens[0], "XXXXXX", 6 * sizeof (char)) == 0) {
					if (strncmp(assemble_tokens[1], "99", 2 * sizeof (char)) == 0) {
						/* dont worry about comments */
					} else {
						if (tmp_str == NULL) {
							/* nothing to dump */
						} else {
                                   			/* dump the contents of tmp_str */
							fprintf(obj_file, "T%06X%02X%s\n", address, (unsigned int) (strlen(tmp_str) / 2), tmp_str);
							xfree(tmp_str);

							tmp_str = NULL;
							size = 0;
						}
					}
				} else {
					if ((size + strlen(assemble_tokens[0])) > 60) {
						/* dump the contents of tmp_str before adding the next instruction to the text record */
						fprintf(obj_file, "T%06X%02X%s\n", address, (unsigned int) (strlen(tmp_str) / 2), tmp_str);
						xfree(tmp_str);

						size = 0;

						tmp_str = (char *) xmalloc(65 * sizeof(char));
						xmemset(tmp_str, 0, 65 * sizeof(char));

                              			strcat(tmp_str, assemble_tokens[0]);
						size += strlen(assemble_tokens[0]);

						address = stoh(assemble_tokens[2]);
					} else {
						if (tmp_str == NULL) {
							/* allocate memory for tmp_str, and initialize text record address */
							tmp_str = (char *) xmalloc(65 * sizeof(char));
							xmemset(tmp_str, 0, 65 * sizeof(char));

							strcat(tmp_str, assemble_tokens[0]);
							size += strlen(assemble_tokens[0]);

							address = stoh(assemble_tokens[2]);
						} else {
							/* add instruction to current text record */
							strcat(tmp_str, assemble_tokens[0]);
							size += strlen(assemble_tokens[0]);
						}
					}
				}

				assemble_line_set();
				parse(stream, line.line, assemble_tokens, 256);
			}

			if (tmp_str != NULL) {
				/* dump the last text record, if there is anything left to dump */
				fprintf( obj_file, "T%06X%02X%s\n", address, (unsigned int) (strlen(tmp_str) / 2), tmp_str);
				xfree(tmp_str);
			}

			/* create end record */
			fprintf(obj_file, "E%06X\n", sic.start);

			fclose(list_file);
			fclose(obj_file);
		}
	}
}

void
assemble_pass_one(FILE *stream, FILE *int_file)
{
	struct opcode_t *opcode;
	struct symbol_t *symbol;

	int i;
	int linenumber;
	int symbolnumber;

	opcode = NULL;
	symbol = NULL;

	i = 0;
	linenumber = 1;
	symbolnumber = 0;

	assemble_line_set(); /* always reset flags before parsing the line */
	parse(stream, line.line, assemble_tokens, 256);

	if (strncmp(assemble_tokens[1], "START", strlen(assemble_tokens[1])) == 0) {
		sic.name = (char *) xmalloc((strlen(assemble_tokens[0]) * sizeof(char)) + sizeof(char));
		strncpy(sic.name, assemble_tokens[0], strlen(assemble_tokens[0]) * sizeof(char));
		sic.name[strlen(assemble_tokens[0])] = '\0';

		if (strlen(assemble_tokens[2]) == 0) {
			fprintf(int_file, "ERROR - Line %02i - Missing or Misplaced Operand in START Statement\n", linenumber);
		} else {
			/* save #[operand] as starting address */
			if (isvalid(assemble_tokens[2])) {
				sic.start = stoh(assemble_tokens[2]);
				sic.counter = sic.start;

				fprintf(int_file, "01  %06X  000000  %s\n", sic.counter, line.line);
			} else {
				sic.start = 0;
				sic.counter = 0;

				fprintf(int_file, "01  %06X  000000  %s\n", sic.counter, line.line);
				fprintf(int_file, "ERROR - Line %02i - Invalid START Address\n", linenumber);
			}
		}

		assemble_line_set(); /* always reset flags before parsing the line */
		parse(stream, line.line, assemble_tokens, 256);
	} else {
		sic.start = 0;
		sic.counter = 0;

		fprintf(int_file, "01  %06X  000000  %s\n", sic.counter, line.line);
		fprintf(int_file, "ERROR - Line %02i - Missing or Misplaced START Directive\n", linenumber);
	}

	linenumber++;

	while (line.status != S_EOF && (strncmp(assemble_tokens[0], "END", strlen(assemble_tokens[0])) != 0 || strlen(assemble_tokens[0]) != 3)) {
		if (line.status == S_CMT) {
			fprintf(int_file, "99  %06X  000000  %s\n", sic.counter, line.line);
		} else if (line.status == S_LBL) {
			if (strlen(assemble_tokens[1]) == 0) {
				fprintf(int_file, "01  %06X  000000  %s\n", sic.counter, line.line);
				fprintf(int_file, "ERROR - Line %02i - Missing or Misplaced Mnemonic\n", linenumber);
			} else {
				opcode = opcode_fetch(assemble_tokens[1]);
				fprintf(int_file, "01  %06X  %02X0000  %s\n", sic.counter, opcode->opcode, line.line);
			}
		} else {
			if (strlen (assemble_tokens[0]) == 0) {
				fprintf(int_file, "01  %06X  000000  %s\n", sic.counter, line.line);
				fprintf(int_file, "ERROR - Line %02i - Missing or Misplaced Mnemonic\n", linenumber);
			} else {
				opcode = opcode_fetch(assemble_tokens[0]);
				fprintf(int_file, "00  %06X  %02X0000  %s\n", sic.counter, opcode->opcode, line.line);
			}
		}

		if (line.status != S_CMT && strlen(assemble_tokens[1]) != 0) {
			if (line.status == S_LBL) {
				symbol = symbol_fetch(assemble_tokens[0]);

				if (symbol->symbol == NULL) {
					if (isdigit(assemble_tokens[0][0])) {
						fprintf(int_file, "ERROR - Line %02i - Invalid Label\n", linenumber);
					} else if (strlen(assemble_tokens[0]) > 6) {
						fprintf(int_file, "ERROR - Line %02i - Label too Long\n", linenumber);
					} else {
						if (symbolnumber > 500) {
							fprintf(int_file, "ERROR - Line %02i - Too Many Symbols in Symbol Table\n", linenumber);
						} else {
							symbol_insert(assemble_tokens[0], sic.counter);
							symbolnumber++;
						}
					}
				} else {
					fprintf(int_file, "ERROR - Line %02i - Duplicate Symbol\n", linenumber);

					xfree(symbol->symbol);
				}

				xfree(symbol);
				i = 1;
			}

			if (opcode->mnemonic == NULL) {
				if (strncmp(assemble_tokens[i], "WORD", 4) == 0 && strlen(assemble_tokens[i]) == 4) {
					if (strlen(assemble_tokens[i+1]) == 0) {
						fprintf(int_file, "ERROR - Line %02i - Missing or Misplaced Operand in WORD Statement\n", linenumber);
					} else {
						sic.counter += 3;
					}
				} else if (strncmp(assemble_tokens[i], "RESW", 4) == 0 && strlen(assemble_tokens[i]) == 4) {
					if (strlen(assemble_tokens[i+1]) == 0) {
						fprintf(int_file, "ERROR - Line %02i - Missing or Misplaced Operand in RESW Statement\n", linenumber);
					} else {
						if (isvalidint(assemble_tokens[i+1])) {
							sic.counter += 3 * stoi(assemble_tokens[i+1]);
						} else {
							fprintf(int_file, "ERROR - Line %02i - Invalid Operand\n", linenumber);
						}
					}
				} else if (strncmp(assemble_tokens[i], "RESB", 4) == 0 && strlen(assemble_tokens[i]) == 4) {
					if (strlen(assemble_tokens[i+1]) == 0) {
						fprintf(int_file, "ERROR - Line %02i - Missing or Misplaced Operand in RESB Statement\n", linenumber);
					} else {
						if (isvalidint(assemble_tokens[i+1])) {
							sic.counter += stoi(assemble_tokens[i+1]);
						} else {
							fprintf(int_file, "ERROR - Line %02i - Invalid Operand\n", linenumber);
						}
					}
				} else if (strncmp(assemble_tokens[i], "BYTE", 4) == 0 && strlen(assemble_tokens[i]) == 4) {
					if (strlen(assemble_tokens[i+1]) == 0) {
						fprintf(int_file, "ERROR - Line %02i - Missing or Misplaced Operand in BYTE Statement\n", linenumber);
					} else {
						if (assemble_byte_valid(assemble_tokens[i+1])) {
							sic.counter += assemble_byte(assemble_tokens[i+1]);
						} else {
							fprintf(int_file, "ERROR - Line %02i - Invalid Operand\n" , linenumber);
						}
					}
				} else if (strncmp(assemble_tokens[i], "START", 5) == 0 && strlen(assemble_tokens[i]) == 5) {
					fprintf(int_file, "ERROR - Line %02i - Duplicate or Misplaced START Directive\n" , linenumber);
				} else {
					fprintf(int_file, "ERROR - Line %02i - Invalid Opcode\n" , linenumber);
				}
			} else {
				sic.counter += 3;

				xfree(opcode->mnemonic);
			}

			xfree(opcode);
		}

		/* quick fix */
		if (strncmp(assemble_tokens[0], "RSUB", 4) == 0 && strlen(assemble_tokens[1]) == 0 && strlen(assemble_tokens[0]) == 4) {
			sic.counter += 3;
		}

		assemble_line_set(); /* always reset flags before parsing the line */
		parse(stream, line.line, assemble_tokens, 256);

		linenumber++;
		i = 0;
	}

	linenumber--;
	sic.length = sic.counter - sic.start;

	fprintf(int_file, "00  %06X  000000  %s\n", sic.counter, line.line);

	if (strlen(assemble_tokens[1]) == 0) {
		fprintf(int_file, "ERROR - Line %02i - Missing or Misplaced Operand in END Statement\n" , linenumber);
	}

	if (strncmp(assemble_tokens[1], sic.name, strlen(assemble_tokens[1])) != 0 && strlen(assemble_tokens[1]) != strlen(sic.name)) {
		fprintf(int_file, "ERROR - Line %02i - Illegal Operand in END Statement\n" , linenumber);
	}

	fclose(int_file);
}

void
assemble(FILE *stream)
{
	FILE *int_file;
	FILE *list_file;

	assemble_init();
	opcode_init();
	symbol_init();

	int_file = fopen("doc/intermediate.sic", "w");

	if (int_file == NULL) {
		error("Unable to open file\n");
	} else {
		assemble_pass_one(stream, int_file);

		int_file = fopen("doc/intermediate.sic", "r");
		list_file = fopen("doc/listing.sic", "w");

		if (list_file == NULL) {
			error("Unable to open file\n");
		} else {
			assemble_pass_two(int_file, list_file);
		}
	}

	assemble_cleanup();
}

void
assemble_init()
{
	int i;

	assemble_tokens = (char **) xmalloc(max_argc * sizeof(char **));

	for (i = 0; i < max_argc; i++) {
		assemble_tokens[i] = (char *) xmalloc(max_argv * sizeof(char));
		xmemset(assemble_tokens[i], 0, max_argv * sizeof(char));
	}

	sic.name = NULL;
	sic.counter = 0;
	sic.length = 0;

	assemble_line_set();
}
