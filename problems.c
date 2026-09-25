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
    menu_add_item(" Aula 05 ", "· Exercícios · Questão 01 · Lista Vazia ·", " 19/09/26", NULL);
    menu_add_item("         ", "· Exercícios · Questão 02 · Maior Valor ·", "",          NULL);
    menu_add_item("         ", "· Exercícios · Questão 03 · Concatenar Listas ·", "",    NULL);
    menu_add_item("         ", "· Exercícios · Questão 04 · Comparar Listas ·",   "",    NULL);
    menu_add_item("         ", "· Exercícios · Questão 05 · Valor Ausente ·",     "",          NULL);
    menu_add_item("         ", "· Exercícios · Questão 06 · Coordenadas ·", "",          NULL);
    menu_add_item("         ", "· Exercícios · Questão 07 · Inverter ·",    "",          NULL);
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