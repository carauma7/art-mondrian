#include "problem_110.h"
#include "merge-sort.h"
#include <stdlib.h>

static size_t merge_progress_count;
static size_t merge_progress_total;

static void merge(int *values, int *temporary, size_t left, size_t middle, size_t right, Lesson11Metrics *metrics)
{
    size_t i = left;
    size_t j = middle;
    size_t k = left;
    while (i < middle && j < right)
    {
        metrics->comparisons++;
        if (values[i] <= values[j]) temporary[k++] = values[i++];
        else temporary[k++] = values[j++];
        metrics->movements++;
    }
    while (i < middle) { temporary[k++] = values[i++]; metrics->movements++; }
    while (j < right) { temporary[k++] = values[j++]; metrics->movements++; }
    for (i = left; i < right; i++) { values[i] = temporary[i]; metrics->movements++; }
    merge_progress_count += right - left;
    if ((merge_progress_count & 4095U) == 0)
        lesson11_algorithm_progress("Merge Sort", merge_progress_count, merge_progress_total);
}

static void merge_range(int *values, int *temporary, size_t left, size_t right, Lesson11Metrics *metrics)
{
    size_t middle;
    if (right - left < 2) return;
    middle = left + (right - left) / 2;
    merge_range(values, temporary, left, middle, metrics);
    merge_range(values, temporary, middle, right, metrics);
    merge(values, temporary, left, middle, right, metrics);
}

static void run_merge_sort(int *values, size_t count, int target, Lesson11Metrics *metrics)
{
    int *temporary;
    (void)target;
    temporary = (int *)malloc(count * sizeof(*temporary));
    if (temporary == NULL) return;
    merge_progress_count = 0;
    merge_progress_total = count * 20;
    merge_range(values, temporary, 0, count, metrics);
    lesson11_algorithm_progress("Merge Sort", merge_progress_total, merge_progress_total);
    free(temporary);
}

static const Lesson11AlgorithmInfo merge_sort_info = {
    "Merge Sort", "melhor/médio/pior O(n log n)",
    "C(n) = 2C(n/2) + (n - 1)", "O(n)", "merge-sort",
    "comparações", 0
};

static const Lesson11AlgorithmDefinition merge_sort_definition_data = {
    { "Merge Sort", "melhor/médio/pior O(n log n)",
      "C(n) = 2C(n/2) + (n - 1)", "O(n)", "merge-sort",
      "comparações", 0 },
    NULL,
    run_merge_sort
};

const Lesson11AlgorithmDefinition *merge_sort_definition(void)
{
    return &merge_sort_definition_data;
}

void merge_sort(void)
{
    lesson11_run_algorithm(&merge_sort_info, NULL, run_merge_sort);
}
