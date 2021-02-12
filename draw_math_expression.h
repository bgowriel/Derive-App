#ifndef DRAW_MATH_EXPRESSION_H_INCLUDED
#define DRAW_MATH_EXPRESSION_H_INCLUDED

#include <stdio.h>
#include <graphics.h>

int subformula_nr_of_paranthesis(treePointer a, int &nr_of_paranthesis)
{
    if (a == NULL) return nr_of_paranthesis;
    else {
        switch (character_type(a->info[0])) {
            case 7: // operator *
                {
                    if (((a->left->left != NULL) || (a->left->right != NULL)) && (a->left != NULL && a->left->info[0] != '*' && a->left->info[0] != '^' && a->left->info[0] != '/' && character_type(a->left->info[0]) != 10)) {
                        nr_of_paranthesis += 2; }
                    subformula_nr_of_paranthesis(a->left, nr_of_paranthesis);
                    if (((a->right->left != NULL) || (a->right->right != NULL)) && (a->right != NULL && a->right->info[0] != '*' && a->right->info[0] != '^' && a->right->info[0] != '/' && character_type(a->right->info[0]) != 10)) {
                        nr_of_paranthesis += 2; }
                    subformula_nr_of_paranthesis(a->right, nr_of_paranthesis);
                    break;
                }
            case 8: // operator /
                {   /// La fractie sa nu numar parantezele si sus si jos!
                    subformula_nr_of_paranthesis(a->left, nr_of_paranthesis);
                    //subformula_nr_of_paranthesis(a->right, nr_of_paranthesis);
                    break;
                }
            case 9: // operator ^
                {
                    if ((a->left->left != NULL) || (a->left->right != NULL)) {
                        nr_of_paranthesis += 2;
                        subformula_nr_of_paranthesis(a->left, nr_of_paranthesis);
                    }
                    break;
                }
            case 10: // operatori trigonometrici, sqrt, ln
                {
                    nr_of_paranthesis += 2;
                    subformula_nr_of_paranthesis(a->left, nr_of_paranthesis);
                    break;
                }
            default:
                {
                    if (a->info[0]=='-' && a->right==NULL) nr_of_paranthesis += 2; // minus unar
                    if (strcmp(a->info, "-")==0 && a->right!=NULL && (strcmp(a->right->info, "-")==0 || strcmp(a->right->info, "+")==0)) nr_of_paranthesis += 2;
                    subformula_nr_of_paranthesis(a->left, nr_of_paranthesis);
                    subformula_nr_of_paranthesis(a->right, nr_of_paranthesis);
                    break;
                }
        }
        return nr_of_paranthesis;
    }
}

int subformula_width(treePointer a)
{
    if (a == NULL) return 0;
    else switch (character_type(a->info[0]))
    {
        case 8: return (subformula_width(a->left)>subformula_width(a->right) ? subformula_width(a->left) : subformula_width(a->right));
        case 9: return (subformula_width(a->left) + subformula_width(a->right)*3/4);
        default: return (subformula_width(a->left) + textwidth(a->info) + subformula_width(a->right));
    }
}

int subformula_width_with_paranthesis(treePointer a)
{
    char p[2];
    strcpy(p, "+");
    int nr_of_paranthesis = 0;
    return (subformula_width(a)+subformula_nr_of_paranthesis(a, nr_of_paranthesis)*(textwidth(p)));
}

void div_and_pow_flags(treePointer a, int &div_flag, int &pow_flag)
{
    if (strcmp(a->info, "/") == 0) { div_flag++; return; }
    if (strcmp(a->info, "^") == 0) { pow_flag++; return; }
}

void basic_inorder_walk(treePointer a, int &div_flag, int &pow_flag)
{
    if (a == NULL) return;
    else {
        basic_inorder_walk(a->left, div_flag, pow_flag);
        div_and_pow_flags(a, div_flag, pow_flag);
        basic_inorder_walk(a->right, div_flag, pow_flag);
    }
    return;
}

int subformula_height(treePointer a)
{
    if (a == NULL) return 0;
    int div_flag = 0;
    int pow_flag = 0;
    char c[2];
    strcpy(c, "L");

    basic_inorder_walk(a, div_flag, pow_flag);
    return div_flag*(6+2*textheight(c))+(pow_flag != 0 ? textheight(c)/2 : 4);
}

void fraction_flag(treePointer a, int &div_flag)
{
    if (a==NULL) return;
    if(strcmp(a->info,"/") == 0)
    {
        div_flag = 1;
        return;
    }
    fraction_flag(a->left, div_flag);
    fraction_flag(a->right, div_flag);
}

