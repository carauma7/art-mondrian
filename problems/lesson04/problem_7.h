#ifndef PROBLEM_7_H
#define PROBLEM_7_H

typedef struct Aluno {
    char nome[50];
    float notas[3];
    struct Aluno *proximo;
} Aluno;

void problem_7(void);

void alunos_inicializar ( Aluno **alunos );
void alunos_apagarTudo ( Aluno **alunos );
void alunos_imprimir ( Aluno **alunos, int x, int y );
int alunos_cadastrar ( Aluno **inicio, char *nome, float nota1, float nota2, float nota3);
int alunos_totalDeAlunos ( Aluno **inicio );
Aluno *alunos_remover ( Aluno **inicio, char *nome );
Aluno *alunos_buscar ( Aluno **inicio, char *nome );
Aluno *alunos_atualizar ( Aluno **inicio, char *nome, float nota1, float nota2, float nota3);
Aluno *alunos_maiorMedia ( Aluno **inicio );
Aluno *alunos_menorMedia ( Aluno **inicio );
Aluno *alunos_maiorNota ( Aluno **inicio );
Aluno *alunos_menorNota ( Aluno **inicio );
Aluno *alunos_concatenar ( Aluno **listaA, Aluno **listaB );


#endif /* PROBLEM_7_H */