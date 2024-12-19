#ifndef SPU_H
#define SPU_H

#include "../../Includes/Aggregate.h"

const size_t   SIZE_OF_RAM      = 512;

typedef struct
{
    codeElem creator_name = NULL;
    codeElem version      = NULL;

    const char * log_file       = NULL;
    const char * assembler_file = NULL;

    codeElem * code  = NULL;
    codeElem * reg   = NULL;
    codeElem * ram   = NULL;

    size_t file_size = 0;
    size_t ip        = 0;
    size_t n_elems   = 0;

} SPU;

#endif