#ifndef GRAPHIC_MENU_READ_AND_WRITE_TO_FILE_H_INCLUDED
#define GRAPHIC_MENU_READ_AND_WRITE_TO_FILE_H_INCLUDED

#include <stdio.h>
#include <graphics.h>
#include <shellapi.h>

#define ON 1
#define OFF 0

/// FUNCTIONS FOR GRAPHIC MENU, READING & WRITING TO FILES ///

void doCursor(int on)
{
    int cursX, textColor;
    char cursor[2] = { '_',0 };
    if (!on) {
      textColor = getcolor();
      setcolor(getbkcolor());
    }
    cursX=getx();
    outtext(cursor);
    moveto(cursX,gety());
    if (!on) setcolor(textColor);
}

void newLine()
{
    char c[2] = {'A', 0};
    moveto(130, gety() + textheight(c)+5);
}

void In_Order_Walk_Through_Tree(treePointer a)
{
    if (getx()>930) newLine();
    char p1[2], p2[2];
    strcpy(p1, "(");
    strcpy(p2, ")");
    if (!Is_Void_Tree(a))
    {   // caz f/ceva -> (f)/ceva si caz f^n -> (f)^n
        if ((7<character_type(a->info[0]) && character_type(a->info[0])<10) && ((a->left->left != NULL) || (a->left->right != NULL))) outtext(p1);
        // caz f*ceva -> (f)*ceva
        if ((character_type(a->info[0]) == 7) && ((a->left->left != NULL) || (a->left->right != NULL)) && (a->left != NULL && a->left->info[0] != '*')) outtext(p1);
        // caz functii trigonometrice ()
        if (character_type(a->info[0]) == 10) {
                outtext(a->info);
                outtext(p1);
        }
        // caz general - coboram pe ramura stanga
        if (!(a->info[0]=='-' && a->right==NULL)) In_Order_Walk_Through_Tree(a->left);
        else {
            if (a->left!=NULL && character_type(a->left->info[0])==10)
            { // caz ex: (-cos(x))' -> -(-sin(x))
                outtext(a->info);
                In_Order_Walk_Through_Tree(a->left);
                return;
            }
            else
            { // caz ex: (x*(-2))' -> (-2)
                outtext(p1);
                outtext(a->info);
                In_Order_Walk_Through_Tree(a->left);
                outtext(p2);
                return;
            }
        }

        if ((7<character_type(a->info[0]) && character_type(a->info[0])<10) && ((a->left->left != NULL) || (a->left->right != NULL))) outtext(p2);
        if ((character_type(a->info[0]) == 7) && ((a->left->left != NULL) || (a->left->right != NULL)) && (a->left != NULL && a->left->info[0] != '*')) outtext(p2);
        if (character_type(a->info[0]) == 10) {
            outtext(p2);
            return;
        }
        // root
        outtext(a->info);

        // ramura dreapta
        if ((7<character_type(a->info[0]) && character_type(a->info[0])<10) && ((a->right->left != NULL) || (a->right->right != NULL))) outtext(p1);
        if ((character_type(a->info[0]) == 7) && ((a->right->left != NULL) || (a->right->right != NULL)) && (a->right != NULL && a->right->info[0] != '*')) outtext(p1);
        // caz ceva-f -> ceva-(f)
        if (strcmp(a->info, "-")==0 && a->right!=NULL && (strcmp(a->right->info, "-")==0 || strcmp(a->right->info, "+")==0)) outtext(p1);

        In_Order_Walk_Through_Tree(a->right);

        if ((7<character_type(a->info[0]) && character_type(a->info[0])<10) && ((a->right->left != NULL) || (a->right->right != NULL))) outtext(p2);
        if ((character_type(a->info[0]) == 7) && ((a->right->left != NULL) || (a->right->right != NULL)) && (a->right != NULL && a->right->info[0] != '*')) outtext(p2);
        if (strcmp(a->info, "-")==0 && a->right!=NULL && (strcmp(a->right->info, "-")==0 || strcmp(a->right->info, "+")==0)) outtext(p2);
    }
}

void Show_loaded_formula_on_display(char* formula)
{
    int i=0;
    char c[2];
    c[1] = 0;
    while (formula[i] != 0) {
        c[0] = formula[i];
        outtext(c);
        i++;
        if (getx()>950) newLine();
    }
}