void Draw_math_formula(treePointer a)
{
    char p1[2], p2[2];
    strcpy(p1, "(");
    strcpy(p2, ")");
    if (!Is_Void_Tree(a))
    {
        switch (character_type(a->info[0])) {
            case 8: // operator /
                {
                    int fraction_width = (subformula_width_with_paranthesis(a->left) > subformula_width_with_paranthesis(a->right) ? subformula_width_with_paranthesis(a->left) : subformula_width_with_paranthesis(a->right));
                    int fraction_x = getx() + 4;
                    int fraction_y = gety() + textheight(p1)/2;

                    line(fraction_x, fraction_y, fraction_x + fraction_width, fraction_y);

                    int div_flag = 0;
                    fraction_flag(a->left, div_flag);
                    if (div_flag == 1) {
                        if (a->left->info[0] == '/') moveto(fraction_x + (fraction_width - subformula_width_with_paranthesis(a->left))/2, fraction_y - subformula_height(a->left) + 6);
                        else moveto(fraction_x + (fraction_width - subformula_width_with_paranthesis(a->left))/2, fraction_y - subformula_height(a->left)/2 - textheight(p1)/2 - 6);
                    }
                    else moveto(fraction_x + (fraction_width - subformula_width_with_paranthesis(a->left))/2, fraction_y - textheight(p1) - 2);

                    Draw_math_formula(a->left);

                    div_flag = 0;
                    fraction_flag(a->right, div_flag);
                    if (div_flag == 1) {
                        if (a->right->info[0] == '/') moveto(fraction_x + (fraction_width - subformula_width_with_paranthesis(a->right))/2, fraction_y + subformula_height(a->right)/2 + 4);
                        else moveto(fraction_x + (fraction_width - subformula_width_with_paranthesis(a->right))/2, fraction_y + subformula_height(a->right)/2 - textheight(p1)/2 + 4);
                    }
                    else moveto(fraction_x + (fraction_width - subformula_width_with_paranthesis(a->right))/2, fraction_y + subformula_height(a->right));

                    Draw_math_formula(a->right);

                    moveto(fraction_x + fraction_width + 4, fraction_y - textheight(p1)/2);

                    return;
                }
            case 9: // operator ^
                {
                    // caz (f)^n
                    if ((a->left->left != NULL) || (a->left->right != NULL)) outtext(p1);
                    Draw_math_formula(a->left);
                    if ((a->left->left != NULL) || (a->left->right != NULL)) outtext(p2);

                    moveto(getx(), gety()-textheight(p1)/5);
                    settextstyle(8, HORIZ_DIR, 1);

                    if ((a->right->left != NULL) || (a->right->right != NULL)) outtext(p1);

                    Draw_math_formula(a->right);

                    if ((a->right->left != NULL) || (a->right->right != NULL)) outtext(p2);

                    settextstyle(8, HORIZ_DIR, 3);
                    moveto(getx(), gety()+textheight(p1)/5);
                    return;
                }
            default:
                {
            // ramura stanga
                    // operator *
                    if ((character_type(a->info[0]) == 7) && ((a->left->left != NULL) || (a->left->right != NULL)) && (a->left != NULL && a->left->info[0] != '*' && a->left->info[0] != '^' && a->left->info[0] != '/' && character_type(a->left->info[0]) != 10)) outtext(p1);
                    // operatori trigonometrici
                    if (character_type(a->info[0]) == 10) {
                        outtext(a->info);
                        outtext(p1);
                    }
                    // caz general - coboram pe ramura stanga
                    if (!(a->info[0]=='-' && a->right==NULL)) Draw_math_formula(a->left); // toate cazurile fara minus unar
                    else {
                        if (a->left!=NULL && character_type(a->left->info[0])==10)
                        { // caz ex: (-cos(x))' -> -(-sin(x))
                            outtext(a->info);
                            Draw_math_formula(a->left);
                            return;
                        }
                        else
                        { // caz ex: (x*(-2))' -> (-2)
                            outtext(p1);
                            outtext(a->info);
                            Draw_math_formula(a->left);
                            outtext(p2);
                            return;
                        }
                    }
                    // operator *
                    if ((character_type(a->info[0]) == 7) && ((a->left->left != NULL) || (a->left->right != NULL)) && (a->left != NULL && a->left->info[0] != '*' && a->left->info[0] != '^' && a->left->info[0] != '/' && character_type(a->left->info[0]) != 10)) outtext(p2);
                    // operatori trigonometrici
                    if (character_type(a->info[0]) == 10) {
                        outtext(p2);
                        return;
                    }
            // root
                    if (a->info[0] != '*') outtext(a->info);
                        else { readimagefile("mul.jpg", getx(), gety(), getx()+15, gety()+24); moveto(getx()+15, gety()); }

            // ramura dreapta
                    // caz f*g -> (f)*(g)
                    if ((character_type(a->info[0]) == 7) && ((a->right->left != NULL) || (a->right->right != NULL)) && (a->right != NULL && a->right->info[0] != '*' && a->right->info[0] != '^' && a->right->info[0] != '/' && character_type(a->right->info[0]) != 10)) outtext(p1);
                    // caz g-f -> g-(f)
                    if (strcmp(a->info, "-")==0 && a->right!=NULL && (strcmp(a->right->info, "-")==0 || strcmp(a->right->info, "+")==0)) outtext(p1);

                    Draw_math_formula(a->right);
                    // caz f*g -> (f)*(g)
                    if ((character_type(a->info[0]) == 7) && ((a->right->left != NULL) || (a->right->right != NULL)) && (a->right != NULL && a->right->info[0] != '*' && a->right->info[0] != '^' && a->right->info[0] != '/' && character_type(a->right->info[0]) != 10)) outtext(p2);
                    // caz g-f -> g-(f)
                    if (strcmp(a->info, "-")==0 && a->right!=NULL && (strcmp(a->right->info, "-")==0 || strcmp(a->right->info, "+")==0)) outtext(p2);
                    return;
                }
        }
    }
}

bool Back_button()
{
    int y = mousey();
    int x = mousex();
    if (y>480 && y<525 && x>940 && x<1025) return true;
    else return false;
}

void Main_Function_To_Draw_Formula(treePointer a)
{
    setlinestyle(0, 0, 2);
    setcolor(0);
    setbkcolor(15);
    Draw_math_formula(a);
    setcolor(15);
    setbkcolor(0);
    return;
}

#endif // DRAW_MATH_EXPRESSION_H_INCLUDED
