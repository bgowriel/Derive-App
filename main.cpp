#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <string.h>
#include <graphics.h>
#include <cmath>

//#define tipDate int

#define L_MAX 20
#define EXPR_MAX 6

using namespace std;

struct nodArb {
    char info[6];
    nodArb* st;
    nodArb* dr;
};
typedef nodArb* arbore;

struct nodStiva {
    arbore opr;
    nodStiva* urm;
};

struct lista_stiva {
    nodStiva* varf;
    int nrElemente;
};

bool TestVida(lista_stiva S) {
    return S.nrElemente == 0;
}

void InitStiva(lista_stiva &S) {
    S.varf = NULL;
    S.nrElemente = 0;
}

void Push(lista_stiva &S, arbore &termen)
{
    nodStiva *nod_nou;
    if (TestVida(S))
    {
        S.nrElemente = 1;
        S.varf = new nodStiva;
        S.varf->opr = termen;
        S.varf->urm = NULL;
    }
    else
    {
        S.nrElemente++;
        nod_nou = new nodStiva;
        nod_nou->opr = termen;
        nod_nou->urm = S.varf;
        S.varf = nod_nou;
    }
}

arbore Pop(lista_stiva &S) {
    if (!TestVida(S))
    {
        arbore termen;
        nodStiva *varf_nou;
        termen = S.varf->opr;
        varf_nou = S.varf->urm;
        delete S.varf;
        S.varf = varf_nou;
        S.nrElemente--;
        return termen;
    }
    else
    {
        //cout << "\n Stiva este goala";
        return NULL;
    }
}

int EsteArboreNul(arbore a)
{
    return (a == NULL);
}

void InitArbore(arbore &a)
{
    if (!EsteArboreNul(a)) a = NULL;
}

/*
void parcurgereInPreordine(arbore a)
{
    if (!EsteArboreNul(a))
    {
        cout << a->num << ", ";
        parcurgereInPreordine(a->st);
        parcurgereInPreordine(a->dr);
    }
}

void parcurgereInPostordine(arbore a)
{
    if (!EsteArboreNul(a))
    {
        parcurgereInPostordine(a->st);
        parcurgereInPostordine(a->dr);
        cout << a->num << ", ";
    }
}

void parcurgereInInordine(arbore a)
{
    if (!EsteArboreNul(a))
    {
        parcurgereInInordine(a->st);
        cout << a->num <<", ";
        parcurgereInInordine(a->dr);
    }
}*/

int NrNiveluri(arbore a)
{
    if (a == NULL) return 0;
    else {
        int nivSt = NrNiveluri(a->st);
        int nivDr = NrNiveluri(a->dr);
        return 1 + (nivDr > nivSt ? nivDr : nivSt);
    }
}

int NrColoane(arbore a)
{
    if (a == NULL) return 0;
    else {
        int colSt = NrColoane(a->st);
        int colDr = NrColoane(a->dr);
        return 1 + colDr + colSt;
    }
}

void ReprezentareArb(arbore a, int coloana, int nivel)
{
    if (a!=NULL)
    {
        setcolor(0);
        setfillstyle(1, 9);
        fillellipse(coloana*60 - 30, nivel*60 - 30, 17, 17);
        setcolor(15);
        //char num[3];
        //sprintf(num, "%d", a->num);
        outtextxy(coloana*60 - 43, nivel*60 - 40, a->info);

        if (!EsteArboreNul(a->st)) {
            arbore b = a->st;
            ReprezentareArb(a->st, coloana - NrColoane(b->dr) - 1, nivel + 1);
        }
        if (!EsteArboreNul(a->dr)) {
            arbore b = a->dr;
            ReprezentareArb(a->dr, coloana + NrColoane(b->st) + 1, nivel + 1);
        }
    }
    else return;
}

