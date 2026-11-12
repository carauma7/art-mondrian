#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../graphics.h"
#include "../../utils.h"
#include "problem_110.h"
#include "sequential-search.h"
#include "binary-search.h"
#include "bubble-sort.h"
#include "selection-sort.h"
#include "insertion-sort.h"
#include "merge-sort.h"
#include "quick-sort.h"
#include "radix-sort.h"
#include "heap-sort.h"

#define LESSON_11_DATA_DIR "problems/lesson11/data"
#define LESSON_11_MAX_LINES 128
#define LESSON_11_LINE_WIDTH 61

typedef struct
{
    char text[LESSON_11_LINE_WIDTH + 1];
    int color;
} Lesson11Line;

static void clear_lesson11_screen(void)
{
    clrscr();
    textbackground(BLACK);
    textcolor(WHITE);
}

static double elapsed_milliseconds(LARGE_INTEGER start)
{
    LARGE_INTEGER end;
    LARGE_INTEGER frequency;

    QueryPerformanceCounter(&end);
    QueryPerformanceFrequency(&frequency);
    return (double)(end.QuadPart - start.QuadPart) * 1000.0 /
           (double)frequency.QuadPart;
}

static void format_duration(char *buffer, size_t size, double milliseconds)
{
    if (milliseconds < 1.0)
    {
        snprintf(buffer, size, "%.3f us", milliseconds * 1000.0);
    }
    else if (milliseconds < 1000.0)
    {
        snprintf(buffer, size, "%.3f ms", milliseconds);
    }
    else
    {
        snprintf(buffer, size, "%.3f s", milliseconds / 1000.0);
    }
}

static void format_processing_rate(
    char *buffer,
    size_t size,
    size_t processed,
    double milliseconds)
{
    if (milliseconds <= 0.0)
    {
        snprintf(buffer, size, "N/A");
        return;
    }
    snprintf(buffer, size, "%.4g elem/s",
             (double)processed * 1000.0 / milliseconds);
}

static void format_factor(char *buffer, size_t size, double value)
{
    unsigned long long scaled;

    if (value < 0.0) value = 0.0;
    if (value > 999999.99) value = 999999.99;
    scaled = (unsigned long long)(value * 100.0 + 0.5);
    snprintf(buffer, size, "%llu.%02llux", scaled / 100, scaled % 100);
}

static void add_line(Lesson11Line *lines, int *count, const char *text, int color)
{
    if (*count < LESSON_11_MAX_LINES)
    {
        snprintf(lines[*count].text, sizeof(lines[*count].text), "%s", text);
        lines[*count].color = color;
        (*count)++;
    }
}

static void add_wrapped(Lesson11Line *lines, int *count, const char *prefix, const char *text, int color)
{
    char line[LESSON_11_LINE_WIDTH + 1];
    const char *word = text;
    size_t length;
    size_t word_length;
    int first_word = 1;

    snprintf(line, sizeof(line), "%s", prefix);
    length = strlen(line);
    while (*word != '\0')
    {
        while (*word == ' ') word++;
        if (*word == '\0') break;
        word_length = 0;
        while (word[word_length] != '\0' && word[word_length] != ' ') word_length++;
        if (!first_word && length + word_length + 1 > LESSON_11_LINE_WIDTH)
        {
            add_line(lines, count, line, color);
            snprintf(line, sizeof(line), "    ");
            length = 4;
            first_word = 1;
        }
        if (!first_word) line[length++] = ' ';
        if (word_length > LESSON_11_LINE_WIDTH - length)
            word_length = LESSON_11_LINE_WIDTH - length;
        memcpy(line + length, word, word_length);
        length += word_length;
        line[length] = '\0';
        first_word = 0;
        word += word_length;
    }
    if (length > 0) add_line(lines, count, line, color);
}

static void refresh_lines(const Lesson11Line *lines, int count, int first_line)
{
    int row;
    int i;

    textbackground(BLACK);
    for (row = 6; row <= 20; row++)
    {
        gotoxy(9, row);
        printf("%-61s", "");
    }
    for (i = 0; i <= 14 && first_line + i < count; i++)
    {
        gotoxy(9, 6 + i);
        textcolor(lines[first_line + i].color);
        printf("%s", lines[first_line + i].text);
    }
    textcolor(WHITE);
    gotoxy(9, 21);
    printf("[Esc] voltar");
    gotoxy(SCREEN_WIDTH, SCREEN_HEIGHT);
}

