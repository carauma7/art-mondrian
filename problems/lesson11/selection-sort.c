#include "problem_110.h"
#include "selection-sort.h"

static void run_selection_sort(int *values, size_t count, int target, Lesson11Metrics *metrics)
{
    size_t i;
    size_t processed = 0;
    size_t total = count * (count > 0 ? count - 1 : 0) / 2;
    (void)target;
    for (i = 0; i + 1 < count; i++)
    {
        size_t j;
        size_t minimum = i;
        for (j = i + 1; j < count; j++)
        {
            metrics->comparisons++;
            processed++;
            if ((processed & 4095U) == 0) lesson11_algorithm_progress("Selection Sort", processed, total);
            if (values[j] < values[minimum]) minimum = j;
        }
        if (minimum != i)
        {
            int temp = values[i];
            values[i] = values[minimum];
            values[minimum] = temp;
            metrics->movements += 3;
        }
    }
    lesson11_algorithm_progress("Selection Sort", total, total);
}

static const Lesson11AlgorithmInfo selection_sort_info = {
    "Selection Sort", "melhor/médio/pior O(n²)",
    "n(n - 1) / 2 comparações; M <= 3(n - 1)", "O(1)",
    "selection-sort", "comparações", 0
};

static const Lesson11AlgorithmDefinition selection_sort_definition_data = {
    { "Selection Sort", "melhor/médio/pior O(n²)",
      "n(n - 1) / 2 comparações; M <= 3(n - 1)", "O(1)",
      "selection-sort", "comparações", 0 },
    NULL,
    run_selection_sort
};

const Lesson11AlgorithmDefinition *selection_sort_definition(void)
{
    return &selection_sort_definition_data;
}

void selection_sort(void)
{
    lesson11_run_algorithm(&selection_sort_info, NULL, run_selection_sort);
}
