#include "problem_110.h"
#include "radix-sort.h"
#include <stdlib.h>

static void run_radix_sort(int *values, size_t count, int target, Lesson11Metrics *metrics)
{
    int *output;
    int maximum = 0;
    int exponent;
    size_t i;
    size_t processed = 0;
    size_t total = count * 5;

    (void)target;
    output = (int *)malloc(count * sizeof(*output));
    if (output == NULL) return;
    for (i = 0; i < count; i++)
    {
        if (values[i] > maximum) maximum = values[i];
    }
    for (exponent = 1; maximum / exponent > 0; exponent *= 10)
    {
        size_t buckets[10] = { 0 };
        for (i = 0; i < count; i++)
        {
            buckets[(values[i] / exponent) % 10]++;
            metrics->comparisons++;
            processed++;
            if ((processed & 4095U) == 0) lesson11_algorithm_progress("Radix Sort", processed, total);
        }
        for (i = 1; i < 10; i++) buckets[i] += buckets[i - 1];
        for (i = count; i > 0; i--)
        {
            int value = values[i - 1];
            output[--buckets[(value / exponent) % 10]] = value;
            metrics->movements++;
        }
        for (i = 0; i < count; i++)
        {
            values[i] = output[i];
            metrics->movements++;
        }
    }
    lesson11_algorithm_progress("Radix Sort", total, total);
    free(output);
}

static const Lesson11AlgorithmInfo radix_sort_info = {
    "Radix Sort", "O(d(n + b)) em todos os casos",
    "d passagens; n elementos; b = 10", "O(n + b)",
    "radix-sort", "inspeções", 0
};

static const Lesson11AlgorithmDefinition radix_sort_definition_data = {
    { "Radix Sort", "O(d(n + b)) em todos os casos",
      "d passagens; n elementos; b = 10", "O(n + b)",
      "radix-sort", "inspeções", 0 },
    NULL,
    run_radix_sort
};

const Lesson11AlgorithmDefinition *radix_sort_definition(void)
{
    return &radix_sort_definition_data;
}

void radix_sort(void)
{
    lesson11_run_algorithm(&radix_sort_info, NULL, run_radix_sort);
}
