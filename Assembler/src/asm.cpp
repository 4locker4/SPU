#include "..\inc\Assembler.h"

const char *   ASM_DEBUG        = "../Debug/AssembledDebugV.txt";

int ReadElemDump (char a);
int SubDump (char * a);
int BufWriterDump (ASSEMBLER * asm_st);

int main ()
{
    ASSEMBLER asm_st = {};

    AsmCtor        (&asm_st);

    InputFromFile  (&asm_st);

    CodeConverter  (&asm_st);

    AddLabel       (&asm_st);

    FileWriter     (&asm_st);

    return 0;
}

int AsmCtor (ASSEMBLER * asm_st)
{
//  ASM_CONSTS
    asm_st->creator_name    = CREATORS_NAME;
    asm_st->version         = VERSION;

    asm_st->read_file       = "../Codes/Read.txt";
;
    asm_st->assembler_file  = ASSEMBLER_FILE;
    asm_st->assembler_bin   = ASSEMBLER_BIN_F;

//  LABEL STRUCT
    asm_st->labels.free_cell   = 0;
    asm_st->labels.capacity    = 0;

    asm_st->file_size = 0;
    asm_st->n_elems   = 0;
    asm_st->n_cmd     = 0;
    asm_st->ip        = 0;

    return 0;
}

int InputFromFile (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    asm_st->text = FileToStr (asm_st->read_file, &asm_st->file_size);

    asm_st->text = DelSeveralSpacesAndComms (asm_st->text, asm_st->file_size);

    ReadElemNum (asm_st);

    /*  SubStringMaker count strings too    */
    SubStringsMaker (asm_st);                                                   // Return string whitout any comments and every \n -> \0 
    
    return 0;
}   

