/*
 * src/console.c
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sic/console.h>
#include <sic/sicdef.h>
#include <sic/xmalloc.h>
#include <sic/parse.h>
#include <sic/command.h>
#include <sic/execute.h>

void
console_deinit()
{
	parse_deinit();
}

void
console_parse()
{
	int i;

	if (console_tokens[0] == NULL) {
		return;
	}

	/* i should have a definition for num_commands instead of 8 */
	for (i = 0; i < (8); i++) {
		/* empty line */
		if (strlen(console_tokens[0]) == 0) {
			return;
		}

		if (strncmp(console_tokens[0], command[i].name, strlen(console_tokens[0])) == 0) {
			switch (i) {
				case 0:
					execute_assemble(console_tokens);
					return;
				case 1:
					execute_debug();
					return;
				case 2:
					execute_directory();
					return;
				case 3:
					execute_dump(console_tokens);
					return;
				case 4:
					execute_execute();
					return;
				case 5:
					execute_exit(console_tokens);
					return;
				case 6:
					execute_help();
					return;
				case 7:
					execute_load(console_tokens);
					return;
			}
		}
	}

	printf("Error: Command not found\n");
	printf("Error: Command = %s\n", console_tokens[0]);
}

void
console_cleanup()
{
	int i;

	for (i = 0; i < max_argc; i++) {
		xmemset(console_tokens[i], 0, max_argv * sizeof(char));
	}

	xmemset(line.line, 0, max_argv * sizeof(char));
}

void
console_prompt()
{
	printf("(sic) ");
}

void
console_read()
{
	console_prompt();

	console_cleanup();

	parse(stdin, line.line, console_tokens, 256);

	console_parse();
}

void
console_init()
{
	int i;

	console_tokens = (char **) xmalloc(max_argc * sizeof(char **));

	for (i = 0; i < max_argc; i++) {
		console_tokens[i] = (char *) xmalloc(max_argv * sizeof(char));
	}

	parse_init();
}
