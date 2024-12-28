#include "..\inc\Assembler.h"

#define OFFSET_ asm_st->code[asm_st->ip].offset

const char *   ASM_DEBUG        = "../Debug/AssembledDebugV.txt";
const char *   TRYS             = "../Codes/Read.txt";
const char *   FACTORIAL        = "../Codes/Factorial.txt";
const char *   SQUARE_SOLV      = "../Codes/Square.txt";

int main ()
{
    ASSEMBLER asm_st = {};

    AsmCtor        (&asm_st);

    InputFromFile  (&asm_st);

    CodeConverter  (&asm_st);

    FileWriter     (&asm_st);

    return 0;
}

int AsmCtor (ASSEMBLER * asm_st)
{
//  ASM_CONSTS
    asm_st->creator_name    = CREATORS_NAME;
    asm_st->version         = VERSION;

    asm_st->read_file       = TRYS;

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

    asm_st->text = DelSeveralSpacesAndComms (asm_st, asm_st->text, asm_st->file_size);

    ReadElemNum (asm_st);

    SubStringsMaker (asm_st);

    return 0;
}

void ReadElemNum (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    for (int ip = 0; asm_st->file_size > ip; ip++)
    {
        if (asm_st->text[ip] == '\0')
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

char * DelSeveralSpacesAndComms (ASSEMBLER * asm_st, char * text, size_t text_size)
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
            new_text[ip - 1] = '\0';

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

    new_text = (char *) realloc (new_text, sizeof (char) * (ip + 1));
    my_assert (new_text);

    new_text[ip] = '\0';

    asm_st->file_size = ip;

    for (int i = 0; ip > i; i++)
    {
        printf ("%c", new_text[i]);
    }

    printf ("\n");

    return new_text;
}

void SubStringsMaker (ASSEMBLER * asm_st)                                                                                 // Splits strings into substings
{
    my_assert (asm_st);

    char * text = asm_st->text;

    asm_st->code = (CMD *) calloc (asm_st->n_cmd, sizeof (CMD));
    my_assert (asm_st->code);

    asm_st->labels.label_array = (LABEL_DATA *) calloc (asm_st->labels.capacity, sizeof (LABEL_DATA));
    my_assert (asm_st->labels.label_array);

    int cmd_len  = 0;                                                                                       // Offset in text (not asm_st->text)
    int cur_elem = 0;

    for (asm_st->ip = 0 ; asm_st->n_cmd > asm_st->ip; asm_st->ip++)
    {
        cmd_len = 0;

        asm_st->code[asm_st->ip].cmd_start = text;

        while (*text != '\0')
        {
            if (*text == ' ' && *(text + 1) != '+')
                cur_elem++;

            text++;
            cmd_len++;
        }

        if (*(text - 1) == ':')
        {
            asm_st->labels.label_array[asm_st->labels.free_cell].name = asm_st->code[asm_st->ip].cmd_start;

            asm_st->labels.label_array[asm_st->labels.free_cell++].ip_to_jmp = cur_elem;

            *(text - 1) = '\0';

            asm_st->ip--;

            text++;

            continue;
        }

        cur_elem++;

        asm_st->code[asm_st->ip].cmd_len = cmd_len;

        text++;
    }
    
    return;
}

#define  CMD_CODE_(name)                                                                                                    \
                    else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, #name, sizeof (#name) - 1))                  \
                    {                                                                                                       \
                        asm_st->code[asm_st->ip].cmd_code  = name;                                                          \
                                                                                                                            \
                        OFFSET_ = sizeof (#name);                                                                           \
                    }                                                                                                       \

#define JMP_CODE_(jmp_name)                                                                                                 \
                            else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, #jmp_name, sizeof (#jmp_name) - 1))  \
                            {                                                                                               \
                                asm_st->code[asm_st->ip].cmd_code = jmp_name + HAVE_ARG;                                    \
                                                                                                                            \
                                OFFSET_ = sizeof (#jmp_name);                                                               \
                                                                                                                            \
                                ArgsParser (asm_st);                                                                        \
                            }                                                                                               \

int CodeConverter (ASSEMBLER * asm_st)
{
    my_assert(asm_st);

    asm_st->buffer = (codeElem *) calloc (asm_st->n_elems, sizeof (codeElem));
    my_assert (asm_st->buffer);

    int buffer_ip = 0;

    for (asm_st->ip = 0 ; asm_st->n_cmd > asm_st->ip; asm_st->ip++)
    {
        if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "push", sizeof ("push") - 1))
        {
            asm_st->code[asm_st->ip].cmd_code  = PUSH;

            OFFSET_ = sizeof ("push");

            ArgsParser (asm_st);
        }
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "pop", sizeof ("pop") - 1))    
        {
            asm_st->code[asm_st->ip].cmd_code  = POP;

            OFFSET_ = sizeof ("pop");

            ArgsParser (asm_st);
        }

        #include "../../Includes/CmdCodeGen.h"
        #include "../../Includes/JmpCodeGen.h"
        
        else if (!strncasecmp (asm_st->code[asm_st->ip].cmd_start, "ret", sizeof ("ret") - 1))
        {
            asm_st->code[asm_st->ip].cmd_code = RET;

            OFFSET_ = sizeof ("ret");

            ArgsParser (asm_st);
        }
        else
        {
            COLOR_PRINT(RED, "Unknown command:%s\n", asm_st->code[asm_st->ip].cmd_start);

            exit (-1);
        }

        BufferWriter (asm_st, &buffer_ip);
    }

    return 0;
}

