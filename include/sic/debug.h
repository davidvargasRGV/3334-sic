/*
 * include/sic/debug.h
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#ifndef _DEBUG_H
#define _DEBUG_H

char **debug_tokens;

int debugging;

void debug_init();
void debug();
void debug_read();
void debug_prompt();
void debug_parse();
void debug_deinit();

#endif
