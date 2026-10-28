#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "devcalc.h"

// Cantos da área interna preta desenhada por draw_problem_screen() (x: 8..73, y: 6..22).
#define DEVCALC_X 9
#define DEVCALC_Y_TOP 6
#define DEVCALC_Y_BOTTOM 22
#define DEVCALC_BOX_LEFT 8
#define DEVCALC_BOX_RIGHT 73
#define DEVCALC_BOX_WIDTH (DEVCALC_BOX_RIGHT - DEVCALC_BOX_LEFT + 1)
#define DEVCALC_INPUT_LABEL_WIDTH 9
#define DEVCALC_INPUT_WIDTH \
    (DEVCALC_BOX_RIGHT - DEVCALC_X - DEVCALC_INPUT_LABEL_WIDTH + 1)

static const char devcalc_title[] =
    "CALCULADORA DE DERIVADAS - v0.1 - DEUS É BOM O TEMPO TODO!";

static int devcalc_mondrian_background(int screen_x, int screen_y)
{
    int x = screen_x - 1;
    int y = screen_y - 1;
    int vertical_line =
        (x == 8 && y < 17) ||
        (x == 18) ||
        (x == 28 && y >= 6 && y < 21) ||
        (x == 42) ||
        (x == 55 && (y < 10 || y >= 14)) ||
        (x == 65 && y >= 3 && y < 17) ||
        (x == 72 && y >= 10);
    int horizontal_line =
        (y == 3 && x >= 18) ||
        (y == 6 && (x < 42 || x >= 55)) ||
        (y == 10 && x >= 8 && x < 65) ||
        (y == 14) ||
        (y == 17 && x < 55) ||
        (y == 21 && x >= 18);

    if (vertical_line || horizontal_line)
    {
        return BLACK;
    }

    if ((x > 18 && x < 42 && y > 6 && y < 14) ||
        (x < 8 && y > 17) ||
        (x > 65 && x < 72 && y > 3 && y < 10))
    {
        return RED;
    }

    if ((x > 42 && x < 55 && y < 3) ||
        (x < 8 && y < 6) ||
        (x > 55 && x < 65 && y > 10 && y < 14) ||
        (x > 72 && y > 21))
    {
        return YELLOW;
    }

    if ((x > 8 && x < 18 && y > 10 && y < 17) ||
        (x > 55 && x < 65 && y < 3) ||
        (x > 42 && x < 55 && y > 17 && y < 21))
    {
        return BLUE;
    }

    return WHITE;
}

static int devcalc_title_foreground(int background)
{
    if (background == WHITE || background == YELLOW)
    {
        return BLACK;
    }

    return WHITE;
}

static int devcalc_remove_last_utf8_character(
    char *buffer,
    int length
)
{
    if (length == 0)
    {
        return 0;
    }

    length--;
    while (
        length > 0 &&
        (((unsigned char) buffer[length] & 0xC0) == 0x80)
    )
    {
        length--;
    }

    buffer[length] = 0;
    return length;
}

static int devcalc_utf8_length(const char *text)
{
    int length = 0;

    while (*text != 0)
    {
        if ((((unsigned char) *text) & 0xC0) != 0x80)
        {
            length++;
        }
        text++;
    }

    return length;
}

static const char *devcalc_utf8_at(const char *text, int index)
{
    while (*text != 0 && index > 0)
    {
        if ((((unsigned char) *text) & 0xC0) != 0x80)
        {
            index--;
            text++;

            while (
                *text != 0 &&
                (((unsigned char) *text & 0xC0) == 0x80)
            )
            {
                text++;
            }
            continue;
        }

        text++;
    }

    return text;
}

static void devcalc_draw_title(int offset)
{
    int title_length = devcalc_utf8_length(devcalc_title);
    int column;

    gotoxy(DEVCALC_BOX_LEFT, DEVCALC_Y_TOP);

    for (column = 0; column < DEVCALC_BOX_WIDTH; column++)
    {
        int source = column - offset;
        const char *character;
        const char *next_character;
        int background = devcalc_mondrian_background(
            DEVCALC_BOX_LEFT + column,
            DEVCALC_Y_TOP
        );

        textcolor(devcalc_title_foreground(background));
        textbackground(background);

        if (source >= 0 && source < title_length)
        {
            character = devcalc_utf8_at(devcalc_title, source);
            next_character = devcalc_utf8_at(devcalc_title, source + 1);
            fwrite(
                character,
                1,
                (size_t) (next_character - character),
                stdout
            );
        }
        else
        {
            putchar(' ');
        }
    }

    textcolor(WHITE);
    textbackground(BLACK);
    fflush(stdout);
}