int FileWriter (ASSEMBLER * asm_st)
{
    FILE * file_asm_st = fopen (asm_st->assembler_file, "w+");
    my_assert(file_asm_st != NULL);

    fprintf (file_asm_st, "%d %d %d\n", asm_st->creator_name, asm_st->version, asm_st->n_elems);

    for (int i = 0; asm_st->n_elems > i; i++)
        fprintf (file_asm_st, "%d\n", asm_st->buffer[i]);

    my_assert (!fclose (file_asm_st));

    FILE * asm_st_bin = fopen (asm_st->assembler_bin, "ab");
    my_assert (asm_st_bin);

    fwrite (asm_st->buffer, sizeof (codeElem), asm_st->n_elems, asm_st_bin);
    my_assert (!fclose (asm_st_bin));

    return 0;
}

int BufferWriter (ASSEMBLER * asm_st, int * buf_ip)
{
    my_assert (asm_st);
    my_assert (buf_ip);

    asm_st->buffer[*buf_ip] = asm_st->code[asm_st->ip].cmd_code;

    *buf_ip += 1;

    if (asm_st->code[asm_st->ip].cmd_code & HAVE_REG)
    {
        asm_st->buffer[*buf_ip] = asm_st->code[asm_st->ip].reg;

        *buf_ip += 1;
    }

    if (asm_st->code[asm_st->ip].cmd_code & HAVE_ARG)
    {
        asm_st->buffer[*buf_ip] = asm_st->code[asm_st->ip].arg;

        *buf_ip += 1;
    }

    return 0;
}

// мох стать похожим меньше на цветок больше на мох японский сад промок воду пьёт зеленый мох неприметным ковром стелется в тени цветов древней их пестиков шипов мы все исчезнем но не мох

int ArgsParser (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    if (asm_st->code[asm_st->ip].cmd_start[OFFSET_] == '[')
    {
        asm_st->code[asm_st->ip].cmd_code += HAVE_RAM;

        OFFSET_ += 1;
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

    if (asm_st->code[asm_st->ip].cmd_start[OFFSET_] == '+')
        OFFSET_ += 2;

    if(*(asm_st->code[asm_st->ip].cmd_start + OFFSET_) == '-')
    {
        asm_st->code[asm_st->ip].arg = atoi (asm_st->code[asm_st->ip].cmd_start + OFFSET_ + 1);

        asm_st->code[asm_st->ip].arg *= -1;

        asm_st->code[asm_st->ip].cmd_code += HAVE_ARG;

        return HAVE_ARG;
    }
    else if (isdigit( *(asm_st->code[asm_st->ip].cmd_start + OFFSET_)))
    {
        asm_st->code[asm_st->ip].arg = atoi (asm_st->code[asm_st->ip].cmd_start + OFFSET_);

        asm_st->code[asm_st->ip].cmd_code += HAVE_ARG;

        return HAVE_ARG;
    }

    return 0;
}

int GetReg (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    if (('r' == asm_st->code[asm_st->ip].cmd_start[OFFSET_]) && 
        ('x' == asm_st->code[asm_st->ip].cmd_start[OFFSET_ + 2]))
    {
        asm_st->code[asm_st->ip].cmd_code += HAVE_REG;

        asm_st->code[asm_st->ip].reg       = *(asm_st->code[asm_st->ip].cmd_start + OFFSET_ + 1) - 'a';

        OFFSET_   += sizeof ("rax");

        return HAVE_REG;
    }

    return 0;
}

codeElem GetLabel (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    for (int i = 0; asm_st->labels.capacity > i; i++)
    {
        if (! strncmp (asm_st->labels.label_array[i].name, &asm_st->code[asm_st->ip].cmd_start[OFFSET_], asm_st->code[asm_st->ip].cmd_len - OFFSET_))
        {
            asm_st->code[asm_st->ip].arg = asm_st->labels.label_array[i].ip_to_jmp;

            return 0;
        }
    }

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