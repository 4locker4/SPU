#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <strings.h>
#include "C:\Users\eremc\.vscode\Tests\TX\TXLib.h"


#include "../../Stack/inc/Stack.h"

//------------------------------------- TYPEDEF ------------------------------------

typedef int codeElem;

//-------------------------------------  CONST  ------------------------------------

const int      STACK_SIZE       = 8;

const int      MASK_COMMAND     = 0x0F;

const int      MASK_REGISTER    = 0x10;

const codeElem VERSION          = 1;

const codeElem CREATORS_NAME    = 0xE1DA;

const char *   LOG_FILE         = "../../Debug/Log.txt";

const char *   READ_FILE        = "../Assembler/Read.txt";

const char *   ASSEMBLED_FILE_D = "../../Debug/AssembledDebugV.txt";

const char *   ASSEMBLER_FILE   = "../Assembler/Assembler.txt";

const char *   RESULT_FILE      = "../../SPU/Results.txt";

const char *   SPU_DEBUG_FILE   = "../Debug/SPUDebugFile.txt";

const int      QUANTITY_OF_REG  = 5;

const size_t   SIZE_OF_RAM      = 512;

//-------------------------------------  ENUM   -------------------------------------

enum PROGRAMMS
{
    HLT,

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

    DUMP
};

enum REGISTERS_INT_VAL
{
    HAVE_NUM = 0x20,
    HAVE_REG = 0x40,
    HAVE_RAM = 0x80,
};

enum REGISTERS
{
    NX = 0,
    AX,
    BX,
    CX,
    DX
};

//------------------------------------- STRUCT ------------------------------------

typedef struct
{
    codeElem creator_name = CREATORS_NAME;
    codeElem version      = VERSION;

    const char * log_file       = LOG_FILE;
    const char * read_file      = READ_FILE;
    const char * assembler_file = ASSEMBLER_FILE;

    codeElem * code = NULL;

    int n_elems = 0;
    int ip      = 0;
} ASSEMBLER;

typedef struct
{
    codeElem creator_name = CREATORS_NAME;
    codeElem version      = VERSION;

    const char * log_file       = LOG_FILE;
    const char * read_file      = READ_FILE;
    const char * assembler_file = ASSEMBLER_FILE;

    codeElem * code = NULL;
    codeElem * reg  = NULL;
    codeElem * ram  = NULL;

    int ip      = 0;
    int n_elems = 0;

} SPU;

//------------------------------------- DEFINE ------------------------------------

#define str_ncase_cmp(text, word)                               \
    else if (!strncasecmp (text, #word, sizeof (#word) - 1))    \
    {                                                           \
        stk->code[stk->ip++] = word;                            \
        text += sizeof (#word) + 1;                             \
        COLOR_PRINT(MANGETA, "%d - strcmp %s\n", stk->code[stk->ip], #word);\
    }

#endif //ASSEMBLER_H