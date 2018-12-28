/*
 SIC simulator, version 1.5
 revised 10/27/89
 translated to C  Summer 1994  (David Egle - Univ Texas Pan American)

 This version of the SIC simulator includes all SIC/XE instructions
 and capabilities except for the following:

    1. floating-point data type and instructions (ADDF, COMPF, DIVF, FIX,
       FLOAT, LDF, MULF, NORM, STF, SUBF)
    2. i/o channels and associated instructions (SIO, TIO, HIO)
    3. interrupts and associated instructions (LPS, STI, SVC)
    4. certain features associated with register SW (user/supervisor modes,
       running/idle states)
    5. virtual memory and associated instructions (LPM)
    6. memory protection and associated instructions (SSK)

 For a simulator that supports only standard SIC features, set the
 global constant XE to FALSE.

 The simulator uses the following external files:

    Log     -- log of commands entered and results displayed (may be printed
               to obtain hard copy record of terminal session)
    DevBoot -- represents device 00 (bootstrap object code)
    Dev[6]  -- represent the input/output devices
               indices 0, 1, and 2 are the input devices, corresponding
                   to the files devf1, devf2, and devf3
               while 3, 4, and 5 are the output devices, corresponding
                   to the files dev04, dev05, dev06
*/

                /* Define a few constants */
#define TRUE    1                       /* Boolean constants */
#define FALSE   0
#define MSIZE   32768L

                /* Define some useful data types */
typedef unsigned char   BYTE;
typedef BYTE            WORD[3];
typedef BYTE            FLOAT[4];
typedef unsigned char   BOOLEAN;
typedef unsigned long   ADDRESS;

extern void GetMem (ADDRESS, BYTE*, int);
extern void PutMem (ADDRESS, BYTE*, int);
extern void GetReg (WORD*);
extern void PutReg (WORD*);
extern ADDRESS GetPC (void);
extern void GetIR (ADDRESS, char *);
extern char GetCC (void);
extern void PutPC (ADDRESS);
extern void SICInit (void);
extern void SICRun (ADDRESS *, BOOLEAN);
