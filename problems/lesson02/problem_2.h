#ifndef PROBLEM_2_H
#define PROBLEM_2_H

#include <stdlib.h>
#include <stdbool.h>

#define CAPACIDADE 100

typedef struct {
    int dados[ CAPACIDADE ]; 
    int topo;              
} Pilha;

void problem_2( void );
void inicializar( Pilha *p );
bool estaCheia( Pilha *p );
bool estaVazia( Pilha *p );
bool push( Pilha *p, int valor );
bool pop( Pilha *p, int *valor );

#endif /* PROBLEM_2_H */