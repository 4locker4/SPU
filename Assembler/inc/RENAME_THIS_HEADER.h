#ifndef RENAME_THIS_HEADER
#define RENAME_THIS_HEADER

#include "Assembler.h"

int StrParser           (ASSEMBLER * stk);
int FileWriter          (ASSEMBLER * stk);
int AssemblerDump       (ASSEMBLER * stk, char * text);
int GetReg              (char * reg);
int ElemCounter         (const char * fileFirstElement);





#endif