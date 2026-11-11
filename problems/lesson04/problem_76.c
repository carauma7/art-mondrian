#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../graphics.h"
#include "../../utils.h"

#include "problem_7.h"
#include "problem_76.h"

/*
* PROBLEMA 7.6 - 
EXERCICIO 06 - PROBLEMA 6 - Especifique um novo TAD 
para representar uma lista de pontos que representam 
um caminho no plano cartesiano a ser percorrido. Além 
das funções para inclusão, pesquisa e remoção implemente 
também uma operação que calcule o tamanho do caminho 
percorrido se todos os pontos da lista forem visitados.
*/
unsigned char ascii_problema_76[] = {
  0x45, 0x58, 0x45, 0x52, 0x43, 0x49, 0x43, 0x49, 0x4f, 0x20, 0x30, 0x36,
  0x20, 0x2d, 0x20, 0x50, 0x52, 0x4f, 0x42, 0x4c, 0x45, 0x4d, 0x41, 0x20,
  0x36, 0x20, 0x2d, 0x20, 0x45, 0x73, 0x70, 0x65, 0x63, 0x69, 0x66, 0x69,
  0x71, 0x75, 0x65, 0x20, 0x75, 0x6d, 0x20, 0x6e, 0x6f, 0x76, 0x6f, 0x20,
  0x54, 0x41, 0x44, 0x20, 0x0a, 0x70, 0x61, 0x72, 0x61, 0x20, 0x72, 0x65,
  0x70, 0x72, 0x65, 0x73, 0x65, 0x6e, 0x74, 0x61, 0x72, 0x20, 0x75, 0x6d,
  0x61, 0x20, 0x6c, 0x69, 0x73, 0x74, 0x61, 0x20, 0x64, 0x65, 0x20, 0x70,
  0x6f, 0x6e, 0x74, 0x6f, 0x73, 0x20, 0x71, 0x75, 0x65, 0x20, 0x72, 0x65,
  0x70, 0x72, 0x65, 0x73, 0x65, 0x6e, 0x74, 0x61, 0x6d, 0x20, 0x0a, 0x75,
  0x6d, 0x20, 0x63, 0x61, 0x6d, 0x69, 0x6e, 0x68, 0x6f, 0x20, 0x6e, 0x6f,
  0x20, 0x70, 0x6c, 0x61, 0x6e, 0x6f, 0x20, 0x63, 0x61, 0x72, 0x74, 0x65,
  0x73, 0x69, 0x61, 0x6e, 0x6f, 0x20, 0x61, 0x20, 0x73, 0x65, 0x72, 0x20,
  0x70, 0x65, 0x72, 0x63, 0x6f, 0x72, 0x72, 0x69, 0x64, 0x6f, 0x2e, 0x20,
  0x41, 0x6c, 0xc3, 0xa9, 0x6d, 0x20, 0x0a, 0x64, 0x61, 0x73, 0x20, 0x66,
  0x75, 0x6e, 0xc3, 0xa7, 0xc3, 0xb5, 0x65, 0x73, 0x20, 0x70, 0x61, 0x72,
  0x61, 0x20, 0x69, 0x6e, 0x63, 0x6c, 0x75, 0x73, 0xc3, 0xa3, 0x6f, 0x2c,
  0x20, 0x70, 0x65, 0x73, 0x71, 0x75, 0x69, 0x73, 0x61, 0x20, 0x65, 0x20,
  0x72, 0x65, 0x6d, 0x6f, 0xc3, 0xa7, 0xc3, 0xa3, 0x6f, 0x20, 0x69, 0x6d,
  0x70, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x65, 0x20, 0x0a, 0x74, 0x61,
  0x6d, 0x62, 0xc3, 0xa9, 0x6d, 0x20, 0x75, 0x6d, 0x61, 0x20, 0x6f, 0x70,
  0x65, 0x72, 0x61, 0xc3, 0xa7, 0xc3, 0xa3, 0x6f, 0x20, 0x71, 0x75, 0x65,
  0x20, 0x63, 0x61, 0x6c, 0x63, 0x75, 0x6c, 0x65, 0x20, 0x6f, 0x20, 0x74,
  0x61, 0x6d, 0x61, 0x6e, 0x68, 0x6f, 0x20, 0x64, 0x6f, 0x20, 0x63, 0x61,
  0x6d, 0x69, 0x6e, 0x68, 0x6f, 0x20, 0x0a, 0x70, 0x65, 0x72, 0x63, 0x6f,
  0x72, 0x72, 0x69, 0x64, 0x6f, 0x20, 0x73, 0x65, 0x20, 0x74, 0x6f, 0x64,
  0x6f, 0x73, 0x20, 0x6f, 0x73, 0x20, 0x70, 0x6f, 0x6e, 0x74, 0x6f, 0x73,
  0x20, 0x64, 0x61, 0x20, 0x6c, 0x69, 0x73, 0x74, 0x61, 0x20, 0x66, 0x6f,
  0x72, 0x65, 0x6d, 0x20, 0x76, 0x69, 0x73, 0x69, 0x74, 0x61, 0x64, 0x6f,
  0x73, 0x2e
};
unsigned int ascii_problema_76_length = 338;

