#ifndef PROBLEM_2_H
#define PROBLEM_2_H

#include <stdlib.h>
#include <stdbool.h>

#define CAPACIDADE_PILHA 100

typedef struct {
    char dados[ CAPACIDADE_PILHA ]; 
    int topo;              
} Pilha;

void problem_2( void );
void inicializar_pilha( Pilha *p );
bool pilha_cheia( Pilha *p );
bool pilha_vazia( Pilha *p );
bool pilha_empilhar( Pilha *p, char valor );
bool pilha_desempilhar( Pilha *p, char *valor );
bool pilha_topo( Pilha *p, char *valor );

#endif /* PROBLEM_2_H */