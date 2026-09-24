#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../../graphics.h"
#include "../../utils.h"
#include "problem_2.h"

/*
* PROBLEMA 2 - Escreva o código correspondente a uma PILHA.
*/
unsigned char ascii_problem_2[] = {
  0x50, 0x52, 0x4f, 0x42, 0x4c, 0x45, 0x4d, 0x41, 0x20, 0x32, 0x20, 0x2d, 
  0x20, 0x45, 0x73, 0x63, 0x72, 0x65, 0x76, 0x61, 0x20, 0x6f, 0x20, 0x63, 
  0xc3, 0xb3, 0x64, 0x69, 0x67, 0x6f, 0x20, 0x63, 0x6f, 0x72, 0x72, 0x65, 
  0x73, 0x70, 0x6f, 0x6e, 0x64, 0x65, 0x6e, 0x74, 0x65, 0x20, 0x61, 0x20, 
  0x75, 0x6d, 0x61, 0x20, 0x50, 0x49, 0x4c, 0x48, 0x41, 0x2e
};
unsigned int ascii_problem_2_length = 58;

void inicializar ( Pilha *pilha )
{
    pilha->topo = -1; 
}

bool estaVazia ( Pilha *pilha ) 
{
    return pilha->topo == -1;
}

bool estaCheia ( Pilha *pilha ) 
{
    return pilha->topo == CAPACIDADE - 1;
}

bool push ( Pilha *pilha, int valor ) 
{
    if ( estaCheia( pilha ) ) 
    {
        //printf("Estouro de pilha! Não é possível inserir o valor %d.\n", valor);
        return false;
    }
    
    pilha->dados[++pilha->topo] = valor;

    return true;
}

bool pop ( Pilha *pilha, int *valor ) 
{
    if ( estaVazia(pilha ) )
    {
        //printf("Pilha vazia! Não é possível remover elementos.\n");
        return false;
    }
    
    *valor = pilha->dados[pilha->topo--];

    return true;
}

bool peek ( Pilha *pilha, int *valor ) 
{
    if ( estaVazia ( pilha ) ) 
    {
        //printf("Pilha vazia! Não há elementos para consultar.\n");
        return false;
    }
    
    *valor = pilha->dados[pilha->topo];

    return true;
}

void print ( Pilha *pilha ) 
{
    if ( estaVazia ( pilha ) ) 
    {
        //printf("Pilha vazia!\n");
        return;
    }

    printf("[PILHA]\t\t: ");

    for (int i = pilha->topo; i >= 0; i--) 
    {
        i == 0 ? printf("%d ", pilha->dados[ i ]) : printf("%d ← ", pilha->dados[ i ]);
    }

    printf("\n");
}

void problem_2(void)
{
    draw_problem_screen(2, ( const char * ) ascii_problem_2);
    //----------------------------------------------------

    textcolor(WHITE); textbackground(BLACK);

    Pilha _pilha;

    inicializar( &_pilha);

    gotoxy(9, 8);textcolor(GREEN); printf("[EMPILHAR]\t: 1\n"); push(&_pilha, 1);
    gotoxy(9, 9);textcolor(GREEN); printf("[EMPILHAR]\t: 2\n"); push(&_pilha, 2);
    gotoxy(9, 10);textcolor(GREEN); printf("[EMPILHAR]\t: 3\n"); push(&_pilha, 3);
    gotoxy(9, 11);textcolor(GREEN); printf("[EMPILHAR]\t: 4\n"); push(&_pilha, 4);

    gotoxy(9, 12);textcolor(BLUE); print(& _pilha);
    gotoxy(9, 13);textcolor(WHITE); printf("[TOPO DA PILHA]\t: %d\n", _pilha.dados[ _pilha.topo ]);
        
    int valor;
    if (pop(&_pilha, &valor)) 
    {
        gotoxy(9, 14);textcolor(RED);
        printf("[DESEMPILHAR]\t: %d\n", valor);
    }

    gotoxy(9, 15);textcolor(BLUE); print(&_pilha);
    
    gotoxy(9, 16);textcolor(GREEN); printf("[EMPILHAR]\t: 5\n");
    push(&_pilha, 5);
    
    gotoxy(9, 17);textcolor(BLUE); print(&_pilha);
    
    //---------------------------------------------------
    waitEsc();
}