/*
 * include/sic/ctype.h
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#ifndef _CTYPE_H
#define _CTYPE_H

int isspace(int c);
int isdelim(int c);
int isvalid(char *str);
int isvalidint(char *str);
int ishex(int c);
int isalpha(int c);
int isdigit(int c);
int stoh(char *str);
int stoi(char *str);

#endif
