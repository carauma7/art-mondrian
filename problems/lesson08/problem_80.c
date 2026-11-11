#include <stdio.h>
#include <string.h>

#include "../../graphics.h"
#include "../../sound.h"
#include "../../utils.h"

#include "problem_80.h"

const HeapQuestion heap_questions[] = {
    {
        "O que é um heap? Explique brevemente como os elementos são organizados.",
        "Heap é uma árvore binária quase completa, normalmente armazenada em um vetor. "
        "Ela obedece a uma propriedade de ordem entre cada pai e seus filhos."
    },
    {
        "Em uma árvore que representa um heap, qual parte pode não estar completamente preenchida?",
        "Somente o último nível pode não estar completo. Seus elementos ficam preenchidos "
        "da esquerda para a direita, sem deixar espaços entre eles."
    },
    {
        "Em um max-heap, o valor da raiz é maior, menor ou igual aos valores dos demais nós? Explique.",
        "A raiz é maior ou igual aos seus filhos. Como essa propriedade se repete em todos "
        "os níveis, a raiz contém um dos maiores valores do heap."
    },
    {
        "Qual é a diferença entre um max-heap e um min-heap?",
        "No max-heap, cada pai é maior ou igual aos filhos. No min-heap, cada pai é menor "
        "ou igual aos filhos."
    },
    {
        "Considere A = [20, 15, 18, 8, 10, 12, 14], usando índices a partir de 1. Qual é o valor da raiz?",
        "A raiz está na posição 1. Portanto, o valor armazenado na raiz é 20."
    },
    {
        "No vetor anterior, qual é o índice do pai do elemento que está na posição 5?",
        "Para um vetor com índices a partir de 1, o pai de i está em floor(i / 2). "
        "Assim, o pai da posição 5 está na posição 2."
    },
    {
        "Se um nó está na posição 3, quais são as posições de seu filho esquerdo e direito?",
        "Os filhos de i ficam em 2*i e 2*i+1. Para i igual a 3, as posições são 6 "
        "para o filho esquerdo e 7 para o filho direito."
    },
    {
        "Considere um heap com 7 elementos. Quais posições do vetor correspondem às folhas?",
        "As folhas ocupam as posições 4, 5, 6 e 7. Em geral, as folhas de um heap com n "
        "elementos começam em floor(n / 2) + 1."
    },
    {
        "A = [30, 20, 25, 10, 15, 18, 22] representa um max-heap? Justifique.",
        "Sim. A raiz 30 é maior que 20 e 25; o nó 20 é maior que 10 e 15; "
        "o nó 25 é maior que 18 e 22. Todas as relações pai-filho são válidas."
    },
    {
        "A = [10, 20, 15, 5, 8] representa um max-heap? Por quê?",
        "Não. A raiz 10 tem um filho 20 maior que ela, violando a propriedade de max-heap."
    },
    {
        "A = [40, 25, 30, 10, 20]. Identifique a raiz e os dois filhos da raiz.",
        "A raiz é 40, na posição 1. Seus filhos são 25, na posição 2, e 30, na posição 3."
    },
    {
        "Para que serve o procedimento MAX-HEAPIFY?",
        "Ele corrige a propriedade de max-heap em um nó que pode estar menor que um de "
        "seus filhos. O maior filho sobe e o valor continua descendo até a posição correta."
    },
    {
        "Ao executar MAX-HEAPIFY, o valor de um nó pode ser deslocado para cima ou para baixo?",
        "No MAX-HEAPIFY, o valor do nó pode descer, ou flutuar para baixo, enquanto o "
        "maior filho sobe. Esse processo termina quando a propriedade é restaurada."
    },
    {
        "Para que serve o procedimento BUILD-MAX-HEAP?",
        "Ele transforma um vetor sem ordem garantida em um max-heap, aplicando "
        "MAX-HEAPIFY nos nós internos, do último pai até a raiz."
    },
    {
        "Em uma fila de prioridade, o que a operação EXTRACT-MAX faz?",
        "Ela remove e devolve o maior elemento, que está na raiz do max-heap. Depois, "
        "o último elemento ocupa a raiz e o heap é corrigido."
    },
    {
        "Dê um exemplo simples de situação em que uma fila de prioridade seria útil.",
        "Em um sistema operacional, processos urgentes podem receber prioridade maior. "
        "A fila permite executar primeiro o processo de maior prioridade."
    },
    {
        "Considere A = [10, 30, 20]. a) Desenhe a árvore binária representada pelo vetor.",
        "A árvore é: 10 na raiz, 30 como filho esquerdo e 20 como filho direito."
    },
    {
        "b) O vetor [10, 30, 20] satisfaz a propriedade de max-heap? Por quê?",
        "Não. A raiz 10 é menor que os filhos 30 e 20, portanto a propriedade de max-heap "
        "é violada nas duas comparações."
    },
    {
        "c) Troque os elementos de posição para obter um max-heap.",
        "Trocando 10 com 30, obtemos [30, 10, 20]. Agora 30 é maior que 10 e 20, "
        "então o vetor representa um max-heap."
    }
};

