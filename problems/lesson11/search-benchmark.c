#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../graphics.h"
#include "../../utils.h"
#include "binary-search.h"
#include "problem_110.h"
#include "search-benchmark.h"
#include "sequential-search.h"

#define SEARCH_BENCHMARK_DATA_PATH "problems/lesson11/data/inteiros_10000.txt"
#define SEARCH_BENCHMARK_VECTOR_SIZE 10000
#define SEARCH_BENCHMARK_QUERY_COUNT 10000
#define SEARCH_BENCHMARK_VALUE_RANGE 100000U

static int compare_integers(const void *left, const void *right)
{
    int a = *(const int *)left;
    int b = *(const int *)right;
    return (a > b) - (a < b);
}

static void show_benchmark_error(const char *message)
{
    draw_problem_screen(11, "    AULA 11 - BUSCAS EM LOTE");
    gotoxy(11, 9);
    textcolor(LIGHTRED);
    printf("%s", message);
    gotoxy(11, 12);
    textcolor(WHITE);
    printf("Pressione Esc para voltar.");
    waitEsc();
}

static void format_benchmark_duration(char *buffer, size_t size, double milliseconds)
{
    if (milliseconds < 1.0)
    {
        snprintf(buffer, size, "%.3f us", milliseconds * 1000.0);
    }
    else if (milliseconds < 1000.0)
    {
        snprintf(buffer, size, "%.3f ms", milliseconds);
    }
    else
    {
        snprintf(buffer, size, "%.3f s", milliseconds / 1000.0);
    }
}

static int elapsed_milliseconds(
    LARGE_INTEGER start,
    LARGE_INTEGER frequency,
    double *milliseconds)
{
    LARGE_INTEGER end;

    if (frequency.QuadPart <= 0 || !QueryPerformanceCounter(&end)) return 0;
    *milliseconds = (double)(end.QuadPart - start.QuadPart) * 1000.0 /
                    (double)frequency.QuadPart;
    return 1;
}

