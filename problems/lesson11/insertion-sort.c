#include "problem_110.h"
#include "insertion-sort.h"

static void run_insertion_sort(int *values, size_t count, int target, Lesson11Metrics *metrics)
{
    size_t i;
    size_t processed = 0;
    size_t total = count * (count > 0 ? count - 1 : 0) / 2;
    (void)target;
    for (i = 1; i < count; i++)
    {
        size_t j = i;
        int value = values[i];
        while (j > 0)
        {
            metrics->comparisons++;
            processed++;
            if ((processed & 4095U) == 0) lesson11_algorithm_progress("Insertion Sort", processed, total);
            if (values[j - 1] <= value) break;
            values[j] = values[j - 1];
            metrics->movements++;
            j--;
        }
        values[j] = value;
        if (j != i) metrics->movements++;
    }
    lesson11_algorithm_progress("Insertion Sort", total, total);
}

static const Lesson11AlgorithmInfo insertion_sort_info = {
    "Ordenação por Inserção", "melhor O(n); médio/pior O(n²)",
    "melhor n - 1; pior n(n - 1) / 2", "O(1)",
    "insertion-sort", "comparações", 0
};

static const Lesson11AlgorithmDefinition insertion_sort_definition_data = {
    { "Ordenação por Inserção", "melhor O(n); médio/pior O(n²)",
      "melhor n - 1; pior n(n - 1) / 2", "O(1)",
      "insertion-sort", "comparações", 0 },
    NULL,
    run_insertion_sort
};

const Lesson11AlgorithmDefinition *insertion_sort_definition(void)
{
    return &insertion_sort_definition_data;
}

void insertion_sort(void)
{
    lesson11_run_algorithm(&insertion_sort_info, NULL, run_insertion_sort);
}
