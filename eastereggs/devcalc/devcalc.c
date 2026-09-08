#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "devcalc.h"

// Cantos da área interna preta desenhada por draw_problem_screen() (x: 8..73, y: 6..22).
#define DEVCALC_X 9
#define DEVCALC_Y_TOP 6
#define DEVCALC_Y_BOTTOM 22

// Limpa apenas a área interna preta, preservando a moldura Mondrian ao redor.
static void devcalc_clear_box(void)
{
    int x, y;

    textbackground(BLACK);
    for (y = DEVCALC_Y_TOP; y <= DEVCALC_Y_BOTTOM; y++)
    {
        for (x = 8; x <= 73; x++)
        {
            gotoxy(x, y);
            printf(" ");
        }
    }
}

// Detecta identificadores alfabéticos que não são "x", "e", "pi" nem uma das funções
// suportadas (o parser não valida isso e trava com entradas como "sin(x)").
static bool devcalc_has_unknown_token(char *str)
{
    int i = 0, len = strlen(str);

    while (i < len)
    {
        if (isalpha((unsigned char) str[i]))
        {
            int j = i;
            while (j < len && isalpha((unsigned char) str[j]))
            {
                j++;
            }

            int name_len = j - i;
            char name[8];

            if (name_len >= (int) sizeof(name))
            {
                return true;
            }
            strncpy(name, str + i, name_len);
            name[name_len] = 0;

            bool is_var_or_const =
                (strcmp(name, "x") == 0) || (strcmp(name, "e") == 0) || (strcmp(name, "pi") == 0);
            bool is_known_func =
                (strcmp(name, "ln") == 0) || (strcmp(name, "log") == 0) ||
                (strcmp(name, "sen") == 0) || (strcmp(name, "cos") == 0) ||
                (strcmp(name, "tan") == 0) || (strcmp(name, "csc") == 0) ||
                (strcmp(name, "sec") == 0) || (strcmp(name, "cot") == 0) ||
                (strcmp(name, "senh") == 0) || (strcmp(name, "cosh") == 0) ||
                (strcmp(name, "tanh") == 0) || (strcmp(name, "csch") == 0) ||
                (strcmp(name, "sech") == 0) || (strcmp(name, "coth") == 0);

            if (!is_var_or_const && !is_known_func)
            {
                return true;
            }

            i = j;
        }
        else
        {
            i++;
        }
    }

    return false;
}

void devcalcRun(void)
{
    char *m_func = (char *) malloc(sizeof(char) * MAX_CHAR);
    char *derv;
    int line = DEVCALC_Y_TOP;

    draw_problem_screen(DEVCALC_PROBLEM, "");

    textbackground(BLACK);
    textcolor(CYAN);
    gotoxy(DEVCALC_X, line);
    printf("CALCULADORA DE DERIVADAS -- versão: 0.1");
    line += 2;
    gotoxy(DEVCALC_X, line);
    printf("(?) Insira uma expressão e pressione ENTER.");
    line++;
    gotoxy(DEVCALC_X, line);
    printf("(?) Exemplo: ln(x) + cos(x^3) + x");
    line += 2;
    gotoxy(DEVCALC_X, line);
    printf("(?) Para finalizar, digite \"sair\".");
    line += 2;

    textcolor(GREEN);
    gotoxy(DEVCALC_X, line);
    printf("Entrada: ");
    textcolor(WHITE);
    fgets(m_func, MAX_CHAR, stdin);
    m_func[strlen(m_func) - 1] = 0;
    m_func = wo_space(m_func);

    while (strcmp(m_func, "sair") != 0)
    {
        line++;
        if (line > DEVCALC_Y_BOTTOM)
        {
            devcalc_clear_box();
            line = DEVCALC_Y_TOP;
        }
        gotoxy(DEVCALC_X, line);

        if (!par_paired(m_func, strlen(m_func)))
        {
            textcolor(RED);
            printf("Número de parênteses abertos/fechados é desigual.");
        }
        else if (devcalc_has_unknown_token(m_func))
        {
            textcolor(RED);
            printf("Expressão não reconhecida.");
        }
        else
        {
            derv = differentiate(simp_input(m_func), 1);
            derv = simp_output(derv);

            while (par_enclosed(derv))
            {
                derv = rm_par(derv);
            }

            textcolor(GREEN);
            printf("Resultado: ");
            textcolor(YELLOW);
            if (strcmp(derv, "") == 0)
            {
                printf("0");
            }
            else if (*derv == '+')
            {
                printf("%s", derv + 1);
            }
            else
            {
                printf("%s", derv);
            }
        }

        line += 2;
        if (line > DEVCALC_Y_BOTTOM)
        {
            devcalc_clear_box();
            line = DEVCALC_Y_TOP;
        }

        textcolor(GREEN);
        gotoxy(DEVCALC_X, line);
        printf("Entrada: ");
        textcolor(WHITE);
        fgets(m_func, MAX_CHAR, stdin);
        m_func[strlen(m_func) - 1] = 0;
        m_func = wo_space(m_func);
    }

    free(m_func);
    textcolor(WHITE);
    textbackground(BLACK);
}