int CodeConverter (ASSEMBLER * asm_st)
{
    my_assert(asm_st);

    asm_st->buffer = (codeElem *) calloc (asm_st->n_elems, sizeof (codeElem));

    int buffer_ip = 0;

    for (asm_st->ip = 0 ; asm_st->n_cmd > asm_st->ip; asm_st->ip++)
    {
        if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "push", sizeof ("push") - 1))
        {
            printf ("Pushed ip = %d\n", asm_st->ip);

            asm_st->code[asm_st->ip].cmd_code  = PUSH;

            asm_st->code[asm_st->ip].offset += sizeof ("push");

            ArgsParser (asm_st);
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "pop", sizeof ("pop") - 1))    
        {
            printf ("Pop\n");
            asm_st->code[asm_st->ip].cmd_code  = POP;

            asm_st->code[asm_st->ip].offset += sizeof ("pop");

            ArgsParser (asm_st);
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "add", sizeof ("add") - 1))        
        {              
            printf ("Added\n");

            asm_st->code[asm_st->ip].cmd_code  = ADD;  

            asm_st->code[asm_st->ip].offset += sizeof ("add");
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "mul", sizeof ("mul") - 1))        
        {
            printf ("Mul\n");

            asm_st->code[asm_st->ip].cmd_code  = MUL;

            asm_st->code[asm_st->ip].offset += sizeof ("mul");
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "sub", sizeof ("sub") - 1))        
        {
            asm_st->code[asm_st->ip].cmd_code  = SUB;

            asm_st->code[asm_st->ip].offset += sizeof ("sub");
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "div", sizeof ("div") - 1))
        {
            asm_st->code[asm_st->ip].cmd_code  = DIV;

            asm_st->code[asm_st->ip].offset += sizeof ("div");
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "outp", sizeof ("outp") - 1))        
        {
            printf ("outp\n");
            asm_st->code[asm_st->ip].cmd_code  = OUTP;

            asm_st->code[asm_st->ip].offset += sizeof ("outp");
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "inp", sizeof ("inp") - 1))        
        {
            printf ("inp\n");
            asm_st->code[asm_st->ip].cmd_code  = INP;

            asm_st->code[asm_st->ip].offset += sizeof ("inp");
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "sin", sizeof ("sin") - 1))        
        {
            asm_st->code[asm_st->ip].cmd_code  = SIN;

            asm_st->code[asm_st->ip].offset += sizeof ("sin");
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "cos", sizeof ("cos") - 1))        
        {
            asm_st->code[asm_st->ip].cmd_code  = COS;

            asm_st->code[asm_st->ip].offset += sizeof ("cos");
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "sqrt", sizeof ("sqrt") - 1))        
        {
            asm_st->code[asm_st->ip].cmd_code  = SQRT;

            asm_st->code[asm_st->ip].offset += sizeof ("sqrt");
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "dump", sizeof ("dump") - 1))        
        {
            asm_st->code[asm_st->ip].cmd_code  = DUMP;

            asm_st->code[asm_st->ip].offset += sizeof ("dump");
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "hlt", sizeof ("hlt") - 1))        
        {
            printf ("hlt\n");
            asm_st->code[asm_st->ip].cmd_code  = HLT;

            asm_st->code[asm_st->ip].offset += sizeof ("hlt");
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "jmp", sizeof ("jmp") - 1))
        {
            printf ("In jmp\n");
            asm_st->code[asm_st->ip].cmd_code = JMP + HAVE_ARG;

            asm_st->code[asm_st->ip].offset += sizeof ("jmp");

            ArgsParser (asm_st);
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "ja", sizeof ("ja") - 1))
        {
            printf ("In ja\n");
            asm_st->code[asm_st->ip].cmd_code  = JA + HAVE_ARG;

            asm_st->code[asm_st->ip].offset += sizeof ("ja");

            ArgsParser (asm_st);
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "jb", sizeof ("jb") - 1))
        {
            printf ("In jb\n");
            asm_st->code[asm_st->ip].cmd_code  = JB + HAVE_ARG;

            asm_st->code[asm_st->ip].offset += sizeof ("jb");

            ArgsParser( asm_st);
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "je", sizeof ("je") - 1))
        {
            printf ("In je\n");
            asm_st->code[asm_st->ip].cmd_code  = JE + HAVE_ARG;

            asm_st->code[asm_st->ip].offset += sizeof ("je");

            ArgsParser (asm_st);
        }       
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "jbe", sizeof ("jbe") - 1))
        {
            printf ("In jnb\n");
            asm_st->code[asm_st->ip].cmd_code  = JBE + HAVE_ARG;

            asm_st->code[asm_st->ip].offset += sizeof ("jbe");

            ArgsParser (asm_st);
        }        
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "jae", sizeof ("jae") - 1))
        {
            printf ("In jae\n");
            asm_st->code[asm_st->ip].cmd_code  = JAE + HAVE_ARG;

            asm_st->code[asm_st->ip].offset += sizeof ("jae");

            ArgsParser (asm_st);
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "call", sizeof ("call") - 1))
        {
            asm_st->code[asm_st->ip].cmd_code = CALL + HAVE_ARG;

            asm_st->code[asm_st->ip].offset += sizeof ("call");

            ArgsParser (asm_st);
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "ret", sizeof ("ret") - 1))
        {
            printf ("ret\n");
            asm_st->code[asm_st->ip].cmd_code = RET;

            asm_st->code[asm_st->ip].offset += sizeof ("ret");

            ArgsParser (asm_st);
        }
        else
        {
            COLOR_PRINT(RED, "Unknown command: %s\n", asm_st->code[asm_st->ip].cmd_start);

            exit (-1);
        }

        BufferWriter (asm_st, &buffer_ip);
    }

    printf ("end pars\n");
    return 0;
}

int BufferWriter (ASSEMBLER * asm_st, int * buf_ip)
{
    my_assert (asm_st);

    asm_st->buffer[*buf_ip++] = asm_st->code[asm_st->ip].cmd_code;

    if (asm_st->code[asm_st->ip].cmd_code & HAVE_REG)
        asm_st->buffer[*buf_ip++] = asm_st->code[asm_st->ip].reg;

    if (asm_st->code[asm_st->ip].cmd_code & HAVE_ARG)
        asm_st->buffer[*buf_ip++] = asm_st->code[asm_st->ip].arg;

    if (*(asm_st->code[asm_st->ip].cmd_start + asm_st->code[asm_st->ip].cmd_len - 2) == ':');

    BufWriterDump (asm_st);
    return 0;
}

int BufWriterDump (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    // FILE * debug = fopen (ASM_DEBUG, "a+");

    for (int i = 0; asm_st->n_elems > i; i++)
    {
        printf (" %d\n ", asm_st->buffer[i]);
    }

    // my_assert (fclose (debug));
    return 0;

}

// мох стать похожим меньше на цветок больше на мох японский сад промок воду пьёт зеленый мох неприметным ковром стелется в тени цветов древней их пестиков шипов мы все исчезнем но не мох

