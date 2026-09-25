#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../graphics.h"
#include "../../utils.h"

#include "problem_7.h"

/*
* PROBLEMA 7 - Criar uma lista simplesmente encadeada para
* cadastrar alunos, isto é, nome e três notas para cada aluno.
* Realizar inserção, remoção, atualização e busca.
*/
unsigned char ascii_problema_7[] = {
  0x50, 0x52, 0x4f, 0x42, 0x4c, 0x45, 0x4d, 0x41, 0x20, 0x37, 0x20, 0x2d,
  0x20, 0x43, 0x72, 0x69, 0x61, 0x72, 0x20, 0x75, 0x6d, 0x61, 0x20, 0x6c,
  0x69, 0x73, 0x74, 0x61, 0x20, 0x73, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x73,
  0x6d, 0x65, 0x6e, 0x74, 0x65, 0x20, 0x65, 0x6e, 0x63, 0x61, 0x64, 0x65,
  0x61, 0x64, 0x61, 0x20, 0x70, 0x61, 0x72, 0x61, 0x0a, 0x63, 0x61, 0x64,
  0x61, 0x73, 0x74, 0x72, 0x61, 0x72, 0x20, 0x61, 0x6c, 0x75, 0x6e, 0x6f,
  0x73, 0x2c, 0x20, 0x69, 0x73, 0x74, 0x6f, 0x20, 0xc3, 0xa9, 0x2c, 0x20,
  0x6e, 0x6f, 0x6d, 0x65, 0x20, 0x65, 0x20, 0x74, 0x72, 0xc3, 0xaa, 0x73,
  0x20, 0x6e, 0x6f, 0x74, 0x61, 0x73, 0x20, 0x70, 0x61, 0x72, 0x61, 0x20,
  0x63, 0x61, 0x64, 0x61, 0x20, 0x61, 0x6c, 0x75, 0x6e, 0x6f, 0x2e, 0x0a,
  0x52, 0x65, 0x61, 0x6c, 0x69, 0x7a, 0x61, 0x72, 0x20, 0x69, 0x6e, 0x73,
  0x65, 0x72, 0xc3, 0xa7, 0xc3, 0xa3, 0x6f, 0x2c, 0x20, 0x72, 0x65, 0x6d,
  0x6f, 0xc3, 0xa7, 0xc3, 0xa3, 0x6f, 0x2c, 0x20, 0x61, 0x74, 0x75, 0x61,
  0x6c, 0x69, 0x7a, 0x61, 0xc3, 0xa7, 0xc3, 0xa3, 0x6f, 0x20, 0x65, 0x20,
  0x62, 0x75, 0x73, 0x63, 0x61, 0x2e
};
unsigned int ascii_problema_7_length = 174;

void alunos_inicializar ( Aluno **inicio ) 
{
    *inicio = NULL;
}