void TrasareLinii(arbore a, int coloana, int nivel, int x_ant, int y_ant)
{
    if (a!=NULL)
    {
        if (x_ant && y_ant) line(x_ant, y_ant, coloana*60 - 30, nivel*60 - 30);
        if (!EsteArboreNul(a->st)) {
            arbore b = a->st;
            TrasareLinii(a->st, coloana - NrColoane(b->dr) - 1, nivel + 1, coloana*60 - 30, nivel*60 - 30);
        }
        if (!EsteArboreNul(a->dr)) {
            arbore b = a->dr;
            TrasareLinii(a->dr, coloana + NrColoane(b->st) + 1, nivel + 1, coloana*60 - 30, nivel*60 - 30);
        }
    }
    else return;
}

void ReadPostfixExpr(FILE* f, char expr_postfx[L_MAX][EXPR_MAX], int &lung)
{
    int i = 0, j;
    char c;
    while (getc(f) != EOF)
    {
        fseek(f, -sizeof(char), SEEK_CUR);
        j = 0;
        while (getc(f) != EOF)
        {
            fseek(f, -sizeof(char), SEEK_CUR);
            fscanf(f, "%c", &c);
            if (c != ' ') expr_postfx[i][j] = c;
            else {
                expr_postfx[i][j] = '\0';
                break;
            }
            j++;
		};
		if (expr_postfx[i][j] != '\0') expr_postfx[i][j] = '\0';
        i++;
    }
    expr_postfx[i][0] = '\0';
    lung = i;
    return;
}

arbore MakeTreeNormalExpr(lista_stiva &S, arbore &a, char expr_postfx[L_MAX][EXPR_MAX], int &lung)
{
    /*citesc info pana la un operator
    cand am ajuns la un operator scot din stiva ultimii doi operanzi
    fac mini-arborele*/
    int i = 0;
    while (expr_postfx[i][0] != '\0')
    {
        switch (expr_postfx[i][0])
        {
            case '+':
                a = new nodArb;
                strcpy(a->info, expr_postfx[i]);
                a->dr = Pop(S);
                a->st = Pop(S);
                Push(S, a);
                break;
            case '-':
                if (expr_postfx[i][1] == '\0') {
                    a = new nodArb;
                    strcpy(a->info, expr_postfx[i]);
                    a->dr = Pop(S);
                    a->st = Pop(S);
                    Push(S, a);
                    break;
                }
            case '*':
                a = new nodArb;
                strcpy(a->info, expr_postfx[i]);
                a->dr = Pop(S);
                a->st = Pop(S);
                Push(S, a);
                break;
            case '/':
                a = new nodArb;
                strcpy(a->info, expr_postfx[i]);
                a->dr = Pop(S);
                a->st = Pop(S);
                Push(S, a);
                break;
            case '^':
                a = new nodArb;
                strcpy(a->info, expr_postfx[i]);
                a->dr = Pop(S);
                a->st = Pop(S);
                Push(S, a);
                break;
            default:
                a = new nodArb;
                strcpy(a->info, expr_postfx[i]);
                a->st = NULL;
                a->dr = NULL;
                Push(S, a);
        }
        i++;
    }
    return a;
}

void DuplicateArb(arbore a, arbore &b)
{
    if (a != NULL) {
        strcpy(b->info, a->info);
        if (a->st != NULL) {
            b->st = new nodArb;
            DuplicateArb(a->st, b->st);
        }
        else b->st = NULL;
        if (a->dr != NULL) {
            b->dr = new nodArb;
            DuplicateArb(a->dr, b->dr);
        }
        else b->dr = NULL;
    }
}

