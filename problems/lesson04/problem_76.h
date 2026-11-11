#ifndef PROBLEM_76_H
#define PROBLEM_76_H

typedef struct Ponto {
    float x;
    float y;
    struct Ponto *proximo;
} Ponto;

void problem_76(void);

void pontos_inicializar(Ponto **inicio);
void pontos_apagarTudo(Ponto **inicio);
int pontos_inserir(Ponto **inicio, float x, float y);
Ponto *pontos_buscar(Ponto **inicio, float x, float y);
int pontos_remover(Ponto **inicio, float x, float y);
float pontos_tamanhoCaminho(Ponto **inicio);

#endif /* PROBLEM_76_H */