void Get_Formula_From_Keyboard_Input(char* formula)
{
    int formulaIndex = 0;
    int xVal[255]; // coordonata x pe grafic a fiecarui caracter din formula
    char inputChar, outString[2]; // caracterul introdus de la tastatura
    int textColor; // culoarea curenta a textului
    outString[1]=0; // terminator de sir
    xVal[0] = getx(); // pornim de la pozitia curenta a cursorului
    do {
        if (xVal[formulaIndex]>950) newLine();
        doCursor(ON); // afisam cursorul
        inputChar = getch();
        doCursor(OFF);
        if (inputChar == 13 || inputChar == 10) break;
        if (inputChar == 0) getch(); // getch returneaza 0 pentru taste nonalfanumerice pe care le refuzam
        else
        {
            if (inputChar==8) { // 8 este BS sau backspace
                textColor = getcolor();
                --formulaIndex;
                if (formulaIndex<0) formulaIndex=0; // nu stergem dincolo de margine
                moveto(xVal[formulaIndex],gety()); // ne mutam fix in fata caracterului curent
                setcolor(getbkcolor()); // scriem peste cu negru
                outString[0] = formula[formulaIndex];
                outtext(outString);
                moveto(xVal[formulaIndex],gety()); // reintoarcem cursorul o pozitie
                setcolor(textColor); // revenim la culoarea textului
            }
            else {
                formula[formulaIndex] = inputChar; // introducem in formula caracterul citit
                outString[0] = inputChar;
                outtext(outString); // il afisam si pe grafic
                ++formulaIndex; // indexam formula
                xVal[formulaIndex] = getx(); // retinem coordonata x a caracterului tocmai introdus
            }
         }
    } while(inputChar!=13 && inputChar!=10); // conditia de oprire LF sau CR adica enter
    formula[formulaIndex]=0; // terminator de sir
}

void Read_formula_from_file(char* formula)
{
    int i = 0;
    char c;
    FILE* f;
    f = fopen("formula.txt", "r");
    while (getc(f) != EOF)
    {
        fseek(f, -sizeof(char), SEEK_CUR);
        fscanf(f, "%c", &c);
        formula[i] = c;
        i++;
    }
    fclose(f);
    formula[i] = 0;
}

void Save_to_file_derivative_formula(treePointer a, FILE *f)
{
    char p1[2], p2[2];
    strcpy(p1, "(");
    strcpy(p2, ")");
    if (!Is_Void_Tree(a))
    {   // caz f/ceva -> (f)/ceva si caz f^n -> (f)^n
        if ((7<character_type(a->info[0]) && character_type(a->info[0])<10) && ((a->left->left != NULL) || (a->left->right != NULL))) fprintf(f, "%s", p1);
        // caz f*ceva -> (f)*ceva
        if ((character_type(a->info[0]) == 7) && ((a->left->left != NULL) || (a->left->right != NULL)) && (a->left != NULL && a->left->info[0] != '*')) fprintf(f, "%s", p1);
        // caz functii trigonometrice ()
        if (character_type(a->info[0]) == 10) {
                fprintf(f, "%s", a->info);
                fprintf(f, "%s", p1);
        }
        // caz general - coboram pe ramura stanga
        if (!(a->info[0]=='-' && a->right==NULL)) Save_to_file_derivative_formula(a->left, f);
        else {
            if (a->left!=NULL && character_type(a->left->info[0])==10)
            { // caz ex: (-cos(x))' -> -(-sin(x))
                fprintf(f, "%s", a->info);
                Save_to_file_derivative_formula(a->left, f);
                return;
            }
            else
            { // caz ex: (x*(-2))' -> (-2)
                fprintf(f, "%s", p1);
                fprintf(f, "%s", a->info);
                Save_to_file_derivative_formula(a->left, f);
                fprintf(f, "%s", p2);
                return;
            }
        }

        if ((7<character_type(a->info[0]) && character_type(a->info[0])<10) && ((a->left->left != NULL) || (a->left->right != NULL))) fprintf(f, "%s", p2);
        if ((character_type(a->info[0]) == 7) && ((a->left->left != NULL) || (a->left->right != NULL)) && (a->left != NULL && a->left->info[0] != '*')) fprintf(f, "%s", p2);
        if (character_type(a->info[0]) == 10) {
            fprintf(f, "%s", p2);
            return;
        }
        // root
        fprintf(f, "%s", a->info);

        // ramura dreapta
        if ((7<character_type(a->info[0]) && character_type(a->info[0])<10) && ((a->right->left != NULL) || (a->right->right != NULL))) fprintf(f, "%s", p1);
        if ((character_type(a->info[0]) == 7) && ((a->right->left != NULL) || (a->right->right != NULL)) && (a->right != NULL && a->right->info[0] != '*')) fprintf(f, "%s", p1);
        // caz ceva-f -> ceva-(f)
        if (strcmp(a->info, "-")==0 && a->right!=NULL && (strcmp(a->right->info, "-")==0 || strcmp(a->right->info, "+")==0)) fprintf(f, "%s", p1);

        Save_to_file_derivative_formula(a->right, f);

        if ((7<character_type(a->info[0]) && character_type(a->info[0])<10) && ((a->right->left != NULL) || (a->right->right != NULL))) fprintf(f, "%s", p2);
        if ((character_type(a->info[0]) == 7) && ((a->right->left != NULL) || (a->right->right != NULL)) && (a->right != NULL && a->right->info[0] != '*')) fprintf(f, "%s", p2);
        if (strcmp(a->info, "-")==0 && a->right!=NULL && (strcmp(a->right->info, "-")==0 || strcmp(a->right->info, "+")==0)) fprintf(f, "%s", p2);
    }
}

