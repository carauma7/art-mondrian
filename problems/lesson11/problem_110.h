#ifndef PROBLEM_110_H
#define PROBLEM_110_H

#include <stddef.h>

typedef struct
{
    unsigned long long comparisons;
    unsigned long long movements;
    double milliseconds;
    int found;
} Lesson11Metrics;

typedef struct
{
    const char *name;
    const char *complexity;
    const char *formula;
    const char *memory;
    const char *output_directory;
    const char *operation_label;
    int is_search;
} Lesson11AlgorithmInfo;

typedef void (*Lesson11Algorithm)(int *values, size_t count, int target, Lesson11Metrics *metrics);

typedef struct
{
    Lesson11AlgorithmInfo info;
    Lesson11Algorithm preparation;
    Lesson11Algorithm algorithm;
} Lesson11AlgorithmDefinition;

void problem_110_generate_files(void);
void problem_110_compare(void);
void lesson11_progress(const char *phase, size_t current, size_t total);
void lesson11_algorithm_progress(const char *phase, size_t current, size_t total);
void lesson11_run_algorithm(
    const Lesson11AlgorithmInfo *info,
    Lesson11Algorithm preparation,
    Lesson11Algorithm algorithm);

#endif /* PROBLEM_110_H */
