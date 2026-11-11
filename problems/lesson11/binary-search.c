#include "problem_110.h"
#include "binary-search.h"
#include <stdlib.h>

static int compare_integers(const void *left, const void *right)
{
    int a = *(const int *)left;
    int b = *(const int *)right;
    return (a > b) - (a < b);
}

static void prepare_binary_search(int *values, size_t count, int target, Lesson11Metrics *metrics)
{
    (void)target;
    (void)metrics;
    lesson11_algorithm_progress("Preparando Busca Binária", 0, 1);
    qsort(values, count, sizeof(*values), compare_integers);
    lesson11_algorithm_progress("Preparando Busca Binária", 1, 1);
}

static void run_binary_search(int *values, size_t count, int target, Lesson11Metrics *metrics)
{
    size_t left = 0;
    size_t right = count;

    while (left < right)
    {
        size_t middle = left + (right - left) / 2;
        metrics->comparisons++;
        lesson11_algorithm_progress("Busca Binária", count - (right - left), count);
        if (values[middle] == target)
        {
            metrics->found = 1;
            return;
        }
        if (values[middle] < target) left = middle + 1;
        else right = middle;
    }
}

static const Lesson11AlgorithmInfo binary_search_info = {
    "Busca Binária",
    "melhor O(1); médio/pior O(log n)",
    "pior: piso(log2(n)) + 1 comparações",
    "O(1), após ordenar os dados",
    NULL,
    "comparações",
    1
};

static const Lesson11AlgorithmDefinition binary_search_definition_data = {
    { "Busca Binária", "melhor O(1); médio/pior O(log n)",
      "pior: piso(log2(n)) + 1 comparações", "O(1), após ordenar os dados",
      NULL, "comparações", 1 },
    prepare_binary_search,
    run_binary_search
};

const Lesson11AlgorithmDefinition *binary_search_definition(void)
{
    return &binary_search_definition_data;
}

void binary_search(void)
{
    lesson11_run_algorithm(&binary_search_info, prepare_binary_search, run_binary_search);
}
