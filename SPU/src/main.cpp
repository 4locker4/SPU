#include "../../Assembler/inc/Assembler.h"
#include "../inc/RENAME_ME_PLS.h"

static int RunDump (SPU * processor);

int main ()
{
    SPU processor = {};

    processor.reg = (codeElem *) calloc (QUANTITY_OF_REG, sizeof (codeElem));
    my_assert (processor.reg);

    processor.ram = (codeElem *) calloc (SIZE_OF_RAM, sizeof (codeElem));

    CodeReader (&processor);                                                                // Read assembler`s file

    printf ("start running\n");
    
    Run (&processor);                                                                       // Do code commands

    return 0;
}

int Run (SPU * processor)
{
    Stack_t stk = {};                                                                       // Init stack

// ДЕБАЖНЫЕ

    COLOR_PRINT(GREEN, "%d - capacity\t%d - size\n %p - data\n", stk.capacity, stk.size, &stk.data);

    printf ("ctor start\n");
    
// СTOR  
    StackCtor (&stk, STACK_SIZE);                                                           // Create stack
// CTRO_ENDS

    printf ("ctor done\n");

// КОНЕЦ ДЕБАЖНЫМ                                                                 

    //FILE * RES_FILE = fopen (RESULT_FILE, "a+");                                            // Open file with results of SPU
    while (true)
    {
        switch (processor->code[processor->ip] & MASK_COMMAND)                            // Switch to command
        {
            case (PUSH):
            { 
                COLOR_PRINT (GREEN, "\nPush\n\n");

                RunDump (processor);

                int a = GetArgPush (processor);
                COLOR_PRINT (STRANGE, "%d\n", a);
                StackPush (&stk, a);

                StackDump (&stk, 0, "PUSH", __FILE__, __LINE__);

                break;
            }
            case (POP):
            {
                COLOR_PRINT (GREEN, "\nPop\n\n");

                RunDump (processor);

                *GetArgPop(processor) = StackPop (&stk);

                StackDump (&stk, 0, "POP", __FILE__, __LINE__);

                break;
            }
            case (ADD):
            {
                RunDump (processor);

                COLOR_PRINT(GREEN, "ADD\n");                

                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                StackPush (&stk, a + b);
                StackDump (&stk, 0, "ADD", __FILE__, __LINE__);


                break;
            }
            case (SUB):
            {
                RunDump (processor);

                COLOR_PRINT(GREEN, "SUB\n");

                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                StackPush (&stk, b - a);
                StackDump (&stk, 0, "SUB", __FILE__, __LINE__);


                break;
            }
            case (MUL):
            {
                RunDump (processor);

                COLOR_PRINT(GREEN, "MUL\n");

                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                StackPush (&stk, a * b);
                StackDump (&stk, 0, "MUL", __FILE__, __LINE__);

                break;
            }
            case (DIV):
            {
                RunDump (processor);

                COLOR_PRINT(GREEN, "DIV\n");

                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                StackPush (&stk, a / b);
                StackDump (&stk, 0, "DIV", __FILE__, __LINE__);


                break;
            }
            case (OUTP):
            {
                RunDump (processor);

                COLOR_PRINT (BLUE, "Output val: <%d>\n", StackPop (&stk));

                break;
            }
            case (INP):
            {
                RunDump (processor);

                COLOR_PRINT(GREEN, "IN\n");

                stackElem a = 0;

                COLOR_PRINT (MANGETA, "Enter your num\n");
                scanf ("%d", &a);

                StackPush (&stk, a);
                StackDump (&stk, 0, "IN", __FILE__, __LINE__);

                break;
            }
            case (SQRT):
            {
                RunDump (processor);

                COLOR_PRINT(GREEN, "SQRT\n");

                stackElem a = StackPop (&stk);

                StackPush (&stk, sqrt (a));

                break;
            }
            case (SIN):
            {
                COLOR_PRINT(GREEN, "SIN\n");

                stackElem a = StackPop (&stk);

                StackPush (&stk, sin (a));

                break;
            }
            case (COS):
            {
                COLOR_PRINT(GREEN, "COS\n");

                stackElem a = StackPop (&stk);

                StackPush (&stk, cos (a));

                break;
            }
            case (DUMP):
            {
                COLOR_PRINT(GREEN, "DUMP\n");

                StackDump (&stk, 0, "stk", __FILE__, __LINE__);

                break;
            }
            case (HLT):
            {
                printf ("%d\n\n%d\n", processor->code[processor->ip] & MASK_COMMAND, processor->code[processor->ip]);
                RunDump (processor);

                COLOR_PRINT(GREEN, "HLT\n");

                StackDump (&stk, 0, "HLT", __FILE__, __LINE__);
                StackDtor (&stk);

                //my_assert(fclose (RES_FILE));

                return 0;
            }
            default:
            {
                COLOR_PRINT (RED, "You entered wrong data\n");
                return -1;
            }
        }

        processor->ip++;
    }
    return 0;
}

