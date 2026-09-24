#ifndef PROBLEM_3_H
#define PROBLEM_3_H

#include <stdlib.h>
#include <stdbool.h>

#define CAPACIDADE_FILA 100

typedef struct {
    char dados[CAPACIDADE_FILA];
    int inicio, final, tamanho;   
} Fila;

void problem_3( void );
void fila_inicializar ( Fila *fila );
bool fila_cheia ( Fila *fila );
bool fila_vazia ( Fila *fila );
bool fila_enfileirar ( Fila *fila, char valor );
bool fila_desenfileirar ( Fila *fila, char *valor );
bool fila_frente ( Fila *fila, char *valor );
void fila_imprimir ( Fila *fila );

#endif /* PROBLEM_3_H */