static int devcalc_read_line(
    char *buffer,
    int capacity,
    int line
)
{
    int title_length = devcalc_utf8_length(devcalc_title);
    int length = 0;
    int frame = 0;
    int offset;
    int travel = DEVCALC_BOX_WIDTH + title_length + 8;
    int visible_start;
    int visible_length;
    const char *visible_buffer;
    DWORD next_frame = GetTickCount();
    int key;

    buffer[0] = 0;

    for (;;)
    {
        if (GetTickCount() >= next_frame)
        {
            offset = DEVCALC_BOX_WIDTH - (frame % travel);
            devcalc_draw_title(offset);

            gotoxy(DEVCALC_X, line);
            textbackground(BLACK);
            textcolor(GREEN);
            printf("Entrada: ");
            textcolor(WHITE);
            visible_start = length > DEVCALC_INPUT_WIDTH
                ? length - DEVCALC_INPUT_WIDTH
                : 0;
            visible_length = length - visible_start;
            visible_buffer = buffer + visible_start;
            printf("%-*.*s", DEVCALC_INPUT_WIDTH,
                DEVCALC_INPUT_WIDTH, visible_buffer);
            gotoxy(
                DEVCALC_X + DEVCALC_INPUT_LABEL_WIDTH + visible_length,
                line
            );
            fflush(stdout);

            frame++;
            next_frame = GetTickCount() + 40;
        }

        if (!keypressed_available())
        {
            delay(5);
            continue;
        }

        key = getch();

        if (key == 10)
        {
            return 1;
        }

        if (key == 8)
        {
            length = devcalc_remove_last_utf8_character(buffer, length);
        }
        else if (
            key >= 32 && key != 127 &&
            length < capacity - 1
        )
        {
            buffer[length++] = (char) key;
            buffer[length] = 0;
        }
    }
}

// Limpa apenas a área interna preta, preservando a moldura Mondrian ao redor.
static void devcalc_clear_box(void)
{
    int x, y;

    textbackground(BLACK);
    for (y = DEVCALC_Y_TOP; y <= DEVCALC_Y_BOTTOM; y++)
    {
        for (x = 8; x <= 73; x++)
        {
            gotoxy(x, y);
            printf(" ");
        }
    }
}

// Detecta identificadores alfabéticos que não são "x", "e", "pi" nem uma das funções
// suportadas (o parser não valida isso e trava com entradas como "sin(x)").
static bool devcalc_has_unknown_token(char *str)
{
    int i = 0, len = strlen(str);

    while (i < len)
    {
        if (isalpha((unsigned char) str[i]))
        {
            int j = i;
            while (j < len && isalpha((unsigned char) str[j]))
            {
                j++;
            }

            int name_len = j - i;
            char name[8];

            if (name_len >= (int) sizeof(name))
            {
                return true;
            }
            strncpy(name, str + i, name_len);
            name[name_len] = 0;

            bool is_var_or_const =
                (strcmp(name, "x") == 0) || (strcmp(name, "e") == 0) || (strcmp(name, "pi") == 0);
            bool is_known_func =
                (strcmp(name, "ln") == 0) || (strcmp(name, "log") == 0) ||
                (strcmp(name, "sen") == 0) || (strcmp(name, "cos") == 0) ||
                (strcmp(name, "tan") == 0) || (strcmp(name, "csc") == 0) ||
                (strcmp(name, "sec") == 0) || (strcmp(name, "cot") == 0) ||
                (strcmp(name, "senh") == 0) || (strcmp(name, "cosh") == 0) ||
                (strcmp(name, "tanh") == 0) || (strcmp(name, "csch") == 0) ||
                (strcmp(name, "sech") == 0) || (strcmp(name, "coth") == 0);

            if (!is_var_or_const && !is_known_func)
            {
                return true;
            }

            i = j;
        }
        else
        {
            i++;
        }
    }

    return false;
}

