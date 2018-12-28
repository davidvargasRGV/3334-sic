/*
 * src/execute.c
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sic/execute.h>
#include <sic/ctype.h>
#include <sic/assemble.h>
#include <sic/xmalloc.h>
#include <sic/sicdef.h>
#include <sic/sic.h>
#include <sic/dump.h>
#include <sic/load.h>
#include <sic/debug.h>

void
execute_load(char **console_tokens)
{
	FILE *fp;

	if (console_tokens[1] == NULL) {
		printf("Error: Missing parameters\n");
		return;
	}

	fp = fopen(console_tokens[1], "r");

	if (fp) {
		load(fp);
	} else {
		printf("Error: File not found\n");
		return;
	}

	fclose(fp);
}

void
execute_help()
{
	printf("Usage: (sic) [commands] [options]...\n");
	printf("Commands:\n");
	printf("  assemble <file>\tAssemble a SIC assembly language program\n");
	printf("  debug\t\t\tExecute in debug mode\n");
	printf("  directory\t\tList files stored in the current directory\n");
	printf("  dump <start> <end>\tDump contents of memory\n");
	printf("  execute\t\tExecute the program that was loaded into memory\n");
	printf("  help\t\t\tDisplay this help message\n");
	printf("  load <file>\t\tLoad the specified file into memory\n");
}

void
execute_exit(char **console_tokens)
{
	int i;

	for (i = 0; i < max_argc; i++) {
		xfree(console_tokens[i]);
	}

	xfree(console_tokens);

	status = 0;
}

void
execute_execute()
{
	if (start_address[0] == 32768) {
		printf("Error: No Object File has been Loaded\n");
	} else {
		SICRun(start_address, 0);
	}
}

void
execute_dump(char **console_tokens)
{
	if (console_tokens[2] == NULL) {
		printf("Error: Missing parameters\n\n");
		return;
	}

	/* check argv[1] and argv[2]: are they valid hex values? */
	if (!isvalid(console_tokens[1]) || !isvalid(console_tokens[2])) {
		printf("Error: Invalid parameter\n");
		return;
	}

	/* check argv[1] and argv[2]: are they within memory boundaries? */
	if (stoh(console_tokens[1]) > 32767 || stoh(console_tokens[2]) > 32768) {
		printf ( "Error: Invalid parameter\n" );
		return;
	}

	dump(console_tokens[1], console_tokens[2]);
}

void
execute_directory()
{
	system("ls -l");
}

void
execute_debug()
{
	debugging = 1;

	debug();
}

void
execute_assemble(char **console_tokens)
{
	FILE *fp;

	if (console_tokens[1] == NULL) {
		printf("Error: Missing parameters\n");
		return;
	}

	fp = fopen(console_tokens[1] , "r");

	if (fp) {
		assemble(fp);
	} else {
		printf("Error: File not found\n");
		return;
	}

	fclose(fp);
}
