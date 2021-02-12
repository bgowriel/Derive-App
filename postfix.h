#ifndef POSTFIX_H_INCLUDED
#define POSTFIX_H_INCLUDED

#include <stdio.h>
#include <ctype.h>

#define MAX 200
#define L_MAX 30
#define EXPR_MAX 8

/// FUNCTIONS & DATA STRUCTURES FOR OBTAINING POSTFIX FORMULA FROM INFIX FORMULA ///

struct node_for_postfix_stack {
    char* data;
    node_for_postfix_stack* next;
};

struct stack_for_postfix {
    int numberElements;
    node_for_postfix_stack* top;
};

bool empty_stack_for_postfix(stack_for_postfix S)
{
    return S.numberElements == 0;
}

void initialise_stack_for_postfix(stack_for_postfix &S)
{
    S.top = NULL;
    S.numberElements = 0;
}

void push_stack_for_postfix(stack_for_postfix &S, char* x)
{
    node_for_postfix_stack *p;
    if (empty_stack_for_postfix(S))
    {
        S.numberElements = 1;
        S.top = new node_for_postfix_stack;
        S.top->data = x;
        S.top->next = NULL;
    }
    else
    {
        S.numberElements++;
        p = new node_for_postfix_stack;
        p->data = x;
        p->next = S.top;
        S.top = p;
    }
}

void pop_stack_for_postfix(stack_for_postfix &S)
{
    if (!empty_stack_for_postfix(S))
    {
        node_for_postfix_stack* p;
        p = S.top;
        S.top = S.top->next;
        delete p;
        S.numberElements--;
    }
}

char* top_stack_for_postfix(stack_for_postfix S)
{
    if (S.top)
        return S.top->data;
    else
        return NULL;
}

bool isOperand(char x)
{
    if (x=='+' || x=='-' || x=='*' || x=='/' || x=='^' || x=='(' || x==')' || x=='s' || x=='c' || x=='l' || x=='a'||x=='t')
        return 0;
    return 1;
}

int outPrecedence(char x)
{
    if (x == '+' || x == '-')
        return 1;
    if (x == '*' || x == '/')
        return 3;
    if (x == '^')
        return 6;
    if(x=='s' || x=='c' || x=='l' || x=='a' || x=='t')
        return 8;
    if (x == '(')
        return 9;
    if (x == ')')
        return 0;
    return -1;
}

int inPrecedence(char x)
{
    if (x == '+' || x == '-')
        return 2;
    if (x == '*' || x == '/')
        return 4;
    if (x == '^')
        return 5;
    if(x=='s' || x=='c' || x=='l' || x=='a'|| x=='t')
        return 7;
    if (x == '(')
        return 0;
    return -1;
}

char** Convert_Infix_To_Postfix_Formula(char** infix, stack_for_postfix &S, int number_tokens)
{
    char** postfix = new char*[number_tokens];
    int i=0, j=0;
    if (infix[0][0]=='-' && (infix[i][1]>=49 && infix[i][1]<=57)) postfix[j++]=infix[i++];
    while(infix[i] != NULL)
    {
        if (isOperand(infix[i][0]))
            postfix[j++]=infix[i++];
        else
            if(infix[i][0]==')')
                {
                  while(strcmp(top_stack_for_postfix(S),"("))
                  {
                      postfix[j++] = top_stack_for_postfix(S);
                      pop_stack_for_postfix(S);
                  }
                  pop_stack_for_postfix(S);
                  i++;
                }
            else
                if(empty_stack_for_postfix(S) || strcmp(top_stack_for_postfix(S),"(")==0 || infix[i][0]=='(')
                    push_stack_for_postfix(S, infix[i++]);
                else
                {
                    char first_character[7];
                    strcpy(first_character, top_stack_for_postfix(S));
                    if (outPrecedence(infix[i][0]) > inPrecedence(first_character[0]))
                        push_stack_for_postfix(S, infix[i++]);
                    else
                        if (outPrecedence(infix[i][0]) < inPrecedence(first_character[0]))
                        {
                            while (outPrecedence(infix[i][0]) < inPrecedence(first_character[0]) && !empty_stack_for_postfix(S))
                            {
                                postfix[j++] = top_stack_for_postfix(S);
                                pop_stack_for_postfix(S);
                                if (top_stack_for_postfix(S) != NULL)
                                    strcpy(first_character, top_stack_for_postfix(S));
                                else
                                    break;
                            }
                            push_stack_for_postfix(S, infix[i++]);
                        }
                        else
                            if (outPrecedence(infix[i][0]) == inPrecedence(first_character[0]))
                                pop_stack_for_postfix(S);
                }
    }
    while (!empty_stack_for_postfix(S))
    {
        postfix[j++] = top_stack_for_postfix(S);
        pop_stack_for_postfix(S);
    }
    postfix[j] = NULL;
    return postfix;
}

int character_type(char a)
{
    if((a>=48 && a<=57)|| a=='.') return 1;
    if(a=='(') return 3;
    if(a==')') return 4;
    if(a=='+') return 5;
    if(a=='-') return 6;
    if(a=='*') return 7;
    if(a=='/') return 8;
    if(a=='^') return 9;
    if(strchr("sincosartglrdq",a)) return 10;
    return 11;
}

void Insert_Spaces_In_Formula(char expression[])
{
    int i;
    int length = strlen(expression);
    if (expression[0] == '-' && (expression[1]>=49 && expression[1]<=57))
        i=1;
    else
        i=0;
    for (; i<length; i++)
    {
        if(character_type(expression[i])==6 && isdigit(expression[i+1]) && expression[i-2]=='(')
            continue;
        else {
            if(character_type(expression[i])!=character_type(expression[i+1]) || expression[i]=='(' || expression[i]==')')
            {
                for (int j=length; j>=i+1; j--)
                    expression[j+1]=expression[j];
                expression[i+1]=' ';
                i++;
                length++;
            }
        }
    }
    expression[strlen(expression)]='\0';
}

char** Make_Infix_Formula_An_Array_Of_Tokens(char expression[], int &number_tokens)
{
    char** infix = new char*[strlen(expression)];
    char* p;
    int i = 0;
    p = strtok(expression, " ");
    while (p != NULL)
    {
        infix[i++] = p;
        p = strtok(NULL, " ");
        number_tokens++;
    }
    infix[i] = NULL;
    return infix;
}

char** Main_Function_To_Create_Postix_From_Infix_Formula(char* formula)
{
    int number_tokens = 0;
    stack_for_postfix SR;
    initialise_stack_for_postfix(SR);
    Insert_Spaces_In_Formula(formula);
    char** token_vector = Make_Infix_Formula_An_Array_Of_Tokens(formula, number_tokens);
    char** postfix = Convert_Infix_To_Postfix_Formula(token_vector, SR, number_tokens);
    return postfix;
}


#endif // POSTFIX_H_INCLUDED