// Detecta operadores mal formados (soletrados em duplicidade, pendurados no início/fim,
// parênteses vazios etc.) que o parser não valida e travam a diferenciação.
static bool devcalc_has_bad_syntax(char *str)
{
    int i, len = strlen(str);

    if (len == 0)
    {
        return true;
    }
    if (strchr("*/^", str[0]) != NULL)
    {
        return true; // não pode começar com um operador binário
    }
    if (strchr("+-*/^", str[len - 1]) != NULL)
    {
        return true; // não pode terminar com um operador pendurado
    }

    for (i = 0; i < len - 1; i++)
    {
        char c0 = str[i], c1 = str[i + 1];
        bool op0 = strchr("+-*/^", c0) != NULL;
        bool op1 = strchr("+-*/^", c1) != NULL;

        // dois operadores seguidos só fazem sentido como "operador" + "sinal unário"
        if (op0 && op1 && !((strchr("*/^", c0) != NULL) && (strchr("+-", c1) != NULL)))
        {
            return true;
        }
        if ((c0 == '(') && op1 && (strchr("+-", c1) == NULL))
        {
            return true; // "(" seguido de */^ não tem operando à esquerda
        }
        if ((c1 == ')') && op0)
        {
            return true; // operador pendurado antes do fechamento
        }
        if ((c0 == '(') && (c1 == ')'))
        {
            return true; // parênteses vazios
        }
    }

    return false;
}

void devcalcRun(void)
{
    char *m_func = (char *) malloc(sizeof(char) * MAX_CHAR);
    char *input_without_spaces;
    char *derv;
    int line = DEVCALC_Y_TOP;

    draw_problem_screen(DEVCALC_PROBLEM, "");
    hidecursor();

    textbackground(BLACK);
    textcolor(CYAN);
    devcalc_draw_title(0);
    line += 2;
    gotoxy(DEVCALC_X, line);textcolor(WHITE);
    printf("Insira uma expressão e pressione ENTER.");
    line++;
    gotoxy(DEVCALC_X, line);textcolor(LIGHTRED);
    printf("Por exemplo: ln(x) + cos(x^3) + x");
    line += 2;
    gotoxy(DEVCALC_X, line);textcolor(WHITE);
    printf("Para finalizar, digite \"sair\".");
    line += 2;

    textcolor(GREEN);
    gotoxy(DEVCALC_X, line);
    printf("Entrada: ");
    textcolor(WHITE);
    if (!devcalc_read_line(m_func, MAX_CHAR, line))
    {
        hidecursor();
        free(m_func);
        return;
    }
    m_func[strcspn(m_func, "\r\n")] = 0;
    input_without_spaces = wo_space(m_func);
    strcpy(m_func, input_without_spaces);
    free(input_without_spaces);

    while (strcmp(m_func, "sair") != 0)
    {
        line++;
        if (line > DEVCALC_Y_BOTTOM)
        {
            devcalc_clear_box();
            line = DEVCALC_Y_TOP;
        }
        gotoxy(DEVCALC_X, line);

        if (!par_paired(m_func, strlen(m_func)))
        {
            textcolor(RED);
            printf("Número de parênteses abertos/fechados é desigual.");
        }
        else if (devcalc_has_unknown_token(m_func))
        {
            textcolor(RED);
            printf("Expressão não reconhecida.");
        }
        else if (devcalc_has_bad_syntax(m_func))
        {
            textcolor(RED);
            printf("Expressão mal formada.");
        }
        else
        {
            derv = differentiate(simp_input(m_func), 1);
            derv = simp_output(derv);

            while (par_enclosed(derv))
            {
                derv = rm_par(derv);
            }

            textcolor(GREEN);
            printf("Resultado: ");
            textcolor(YELLOW);
            if (strcmp(derv, "") == 0)
            {
                printf("0");
            }
            else if (*derv == '+')
            {
                printf("%s", derv + 1);
            }
            else
            {
                printf("%s", derv);
            }
        }

        line += 2;
        if (line > DEVCALC_Y_BOTTOM)
        {
            devcalc_clear_box();
            line = DEVCALC_Y_TOP;
        }

        gotoxy(DEVCALC_X, line);
        textcolor(GREEN);
        printf("Entrada: ");
        textcolor(WHITE);
        if (!devcalc_read_line(m_func, MAX_CHAR, line))
        {
            break;
        }
        m_func[strcspn(m_func, "\r\n")] = 0;
        input_without_spaces = wo_space(m_func);
        strcpy(m_func, input_without_spaces);
        free(input_without_spaces);
    }

    hidecursor();
    free(m_func);
    textcolor(WHITE);
    textbackground(BLACK);
}
