/*
 * include/sic/execute.h
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#ifndef _EXECUTE_H
#define _EXECUTE_H

extern int status;

void execute_assemble(char **console_tokens);
void execute_debug();
void execute_directory();
void execute_dump(char **console_tokens);
void execute_execute();
void execute_exit(char **console_tokens);
void execute_help();
void execute_load(char **console_tokens);

#endif
