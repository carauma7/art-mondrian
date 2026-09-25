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
#include "problems/lesson04/problem_7.h"
#include "problems/lesson04/problem_71.h"
#include "problems/lesson04/problem_72.h"
#include "problems/lesson04/problem_73.h"
#include "problems/lesson04/problem_74.h"
#include "problems/lesson04/problem_75.h"
#include "problems/lesson04/problem_76.h"
#include "problems/lesson04/problem_77.h"
#include "eastereggs/devcalc/devcalc.h"
#include "eastereggs/cobra/cobra.h"

static void run_cobra(void)
{
    cobraRun();
}

void menu_add_item(
    const char *lesson,
    const char *title,
    const char *date,
    MenuAction action)
{
    if (menu_item_count >= MENU_MAIN_CAPACITY) return;

    menu_items[menu_item_count].lesson = lesson;
    menu_items[menu_item_count].title = title;
    menu_items[menu_item_count].date = date;
    menu_items[menu_item_count].action = action;
    menu_item_count++;
}

void initialize_menu_items(void)
{
    menu_add_item(" Aula 01 ", "· Núm. Total de Colisões ··········",       " 21/08/26", problem_1);
    menu_add_item(" Aula 02 ", "· Pilha ···························",       " 29/08/26", problem_2);
    menu_add_item("         ", "· Fila ····························",       "",          problem_3);
    menu_add_item(" Aula 03 ", "· Palíndromo ······················",       " 05/09/26", problem_4);
    menu_add_item("         ", "· Balanceamento ···················",       "",          problem_5);
    menu_add_item("         ", "· Problema de Josephus (Batata Quente)",    "",          problem_6);
    menu_add_item(" Aula 04 ", "· Lista Simplesmente Encadeada ····",       " 12/09/26", problem_7);
    menu_add_item(" Aula 05 ", "· Exercícios · Questão 01 · Lista Vazia e Maior Valor ·", " 19/09/26", problem_71);
    menu_add_item("         ", "· Exercícios · Questão 02 · Concatenar Listas ·", "",    problem_72);
    menu_add_item("         ", "· Exercícios · Questão 03 · Comparar Listas ·",   "",    problem_73);
    menu_add_item("         ", "· Exercícios · Questão 04 · Listas Contidas ·",   "",    problem_74);
    menu_add_item("         ", "· Exercícios · Questão 05 · Valor Ausente ·",     "",    problem_75);
    menu_add_item("         ", "· Exercícios · Questão 06 · Coordenadas ·", "",          problem_76);
    menu_add_item("         ", "· Exercícios · Questão 07 · Inverter ·",    "",          problem_77);
    menu_add_item("         ", "··································",                 "",          NULL);
    menu_add_item("         ", "· Surpresa: Calculadora de Derivadas", "", devcalcRun);
    menu_add_item("         ", "· Surpresa: Jogo da Cobrinha", "", run_cobra);
}

void run_selected_problem(int selected)
{
    if (selected >= 1 && selected <= menu_item_count && menu_items[selected - 1].action != NULL)
    {
        menu_items[selected - 1].action();
        return;
    }

    draw_content_placeholder(selected);
    waitEsc();
}