int buttonCase(int x, int y)
{
    if (y>400 && y<453) {
        if (x>100 && x<250) return 1;
        if (x>265 && x<477) return 2;
        if (x>492 && x<678) return 3;
        if (x>693 && x<843) return 4;
        if (x>858 && x<1008) return 5;
    }
    if (y>320 && y<360 && x>875 && x<975) return 6;
    return 0;
}

void loadDefaultMenu()
{
    readimagefile("background.jpg", 0, 0, 1100, 591);
    readimagefile("button derive.jpg", 100, 400, 250, 453);
    readimagefile("button loadfromfile.jpg", 265, 400, 477, 453);
    readimagefile("button savetofile.jpg", 492, 400, 678, 453);
    readimagefile("button info.jpg", 693, 400, 843, 453);
    readimagefile("button about.jpg", 858, 400, 1008, 453);
}

void Draw_button(bool light)
{
    if (!light) readimagefile("button draw.jpg", 875, 320, 975, 360);
        else readimagefile("button draw2.jpg", 875, 320, 975, 360);
}

void Read_derive_simplify_and_display_formula(bool load)
{
    // eliberam display-ul, incarcam meniul
    setvisualpage(1);
    cleardevice();
    loadDefaultMenu();
    setvisualpage(2);
    if (!load) readimagefile("button derive2.jpg", 100, 400, 250, 453);
        else readimagefile("button loadfromfile2.jpg", 265, 400, 477, 453);
    // pregatim introducerea formulei sau // ne pregatim sa citim si sa afisam formula din fisier
    moveto(130, 125);
    char formula[MAX];
    char mesaj[30];
    if (!load) strcpy(mesaj, "Insert formula from keyboard:");
        else strcpy(mesaj, "Formula loaded from file:");
    setcolor(9);
    outtext(mesaj);
    setcolor(15);
    newLine();
    // citim sau incarcam formula si o salvam in fisier
    if (!load) Get_Formula_From_Keyboard_Input(formula);
        else { Read_formula_from_file(formula); Show_loaded_formula_on_display(formula); }
    FILE *f;
    f = fopen("new_formula.txt", "w");
    fprintf(f, "Formula:\n\n%s\n", formula);
    // facem din formula un array de token-uri/termeni in ordine postfixata
    char** postfix = Main_Function_To_Create_Postix_From_Infix_Formula(formula);
    // pregatim afisarea derivatei
    newLine();
    strcpy(mesaj, "First order derivative:");
    setcolor(9);
    outtext(mesaj);
    setcolor(15);
    newLine();
    // transformam array-ul de token-uri/termeni intr-un tablou
    char expr_postfx[L_MAX][EXPR_MAX]; int j=0;
    while (postfix[j] != 0) {
        strcpy(expr_postfx[j], postfix[j]);
        j++;
    }
    expr_postfx[j][0]=0;
    // cream arborele formulei, derivam, simplificam, afisam pe display
    treePointer a;
    Initialize_Tree(a);
    stack_of_trees S;
    Initialize_Stack_Of_Trees(S);
    Make_Formula_Tree(S, a, expr_postfx);
    // punem formula in modul draw math;
    int x_page2 = getx();
    int y_page2 = gety();
    setactivepage(3);
    readimagefile("background draw math.jpg", 0, 0, 1100, 591);
    moveto(100, 150 + subformula_height(a)/2);
    Main_Function_To_Draw_Formula(a);
    // derivam formula
    Derive(a);
    Simplify_Derivative_Tree(a);
    Simplify_Derivative_Tree(a);
    Simplify_Derivative_Tree(a);
    //a = Simplify_Derivative_Tree(a);
    // punem formula derivata in modul draw math;
    moveto(100, 340 + subformula_height(a)/2);
    Main_Function_To_Draw_Formula(a);
    // revenim
    setactivepage(2);
    moveto(x_page2, y_page2);
    In_Order_Walk_Through_Tree(a);
    // salvam in fisier derivata de ordin I
    fprintf(f, "\nFirst order derivative:\n\n");
    Save_to_file_derivative_formula(a, f);
    // pregatim afisarea derivatei de ordin II
    newLine();
    strcpy(mesaj, "Second order derivative:");
    setcolor(9);
    outtext(mesaj);
    setcolor(15);
    newLine();
    // facem derivata de ordin II, simplificam, afisam
    Derive(a);
    Simplify_Derivative_Tree(a);
    Simplify_Derivative_Tree(a);
    Simplify_Derivative_Tree(a);
    In_Order_Walk_Through_Tree(a);
    // salvam in fisier derivata de ordin II
    fprintf(f, "\n\nSecond order derivative:\n\n");
    Save_to_file_derivative_formula(a, f);
    Draw_button(true);
    fclose(f);
}

