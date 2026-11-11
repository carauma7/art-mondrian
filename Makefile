CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g3
SOURCES = graphics.c utils.c sound.c eastereggs/cobra/cobra.c \
	eastereggs/devcalc/diff.c eastereggs/devcalc/error.c \
	eastereggs/devcalc/parse.c eastereggs/devcalc/simplify.c \
	eastereggs/devcalc/struct.c eastereggs/devcalc/utility.c \
	eastereggs/devcalc/devcalc.c problems.c \
	problems/problem_0.c problems/lesson01/problem_1.c problems/lesson02/problem_2.c \
	problems/lesson02/problem_3.c problems/lesson03/problem_4.c problems/lesson03/problem_5.c \
	problems/lesson03/problem_6.c problems/lesson04/problem_7.c \
	problems/lesson04/problem_71.c problems/lesson04/problem_72.c problems/lesson04/problem_73.c \
	problems/lesson04/problem_74.c problems/lesson04/problem_75.c problems/lesson04/problem_76.c \
	problems/lesson04/problem_77.c problems/lesson08/problem_80.c main.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(subst /,\,$(OBJECTS)) $(TARGET)