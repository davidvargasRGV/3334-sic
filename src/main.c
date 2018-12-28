/*
 * src/main.c
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#include <stdio.h>

#include <sic/main.h>
#include <sic/console.h>
#include <sic/assemble.h>
#include <sic/sic.h>
#include <sic/load.h>
#include <sic/debug.h>

void
deinit()
{
	console_deinit();
	debug_deinit();
}

void
init()
{
	status = 1;

	console_init();
	load_init();
	debug_init();

	SICInit();
}

int
main(int argc, char **argv)
{
	init();

	while (status) {
		console_read();
	}

	deinit();

	return 0;
}
