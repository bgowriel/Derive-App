#ifndef FORMULA_TREE_H_INCLUDED
#define FORMULA_TREE_H_INCLUDED

#include <stdio.h>
#include <ctype.h>
//#include <graphics.h>

#define MAX 200
#define L_MAX 30
#define EXPR_MAX 8

/// FUNCTIONS & DATA STRUCTURES FOR OBTAINING POSTFIX FORMULA TREE ///

struct treeNode {
    char info[8];
    treeNode* left;
    treeNode* right;
};
typedef treeNode* treePointer;

struct node_for_stack_of_trees {
    treePointer fterm;
    node_for_stack_of_trees* next;
};

struct stack_of_trees {
    node_for_stack_of_trees* top;
    int nrElements;
};

bool Test_Void_Stack_Of_Trees(stack_of_trees S) {
    return S.nrElements == 0;
}

void Initialize_Stack_Of_Trees(stack_of_trees &S) {
    S.top = NULL;
    S.nrElements = 0;
}

void Push(stack_of_trees &S, treePointer &new_term)
{
    if (Test_Void_Stack_Of_Trees(S))
    {
        S.nrElements = 1;
        S.top = new node_for_stack_of_trees;
        S.top->fterm = new_term;
        S.top->next = NULL;
    }
    else
    {
        node_for_stack_of_trees* new_node;
        S.nrElements++;
        new_node = new node_for_stack_of_trees;
        new_node->fterm = new_term;
        new_node->next = S.top;
        S.top = new_node;
    }
}

treePointer Pop(stack_of_trees &S) {
    if (!Test_Void_Stack_Of_Trees(S))
    {
        treePointer term;
        node_for_stack_of_trees* new_top;
        term = S.top->fterm;
        new_top = S.top->next;
        delete S.top;
        S.top = new_top;
        S.nrElements--;
        return term;
    }
    else
    {
        return NULL;
    }
}

int Is_Void_Tree(treePointer a)
{
    return (a == NULL);
}

void Initialize_Tree(treePointer &a)
{
    if (!Is_Void_Tree(a)) a = NULL;
}

treePointer Make_Formula_Tree(stack_of_trees &S, treePointer &a, char expr_postfx[L_MAX][EXPR_MAX])
{
    int i = 0;
    while (expr_postfx[i][0] != '\0')
    {
        switch (expr_postfx[i][0])
        {
            case '+':
                a = new treeNode;
                strcpy(a->info, expr_postfx[i]);
                a->right = Pop(S);
                a->left = Pop(S);
                Push(S, a);
                break;
            case '-':
                if (isdigit(expr_postfx[i][1]))
                {
                    a = new treeNode;
                    strcpy(a->info, expr_postfx[i]);
                    a->left = NULL;
                    a->right = NULL;
                    Push(S, a);
                    break;
                }
                else {
                    if (expr_postfx[i][1] == '\0') {
                        a = new treeNode;
                        strcpy(a->info, expr_postfx[i]);
                        a->right = Pop(S);
                        a->left = Pop(S);
                        Push(S, a);
                        break;
                    }
                }
            case '*':
                a = new treeNode;
                strcpy(a->info, expr_postfx[i]);
                a->right = Pop(S);
                a->left = Pop(S);
                Push(S, a);
                break;
            case '/':
                a = new treeNode;
                strcpy(a->info, expr_postfx[i]);
                a->right = Pop(S);
                a->left = Pop(S);
                Push(S, a);
                break;
            case '^':
                a = new treeNode;
                strcpy(a->info, expr_postfx[i]);
                a->right = Pop(S);
                a->left = Pop(S);
                Push(S, a);
                break;
            case 's':
                a=new treeNode;
                strcpy(a->info,expr_postfx[i]);
                a->left=Pop(S);
                a->right=NULL;
                Push(S, a);
                break;
            case 'c':
                a=new treeNode;
                strcpy(a->info,expr_postfx[i]);
                a->left=Pop(S);
                a->right=NULL;
                Push(S, a);
                break;
            case 't':
                a=new treeNode;
                strcpy(a->info,expr_postfx[i]);
                a->left=Pop(S);
                a->right=NULL;
                Push(S, a);
                break;
            case 'a':
                a=new treeNode;
                strcpy(a->info,expr_postfx[i]);
                a->left=Pop(S);
                a->right=NULL;
                Push(S, a);
                break;
            case 'l':
                a=new treeNode;
                strcpy(a->info,expr_postfx[i]);
                a->left=Pop(S);
                a->right=NULL;
                Push(S, a);
                break;
            default:
                a = new treeNode;
                strcpy(a->info, expr_postfx[i]);
                a->left = NULL;
                a->right = NULL;
                Push(S, a);
        }
        i++;
    }
    return a;
}

