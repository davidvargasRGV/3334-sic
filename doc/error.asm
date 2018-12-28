COPY     START    100X                      ILLEGAL OPERAND IN START STATEMENT
         START    1000                      DUPLICATE OR MISPLACED START DIRECTIVE
FIRST    STL      RETADR
CLOOP    JSUB     RDREC
         LDA      LENGTH
         CMP      ZERO                      ILLEGAL FORMAT IN OPERATION FIELD
         JEQ      ENDFI                     UNDEFINED SYMBOL IN OPERAND
         JSUB
         J        CLOOP
ENDFIL   LDA      EOF
         STA      BUFFER
         LDA      THREE                     NEXT LINE IS EMPTY

         JSUB     WRREC
         LDL      RETADR
         RSUB
EOF      BITE     C'EOF'                    ILLEGAL FORMAT IN OPERATION FIELD
EOF      BYTE     C'EOF'                    DUPLICATE LABEL DEFINITION
THREE    WORD     3X                        MISSING OR MISPLACED MNEMONIC ON NEXT LINE
ZERO     
RETADR   RESWS    1                         ILLEGAL FORMAT IN OPERATION FIELD
LENGTH   RESW     1                         MISSING OR MISPLACED OPERAND IN NEXT LINE
TEST     RESW     1X
TEST2    RESB     1Z
BUFFER   RESB     
.
.        SUBROUTINE TO READ INTO BUFFER
.
RDREC    LDX      ZERO
         LDA      ZERO
RLOOP    TD       INPUT
         JEQ      RLOOP
         RD       INPUT
         COMP     ZERO
         JEQ      EXIT
         STCH     BUFFER,Z                  UNDEFINED SYMBOL IN OPERAND
         TIX      MAXLEN
         JLT      RLOOP
EXIT     STX      LENGTH
         RSUB
INPUT    BYTE     X'F16'                    ODD LENGTH HEX STRING IN BYTE STATEMENT
MAXLEN   WORD     4096
.
.        SUBROUTINE TO WRITE RECORD FROM BUFFER
.
WRREC    LDX      ZERO
WLOOP    TD       OUTPUT
         JEQ      WLOOP
         LDCH     BUFFER,X
         WD       OUTPUT                    MISSING OPERAND
         TIX
         JLT      WLOOP
         RSUB                               ILLEGAL FORMAT IN BYTE STATEMENT ON NEXT LINE
OUTPUT   BYTE     X'0X'                     MISPLACED OR MISSING OPERAND IN END STATEMENT ON NEXT LINE
         END
