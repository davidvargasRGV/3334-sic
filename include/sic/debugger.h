/*
 * include/sic/debugger.h
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#ifndef _DEBUGGER_H
#define _DEBUGGER_H

/* Names the values of the 'type' field of 'struct command_t' */
enum {
	no_argument = 0,
	required_argument = 1,
	optional_argument = 2
};

/* Describe the commands requested by the application

   The field 'type' is:
   no_argument       (or 0) if the command does not take an argument,
   required_argument (or 1) if the command requires an argument,
   optional_argument (or 2) if the command takes an optional argument.

   If the field 'type' is either required_argument or optional_argument,
   it should be set to the number of optional/required arguments,
   otherwise it should be set to 0.
 */
struct debug_command_t {
	const char *name;
	int type;
	int requirement;
};

/* available debugging commands */
struct debug_command_t debug_command[] = {
	{"exit", no_argument, 0},
	{"help", no_argument, 0},
	{"print", required_argument, 2},
	{"run" , no_argument, 0},
	{"step", no_argument, 0}
};

#endif
