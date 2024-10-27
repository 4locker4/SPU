#include "..\inc\Assembler.h"
#include "..\inc\AssemblerFuncs.h"

int InputFromFile (ASSEMBLER * stk)
{
    my_assert (stk);

    char * raw_text = FileToStr (stk->read_file);
    printf ("raw string:\n%s\n", raw_text);
    /*  SubStringMaker count strings too    */
    SubStingsMaker (raw_text, stk);                                                   // Return string whitout any comments and every \n -> \0 
    
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
    
    printf ("%c - first\n", *text);
    stk->text[offset++] = *text;
    text++;

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

    stk->text[offset] = '\0';
    stk->text[offset + 1] = '`';                                       // For DUMP!

    stk->text = (char *) realloc (stk->text, offset);
    my_assert (stk->text);

    COLOR_PRINT (GREEN, "\nIn SubStringMaker:\n n_strings = %d\n", stk->n_strings);

    return 0;
}