static void show_progress(size_t current, size_t total, int row)
{
    size_t filled = current * 40 / total;
    size_t i;

    gotoxy(11, row);
    textcolor(LIGHTGREEN);
    printf("[");
    for (i = 0; i < 40; i++)
    {
        printf("%s", i < filled ? CHAR_DOT : " ");
    }
    printf("] %3lu%%", (unsigned long)(current * 100 / total));
    fflush(stdout);
    gotoxy(SCREEN_WIDTH, SCREEN_HEIGHT);
}

static int comparison_progress_active;
static size_t comparison_progress_completed;
static size_t comparison_progress_total;
static LARGE_INTEGER comparison_progress_start;
static double comparison_progress_fraction;
static double comparison_progress_last_sample_fraction;
static double comparison_progress_last_sample_ms;
static double comparison_progress_smoothed_rate;

void lesson11_progress(const char *phase, size_t current, size_t total)
{
    gotoxy(11, 16);
    textbackground(BLACK);
    textcolor(YELLOW);
    printf("%-61s", phase);
    show_progress(current, total == 0 ? 1 : total, 17);
}

void lesson11_algorithm_progress(const char *phase, size_t current, size_t total)
{
    double fraction;
    double elapsed;
    double remaining;
    double interval_seconds;
    double interval_rate;
    char eta[32];
    char elapsed_text[32];
    char status[LESSON_11_LINE_WIDTH + 1];

    if (!comparison_progress_active) return;
    fraction = total == 0 ? 0.0 : (double)current / (double)total;
    if (fraction < 0.0) fraction = 0.0;
    if (fraction > 1.0) fraction = 1.0;
    fraction = ((double)comparison_progress_completed + fraction) /
               (double)comparison_progress_total;
    if (fraction < comparison_progress_fraction)
        fraction = comparison_progress_fraction;
    comparison_progress_fraction = fraction;
    elapsed = elapsed_milliseconds(comparison_progress_start);
    if (fraction >= 1.0)
    {
        remaining = 0.0;
        format_duration(eta, sizeof(eta), remaining);
    }
    else if (fraction > comparison_progress_last_sample_fraction &&
             elapsed > comparison_progress_last_sample_ms)
    {
        interval_seconds =
            (elapsed - comparison_progress_last_sample_ms) / 1000.0;
        interval_rate =
            (fraction - comparison_progress_last_sample_fraction) / interval_seconds;
        comparison_progress_smoothed_rate =
            comparison_progress_smoothed_rate == 0.0
                ? interval_rate
                : comparison_progress_smoothed_rate * 0.75 + interval_rate * 0.25;
        comparison_progress_last_sample_fraction = fraction;
        comparison_progress_last_sample_ms = elapsed;
        remaining = (1.0 - fraction) / comparison_progress_smoothed_rate;
        format_duration(eta, sizeof(eta), remaining * 1000.0);
    }
    else if (comparison_progress_smoothed_rate > 0.0)
    {
        remaining = (1.0 - fraction) / comparison_progress_smoothed_rate;
        format_duration(eta, sizeof(eta), remaining * 1000.0);
    }
    else
    {
        snprintf(eta, sizeof(eta), "calculando...");
    }
    format_duration(elapsed_text, sizeof(elapsed_text), elapsed);
    gotoxy(11, 15);
    textbackground(BLACK);
    textcolor(YELLOW);
    printf("%-61s", phase);
    snprintf(status, sizeof(status), "Decorrido: %.12s · Restante estimado: %.12s",
             elapsed_text, eta);
    gotoxy(11, 16);
    printf("%-61s", status);
    show_progress((size_t)(fraction * comparison_progress_total),
                  comparison_progress_total, 17);
}

static int write_sorted_output(
    const Lesson11AlgorithmInfo *info,
    size_t count,
    const int *values,
    double milliseconds,
    char *path,
    size_t path_size)
{
    char directory[128];
    char filename[64];
    FILE *file;
    size_t i;

    if (info->output_directory == NULL) return 1;
    snprintf(directory, sizeof(directory), "%s/%s", LESSON_11_DATA_DIR, info->output_directory);
    _mkdir(directory);
    if (milliseconds < 1.0)
    {
        snprintf(filename, sizeof(filename), "%lu_%.0fus.txt",
                 (unsigned long)count, milliseconds * 1000.0);
    }
    else if (milliseconds < 1000.0)
    {
        snprintf(filename, sizeof(filename), "%lu_%.0fms.txt",
                 (unsigned long)count, milliseconds);
    }
    else
    {
        snprintf(filename, sizeof(filename), "%lu_%.0fs.txt",
                 (unsigned long)count, milliseconds / 1000.0);
    }
    snprintf(path, path_size, "%s/%s", directory, filename);
    file = fopen(path, "w");
    if (file == NULL) return 0;
    for (i = 0; i < count; i++) fprintf(file, "%d\n", values[i]);
    fclose(file);
    return 1;
}

