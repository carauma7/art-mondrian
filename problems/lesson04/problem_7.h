#ifndef PROBLEM_7_H
#define PROBLEM_7_H

typedef struct Aluno {
    char nome[50];
    float notas[3];
    struct Aluno *proximo;
} Aluno;

void problem_7(void);

#endif /* PROBLEM_7_H */