/// FUNCTIONS FOR DRAWING FORMULA TREE /// --- FOR DEBUGGING ---
/*
int Nr_Of_Levels(treePointer a)
{
    if (a == NULL) return 0;
    else {
        int nivSt = Nr_Of_Levels(a->left);
        int nivDr = Nr_Of_Levels(a->right);
        return 1 + (nivDr > nivSt ? nivDr : nivSt);
    }
}

int Nr_Of_Columns(treePointer a)
{
    if (a == NULL) return 0;
    else {
        int colSt = Nr_Of_Columns(a->left);
        int colDr = Nr_Of_Columns(a->right);
        return 1 + colDr + colSt;
    }
}

void Represent_Tree(treePointer a, int column, int level)
{
    if (a!=NULL)
    {
        setcolor(0);
        setfillstyle(1, 9);
        fillellipse(column*60 - 30, level*60 - 34, textwidth(a->info), textheight(a->info));
        setbkcolor(9);
        outtextxy(column*60 - 30, level*60 - 30, a->info);
        if (!Is_Void_Tree(a->left)) {
            treePointer b = a->left;
            Represent_Tree(a->left, column - Nr_Of_Columns(b->right) - 1, level + 1);
        }
        if (!Is_Void_Tree(a->right)) {
            treePointer b = a->right;
            Represent_Tree(a->right, column + Nr_Of_Columns(b->left) + 1, level + 1);
        }
    }
    else return;
}

void Represent_Tree_Lines(treePointer a, int column, int level, int x_ant, int y_ant)
{
    if (a!=NULL)
    {
        if (x_ant && y_ant) line(x_ant, y_ant, column*60 - 30, level*60 - 30);
        if (!Is_Void_Tree(a->left)) {
            treePointer b = a->left;
            Represent_Tree_Lines(a->left, column - Nr_Of_Columns(b->right) - 1, level + 1, column*60 - 30, level*60 - 30);
        }
        if (!Is_Void_Tree(a->right)) {
            treePointer b = a->right;
            Represent_Tree_Lines(a->right, column + Nr_Of_Columns(b->left) + 1, level + 1, column*60 - 30, level*60 - 30);
        }
    }
    else return;
}

void Main_Function_To_Represent_Tree(treePointer a)
{
    initwindow(60*Nr_Of_Columns(a)+25, 60*Nr_Of_Levels(a)+25, "First order derivative", 100, 100, false, false);
    setlinestyle(0, 0, 2);
    setfillstyle(SOLID_FILL, 15);
    floodfill(1, 1, 15);
    setcolor(6);
    Represent_Tree_Lines(a, Nr_Of_Columns(a->left) + 1, 1, 0, 0);
    settextstyle(4, HORIZ_DIR, 2);
    settextjustify(1, 1);
    Represent_Tree(a, Nr_Of_Columns(a->left) + 1, 1);
    clearmouseclick(WM_LBUTTONDOWN);
    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int win = getcurrentwindow();
            setcurrentwindow(0);
            closegraph(win);
            clearmouseclick(WM_LBUTTONDOWN);
            return;
        }
    }
}
*/

#endif // FORMULA_TREE_H_INCLUDED