void alunos_apagarTudo ( Aluno **inicio )
{
    Aluno *atual = *inicio;
    while ( atual != NULL ) {
        Aluno *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    *inicio = NULL;
}

void alunos_imprimir ( Aluno **inicio ) 
{
    int indice = 0;
    Aluno *atual = *inicio;
    textcolor(WHITE);
    gotoxy(9, 10); printf("NOME \t\t NOTA 1  NOTA 2  NOTA 3  MÉDIA\n");
    while ( atual != NULL )
    {
        float mediaGeral = (atual->notas[0] + atual->notas[1] + atual->notas[2] ) / 3.0f;
        gotoxy(9, 10 + indice + 1);
        textcolor(WHITE);
        printf("%s \t", atual->nome);

        textcolor(atual->notas[0] < 7.0f ? RED : atual->notas[0] > 7.0f ? GREEN : WHITE);
        printf("%.2f \t", atual->notas[0]);
        textcolor(atual->notas[1] < 7.0f ? RED : atual->notas[1] > 7.0f ? GREEN : WHITE);
        printf("%.2f \t", atual->notas[1]);
        textcolor(atual->notas[2] < 7.0f ? RED : atual->notas[2] > 7.0f ? GREEN : WHITE);
        printf("%.2f \t", atual->notas[2]);
        textcolor(mediaGeral < 7.0f ? RED : mediaGeral > 7.0f ? GREEN : WHITE);
        printf("%.2f\n", mediaGeral);

        indice++;        
        atual = atual->proximo;
    }
    textcolor(WHITE);
}

int alunos_cadastrar ( Aluno **inicio, char *nome, float nota1, float nota2, float nota3)
{
    Aluno *novo = (Aluno *) malloc ( sizeof ( Aluno ) );

    if ( novo == NULL ) return -1;

    strcpy(novo->nome, nome);
    novo->notas[0] = nota1; 
    novo->notas[1] = nota2;
    novo->notas[2] = nota3;

    novo->proximo = *inicio;
    *inicio = novo;

    return 0;
}

Aluno *alunos_remover ( Aluno **inicio, char *nome ) 
{
    Aluno *atual = *inicio;
    Aluno *anterior = NULL;
    while ( atual != NULL && strcmp( atual->nome, nome ) != 0 )
    {
        anterior = atual;
        atual = atual->proximo;
    }

    if ( atual == NULL )
    {
        return NULL;
    }

    if ( anterior == NULL )
    {
        *inicio = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    return atual;
}

Aluno *alunos_buscar ( Aluno **inicio, char *nome)
{
    Aluno *atual = *inicio;
    while ( atual != NULL && strcmp ( atual->nome, nome ) != 0 ) 
    {
        atual = atual->proximo;
    }
    return atual;
}

Aluno *alunos_atualizar ( Aluno **inicio, char *nome, float nota1, float nota2, float nota3)
{
    Aluno *atual = *inicio;
    while ( atual != NULL && strcmp ( atual->nome, nome ) != 0 )
    {
        atual = atual->proximo;
    }

    if ( atual == NULL ) 
    {
        return NULL;
    }

    strcpy(atual->nome, nome);
    atual->notas[0] = nota1; atual->notas[1] = nota2; atual->notas[2] = nota3;

    return atual;
}

void problem_7(void)
{
    draw_problem_screen( 4 , ( const char * ) ascii_problema_7 );
    //----------------------------------------------------
    Aluno *_lista;
    alunos_inicializar ( &_lista );

    alunos_cadastrar ( &_lista, "O. Sumido", 7.77, 7.77, 7.77 );

    Aluno *remover = alunos_remover ( &_lista, "O. Sumido");
    gotoxy(9, 15); remover != NULL ? printf("Aluno '%s' foi removido.", remover->nome) : printf("Alun '%s' não cadastrado.", "O. Sumido");

    alunos_cadastrar ( &_lista, "A. Tordoada", 7.12, 8.34, 7.56 );
    alunos_cadastrar ( &_lista, "B. B. Cana",  9.78, 6.89, 7.98 );
    alunos_cadastrar ( &_lista, "A. Toa    ",  6.89, 9.87, 8.76 );

    Aluno *buscar1 = alunos_buscar ( &_lista, "A. Tordoada");
    Aluno *buscar2 = alunos_buscar ( &_lista, "A. Bilolado");

    gotoxy(9, 16); buscar1 != NULL ? printf("Aluna '%s' cadastrada.", buscar1->nome) : printf("Aluna '%s' não cadastrada.", "A. Tordoada");
    gotoxy(9, 17); buscar2 != NULL ? printf("Aluno '%s' cadastrado.", buscar1->nome) : printf("Aluno '%s' não cadastrado.", "A. Bilolado");

    Aluno *atualizar = alunos_atualizar ( &_lista, "A. Tordoada", 7.12, 8.34, 8.56);
    gotoxy(9, 18); atualizar != NULL ? printf("Atualizou a NOTA 3 da '%s' de '%.2f' para '%.2f'.", atualizar->nome, 7.56, atualizar->notas[2]) : printf("Aluna '%s' não cadastrada.", "A. Tordoada");

    alunos_imprimir ( &_lista );

    //textcolor(WHITE); 
    //textbackground(BLACK);

    //textcolor(GREEN);
    //gotoxy(9, 10); printf("Digite o número de crianças (n)");
    //gotoxy(9, 11); printf("ou 'sair' para encerrar.");

    alunos_apagarTudo ( &_lista );
    //---------------------------------------------------
    waitEsc();
}