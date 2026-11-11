#ifndef PROBLEM_80_H
#define PROBLEM_80_H

#include <stddef.h>

#define HEAP_MAX_LINES 256
#define HEAP_LINE_WIDTH 61
#define HEAP_FIRST_ROW 6
#define HEAP_LAST_ROW 20

typedef struct
{
    char text[HEAP_LINE_WIDTH + 1];
    int color;
} HeapLine;

typedef struct
{
    const char *question;
    const char *answer;
} HeapQuestion;

extern const HeapQuestion heap_questions[];
extern const size_t heap_question_count;
extern HeapLine heap_lines[HEAP_MAX_LINES];
extern int heap_line_count;

void problem_80(void);

#endif /* PROBLEM_80_H */