arbore Deriveaza(arbore &a)
{
    if (a == NULL) return a;
    switch (a->info[0])
    {
        case '+': // (f+g)'=f'+g'
            a->st = Deriveaza(a->st);
            a->dr = Deriveaza(a->dr);
            return a;
        case '-': // (f-g)'=f'-g'
            if (a->info[1] == '\0') {
                a->st = Deriveaza(a->st);
                a->dr = Deriveaza(a->dr);
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
                arbore leftNode = new nodArb;
                leftNode->info[0] = '*';
                leftNode->info[1] = '\0';
                leftNode->st = new nodArb;
                DuplicateArb(a->st, leftNode->st);
                leftNode->dr = new nodArb;
                DuplicateArb(a->dr, leftNode->dr);
                // ramura dreapta g'*f
                arbore rightNode = new nodArb;
                rightNode->info[0] = '*';
                rightNode->info[1] = '\0';
                rightNode->st = new nodArb;
                DuplicateArb(a->dr, rightNode->st);
                rightNode->dr = new nodArb;
                DuplicateArb(a->st, rightNode->dr);
                // legaturi si derivare
                a->st = leftNode;
                a->dr = rightNode;
                a->st->st = Deriveaza(leftNode->st);
                a->dr->st = Deriveaza(rightNode->st);
                return a;
            }
        case '/': //(f/g)'=(f'*g-g'*f)/(g^2)
            {
                a->info[0] = '/'; // root
                // ramura stanga f'*g-g'*f
                arbore leftNode = new nodArb;
                leftNode->info[0] = '-';
                leftNode->info[1] = '\0';
                //ramura stanga-stanga f'*g
                arbore leftleftNode = new nodArb;
                leftNode->st = leftleftNode;
                leftleftNode->info[0] = '*';
                leftleftNode->info[1] = '\0';
                leftleftNode->st = new nodArb;
                DuplicateArb(a->st, leftleftNode->st);
                leftleftNode->dr = new nodArb;
                DuplicateArb(a->dr, leftleftNode->dr);
                //ramura stanga-dreapta g'*f
                arbore leftrightNode = new nodArb;
                leftNode->dr = leftrightNode;
                leftrightNode->info[0] = '*';
                leftrightNode->info[1] = '\0';
                leftrightNode->st = new nodArb;
                DuplicateArb(a->dr, leftrightNode->st);
                leftrightNode->dr = new nodArb;
                DuplicateArb(a->st, leftrightNode->dr);
                // ramura dreapta g^2
                arbore rightNode = new nodArb;
                rightNode->info[0] = '^';
                rightNode->info[1] = '\0';
                rightNode->st = new nodArb;
                DuplicateArb(a->dr, rightNode->st);
                rightNode->dr = new nodArb;
                rightNode->dr->info[0]='2';
                rightNode->dr->info[1]='\0';
                // legaturi si derivare
                a->st = leftNode;
                a->dr = rightNode;
                a->st->st->st = Deriveaza(leftleftNode->st);
                a->st->dr->st = Deriveaza(leftrightNode->st);
                return a;
            }
        case '^': // (f^n)'= (f^(n-1))*n*f'
            {
                a->info[0] = '*'; // root
                // ramura stanga (f^(n-1))*n
                arbore leftNode = new nodArb;
                leftNode->info[0] = '*';
                leftNode->info[1] = '\0';
                arbore leftleftNode = new nodArb;
                leftNode->st = leftleftNode;
                leftleftNode->info[0] = '^';
                leftleftNode->info[1] = '\0';
                leftleftNode->st = new nodArb;
                DuplicateArb(a->st, leftleftNode->st);
                leftleftNode->dr = new nodArb;
                int exponent;
                sscanf(a->dr->info, "%d", &exponent);
                exponent--;
                sprintf(leftleftNode->dr->info, "%d", exponent);
                leftNode->dr = new nodArb;
                strcpy(leftNode->dr->info, a->dr->info);
                //ramura dreapta f'
                arbore rightNode = new nodArb;
                DuplicateArb(a->st, rightNode);
                // legaturi si derivare
                a->st = leftNode;
                a->dr = rightNode;
                a->dr = Deriveaza(rightNode);
                return a;
            }
        case 'x':
            a->info[0] = '1';
            return a;
        default:
            a->info[0] = '0';
            a->info[1] = '\0';
            return a;
    }
}

