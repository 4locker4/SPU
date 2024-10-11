#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "../Stack/inc/Stack.h"
#include "../Stack/inc/Defines.h"

//------------------------------------- TYPEDEF ------------------------------------


//-------------------------------------  CONST  ------------------------------------

const int STACK_SIZE = 8;

const char * ASSEMBLER_FILE = "assembler.txt";

//-------------------------------------  ENUM   -------------------------------------

enum PROGRAMMS
{
    push,
    add, 
    sub,
    mul,
    div,

    out,
    in,

    square,
    sinus,
    cosinus,

    dump,
    hlt = -1
};
//------------------------------------- STRUCT ------------------------------------

typedef struct
{
    stackElem * code   = NULL;
    int         nElems = 0;
} Code_stk;


#endif //ASSEMBLER_H