const size_t heap_question_count = sizeof(heap_questions) / sizeof(heap_questions[0]);
HeapLine heap_lines[HEAP_MAX_LINES];
int heap_line_count;

static void heap_add_line(const char *text, int color)
{
    if (heap_line_count >= HEAP_MAX_LINES)
    {
        return;
    }

    snprintf(heap_lines[heap_line_count].text, sizeof(heap_lines[heap_line_count].text), "%s", text);
    heap_lines[heap_line_count].color = color;
    heap_line_count++;
}

static void heap_add_wrapped(const char *prefix, const char *text, int color)
{
    const char *word = text;
    char line[HEAP_LINE_WIDTH + 1];
    size_t line_length;
    size_t word_length;
    int first_word = 1;

    snprintf(line, sizeof(line), "%s", prefix);
    line_length = strlen(line);

    while (*word != '\0')
    {
        while (*word == ' ')
        {
            word++;
        }

        if (*word == '\0')
        {
            break;
        }

        word_length = 0;
        while (word[word_length] != '\0' && word[word_length] != ' ')
        {
            word_length++;
        }

        if (!first_word && line_length + 1 + word_length > HEAP_LINE_WIDTH)
        {
            heap_add_line(line, color);
            snprintf(line, sizeof(line), "    ");
            line_length = 4;
            first_word = 1;
        }

        if (!first_word)
        {
            line[line_length++] = ' ';
        }

        if (word_length > HEAP_LINE_WIDTH - line_length)
        {
            word_length = HEAP_LINE_WIDTH - line_length;
        }

        memcpy(line + line_length, word, word_length);
        line_length += word_length;
        line[line_length] = '\0';
        first_word = 0;
        word += word_length;
    }

    if (line_length > 0)
    {
        heap_add_line(line, color);
    }
}

static void heap_build_lines(void)
{
    static const int question_colors[] = { YELLOW, WHITE, BLUE, RED };
    static const int answer_colors[] = { WHITE, YELLOW, LIGHTBLUE, LIGHTRED };
    size_t i;
    char prefix[16];

    heap_line_count = 0;
    for (i = 0; i < heap_question_count; i++)
    {
        snprintf(prefix, sizeof(prefix), "%lu. ", (unsigned long)(i + 1));
        heap_add_wrapped(
            prefix,
            heap_questions[i].question,
            question_colors[i % (sizeof(question_colors) / sizeof(question_colors[0]))]
        );
        heap_add_wrapped(
            "Resposta: ",
            heap_questions[i].answer,
            answer_colors[i % (sizeof(answer_colors) / sizeof(answer_colors[0]))]
        );
        heap_add_line("", WHITE);
    }
}

static void heap_refresh_content(int first_line)
{
    int i;
    int row;

    textbackground(BLACK);
    for (row = HEAP_FIRST_ROW; row <= HEAP_LAST_ROW; row++)
    {
        gotoxy(9, row);
        printf("%-61s", "");
    }

    for (i = 0; i <= HEAP_LAST_ROW - HEAP_FIRST_ROW; i++)
    {
        int line_index = first_line + i;

        if (line_index >= heap_line_count)
        {
            break;
        }

        textbackground(BLACK);
        gotoxy(9, HEAP_FIRST_ROW + i);
        textcolor(heap_lines[line_index].color);
        printf("%s", heap_lines[line_index].text);
    }

    textcolor(WHITE);
    textbackground(BLACK);
    gotoxy(9, 21);
    printf("[↑] anterior  [↓] próxima  [%d/%d linhas]  [Esc] voltar",
           first_line + 1, heap_line_count);
    gotoxy(SCREEN_WIDTH, SCREEN_HEIGHT);
}

static void heap_draw_page(int selected, int first_line)
{
    draw_problem_screen(selected, "    AULA 08 - HEAP");
    heap_refresh_content(first_line);
}

void problem_80(void)
{
    int first_line = 0;
    KEY key;

    heap_build_lines();
    heap_draw_page(8, first_line);

    for (;;)
    {
        key = getKeypressed();

        if (key.x == 27 && key.y == 91 && key.z == 65)
        {
            audio_play(&audio_effectchannel, AUDIO_MENU_SELECT, FALSE);
            if (first_line > 0)
            {
                first_line--;
                heap_refresh_content(first_line);
            }
        }
        else if (key.x == 27 && key.y == 91 && key.z == 66)
        {
            audio_play(&audio_effectchannel, AUDIO_MENU_SELECT, FALSE);
            if (first_line < heap_line_count - 1)
            {
                first_line++;
                heap_refresh_content(first_line);
            }
        }
        else if (key.x == 27)
        {
            break;
        }
    }
}
