#ifndef DERIVE_SIMPLIFY_H_INCLUDED
#define DERIVE_SIMPLIFY_H_INCLUDED

#include <stdio.h>
#include <ctype.h>

/// FUNCTIONS TO DERIVE AND SIMPLIFY FORMULA TREE ///

void Duplicate_Tree(treePointer a, treePointer &b)
{
    if (a != NULL) {
        strcpy(b->info, a->info);
        if (a->left != NULL) {
            b->left = new treeNode;
            Duplicate_Tree(a->left, b->left);
        }
        else b->left = NULL;
        if (a->right != NULL) {
            b->right = new treeNode;
            Duplicate_Tree(a->right, b->right);
        }
        else b->right = NULL;
    }
}

treePointer Derive(treePointer &a)
{
    if (a == NULL) return NULL;
    switch (a->info[0])
    {
        case '+': // (f+g)'=f'+g'
            if (a->left != NULL) Derive(a->left);
            if (a->right != NULL) Derive(a->right);
            return a;
        case '-': // (f-g)'=f'-g'
            if (a->info[1] == '\0') {
                if (a->left != NULL) Derive(a->left);
                if (a->right != NULL) Derive(a->right);
                return a;
            }
            else {
                a->info[0] = '0';
                a->info[1] = '\0';
                return a;
            }
        case '*': //(f*g)'=f'*g+g'*f
            {
                a->info[0] = '+'; // root
                // ramura stanga f'*g
                treePointer leftNode = new treeNode;
                leftNode->info[0] = '*';
                leftNode->info[1] = '\0';
                leftNode->left = new treeNode;
                Duplicate_Tree(a->left, leftNode->left);
                leftNode->right = new treeNode;
                Duplicate_Tree(a->right, leftNode->right);
                // ramura dreapta g'*f
                treePointer rightNode = new treeNode;
                rightNode->info[0] = '*';
                rightNode->info[1] = '\0';
                rightNode->left = new treeNode;
                Duplicate_Tree(a->right, rightNode->left);
                rightNode->right = new treeNode;
                Duplicate_Tree(a->left, rightNode->right);
                // legaturi si derivare
                a->left = leftNode;
                a->right = rightNode;
                Derive(a->left->left);
                Derive(a->right->left);
                return a;
            }
        case '/':
            {
                if (!isdigit(a->right->info[0])) { //(f/g)'=(f'*g-g'*f)/(g^2)
                    a->info[0] = '/'; // root
                    // ramura stanga f'*g-g'*f
                    treePointer leftNode = new treeNode;
                    leftNode->info[0] = '-';
                    leftNode->info[1] = '\0';
                    //ramura stanga-stanga f'*g
                    treePointer leftleftNode = new treeNode;
                    leftNode->left = leftleftNode;
                    leftleftNode->info[0] = '*';
                    leftleftNode->info[1] = '\0';
                    leftleftNode->left = new treeNode;
                    Duplicate_Tree(a->left, leftleftNode->left);
                    leftleftNode->right = new treeNode;
                    Duplicate_Tree(a->right, leftleftNode->right);
                    //ramura stanga-dreapta g'*f
                    treePointer leftrightNode = new treeNode;
                    leftNode->right = leftrightNode;
                    leftrightNode->info[0] = '*';
                    leftrightNode->info[1] = '\0';
                    leftrightNode->left = new treeNode;
                    Duplicate_Tree(a->right, leftrightNode->left);
                    leftrightNode->right = new treeNode;
                    Duplicate_Tree(a->left, leftrightNode->right);
                    // ramura dreapta g^2
                    treePointer rightNode = new treeNode;
                    rightNode->info[0] = '^';
                    rightNode->info[1] = '\0';
                    rightNode->left = new treeNode;
                    Duplicate_Tree(a->right, rightNode->left);
                    rightNode->right = new treeNode;
                    rightNode->right->info[0]='2';
                    rightNode->right->info[1]='\0';
                    rightNode->right->left = NULL;
                    rightNode->right->right = NULL;
                    // legaturi si derivare
                    a->left = leftNode;
                    a->right = rightNode;
                    Derive(a->left->left->left);
                    Derive(a->left->right->left);
                    return a;
                }
                else { // (f/const)'
                    Derive(a->left);
                    return a;
                }
            }
        case '^':
            {   /// NU E IMPLEMENTATA DERIVATA COMPUSA FUNCTIE ^ FUNCTIE -> (f^f)'
                /*if (!isdigit(a->left->info[0]) && !isdigit(a->right->info[0]))
                { (f^g)'=(e^(g*ln(f))'= (e^(g*ln(f)) * (g*ln(f))'= (e^(g*ln(f)) * (g'*ln(f)+g*f'/f) }*/
                if (isdigit(a->left->info[0]) || a->left->info[0]=='e')
                { // (a^f)'= a^f * ln(a) * f'
                    //ramura stanga a^f * ln(a)
                    treePointer leftNode=new treeNode;
                    leftNode->info[0]='*';
                    leftNode->info[1]='\0';
                    leftNode->left=new treeNode;
                    leftNode->right=new treeNode;
                    Duplicate_Tree(a, leftNode->left);
                    treePointer leftrightNode=new treeNode;
                    leftNode->right=leftrightNode;
                    strcpy(leftrightNode->info,"ln");
                    leftrightNode->left=new treeNode;
                    strcpy(leftrightNode->left->info,a->left->info);
                    leftrightNode->left->left = NULL;
                    leftrightNode->left->right = NULL;
                    leftrightNode->right=NULL;
                    //ramura dreapta f'
                    treePointer rightNode=new treeNode;
                    Duplicate_Tree(a->right,rightNode);
                    //legaturi si derivare
                    strcpy(a->info,"*");
                    a->left=leftNode;
                    a->right=rightNode;
                    Derive(a->right);
                    return a;
                }
                else // (f^n)'= (f^(n-1))*n*f'
                {
                    a->info[0] = '*'; // root
                    // ramura stanga (f^(n-1))*n
                    treePointer leftNode = new treeNode;
                    leftNode->info[0] = '*';
                    leftNode->info[1] = '\0';
                    treePointer leftleftNode = new treeNode;
                    leftNode->left = leftleftNode;
                    leftleftNode->info[0] = '^';
                    leftleftNode->info[1] = '\0';
                    leftleftNode->left = new treeNode;
                    Duplicate_Tree(a->left, leftleftNode->left);
                    leftleftNode->right = new treeNode;
                    int exponent;
                    sscanf(a->right->info, "%d", &exponent);
                    exponent--;
                    sprintf(leftleftNode->right->info, "%d", exponent);
                    leftleftNode->right->left = NULL;
                    leftleftNode->right->right = NULL;
                    leftNode->right = new treeNode;
                    strcpy(leftNode->right->info, a->right->info);
                    leftNode->right->left = NULL;
                    leftNode->right->right = NULL;
                    //ramura dreapta f'
                    treePointer rightNode = new treeNode;
                    Duplicate_Tree(a->left, rightNode);
                    // legaturi si derivare
                    a->left = leftNode;
                    a->right = rightNode;
                    Derive(a->right);
                    return a;
                }
            }
        case 's':
            {
                if(a->info[1]=='i')
                //(sin f)'=cos f * f'
                {
                    strcpy(a->info,"*");
                    //ramura stanga cos f
                    treePointer leftNode = new treeNode;
                    strcpy(leftNode->info, "cos");
                    //leftNode->info[3] = '\0';
                    leftNode->left = new treeNode;
                    Duplicate_Tree(a->left,leftNode->left);
                    leftNode->right=NULL;
                    //ramura dreapta f'
                    treePointer rightNode= new treeNode;
                    Duplicate_Tree(a->left, rightNode);
                    //legaturi si derivare
                    a->left=leftNode;
                    a->right=rightNode;
                    Derive(a->right);
                    return a;
                }
                if(a->info[1]=='q')
                {
                    //(sqrt f)'=f'/(2*sqrt f)
                    strcpy(a->info,"/");
                    //ramura stanga f'
                    treePointer leftNode=new treeNode;
                    Duplicate_Tree(a->left,leftNode);
                    //ramura dreapta 2*sqrt f
                    treePointer rightNode=new treeNode;
                    rightNode->info[0]='*';
                    rightNode->info[1]='\0';
                    rightNode->left=new treeNode;
                    rightNode->left->info[0]='2';
                    rightNode->left->info[1]='\0';
                    rightNode->left->left=NULL;
                    rightNode->left->right=NULL;
                    treePointer rightrightNode=new treeNode;
                    //rightNode->right=new treeNode;
                    rightNode->right=rightrightNode;
                    strcpy(rightrightNode->info,"sqrt");
                    rightrightNode->left=new treeNode;
                    Duplicate_Tree(a->left,rightrightNode->left);
                    rightrightNode->right=NULL;
                    //derivare si legaturi
                    a->left=leftNode;
                    a->right=rightNode;
                    Derive(a->left);
                    return a;
                }
            }
        case 'c':
            {
                if(a->info[1]=='o')
                {// (cos f)'=-sin f * f'
                    strcpy(a->info,"*");
                    //ramura stanga -sin f
                    treePointer leftNode=new treeNode;
                    leftNode->info[0]='-';
                    leftNode->info[1]='\0';
                    leftNode->right=NULL;
                    treePointer leftleftNode=new treeNode;
                    leftNode->left=leftleftNode;
                    strcpy(leftleftNode->info,"sin");
                    leftleftNode->left=new treeNode;
                    Duplicate_Tree(a->left,leftleftNode->left);
                    leftleftNode->right=NULL;
                    //ramura dreapta f'
                    treePointer rightNode=new treeNode;
                    Duplicate_Tree(a->left,rightNode);
                    //derivare si legaturi
                    a->left=leftNode;
                    a->right=rightNode;
                    Derive(a->right);
                    return a;
                }
                if(a->info[1]=='t')
                {// (ctg f)'= (-f')/(sin f)^2
                    strcpy(a->info,"/");
                    //ramura stanga (-f')
                    treePointer leftNode=new treeNode;
                    leftNode->info[0]='-';
                    leftNode->info[1]='\0';
                    leftNode->left=new treeNode;
                    leftNode->right=NULL;
                    Duplicate_Tree(a->left,leftNode->left);
                    //ramura dreapta (sin f)^2
                    treePointer rightNode=new treeNode;
                    rightNode->info[0]='^';
                    rightNode->info[1]='\0';
                    treePointer rightleftNode=new treeNode;
                    rightNode->left=rightleftNode;
                    strcpy(rightleftNode->info,"sin");
                    rightleftNode->left=new treeNode;
                    Duplicate_Tree(a->left,rightleftNode->left);
                    rightleftNode->right=NULL;
                    rightNode->right=new treeNode;
                    rightNode->right->info[0]='2';
                    rightNode->right->info[1]='\0';
                    rightNode->right->left=NULL;
                    rightNode->right->right=NULL;
                    //legaturi si derivare
                    a->left=leftNode;
                    a->right=rightNode;
                    Derive(a->left->left);
                    return a;
                }
            }
        case 't':
            {//(tg f)'=(f')/(cos f)^2
                strcpy(a->info,"/");
                //ramura stanga f'
                treePointer leftNode=new treeNode;
                Duplicate_Tree(a->left,leftNode);
                //ramura dreapta (cos f)^2
                treePointer rightNode=new treeNode;
                rightNode->info[0]='^';
                rightNode->info[1]='\0';
                treePointer rightleftNode=new treeNode;
                rightNode->left=rightleftNode;
                strcpy(rightleftNode->info,"cos");
                rightleftNode->left=new treeNode;
                Duplicate_Tree(a->left,rightleftNode->left);
                rightleftNode->right=NULL;
                rightNode->right=new treeNode;
                rightNode->right->info[0]='2';
                rightNode->right->info[1]='\0';
                rightNode->right->left=NULL;
                rightNode->right->right=NULL;
                //legaturi
                a->left=leftNode;
                a->right=rightNode;
                Derive(a->left);
                return a;
            }
        case 'l':
            {//(ln f)'=f'/f
                strcpy(a->info,"/");
                //ramura stanga f'
                treePointer leftNode=new treeNode;
                Duplicate_Tree(a->left,leftNode);
                //ramura dreapta f
                treePointer rightNode=new treeNode;
                Duplicate_Tree(a->left,rightNode);
                //legaturi si derivare
                a->left=leftNode;
                a->right=rightNode;
                Derive(a->left);
                return a;
            }
        case 'a':
            {
                if(a->info[3]=='s')
                {//(arcsin f)'=f'/sqrt(1-f^2)
                    strcpy(a->info,"/");
                    //ramura stanga f'
                    treePointer leftNode=new treeNode;
                    Duplicate_Tree(a->left,leftNode);
                    //ramura dreapta sqrt(1-f^2)
                    treePointer rightNode=new treeNode;
                    strcpy(rightNode->info,"sqrt");
                    rightNode->right=NULL;
                    treePointer rightleftNode=new treeNode;
                    rightNode->left=rightleftNode;
                    rightleftNode->info[0]='-';
                    rightleftNode->info[1]='\0';
                    rightleftNode->left=new treeNode;
                    rightleftNode->left->info[0]='1';
                    rightleftNode->left->info[1]='\0';
                    rightleftNode->left->left=NULL;
                    rightleftNode->left->right=NULL;
                    treePointer rightleftrightNode=new treeNode;
                    rightleftNode->right=rightleftrightNode;
                    rightleftrightNode->info[0]='^';
                    rightleftrightNode->info[1]='\0';
                    rightleftrightNode->left=new treeNode;
                    Duplicate_Tree(a->left,rightleftrightNode->left);
                    rightleftrightNode->right=new treeNode;
                    rightleftrightNode->right->info[0]='2';
                    rightleftrightNode->right->info[1]='\0';
                    rightleftrightNode->right->left=NULL;
                    rightleftrightNode->right->right=NULL;
                    //derivare si legaturi
                    a->left=leftNode;
                    a->right=rightNode;
                    Derive(a->left);
                    return a;
                }
                if(a->info[4]=='o')
                {//(arccos f)'=(-f')/sqrt(1-f^2)
                    strcpy(a->info,"/");
                    //ramura stanga -f'
                    treePointer leftNode=new treeNode;
                    leftNode->info[0]='-';
                    leftNode->info[1]='\0';
                    leftNode->left=new treeNode;
                    leftNode->right=NULL;
                    Duplicate_Tree(a->left,leftNode->left);
                    //ramura dreapta sqrt(1-f^2)
                    treePointer rightNode=new treeNode;
                    strcpy(rightNode->info,"sqrt");
                    rightNode->right=NULL;
                    treePointer rightleftNode=new treeNode;
                    rightNode->left=rightleftNode;
                    rightleftNode->info[0]='-';
                    rightleftNode->info[1]='\0';
                    rightleftNode->left=new treeNode;
                    rightleftNode->left->info[0]='1';
                    rightleftNode->left->info[1]='\0';
                    rightleftNode->left->left=NULL;
                    rightleftNode->left->right=NULL;
                    treePointer rightleftrightNode=new treeNode;
                    rightleftNode->right=rightleftrightNode;
                    rightleftrightNode->info[0]='^';
                    rightleftrightNode->info[1]='\0';
                    rightleftrightNode->left=new treeNode;
                    Duplicate_Tree(a->left,rightleftrightNode->left);
                    rightleftrightNode->right=new treeNode;
                    rightleftrightNode->right->info[0]='2';
                    rightleftrightNode->right->info[1]='\0';
                    rightleftrightNode->right->left=NULL;
                    rightleftrightNode->right->right=NULL;
                    //derivare si legaturi
                    a->left=leftNode;
                    a->right=rightNode;
                    Derive(a->left->left);
                    return a;
                }
                if(a->info[3]=='t')
                {//(arctg)'=f'/(1+f^2)
                    strcpy(a->info,"/");
                    //ramura stanga f'
                    treePointer leftNode=new treeNode;
                    Duplicate_Tree(a->left,leftNode);
                    //ramura dreapta (1+f^2)
                    treePointer rightNode=new treeNode;
                    rightNode->info[0]='+';
                    rightNode->info[1]='\0';
                    rightNode->left=new treeNode;
                    rightNode->left->info[0]='1';
                    rightNode->left->info[1]='\0';
                    rightNode->left->left=NULL;
                    rightNode->left->right=NULL;
                    treePointer rightrightNode=new treeNode;
                    rightNode->right=rightrightNode;
                    rightrightNode->info[0]='^';
                    rightrightNode->info[1]='\0';
                    rightrightNode->left=new treeNode;
                    Duplicate_Tree(a->left,rightrightNode->left);
                    rightrightNode->right=new treeNode;
                    rightrightNode->right->info[0]='2';
                    rightrightNode->right->info[1]='\0';
                    rightrightNode->right->left=NULL;
                    rightrightNode->right->right=NULL;
                    //legaturi si derivare
                    a->left=leftNode;
                    a->right=rightNode;
                    Derive(a->left);
                    return a;
                }
                if(a->info[4]=='t')
                {//(arcctg)'=(-f')/(1+f^2)
                    strcpy(a->info,"/");
                    //ramura stanga -f'
                    treePointer leftNode=new treeNode;
                    leftNode->info[0]='-';
                    leftNode->info[1]='\0';
                    leftNode->left=new treeNode;
                    leftNode->right=NULL;
                    Duplicate_Tree(a->left,leftNode->left);
                    //ramura dreapta 1+f^2
                    treePointer rightNode=new treeNode;
                    rightNode->info[0]='+';
                    rightNode->info[1]='\0';
                    rightNode->left=new treeNode;
                    rightNode->left->info[0]='1';
                    rightNode->left->info[1]='\0';
                    rightNode->left->left=NULL;
                    rightNode->left->right=NULL;
                    treePointer rightrightNode=new treeNode;
                    rightNode->right=rightrightNode;
                    rightrightNode->info[0]='^';
                    rightrightNode->info[1]='\0';
                    rightrightNode->left=new treeNode;
                    Duplicate_Tree(a->left,rightrightNode->left);
                    rightrightNode->right=new treeNode;
                    rightrightNode->right->info[0]='2';
                    rightrightNode->right->info[1]='\0';
                    rightrightNode->right->left=NULL;
                    rightrightNode->right->right=NULL;
                    a->left=leftNode;
                    a->right=rightNode;
                    Derive(a->left->left);
                    return a;
                }
            }
        case 'x':
            a->info[0] = '1';
            a->info[1] = '\0';
            return a;
        default:
            a->info[0] = '0';
            a->info[1] = '\0';
            return a;
    }
}

