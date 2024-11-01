#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../../Includes/Aggregate.h"

//-------------------------------------  CONST  ------------------------------------

const char *   READ_FILE        = "../Assembler/Read.txt";

const char *   ASM_DEBUG        = "../Debug/AssembledDebugV.txt";

const int      REALLOC_SIZE      = 8;

//------------------------------------- STRUCT ------------------------------------
typedef struct
{
    int    cmd_code  = -1;

    char * cmd_start = NULL;

    codeElem arg = 0;
    codeElem reg = 0;
} CMD;

typedef struct
{
    codeElem creator_name = CREATORS_NAME;
    codeElem version      = VERSION;

    const char * read_file      = READ_FILE;
    const char * assembler_file = ASSEMBLER_FILE;
    const char * assembler_bin  = ASSEMBLER_BIN_F;

    CMD  * code = NULL;
    char * text = NULL;
    
    size_t file_size = 0;
    size_t n_elems   = 0;
    size_t n_strings = 0;
    size_t ip        = 0;
} ASSEMBLER;

typedef struct
{
    codeElem ip_to_jmp = -1;
    int      label_ip  = -1;
    char *   name      = NULL;
} LABEL_DATA;

typedef struct
{
    size_t free_cell         = 0;
    size_t array_size        = 0;             // TODO rename capacity
    LABEL_DATA * label_array = NULL;
} LABEL_STRUCT;



int InputFromFile       (ASSEMBLER * stk);
int CodeConverter       (ASSEMBLER * stk);
int SubStingsMaker      (ASSEMBLER * stk);                                     // Splits strings into substings

int FileWriter          (ASSEMBLER * stk);
int AssemblerDump       (ASSEMBLER * stk);
int PushParser          (char * str, CMD * command);
int PopParser           (char * str, CMD * command);
int LabelFromJMP        (ASSEMBLER * stk);
int MemManipulation     (REALLOC_MODE flag);
// int LabelCleaner        (LABEL_DATA * label);
int LabelFromLabel      (ASSEMBLER * stk);

#endif //ASSEMBLER_H