void pontos_inicializar(Ponto **inicio)
{
    if (inicio != NULL) *inicio = NULL;
}

void pontos_apagarTudo(Ponto **inicio)
{
    if (inicio == NULL) return;

    Ponto *atual = *inicio;
    while (atual != NULL)
    {
        Ponto *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    *inicio = NULL;
}

int pontos_inserir(Ponto **inicio, float x, float y)
{
    if (inicio == NULL) return 0;

    Ponto *novo = malloc(sizeof(*novo));
    if (novo == NULL) return 0;

    novo->x = x;
    novo->y = y;
    novo->proximo = NULL;

    if (*inicio == NULL)
    {
        *inicio = novo;
        return 1;
    }

    Ponto *atual = *inicio;
    while (atual->proximo != NULL) atual = atual->proximo;
    atual->proximo = novo;
    return 1;
}

Ponto *pontos_buscar(Ponto **inicio, float x, float y)
{
    if (inicio == NULL) return NULL;

    for (Ponto *atual = *inicio; atual != NULL; atual = atual->proximo)
    {
        if (atual->x == x && atual->y == y) return atual;
    }
    return NULL;
}

int pontos_remover(Ponto **inicio, float x, float y)
{
    if (inicio == NULL) return 0;

    Ponto *atual = *inicio;
    Ponto *anterior = NULL;
    while (atual != NULL && (atual->x != x || atual->y != y))
    {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) return 0;

    if (anterior == NULL) *inicio = atual->proximo;
    else anterior->proximo = atual->proximo;
    free(atual);
    return 1;
}

static double raizQuadradaBabylonica(double valor)
{
    if (valor <= 0.0) return 0.0;

    double estimativa = valor < 1.0 ? 1.0 : valor;
    for (int iteracao = 0; iteracao < 256; iteracao++)
    {
        double proxima = 0.5 * (estimativa + valor / estimativa);
        double diferenca = proxima > estimativa
            ? proxima - estimativa
            : estimativa - proxima;

        if (proxima == estimativa || diferenca <= estimativa * 1e-12)
            return proxima;

        estimativa = proxima;
    }

    return estimativa;
}

float pontos_tamanhoCaminho(Ponto **inicio)
{
    if (inicio == NULL || *inicio == NULL) return 0.0f;

    double tamanho = 0.0;
    for (Ponto *atual = *inicio; atual->proximo != NULL; atual = atual->proximo)
    {
        double deltaX = (double)atual->proximo->x - atual->x;
        double deltaY = (double)atual->proximo->y - atual->y;
        tamanho += raizQuadradaBabylonica(deltaX * deltaX + deltaY * deltaY);
    }
    return (float)tamanho;
}

void problem_76(void)
{
    draw_problem_screen( 5 , ( const char * ) ascii_problema_76 );
    //----------------------------------------------------

    Ponto *caminho;
    pontos_inicializar(&caminho);
    if (!pontos_inserir(&caminho, 0.0f, 0.0f)
        || !pontos_inserir(&caminho, 3.0f, 4.0f)
        || !pontos_inserir(&caminho, 6.0f, 4.0f)
        || !pontos_inserir(&caminho, 6.0f, 8.0f))
    {
        pontos_apagarTudo(&caminho);
        textcolor(RED);textbackground(BLACK);
        gotoxy(9, 11); printf("Não foi possivel alocar os pontos do caminho.");
        waitEsc();
        return;
    }

    textcolor(CYAN);textbackground(BLACK);
    gotoxy(9, 13); printf("Caminho: (0,0) -> (3,4) -> (6,4) -> (6,8)");
    textcolor(WHITE);
    gotoxy(9, 14); printf("Tamanho do caminho: %.2f unidades.",
        pontos_tamanhoCaminho(&caminho));

    Ponto *encontrado = pontos_buscar(&caminho, 3.0f, 4.0f);
    textcolor(GREEN);
    gotoxy(9, 16); printf("Busca por (3,4): %s",
        encontrado != NULL ? "ponto encontrado" : "ponto nao encontrado");

    if (pontos_remover(&caminho, 3.0f, 4.0f))
    {
        textcolor(YELLOW);
        gotoxy(9, 17); printf("Após remover (3,4), tamanho do caminho: %.2f unidades",
            pontos_tamanhoCaminho(&caminho));
    }
    else
    {
        textcolor(RED);
        gotoxy(9, 17); printf("Nao foi possivel remover o ponto (3,4).");
    }

    pontos_apagarTudo(&caminho);

    textcolor(GREEN);textbackground(BLACK);
    gotoxy(9, 19); printf("Lista de pontos liberada :)");

    //---------------------------------------------------
    waitEsc();
}