int main()
{
    int lungime;
    char postfixat[L_MAX][EXPR_MAX];
    FILE* fread;
    arbore a;
    InitArbore(a);
    lista_stiva S;
    InitStiva(S);

    if ((fread = fopen("postfixat.txt", "r")) == NULL) {
            printf("Error! opening file");
            exit(1);
    }
    ReadPostfixExpr(fread, postfixat, lungime);
    fclose(fread);

    for (int i=0; i<lungime; i++) cout << postfixat[i] << " ";

    cout << '\n';
    a = MakeTreeNormalExpr(S, a, postfixat, lungime);

    /*char comanda;
    do {
        comanda = meniu();
        switch (comanda)
        {
            case 'c':
                cout << " Dati numarul de elemente ale arborelui: ";
                cin >> n;
                for (i=1; i<=n; i++)
                {
                    cout << " Dati elementul al "<<i<<"-lea: ";
                    cin >> x;
                    adaugaLaArboreElement(a, x);
                }
                break;
            case 'g':
                Goleste(a);
                initArbore(a);
                break;
            case 'f':
                tipDate nr_cautat;
                cout << " Care este numarul cautat? ";
                cin >> nr_cautat;
                if (existaElementInArbore(a, nr_cautat)) cout << "\n Numarul se afla in arbore. \n";
                else cout << "\n Numarul nu se afla in arbore. \n";
                break;
            case 'x':
                cout << " Eliminam elementul: ";
                //cin >> nr_elim;
                //EliminaNr(S, nr_elim);
                break;
            case 'p':
                cout << "\n Elementele parcurse in inordine:\n";
                parcurgereInInordine(a);
                cout << "\n Elementele parcurse in preordine:\n";
                parcurgereInPreordine(a);
                cout << "\n Elementele parcurse in postordine:\n";
                parcurgereInPostordine(a);
                break;
            case 'r':
                cout << "\n Nr coloane = " << NrColoane(a) << "\n Nr niveluri = " << NrNiveluri(a);
                initwindow(100*NrColoane(a), 100*NrNiveluri(a));
                setlinestyle(0, 0, 3);
                setfillstyle(SOLID_FILL, 15);
                floodfill(1, 1, 15);
                setcolor(0);
                TrasareLinii(a, NrColoane(a->st) + 1, 1, 0, 0);
                settextstyle(9, HORIZ_DIR, 2);
                ReprezentareArb(a, NrColoane(a->st) + 1, 1);
                break;
        }
    } while (comanda!='i');*/

    cout << "\n Nr coloane = " << NrColoane(a) << "\n Nr niveluri = " << NrNiveluri(a);
    initwindow(60*NrColoane(a), 60*NrNiveluri(a));
    setlinestyle(0, 0, 2);
    setfillstyle(SOLID_FILL, 15);
    floodfill(1, 1, 15);
    setcolor(6);
    TrasareLinii(a, NrColoane(a->st) + 1, 1, 0, 0);
    settextstyle(0, HORIZ_DIR, 2);
    ReprezentareArb(a, NrColoane(a->st) + 1, 1);
    //getch();
    //closegraph();
    a = Deriveaza(a);
    //arbore b = new nodArb;
    //DuplicateArb(a, b);
    cout << "\n Nr coloane = " << NrColoane(a) << "\n Nr niveluri = " << NrNiveluri(a);
    initwindow(60*NrColoane(a), 60*NrNiveluri(a));
    setlinestyle(0, 0, 2);
    setfillstyle(SOLID_FILL, 15);
    floodfill(1, 1, 15);
    setcolor(6);
    TrasareLinii(a, NrColoane(a->st) + 1, 1, 0, 0);
    settextstyle(0, HORIZ_DIR, 2);
    ReprezentareArb(a, NrColoane(a->st) + 1, 1);
    getch();
    closegraph();
    getch();
    closegraph();
    return 0;
}
