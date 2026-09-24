#ifndef PROBLEM_2_H
#define PROBLEM_2_H

#include <stdlib.h>
#include <stdbool.h>

#define CAPACIDADE_PILHA 100

typedef struct {
    int dados[ CAPACIDADE_PILHA ]; 
    int topo;              
} Pilha;

void problem_2( void );
void inicializar_pilha( Pilha *p );
bool pilha_cheia( Pilha *p );
bool pilha_vazia( Pilha *p );
bool pilha_empilhar( Pilha *p, int valor );
bool pilha_desempilhar( Pilha *p, int *valor );
bool pilha_topo( Pilha *p, int *valor );

#endif /* PROBLEM_2_H */