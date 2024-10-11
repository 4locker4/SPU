#include "./inc/assembler.h"

int main (int argc, char * argv[])
{
    Code_stk code = {}; 
}

int Run (int code[], size_t size)
{
    Stack_t stk = {};
    StackCtor (&stk, STACK_SIZE);

    int  ip    = 0;
    bool input = true;

    while (input)
    {
        switch (code[ip++])
        {
            case (push):
            {    
                StackPush (&stk, code[ip++]);

                break;
            }    
            case (add):
            {
                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                StackPush (&stk, a + b);

                break;
            }
            case (sub):
            {
                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                StackPush (&stk, b - a);

                break;
            }
            case (mul):
            {
                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                StackPush (&stk, a * b);

                break;
            }
            case (div):
            {
                stackElem a = StackPop (&stk);
                stackElem b = StackPop (&stk);

                StackPush (&stk, a / b);

                break;
            }
            case (out):
            {
                COLOR_PRINT (BLUE, "Output val: <%d>\n", StackPop (&stk));

                break;
            }
            case (in):
            {
                stackElem a = scanf ("%d", a);

                StackPush (&stk, a);

                break;
            }
            case (square):
            {
                stackElem a = StackPop (&stk);

                StackPush (&stk, sqrt (a));

                break;
            }
            case (sinus):
            {
                stackElem a = StackPop (&stk);

                StackPush (&stk, sin (a));

                break;
            }
            case (cosinus):
            {
                stackElem a = StackPop (&stk);

                StackPush (&stk, cos (a));

                break;
            }
            case (dump):
            {
                StackDump (&stk, 0, "stk", __FILE__, __LINE__);

                break;
            }
            case (hlt):
            {
                input = false;

                break;
            }
            default:
            {
                COLOR_PRINT (RED, "You entered wrong data\n");

                break;
            }
        }
    }
}

int AssemblerReader (const char * file_directory, Code_stk * code)
{
    char * text = FileToStr (file_directory);
    code->nElems = StrCounter (text);

    int ip = 0;

    code->code = (stackElem *) calloc (code->nElems, sizeof (stackElem));
    my_assert (!code->code);

    bool input = true;

    while (input)
    {
        if (!strncasecmp (text, "push", 4))
        {
            code->code[ip++] = push;
            text += 5;

            code->code[ip] = atoi (text);

            while (text[])
        }
    }
/*
    for (int i = 0; i < code->nElems; i++)
    {
        code->code[i] = (stackElem) atoi (text);
    }    

    FILE * assembler_file = fopen (ASSEMBLER_FILE, "a+");
    my_assert (!assembler_file);

    for (int i = 0; i < code->nElems; i++)
    {
        fprintf (assembler_file, code->code[i])
    }*/
}

void StrParser (char * str)
{
    int counter = 0;

    while (str[counter] != '\0')
    {
        if (isspace (str[counter]))
            str[counter] = '\0';
        counter++;
    }
}