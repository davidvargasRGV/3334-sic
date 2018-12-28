#
# Sic Assembler Makefile
#
# David Vargas
# CSCI 3334-01 Systems Programming
# Professor David Egle
# University of Texas Pan American
#

CC = gcc

CFLAGS = -ansi -Wall -I./include

all: sic
	@echo "Built SIC Assembler"

sic: main.o console.o error.o xmalloc.o parse.o execute.o ctype.o assemble.o opcode.o symbol.o dump.o load.o debug.o sicengine.o
	$(CC) $(CFLAGS) -o ./bin/sic ./bin/main.o ./bin/console.o ./bin/error.o ./bin/xmalloc.o ./bin/parse.o ./bin/ctype.o ./bin/execute.o ./bin/assemble.o ./bin/opcode.o ./bin/symbol.o ./bin/dump.o ./bin/load.o ./bin/debug.o ./bin/sicengine.o

main.o: ./src/main.c
	$(CC) $(CFLAGS) -o ./bin/main.o -c ./src/main.c

console.o: ./src/console.c
	$(CC) $(CFLAGS) -o ./bin/console.o -c ./src/console.c

error.o: ./src/error.c
	$(CC) $(CFLAGS) -o ./bin/error.o -c ./src/error.c

xmalloc.o: ./src/xmalloc.c
	$(CC) $(CFLAGS) -o ./bin/xmalloc.o -c ./src/xmalloc.c

parse.o: ./src/parse.c
	$(CC) $(CFLAGS) -o ./bin/parse.o -c ./src/parse.c

ctype.o: ./src/ctype.c
	$(CC) $(CFLAGS) -o ./bin/ctype.o -c ./src/ctype.c

execute.o: ./src/execute.c
	$(CC) $(CFLAGS) -o ./bin/execute.o -c ./src/execute.c

assemble.o: ./src/assemble.c
	$(CC) $(CFLAGS) -o ./bin/assemble.o -c ./src/assemble.c

opcode.o: ./src/opcode.c
	$(CC) $(CFLAGS) -o ./bin/opcode.o -c ./src/opcode.c

symbol.o: ./src/symbol.c
	$(CC) $(CFLAGS) -o ./bin/symbol.o -c ./src/symbol.c

dump.o: ./src/dump.c
	$(CC) $(CFLAGS) -o ./bin/dump.o -c ./src/dump.c

load.o: ./src/load.c
	$(CC) $(CFLAGS) -o ./bin/load.o -c ./src/load.c

debug.o: ./src/debug.c
	$(CC) $(CFLAGS) -o ./bin/debug.o -c ./src/debug.c

sicengine.o: ./src/sicengine.c
	$(CC) -Wno-format -o ./bin/sicengine.o -c ./src/sicengine.c

clean:
	rm -f ./bin/*.o
	rm -f ./bin/sic
