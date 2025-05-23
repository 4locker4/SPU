#ifndef AGGREGATE_H
#define AGGREGATE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <strings.h>
#include "C:\Users\eremc\.vscode\Tests\TX\TXLib.h"

#include "../Stack/inc/Utils.h"
#include "../Stack/inc/Stack.h"

//------------------------------------- TYPEDEF ------------------------------------

typedef int codeElem;

//-------------------------------------  CONST  ------------------------------------
const char *   ASSEMBLER_FILE   = "../Codes/AssemblerBin.bin";

const char *   ASSEMBLER_BIN_F  = "../Codes/AssemblerBin.bin";

const int      QUANTITY_OF_REG  = 5;

const int      LEN_OF_REGISTER  = 3;

const int      STACK_SIZE       = 8;

const int      MASK_COMMAND     = 0x1F;

const codeElem VERSION          = 3;

const codeElem CREATORS_NAME    = 0xE1DA;

//-------------------------------------  ENUM   -------------------------------------

enum PROGRAMMS
{
    NOT_INITED = -1,
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
    JAE,
    JB,
    JBE,
    JE,
    JNE,

    CALL,
    RET,
    
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
    rdx,
    rex,
};

#endif