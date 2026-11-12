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
#include "problems/lesson08/problem_80.h"
#include "problems/lesson11/problem_110.h"
#include "problems/lesson11/sequential-search.h"
#include "problems/lesson11/binary-search.h"
#include "problems/lesson11/bubble-sort.h"
#include "problems/lesson11/selection-sort.h"
#include "problems/lesson11/insertion-sort.h"
#include "problems/lesson11/merge-sort.h"
#include "problems/lesson11/quick-sort.h"
#include "problems/lesson11/radix-sort.h"
#include "problems/lesson11/heap-sort.h"
#include "problems/lesson11/search-benchmark.h"
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
    menu_add_item(" Aula 08 ", "· Heap ····························",       " 10/10/26", problem_80);
    menu_add_item(" Aula 11 ", "· Ordenação e Busca (Trabalho) · Gerar Arquivos ·", " 11/11/2026", problem_110_generate_files);
    menu_add_item("         ", "· Busca Sequencial ·················", "", sequential_search);
    menu_add_item("         ", "· Busca Binária ····················", "", binary_search);
    menu_add_item("         ", "· Bubble Sort ······················", "", bubble_sort);
    menu_add_item("         ", "· Selection Sort ···················", "", selection_sort);
    menu_add_item("         ", "· Ordenação por Inserção ············", "", insertion_sort);
    menu_add_item("         ", "· Merge Sort ························", "", merge_sort);
    menu_add_item("         ", "· Quick Sort ························", "", quick_sort);
    menu_add_item("         ", "· Radix Sort ························", "", radix_sort);
    menu_add_item("         ", "· Heap Sort ·························", "", heap_sort);
    menu_add_item("         ", "· Comparativo ······················", "", problem_110_compare);
    menu_add_item("         ", "· Comparativo de buscas (10.000) ···", "", lesson11_search_benchmark);
    menu_add_item("         ", "···································",       "",          NULL);
    menu_add_item("         ", "· Surpresa: Calculadora de Derivadas", "", devcalcRun);
    menu_add_item("         ", "· Surpresa: Jogo da Cobrinha", "", run_cobra);
    menu_add_item("         ", "· Sair", "", NULL);
}
int run_selected_problem(int selected)
{
    if (selected < 1 || selected > menu_item_count)
    {
        return 0;
    }

    if (selected == menu_item_count)
    {
        return 1;
    }

    if (menu_items[selected - 1].action != NULL)
    {
        menu_items[selected - 1].action();
        return 0;
    }

    draw_content_placeholder(selected);
    waitEsc();
    return 0;
}