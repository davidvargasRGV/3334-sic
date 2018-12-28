/*
 * include/sic/load.h
 *
 * David Vargas
 * CSCI 3334-01 Systems Programming
 * Professor David Egle
 * University of Texas Rio Grande Valley
 *
 */

#ifndef _LOAD_H
#define _LOAD_H

unsigned long start_address[1];

void load_init();
void load(FILE *fp);
void load_parse(FILE *fp);

#endif