void ReadElemNum (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    for (int ip = 0; asm_st->file_size > ip; ip++)
    {
        if (asm_st->text[ip] == ';')
        {
            asm_st->n_cmd++;
            asm_st->n_elems++;
            
            while (asm_st->text[ip] != '\0')
                ip++;
        }
        else if (asm_st->text[ip] == '\0')
        {
            asm_st->n_cmd++;
            asm_st->n_elems++;
        }
        else if (asm_st->text[ip] == ':')
        {
            asm_st->labels.capacity++;

            ip++;
        }
        else if (asm_st->text[ip] == ' ' && asm_st->text[ip + 1] != '+')
            asm_st->n_elems++;
    }

    return;
}

char * DelSeveralSpacesAndComms (char * text, size_t text_size)
{
    my_assert (text);

    char * new_text = (char *) calloc (text_size, sizeof (char));
    my_assert (new_text);

    size_t ip = 0;

    if (! isspace (*text))
        new_text[ip++] = *text;

    text++;

    while (*text != '\0')
    {
        if (isspace (*text) && isspace (*(text - 1)))
        {
            text++;

            continue;
        }
        else if (*text == ';')
        {
            *(text - 1) = '\0';

            while (*text != '\n')
                text++;
            
            text++;

            continue;
        }

        if (*text == '\r')
            new_text[ip++] = '\0';
        else
            new_text[ip++] = *text;

        text++;
    }

    new_text = (char *) realloc (new_text, sizeof (char) * (ip));
    my_assert (new_text);

    new_text[ip] = '\0';

    return new_text;
}

// int ReadElemDump (char a)
// {
//     FILE * debug = fopen (ASM_DEBUG, "a+");

//     fprintf (debug, "%c", a);

//     my_assert (!fclose (debug));

//     return 0;
// }

//  SubStringMaker count strings too
void SubStringsMaker (ASSEMBLER * asm_st)                                                                                 // Splits strings into substings
{
    my_assert (asm_st);

    char * text = asm_st->text;

    asm_st->code = (CMD *) calloc (asm_st->n_cmd, sizeof (CMD));
    my_assert (asm_st->code);

    asm_st->labels.label_array = (LABEL_DATA *) calloc (asm_st->labels.capacity, sizeof (LABEL_DATA));
    my_assert (asm_st->labels.label_array);

    int cmd_len = 0;                                                                                       // Offset in text (not asm_st->text)

//  parsing initial string
    for (asm_st->ip = 0 ; asm_st->n_cmd > asm_st->ip; asm_st->ip++)
    {
        cmd_len = 0;

        asm_st->code[asm_st->ip].cmd_start = text;

        while (*text != '\0')
        {
            text++;
            cmd_len++;
        }

        if (*(text - 1) == ':')
        {
            asm_st->labels.label_array[asm_st->labels.free_cell++].name = asm_st->code[asm_st->ip].cmd_start;

            asm_st->labels.label_array[asm_st->labels.free_cell++].ip_to_jmp = asm_st->ip;

            asm_st->ip--;

            text++;

            continue;
        }

        asm_st->code[asm_st->ip].cmd_len = cmd_len;
    }
    
    return;
}

// int SubDump (char * a)
// {
//     FILE * debug = fopen (ASM_DEBUG, "a+");

//     fprintf (debug, "%s\n", a);

//     my_assert (!fclose (debug));

//     return 0;
// }

int FileWriter (ASSEMBLER * asm_st)
{
    printf ("writer\n");

    FILE * file_asm_st = fopen ("result.txt", "w");
    my_assert(file_asm_st != NULL);

    /*    Creator`s data writer    */
    printf ("creator\n");

    printf ("n_elems = %d\n", asm_st->n_elems);

    for (int i = 0; asm_st->n_elems > i; i++)
    {
        printf ("%d - elem\n,", asm_st->buffer[i]);
        fprintf (file_asm_st, "%d\n", asm_st->buffer[i]);
    }

    my_assert (!fclose (file_asm_st));

    // FILE * asm_st_bin = fopen (asm_st->assembler_bin, "ab");

    // fwrite (dupl_final_code, sizeof (codeElem), asm_st->n_elems, asm_st_bin);
    // my_assert (!fclose (asm_st_bin));

    return 0;
}

/*
push [10]   = 2
push 10     = 1

ArgumentParsing()


[ ]     not [ ]

GetArgument(..., )
    GetValue
    GetReg
    GetLabel
*/

