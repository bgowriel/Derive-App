#include <graphics.h>

#include "postfix.h"
#include "formula_tree.h"
#include "derive_simplify.h"
#include "draw_math_expression.h"
#include "graphic_menu_read_and_write_to_file.h"

int main()
{
    GenerateAppMenu();
    getch();
    closegraph();
    return 0;
}
