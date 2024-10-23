#include "..\inc\Assembler.h"
#include "..\inc\RENAME_THIS_HEADER.h"

int main (int argc, char * argv[])
{
    ASSEMBLER stk = {};

    StrParser (&stk); 
    return 0;
}


int StrParser (ASSEMBLER * stk)
{
    my_assert(stk);

    char * text  = FileToStr      (stk->read_file);
    stk->n_elems = ElemCounter    (text);

    text         = SubStingsMaker (text);                                                       // Return string whitout any comments and every \n -> \0 
    stk->code = (codeElem *) calloc (stk->n_elems, sizeof (codeElem));
    my_assert (stk->code);

    while (true)
    {
        while (isspace (*text))
            text++;
        
        int offset = 0;
        
        AssemblerDump (stk, text);

        if (!strncasecmp (text, "push", sizeof ("push") - 1))
        {
            stk->code[stk->ip] = PUSH;
            text += sizeof ("push");

            codeElem num    = 0;
            char reg_str[3] = {};
            
            char * ram = strchr (text, '[');
            if (ram)                                                                                    // If we have ram
            {
                stk->code[stk->ip] += HAVE_RAM;

                text = ram + 1;                                                                         // Take addres with args or/and reg
            }

            if (sscanf (text, "%d%n", &num, &offset))                                                   // If only num we have
            {
                COLOR_PRINT (STRANGE, "num = %d\toffset = %d;\n", num, offset + 1);

                text += offset + 1;                                                                     // +1 -> Skip \0

                stk->code[stk->ip++] += HAVE_NUM;

                stk->code[stk->ip++] = num;

                COLOR_PRINT (BLUE, "NUM\n");
            }
            else if (sscanf (text, "%s%n", &reg_str, &offset))                                          // Read register
            {
                COLOR_PRINT (STRANGE, "reg = %d\toffset = %d;\n", reg_str, offset + 1);

                stk->code[stk->ip++] += HAVE_REG;

                COLOR_PRINT (BLUE, "REG = %s\n", reg_str);

                stk->code[stk->ip++] = GetReg (reg_str);

                text += offset + 1;                                                                     // +1 -> Skip space or \0

                if (sscanf (text, "%d%n", &num, &offset))                                               // If we got num, read it too
                {
                    COLOR_PRINT (STRANGE, "num = %d\toffset = %d;\n", num, offset + 1);
                    
                    stk->code[stk->ip - 2] += HAVE_NUM;

                    stk->code[stk->ip++] = num;

                    text += offset + 1;                                                                 // +1 -> Skip \0

                    COLOR_PRINT (BLUE, "NUM + REG\n");
                }
            }
            else
            {
                COLOR_PRINT (RED, "You entered wrong data\n");
                exit (-1);
            }
        }
        else if (!strncasecmp (text, "pop", sizeof ("pop") - 1))    
        {
            stk->code[stk->ip] = POP;
            text += sizeof ("pop");

            codeElem num   = 0;
            char reg_str[3] = {};

            char * ram = strchr (text, '[');
            if (ram)
            {
                stk->code[stk->ip] += HAVE_RAM;

                text = ram + 1;                
            }

            if (sscanf (text, "%s%n", &reg_str, &offset))
            {
                stk->code[stk->ip++] += HAVE_REG;
                text += offset;

                COLOR_PRINT (BLUE, "REG = %s\n", reg_str);

                stk->code[stk->ip++] = GetReg (reg_str);

                if (sscanf (text, "%d%n", &num, &offset))
                {
                    stk->code[stk->ip - 2] += HAVE_NUM;

                    stk->code[stk->ip++] = num;

                    text += offset;

                    COLOR_PRINT (BLUE, "NUM + REG\n");
                }
            }
            else
            {
                COLOR_PRINT (RED, "You entered wrong data in pop\n");
                exit (-2);
            }

            COLOR_PRINT(BLUE, "%d - popped with %s\n", stk->code[stk->ip], "pop");
        }
        str_ncase_cmp(text, ADD)
        str_ncase_cmp(text, MUL)
        str_ncase_cmp(text, SUB)
        str_ncase_cmp(text, DIV)
        str_ncase_cmp(text, OUTP)
        str_ncase_cmp(text, INP)
        str_ncase_cmp(text, SIN)
        str_ncase_cmp(text, COS)
        str_ncase_cmp(text, SQRT)
        str_ncase_cmp(text, DUMP)
        else if (!strncasecmp (text, "HLT", sizeof ("HLT")))
        {
            stk->code[stk->ip] = HLT;
            text += sizeof ("HLT");

            FileWriter (stk);
            AssemblerDump (stk, text);

            return 0;
        }
        else
        {
            COLOR_PRINT(RED, "Unknown command: %s\n", text + 1);
            exit (-1); 
        }

        COLOR_PRINT(GREEN, "Iteration done\n");
        COLOR_PRINT(MANGETA, "ip = %d\n", stk->ip);
    }

    return 0;
}

int FileWriter (ASSEMBLER * stk)
{
    FILE * file_asm = fopen (stk->assembler_file, "w+");
    my_assert(file_asm != NULL);

    fprintf (file_asm, "%d\n%d\n%d\n", CREATORS_NAME, VERSION, stk->ip);

    for (int i = 0; stk->ip > i; i++)
    {
        COLOR_PRINT(GREEN, "<%d>\n", stk->code[i]);
        fprintf (file_asm, "%d\n", stk->code[i]);
    }

    my_assert(!fclose (file_asm));

    return 0;
}

int AssemblerDump (ASSEMBLER * stk, char * text)
{
    my_assert(&stk);

    FILE * dump_file = fopen ("Dump.txt", "a+");
    my_assert(dump_file);

    fprintf (dump_file, "DUMP\n\n");

    fprintf (dump_file, "CREATOR: %x\n"
                        "VERSION: %d\n"
                        "n_elements = %d;    ip = %d\n", 
                        stk->creator_name, stk->version, stk->n_elems, stk->ip);
    for (int i = 0; stk->ip > i; i++)
    {
        fprintf (dump_file, "Code element №%d:  %d\n", i, stk->code[i]);
    }
    char * test = text;

    while (*test != '`')
    {
        if (*test == '\0')
            fprintf (dump_file, "\n");
        else
            fprintf (dump_file, "%c", *test);
        test++;
    }

    my_assert(!fclose (dump_file));
    return 0;
}

int GetReg (char * reg)
{
    my_assert (reg);

    COLOR_PRINT (YELLOW, "%s\n", reg);

    if ((int) reg[1] == 'X' && isalpha (reg[0]))
        return ((int) reg[0] - 'A' + 1);
    else 
    {
        COLOR_PRINT ("You entered wrong fata in registers\n");

        exit (-3);
    }
}

//TODO
int ElemCounter (const char * fileElement)
{
    int elemCounter = 0;

    while (*fileElement != '\0')
    {
        while (isspace (*fileElement))
            fileElement++;
        
        if (*fileElement == '+')
        {
            fileElement++;
            continue;
        }

        elemCounter++;
        fileElement++;
        // while ((*fileElement) != '\n' && (*fileElement) != ' ')
        // {
        //     printf ("%c\n", *fileElement);
        //     fileElement++;
        // }
    }
    return elemCounter + 1;
}
