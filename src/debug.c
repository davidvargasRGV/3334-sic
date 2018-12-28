/*
 * src/debug.c
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#include <stdio.h>
#include <string.h>

#include <sic/debug.h>
#include <sic/xmalloc.h>
#include <sic/load.h>
#include <sic/sicdef.h>
#include <sic/sic.h>
#include <sic/parse.h>
#include <sic/debugger.h>
#include <sic/assemble.h>

/*
     debugging commands

     set -- set variables
          set memory [memory_location] [hex_value]
          set register [register_value] [hex_value]

     print -- print variables once
          print memory [memory_location] [output_format]
          print register [register_value] [output_format]

     ex:  print register A X (print register A as a hex value)

     display -- display variables after each step
          display memory [memory_location] [output_format]
          display register [register_value] [output_format]

     ex:  display memory 1000 I (display memory location 1000 as an integer value)
 */

void
debug_deinit()
{
	int i;

	for (i = 0; i < max_argc; i++) {
		xfree(debug_tokens[i]);
	}

	xfree(debug_tokens);
}

void
debug_command_run()
{
	SICRun(start_address, 0);

	start_address[0] = sic.start;
}

void
debug_command_step()
{
	SICRun(start_address, 1);

	/* get the address of the next instruction */
	start_address[0] = GetPC();
}

void
debug_command_print()
{

}

void
debug_command_help()
{
	printf("Usage: (deb) [commands] [options]...\n");
	printf("Commands:\n");
	printf("  exit\t\t\tExit the SIC Debugger\n");
	printf("  help\t\t\tDisplay this help message\n");
	printf("  run\t\t\tContinue with normal execution\n");
	printf("  step\t\t\tStep into the next instruction\n");
}

void
debug_command_exit()
{
	debugging = 0;
	start_address[0] = 32768;
}

void
debug_parse()
{
	int i;

	if (debug_tokens[0] == NULL) {
		return;
	}

	/* i should have a definition for num_debug_commands instead of 5 */
	for (i = 0; i < (5); i++) {
		if (strlen(debug_tokens[0]) == 0) {
			return;
		}

		if (strncmp(debug_tokens[0], debug_command[i].name, strlen(debug_tokens[0])) == 0) {
			switch (i) {
				/* exit */
				case 0:
					debug_command_exit();
					return;
				/* help */
				case 1:
					debug_command_help();
					return;
				/* print */
				case 2:
					debug_command_print();
					return;
				/* run */
				case 3:
					debug_command_run();
					return;
				/* step */
				case 4:
					debug_command_step();
					return;
			}
		}
	}

	printf("Error: Command not found\n");
	printf("Error: Command = %s\n", debug_tokens[0]);
}

void
debug_cleanup()
{
	int i;

	for (i = 0; i < max_argc; i++) {
		xmemset(debug_tokens[i], 0, max_argv * sizeof(char));
	}

	xmemset(line.line, 0, max_argv * sizeof(char));
}

void
debug_prompt()
{
	printf("(deb) ");
}

void
debug_read()
{
	debug_prompt();

	debug_cleanup();

	parse(stdin, line.line, debug_tokens, 256);

	debug_parse();
}

void
debug()
{
	while (debugging) {
		if (start_address[0] == 32768) {
			printf("Error: No Object File has been Loaded\n");
			break;
		} else {
			debug_read();
		}
	}
}

void
debug_init()
{
	int i;

	debug_tokens = (char **) xmalloc(max_argc * sizeof(char **));

	for (i = 0; i < max_argc; i++) {
		debug_tokens[i] = (char *) xmalloc(max_argv * sizeof(char));
	}

	debugging = 1;
}
