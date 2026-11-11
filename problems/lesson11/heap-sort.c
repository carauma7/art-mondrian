#include "problem_110.h"
#include "heap-sort.h"

static void sift_down(int *values, size_t root, size_t count, Lesson11Metrics *metrics)
{
    while (root * 2 + 1 < count)
    {
        size_t child = root * 2 + 1;
        metrics->comparisons++;
        if (child + 1 < count && values[child] < values[child + 1]) child++;
        metrics->comparisons++;
        if (values[root] >= values[child]) return;
        {
            int temp = values[root];
            values[root] = values[child];
            values[child] = temp;
            metrics->movements += 3;
        }
        root = child;
    }
}

static void run_heap_sort(int *values, size_t count, int target, Lesson11Metrics *metrics)
{
    size_t i;
    size_t processed = 0;
    size_t total = count * 20;
    (void)target;
    for (i = count / 2; i > 0; i--) sift_down(values, i - 1, count, metrics);
    for (i = count; i > 1; i--)
    {
        int temp = values[0];
        values[0] = values[i - 1];
        values[i - 1] = temp;
        metrics->movements += 3;
        sift_down(values, 0, i - 1, metrics);
        processed++;
        if ((processed & 1023U) == 0) lesson11_algorithm_progress("Heap Sort", processed, total);
    }
    lesson11_algorithm_progress("Heap Sort", total, total);
}

static const Lesson11AlgorithmInfo heap_sort_info = {
    "Heap Sort", "melhor/médio/pior O(n log n)",
    "construção O(n) + n extrações O(log n)", "O(1)",
    "heap-sort", "comparações", 0
};

static const Lesson11AlgorithmDefinition heap_sort_definition_data = {
    { "Heap Sort", "melhor/médio/pior O(n log n)",
      "construção O(n) + n extrações O(log n)", "O(1)",
      "heap-sort", "comparações", 0 },
    NULL,
    run_heap_sort
};

const Lesson11AlgorithmDefinition *heap_sort_definition(void)
{
    return &heap_sort_definition_data;
}

void heap_sort(void)
{
    lesson11_run_algorithm(&heap_sort_info, NULL, run_heap_sort);
}
