#ifndef RENAME_THIS_HEADER
#define RENAME_THIS_HEADER

int InputFromFile       (ASSEMBLER * stk);
int CodeConverter       (ASSEMBLER * stk);
int SubStingsMaker      (char * text, ASSEMBLER * stk);                                     // Splits strings into substings


int FileWriter          (ASSEMBLER * stk);
int AssemblerDump       (ASSEMBLER * stk);
int PushParser          (char * str, CMD * command);
int PopParser           (char * str, CMD * command);
int LabelFromJMP        (ASSEMBLER * stk);
int MemManipulation     (REALLOC_MODE flag);
int LabelCleaner        (LABEL_DATA * label);
int LabelFromLabel      (ASSEMBLER * stk);


#endif