static void data_path(char *path, size_t size, size_t count);

static const char *generation_options[] = {
    "Gerar 100 números aleatórios",
    "Gerar 1.000 números aleatórios",
    "Gerar 10.000 números aleatórios",
    "Gerar 100.000 números aleatórios",
    "Gerar Personalizado",
    "Voltar"
};

static void draw_generation_item(int index, int selected)
{
    gotoxy(14, 8 + index);
    if (selected)
    {
        textbackground((index % 3 == 0) ? RED : (index % 3 == 1) ? BLUE : YELLOW);
        textcolor(WHITE);
    }
    else
    {
        textbackground(BLACK);
        textcolor(WHITE);
    }
    printf("· %-38s", generation_options[index]);
}

static void draw_generation_menu(int selected)
{
    int i;

    draw_problem_screen(11, "    AULA 11 - GERAR ARQUIVOS");
    for (i = 0; i < 6; i++) draw_generation_item(i, i == selected);
    textbackground(BLACK);
    textcolor(YELLOW);
    gotoxy(14, 15);
    printf("[↑] [↓] navegar   [Enter] selecionar   [Esc] voltar");
    gotoxy(SCREEN_WIDTH, SCREEN_HEIGHT);
}

static int read_custom_count(size_t *count)
{
    char input[16] = "";
    size_t length = 0;
    int key;

    draw_problem_screen(11, "    AULA 11 - GERAR PERSONALIZADO");
    gotoxy(12, 8);
    textcolor(YELLOW);
    printf("Digite a quantidade de números (0 a 1.000.000):");
    gotoxy(12, 10);
    textcolor(LIGHTCYAN);
    printf("Quantidade: ");
    fflush(stdout);

    for (;;)
    {
        key = getch();
        if (key == 27) return 0;
        if (key == 8 || key == 127)
        {
            if (length > 0)
            {
                length--;
                input[length] = '\0';
                gotoxy(24 + (int)length, 10);
                printf(" ");
                gotoxy(24, 10);
                printf("%s", input);
                fflush(stdout);
            }
            continue;
        }
        if (key >= '0' && key <= '9')
        {
            if (length < sizeof(input) - 1)
            {
                input[length++] = (char)key;
                input[length] = '\0';
                gotoxy(24, 10);
                printf("%s", input);
                fflush(stdout);
            }
            continue;
        }
        if (key == 10)
        {
            unsigned long value = 0;
            size_t i;
            int valid = length > 0;

            for (i = 0; i < length; i++)
            {
                unsigned long digit = (unsigned long)(input[i] - '0');
                if (value > (1000000UL - digit) / 10UL)
                {
                    valid = 0;
                }
                else
                {
                    value = value * 10UL + digit;
                }
            }
            if (valid)
            {
                *count = (size_t)value;
                return 1;
            }
            gotoxy(12, 12);
            textcolor(LIGHTRED);
            printf("Valor inválido. Use um número entre 0 e 1.000.000.");
            fflush(stdout);
        }
    }
}

static void generate_dataset(size_t count)
{
    char path[128];
    FILE *file;
    LARGE_INTEGER start;
    size_t current;
    unsigned int seed;
    char text[128];

    draw_problem_screen(11, "    AULA 11 - GERAR ARQUIVO");
    data_path(path, sizeof(path), count);
    gotoxy(11, 7);
    textcolor(YELLOW);
    printf("Gerando %s (%lu números)", path, (unsigned long)count);
    show_progress(0, count == 0 ? 1 : count, 8);

    QueryPerformanceCounter(&start);
    _mkdir("problems/lesson11/data");
    file = fopen(path, "w");
    if (file == NULL)
    {
        gotoxy(11, 10);
        textcolor(LIGHTRED);
        printf("Erro: não foi possível criar o arquivo.");
        waitEsc();
        return;
    }

    seed = (unsigned int)(count * 7919U);
    for (current = 0; current < count; current++)
    {
        seed = seed * 1664525U + 1013904223U;
        fprintf(file, "%d%c", (int)(seed % 100000U), current + 1 == count ? '\n' : ' ');
        if (current % (count / 40 + 1) == 0 || current + 1 == count)
        {
            show_progress(current + 1, count, 8);
            delay(12);
        }
    }
    fclose(file);
    if (count == 0) show_progress(1, 1, 8);

    {
        char duration[32];
        format_duration(duration, sizeof(duration), elapsed_milliseconds(start));
        snprintf(text, sizeof(text), "Concluído em %s", duration);
    }
    gotoxy(11, 10);
    textcolor(LIGHTGREEN);
    printf("%s", text);
    gotoxy(11, 12);
    textcolor(WHITE);
    printf("Arquivo salvo em: %s", path);
    waitEsc();
}