void Reset_button_to_default()
{
    readimagefile("button derive.jpg", 100, 400, 250, 453);
    readimagefile("button loadfromfile.jpg", 265, 400, 477, 453);
    readimagefile("button savetofile.jpg", 492, 400, 678, 453);
    readimagefile("button info.jpg", 693, 400, 843, 453);
    readimagefile("button about.jpg", 858, 400, 1008, 453);
}

void Left_Button_Mouse_Click(int x, int y)
{
    switch (buttonCase(x, y)) {
        case 1: /// DERIVE!
            {
                Read_derive_simplify_and_display_formula(false);
                // asteptam comanda urmatoare
                clearmouseclick(WM_LBUTTONDOWN);
                while (1) {
                    if (ismouseclick(WM_LBUTTONDOWN)) {
                        if (buttonCase(mousex(), mousey()) == 6) {
                            //Draw_button(false);
                            setvisualpage(3);
                            clearmouseclick(WM_LBUTTONDOWN);
                            while (1) {
                                if (ismouseclick(WM_LBUTTONDOWN)) {
                                    if (Back_button()) {
                                        setactivepage(2);
                                        setvisualpage(2);
                                        //setcolor(15);
                                        //setbkcolor(0);
                                        clearmouseclick(WM_LBUTTONDOWN);
                                        break;
                                    }
                                    else clearmouseclick(WM_LBUTTONDOWN);
                                }
                            }
                        }
                        else Left_Button_Mouse_Click(mousex(), mousey());
                        return;
                    }
                }
            }
        case 2: /// LOAD
            {
                Read_derive_simplify_and_display_formula(true);
                // asteptam comanda urmatoare
                clearmouseclick(WM_LBUTTONDOWN);
                while (1) {
                    if (ismouseclick(WM_LBUTTONDOWN)) {
                        if (buttonCase(mousex(), mousey()) == 6) {
                            setvisualpage(3);
                            clearmouseclick(WM_LBUTTONDOWN);
                            while (1) {
                                if (ismouseclick(WM_LBUTTONDOWN)) {
                                    if (Back_button()) {
                                        setactivepage(2);
                                        setvisualpage(2);
                                        //setcolor(15);
                                        //setbkcolor(0);
                                        clearmouseclick(WM_LBUTTONDOWN);
                                        break;
                                    }
                                    else clearmouseclick(WM_LBUTTONDOWN);
                                }
                            }
                        }
                        else Left_Button_Mouse_Click(mousex(), mousey());
                        return;
                    }
                }
            }
        case 3: /// SAVE
            {
                Reset_button_to_default();
                readimagefile("button savetofile2.jpg", 492, 400, 678, 453);
                ShellExecuteA(NULL, "open", "new_formula.txt", NULL, NULL, 5);
                clearmouseclick(WM_LBUTTONDOWN);
                while (1) {
                    if (ismouseclick(WM_LBUTTONDOWN)) {
                        Left_Button_Mouse_Click(mousex(), mousey());
                        return;
                    }
                }
            }
        case 4: /// INFO
            {
                Reset_button_to_default();
                readimagefile("button info2.jpg", 693, 400, 843, 453);
                readimagefile("info.jpg", 0, 457, 1100, 591);
                clearmouseclick(WM_LBUTTONDOWN);
                while (1) {
                    if (ismouseclick(WM_LBUTTONDOWN)) {
                        Left_Button_Mouse_Click(mousex(), mousey());
                        return;
                    }
                }
            }
        case 5: /// ABOUT
            {
                Reset_button_to_default();
                readimagefile("button about2.jpg", 858, 400, 1008, 453);
                readimagefile("about.jpg", 0, 457, 1100, 591);
                clearmouseclick(WM_LBUTTONDOWN);
                while (1) {
                    if (ismouseclick(WM_LBUTTONDOWN)) {
                        Left_Button_Mouse_Click(mousex(), mousey());
                        return;
                    }
                }
            }
        default: return;
    }
}

