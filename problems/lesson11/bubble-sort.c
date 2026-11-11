#include "problem_110.h"
#include "bubble-sort.h"

static void run_bubble_sort(int *values, size_t count, int target, Lesson11Metrics *metrics)
{
    size_t end;
    size_t processed = 0;
    size_t total = count * (count > 0 ? count - 1 : 0) / 2;
    (void)target;
    for (end = count; end > 1; end--)
    {
        size_t i;
        int changed = 0;
        for (i = 1; i < end; i++)
        {
            metrics->comparisons++;
            if (values[i - 1] > values[i])
            {
                int temp = values[i - 1];
                values[i - 1] = values[i];
                values[i] = temp;
                metrics->movements += 3;
                changed = 1;
            }
            processed++;
            if ((processed & 4095U) == 0) lesson11_algorithm_progress("Bubble Sort", processed, total);
        }
        if (!changed) break;
    }
    lesson11_algorithm_progress("Bubble Sort", total, total);
}

static const Lesson11AlgorithmInfo bubble_sort_info = {
    "Bubble Sort",
    "melhor O(n); médio/pior O(n²)",
    "pior n(n - 1) / 2; melhor n - 1",
    "O(1)",
    "bubble-sort",
    "comparações",
    0
};

static const Lesson11AlgorithmDefinition bubble_sort_definition_data = {
    { "Bubble Sort", "melhor O(n); médio/pior O(n²)",
      "pior n(n - 1) / 2; melhor n - 1", "O(1)", "bubble-sort",
      "comparações", 0 },
    NULL,
    run_bubble_sort
};

const Lesson11AlgorithmDefinition *bubble_sort_definition(void)
{
    return &bubble_sort_definition_data;
}

void bubble_sort(void)
{
    lesson11_run_algorithm(&bubble_sort_info, NULL, run_bubble_sort);
}