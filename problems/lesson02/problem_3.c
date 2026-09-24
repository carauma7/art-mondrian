#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../../graphics.h"
#include "../../utils.h"
#include "problem_3.h"

/*
* PROBLEMA 3 - Escreva o código correspondente a uma FILA.
*/
unsigned char ascii_problem_3[] = {
  0x50, 0x52, 0x4f, 0x42, 0x4c, 0x45, 0x4d, 0x41, 0x20, 0x33, 0x20, 0x2d,
  0x20, 0x45, 0x73, 0x63, 0x72, 0x65, 0x76, 0x61, 0x20, 0x6f, 0x20, 0x63,
  0xc3, 0xb3, 0x64, 0x69, 0x67, 0x6f, 0x20, 0x63, 0x6f, 0x72, 0x72, 0x65,
  0x73, 0x70, 0x6f, 0x6e, 0x64, 0x65, 0x6e, 0x74, 0x65, 0x20, 0x61, 0x20,
  0x75, 0x6d, 0x61, 0x20, 0x46, 0x49, 0x4c, 0x41, 0x2e
};
unsigned int ascii_problem_3_length = 57;

void fila_inicializar ( Fila *fila ) 
{
    fila->inicio = fila->final = fila->tamanho = 0;
}

bool fila_cheia ( Fila *fila ) 
{
    return fila->tamanho == CAPACIDADE_FILA;
}

bool fila_vazia ( Fila *fila ) 
{
    return fila->tamanho == 0;
}

bool fila_enfileirar ( Fila *fila, char valor ) 
{
    if ( fila_cheia( fila ) ) 
    {
        //printf("Fila cheia! Não é possível inserir o valor %d.\n", valor);
        return false;
    }

    if ( fila_vazia( fila ) ) 
    {
        fila->inicio = 0;
        fila->final  = 0;
    } else {
        fila->final = (fila->final + 1) % CAPACIDADE_FILA;
    }

    fila->dados[fila->final] = valor;
    fila->tamanho++;
    return true;
}

bool fila_desenfileirar ( Fila *fila, char *valor ) 
{
    if ( fila_vazia( fila ) ) 
    {
        //printf("Fila vazia! Não é possível remover elementos.\n");
        return false;
    }

    *valor = fila->dados[fila->inicio];
    fila->inicio = (fila->inicio + 1) % CAPACIDADE_FILA;
    fila->tamanho--;

    return true;
}

bool fila_frente ( Fila *fila, char *valor ) 
{
    if ( fila_vazia( fila ) ) 
    {
        //printf("Fila vazia! Não há elementos para consultar.\n");
        return false;
    }

    *valor = fila->dados[ fila->inicio ];
    return true;
}

void fila_imprimir ( Fila *f ) 
{
    if ( fila_vazia( f ) ) 
    {
        //printf("Fila vazia!\n");
        return;
    }

    printf("[FILA]\t\t: ");
    for (int i = 0; i < f->tamanho; i++) 
    {
        i == f->tamanho - 1 ? printf("%c ", f->dados[(f->inicio + i) % CAPACIDADE_FILA]) : printf("%c ← ", f->dados[(f->inicio + i) % CAPACIDADE_FILA]);
    }
    printf("\n");
}

void problem_3(void)
{
    draw_problem_screen(2, (const char *) ascii_problem_3);
    //----------------------------------------------------

    textcolor(WHITE); textbackground(BLACK);

    Fila _fila;
    fila_inicializar( &_fila );
    
    gotoxy(9, 8);  textcolor(GREEN); printf("[ENFILEIRAR]\t: %c\n", '1'); fila_enfileirar( &_fila, '1');
    gotoxy(9, 9);  textcolor(GREEN); printf("[ENFILEIRAR]\t: %c\n", '2'); fila_enfileirar( &_fila, '2');
    gotoxy(9, 10); textcolor(GREEN); printf("[ENFILEIRAR]\t: %c\n", '3'); fila_enfileirar( &_fila, '3');
    gotoxy(9, 11); textcolor(GREEN); printf("[ENFILEIRAR]\t: %c\n", '4'); fila_enfileirar( &_fila, '4');

    gotoxy(9, 12); textcolor(BLUE); fila_imprimir ( &_fila );
    
    char valor;
    if ( fila_frente( &_fila, &valor )) 
    {
        gotoxy(9, 13); textcolor(WHITE);
        printf("[FRENTE]\t: %c\n", valor);
    }
    
    if (fila_desenfileirar( &_fila, &valor)) 
    {
        gotoxy(9, 14); textcolor(RED);
        printf("[DESENFILEIRAR]\t: %c\n", valor);
    }

    gotoxy(9, 15); textcolor(BLUE); fila_imprimir(&_fila);

    gotoxy(9, 16); textcolor(GREEN); printf("[ENFILEIRAR]\t: %c\n", '5'); fila_enfileirar(&_fila, '5');
    
    gotoxy(9, 17); textcolor(BLUE); fila_imprimir(&_fila);

    //---------------------------------------------------
    waitEsc();
}