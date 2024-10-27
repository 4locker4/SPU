#include "..\inc\Assembler.h"
#include "..\inc\AssemblerFuncs.h"

static LABEL_STRUCT lbls = {};

int main ()
{
    ASSEMBLER stk = {};

    InputFromFile (&stk);
    CodeConverter (&stk);
    FileWriter (&stk);

    return 0;
}

int InputFromFile (ASSEMBLER * stk)
{
    my_assert (stk);

    char * raw_text = FileToStr (stk->read_file);
    printf ("raw string:\n%s\n", raw_text);
    /*  SubStringMaker count strings too    */
    SubStingsMaker (raw_text, stk);                                                   // Return string whitout any comments and every \n -> \0 
    
    return 0;
}   

int CodeConverter (ASSEMBLER * stk)
{
    my_assert(stk);
    AssemblerDump (stk);

    stk->code = (CMD *) calloc (stk->n_strings, sizeof (CMD));
    my_assert (stk->code);

    printf ("%d - n_strings\n", stk->n_strings);
    for ( ; stk->n_strings > stk->ip; stk->ip++)
    {
        COLOR_PRINT(MANGETA, "ip = %d\n", stk->ip);
        if (!strncasecmp (stk->text, "push", sizeof ("push") - 1))
        {
            printf ("Pushed ip = %d\n", stk->ip);

            stk->n_elems += PushParser (stk->text, &stk->code[stk->ip]);

            stk->text += stk->code[stk->ip].cmd_len;
        }
        else if (!strncasecmp (stk->text, "pop", sizeof ("pop") - 1))    
        {
            printf ("Pop\n");

            stk->n_elems += PopParser(stk->text, &stk->code[stk->ip]);

            stk->text += stk->code[stk->ip].cmd_len;
        }
        else if (!strncasecmp (stk->text, "add", sizeof ("add") - 1))        
        {              
            printf ("Added\n");

            stk->code[stk->ip].cmd_code  = ADD;
                                                                                
            stk->n_elems++;                                      
            stk->text += strlen (stk->text) + 1;                        
        }
        else if (!strncasecmp (stk->text, "mul", sizeof ("mul") - 1))        
        {
            printf ("Mul\n");

            stk->code[stk->ip].cmd_code  = MUL;                                             
                                                                    
            stk->n_elems++;                                      
            stk->text += strlen (stk->text) + 1;                        
        }
        else if (!strncasecmp (stk->text, "sub", sizeof ("sub") - 1))        
        {
            stk->code[stk->ip].cmd_code  = SUB;                                             
                                                                    
            stk->n_elems++;                                      
            stk->text += strlen (stk->text) + 1;                        
        }
        else if (!strncasecmp (stk->text, "div", sizeof ("div") - 1))        
        {
            stk->code[stk->ip].cmd_code  = DIV;                                             
                                                                    
            stk->n_elems++;                                      
            stk->text += strlen (stk->text) + 1;                        
        }
        else if (!strncasecmp (stk->text, "outp", sizeof ("outp") - 1))        
        {
            stk->code[stk->ip].cmd_code  = OUTP;                                             
                                                                    
            stk->n_elems++;                                      
            stk->text += strlen (stk->text) + 1;                        
        }
        else if (!strncasecmp (stk->text, "inp", sizeof ("inp") - 1))        
        {
            printf ("Inp\n");

            stk->code[stk->ip].cmd_code  = INP;                                             
                                                                    
            stk->n_elems++;                                      
            stk->text += strlen (stk->text) + 1;                        
        }
        else if (!strncasecmp (stk->text, "sin", sizeof ("sin") - 1))        
        {
            stk->code[stk->ip].cmd_code  = SIN;                                             
                                                                    
            stk->n_elems++;                                      
            stk->text += strlen (stk->text) + 1;                        
        }
        else if (!strncasecmp (stk->text, "cos", sizeof ("cos") - 1))        
        {
            stk->code[stk->ip].cmd_code  = COS;                                             
                                                                    
            stk->n_elems++;                                      
            stk->text += strlen (stk->text) + 1;                        
        }
        else if (!strncasecmp (stk->text, "Ssqrt", sizeof ("sqrt") - 1))        
        {
            stk->code[stk->ip].cmd_code  = SQRT;                                             
                                                                    
            stk->n_elems++;                                      
            stk->text += strlen (stk->text) + 1;                        
        }
        else if (!strncasecmp (stk->text, "dump", sizeof ("dump") - 1))        
        {
            stk->code[stk->ip].cmd_code  = DUMP;                                             
                                                                    
            stk->n_elems++;                                      
            stk->text += strlen (stk->text) + 1;                        
        }
        else if (!strncasecmp (stk->text, "hlt", sizeof ("hlt") - 1))        
        {                                                           
            stk->code[stk->ip].cmd_code  = HLT;                  
                                                                    
            stk->n_elems++;                                     
            stk->text += strlen (stk->text) + 1;                        
        }
        else if (!strncasecmp (stk->text, "jmp", sizeof ("jmp") - 1))
        {
            printf ("In jmp\n");

            stk->code[stk->ip].cmd_code = JMP + HAVE_ARG;

            stk->text += sizeof ("jmp");

            LabelFromJMP (stk);

            stk->n_elems += 2;
        }
        else if (!strncasecmp (stk->text, "ja", sizeof ("ja") - 1))
        {
            printf ("In ja\n");
            
            stk->code[stk->ip].cmd_code  = JA + HAVE_ARG;

            stk->text += sizeof ("ja");

            LabelFromJMP (stk);
            
            stk->n_elems += 2;
        }
        else if (!strncasecmp (stk->text, "jb", sizeof ("jb") - 1))
        {
            printf ("In jb\n");

            stk->code[stk->ip].cmd_code  = JB + HAVE_ARG;

            stk->text += sizeof ("jb");

            LabelFromJMP (stk);
                        
            stk->n_elems += 2;
        }
        else if (!strncasecmp (stk->text, "call", sizeof ("call") - 1))
        {
            stk->code[stk->ip].cmd_code = CALL;

            LabelFromJMP
        }
        else
        {
            printf ("in else\n");

            LabelFromLabel (stk);
            stk->ip--;

            // COLOR_PRINT(RED, "Unknown command: %s\n", stk->text);
            // exit (-1); 
        }

        COLOR_PRINT(GREEN, "StrParser\nIteration done\n");
    }

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

int LabelFromJMP (ASSEMBLER * stk)
{
    my_assert (stk);

    COLOR_PRINT (RED, "In LabelFromJMP\n");

    if (isdigit (*stk->text))
    {
        stk->code[stk->ip].arg = atoi (stk->text);

        stk->text += strlen (stk->text) + 1;

        return 0;
    }

    if (lbls.free_cell == lbls.array_size)
        MemManipulation (INCREASE);
    else if (lbls.array_size > lbls.free_cell + REALLOC_SIZE)
        MemManipulation (DECREASE);
    
    for (size_t i = 0; lbls.free_cell > i; i++)
    {
        printf ("sizeof label = %d\tlbls.label_array[i] = ", sizeof (*lbls.label_array));
        printf ("%p\n", lbls.label_array);
        if (!strncasecmp (lbls.label_array[i].name, stk->text, strlen (stk->text)))
        {
            stk->code[stk->ip].arg = lbls.label_array[i].ip_to_jmp;

            // LabelCleaner (&lbls.label_array[i]);

            return 0;
        }
    }

    lbls.label_array[lbls.free_cell].label_ip    = stk->ip;
    lbls.label_array[lbls.free_cell++].name      = stk->text;

    stk->text += strlen (stk->text) + 1;

    printf ("In LabelFromJMP:\nLabel ip = %d\tlabel name: %s\n", lbls.label_array[lbls.free_cell - 1].label_ip, lbls.label_array[lbls.free_cell - 1].name);

    return 0;
}

int LabelFromLabel (ASSEMBLER * stk)
{
    my_assert (stk);

    if (lbls.free_cell == lbls.array_size)
        MemManipulation (INCREASE);
    else if (lbls.array_size > lbls.free_cell + REALLOC_SIZE)
        MemManipulation (DECREASE);

    for (size_t i = 0; lbls.free_cell > i; i++)
    {
        printf ("In LabelFromLabel:\nIn for, i = %d\n", i);
        printf ("sizeof label = %d\tlbls.label_array[i] = ", sizeof (*lbls.label_array));
        printf ("%p\n", lbls.label_array);

        if (!strncasecmp (lbls.label_array[i].name, stk->text, strlen (stk->text)))
        {
            printf ("In strncasecmp, stk.ip = %d\tn_elems = %d\n", stk->ip, stk->n_elems);

            stk->code[lbls.label_array[i].label_ip].arg = stk->n_elems;

            printf ("%d - cmdcode\n", lbls.label_array[i].label_ip);

            stk->text += strlen (stk->text) + 1;

            stk->n_strings--;

            // LabelCleaner (&lbls.label_array[i]);

            return 0;
        }

        printf ("In LabelFromLabel:\n End of one cycle\n");
                
    }

    printf ("In LabelFromLabel:\nOut of for\n");

    lbls.label_array[lbls.free_cell].ip_to_jmp    = stk->n_elems;
    lbls.label_array[lbls.free_cell++].name      = stk->text;

    stk->text += strlen (stk->text) + 1;

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
    
    lbls.label_array = (LABEL_DATA *) realloc (lbls.label_array, lbls.array_size);
    my_assert (lbls.label_array);

    printf ("In MemManipulat\nlabal_array %p\n", &lbls.label_array);
    printf ("array_size = %d\n", lbls.array_size);

    return 0;
}

/*   SubStringMaker count strings too    */
int SubStingsMaker (char * text, ASSEMBLER * stk)                                      // Splits strings into substings
{
    my_assert (stk);

    stk->text = (char *) calloc (strlen (text), sizeof (char));
    my_assert (stk->text);

    int offset = 0;

    while (isspace (*text))
        text++;
    
    stk->text[offset++] = *text;

    text++;

    printf ("\nSubStringMaker:\nChar - %c\t ASCII code: %d\n", stk->text[offset - 1], stk->text[offset - 1]);

    while (*text != '\0')
    {
        while ((isspace (stk->text[offset - 1]) || stk->text[offset - 1] == '\0') && isspace (*text))
            text++;

        if (*text == ';' || *text == ']')
        {
            stk->n_strings++;

            COLOR_PRINT (BLUE, "\nSubStringMaker:\n");
            COLOR_PRINT (STRANGE, "%d", stk->n_strings)
            COLOR_PRINT (GREEN, "- n_strings\n");

            stk->text[offset++] = '\0';
            text++;

            while (*text != '\r')
                text++;
            text += 2;
            continue;
        }
        
        if (*text == '\r')
        {
            stk->n_strings++;

            COLOR_PRINT (BLUE, "\nSubStringMaker:\n");
            COLOR_PRINT (STRANGE, "%d", stk->n_strings)
            COLOR_PRINT (GREEN, " - n_strings\n");
            
            stk->text[offset++] = '\0';
            text += 2;

            while (*text == '\r')
                text += 2;
            continue;
        }

        stk->text[offset++] = *text;                                   // Put char from text to asm_string
        printf ("\nSubStringMaker:\nChar - %c\t ASCII code: %d\n", stk->text[offset - 1], stk->text[offset - 1]);
        text++;
    }

    stk->n_strings++;                                                  // +1 brcous of after hlt going '\0'

    stk->text[offset] = '\0';
    stk->text[offset + 1] = '`';                                       // For DUMP!

    stk->text = (char *) realloc (stk->text, offset + 1);
    my_assert (stk->text);

    COLOR_PRINT (GREEN, "\nIn SubStringMaker:\n n_strings = %d\n", stk->n_strings);

    return 0;
}

int FileWriter (ASSEMBLER * stk)
{
    codeElem * final_code = (codeElem *) calloc (stk->n_elems, sizeof (codeElem));
    my_assert (final_code);

    codeElem * dupl_final_code = final_code;

    for (int i = 0; stk->n_strings > i; i++)
    {
        COLOR_PRINT(GREEN, "\nFileWriter\ncmd_code: <%d>\n", stk->code[i].cmd_code);
        
        *final_code = stk->code[i].cmd_code;
        final_code++;

        if (stk->code[i].cmd_code & HAVE_REG)
        {
            *final_code = stk->code[i].reg;
            final_code++;
        }
        if (stk->code[i].cmd_code & HAVE_ARG)
        {
            *final_code = stk->code[i].arg;
            final_code++;
        }
    }

    FILE * file_asm = fopen (stk->assembler_bin, "w+");
    my_assert(file_asm != NULL);

    /*    Creator`s data writer    */
    fprintf (file_asm, "%d\n%d\n%d\n", CREATORS_NAME, VERSION, stk->n_elems);

    for (int i = 0; stk->n_elems > i; i++)
    {
        printf ("%d\n", dupl_final_code[i]);
        fprintf (file_asm, "%d\n", dupl_final_code[i]);
    }

    my_assert (!fclose (file_asm));

    // FILE * asm_bin = fopen (stk->assembler_bin, "ab");

    // fwrite (dupl_final_code, sizeof (codeElem), stk->n_elems, asm_bin);
    // my_assert (!fclose (asm_bin));

    return 0;
}

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
    command->cmd_start = str;
    command->cmd_len   = strlen (str) + 1;

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

        str += strlen ("rax +");                                                // Space skipped because of '\0' is in strlen

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
    command->cmd_start = str;
    command->cmd_len   = strlen (str) + 1;

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

        str += strlen ("rax +");                                                // Space skipped because of '\0' is in strlen

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

int AssemblerDump (ASSEMBLER * stk)
{
    my_assert(&stk);

    FILE * dump_file = fopen (ASM_DEBUG, "a+");
    my_assert(dump_file);

    fprintf (dump_file, "DUMP\n\n");

    fprintf (dump_file, "CREATOR: %x\n"
                        "VERSION: %d\n"
                        "n_elements = %d;    ip = %d\n", 
                        stk->creator_name, stk->version, stk->n_elems, stk->ip);
    for (int i = 0; stk->ip > i; i++)
    {
        fprintf (dump_file, "Code element №%d:  %s\n", i, stk->code[i].cmd_start);
    }
    char * test = stk->text;

    while (*test != '`')
    {
        if (*test == '\0')
            fprintf (dump_file, " slash 0\n");
        else
            fprintf (dump_file, "%c", *test);
        test++;
    }

    my_assert(!fclose (dump_file));
    return 0;
}