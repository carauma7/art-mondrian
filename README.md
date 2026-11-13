# ART Mondrian

Aplicação didática em C para executar exercícios de estruturas de dados,
algoritmos de busca e ordenação em um menu interativo de terminal.

## Conteúdo

O menu principal reúne:

- exercícios das aulas 01, 02, 03, 04, 05 e 08;
- pilhas, filas, palíndromos, balanceamento e problema de Josephus;
- listas simplesmente encadeadas e exercícios relacionados;
- heap;
- buscas sequencial e binária;
- bubble sort, selection sort, insertion sort, merge sort, quick sort,
  radix sort e heap sort;
- comparativos de ordenação e de buscas com 10.000 elementos;
- geração de arquivos para o trabalho da aula 11;
- calculadora de derivadas;
- jogo da cobrinha.

## Estrutura

- `main.c`, `graphics.c`, `sound.c` e `utils.c`: aplicação, interface de
  terminal, áudio e utilidades;
- `problems/`: exercícios organizados por aula;
- `eastereggs/`: calculadora de derivadas (`devcalc`) e jogo da cobrinha
  (`cobra`);
- `ascii/`: artes usadas nas telas;
- `audio/`: músicas e efeitos sonoros;
- `lib/`: arquivos da biblioteca BASS;
- `presentation-quicksort/`: apresentação sobre quicksort.

## Compilação e execução

Requisitos:

- Windows;
- GCC/MinGW;
- `mingw32-make`.

Na raiz do repositório, execute:

```bash
mingw32-make
main.exe
```

O executável gerado é `main.exe`. Para remover os arquivos de compilação:

```bash
mingw32-make clean
```

O áudio usa a biblioteca BASS e os arquivos disponíveis no repositório.

## Controles

- setas: navegar pelo menu;
- Enter: selecionar uma opção;
- Ctrl + X: sair;
- no jogo da cobrinha, Esc encerra e Espaço dispara.

Projeto para fins de estudo e experimentação com C.

---

2026 (c) J. Caraumã <https://carauma.com>.
