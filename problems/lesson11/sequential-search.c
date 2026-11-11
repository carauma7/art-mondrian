#include "problem_110.h"
#include "sequential-search.h"

static void run_sequential_search(int *values, size_t count, int target, Lesson11Metrics *metrics)
{
    size_t i;
    for (i = 0; i < count; i++)
    {
        metrics->comparisons++;
        if ((i & 1023U) == 0) lesson11_algorithm_progress("Busca Sequencial", i, count);
        if (values[i] == target)
        {
            metrics->found = 1;
            return;
        }
        lesson11_algorithm_progress("Busca Sequencial", count, count);
    }
}

static const Lesson11AlgorithmInfo sequential_search_info = {
    "Busca Sequencial",
    "melhor O(1); médio/pior O(n)",
    "melhor 1; médio (n + 1) / 2; pior n",
    "O(1)",
    NULL,
    "comparações",
    1
};

static const Lesson11AlgorithmDefinition sequential_search_definition_data = {
    { "Busca Sequencial", "melhor O(1); médio/pior O(n)",
      "melhor 1; médio (n + 1) / 2; pior n", "O(1)", NULL,
      "comparações", 1 },
    NULL,
    run_sequential_search
};

const Lesson11AlgorithmDefinition *sequential_search_definition(void)
{
    return &sequential_search_definition_data;
}

void sequential_search(void)
{
    lesson11_run_algorithm(&sequential_search_info, NULL, run_sequential_search);
}
