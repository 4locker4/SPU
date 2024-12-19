#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../../Includes/Aggregate.h"
#include "Utils.h"

//-------------------------------------  CONST  ------------------------------------

const int      REALLOC_SIZE      = 8;

//------------------------------------- STRUCT ------------------------------------
typedef struct
{
    char *   cmd_start  = NULL;

    size_t   offset     =  0;
    int      cmd_code   = -1;
    size_t   cmd_len    =  0;
    codeElem arg        =  0;
    codeElem reg        =  0;
} CMD;

typedef struct
{
    codeElem ip_to_jmp = -1;

    char *   name      = NULL;
} LABEL_DATA;

typedef struct
{
    size_t free_cell         = 0;
    size_t capacity          = 0;             // TODO rename capacity

    LABEL_DATA * label_array = NULL;
} LABEL_STRUCT;

typedef struct
{
    codeElem creator_name = CREATORS_NAME;
    codeElem version      = VERSION;

    const char * read_file      = "../Codes/Read.txt";
    const char * assembler_file = "../Codes/RES.txt";
    const char * assembler_bin  = ASSEMBLER_BIN_F;

    CMD  * code = NULL;
    char * text = NULL;
    codeElem * buffer = NULL;

    LABEL_STRUCT labels = {};
    
    size_t file_size = 0;
    size_t n_elems   = 0;
    size_t n_cmd     = 0;
    size_t ip        = 0;
} ASSEMBLER;


int InputFromFile       (ASSEMBLER * stk);
int CodeConverter       (ASSEMBLER * stk);
void SubStringsMaker      (ASSEMBLER * stk);                                     // Splits strings into substings
char * DelSeveralSpacesAndComms (char * text, size_t text_size);

int FileWriter          (ASSEMBLER * stk);
int AssemblerDump       (ASSEMBLER * stk);

int GetLabel            (ASSEMBLER * asm_st);
int GetReg              (ASSEMBLER * asm_st);
int GetArg              (ASSEMBLER * asm_st);
int AddLabel            (ASSEMBLER * asm_st);
void ReadElemNum         (ASSEMBLER * asm_st);
int BufferWriter        (ASSEMBLER * asm_st, int * buf_ip);
int ArgsParser          (ASSEMBLER * asm_st);
int AsmCtor             (ASSEMBLER * asm_st);





#endif //ASSEMBLER_H