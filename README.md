# ART Mondrian

**A C learning project for data structures, search and sorting algorithms — with an interactive terminal interface, sound, animations, and a Mondrian-inspired presentation.**

[Português](#português) · [English](#english)

<p align="center">
  <img width="761" src="https://github.com/user-attachments/assets/34dc118d-25f4-489d-8f5c-945327f11380" alt="ART Mondrian interactive terminal menu" />
</p>

---

## Português

O **ART Mondrian** é uma aplicação didática desenvolvida em **C** para reunir, em um único programa, exercícios e experimentos realizados na disciplina de Estrutura de Dados. O projeto organiza atividades de estruturas de dados, algoritmos de busca e ordenação em um menu interativo de terminal.

A ideia foi além de simplesmente armazenar exercícios: o programa ganhou uma interface colorida, navegação por teclado, animações, músicas e efeitos sonoros. Também inclui duas funcionalidades extras: uma calculadora de derivadas e um jogo da cobrinha.

### O que você encontra no programa

- Exercícios de estruturas de dados: pilhas, filas, palíndromos, balanceamento, problema de Josephus e listas simplesmente encadeadas.
- Heap e exercícios relacionados.
- Busca sequencial e busca binária.
- Algoritmos de ordenação: Bubble Sort, Selection Sort, Insertion Sort, Merge Sort, Quick Sort, Radix Sort e Heap Sort.
- Rotinas de comparação de algoritmos de ordenação e busca.
- Geração de arquivos de entrada para os experimentos da aula 11.
- **Calculadora de derivadas** — uma ferramenta interativa para experimentar expressões e derivadas.
- **Jogo da cobrinha** — uma pequena experiência jogável integrada ao menu, com elementos visuais e áudio.

### Apresentação Quick Sort — template Mondrian

O repositório também contém o código-fonte de uma apresentação acadêmica sobre Quick Sort, desenvolvida em **LaTeX/Beamer**.

O design foi inspirado na linguagem visual de **Piet Mondrian**: composição geométrica, linhas pretas, fundo claro e uma paleta reduzida de vermelho, azul e amarelo, equilibrada com preto e branco. Uma regra importante do projeto visual é que blocos coloridos não se encostem diretamente: linhas ou espaços neutros fazem a separação.

A intenção foi criar um modelo de apresentação limpo e reutilizável, no qual a identidade visual acompanhasse o conteúdo sem competir com ele.

- [Abrir a pasta da apresentação](presentation-quicksort/)
- [Ver o arquivo principal LaTeX (main.tex)](presentation-quicksort/main.tex)

### Compilação e execução

**Requisitos**

- Windows
- GCC/MinGW
- `mingw32-make`

Com os requisitos instalados, abra o terminal na raiz do repositório e execute:

```bash
mingw32-make
main.exe
```

Para remover os arquivos de compilação:

```bash
mingw32-make clean
```

O Makefile utiliza GCC com opções de compilação C11. O sistema de áudio utiliza a biblioteca BASS e os arquivos de áudio incluídos no projeto.

### Controles

| Tecla | Ação |
| --- | --- |
| Setas direcionais | Navegar pelo menu |
| Enter | Selecionar uma opção |
| Ctrl + X | Sair do programa |
| Esc | Encerrar o jogo da cobrinha |
| Espaço | Disparar durante o jogo |

### Estrutura do repositório

| Caminho | Conteúdo |
| --- | --- |
| `main.c`, `graphics.c`, `sound.c`, `utils.c` | Aplicação, interface de terminal, áudio e utilidades |
| `problems/` | Exercícios organizados por aula |
| `eastereggs/devcalc/` | Calculadora de derivadas |
| `eastereggs/cobra/` | Jogo da cobrinha |
| `ascii/` | Artes utilizadas nas telas |
| `audio/` | Músicas e efeitos sonoros |
| `lib/` | Arquivos relacionados à biblioteca BASS |
| `presentation-quicksort/` | Fontes da apresentação Quick Sort em LaTeX/Beamer |

### Licença

Este projeto está disponibilizado sob a licença [MIT](LICENSE). Componentes e bibliotecas de terceiros, incluindo dependências de áudio, permanecem sujeitos às respectivas licenças e condições de uso.

Projeto desenvolvido para fins de estudo e experimentação com a linguagem C.

---

## English

**ART Mondrian** is an educational application written in **C**, bringing together exercises and experiments from a Data Structures course in a single interactive terminal program. It covers data structures, search algorithms, and sorting algorithms.

The project grew beyond a collection of exercises: it features a colorful terminal interface, keyboard navigation, animations, music, and sound effects. It also includes two extra features: a derivative calculator and a Snake game.

### What's included

- Data structure exercises: stacks, queues, palindromes, bracket balancing, the Josephus problem, and singly linked lists.
- Heap and related exercises.
- Sequential search and binary search.
- Sorting algorithms: Bubble Sort, Selection Sort, Insertion Sort, Merge Sort, Quick Sort, Radix Sort, and Heap Sort.
- Sorting and searching comparison routines.
- Input-file generation for the experiments in lesson 11.
- **Derivative calculator** — an interactive tool for experimenting with expressions and derivatives.
- **Snake game** — a small playable experience integrated into the menu, with visual elements and audio.

### Quick Sort presentation — Mondrian-inspired template

This repository also includes the source files for an academic presentation about Quick Sort, created with **LaTeX/Beamer**.

Its visual design takes inspiration from **Piet Mondrian**: geometric composition, strong black lines, a light background, and a limited palette of red, blue, and yellow balanced with black and white. One of the visual rules is that colored blocks should never touch each other directly; neutral lines or spaces keep them separated.

The goal was to create a clean, reusable presentation template whose visual identity supports the content instead of competing with it.

- [Browse the presentation directory](presentation-quicksort/)
- [View the main LaTeX source (main.tex)](presentation-quicksort/main.tex)

### Build and run

**Requirements**

- Windows
- GCC/MinGW
- `mingw32-make`

With the requirements installed, open a terminal in the repository root and run:

```bash
mingw32-make
main.exe
```

To remove generated build files:

```bash
mingw32-make clean
```

The Makefile uses GCC with C11 compilation options. Audio playback uses the BASS library and the audio files included in the project.

### Controls

| Key | Action |
| --- | --- |
| Arrow keys | Navigate the menu |
| Enter | Select an option |
| Ctrl + X | Exit the program |
| Esc | Exit the Snake game |
| Space | Shoot during the game |

### Repository structure

| Path | Contents |
| --- | --- |
| `main.c`, `graphics.c`, `sound.c`, `utils.c` | Application, terminal interface, audio, and utilities |
| `problems/` | Exercises organized by lesson |
| `eastereggs/devcalc/` | Derivative calculator |
| `eastereggs/cobra/` | Snake game |
| `ascii/` | Artwork used in the screens |
| `audio/` | Music and sound effects |
| `lib/` | Files related to the BASS library |
| `presentation-quicksort/` | LaTeX/Beamer source files for the Quick Sort presentation |

### License

This project is distributed under the [MIT License](LICENSE). Third-party components and libraries, including audio dependencies, remain subject to their respective licenses and terms of use.

Created for learning and experimentation with the C programming language.

---

2026 · Janderson Gomes (Caraumã) · [carauma.com](https://carauma.com) · [GitHub repository](https://github.com/carauma7/art-mondrian)
