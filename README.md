# 3334-sic

This is the source code for the SIC Assembler assignment for CSCI 3334 Systems Programming at the University of Texas Rio Grande Valley.

The textbook for the course was **System Software: An Introduction to Systems Programming** by Leland L Beck.

## Building

A makefile is provided. Simply execute the following...

```
make
```

The following is the directory structure...

* bin - C object files and SIC Assembler binary
* doc - SIC assembly files and SIC intermediate files
* include/sic - C header files
* src - C source files

## Running

To execute the SIC Assembler, simply execute the following... 

```
./bin/sic
```

The following commands are accepted...

* *assemble <filename>*
* *debug*
* *directory*
* *dump <start address> <end address>*
* *execute*
* *help*
* *load <filename>*

**Note**: Debugging was extra credit and is currently incomplete.

## Example

To assemble and execute *sample.asm*, simply execute the following...

```
./bin/sic
assemble doc/sample.asm
load doc/object.sic
execute
exit
```

**Note**: Exit the assembler before verifying device input/output files.

## Authors

* David Vargas (*david.vargas.rgv [AT] gmail.com*)
* Professor David Egle

## License

All rights reserved.

## Acknowledgments

* Professor David Egle provides students with the sicengine.c and sic.h files.
