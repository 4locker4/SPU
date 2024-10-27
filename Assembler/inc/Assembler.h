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
    int    cmd_code  = NCMD;
    size_t cmd_len   = 0;

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

//TODO sizet
    int n_elems   = 0;
    int n_strings = 0;
    int ip        = 0;
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

//------------------------------------- DEFINE ------------------------------------

#define str_ncase_cmp(text, word)                               \
    else if (!strncasecmp (text, #word, strlen (#word)))        \
    {                                                           \
        stk->code->cmd_code  = word;                            \
        stk->code->cmd_start = text;                            \
        stk->code->cmd_len   = strlen (text);                   \
                                                                \
        stk->n_elems += 1;                                      \
        stk->text += stk->code->cmd_len;                        \
        stk->code++;                                            \
                                                                \
        COLOR_PRINT(MANGETA, "%d - strcmp %s\n", stk->code[stk->ip], #word);\
    }

#endif //ASSEMBLER_H