/**
 * @brief Return branch Register/Int/(Int+Register)
 * \param [in] processor Struct with SPU data
 */

int GetArgPush (SPU * processor)
{
    my_assert (processor);

    codeElem arg_type  = processor->code[processor->ip++];
    codeElem arg_value = 0;

    if (arg_type & HAVE_REG)
        arg_value += processor->reg[processor->code[processor->ip++]];

    if (arg_type & HAVE_NUM)
        arg_value = processor->code[processor->ip++];


    if (arg_type & HAVE_RAM)
        arg_value = processor->ram[arg_value];

    return arg_value;
}

codeElem * GetArgPop (SPU * processor)
{
    my_assert(processor);
    
    codeElem   arg_type  = processor->code[processor->ip++];
    codeElem * arg_value = 0;

    if (arg_type & HAVE_REG)
        arg_value = processor->reg + processor->code[processor->ip++];
    
    if (arg_type & HAVE_RAM)
        arg_value = processor->ram + *arg_value;

    return arg_value;
}

int CodeReader (SPU * processor)
{
    char * pointer_to_text = FileToStr (processor->assembler_file);                         // Take string with all file`s data 

    processor->code = (codeElem *) calloc (processor->n_elems, sizeof (codeElem));          // Allocate memory for array of structs with code datas
    my_assert (processor->code);

    codeElem check_creators_name = 0;                                                       // Here will be creator`s name, read from file
    codeElem check_version       = 0;                                                       // Here will be version of compiler, read from file

    int offset = 0;                                                                         // Offset relatively start of text


    my_assert(sscanf (pointer_to_text, "%d %d %d%n", 
                    &check_creators_name, &check_version, &processor->n_elems, &offset)
                                                                                    == 3); // Here we passing the value to the variable offset!
    printf ("%d - offset\n", offset);

    pointer_to_text += offset;

    printf ("%s", pointer_to_text);

// DEBUG
    COLOR_PRINT(GREEN, "CREATORS NAMME: %d      VERSION: %d\n", check_creators_name, check_version);
    COLOR_PRINT(RED, "CREATORS NAMME: %d      VERSION: %d\n", processor->creator_name, processor->version);
// END PRINTF

    my_assert (check_creators_name == processor->creator_name);
    my_assert (check_version       == processor->version);

    for (int i = 0; i < processor->n_elems; i++)
    {
        my_assert (sscanf (pointer_to_text, "%d%n", &processor->code[i], &offset));

        pointer_to_text += offset;
    }

    return 0;
}

int MakeRegisters (SPU * processor)
{
    my_assert (processor);

    processor->reg = (codeElem *) calloc (QUANTITY_OF_REG, sizeof (codeElem));

    return 0;
}

#define Dump_to_file(...) fprintf(dump_file, __VA_ARGS__);
int RunDump (SPU * processor)
{
    my_assert(processor->code != NULL);
    FILE * dump_file = fopen (SPU_DEBUG_FILE, "a+");

    if (processor->version != VERSION)
        Dump_to_file ("ERROR: Bad version\n");

    if (processor->creator_name != CREATORS_NAME)
        Dump_to_file ("ERROR: Another creator of this file\n");

    Dump_to_file ("IP: %d\tn_elem: %d\n", processor->ip, processor->n_elems);

    for (int i = 0; i < processor->n_elems; i++)
    {
        if ((processor->code[processor->ip] & MASK_COMMAND) == PUSH)
        {
            Dump_to_file ("%5d | %d\tPUSH: %d\t<<\n\t |\n", i, processor->code[i], processor->code[i + 1]);
        }
        else if (i == processor->ip)
            Dump_to_file ("%5d | %d\t<<\n\t |\n", i, processor->code[i])
        else
            Dump_to_file ("%5d | %d\n\t |\n", i, processor->code[i]);
    }
    
    my_assert (!fclose (dump_file));

    return 0;
}
#undef Dump_to_file