int AddLabel (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    int n_elem = 0;

    for (int ip = 0; asm_st->n_cmd > ip; ip++)
    {
        n_elem++;
        COLOR_PRINT (GREEN, "command: %s\n", asm_st->code[ip].cmd_start);

        if (asm_st->code[ip].cmd_code & HAVE_ARG)
        {
            COLOR_PRINT (YELLOW, "arg: %d\n", asm_st->code[ip].arg);
            n_elem++;
        }

        if (asm_st->code[ip].cmd_code & HAVE_REG)
        {
            n_elem++;
            COLOR_PRINT (YELLOW, "reg: %d\n", asm_st->code[ip].reg);
        }

        if (*(asm_st->code[ip].cmd_start + asm_st->code[ip].cmd_len - 1) == ':')
        {
            for (int i = 0; asm_st->labels.capacity > i; i++)
            {
                if (!strncasecmp (asm_st->code[ip].cmd_start, asm_st->labels.label_array[i].name, asm_st->code[ip].cmd_len - 2))
                {
                    asm_st->code[asm_st->labels.label_array[i].ip_to_jmp].arg = n_elem;
                }
            }
        }
    }
    printf ("end add labels\n");
    return 0;
}

int ArgsParser (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    if (*(asm_st->code[asm_st->ip].cmd_start + asm_st->code[asm_st->ip].offset) == '[')
    {
        asm_st->code[asm_st->ip].cmd_code += HAVE_RAM;

        asm_st->code[asm_st->ip].offset += 1;
    }

    if (GetReg (asm_st))
        GetArg (asm_st);  
    else if (!GetArg (asm_st))
        GetLabel (asm_st);

    return 0;
}

int GetArg (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    if (*(asm_st->code[asm_st->ip].cmd_start + asm_st->code[asm_st->ip].offset) == '+')
        asm_st->code[asm_st->ip].offset += 2;

    // char * checker = asm_st->code[asm_st->ip].cmd_start + asm_st->code[asm_st->ip].offset;

    // asm_st->code[asm_st->ip].arg = strtod (asm_st->code[asm_st->ip].cmd_start + asm_st->code[asm_st->ip].offset, &checker);

    if (isdigit( *(asm_st->code[asm_st->ip].cmd_start + asm_st->code[asm_st->ip].offset)))
    {
        asm_st->code[asm_st->ip].arg = atoi (asm_st->code[asm_st->ip].cmd_start + asm_st->code[asm_st->ip].offset);

        asm_st->code[asm_st->ip].cmd_code += HAVE_ARG;

        return HAVE_ARG;
    }
    
    return 0;
}

int GetReg (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    if ('r' ==  (*(asm_st->code[asm_st->ip].cmd_start + asm_st->code[asm_st->ip].offset)) && 
        (*(asm_st->code[asm_st->ip].cmd_start + asm_st->code[asm_st->ip].offset + 2)) == 'x')
    {
        asm_st->code[asm_st->ip].cmd_code += HAVE_REG;

        asm_st->code[asm_st->ip].reg       = *(asm_st->code[asm_st->ip].cmd_start + asm_st->code[asm_st->ip].offset + 1) - 'a';

        asm_st->code[asm_st->ip].offset   += sizeof ("rax");

        return HAVE_REG;
    }

    return 0;
}

int GetLabel (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    asm_st->labels.label_array[asm_st->labels.free_cell].ip_to_jmp = asm_st->ip;
    asm_st->labels.label_array[asm_st->labels.free_cell++].name = asm_st->code[asm_st->ip].cmd_start;

    return 0;
}

int AssemblerDump (ASSEMBLER * asm_st)
{
    my_assert(&asm_st);

    FILE * dump_file = fopen (ASM_DEBUG, "a+");
    my_assert(dump_file);

    fprintf (dump_file, "-------------------------------------------------------------------------\n");
    fprintf (dump_file, "| %4d | %18s |  %8d  |  %8d  |  %4d  |  %4d  |\n", 
                         asm_st->ip, asm_st->code[asm_st->ip].cmd_start, asm_st->code[asm_st->ip].cmd_code, asm_st->code[asm_st->ip].arg, 
                         asm_st->code[asm_st->ip].cmd_code & HAVE_ARG, asm_st->code[asm_st->ip].cmd_code & HAVE_REG);

    my_assert(!fclose (dump_file));
    return 0;
}