#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "devcalc.h"

void devcalcRun(void)
{
    char *m_func = (char *) malloc(sizeof(char) * MAX_CHAR);
    char *derv;

    clrscr();
    textbackground(BLACK);
    textcolor(CYAN);

    printf("CALCULADORA DE DERIVADAS -- versão: 0.1\n\n");
    printf("(?) Insira uma expressão - Ex. ln(x) + cos(x^3) + x - e pressione ENTER.\n\n");
    printf("(?) Para finalizar, digite \"sair\".\n\n");

    textcolor(GREEN);
    printf("Entrada: ");
    textcolor(WHITE);
    fgets(m_func, MAX_CHAR, stdin);
    m_func[strlen(m_func) - 1] = 0;
    m_func = wo_space(m_func);

    while (strcmp(m_func, "sair") != 0)
    {
        if (!par_paired(m_func, strlen(m_func)))
        {
            textcolor(RED);
            printf("Número de parênteses abertos/fechados é desigual.\n\n");
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
                printf("0\n\n");
            }
            else if (*derv == '+')
            {
                printf("%s\n\n", derv + 1);
            }
            else
            {
                printf("%s\n\n", derv);
            }
        }

        textcolor(GREEN);
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
