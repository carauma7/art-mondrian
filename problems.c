#include <stdio.h>
#include "utils.h"
#include "graphics.h"
#include "problems.h"
#include "problems/problem_0.h"
#include "problems/lesson01/problem_1.h"
#include "problems/lesson02/problem_2.h"
#include "problems/lesson02/problem_3.h"
#include "problems/lesson03/problem_4.h"
#include "problems/lesson03/problem_5.h"
#include "problems/lesson03/problem_6.h"
#include "eastereggs/devcalc/devcalc.h"
#include "eastereggs/cobra/cobra.h"

void run_selected_problem(int selected)
{
    switch (selected)
    {
        case 0:
            problem_0();
            break;
        case 1:
            problem_1();
            break;
        case 2:
            problem_2();
            break;
        case 3:
            problem_3();
            break;
        case 4:
            problem_4();
            break;
        case 5:
            problem_5();
            break;
        case 6:
            problem_6();
            break;
        case MENU_MAIN_DEVCALC:
            devcalcRun();
            break;
        case MENU_MAIN_COBRA:
            cobraRun();
            break;
        default:
            draw_content_placeholder(selected);
            waitEsc();
            break;
    }
}