void GenerateAppMenu()
{
    initwindow(1100, 591, "Derive App", 100, 100, true, true);
    setactivepage(1);
    loadDefaultMenu();
    setactivepage(2);
    loadDefaultMenu();
    setvisualpage(1);
    setcolor(15);
    settextstyle(8, HORIZ_DIR, 3);
    settextjustify(0, 2);
    while (1)
    {
        switch (buttonCase(mousex(), mousey())) {
            case 1:
                readimagefile("button derive2.jpg", 100, 400, 250, 453);
                setvisualpage(2);
                if (ismouseclick(WM_LBUTTONDOWN)) Left_Button_Mouse_Click(mousex(), mousey());
                clearmouseclick(WM_LBUTTONDOWN);
                break;
            case 2:
                readimagefile("button loadfromfile2.jpg", 265, 400, 477, 453);
                setvisualpage(2);
                if (ismouseclick(WM_LBUTTONDOWN)) Left_Button_Mouse_Click(mousex(), mousey());
                clearmouseclick(WM_LBUTTONDOWN);
                break;
            case 3:
                readimagefile("button savetofile2.jpg", 492, 400, 678, 453);
                setvisualpage(2);
                if (ismouseclick(WM_LBUTTONDOWN)) Left_Button_Mouse_Click(mousex(), mousey());
                clearmouseclick(WM_LBUTTONDOWN);
                break;
            case 4:
                readimagefile("button info2.jpg", 693, 400, 843, 453);
                setvisualpage(2);
                if (ismouseclick(WM_LBUTTONDOWN)) Left_Button_Mouse_Click(mousex(), mousey());
                clearmouseclick(WM_LBUTTONDOWN);
                break;
            case 5:
                readimagefile("button about2.jpg", 858, 400, 1008, 453);
                setvisualpage(2);
                if (ismouseclick(WM_LBUTTONDOWN)) Left_Button_Mouse_Click(mousex(), mousey());
                clearmouseclick(WM_LBUTTONDOWN);
                break;
            default:
                setvisualpage(1);
                loadDefaultMenu();
                break;
        }
    }
}

#endif // GRAPHIC_MENU_READ_AND_WRITE_TO_FILE_H_INCLUDED
