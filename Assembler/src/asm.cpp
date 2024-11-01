#include "..\inc\Assembler.h"

static LABEL_STRUCT lbls = {};

int main ()
{
    ASSEMBLER asm_st = {};

    InputFromFile (&asm_st);
    CodeConverter (&asm_st);
    FileWriter (&asm_st);

    return 0;
}

int InputFromFile (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    asm_st->text = FileToStr (asm_st->read_file, &asm_st->file_size);
    /*  SubStringMaker count strings too    */
    SubStingsMaker (asm_st);                                                   // Return string whitout any comments and every \n -> \0 
    
    return 0;
}   

int CodeConverter (ASSEMBLER * asm_st)
{
    my_assert(asm_st);
    FILE * dump_file = fopen (ASM_DEBUG, "a+");
    my_assert(dump_file);

    fprintf (dump_file, "DUMP\n\n");

    fprintf (dump_file, "CREATOR: %x\t"
                        "VERSION: %d\n",
                        asm_st->creator_name, asm_st->version);
    
    fprintf (dump_file, "-------------------------------------------------------------------------\n");
    fprintf (dump_file, "| line |    cmd_name    |  cmd_code  |  arg_code  | have_arg | have_reg |\n");
    my_assert (!fclose (dump_file));

    asm_st->code = (CMD *) calloc (asm_st->n_strings, sizeof (CMD));
    my_assert (asm_st->code);

    printf ("%d - n_strings\n", asm_st->n_strings);
    for ( ; asm_st->n_strings > asm_st->ip; asm_st->ip++)
    {
        COLOR_PRINT(MANGETA, "ip = %d\n", asm_st->ip);
        if (!strncasecmp (asm_st->text, "push", sizeof ("push") - 1))
        {
            printf ("Pushed ip = %d\n", asm_st->ip);
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;
            
            asm_st->n_elems += PushParser (asm_st->text, &asm_st->code[asm_st->ip]);
        }
        else if (!strncasecmp (asm_st->text, "pop", sizeof ("pop") - 1))    
        {
            printf ("Pop\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->n_elems += PopParser(asm_st->text, &asm_st->code[asm_st->ip]);
        }
        else if (!strncasecmp (asm_st->text, "add", sizeof ("add") - 1))        
        {              
            printf ("Added\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code  = ADD;
                                                                                
            asm_st->n_elems++;                                      
        }
        else if (!strncasecmp (asm_st->text, "mul", sizeof ("mul") - 1))        
        {
            printf ("Mul\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code  = MUL;                                             
                                                                    
            asm_st->n_elems++;                                      
        }
        else if (!strncasecmp (asm_st->text, "sub", sizeof ("sub") - 1))        
        {
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code  = SUB;                                             
                                                                    
            asm_st->n_elems++;                                      
        }
        else if (!strncasecmp (asm_st->text, "div", sizeof ("div") - 1))
        {
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code  = DIV;                                             
                                                                    
            asm_st->n_elems++;                                      
        }
        else if (!strncasecmp (asm_st->text, "outp", sizeof ("outp") - 1))        
        {
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code  = OUTP;                                             
                                                                    
            asm_st->n_elems++;                                      
        }
        else if (!strncasecmp (asm_st->text, "inp", sizeof ("inp") - 1))        
        {
            printf ("Inp\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code  = INP;                                             
                                                                    
            asm_st->n_elems++;                                      
        }
        else if (!strncasecmp (asm_st->text, "sin", sizeof ("sin") - 1))        
        {
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code  = SIN;                                             
                                                                    
            asm_st->n_elems++;                                      
        }
        else if (!strncasecmp (asm_st->text, "cos", sizeof ("cos") - 1))        
        {
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code  = COS;                                             
                                                                    
            asm_st->n_elems++;                                      
        }
        else if (!strncasecmp (asm_st->text, "sqrt", sizeof ("sqrt") - 1))        
        {
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code  = SQRT;                                             
                                                                    
            asm_st->n_elems++;                                      
        }
        else if (!strncasecmp (asm_st->text, "dump", sizeof ("dump") - 1))        
        {
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code  = DUMP;                                             
                                                                    
            asm_st->n_elems++;                                      
        }
        else if (!strncasecmp (asm_st->text, "hlt", sizeof ("hlt") - 1))        
        {                                 
            printf ("hlt\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code  = HLT;                  
                                                                    
            asm_st->n_elems++;                                     
        }
        else if (!strncasecmp (asm_st->text, "jmp", sizeof ("jmp") - 1))
        {
            printf ("In jmp\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code = JMP + HAVE_ARG;

            asm_st->text += sizeof ("jmp");

            LabelFromJMP (asm_st);

            asm_st->n_elems += 2;
        }
        else if (!strncasecmp (asm_st->text, "ja", sizeof ("ja") - 1))
        {
            printf ("In ja\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;
            
            asm_st->code[asm_st->ip].cmd_code  = JA + HAVE_ARG;

            asm_st->text += sizeof ("ja");

            LabelFromJMP (asm_st);
            
            asm_st->n_elems += 2;
        }
        else if (!strncasecmp (asm_st->text, "jb", sizeof ("jb") - 1))
        {
            printf ("In jb\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code  = JB + HAVE_ARG;

            asm_st->text += sizeof ("jb");

            LabelFromJMP (asm_st);
                        
            asm_st->n_elems += 2;
        }
        else if (!strncasecmp (asm_st->text, "je", sizeof ("je") - 1))
        {
            printf ("In je\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;
            
            asm_st->code[asm_st->ip].cmd_code  = JE + HAVE_ARG;

            asm_st->text += sizeof ("je");

            LabelFromJMP (asm_st);
            
            asm_st->n_elems += 2;
        }       
        else if (!strncasecmp (asm_st->text, "jbe", sizeof ("jbe") - 1))
        {
            printf ("In jnb\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;
            
            asm_st->code[asm_st->ip].cmd_code  = JBE + HAVE_ARG;

            asm_st->text += sizeof ("jnb");

            LabelFromJMP (asm_st);
            
            asm_st->n_elems += 2;
        }        
        else if (!strncasecmp (asm_st->text, "jae", sizeof ("jae") - 1))
        {
            printf ("In jae\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;
            
            asm_st->code[asm_st->ip].cmd_code  = JAE + HAVE_ARG;

            asm_st->text += sizeof ("jae");

            LabelFromJMP (asm_st);
            
            asm_st->n_elems += 2;
        }
        else if (!strncasecmp (asm_st->text, "call", sizeof ("call") - 1))
        {
            printf ("call\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code = CALL + HAVE_ARG;

            asm_st->text += sizeof ("call");

            LabelFromJMP (asm_st);

            asm_st->n_elems += 2;
        }
        else if (!strncasecmp (asm_st->text, "ret", sizeof ("ret") - 1))
        {
            printf ("ret\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            asm_st->code[asm_st->ip].cmd_code = RET;

            asm_st->n_elems += 1;
        }
        else
        {
            printf ("in else\n");
            asm_st->code[asm_st->ip].cmd_start = asm_st->text;

            if (strchr (asm_st->text, ':'))
            {
                printf ("Label - %s\n", asm_st->text);
                LabelFromLabel (asm_st);
            }
            else
            {
                COLOR_PRINT(RED, "Unknown command: %s\n", asm_st->text);
                exit (-1);  
            }
        }

        asm_st->text += strlen (asm_st->text) + 1;
        COLOR_PRINT (MANGETA, "%s\n",  asm_st->text);
        AssemblerDump (asm_st);
        COLOR_PRINT(GREEN, "StrParser\nIteration done\n");
    }
    COLOR_PRINT (BLUE, "n_elems = %d\n", asm_st->n_elems);
    COLOR_PRINT (RED, "Text parsed\n");
    return 0;
}

// int LabelCleaner (LABEL_DATA * label)
// {
//     my_assert (label);

//     label->ip_to_jmp = -1;
//     label->label_ip  = -1;
//     label->name      = NULL;

//     return 0;
// }

int LabelFromJMP (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    COLOR_PRINT (RED, "In LabelFromJMP\n");

    if (isdigit (*asm_st->text))
    {
        asm_st->code[asm_st->ip].arg = atoi (asm_st->text);

        return 0;
    }

    if (lbls.free_cell == lbls.array_size)
        MemManipulation (INCREASE);
    else if (lbls.array_size > lbls.free_cell + REALLOC_SIZE)
        MemManipulation (DECREASE);
    
    for (size_t i = 0; lbls.free_cell > i; i++)
    {
        printf ("sizeof label = %d\tlbls.label_array[i] = ", strlen (lbls.label_array[i].name));
        printf ("%p\n", lbls.label_array + i);

        COLOR_PRINT (MANGETA, "name: %s\n", lbls.label_array[i].name);
        if (!strncasecmp (lbls.label_array[i].name, asm_st->text, strlen (asm_st->text)))
        {
            printf ("in strcmp\n");

            if (lbls.label_array[i].ip_to_jmp != -1)
            {
                asm_st->code[asm_st->ip].arg = lbls.label_array[i].ip_to_jmp;
            }
        }
    }

    lbls.label_array[lbls.free_cell].label_ip    = asm_st->ip;
    lbls.label_array[lbls.free_cell++].name      = asm_st->text;


    printf ("In LabelFromJMP:\nLabel ip = %d\tlabel name: %s\n", lbls.label_array[lbls.free_cell - 1].label_ip, lbls.label_array[lbls.free_cell - 1].name);

    return 0;
}

int LabelFromLabel (ASSEMBLER * asm_st)
{
    my_assert (asm_st);

    if (lbls.free_cell == lbls.array_size)
        MemManipulation (INCREASE);
    else if (lbls.array_size > lbls.free_cell + REALLOC_SIZE)
        MemManipulation (DECREASE);

    bool flag = false;

    for (size_t i = 0; lbls.free_cell > i; i++)
    {
        printf ("In LabelFromLabel:\nIn for, i = %d\n", i);
        printf ("sizeof label = %d\tlbls.label_array[i] = ", sizeof (*lbls.label_array));
        printf ("%s\n", lbls.label_array[i].name);

        if (!strncasecmp (lbls.label_array[i].name, asm_st->text, sizeof (asm_st->text) - 2))
        {
            printf ("In strncasecmp, asm_st.ip = %d\tn_elems = %d\n", asm_st->ip, asm_st->n_elems);
            
            asm_st->code[lbls.label_array[i].label_ip].arg = asm_st->n_elems - 1;

            printf ("%d - cmdcode\n", lbls.label_array[i].label_ip);
        }

        printf ("In LabelFromLabel:\n End of one cycle\n");
                
    }

    printf ("In LabelFromLabel:\nOut of for\n");

    lbls.label_array[lbls.free_cell].ip_to_jmp    = asm_st->n_elems;
    lbls.label_array[lbls.free_cell++].name       = asm_st->text;

    printf ("ip_to_jmp = %d\n", lbls.label_array[lbls.free_cell - 1].ip_to_jmp);

    // мох стать похожим меньше на цветок больше на мох японский сад промок воду пьёт зеленый мох неприметным ковром стелется в тени цветов древней их пестиков шипов мы все исчезнем но не мох
    asm_st->n_strings--;
    asm_st->ip--;

    return 0;
}

int MemManipulation (REALLOC_MODE flag)
{
    if (flag)
    {
        printf ("INCREAS\n");
        lbls.array_size += REALLOC_SIZE;
    }
    else
    {
        printf ("DECREAS\n");
        lbls.array_size -= REALLOC_SIZE;
    }
    
    lbls.label_array = (LABEL_DATA *) realloc (lbls.label_array, lbls.array_size * sizeof (LABEL_DATA));
    my_assert (lbls.label_array);

    printf ("In MemManipulat\nlabal_array %p\n", &lbls.label_array);
    printf ("array_size = %d\n", lbls.array_size);

    return 0;
}

/*   SubStringMaker count strings too    */
int SubStingsMaker (ASSEMBLER * asm_st)                                      // Splits strings into substings
{
    my_assert (asm_st);

    char * text = (char *) calloc (asm_st->file_size, sizeof (char));
    my_assert (text);

    int offset = 0;

    while (isspace (*text))
        text++;
    
    asm_st->text[offset++] = *text;

    text++;

    printf ("\nSubStringMaker:\nChar - %c\t ASCII code: %d\n", asm_st->text[offset - 1], asm_st->text[offset - 1]);

    while (*text != '\0')
    {
        while ((isspace (asm_st->text[offset - 1]) || asm_st->text[offset - 1] == '\0') && isspace (*text))
            text++;

        if (*text == ';' || *text == ']')
        {
            asm_st->n_strings++;

            COLOR_PRINT (BLUE, "\nSubStringMaker:\n");
            COLOR_PRINT (STRANGE, "%d", asm_st->n_strings)
            COLOR_PRINT (GREEN, "- n_strings\n");

            asm_st->text[offset++] = '\0';
            text++;

            while (*text != '\r')
                text++;
            
            text += 2;

            continue;
        }
        
        if (*text == '\r')
        {
            asm_st->n_strings++;
            
            COLOR_PRINT (BLUE, "\nSubStringMaker:\n");
            COLOR_PRINT (STRANGE, "%d", asm_st->n_strings)
            COLOR_PRINT (GREEN, " - n_strings\n");
            
            asm_st->text[offset++] = '\0';
            text += 2;

            while (*text == '\r')
                text += 2;

            continue;
        }

        asm_st->text[offset++] = *text;                                   // Put char from text to asm_st_string
        printf ("\nSubStringMaker:\nChar - %c\t ASCII code: %d\n", asm_st->text[offset - 1], asm_st->text[offset - 1]);
        text++;
    }
    asm_st->n_strings++;

/*  after hlt going '\0'  */
    asm_st->text[offset] = '\0';

    asm_st->text = (char *) realloc (asm_st->text, offset);
    my_assert (asm_st->text);

    COLOR_PRINT (GREEN, "\nIn SubStringMaker:\n n_strings = %d\n", asm_st->n_strings);

    return 0;
}

int FileWriter (ASSEMBLER * asm_st)
{
    codeElem * final_code = (codeElem *) calloc (asm_st->n_elems, sizeof (codeElem));
    my_assert (final_code);

    codeElem * dupl_final_code = final_code;

    for (int i = 0; asm_st->n_strings > i; i++)
    {
        COLOR_PRINT(GREEN, "\nFileWriter\ncmd_code: <%d>\n", asm_st->code[i].cmd_code);
        
        *final_code = asm_st->code[i].cmd_code;
        final_code++;

        if (asm_st->code[i].cmd_code & HAVE_REG)
        {
            *final_code = asm_st->code[i].reg;
            final_code++;
        }
        if (asm_st->code[i].cmd_code & HAVE_ARG)
        {
            *final_code = asm_st->code[i].arg;
            final_code++;
        }
    }

    FILE * file_asm_st = fopen (asm_st->assembler_bin, "w+b");
    my_assert(file_asm_st != NULL);

    /*    Creator`s data writer    */
    fprintf (file_asm_st, "%d\n%d\n%d\n", CREATORS_NAME, VERSION, asm_st->n_elems);

    printf ("n_elems = %d\n", asm_st->n_elems);

    for (int i = 0; asm_st->n_elems > i; i++)
    {
        printf ("%d\n", dupl_final_code[i]);
        fprintf (file_asm_st, "%d\n", dupl_final_code[i]);
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


/**
 * @brief This func pars string with code "Push"
 * \param [in] str     String with text
 * \param [in] command Struct with datas about command
 * 
 * IMPORTANT! This func returns how many code elements is in this "Push"
 */

int PushParser (char * str, CMD * command)
{
    my_assert (command);
    my_assert (str);

    command->cmd_code  = PUSH;

    str = str + sizeof ("push");                                                // Space skipped because of '\0' is in strlen

    char * ram = strchr (str, '[');
    if (ram)
    {
        command->cmd_code += HAVE_RAM;

        str = ram + 1;
    }

    if (strchr (str, '+'))
    {
        command->reg = *(str + 1) - 'a';
        command->cmd_code += HAVE_REG;

        str += sizeof ("rax +") - 1;                                                // Space skipped because of '\0' is in strlen

        command->arg = atoi (str);
        command->cmd_code += HAVE_ARG;

        return 3;
    }

    if (*str == 'r' && *(str + 2) == 'x')
    {
        command->reg       = *(str + 1) - 'a';
        command->cmd_code += HAVE_REG;

        return 2;
    }
    else
    {
        command->arg       = atoi (str);
        command->cmd_code += HAVE_ARG;

        return 2;
    }
}

/**
 * @brief This func pars string with code "Pop"
 * \param [in] str     String with text
 * \param [in] command Struct with datas about command
 * 
 * IMPORTANT! This func returns how many code elements is in this "Pop"
 */

int PopParser (char * str, CMD * command)
{
    my_assert (command);
    my_assert (str);
    

    command->cmd_code  = POP;

    str += sizeof ("pop");

    char * ram = strchr (str, '[');
    if (ram)
    {
        command->cmd_code += HAVE_RAM;

        str = ram + 1;
    }

    if (strchr (str, '+'))
    {
        command->reg = *(str + 1) - 'a';
        command->cmd_code += HAVE_REG;

        str += sizeof ("rax +") - 1;                                                // Space skipped because of '\0' is in strlen

        command->arg = atoi (str);
        command->cmd_code += HAVE_ARG;

        return 3;
    }
    
    if (*str == 'r' && *(str + 2) == 'x')
    {
        command->reg       = *(str + 1) - 'a';
        command->cmd_code += HAVE_REG;

        return 2;
    }
    else
    {
        COLOR_PRINT (RED, "ERROR, you entered wrong data in pop\n");
        exit (-1);
    }
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