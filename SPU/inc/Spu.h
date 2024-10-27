#ifndef SPU_H
#define SPU_H

#include "../../Includes/Aggregate.h"

const char *   RESULT_FILE      = "../../SPU/Results.txt";

const char *   SPU_DEBUG_FILE   = "../Debug/SPUDebugFile.txt";

const char *   LOG_FILE         = "../Debug/Log.txt";

const size_t   SIZE_OF_RAM      = 512;

typedef struct
{
    codeElem creator_name = CREATORS_NAME;
    codeElem version      = VERSION;

    const char * log_file       = LOG_FILE;
    const char * assembler_file = ASSEMBLER_FILE;

    codeElem * code = NULL;
    codeElem * reg  = NULL;
    codeElem * ram  = NULL;

    int ip      = 0;
    int n_elems = 0;

} SPU;

#endif