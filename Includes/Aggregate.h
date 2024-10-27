#ifndef AGGREGATE_H
#define AGGREGATE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <strings.h>
#include "C:\Users\eremc\.vscode\Tests\TX\TXLib.h"

#include "../Stack/inc/Stack.h"
#include "../Stack/inc/Utils.h"
#include "../Stack/inc/Errors.h"

//------------------------------------- TYPEDEF ------------------------------------

typedef int codeElem;

//-------------------------------------  CONST  ------------------------------------
const char *   ASSEMBLER_FILE   = "../Assembler/Assembler.txt";

const char *   ASSEMBLER_BIN_F  = "../Assembler/AssemblerBin.txt";

const int      QUANTITY_OF_REG  = 4;

const int      LEN_OF_REGISTER  = 3;

const int      STACK_SIZE       = 8;

const int      MASK_COMMAND     = 0x0F;

const int      MASK_REGISTER    = 0x10;

const codeElem VERSION          = 1;

const codeElem CREATORS_NAME    = 0xE1DA;

//-------------------------------------  ENUM   -------------------------------------

enum PROGRAMMS
{
    HLT = 0,

    PUSH,
    POP,
    ADD, 
    SUB,
    MUL,
    DIV,

    OUTP,
    INP,

    SQRT,
    SIN,
    COS,

    DUMP,

    JMP, 
    JA,
    JB,

    CALL,
    
    NCMD
};

enum REGISTERS_INT_VAL
{
    HAVE_ARG = 0x20,
    HAVE_REG = 0x40,
    HAVE_RAM = 0x80,
};

enum REGISTERS
{
    rax,
    rbx,
    rcx,
    rdx
};

#endif