#include "problem_110.h"
#include "quick-sort.h"

static size_t quick_progress_count;
static size_t quick_progress_total;

static void quick_range(int *values, size_t left, size_t right, Lesson11Metrics *metrics)
{
    size_t store;
    size_t i;
    int pivot;
    if (right - left < 2) return;
    pivot = values[right - 1];
    store = left;
    for (i = left; i + 1 < right; i++)
    {
        metrics->comparisons++;
        quick_progress_count++;
        if ((quick_progress_count & 4095U) == 0)
            lesson11_algorithm_progress("Quick Sort", quick_progress_count, quick_progress_total);
        if (values[i] < pivot)
        {
            int temp = values[i];
            values[i] = values[store];
            values[store] = temp;
            metrics->movements += 3;
            store++;
        }
    }
    {
        int temp = values[store];
        values[store] = values[right - 1];
        values[right - 1] = temp;
        metrics->movements += 3;
    }
    quick_range(values, left, store, metrics);
    quick_range(values, store + 1, right, metrics);
}

static void run_quick_sort(int *values, size_t count, int target, Lesson11Metrics *metrics)
{
    (void)target;
    quick_progress_count = 0;
    quick_progress_total = count * 20;
    quick_range(values, 0, count, metrics);
    lesson11_algorithm_progress("Quick Sort", quick_progress_total, quick_progress_total);
}

static const Lesson11AlgorithmInfo quick_sort_info = {
    "Quick Sort", "melhor/médio O(n log n); pior O(n²)",
    "partição n - 1; pior n(n - 1) / 2", "O(log n) médio; O(n) pior",
    "quick-sort", "comparações", 0
};

static const Lesson11AlgorithmDefinition quick_sort_definition_data = {
    { "Quick Sort", "melhor/médio O(n log n); pior O(n²)",
      "partição n - 1; pior n(n - 1) / 2",
      "O(log n) médio; O(n) pior", "quick-sort", "comparações", 0 },
    NULL,
    run_quick_sort
};

const Lesson11AlgorithmDefinition *quick_sort_definition(void)
{
    return &quick_sort_definition_data;
}

void quick_sort(void)
{
    lesson11_run_algorithm(&quick_sort_info, NULL, run_quick_sort);
}