static void data_path(char *path, size_t size, size_t count)
{
    snprintf(path, size, "%s/inteiros_%lu.txt", LESSON_11_DATA_DIR, (unsigned long)count);
}

static int read_values(size_t count, int **values)
{
    char path[128];
    FILE *file;
    size_t i;

    data_path(path, sizeof(path), count);
    file = fopen(path, "r");
    if (file == NULL) return 0;
    *values = (int *)malloc(count * sizeof(**values));
    if (*values == NULL)
    {
        fclose(file);
        return 0;
    }
    for (i = 0; i < count; i++)
    {
        if (fscanf(file, "%d", &(*values)[i]) != 1)
        {
            free(*values);
            *values = NULL;
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

void problem_110_generate_files(void)
{
    int selected = 0;
    int previous;
    KEY key;

    draw_generation_menu(selected);
    for (;;)
    {
        key = getKeypressed();
        if (key.x == 27 && key.y == 91 && key.z == 65)
        {
            previous = selected;
            selected = selected == 0 ? 5 : selected - 1;
            draw_generation_item(previous, 0);
            draw_generation_item(selected, 1);
        }
        else if (key.x == 27 && key.y == 91 && key.z == 66)
        {
            previous = selected;
            selected = selected == 5 ? 0 : selected + 1;
            draw_generation_item(previous, 0);
            draw_generation_item(selected, 1);
        }
        else if (key.x == 10)
        {
            if (selected == 0) generate_dataset(100);
            else if (selected == 1) generate_dataset(1000);
            else if (selected == 2) generate_dataset(10000);
            else if (selected == 3)
            {
                generate_dataset(100000);
            }
            else if (selected == 4)
            {
                size_t count;
                if (read_custom_count(&count)) generate_dataset(count);
            }
            else
            {
                clear_lesson11_screen();
                return;
            }
                clear_lesson11_screen();
            draw_generation_menu(selected);
        }
        else if (key.x == 27)
        {
                clear_lesson11_screen();
            return;
        }
    }
}

static unsigned long long theoretical_comparisons(const Lesson11AlgorithmInfo *info, size_t count)
{
    unsigned long long n = (unsigned long long)count;
    unsigned long long result = 0;
    size_t range = count;

    if (strcmp(info->name, "Busca Sequencial") == 0) return n;
    if (strcmp(info->name, "Busca Binária") == 0)
    {
        while (range > 0)
        {
            result++;
            range /= 2;
        }
        return result;
    }
    if (strcmp(info->name, "Bubble Sort") == 0 ||
        strcmp(info->name, "Selection Sort") == 0 ||
        strcmp(info->name, "Ordenação por Inserção") == 0)
    {
        return n * (n - 1) / 2;
    }
    if (strcmp(info->name, "Quick Sort") == 0)
    {
        return n * (n - 1) / 2;
    }
    if (strcmp(info->name, "Radix Sort") == 0)
    {
        return 5 * (n + 10);
    }
    {
        unsigned long long levels = 0;
        size_t value = count;
        while (value > 1)
        {
            levels++;
            value = (value + 1) / 2;
        }
        return n * levels;
    }
}

        static void show_comparison_lines(const Lesson11Line *lines, int count)
        {
            int first_line = 0;
            int maximum = count > 15 ? count - 15 : 0;
            KEY key;

            refresh_lines(lines, count, first_line);
            for (;;)
            {
                key = getKeypressed();
                if (key.x == 27 && key.y == 91 && key.z == 65)
                {
                    if (first_line > 0) refresh_lines(lines, count, --first_line);
                }
                else if (key.x == 27 && key.y == 91 && key.z == 66)
                {
                    if (first_line < maximum) refresh_lines(lines, count, ++first_line);
                }
                else if (key.x == 27)
                {
                    return;
                }
            }
        }

        void problem_110_compare(void)
        {
            static const size_t sizes[] = { 100, 1000, 10000, 100000 };
            static const Lesson11AlgorithmDefinition *algorithms[] = {
                NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
            };
            Lesson11Line lines[LESSON_11_MAX_LINES];
            double times[9][4] = { { 0.0 } };
            unsigned long long theoretical[9][4] = { { 0 } };
            int projected[9][4] = { { 0 } };
            int available[4] = { 0, 0, 0, 0 };
            int line_count = 0;
            size_t algorithm_index;
            size_t size_index;
            char text[LESSON_11_LINE_WIDTH + 1];

            algorithms[0] = sequential_search_definition();
            algorithms[1] = binary_search_definition();
            algorithms[2] = bubble_sort_definition();
            algorithms[3] = selection_sort_definition();
            algorithms[4] = insertion_sort_definition();
            algorithms[5] = merge_sort_definition();
            algorithms[6] = quick_sort_definition();
            algorithms[7] = radix_sort_definition();
            algorithms[8] = heap_sort_definition();

            draw_problem_screen(11, "    AULA 11 - COMPARATIVO");
            comparison_progress_active = 1;
            comparison_progress_completed = 0;
            comparison_progress_total = 4 * 9;
            comparison_progress_fraction = 0.0;
            comparison_progress_last_sample_fraction = 0.0;
            comparison_progress_last_sample_ms = 0.0;
            comparison_progress_smoothed_rate = 0.0;
            QueryPerformanceCounter(&comparison_progress_start);
            for (size_index = 0; size_index < 4; size_index++)
            {
                int *original;

                lesson11_progress("Lendo arquivo comparativo...", 0, sizes[size_index]);
                if (!read_values(sizes[size_index], &original))
                {
                    snprintf(text, sizeof(text), "%lu: arquivo ausente; gere os arquivos.",
                             (unsigned long)sizes[size_index]);
                    add_line(lines, &line_count, text, LIGHTRED);
                    comparison_progress_completed += 9;
                    lesson11_algorithm_progress("Arquivos ausentes.", 0, 1);
                    continue;
                }
                available[size_index] = 1;
                for (algorithm_index = 0; algorithm_index < 9; algorithm_index++)
                {
                    int *values;
                    Lesson11Metrics metrics = { 0, 0, 0.0, 0 };
                    Lesson11Metrics preparation_metrics = { 0, 0, 0.0, 0 };
                    LARGE_INTEGER start;
                    char output_path[192];

                    values = (int *)malloc(sizes[size_index] * sizeof(*values));
                    if (values == NULL)
                    {
                        comparison_progress_completed++;
                        lesson11_algorithm_progress("Memória insuficiente.", 1, 1);
                        continue;
                    }
                    memcpy(values, original, sizes[size_index] * sizeof(*values));
                    snprintf(text, sizeof(text), "%s · %lu", algorithms[algorithm_index]->info.name,
                             (unsigned long)sizes[size_index]);
                    lesson11_algorithm_progress(text, 0, 1);
                    if (size_index == 3 && algorithm_index >= 2 && algorithm_index <= 4)
                    {
                        theoretical[algorithm_index][size_index] =
                            theoretical_comparisons(&algorithms[algorithm_index]->info, sizes[size_index]);
                        if (times[algorithm_index][2] > 0.0 &&
                            theoretical[algorithm_index][2] > 0)
                        {
                            times[algorithm_index][size_index] =
                                times[algorithm_index][2] *
                                (double)theoretical[algorithm_index][size_index] /
                                (double)theoretical[algorithm_index][2];
                        }
                        projected[algorithm_index][size_index] = 1;
                        comparison_progress_completed++;
                        lesson11_algorithm_progress("Projeção quadrática concluída.", 1, 1);
                        free(values);
                        continue;
                    }
                    if (algorithms[algorithm_index]->preparation != NULL)
                    {
                        algorithms[algorithm_index]->preparation(
                            values, sizes[size_index], values[sizes[size_index] - 1],
                            &preparation_metrics);
                    }
                    comparison_progress_active = 0;
                    QueryPerformanceCounter(&start);
                    algorithms[algorithm_index]->algorithm(
                        values, sizes[size_index], values[sizes[size_index] - 1], &metrics);
                    metrics.milliseconds = elapsed_milliseconds(start);
                    comparison_progress_active = 1;
                    comparison_progress_completed++;
                    lesson11_algorithm_progress("Algoritmo concluído.", 1, 1);
                    times[algorithm_index][size_index] = metrics.milliseconds;
                    theoretical[algorithm_index][size_index] =
                        theoretical_comparisons(&algorithms[algorithm_index]->info, sizes[size_index]);
                    if (!algorithms[algorithm_index]->info.is_search)
                    {
                        write_sorted_output(
                            &algorithms[algorithm_index]->info,
                            sizes[size_index],
                            values,
                            metrics.milliseconds,
                            output_path,
                            sizeof(output_path));
                    }
                    free(values);
                }
                lesson11_progress("Caso concluído.", sizes[size_index], sizes[size_index]);
                free(original);
            }
            comparison_progress_active = 0;

            add_line(lines, &line_count, "BUSCAS", RED);
            add_line(lines, &line_count, " n       t. prático  t. teórico  x prát. x teór. ader.", YELLOW);
            for (algorithm_index = 0; algorithm_index < 2; algorithm_index++)
            {
                add_line(lines, &line_count, algorithms[algorithm_index]->info.name,
                         algorithm_index % 3 == 0 ? RED : algorithm_index % 3 == 1 ? BLUE : YELLOW);
                for (size_index = 0; size_index < 4; size_index++)
                {
                    double estimated = 0.0;
                    double practical_growth = 0.0;
                    double theoretical_growth = 0.0;
                    double ratio = 0.0;
                    char practical[32];
                    char estimate[32];
                    char practical_factor[24];
                    char theoretical_factor[24];
                    char ratio_factor[24];
                    char marker[2] = "";

                    if (!available[size_index]) continue;
                    if (times[algorithm_index][0] > 0.0)
                    {
                        estimated = times[algorithm_index][0] *
                            (double)theoretical[algorithm_index][size_index] /
                            (double)theoretical[algorithm_index][0];
                        practical_growth = times[algorithm_index][size_index] /
                            times[algorithm_index][0];
                        theoretical_growth = (double)theoretical[algorithm_index][size_index] /
                            (double)theoretical[algorithm_index][0];
                        ratio = theoretical_growth > 0.0 ?
                            practical_growth / theoretical_growth : 0.0;
                    }
                    format_duration(practical, sizeof(practical), times[algorithm_index][size_index]);
                    format_duration(estimate, sizeof(estimate), estimated);
                    format_factor(practical_factor, sizeof(practical_factor), practical_growth);
                    format_factor(theoretical_factor, sizeof(theoretical_factor), theoretical_growth);
                    format_factor(ratio_factor, sizeof(ratio_factor), ratio);
                    if (projected[algorithm_index][size_index]) marker[0] = '~';
                    snprintf(text, sizeof(text), " %s%4lu %10.12s %10.12s %7.7s %7.7s %9.9s",
                             marker, (unsigned long)sizes[size_index], practical, estimate,
                             practical_factor, theoretical_factor, ratio_factor);
                    add_line(lines, &line_count, text,
                             size_index % 2 == 0 ? LIGHTGREEN : WHITE);
                }
            }

            add_line(lines, &line_count, "", WHITE);
            add_line(lines, &line_count, "ORDENAÇÕES", BLUE);
            add_line(lines, &line_count, " n       t. prático  t. teórico  x prát. x teór. ader.", YELLOW);
            for (algorithm_index = 2; algorithm_index < 9; algorithm_index++)
            {
                add_line(lines, &line_count, algorithms[algorithm_index]->info.name,
                         algorithm_index % 3 == 0 ? RED : algorithm_index % 3 == 1 ? BLUE : YELLOW);
                for (size_index = 0; size_index < 4; size_index++)
                {
                    double estimated = 0.0;
                    double practical_growth = 0.0;
                    double theoretical_growth = 0.0;
                    double ratio = 0.0;
                    char practical[32];
                    char estimate[32];
                    char practical_factor[24];
                    char theoretical_factor[24];
                    char ratio_factor[24];
                    char marker[2] = "";

                    if (!available[size_index]) continue;
                    if (times[algorithm_index][0] > 0.0)
                    {
                        estimated = times[algorithm_index][0] *
                            (double)theoretical[algorithm_index][size_index] /
                            (double)theoretical[algorithm_index][0];
                        practical_growth = times[algorithm_index][size_index] /
                            times[algorithm_index][0];
                        theoretical_growth = (double)theoretical[algorithm_index][size_index] /
                            (double)theoretical[algorithm_index][0];
                        ratio = theoretical_growth > 0.0 ?
                            practical_growth / theoretical_growth : 0.0;
                    }
                    format_duration(practical, sizeof(practical), times[algorithm_index][size_index]);
                    format_duration(estimate, sizeof(estimate), estimated);
                    format_factor(practical_factor, sizeof(practical_factor), practical_growth);
                    format_factor(theoretical_factor, sizeof(theoretical_factor), theoretical_growth);
                    format_factor(ratio_factor, sizeof(ratio_factor), ratio);
                    if (projected[algorithm_index][size_index]) marker[0] = '~';
                    snprintf(text, sizeof(text), " %s%4lu %10.12s %10.12s %7.7s %7.7s %9.9s",
                             marker, (unsigned long)sizes[size_index], practical, estimate,
                             practical_factor, theoretical_factor, ratio_factor);
                    add_line(lines, &line_count, text,
                             size_index % 2 == 0 ? LIGHTGREEN : WHITE);
                }
            }
            add_line(lines, &line_count, "", WHITE);
            add_line(lines, &line_count, "x prático: crescimento medido desde n = 100.", LIGHTBLUE);
            add_line(lines, &line_count, "x teórico: crescimento previsto pela fórmula do algoritmo.", LIGHTBLUE);
            add_line(lines, &line_count, "prát./teór.: aderência do crescimento observado ao previsto.", LIGHTBLUE);
            add_line(lines, &line_count, "~ indica projeção teórica para custo quadrático em n = 100.000.", YELLOW);
            add_line(lines, &line_count, "", WHITE);
            add_line(lines, &line_count, "VELOCIDADE DAS ORDENAÇÕES (elementos por segundo)", BLUE);
            for (algorithm_index = 2; algorithm_index < 9; algorithm_index++)
            {
                add_line(lines, &line_count, algorithms[algorithm_index]->info.name,
                         algorithm_index % 3 == 0 ? RED : algorithm_index % 3 == 1 ? BLUE : YELLOW);
                for (size_index = 0; size_index < 4; size_index++)
                {
                    char speed[32];

                    if (!available[size_index]) continue;
                    format_processing_rate(
                        speed,
                        sizeof(speed),
                        sizes[size_index],
                        times[algorithm_index][size_index]);
                    snprintf(
                        text,
                        sizeof(text),
                        "%s%lu: %s",
                        projected[algorithm_index][size_index] ? "~" : "",
                        (unsigned long)sizes[size_index],
                        speed);
                    add_line(lines, &line_count, text,
                             size_index % 2 == 0 ? LIGHTGREEN : WHITE);
                }
            }
            show_comparison_lines(lines, line_count);
        }
void lesson11_run_algorithm(
    const Lesson11AlgorithmInfo *info,
    Lesson11Algorithm preparation,
    Lesson11Algorithm algorithm)
{
    static const size_t sizes[] = { 100, 1000, 10000 };
    Lesson11Line lines[LESSON_11_MAX_LINES];
    char text[128];
    char duration[32];
    char theoretical_duration[32];
    char processing_rate[32];
    double times[3] = { 0.0, 0.0, 0.0 };
    unsigned long long practical_comparisons[3] = { 0, 0, 0 };
    unsigned long long practical_movements[3] = { 0, 0, 0 };
    unsigned long long theoretical_operations[3] = { 0, 0, 0 };
    int available[3] = { 0, 0, 0 };
    int line_count = 0;
    size_t i;

    draw_problem_screen(11, info->name);
    add_wrapped(lines, &line_count, "Algoritmo: ", info->name, LIGHTCYAN);
    add_wrapped(lines, &line_count, "Complexidade: ", info->complexity, YELLOW);
    add_wrapped(lines, &line_count, "Fórmula: ", info->formula, LIGHTBLUE);
    add_wrapped(lines, &line_count, "Memória: ", info->memory, LIGHTMAGENTA);
    if (!info->is_search)
    {
        snprintf(text, sizeof(text), "Saída: problems/lesson11/%s/", info->output_directory);
        add_line(lines, &line_count, text, LIGHTCYAN);
    }
    add_line(lines, &line_count, "Desempenho:", WHITE);
    for (i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
    {
        int *values;
        Lesson11Metrics metrics = { 0, 0, 0.0, 0 };
        Lesson11Metrics preparation_metrics = { 0, 0, 0.0, 0 };
        LARGE_INTEGER start;
        int target;
        char output_path[192];

        lesson11_progress("Lendo arquivo...", 0, sizes[i]);
        if (!read_values(sizes[i], &values))
        {
            snprintf(text, sizeof(text), "%lu: arquivo ausente; gere os arquivos.",
                     (unsigned long)sizes[i]);
            add_line(lines, &line_count, text, LIGHTRED);
            continue;
        }
        lesson11_progress("Arquivo carregado.", sizes[i], sizes[i]);
        target = values[sizes[i] - 1];
        if (preparation != NULL)
        {
            lesson11_progress("Preparando busca...", sizes[i] / 4, sizes[i]);
            preparation(values, sizes[i], target, &preparation_metrics);
        }
        lesson11_progress("Executando algoritmo...", sizes[i] / 3, sizes[i]);
        QueryPerformanceCounter(&start);
        algorithm(values, sizes[i], target, &metrics);
        metrics.milliseconds = elapsed_milliseconds(start);
        lesson11_progress("Algoritmo concluído.", sizes[i] * 4 / 5, sizes[i]);
        format_duration(duration, sizeof(duration), metrics.milliseconds);
        times[i] = metrics.milliseconds;
        practical_comparisons[i] = metrics.comparisons;
        practical_movements[i] = metrics.movements;
        theoretical_operations[i] = theoretical_comparisons(info, sizes[i]);
        available[i] = 1;
        if (!info->is_search)
        {
            lesson11_progress("Salvando arquivo ordenado...", sizes[i] * 9 / 10, sizes[i]);
            if (!write_sorted_output(
                    info,
                    sizes[i],
                    values,
                    metrics.milliseconds,
                    output_path,
                    sizeof(output_path)))
            {
                snprintf(text, sizeof(text), "%lu: erro ao salvar o resultado.",
                         (unsigned long)sizes[i]);
                add_line(lines, &line_count, text, LIGHTRED);
            }
        }
        lesson11_progress("Concluído.", sizes[i], sizes[i]);
        free(values);
    }

    add_line(lines, &line_count, " n      t. prático   t. teórico   velocidade", YELLOW);
    for (i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
    {
        double estimated_time = 0.0;

        if (!available[i]) continue;
        if (available[0] && theoretical_operations[0] > 0)
        {
            estimated_time = times[0] *
                (double)theoretical_operations[i] /
                (double)theoretical_operations[0];
        }
        format_duration(duration, sizeof(duration), times[i]);
        format_processing_rate(
            processing_rate,
            sizeof(processing_rate),
            sizes[i],
            times[i]);
        if (estimated_time > 0.0)
        {
            format_duration(theoretical_duration, sizeof(theoretical_duration), estimated_time);
        }
        else
        {
            snprintf(theoretical_duration, sizeof(theoretical_duration), "N/A");
        }
        snprintf(
            text,
            sizeof(text),
            "%5lu %12s %12s %18s",
            (unsigned long)sizes[i],
            duration,
            theoretical_duration,
            processing_rate
        );
        add_line(lines, &line_count, text, LIGHTGREEN);
    }

    add_line(lines, &line_count, "", WHITE);
    if (info->is_search)
    {
        add_line(lines, &line_count, "Operações realizadas:", WHITE);
        snprintf(text, sizeof(text), " n       %s", info->operation_label);
        add_line(lines, &line_count, text, YELLOW);
        snprintf(
            text,
            sizeof(text),
            "%5lu %16llu",
            (unsigned long)sizes[0],
            practical_comparisons[0]
        );
        add_line(lines, &line_count, text, WHITE);
        for (i = 1; i < sizeof(sizes) / sizeof(sizes[0]); i++)
        {
            snprintf(text, sizeof(text), "%5lu %16llu",
                     (unsigned long)sizes[i], practical_comparisons[i]);
            add_line(lines, &line_count, text, WHITE);
        }
    }
    else if (info->operation_label != NULL)
    {
        add_line(lines, &line_count, "Operações realizadas:", WHITE);
        snprintf(text, sizeof(text), " n       %-13s trocas",
                 info->operation_label);
        add_line(lines, &line_count, text, YELLOW);
        for (i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
        {
            snprintf(text, sizeof(text), "%5lu %16llu %16llu",
                     (unsigned long)sizes[i],
                     practical_comparisons[i],
                     practical_movements[i]);
            add_line(lines, &line_count, text, WHITE);
        }
    }
    else
    {
        add_line(lines, &line_count,
                 "Contagem de comparações/trocas omitida para Radix Sort.",
                 LIGHTBLUE);
    }
    refresh_lines(lines, line_count, 0);
    waitEsc();
}