static int load_benchmark_data(int *values)
{
    FILE *file = fopen(SEARCH_BENCHMARK_DATA_PATH, "r");
    size_t i;

    if (file == NULL) return 0;
    for (i = 0; i < SEARCH_BENCHMARK_VECTOR_SIZE; i++)
    {
        if (fscanf(file, "%d", &values[i]) != 1)
        {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

void lesson11_search_benchmark(void)
{
    int *random_values;
    int *sorted_values;
    int *targets;
    unsigned int random_state = 0xA341316CU;
    size_t sequential_found = 0;
    size_t binary_found = 0;
    size_t i;
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    double sequential_milliseconds;
    double binary_milliseconds;
    char sequential_duration[32];
    char binary_duration[32];

    random_values = (int *)malloc(SEARCH_BENCHMARK_VECTOR_SIZE * sizeof(*random_values));
    sorted_values = (int *)malloc(SEARCH_BENCHMARK_VECTOR_SIZE * sizeof(*sorted_values));
    targets = (int *)malloc(SEARCH_BENCHMARK_QUERY_COUNT * sizeof(*targets));
    if (random_values == NULL || sorted_values == NULL || targets == NULL)
    {
        free(random_values);
        free(sorted_values);
        free(targets);
        show_benchmark_error("Erro: memoria insuficiente para executar o comparativo.");
        return;
    }
    if (!load_benchmark_data(random_values))
    {
        free(random_values);
        free(sorted_values);
        free(targets);
        show_benchmark_error(
            "Arquivo de 10.000 valores ausente ou invalido. Gere os arquivos primeiro.");
        return;
    }
    memcpy(sorted_values, random_values,
           SEARCH_BENCHMARK_VECTOR_SIZE * sizeof(*sorted_values));
    qsort(sorted_values, SEARCH_BENCHMARK_VECTOR_SIZE,
          sizeof(*sorted_values), compare_integers);

    for (i = 0; i < SEARCH_BENCHMARK_QUERY_COUNT; i++)
    {
        random_state = random_state * 1664525U + 1013904223U;
        targets[i] = (int)(random_state % SEARCH_BENCHMARK_VALUE_RANGE);
    }
    if (!QueryPerformanceFrequency(&frequency) ||
        !QueryPerformanceCounter(&start))
    {
        free(random_values);
        free(sorted_values);
        free(targets);
        show_benchmark_error("Erro: nao foi possivel iniciar o contador de tempo.");
        return;
    }
    for (i = 0; i < SEARCH_BENCHMARK_QUERY_COUNT; i++)
    {
        sequential_found += (size_t)sequential_search_contains(
            random_values, SEARCH_BENCHMARK_VECTOR_SIZE, targets[i]);
    }
    if (!elapsed_milliseconds(start, frequency, &sequential_milliseconds))
    {
        free(random_values);
        free(sorted_values);
        free(targets);
        show_benchmark_error("Erro: nao foi possivel obter o tempo da busca sequencial.");
        return;
    }

    if (!QueryPerformanceCounter(&start))
    {
        free(random_values);
        free(sorted_values);
        free(targets);
        show_benchmark_error("Erro: nao foi possivel iniciar o contador de tempo.");
        return;
    }
    for (i = 0; i < SEARCH_BENCHMARK_QUERY_COUNT; i++)
    {
        binary_found += (size_t)binary_search_contains(
            sorted_values, SEARCH_BENCHMARK_VECTOR_SIZE, targets[i]);
    }
    if (!elapsed_milliseconds(start, frequency, &binary_milliseconds))
    {
        free(random_values);
        free(sorted_values);
        free(targets);
        show_benchmark_error("Erro: nao foi possivel obter o tempo da busca binaria.");
        return;
    }

    format_benchmark_duration(
        sequential_duration, sizeof(sequential_duration), sequential_milliseconds);
    format_benchmark_duration(
        binary_duration, sizeof(binary_duration), binary_milliseconds);

    draw_problem_screen(11, "    AULA 11 - BUSCAS EM LOTE");
    textcolor(LIGHTCYAN);
    gotoxy(11, 7);
    printf("10.000 buscas nos vetores de 10.000 elementos");
    gotoxy(11, 9);
    textcolor(YELLOW);
    printf("Busca sequencial - vetor aleatorio");
    gotoxy(13, 10);
    textcolor(WHITE);
    printf("Tempo: %s", sequential_duration);
    gotoxy(13, 11);
    printf("Velocidade: %.4g buscas/s",
           sequential_milliseconds > 0.0
               ? (double)SEARCH_BENCHMARK_QUERY_COUNT * 1000.0 /
                     sequential_milliseconds
               : 0.0);
    gotoxy(13, 12);
    printf("Valores encontrados: %lu / %d",
           (unsigned long)sequential_found, SEARCH_BENCHMARK_QUERY_COUNT);
    gotoxy(11, 14);
    textcolor(LIGHTGREEN);
    printf("Busca binaria - vetor ordenado");
    gotoxy(13, 15);
    textcolor(WHITE);
    printf("Tempo: %s", binary_duration);
    gotoxy(13, 16);
    printf("Velocidade: %.4g buscas/s",
           binary_milliseconds > 0.0
               ? (double)SEARCH_BENCHMARK_QUERY_COUNT * 1000.0 /
                     binary_milliseconds
               : 0.0);
    gotoxy(13, 17);
    printf("Valores encontrados: %lu / %d",
           (unsigned long)binary_found, SEARCH_BENCHMARK_QUERY_COUNT);
    gotoxy(11, 19);
    textcolor(LIGHTBLUE);
    printf("As duas buscas usaram a mesma sequencia aleatoria.");
    gotoxy(11, 21);
    textcolor(WHITE);
    printf("Pressione Esc para voltar.");
    gotoxy(SCREEN_WIDTH, SCREEN_HEIGHT);

    free(random_values);
    free(sorted_values);
    free(targets);
    waitEsc();
}
