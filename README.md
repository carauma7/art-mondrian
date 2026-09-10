# ART Mondrian

Projeto em C para estudo de estruturas de dados e lógica de programação, com interface textual em console e elementos visuais inspirados em arte de matriz de cores.

<img width="738" height="521" alt="image" src="https://github.com/user-attachments/assets/ff1d4d5b-336b-43b2-adea-a8467d4cc10a" />

## O que é

Este repositório reúne uma série de exercícios e mini-aplicativos em console, organizados por menu, com foco em:

- problemas de estrutura de dados
- manipulação de ASCII e interface textual
- pequenos jogos/easter eggs
- ferramentas auxiliares em C

## Estrutura principal

- `main.c` e `main.h`: ponto de entrada e configuração do programa
- `problems/`: problemas selecionáveis no menu principal
- `eastereggs/`: mini-aplicativos extras, como `cobra` e `devcalc`
- `graphics.*`, `utils.*`, `sound.*`: base de renderização e utilidades
- `ascii/`: arte textual usada na interface
- `lib/`: biblioteca externa do projeto (`bass`)

## Como executar

No ambiente Windows/MinGW, use:

```bash
make
main.exe
```

O programa é navegável pelo teclado:

- setas: navegar no menu
- Enter: abrir item selecionado
- Ctrl + X: sair

## Menu principal

O projeto oferece exercícios e ferramentas, incluindo:

- problemas de algoritmos e estruturas
- calculadora de derivadas (`devcalc`)
- jogo da cobrinha (`cobra`)

## Observação

É apenas um projeto didático, com foco em registrar algumas implementações em terminal usando C.

---
2026 (c) J. Caraumã <https://carauma.com>.
