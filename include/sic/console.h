/*
 * include/sic/console.h
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#ifndef _CONSOLE_H
#define _CONSOLE_H

char **console_tokens;

void console_init();
void console_prompt();
void console_read();
void console_parse();
void console_cleanup();
void console_deinit();

#endif
