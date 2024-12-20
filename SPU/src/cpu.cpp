#include "../inc/Spu.h"
#include "../inc/FuncDeclar.h"

const char *   RESULT_FILE      = "../../Codes/Results.txt";

const char *   SPU_DEBUG_FILE   = "../Debug/SPUDebugFile.txt";

const char *   LOG_FILE         = "../Debug/Log.txt";

static int RunDump (SPU * processor);

int main ()
{
    SPU processor = {};

    SPUCtor (&processor);
    
    CodeReader (&processor);

    Run (&processor);

    return 0;
}

int SPUCtor (SPU * processor)
{
    my_assert (processor);

    processor->creator_name     = CREATORS_NAME;

    processor->version          = VERSION;

    processor->assembler_file   = ASSEMBLER_FILE;

    processor->log_file         = LOG_FILE;

    processor->reg = (codeElem *) calloc (QUANTITY_OF_REG, sizeof (codeElem));
    my_assert (processor->reg);

    processor->ram = (codeElem *) calloc (SIZE_OF_RAM, sizeof (codeElem));
    my_assert (processor->ram);

    return 0;
}

int CodeReader (SPU * processor)
{
    char * pointer_to_text = FileToStr (processor->assembler_file, &processor->file_size);      // Take string with all file`s data 

    processor->code = (codeElem *) calloc (processor->n_elems, sizeof (codeElem));              // Allocate memory for array of structs with code datas
    my_assert (processor->code);    

    codeElem check_creators_name = 0;                                                           // Here will be creator`s name, read from file
    codeElem check_version       = 0;                                                           // Here will be version of compiler, read from file

    int offset = 0;                                                                             // Offset relatively start of text

    my_assert(sscanf (pointer_to_text, "%d %d %d\n", 
                    &check_creators_name, &check_version, &processor->n_elems) == 3);           // Here we passing the value to the variable offset!

    while (*pointer_to_text != '\n')
        pointer_to_text += 1;
    
    pointer_to_text++;

    printf ("%s", pointer_to_text);

// DEBUG
    COLOR_PRINT(GREEN, "CREATORS NAMME: %d      VERSION: %d\n", check_creators_name, check_version);
    COLOR_PRINT(RED,   "CREATORS NAMME: %d      VERSION: %d\n", processor->creator_name, processor->version);
    COLOR_PRINT(RED,   "n_elements %d\n",                       processor->n_elems);
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

int Run (SPU * processor)
{
    Stack_t stk = {};                                                                       // Init stack of args
    Stack_t ret_addresses = {};                                                             // Init stack of ret ips
    
    StackCtor (&stk, STACK_SIZE);                                                           // Create stack with args

    StackCtor (&ret_addresses, STANDART_SIZE);                                              // Create stack with ret ips

    for ( ; processor->n_elems > processor->ip; processor->ip++)
    {
        RunDump (processor);

        switch (processor->code[processor->ip] & MASK_COMMAND)                              // Switch to command
        {
            case (PUSH):
            { 
                int a = GetArgPush (processor);

                StackPush (&stk, a);

                StackDump (&stk, 0, "PUSH", __FILE__, __LINE__);

                break;
            }
            case (POP):
            {
                RunDump (processor);

                *GetArgPop(processor) = StackPop (&stk);

                StackDump (&stk, 0, "POP", __FILE__, __LINE__);

                break;
            }
            case (JA):
            {
                RunDump (processor);

                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                if (a > b)
                    processor->ip = processor->code[++processor->ip] - 1;
                else
                    processor->ip++;
                
                break;                
            }
            case (JB):
            {
                RunDump (processor);

                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                if (a < b)
                    processor->ip = processor->code[++processor->ip] - 1;
                else
                    processor->ip++;

                break;                
            }
            case (JBE):
            {
                RunDump (processor);

                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                if (a <= b)
                    processor->ip = processor->code[++processor->ip] - 1;
                else
                    processor->ip++;

                break;
            }
            case (JAE):
            {
                RunDump (processor);

                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                if (a >= b)
                    processor->ip = processor->code[++processor->ip] - 1;
                else
                    processor->ip++;

                break;
            }
            case (JE):
            {
                RunDump (processor);

                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                if (a == b)
                    processor->ip = processor->code[++processor->ip] - 1;
                else
                    processor->ip++;

                break;
            }
            case (JMP):
            {
                RunDump (processor);

                processor->ip = processor->code[++processor->ip] - 1;

                break;
            }
            case (CALL):
            {
#define PUSH_RET_OP(stk, arg) StackPush (stk, arg);

                PUSH_RET_OP (&ret_addresses, processor->ip + 1)

#undef PUSH_RET_OP

                processor->ip = processor->code[++processor->ip] - 1;

                break;
            }
            case (RET):
            {
#define POP_RET_OP(stk) StackPop (stk)

                processor->ip = POP_RET_OP (&ret_addresses);

#undef POP_RET_OP

                break;
            }
            case (ADD):
            {
                RunDump (processor);

                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                StackPush (&stk, a + b);
                StackDump (&stk, 0, "ADD", __FILE__, __LINE__);

                break;
            }
            case (SUB):
            {
                RunDump (processor);

                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                StackPush (&stk, b - a);
                StackDump (&stk, 0, "SUB", __FILE__, __LINE__);

                break;
            }
            case (MUL):
            {
                RunDump (processor);

                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                StackPush (&stk, a * b);
                StackDump (&stk, 0, "MUL", __FILE__, __LINE__);

                break;
            }
            case (DIV):
            {
                RunDump (processor);

                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                StackPush (&stk, b / a);
                StackDump (&stk, 0, "DIV", __FILE__, __LINE__);


                break;
            }
            case (OUTP):
            {
                RunDump (processor);

                codeElem myu = StackPop (&stk);

                for (int i = 0; myu > i; i++)
                    printf ("MIAU\n");

                COLOR_PRINT (BLUE, "Output val: <%d>\n", myu);

                break;
            }
            case (INP):
            {
                RunDump (processor);

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

                stackElem a = StackPop (&stk);

                StackPush (&stk, sqrt (a));

                break;
            }
            case (SIN):
            {
                stackElem a = StackPop (&stk);

                StackPush (&stk, sin (a));

                break;
            }
            case (COS):
            {
                stackElem a = StackPop (&stk);

                StackPush (&stk, cos (a));

                break;
            }
            case (DUMP):
            {
                StackDump (&stk, 0, "stk", __FILE__, __LINE__);

                break;
            }
            case (HLT):
            {
                RunDump (processor);

                StackDump (&stk, 0, "HLT", __FILE__, __LINE__);
                StackDtor (&stk);

                return 0;
            }
            default:
            {
                COLOR_PRINT (RED, "You entered wrong data\n");
                return -1;
            }
        }
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

    size_t   offset    = 0;
    codeElem arg_type  = processor->code[processor->ip++];

    codeElem arg_value = 0;

    if (arg_type & HAVE_REG)
    {    
        arg_value += processor->reg[processor->code[processor->ip]];
        offset++;
    }

    if (arg_type & HAVE_ARG)
        arg_value += processor->code[processor->ip + offset];
    else
        offset--;

    if (arg_type & HAVE_RAM)
        arg_value = processor->ram[arg_value];

    processor->ip += offset;

    return arg_value;
}

codeElem * GetArgPop (SPU * processor)
{
    my_assert(processor);
    
    codeElem   arg_type  = processor->code[processor->ip++];
    codeElem * arg_value = NULL;

    int offset = 0;

    if (arg_type & HAVE_REG && arg_type & HAVE_ARG)
    {
        arg_value = processor->reg + processor->code[processor->ip++];

        *arg_value += processor->code[processor->ip];
    }
    else if (arg_type & HAVE_REG)
        arg_value = processor->reg + processor->code[processor->ip];
    else if (arg_type & HAVE_ARG)
        *arg_value = processor->code[processor->ip];

    if (arg_type & HAVE_RAM)
        arg_value = &(processor->ram[*arg_value]);
    
    return arg_value;
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