treePointer Simplify_Derivative_Tree(treePointer &a)
{
    if (a == NULL ) return a;
    switch (a->info[0])
    {
        case '+':
            { // caz f+0
                if (a->right !=NULL && strcmp(a->right->info, "0") == 0) {
                    delete a->right;
                    a = a->left;
                    Simplify_Derivative_Tree(a);
                } // caz 0+f
                else if (a->left !=NULL && strcmp(a->left->info, "0") == 0) {
                    delete a->left;
                    a = a->right;
                    Simplify_Derivative_Tree(a);
                }
                if(a->left!=NULL) Simplify_Derivative_Tree(a->left);
                if(a->right!=NULL) Simplify_Derivative_Tree(a->right);
                return a;
            }
        case '-':
            {
                /// Conditie noua - in Testing /// (-(-1))  -> +1 /// INCEPUT
                if (a->right == NULL && a->left != NULL && a->left->info[0] == '-')
                {
                    a = a->left;
                    a->info[0] = '+';
                    return a;
                }
                /// Conditie noua - in Testing /// (-(-1))  -> +1 /// SFARSIT
                // caz 0-f
                if (a->left !=NULL && a->right != NULL && strcmp(a->left->info, "0") == 0) {
                    delete a->left;
                    a->left=NULL;
                    if (isdigit(a->right->info[0])) {
                        strcat(a->info,a->right->info);
                        a->info[strlen(a->info)]=0;
                        a->right=NULL;
                        if(a->info[1]=='0') strcpy(a->info,"0");
                        return a;
                    }
                    else { Simplify_Derivative_Tree(a->right); return a; }
                }
                else // caz f-0
                    if (a->left !=NULL && a->right != NULL && strcmp(a->right->info, "0") == 0) {
                        delete a->right;
                        a=a->left;
                        Simplify_Derivative_Tree(a);
                    }
                if(a->left!=NULL) Simplify_Derivative_Tree(a->left);
                if(a->right!=NULL) Simplify_Derivative_Tree(a->right);
                return a;
            }
        case '*':
            {   // caz 0*f
                if (a->left !=NULL && strcmp(a->left->info, "0") == 0) a = a->left;
                else // caz f*0
                    if (a->right !=NULL && strcmp(a->right->info, "0") == 0) a = a->right;
                    else // caz 1*f
                        if (a->left !=NULL && strcmp(a->left->info, "1") == 0) { a = a->right; Simplify_Derivative_Tree(a); return a; }
                        else // caz f*1
                            if (a->right !=NULL && strcmp(a->right->info, "1") == 0) { a = a->left; Simplify_Derivative_Tree(a); return a; }
                if(a->left!=NULL) Simplify_Derivative_Tree(a->left);
                if(a->right!=NULL) Simplify_Derivative_Tree(a->right);
                return a;
            }
        case '/':
            {   // caz f/1
                if (a->right !=NULL && strcmp(a->right->info, "1") == 0) { a = a->left; Simplify_Derivative_Tree(a); return a; }
                else // caz 0/f
                    if (a->left !=NULL && strcmp(a->left->info, "0") == 0) { a = a->left; return a; }
                    else // caz f/0
                        if (a->right !=NULL && strcmp(a->right->info, "0") == 0) {
                                strcpy(a->info, "error");
                                a->left = NULL;
                                a->right = NULL;
                                return a;
                            }
                if(a->left!=NULL) Simplify_Derivative_Tree(a->left);
                if(a->right!=NULL) Simplify_Derivative_Tree(a->right);
                return a;
            }
        case '^':
            { // caz f^0
                if (a->right !=NULL && strcmp(a->right->info, "0") == 0) {
                    strcpy(a->info, "1");
                    a->left=NULL;
                    a->right=NULL;
                    return a;
                }
                else //caz f^1
                    if (a->right !=NULL && strcmp(a->right->info, "1") == 0) { a = a->left; Simplify_Derivative_Tree(a); return a; }
                // caz 0^f
                if (a->left !=NULL && strcmp(a->left->info, "0") == 0) { a = a->left; return a;}
                else // caz 1^f
                    if (a->left !=NULL && strcmp(a->left->info, "1") == 0) { a = a->left; return a; }
                if(a->left!=NULL) Simplify_Derivative_Tree(a->left);
                if(a->right!=NULL) Simplify_Derivative_Tree(a->right);
                return a;
            }
        case 'l':
            { // caz ln(e) = 1
                if(a->left !=NULL && strcmp(a->left->info,"e") == 0) {
                    strcpy(a->info,"1");
                    a->left=NULL;
                    a->right=NULL;
                    return a;
                }
                if(a->left!=NULL) Simplify_Derivative_Tree(a->left);
                return a;
            }
        case 's':
            { // caz sin(0) = 0
                if(strcmp(a->left->info,"0")==0) { a = a->left; return a; }
                if(a->left!=NULL) Simplify_Derivative_Tree(a->left);
                return a;
            }
        case 'c':
            { // caz cos(0) = 1
                if(a->info[1]=='o' && strcmp(a->left->info,"0")==0) {
                    a=a->left;
                    strcpy(a->info,"1");
                    return a;
                }
                if(a->left!=NULL) Simplify_Derivative_Tree(a->left);
                return a;
            }
        case 't':
            { // caz tg(0) = 0
                if(strcmp(a->left->info,"0")==0) { a=a->left; return a; }
                if(a->left!=NULL) Simplify_Derivative_Tree(a->left);
                return a;
            }
        case 'a':
            {
                if(a->left!=NULL) Simplify_Derivative_Tree(a->left);
                return a;
            }
        default:
            {
                if(a->left!=NULL) Simplify_Derivative_Tree(a->left);
                if(a->right!=NULL) Simplify_Derivative_Tree(a->right);
                return a;
            }
    }
}


#endif // DERIVE_SIMPLIFY_H_INCLUDED
